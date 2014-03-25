/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef __linux__
#include <unistd.h>
#endif

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_profiles.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_pas_lib.h"
#include "csr_bt_pas_prim.h"
#include "csr_bt_obex.h"
#include "csr_bt_platform.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_sched_init.h"
#include "csr_unicode.h"
#include "csr_bt_demoapp.h"

#define KEYB_CHECK_TIMEOUT              (100000)
#define MAX_DISCOVERY_RESULTS_IN_LIST   16
#define PAS_ROOT_PATH                   "root"
#define PAS_PHONE_PATH                  "telecom"
#define PAS_SIM_PATH                    "SIM1/telecom"
#define PAS_PHONE_BACK_PATH             ".."
#define PAS_SIM_BACK_PATH               "../.."
#define PAS_BODYHEADER_SIZE             (CsrUint16)(6)
#define TMPFILE                         "pas.tmp"
#define BEGIN_VCARD_STR                 "BEGIN:VCARD"
#define VCARD_FILE_OPEN_ERROR           0xffff

#define PB_LOCATION_PHONE               0
#define PB_LOCATION_SIM                 1

typedef CsrUint8 state;
#define IDLE_STATE (state) 0x00
#define ACTIVATION_PENDING_STATE (state) 0x01
#define ACTIVE_STATE (state) 0x02
#define AUTHENTICATING_STATE (state) 0x03
#define AUTHENTICATED_STATE (state) 0x04
#define DEACTIVATION_PENDING_STATE (state) 0x05
#define LAST_STATE (state) 0x06

#define AUTH_LOCAL_INITIATED_STATE      ((CsrUint8) 0x00)
#define AUTH_REMOTE_INITIATED_STATE     ((CsrUint8) 0x01)

typedef CsrUint8 keyState;
#define IDLE_KEYBOARD_STATE (keyState) 0x00
#define PASSKEY_KEYBOARD_STATE (keyState) 0x01
#define SECURITY_KEYBOARD_STATE (keyState) 0x02
#define OBEX_PASSKEY_KEYBOARD_STATE (keyState) 0x03
#define ACTIVATE_OBEX_PASSKEY_KEYBOARD_STATE (keyState) 0x04
#define KEYS_LAST_STATE (keyState) 0x05

typedef CsrUint32 appTarget;
#define APP_NONE_TARGET                 (appTarget)(0)
#define APP_PULL_PB_TARGET              (appTarget)(1)
#define APP_SET_FOLDER_TARGET           (appTarget)(2)
#define APP_PULL_LISTING_TARGET         (appTarget)(3)
#define APP_PULL_ENTRY_TARGET           (appTarget)(4)

typedef struct
{
    CsrSchedQid           CsrSchedQid;

    CsrUint16            event;
    void                *pRecvMsg;
    state               state;

    CsrUint8             authInitiator;
    CsrBool              doAuth;
    CsrBool              authenticated;
    CsrUint8             nPasskeyLen;
    CsrUint8             szPasskey[CSR_BT_PASSKEY_MAX_LEN];

    CsrUint32            connectionId;
    CsrUint16            obexPeerMaxPacketSize;
    CsrBtDeviceAddr        clientDevAddr;

    char                path[CSR_BT_MAX_PATH_LENGTH];

    keyState            keyboardState;
    keyState            prevKeyboardState;

    appTarget           target;
    CsrUint16            nPbSize;
    CsrUint8             nNewMissedCall;
    CsrFileHandle                *pFile;

    char                currentPath[CSR_BT_MAX_PATH_LENGTH];
    char                rootPath[CSR_BT_MAX_PATH_LENGTH];
    CsrBool              isMch;
    ScAppInstData_t     scInstData;
} appInstData;


#define ROOT "root"
#define LIST "folder.vcf"
#define TEMPDIR "/tempdir"
#define MCH "mch.vcf"
#define MCHDIR "mch"

static void playIntro(void);
static void keyMessageService(CsrUint16 m, void *data);
static void handleKeybPrim(appInstData *pInst);
static CsrBool idleState(appInstData *pInst, CsrUint8 cKey);

static CsrBool passkeyState(appInstData *pInst, CsrUint8 cKey);

static void handlePasPrim(appInstData *pInst);
static void handleCsrBtPasDeactivateCfm(appInstData *pInst);
static void handleCsrBtPasConnectInd(appInstData *pInst);
static void handlePasAuthCfm(appInstData *pInst);
static void handlePasAuthInd(appInstData *pInst);
static void handleCsrBtPasPullPbInd(appInstData *pInst);
static void handleCsrBtPasPullPbSizeInd(appInstData *pInst);
static void handleCsrBtPasPullMchInd(appInstData *pInst);
static void handleCsrBtPasPullMchSizeInd(appInstData *pInst);
static void handleSetFolderInd(appInstData *pInst);
static void handleSetBackFolderInd(appInstData *pInst);
static void handleSetRootFolderInd(appInstData *pInst);
static void handlePullvCardListSizeInd(appInstData *pInst);
static void handlePullvCardMchListSizeInd(appInstData *pInst);
static void handlePullvCardListIndFile(appInstData *pInst);
static void handlePullvCardMchListIndFile(appInstData *pInst);
static void handlePullvCardEntryInd(appInstData *pInst);
static void handleCsrBtPasNextInd(appInstData *pInst);
static void handleCsrBtPasAbortInd(appInstData *pInst);
static void handleCsrBtPasDisconnectInd(appInstData *pInst);

static void setDir(char *path);
static char *unicode2char(CsrUcs2String *unicode);

static void pbScAppSetFnFini(void *pFiniData)
{
    appInstData *instData = (appInstData *) pFiniData;
    scAppRestoreState((CsrUint8 *) &instData->prevKeyboardState, (CsrUint8 *) &instData->keyboardState);
    system_cls();
    playIntro();
}

static void pbScAppSetFnInit(void *pInitData)
{
    appInstData *instData = (appInstData *) pInitData;
    scAppSaveState((CsrUint8 *) &instData->prevKeyboardState, (CsrUint8 *) &instData->keyboardState, SECURITY_KEYBOARD_STATE);
}

static void* pbScAppSetFnGetMessage(void *pRecvMsgPData)
{
    appInstData *instData = (appInstData *) pRecvMsgPData;
    void *result          = instData->pRecvMsg;
    instData->pRecvMsg    = NULL;
    return result;
}

/* == GLOBAL FUNCTIONS ====================================================== */
/*******************************************************************************
*
* init function called by the scheduler upon initialisation. This function is
* used to boot the demo application by sending a request to bond with the
* headset. Bonding is not mandated according to the profile but is needed if
* encryption of the speech is required.
*
*******************************************************************************/
void CsrBtAppInit(void **gash)
{
    appInstData  *pInst;

    *gash = CsrPmemAlloc(sizeof(appInstData));
    pInst = *gash;
    pInst->state = IDLE_STATE;
    pInst->event = 0;
    pInst->pRecvMsg = NULL;
    pInst->doAuth = FALSE;
    pInst->authenticated = FALSE;
    pInst->connectionId = 0;
    pInst->obexPeerMaxPacketSize = 0;
    pInst->clientDevAddr.nap = 0;
    pInst->clientDevAddr.uap = 0;
    pInst->clientDevAddr.lap = 0;
    pInst->target = APP_NONE_TARGET;
    pInst->nPbSize = 255; /* should be set to the number of phonebook entries */
    pInst->nNewMissedCall = 0; /* should concurrently be update with the currently number of missing calls */
    pInst->pFile = NULL;
    pInst->szPasskey[0] = '\0';
    pInst->nPasskeyLen = 0;
    pInst->keyboardState = IDLE_KEYBOARD_STATE;

    /* Application handle */
    pInst->CsrSchedQid = TESTQUEUE;

    /* SetFolder */
    CsrBtGetCwd(pInst->currentPath, CSR_BT_MAX_PATH_LENGTH);
    CsrStrCat(pInst->currentPath, "/");
    CsrStrCat(pInst->currentPath, ROOT);
    CsrStrCpy(pInst->rootPath, pInst->currentPath);
    pInst->path[0] = '\0';
    if( CsrBtChdir(PAS_ROOT_PATH) )
    {
        printf( "ERROR: Unable to locate server root directory: %s\n", PAS_ROOT_PATH);
        CsrSchedStop();
    }
    else
    {
    pInst->isMch = FALSE;

    printf("\nPhonebook Access Server Demo:\n\n");
    printf("Hit <ESC> to quit program!\n\n");

    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, keyMessageService, 0, NULL);

    /* Security App Configuration */
    CsrBtScActivateReqSend(pInst->CsrSchedQid);
    scAppInitInstanceData(&pInst->scInstData);

    scAppSetFnFini(&pInst->scInstData, pbScAppSetFnFini, pInst);
    scAppSetFnInit(&pInst->scInstData, pbScAppSetFnInit, pInst);
    scAppSetSecLevelSend(&pInst->scInstData, scAppSendProfileSecurityLevelReq, &pInst->scInstData);

    scAppSetFnGetMessage(&pInst->scInstData, pbScAppSetFnGetMessage, pInst);

    scAppSetSecLevelPrim(&pInst->scInstData,
                         0,     /* Index 0. Single profile */
                         "PAS", CSR_BT_PAS_PRIM,
                         CSR_BT_PAS_IFACEQUEUE,
                         CSR_BT_PAS_SECURITY_IN_REQ, SC_APP_SET_SEC_LEVEL_NA);

    playIntro();
    }
}

#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    appInstData *pInst = *gash;

    scAppDeinitInstanceData(&pInst->scInstData);

    /* get a message from the demoapplication message queue. The message is returned in prim
       and the event type in eventType */
    while(CsrSchedMessageGet(&pInst->event, &pInst->pRecvMsg) )
    {
        switch (pInst->event)
        {
            case CSR_BT_PAS_PRIM:
            {
                CsrBtPasFreeUpstreamMessageContents(pInst->event, pInst->pRecvMsg);
                break;
            }
            case CSR_BT_SC_PRIM:
            {
                CsrBtScFreeUpstreamMessageContents(pInst->event, pInst->pRecvMsg);
                break;
            }
            default:
            { /* Just ignore    */
                break;
            }
        }
        CsrPmemFree(pInst->pRecvMsg);
    }
    CsrPmemFree(pInst);
}
#endif

/*******************************************************************************
*
* this is the demo application handler function. All primitives sent to the demo
* application will be received in here. The scheduler ensures that the function
* is activated when a signal is put on the demo application signal queue.
*
*******************************************************************************/
void CsrBtAppHandler(void **gash)
{
    appInstData *pInst = *gash;

    /* get a message from the demoapplication message queue. The
     * message is returned in prim and the event type in eventType
 */
    if(CsrSchedMessageGet(&pInst->event, &pInst->pRecvMsg))
    {
        /* Two event types must be handled. SC for bonding and PAS for
         * the connection and PB related signalling
 */
        switch (pInst->event)
        {
            case CSR_BT_SC_PRIM:
                handleScPrim(&pInst->scInstData);
                break;

            case CSR_BT_PAS_PRIM:
                {
                    CsrPrim *prim = (CsrPrim *) pInst->pRecvMsg;
                    switch(*prim)
                    {
                        case CSR_BT_PAS_SECURITY_IN_CFM:
                            handleScSecLevelPrim(&pInst->scInstData);
                            break;

                        default:
                            handlePasPrim(pInst);
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
            /* free the received signal again. If the signal for some reason
               must be stored in the application the pointer (prim) must be set
               to NULL in order not to free it here */
            CsrPmemFree(pInst->pRecvMsg);
            pInst->pRecvMsg = NULL;
        }
    }
}

/* == LOCAL FUNCTIONS =============================================================================== */
static void playIntro(void)
{
    system_cls();
    printf("\nOBEX Phonebook Access SERVER:\n\n");
    printf("\nProgram options:\n\n");
    printf("    1) Activate Obex Phonebook Access with support for both PB and SIM\n");
    printf("    2) Activate Obex Phonebook Access with support for PB only\n");
    printf("    3) Activate Obex Phonebook Access with OBEX authentication\n");
    printf("    4) Security Controller Configuration\n");
    printf("    x) Deactivate Obex Phonebook Access\n");
    printf("\nHit <ESC> to quit program!\n\n");
}

/******************************  PAS PRIMITIVES  ******************************/

/*******************************************************************************
*
*   function to handle all pas-primitives.
*
*******************************************************************************/
static void handlePasPrim(appInstData *pInst)
{
    CsrBtPasPrim *pMsg = pInst->pRecvMsg;

    switch (*pMsg)
    {
        case CSR_BT_PAS_DEACTIVATE_CFM:
            handleCsrBtPasDeactivateCfm(pInst);
            break;

        case CSR_BT_PAS_CONNECT_IND:
            handleCsrBtPasConnectInd(pInst);
            break;

        case CSR_BT_PAS_AUTHENTICATE_CFM:
            handlePasAuthCfm(pInst);
            break;

        case CSR_BT_PAS_AUTHENTICATE_IND:
            handlePasAuthInd(pInst);
            break;

        case CSR_BT_PAS_PULL_PB_IND:
            handleCsrBtPasPullPbInd(pInst);
            break;

        case CSR_BT_PAS_PULL_PB_SIZE_IND:
            handleCsrBtPasPullPbSizeInd(pInst);
            break;

        case CSR_BT_PAS_PULL_MCH_IND:
            handleCsrBtPasPullMchInd(pInst);
            break;

        case CSR_BT_PAS_PULL_MCH_SIZE_IND:
            handleCsrBtPasPullMchSizeInd(pInst);
            break;

        case CSR_BT_PAS_SET_FOLDER_IND:
            handleSetFolderInd(pInst);
            break;

        case CSR_BT_PAS_SET_BACK_FOLDER_IND:
            handleSetBackFolderInd(pInst);
            break;

        case CSR_BT_PAS_SET_ROOT_FOLDER_IND:
            handleSetRootFolderInd(pInst);
            break;

        case CSR_BT_PAS_PULL_VCARD_LIST_IND:
            handlePullvCardListIndFile(pInst);
            break;

        case CSR_BT_PAS_PULL_VCARD_LIST_SIZE_IND:
            handlePullvCardListSizeInd(pInst);
            break;

        case CSR_BT_PAS_PULL_VCARD_MCH_LIST_IND:
            handlePullvCardMchListIndFile(pInst);
            break;

        case CSR_BT_PAS_PULL_VCARD_MCH_LIST_SIZE_IND:
            handlePullvCardMchListSizeInd(pInst);
            break;

        case CSR_BT_PAS_PULL_VCARD_ENTRY_IND:
            handlePullvCardEntryInd(pInst);
            break;

        case CSR_BT_PAS_NEXT_IND:
            handleCsrBtPasNextInd(pInst);
            break;

        case CSR_BT_PAS_ABORT_IND:
            handleCsrBtPasAbortInd(pInst);
            break;

        case CSR_BT_PAS_DISCONNECT_IND:
            handleCsrBtPasDisconnectInd(pInst);
            break;

        default:
            CsrBtPasFreeUpstreamMessageContents(CSR_BT_PAS_PRIM, pInst->pRecvMsg);
            printf("####### Unhandled pasPrim: %x received\n", *pMsg);
            break;
    }
}

static CsrUint16 pbEntriesCount(CsrUint8 dir, char* pbName)
{
    CsrUint16 size = 0;
    CsrFileHandle *fName = NULL;
    char *c = NULL;
    char testStr[] = "01234567890";
    CsrUint8 strCount = 0;
    CsrBool found = 0;
    CsrInt32 count, fEndPos;

    if (dir == PB_LOCATION_PHONE)
    {
        CsrBtChdir(PAS_ROOT_PATH);
        CsrBtChdir(PAS_PHONE_PATH);
    }
    else if (dir == PB_LOCATION_SIM)
    {
        CsrBtChdir(PAS_ROOT_PATH);
        CsrBtChdir(PAS_SIM_PATH);
    }
    else
    {
        printf("Phone book location could not be determined");
    }
    fName = CsrBtFopen(pbName, "r");
    if (fName == NULL)
    {
        size = VCARD_FILE_OPEN_ERROR;
    }
    else
    {
        CsrBtFseek(fName, 0L, SEEK_END);
        fEndPos = CsrBtFtell(fName);
        CsrBtFseek(fName, 0L, SEEK_SET);
        c = CsrPmemAlloc(sizeof(CsrBtChar));

        for (count = 0 ; count < (fEndPos - 1); count++)
        {
           CsrBtFread(c, sizeof(CsrUint8), 1, fName);
           if (*c != '\n' && *c != 0x0d && found == 1)
           {
               testStr[strCount] = *c;
               strCount++;
               if (strCount > CsrStrLen((char*)BEGIN_VCARD_STR) - 1)
               {
                   strCount = 0;
                }
           }
           else if (*c == 'B' && found == 0)
           {
               found = 1;
               testStr[0] = 'B';
               strCount = 1;
           }
           else
           {
                strCount = 0;
                found = 0;
                if (CsrStrCmp(testStr, BEGIN_VCARD_STR) == 0)
                {
                    size++;
                    testStr[0] = '0';
                }
           }
        }
    }
    CsrPmemFree(c);
    if (fName)
    {
        CsrBtFclose(fName);
    }
    if (dir == PB_LOCATION_PHONE)
    {
        CsrBtChdir(".." PATH_SEP);
    }
    else if (dir == PB_LOCATION_SIM)
    {
        CsrBtChdir(".." PATH_SEP "..");
    }
    else
    {
        /* Do nothing */
    }

    return size;
}


static void handleCsrBtPasDeactivateCfm(appInstData *pInst)
{
    if (pInst->pFile)
    {
        CsrBtFclose(pInst->pFile);
        pInst->pFile = NULL;
    }
    pInst->target = APP_NONE_TARGET;
    pInst->state = IDLE_STATE;
    if (0 == CsrBtChdir(ROOT))
    {
        if (0 == CsrBtChdir(pInst->rootPath))
        {
            CsrBtGetCwd(pInst->currentPath, CSR_BT_MAX_PATH_LENGTH);
            pInst->path[0] = '\0';
        }
    }
    printf("Pas Server Deactivated...\n\n");
    playIntro();
}

static void handleCsrBtPasConnectInd(appInstData *pInst)
{
    CsrBtPasConnectInd *pMsg = pInst->pRecvMsg;

    if (0 == CsrBtChdir(pInst->rootPath))
    {
        CsrBtGetCwd(pInst->currentPath, CSR_BT_MAX_PATH_LENGTH);
        pInst->path[0] = '\0';
    }
    else
    {
        printf("Error:  unable to change to root dir\n");
    }

    pInst->connectionId = pMsg->connectionId;
    pInst->obexPeerMaxPacketSize = pMsg->obexPeerMaxPacketSize;
    pInst->clientDevAddr = pMsg->deviceAddr;
    printf("Client: %04X:%02X:%08X request connection.\n",
           pInst->clientDevAddr.nap,
           pInst->clientDevAddr.uap,
           pInst->clientDevAddr.lap);

    if (pInst->doAuth && pInst->state != AUTHENTICATED_STATE)
    {
        if (pInst->nPasskeyLen > 0)
        {
            /* Reuse OBEX passkey if we have one */
            printf("Reuse local OBEX passkey %s\n", pInst->szPasskey);
            CsrBtPasAuthReqSend(0, NULL, (CsrUint16)CsrStrLen((char *) &pInst->szPasskey[0]), (CsrUint8 *)CsrStrDup((char*)pInst->szPasskey), NULL);
        }
        else
        {
            printf("Enter local OBEX authenticate passkey for \"%04X:%02X:%06X\": ",
                   pInst->clientDevAddr.nap,
                   pInst->clientDevAddr.uap,
                   pInst->clientDevAddr.lap);

            pInst->authInitiator = AUTH_LOCAL_INITIATED_STATE;
            pInst->keyboardState = PASSKEY_KEYBOARD_STATE;
            pInst->nPasskeyLen = 0;
            pInst->szPasskey[0] = 0;
    }
    }
    else
    {
        if ('\0' == pInst->path[0])
        {
            char szBuf[CSR_BT_MAX_PATH_LENGTH];

            /* Get the current working directory: */
            CsrBtGetCwd(szBuf, CSR_BT_MAX_PATH_LENGTH);
            if(szBuf == NULL)
            {
                perror("Get current directory error");
            }
            sprintf(pInst->path, "%s"PATH_SEP, szBuf);
        }
        pInst->state = ACTIVE_STATE;
        CsrBtPasConnectResSend(pMsg->connectionId,
                               CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
        printf("Connection Accepted.\n");
    }
}

static void handlePasAuthCfm(appInstData *pInst)
{
    printf("Authentication confirmed.\n");
    pInst->state = AUTHENTICATED_STATE;
}

static void handlePasAuthInd(appInstData *pInst)
{
    CsrBtPasAuthenticateInd *pMsg = (CsrBtPasAuthenticateInd *) pInst->pRecvMsg;

    /* client want to authenticate server */
    if (pInst->nPasskeyLen > 0)
    {
        /* (Re)use OBEX passkey to authentication of server */
        printf("Reuse remote OBEX passkey: %s\n", pInst->szPasskey);
        CsrBtPasAuthResSend((CsrUint16)CsrStrLen((char*)pInst->szPasskey), (CsrUint8 *)CsrStrDup((char*)pInst->szPasskey), NULL);
    }
    else
    {
        printf("Enter remote OBEX authenticate passkey for \"%04X:%02X:%06X\": ",
               pInst->clientDevAddr.nap,
               pInst->clientDevAddr.uap,
               pInst->clientDevAddr.lap);

        pInst->authInitiator = AUTH_REMOTE_INITIATED_STATE;
        pInst->keyboardState = PASSKEY_KEYBOARD_STATE;
        pInst->nPasskeyLen = 0;
        pInst->szPasskey[0] = 0;
    }
    CsrPmemFree(pMsg->realm);
}

static void handleCsrBtPasPullPbInd(appInstData *pInst)
{
    CsrBtPasPullPbInd *pMsg = pInst->pRecvMsg;
    char *pName;

    pName = NULL;

    CsrBtChdir(pInst->path);
    if (pMsg->ucs2nameOffset)
    {
        pName = unicode2char((pMsg->payload + pMsg->ucs2nameOffset));
        printf("upload phonebook: %s\n", pName);
    }
    else
    {
        printf("No phonebook name\n");
    }

    /* do not upload more than pMsg->maxListCnt phonebook entries */
    printf("Maximum size of listing: %d\n", pMsg->maxListCnt);

    /* Remark: the maxListCnt parameter is not used by the demo application,
     * because all data used by the demo app. prerecorded and stored in files
     * For real server app. it must never upload more entries than given in this
     * parameter.
 */
    if (CSR_BT_PAS_FORMAT_VCARD3_0 == pMsg->format)
    {
        /* Use VCARD version 3.0 */
        printf("Format: VCARD 3.0\n");
    }
    else
    {
        /* Use VCARD cersion 2.1 */
        printf("Format: VCARD 2.1\n");
    }

    /* use phonebook store on SIM */
    if (CSR_BT_PAS_SRC_SIM == pMsg->src)
    {
        printf("Use SIM\n");
        setDir(PAS_SIM_PATH);
    }
    else
    {
        /* use phonebook store on the phonebooks internal memory */
        printf("Use internal memory\n");
        setDir(PAS_PHONE_PATH);
    }

    if (NULL != pInst->pFile)
    {
        CsrBtFclose(pInst->pFile);
        pInst->pFile = NULL;
    }

    if(pName)
    {
        pInst->pFile = CsrBtFopen(pName, "rb");
    }

    /* Success */
    if (NULL != pInst->pFile)
    {
        CsrInt32 pos;

        CsrBtFseek(pInst->pFile, 0L, SEEK_SET);
        CsrBtFseek(pInst->pFile, 0L, SEEK_END);
        pos = CsrBtFtell(pInst->pFile);
        CsrBtFseek(pInst->pFile, 0L, SEEK_SET);
        if (pos > 0)
        {
            pInst->target = APP_PULL_PB_TARGET;
        }
        else
        {
            pInst->target = APP_NONE_TARGET;
        }
        CsrBtPasPullPbResSend(pInst->connectionId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE);
        printf("Start uploading phonebook: %s size: %ld\n", pName, pos);
    }
    else
    {
        /* failed */
        pInst->target = APP_NONE_TARGET;
        CsrBtPasPullPbResSend(pInst->connectionId, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE);
        printf("Phonebook: %s file not found!\n", pName);
    }

    if (CSR_BT_PAS_SRC_SIM == pMsg->src)
    {
        /* use phonebook store on SIM */
        setDir(PAS_SIM_BACK_PATH);
    }
    else
    {
        /* use phonebook store on the phonebooks internal memory */
        setDir(PAS_PHONE_BACK_PATH);
    }

    CsrPmemFree(pName);

    CsrPmemFree(pMsg->payload);
}

static void handleCsrBtPasPullPbSizeInd(appInstData *pInst)
{
    CsrBtPasPullPbSizeInd *pMsg = pInst->pRecvMsg;
    char *pName = NULL;
    CsrUint16 entries = 0;

    pInst->nPbSize = 0;

    if (pMsg->ucs2nameOffset)
    {
        pName = unicode2char((pMsg->payload + pMsg->ucs2nameOffset));
        printf("Upload phonebook size for: %s\n", pName);
    }
    else
    {
        printf("No phonebook name\n");
    }
    if (CSR_BT_PAS_SRC_SIM == pMsg->src)
    {
        /* use phonebook store on SIM */
        printf("Use SIM\n");
        entries = pbEntriesCount(PB_LOCATION_SIM, pName);
    }
    else
    {   /* use phonebook store on the phonebooks internal memory */
        printf("Use internal memory\n");
        entries = pbEntriesCount(PB_LOCATION_PHONE, pName);
    }

    if (entries == VCARD_FILE_OPEN_ERROR)
    {
        printf("\nERROR: Phonebook file could not be opened\n");
        CsrBtPasPullPbSizeResSend(pInst->connectionId, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE, 0, FALSE);
    }
    else
    {
        pInst->nPbSize = entries;
        printf("Phonebook size: %i\n", pInst->nPbSize);
        CsrBtPasPullPbSizeResSend(pInst->connectionId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, pInst->nPbSize, FALSE);
    }

    CsrPmemFree(pName);
    CsrPmemFree(pMsg->payload);
}

static void handleCsrBtPasPullMchInd(appInstData *pInst)
{
    CsrBtPasPullMchInd *pMsg = pInst->pRecvMsg;
    const char *name;

    name = MCH;

    CsrBtChdir(pInst->path);
    printf("Upload Missed Call History\n");

    /* do not upload more than pMsg->maxListCnt phonebook entries */
    printf("Maximum size of listing: %d\n", pMsg->maxListCnt);

    /* Remark: the maxListCnt parameter is not used by the demo application,
     * because all data used by the demo app. prerecorded and stored in files
     * For real server app. it must never upload more entries than given in this
     * parameter.
 */


    /* Use VCARD version 3.0 */
    if (CSR_BT_PAS_FORMAT_VCARD3_0 == pMsg->format)
    {
        printf("Format: VCARD 3.0\n");
    }
    else
    {
        /* Use VCARD cersion 2.1 */
        printf("Format: VCARD 2.1\n");
    }

    /* use phonebook store on SIM */
    if (CSR_BT_PAS_SRC_SIM == pMsg->src)
    {
        printf("Use SIM\n");
        setDir(PAS_SIM_PATH);
    }
    else
    {
        /* use phonebook store on the phonebooks internal memory */
        printf("Use internal memory\n");
        setDir(PAS_PHONE_PATH);
    }

    if (NULL != pInst->pFile)
    {
        CsrBtFclose(pInst->pFile);
        pInst->pFile = NULL;
    }
    pInst->pFile = CsrBtFopen(name, "rb");

    if (NULL != pInst->pFile)
    {   /* success */
        CsrInt32 pos;

        CsrBtFseek(pInst->pFile, 0L, SEEK_SET);
        CsrBtFseek(pInst->pFile, 0L, SEEK_END);
        pos = CsrBtFtell(pInst->pFile);
        CsrBtFseek(pInst->pFile, 0L, SEEK_SET);
        if (pos > 0)
        {
            pInst->target = APP_PULL_PB_TARGET;
        }
        else
        {
            pInst->target = APP_NONE_TARGET;
        }
        CsrBtPasPullMchResSend(pInst->connectionId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, pInst->nNewMissedCall, FALSE);
        printf("Start uploading mch size: %ld\n", pos);
    }
    else
    {   /* failed */
        pInst->target = APP_NONE_TARGET;
        CsrBtPasPullMchResSend(pInst->connectionId, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE, 0, FALSE);
        printf("mch.vcf: file not found!\n");
    }
    if (CSR_BT_PAS_SRC_SIM == pMsg->src)
    {   /* use phonebook store on SIM */
        setDir(PAS_SIM_BACK_PATH);
    }
    else
    {   /* use phonebook store on the phonebooks internal memory */
        setDir(PAS_PHONE_BACK_PATH);
    }
}

static void handleCsrBtPasPullMchSizeInd(appInstData *pInst)
{
    CsrBtPasPullMchSizeInd *pMsg = pInst->pRecvMsg;

    printf("Upload Mch size\n");
    if (CSR_BT_PAS_SRC_SIM == pMsg->src)
    {   /* use phonebook store on SIM */
        printf("Use SIM\n");
    }
    else
    {   /* use phonebook store on the phonebooks internal memory */
        printf("Use internal memory\n");
    }
    printf("Phonebook size: %i\n", pInst->nPbSize);
    CsrBtPasPullMchSizeResSend(pInst->connectionId, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, pInst->nPbSize, pInst->nNewMissedCall, FALSE);
}

static void handleSetFolderInd(appInstData *pInst)
{
    CsrBtPasSetFolderInd *pMsg = pInst->pRecvMsg;
    CsrBtObexResponseCode responseCode;
    char *pName = NULL;

    pInst->target = APP_NONE_TARGET;
    if (pMsg->ucs2nameOffset)
    {
        pName = unicode2char((pMsg->payload + pMsg->ucs2nameOffset));
        printf("Set folder to: %s\n", pName);
        CsrBtChdir(pInst->currentPath);
        if (CsrBtChdir((char*)pName))
        {   /* File error */
            char buffer[CSR_BT_MAX_PATH_LENGTH];

            CsrBtGetCwd(buffer, CSR_BT_MAX_PATH_LENGTH);

            responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
            printf("File error: %i\n", errno);
            printf("CWD: %s\n", buffer);

        }
        else
        {
            CsrBtGetCwd(pInst->currentPath, CSR_BT_MAX_PATH_LENGTH);
            responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            printf("Path set successful\n");
        }
    }
    else
    {
        printf("No folder name\n");
        responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
    }
    CsrPmemFree(pName);
    CsrPmemFree(pMsg->payload);
    CsrBtPasSetFolderResSend(pInst->connectionId, responseCode);
}

static void handleSetBackFolderInd(appInstData *pInst)
{
    CsrBtObexResponseCode responseCode;

    printf("Set folder back\n");
    CsrBtChdir(pInst->currentPath);
    if (CsrBtChdir(".."))
    {
        /* File error */
        responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
        printf("File error: %i\n", errno);
    }
    else
    {
        CsrBtGetCwd(pInst->currentPath, CSR_BT_MAX_PATH_LENGTH);
        responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
        printf("Path set back successful\n");
    }
    CsrBtPasSetFolderResSend(pInst->connectionId, responseCode);
}

static void handleSetRootFolderInd(appInstData *pInst)
{
    CsrBtObexResponseCode   responseCode;

    printf("Set folder to root\n");
    if (CsrBtChdir(pInst->rootPath))
    {   /* File error */
        responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
        printf("File error: %i\n", errno);
    }
    else
    {
        CsrBtGetCwd(pInst->currentPath, CSR_BT_MAX_PATH_LENGTH);
        responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
        printf("Path set to root successful\n");
    }
    CsrBtPasSetFolderResSend(pInst->connectionId, responseCode);
}

static void handlePullvCardListSizeInd(appInstData *pInst)
{
    CsrBtPasPullVcardListSizeInd *pMsg = pInst->pRecvMsg;
    DIR *pDir = NULL;
    CsrUint16 nFileCnt = 0;
    CsrBtObexResponseCode responseCode;
    char *pName = NULL;

    printf("Pull v-card listing size request.\n");
    pInst->target = APP_NONE_TARGET;
    if (pMsg->ucs2nameOffset)
    {
        pName = unicode2char((pMsg->payload + pMsg->ucs2nameOffset));
        printf("Name: %s\n", pName);
        responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
    }
    else
    {
        printf("No dir specified, using current dir \n");
        responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
    }
    /* do folder list stuff here */
    CsrBtChdir(pInst->currentPath);
    if (pName)
    {
        CsrBtChdir(pName);
        CsrPmemFree(pName);
    }
    pDir = CsrBtOpenDir(".");
    if (NULL == pDir)
    {
        responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
        printf("No files match in current folder.\n");
    }
    else
    {
        char path[CSR_BT_MAX_PATH_LENGTH];
        path[0] = '\0';

        if (NULL != pInst->pFile)
        {
            CsrBtFclose(pInst->pFile);
            pInst->pFile = NULL;
        }
        CsrStrCpy(path, pInst->rootPath);
        CsrStrCat(path, TMPFILE);
        pInst->pFile = CsrBtFopen(path, "wb");
        if (NULL == pInst->pFile)
        {
            printf("File error: could not create file\n");
            CsrBtPasPullVcardListSizeResSend(
                pInst->connectionId, CSR_BT_OBEX_INTERNAL_SERVER_ERROR_RESPONSE_CODE, 0, FALSE);
        }
        nFileCnt = 1;
        responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
        while (CsrBtReadDir(pDir) == 0)
        {
            nFileCnt++;
        }
        CsrBtCloseDir(pDir);

        CsrBtFclose(pInst->pFile);
        pInst->pFile = NULL;
    }
    CsrPmemFree(pMsg->payload);
    printf("vcard list size %d\n", nFileCnt);
    CsrBtPasPullVcardListSizeResSend(pInst->connectionId, responseCode, nFileCnt, FALSE);
}

static void handlePullvCardMchListSizeInd(appInstData *pInst)
{
    DIR *pDir;
    CsrUint16 nFileCnt = 0;
    CsrBtObexResponseCode   responseCode;

    printf("Pull v-card listing size request.\n");
    pInst->target = APP_NONE_TARGET;
    if (!pInst->isMch)
    {
        printf("Name: %s\n", MCHDIR);
    }
    else
    {
        printf("Current folder\n");
    }
    /* do folder list stuff here */
    CsrBtChdir(pInst->currentPath);
    if (!pInst->isMch)
    {
        CsrBtChdir(MCHDIR);
    }
    pDir = CsrBtOpenDir(".");
    if (NULL == pDir)
    {
        responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
        printf("No files match in current folder.\n");
    }
    else
    {
        char path[CSR_BT_MAX_PATH_LENGTH];
        path[0] = '\0';

        if (NULL != pInst->pFile)
        {
            CsrBtFclose(pInst->pFile);
            pInst->pFile = NULL;
        }

        CsrStrCpy(path, pInst->rootPath);
        CsrStrCat(path, TMPFILE);
        pInst->pFile = CsrBtFopen(path, "wb");
        if (NULL == pInst->pFile)
        {
            printf("File error: could not create file\n");
            CsrBtPasPullVcardMchListSizeResSend(pInst->connectionId, CSR_BT_OBEX_INTERNAL_SERVER_ERROR_RESPONSE_CODE, 0, 0, FALSE);
        }
        nFileCnt = 1;
        responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
        while ( CsrBtReadDir(pDir) == 0)
        {
            nFileCnt++;
        }
        CsrBtCloseDir(pDir);

        CsrBtFclose(pInst->pFile);
    }
    CsrBtPasPullVcardMchListSizeResSend(pInst->connectionId, responseCode, nFileCnt, pInst->nNewMissedCall, FALSE);
}

static void handlePullvCardListIndFile(appInstData *pInst)
{
    CsrBtPasPullVcardListInd *pMsg = pInst->pRecvMsg;
    CsrUint32 nObjLen = 0;
    char name[CSR_BT_MAX_PATH_LENGTH];
    CsrBtObexResponseCode responseCode;
    char *pName = NULL;

    name[0] = '\0';

    printf("Pull v-card listing request.\n");
    pInst->target = APP_PULL_LISTING_TARGET;
    if (pMsg->ucs2nameOffset)
    {
        pName = unicode2char((pMsg->payload + pMsg->ucs2nameOffset));
        CsrStrCpy(name, pName);
        CsrStrCat(name, "*");
        printf("Name: %s\n", pName);
        CsrPmemFree(pName);
    }
    else
    {
        CsrStrCpy(name, ".*");
        printf("Current folder\n");
    }
    if (pMsg->uft8SearchValOffset)
    {
        char *pSearch = NULL;

        if (*(pMsg->payload + pMsg->uft8SearchValOffset + pMsg->searchValLength - 1) != '\0')
        {
            pSearch = CsrPmemAlloc((pMsg->searchValLength + 1));
            CsrStrNCpyZero(pSearch, (char*)(pMsg->payload + pMsg->uft8SearchValOffset), pMsg->searchValLength + 1);
        }
        else
        {
            pSearch = CsrPmemAlloc((pMsg->searchValLength));
            CsrStrNCpyZero(pSearch, (char*)(pMsg->payload + pMsg->uft8SearchValOffset), pMsg->searchValLength);
        }

        if (NULL != pSearch)
        {
            printf("Search string: %s\n", pSearch);
            CsrPmemFree(pSearch);
        }
        else
        {
            printf("No search\n");
        }
    }
    else
    {
        printf("No search\n");
    }
    printf("Search attribute: ");
    switch (pMsg->searchAtt)
    {
        default:
        case CSR_BT_PAS_SEARCH_ATT_NAME:
            printf("Name\n");
            break;

        case CSR_BT_PAS_SEARCH_ATT_NUMBER:
            printf("Number\n");
            break;

        case CSR_BT_PAS_SEARCH_ATT_SOUND:
            printf("Sound\n");
            break;
    }
    printf("Order: ");
    switch (pMsg->order)
    {
        case CSR_BT_PAS_ORDER_ALPHABETICAL:
            printf("Alphabetical\n");
            break;

        default:
        case CSR_BT_PAS_ORDER_INDEXED:
            printf("Indexed\n");
            break;

        case CSR_BT_PAS_ORDER_PHONETICAL:
            printf("Phonetical\n");
            break;
    }
    printf(
        "Start offset: %u, Max number entries in listing: %u\n",
        pMsg->listStartOffset, pMsg->maxListCnt
    );
    /* do folder list stuff here */
    CsrBtChdir(pInst->rootPath);
    if (NULL != pInst->pFile)
    {
        CsrBtFclose(pInst->pFile);
        pInst->pFile = NULL;
    }
    pInst->pFile = CsrBtFopen(LIST, "rb");
    if (NULL != pInst->pFile)
    {   /* success */
        CsrInt32 pos;
        CsrUint8 finalFlag;

        CsrBtFseek(pInst->pFile, 0L, SEEK_SET);
        CsrBtFseek(pInst->pFile, 0L, SEEK_END);
        pos = CsrBtFtell(pInst->pFile);
        CsrBtFseek(pInst->pFile, 0L, SEEK_SET);
        if (pos > 0)
        {
            finalFlag = FALSE;
            pInst->target = APP_PULL_LISTING_TARGET;
        }
        else
        {
            pInst->target = APP_NONE_TARGET;
            finalFlag = TRUE;
        }
        nObjLen = pos;
        responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
    }
    else
    {
        printf("File error: could not open file %s\n", LIST);
        responseCode = CSR_BT_OBEX_INTERNAL_SERVER_ERROR_RESPONSE_CODE;
        nObjLen = 0;
    }

    CsrPmemFree(pMsg->payload);
    CsrBtPasPullVcardListResSend(pInst->connectionId, responseCode, FALSE);
}

static void handlePullvCardMchListIndFile(appInstData *pInst)
{
    CsrBtPasPullVcardMchListInd *pMsg = pInst->pRecvMsg;
    CsrUint32 nObjLen = 0;
    char name[CSR_BT_MAX_PATH_LENGTH];
    CsrBtObexResponseCode   responseCode;

    printf("Pull Mch v-card listing request.\n");
    CsrStrCpy(name, MCH);
    pInst->target = APP_PULL_LISTING_TARGET;

    printf("Name: %s\n", name);
    if (pMsg->uft8SearchValOffset)
    {
        char *pSearch = NULL;

        if (*(pMsg->payload + pMsg->uft8SearchValOffset + pMsg->searchValLength - 1) != '\0')
        {
            /* we need to add zero-termination to string */
            char *pVal = CsrPmemAlloc((pMsg->searchValLength + 1));

            pSearch = pVal;
            CsrStrNCpyZero(pVal,
                        (char*)(pMsg->payload + pMsg->uft8SearchValOffset),
                        pMsg->searchValLength + 1);
            pSearch = pVal;
            CsrPmemFree(pVal);
        }
        else
        {
            pSearch = (char*)(pMsg->payload + pMsg->uft8SearchValOffset);
        }

        if (NULL != pSearch)
        {
            printf("Search string: %s\n", pSearch);
            CsrPmemFree(pSearch);
        }
        else
        {
            printf("No search\n");
        }
    }
    else
    {
        printf("No search\n");
    }
    printf("Search attribute: ");
    switch (pMsg->searchAtt)
    {
        default:
        case CSR_BT_PAS_SEARCH_ATT_NAME:
            printf("Name\n");
            break;

        case CSR_BT_PAS_SEARCH_ATT_NUMBER:
            printf("Number\n");
            break;

        case CSR_BT_PAS_SEARCH_ATT_SOUND:
            printf("Sound\n");
            break;
    }
    printf("Order: ");
    switch (pMsg->order)
    {
        case CSR_BT_PAS_ORDER_ALPHABETICAL:
            printf("Alphabetical\n");
            break;

        default:
        case CSR_BT_PAS_ORDER_INDEXED:
            printf("Indexed\n");
            break;

        case CSR_BT_PAS_ORDER_PHONETICAL:
            printf("Phonetical\n");
            break;
    }
    printf(
        "Start offset: %u, Max number entries in listing: %u\n",
        pMsg->listStartOffset, pMsg->maxListCnt
    );
    /* do folder list stuff here */
    CsrBtChdir(pInst->rootPath);
    if (NULL != pInst->pFile)
    {
        CsrBtFclose(pInst->pFile);
        pInst->pFile = NULL;
    }
    pInst->pFile = CsrBtFopen(LIST, "rb");
    if (NULL != pInst->pFile)
    {   /* success */
        CsrInt32 pos;
        CsrUint8 finalFlag;

        CsrBtFseek(pInst->pFile, 0L, SEEK_SET);
        CsrBtFseek(pInst->pFile, 0L, SEEK_END);
        pos = CsrBtFtell(pInst->pFile);
        CsrBtFseek(pInst->pFile, 0L, SEEK_SET);
        if (pos > 0)
        {
            finalFlag = FALSE;
            pInst->target = APP_PULL_LISTING_TARGET;
        }
        else
        {
            pInst->target = APP_NONE_TARGET;
            finalFlag = TRUE;
        }
        nObjLen = pos;
        responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
    }
    else
    {
        printf("File error: could not open file %s\n", LIST);
        responseCode = CSR_BT_OBEX_INTERNAL_SERVER_ERROR_RESPONSE_CODE;
        nObjLen = 0;
    }
    CsrPmemFree(pMsg->payload);
    CsrBtPasPullVcardMchListResSend(pInst->connectionId, responseCode, (CsrUint8)nObjLen, FALSE);
}

static void handlePullvCardEntryInd(appInstData *pInst)
{
    CsrBtPasPullVcardEntryInd *pMsg = pInst->pRecvMsg;

    printf("Upload Phonebook entry.\n");
    if (pMsg->ucs2nameOffset)
    {
        char *pName = NULL;

        if (NULL != pInst->pFile)
        {
            CsrBtFclose(pInst->pFile);
            pInst->pFile = NULL;
        }

        /* set current folder to root */
        CsrBtChdir(pInst->currentPath);
        pName = unicode2char((pMsg->payload + pMsg->ucs2nameOffset));
        if (pName)
        {
            pInst->pFile = CsrBtFopen(pName, "rb");
        }

        if(NULL != pInst->pFile)
        {
            /* success */
            CsrInt32 pos;
            CsrUint8 finalFlag;

            CsrBtFseek(pInst->pFile, 0L, SEEK_SET);
            CsrBtFseek(pInst->pFile, 0L, SEEK_END);
            pos = CsrBtFtell(pInst->pFile);
            CsrBtFseek(pInst->pFile, 0L, SEEK_SET);
            if (pos > 0)
            {
                finalFlag = FALSE;
                pInst->target = APP_PULL_ENTRY_TARGET;
            }
            else
            {
                pInst->target = APP_NONE_TARGET;
                finalFlag = TRUE;
            }
            printf("Start uploading phonebook: %s size: %lu, filter: %02X%02X%02X%02X:%02X%02X%02X%02X\n",
                       pName,
                       pos, pMsg->filter[7],
                       pMsg->filter[6],
                       pMsg->filter[5],
                       pMsg->filter[4],
                       pMsg->filter[3],
                       pMsg->filter[2],
                       pMsg->filter[1],
                       pMsg->filter[0]);
            if (CSR_BT_PAS_FORMAT_VCARD3_0 == pMsg->format)
            {   /* Use VCARD version 3.0 */
                printf("Format: VCARD 3.0\n");
            }
            else
            {   /* Use VCARD cersion 2.1 */
                printf("Format: VCARD 2.1\n");
            }
            CsrBtPasPullVcardEntryResSend(pInst->connectionId, finalFlag, CSR_BT_OBEX_SUCCESS_RESPONSE_CODE, FALSE);
        }
        else
        {   /* failed */
            pInst->target = APP_NONE_TARGET;
            CsrBtPasPullVcardEntryResSend(pInst->connectionId, TRUE, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE, FALSE);
            printf("Entry: %s file not found!\n", pName);
        }
        CsrPmemFree(pName);
    }
    else
    {
        pInst->target = APP_NONE_TARGET;
        CsrBtPasPullVcardEntryResSend(pInst->connectionId, TRUE, CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE, FALSE);
    }
    CsrPmemFree(pMsg->payload);
}

static void handleCsrBtPasNextInd(appInstData *pInst)
{
    CsrInt32              nBodyLen;
    CsrUint8               finalFlag;
    CsrUint8            *pBody       = NULL;
    CsrBtObexResponseCode responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;


    if (NULL != pInst->pFile)
    {
        CsrInt32 nOldPos = CsrBtFtell(pInst->pFile), nNewPos;

        CsrBtFseek(pInst->pFile, 0L, SEEK_END);
        nNewPos = CsrBtFtell(pInst->pFile);
        if (nNewPos > nOldPos)
        {
            size_t nRead;

            if (nNewPos - nOldPos > pInst->obexPeerMaxPacketSize)
            {
                finalFlag = FALSE;
                nBodyLen = pInst->obexPeerMaxPacketSize;
            }
            else
            {
                finalFlag = TRUE;
                nBodyLen = nNewPos - nOldPos;
            }
            CsrBtFseek(pInst->pFile, nOldPos, SEEK_SET); /* set file position back to start value */
            pBody = CsrPmemAlloc(nBodyLen*sizeof(CsrUint8));
            nRead = CsrBtFread(pBody, sizeof(CsrUint8), nBodyLen, pInst->pFile);
            if (nRead == (size_t)nBodyLen)
            {
                responseCode = CSR_BT_OBEX_SUCCESS_RESPONSE_CODE;
            }
            else
            {
                /* file error, send cfm */
                responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
                finalFlag = TRUE;
                nBodyLen = 0;
                CsrPmemFree(pBody);
                pBody = NULL;
            }
        }
        else
        {
            /* end of file, send cfm */
            responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
            finalFlag = TRUE;
            nBodyLen = 0;
        }
    }
    else
    {
        responseCode = CSR_BT_OBEX_NOT_FOUND_RESPONSE_CODE;
        finalFlag = TRUE;
        nBodyLen = 0;
    }
    if (finalFlag && pInst->pFile)
    {
        CsrBtFclose(pInst->pFile);
        pInst->pFile = NULL;
    }
    printf("Transfer: %ld bytes\n", nBodyLen);
    if (finalFlag)
    {
        printf("Download ended\n\n");
    }
    CsrBtPasNextResSend(pInst->connectionId, finalFlag, responseCode, (CsrUint16)nBodyLen, pBody, FALSE);
}

static void handleCsrBtPasAbortInd(appInstData *pInst)
{
    CsrBtPasAbortInd *pMsg = pInst->pRecvMsg;

    printf("Aborting current operation.\n\n");

    pInst->target = APP_NONE_TARGET;
    if (NULL != pInst->pFile)
    {
        CsrBtFclose(pInst->pFile);
        pInst->pFile = NULL;
    }
    CsrBtChdir(pInst->rootPath);

    if (pMsg->payload)
    {
        CsrPmemFree(pMsg->payload);
        pMsg->payload = NULL;
    }
}

static void handleCsrBtPasDisconnectInd(appInstData *pInst)
{
    if (pInst->pFile)
    {
        CsrBtFclose(pInst->pFile);
        pInst->pFile = NULL;
    }
    pInst->target = APP_NONE_TARGET;
    pInst->state = IDLE_STATE;
    pInst->nPasskeyLen = 0;

    printf("Phonebook client disconnected.\n");
}

/*************************  KBM PRIMITIVES  ***************************/

/**************************************************************************************************
*
*   function to handle all keyboard presses (sent as a keyboard event signal).
*
**************************************************************************************************/

static void keyMessageService(CsrUint16 m, void *data)
{
    if (_kbhit())
    {
        CsrBtKeyPress_t *key;
        key = CsrPmemAlloc(sizeof(CsrBtKeyPress_t));
        key->type = KEY_MESSAGE;
        key->key =_getch();
        CsrSchedMessagePut(TESTQUEUE, KEY_MESSAGE, key);
    }
    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, keyMessageService, 0, NULL);
}

/**************************************************************************************************
*
*   a keyboard event is received.
*   first check for selection of a found device during inquiry if state is inquiry. If not start
*   proper action as selected.
*
**************************************************************************************************/
static void handleKeybPrim(appInstData *pInst)
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
        return;
    }
    switch (pInst->keyboardState)
    {
        case PASSKEY_KEYBOARD_STATE:
            passkeyState(pInst, key->key);
            break;

        case SECURITY_KEYBOARD_STATE:
            handleScKeybPrim(&pInst->scInstData);
            break;

        case IDLE_KEYBOARD_STATE:
            idleState(pInst, key->key);
            break;

        default:
            printf("Unknown state: %d\n", pInst->keyboardState);
            break;
    }
}

static CsrBool passkeyState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool isPasskey = TRUE;

    if (RETURN_KEY == cKey)
    {
        pInst->szPasskey[pInst->nPasskeyLen] = '\0';
        pInst->keyboardState = IDLE_KEYBOARD_STATE;
        printf("\nSend OBEX server authenticate\n");

        switch (pInst->authInitiator)
        {
            case AUTH_LOCAL_INITIATED_STATE:
                CsrBtPasAuthReqSend(0, NULL, (CsrUint16)CsrStrLen((char*)pInst->szPasskey), (CsrUint8 *)CsrStrDup((char*)pInst->szPasskey), NULL);
                break;

            case AUTH_REMOTE_INITIATED_STATE:
                CsrBtPasAuthResSend((CsrUint16)CsrStrLen((char*)pInst->szPasskey), (CsrUint8 *)CsrStrDup((char*)pInst->szPasskey), NULL);
                break;

            default:
                printf("Unknown state: %d\n", pInst->state);
                break;
        }
    }
    else if (BACKSPACE_KEY == cKey)
    {
        if (pInst->nPasskeyLen > 0)
        {
            pInst->nPasskeyLen--;
            printf("%c %c", cKey, cKey);
        }
    }
    else if ((cKey >= 32) && (cKey < 127))
    {
        if (pInst->nPasskeyLen < CSR_BT_PASSKEY_MAX_LEN)
        {
            pInst->szPasskey[pInst->nPasskeyLen++] = cKey;
            printf("%c", cKey);
        }
    }
    else
    {
        isPasskey = FALSE;
    }
    return isPasskey;
}

static CsrBool idleState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool isProcessed = TRUE;

    switch(cKey)
    {
        case '1':
        {
            if ( IDLE_STATE == pInst->state )
            {
                pInst->state = ACTIVE_STATE;
                pInst->doAuth = FALSE;
                CsrBtPasActivateReqSend(pInst->CsrSchedQid,
                                        ( CSR_BT_PAS_SUPPORT_FEATURE_DOWNLOAD |
                                          CSR_BT_PAS_SUPPORT_FEATURE_BROWSING),
                                        ( CSR_BT_PAS_SUPPORT_REPOSITORY_PB | 
                                          CSR_BT_PAS_SUPPORT_REPOSITORY_SIM),
                                        CSR_BT_MAX_OBEX_SIGNAL_LENGTH,
                                        0, /*windowSize*/
                                        TRUE); /*enableSrm*/

                printf("\nServer started with PB and SIM support...\n");
            }
            break;
        }
        case '2':
        {
            if ( IDLE_STATE == pInst->state )
            {
                pInst->state = ACTIVE_STATE;
                pInst->doAuth = FALSE;
                CsrBtPasActivateReqSend(pInst->CsrSchedQid,
                                        ( CSR_BT_PAS_SUPPORT_FEATURE_DOWNLOAD |
                                          CSR_BT_PAS_SUPPORT_FEATURE_BROWSING),
                                        ( CSR_BT_PAS_SUPPORT_REPOSITORY_PB),
                                        CSR_BT_MAX_OBEX_SIGNAL_LENGTH,
                                        0, /*windowSize*/
                                        TRUE); /*enableSrm*/

                printf("\nServer started with PB only support...\n");
            }
            break;
        }
        case '3':
        {
            if ( IDLE_STATE == pInst->state )
            {
                pInst->state = ACTIVE_STATE;
                pInst->doAuth = TRUE;
                CsrBtPasActivateReqSend(pInst->CsrSchedQid,
                                        ( CSR_BT_PAS_SUPPORT_FEATURE_DOWNLOAD | 
                                          CSR_BT_PAS_SUPPORT_FEATURE_BROWSING),
                                        ( CSR_BT_PAS_SUPPORT_REPOSITORY_PB |
                                          CSR_BT_PAS_SUPPORT_REPOSITORY_SIM),
                                        CSR_BT_MAX_OBEX_SIGNAL_LENGTH,
                                        0, /*windowSize*/
                                        TRUE); /*enableSrm*/
                                        
                printf("\nServer started ...");
            }
            break;
        }
        case '4':
        {
            if ( IDLE_STATE == pInst->state )
            {
                handleScKeybPrim(&pInst->scInstData);
            }
            break;
        }
        case 'x':
        case 'X':
            printf("Deactivate server\n");
            pInst->state = DEACTIVATION_PENDING_STATE;
            CsrBtPasDeactivateReqSend();
            break;

        default:
            playIntro();
            isProcessed = FALSE;
            break;

    }
    return isProcessed;
}

static void setDir(char *path)
{
    if (CsrBtChdir(path))
    {
        printf("Unable to locate the directory: %s\n", path);
        CsrBtMkDir(path);
        CsrBtChdir(path);
    }
}

static char *unicode2char(CsrUcs2String *unicode)
{
    CsrUint8 *str;

    str = CsrUcs2ByteString2Utf8(unicode);
    return ((char *)str);
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
