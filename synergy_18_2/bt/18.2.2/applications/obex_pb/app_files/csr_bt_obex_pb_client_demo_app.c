/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csr_bt_platform.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_profiles.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_pac_lib.h"
#include "csr_bt_pac_prim.h"
#include "csr_bt_obex.h"
#include "csr_bt_platform.h"
#include "csr_bt_demoapp.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_sched_init.h"

#include "csr_unicode.h"

#define KEYB_CHECK_TIMEOUT              (100000)

#define MAX_DISCOVERY_RESULTS_IN_LIST   (10)
#define PAC_MAX_OBEX_PACKET_SIZE        (0x1f00)
#define PB_LIST_MAX                     (0xff)
#define PB_DEFAULT_FOLDER_NAME          "folder"
#define MAX_ENTRIES                     6
#define MAX_SEARCH                      50

typedef CsrUint8 appState;
#define IDLE_STATE (appState) 0x00
#define SEARCHING_STATE (appState) 0x01
#define CLOSING_SEARCH_STATE (appState) 0x02
#define CONNECT_INIT_STATE (appState) 0x03
#define CONNECT_STATE (appState) 0x04
#define AUTHENTICATING_STATE (appState) 0x05
#define CONNECTED_STATE (appState) 0x06
#define BONDING_STATE (appState) 0x07
#define LAST_STATE (appState) 0x08


typedef CsrUint8 kbmState;
#define NORMAL_STATE (kbmState) 0x00
#define PASSKEY_STATE (kbmState) 0x01
#define SECURITY_KEYBOARD_STATE (kbmState) 0x02
#define ADDRESS_STATE (kbmState) 0x03
#define FILTER_STATE (kbmState) 0x04
#define PB_SOURCE_STATE (kbmState) 0x05
#define PB_NAME_STATE (kbmState) 0x06
#define PB_ENTRIES_STATE (kbmState) 0x07
#define FOLDER_NAME_STATE (kbmState) 0x08
#define VLIST_NAME_STATE (kbmState) 0x09
#define VLIST_ORDER_STATE (kbmState) 0x0A
#define VLIST_SEARCH_ATT_STATE (kbmState) 0x0B
#define VLIST_SEARCH_VAL_STATE (kbmState) 0x0C
#define VLIST_ENTRIES_STATE (kbmState) 0x0D
#define VLIST_OFFSET_STATE (kbmState) 0x0E
#define VCARD_NAME_STATE (kbmState) 0x0F
#define LAST_KBM_STATE (kbmState) 0x10

typedef struct {
    CsrSchedQid       CsrSchedQid;
    CsrUint16        event;
    void            *pRecvMsg;
    appState        state;
    kbmState        kbm;
    kbmState        prevKbm;
    CsrUint32        nDevice;
    CsrUint32        nDeviceInList;
    CsrBool          bFullDeviceList;
    CsrBtDeviceAddr    bdAddrList[MAX_DISCOVERY_RESULTS_IN_LIST];
    CsrBool          bAuth;
    CsrUint8         passkeyLength;
    CsrUint8         passkey[CSR_BT_PASSKEY_MAX_LEN];
    char            szBdAddr[15];
    CsrUint32        BdAddrLen;
    CsrBtPacSrcType      src;
    char            name[CSR_BT_MAX_PATH_LENGTH];
    CsrUint32        nNameLen;
    char            szEntries[MAX_ENTRIES];
    CsrUint32        nEntriesLen;
    char            path[CSR_BT_MAX_PATH_LENGTH];
    CsrUint32        nPathLen;
    CsrBtPacOrderType    order;
    CsrBtPacSearchAtt    searchAtt;
    char            search[MAX_SEARCH];
    CsrUint32        nSearchLen;
    CsrUint16        nMaxLstCnt;
    char            szOffset[MAX_ENTRIES];
    CsrUint32        nOffsetLen;
    CsrFileHandle            *pFile;
    CsrUint32        nObjLen;
    ScAppInstData_t scInstData;
    CsrUint8         filter[8];
} appInstData;

static void playIntro(appInstData *pInst);
static void KeyMessageService(CsrUint16 pInst, void *data);

static void handleSdPrim(appInstData *pInst);

static void handlePacPrim(appInstData *pInst);
static void handleCsrBtPacConnectCfm(appInstData *pInst);
static void handlePacAuthInd(appInstData *pInst);
static void handleCsrBtPacPullPbCfm(appInstData *pInst);
static void handleCsrBtPacPullPbInd(appInstData *pInst);
static void handleSetFolderCfm(appInstData *pInst);
static void handlePullvCardListCfm(appInstData *pInst);
static void handlePullvCardListInd(appInstData *pInst);
static void handlePullvCardEntryCfm(appInstData *pInst);
static void handlePullvCardEntryInd(appInstData *pInst);
static void handlePacAbort(appInstData *pInst);
static void handlePacDisconnect(appInstData *pInst);

static void startConnect(appInstData *pInst);
static void searchStart(appInstData *pInst);
static void pullPbStart(appInstData *pInst);
static void setFolderStart(appInstData *pInst);
static void pullPbListStart(appInstData *pInst);
static void pullPbEntryStart(appInstData *pInst);
static void setAuthStart(appInstData *pInst);
static void abortStart(appInstData *pInst);
static void disconnectPacStart(appInstData *pInst);

static void handleKeybPrim(appInstData *pInst);
static CsrBool passkeyState(appInstData *pInst, CsrUint8 cKey);
static CsrBool addressState(appInstData *pInst, CsrUint8 cKey);
static CsrBool idleState(appInstData *pInst, CsrUint8 cKey);
static CsrBool connectState(appInstData *pInst, CsrUint8 cKey);
static CsrBool connectedState(appInstData *pInst, CsrUint8 cKey);
static CsrBool searchState(appInstData *pInst, CsrUint8 cKey);

static CsrBool filterState(appInstData *pInst, CsrUint8 cKey);
static void filterMenu(void);


static char *getPathStart(char *path);


static void pbScAppSetFnFini(void *pFiniData)
{
    appInstData *instData = (appInstData *) pFiniData;
    scAppRestoreState((CsrUint8 *) &instData->prevKbm, (CsrUint8 *) &instData->kbm);
    system_cls();
    playIntro(instData);
}

static void pbScAppSetFnInit(void *pInitData)
{
    appInstData *instData = (appInstData *) pInitData;
    scAppSaveState((CsrUint8 *) &instData->prevKbm, (CsrUint8 *) &instData->kbm, SECURITY_KEYBOARD_STATE);
}

static void* pbScAppSetFnGetMessage(void *pRecvMsgPData)
{
    appInstData *instData = (appInstData *) pRecvMsgPData;
    void *result          = instData->pRecvMsg;
    instData->pRecvMsg    = NULL;
    return result;
}

/*******************************************************************************
 *
 *   init function called by the scheduler upon initialisation. This
 *   function is used to boot the demo application by sending a
 *   request to bond with the headset. Bonding is not mandated
 *   according to the profile but is needed if encryption of the
 *   speech is required.
 *
 *******************************************************************************/
void CsrBtAppInit(void **gash)
{
    appInstData  *pInst;
    extern BD_ADDR_T defGlobalBdAddr;
    CsrBtDeviceAddr zeroAddr;

    *gash = CsrPmemAlloc(sizeof(appInstData));
    pInst = (appInstData*)*gash;
    pInst->state = IDLE_STATE;
    pInst->nDevice = 0;
    pInst->nDeviceInList = 0;
    pInst->bFullDeviceList = FALSE;
    pInst->pRecvMsg = NULL;
    pInst->bAuth = FALSE;
    pInst->pFile = NULL;
    pInst->nObjLen = 0;

    /* Authentication */
    pInst->kbm = NORMAL_STATE;
    pInst->szBdAddr[0] = 0;
    pInst->BdAddrLen = 0;
    pInst->passkey[0] = '\0';
    pInst->passkeyLength = 0;

    /* Default filter gets everything */
    pInst->filter[0] = 0xff;
    pInst->filter[1] = 0xff;
    pInst->filter[2] = 0xff;
    pInst->filter[3] = 0x1f;
    pInst->filter[4] = 0x00;
    pInst->filter[5] = 0x00;
    pInst->filter[6] = 0x00;
    pInst->filter[7] = 0x00;

    /* pull pb */
    pInst->src = CSR_BT_PAC_SRC_PHONE;
    pInst->nNameLen = 0;
    CsrBtBdAddrZero(&zeroAddr);
    if (!CsrBtBdAddrEq(&defGlobalBdAddr, &zeroAddr))
    {
        pInst->bdAddrList[pInst->nDevice].lap = defGlobalBdAddr.lap;
        pInst->bdAddrList[pInst->nDevice].nap = defGlobalBdAddr.nap;
        pInst->bdAddrList[pInst->nDevice].uap = defGlobalBdAddr.uap;
    }
    else
    {
        pInst->bdAddrList[pInst->nDevice].nap = 0;
        pInst->bdAddrList[pInst->nDevice].uap = 0;
        pInst->bdAddrList[pInst->nDevice].lap = 0;
    }

    playIntro(pInst);

    /* Support for usr2usr/usr2kernel splits */
    pInst->CsrSchedQid = TESTQUEUE;

    /* Security App Configuration */
    CsrBtScActivateReqSend(pInst->CsrSchedQid);
    scAppInitInstanceData(&pInst->scInstData);

    scAppSetFnFini(&pInst->scInstData, pbScAppSetFnFini, pInst);
    scAppSetFnInit(&pInst->scInstData, pbScAppSetFnInit, pInst);
    scAppSetSecLevelSend(&pInst->scInstData, scAppSendProfileSecurityLevelReq, &pInst->scInstData);

    scAppSetFnGetMessage(&pInst->scInstData, pbScAppSetFnGetMessage, pInst);

    scAppSetSecLevelPrim(&pInst->scInstData,
                         0,     /* Index 0. Single profile */
                         "PAC", CSR_BT_PAC_PRIM,
                         CSR_BT_PAC_IFACEQUEUE,
                         SC_APP_SET_SEC_LEVEL_NA, CSR_BT_PAC_SECURITY_OUT_REQ);

    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, NULL);
}

#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    appInstData *pInst = *gash;

    scAppDeinitInstanceData(&pInst->scInstData);

    /*  Get a message from the demoapplication message queue. The
     *  message is returned in prim and the event type in eventType */
    while(CsrSchedMessageGet(&pInst->event, &pInst->pRecvMsg) )
    {
        switch(pInst->event)
        {
            case CSR_BT_PAC_PRIM:
            {
                CsrBtPacFreeUpstreamMessageContents(pInst->event, pInst->pRecvMsg);
                break;
            }
            case CSR_BT_SC_PRIM:
            {
                CsrBtScFreeUpstreamMessageContents(pInst->event, pInst->pRecvMsg);
                break;
            }
            case CSR_BT_SD_PRIM:
            {
                CsrBtSdFreeUpstreamMessageContents(pInst->event, pInst->pRecvMsg);
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

/**************************************************************************************************
 *
 *   this is the demo application handler function. All primitives sent to the demo application will
 *   be received in here. The scheduler ensures that the function is activated when a signal is put
 *   on the demo application signal queue.
 *
 **************************************************************************************************/
void CsrBtAppHandler(void **gash)
{
    appInstData *pInst = *gash;

    /*  get a message from the demoapplication message queue. The message is
        returned in prim and the event type in eventType */
    if(CsrSchedMessageGet(&pInst->event, &pInst->pRecvMsg))
    {
        /*  two event types must be handled. SC for bonding and PAC for the
            connection and PB related signalling */
        switch (pInst->event)
        {
            case CSR_BT_SC_PRIM:
                handleScPrim(&pInst->scInstData);
                break;

            case CSR_BT_SD_PRIM:
                handleSdPrim(pInst);
                break;

            case CSR_BT_PAC_PRIM:
                {
                    CsrPrim *prim = (CsrPrim *) pInst->pRecvMsg;
                    switch(*prim)
                    {
                        case CSR_BT_PAC_SECURITY_OUT_CFM:
                            handleScSecLevelPrim(&pInst->scInstData);
                            break;

                        default:
                            handlePacPrim(pInst);
                            break;
                    }
                    break;
                }
            case KEY_MESSAGE:
                handleKeybPrim(pInst);
                break;

            default:
                /*  unexpected primitive received */
                printf("####### default in testhandler %x,\n", pInst->event);
                break;
        }
        if (NULL != pInst->pRecvMsg)
        {
            /*  free the received signal again. If the signal for some reason must be stored in the application
                the pointer (prim) must be set to NULL in order not to free it here */
            CsrPmemFree(pInst->pRecvMsg);
            pInst->pRecvMsg = NULL;
        }
    }
}


/* == LOCAL FUNCTIONS =============================================================================== */
static void playIntro(appInstData *pInst)
{
    system_cls();
    printf("\nCSR plc. -- Synergy BT Phonebook Access Client Demo\n\n");
    printf("Hit <ESC> to quit program!\n\n");

    printf("Options:\n\n");
    if(pInst->state == IDLE_STATE)
    {
        printf("  1) Search for Phonebook Access Server\n");
        printf("  2) Connect to server\n");
        printf("  7) Security Controller Configuration\n");

        printf("  b) Enter Bluetooth address\n");
        printf("  s) Set Authentication [%s]\n", pInst->bAuth ? "ON" : "OFF");
    }
    else if(pInst->state == CONNECT_STATE)
    {
        printf("  d) Cancel Connection attempt (stop scanning)\n");
    }
    else
    {
        printf("  3) download phonebook\n");
        printf("  4) set phonebook folder\n");
        printf("  5) download phonebook listing\n");
        printf("  6) download phonebook entry\n");

        printf("  a) Abort operation\n");
        printf("  d) disconnect (close connection)\n");
        printf("  f) Enter vcard filter\n");
        printf("  g) Clear vcard filter (clear all bits)\n");
        printf("  h) Set all non-reserved vcard filter bits\n");
    }
}

static void KeyMessageService(CsrUint16 m, void *data)
{
    CSR_UNUSED(m);
    CSR_UNUSED(data);
    if (_kbhit())
    {
        CsrBtKeyPress_t *key;
        key = CsrPmemAlloc(sizeof(CsrBtKeyPress_t));
        key->type = KEY_MESSAGE;
        key->key =_getch();
        CsrSchedMessagePut(TESTQUEUE, KEY_MESSAGE, key);
    }
    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, NULL);
}

/*************************  SD PRIMITIVES   ***************************/
/*******************************************************************************
 *
 *   function to handle all sd-primitives.
 *
 *******************************************************************************/

static void handleSdPrim(appInstData *pInst)
{
    CsrPrim *pMsg = pInst->pRecvMsg;

    switch (*pMsg)
    {
        case CSR_BT_SD_SEARCH_RESULT_IND:
            {
                CsrBtSdSearchResultInd *prim = (CsrBtSdSearchResultInd *) pInst->pRecvMsg;
                CsrUint16 nameLen;
                CsrUint8 *name;

                pInst->bdAddrList[pInst->nDeviceInList] = prim->deviceAddr;

                printf("\n%u: ", pInst->nDeviceInList);
                nameLen = CsrBtSdDecodeFriendlyName(prim->info, prim->infoLen, &name);
                if (nameLen > 0)
                {
                    printf("%s\n   ", name);
                    CsrPmemFree(name);
                }
                printf("%04X:%02X:%06X\n   ",
                    prim->deviceAddr.nap,
                    prim->deviceAddr.uap,
                    prim->deviceAddr.lap);
                printf("%s\n",CodDecodeDeviceClass(prim->deviceClass));

                pInst->nDeviceInList++;
                if ( pInst->nDeviceInList > MAX_DISCOVERY_RESULTS_IN_LIST - 1)
                {
                    /* Reached maximum result list entries. Cancelling search */
                    CsrBtSdCancelSearchReqSend(pInst->CsrSchedQid);
                    pInst->nDeviceInList = MAX_DISCOVERY_RESULTS_IN_LIST - 1;
                    pInst->bFullDeviceList = TRUE;
                }
                CsrPmemFree(prim->info);
                break;
            }
        case CSR_BT_SD_CLOSE_SEARCH_IND:
            {
                printf("\nClosed Search\n");
                if(!pInst->bFullDeviceList)
                {
                    printf(
                        "Selecting device with addr: %04X:%02X:%06X\n",
                        pInst->bdAddrList[pInst->nDevice].nap, pInst->bdAddrList[pInst->nDevice].uap,
                        pInst->bdAddrList[pInst->nDevice].lap
                        );
                    scAppSetBondingAddr(&pInst->scInstData,
                                        pInst->bdAddrList[pInst->nDevice]);
                    pInst->state = IDLE_STATE;
                }
                break;
            }
        default:
            {
                printf("####### Unhandled sdPrim: %x received\n", *pMsg);
                CsrBtSdFreeUpstreamMessageContents(CSR_BT_SD_PRIM, pInst->pRecvMsg);
                break;
            }
    }
}

/*************************  PAC PRIMITIVES  ***************************/
static void handleCsrBtPacConnectCfm(appInstData *pInst)
{
    CsrBtPacConnectCfm *pMsg = pInst->pRecvMsg;

    if ((CONNECT_STATE == pInst->state || AUTHENTICATING_STATE == pInst->state) &&
         pMsg->resultCode     == CSR_BT_OBEX_SUCCESS_RESPONSE_CODE                     &&
         pMsg->resultSupplier == CSR_BT_SUPPLIER_IRDA_OBEX)
    {
        pInst->state = CONNECTED_STATE;

        playIntro(pInst);

        printf("Connected to Server which has support for");
        if(pMsg->supportedRepositories & CSR_BT_PAC_SRC_PHONE)
        {
            printf(" telecom");
        }
        if(pMsg->supportedRepositories == (CSR_BT_PAC_SRC_SIM | CSR_BT_PAC_SRC_PHONE) )
        {
            printf(" and");
        }
        if(pMsg->supportedRepositories & CSR_BT_PAC_SRC_SIM)
        {
            printf(" SIM1");
        }
        if(!pMsg->supportedRepositories)
        {
            printf(" nothing!!");
        }

        printf("\n");
    }
    else
    {
        pInst->state = IDLE_STATE;

        playIntro(pInst);

        printf("Connection failed, resultCode: 0x%04X, resultSupplier: 0x%04X\n", pMsg->resultCode, pMsg->resultSupplier);
        pInst->passkeyLength = 0;
    }
}

static void handlePacAuthInd(appInstData *pInst)
{
    CsrBtPacAuthenticateInd *pMsg = (CsrBtPacAuthenticateInd *) pInst->pRecvMsg;

    if (pInst->passkeyLength)
    {
        if (pInst->state != AUTHENTICATING_STATE)
        {
            CsrUint8 *key;

            printf("Reuse OBEX passkey: %s.\n", pInst->passkey);
            pInst->state = AUTHENTICATING_STATE;
            key = CsrMemDup(pInst->passkey, pInst->passkeyLength);
            CsrBtPacAuthenticateResSend(key, pInst->passkeyLength, NULL);
        }
        else
        {
            printf("OBEX authentication failed with passkey: %s\n", pInst->passkey);
            pInst->state = IDLE_STATE;
            pInst->passkeyLength = 0;
            CsrBtPacAuthenticateResSend(NULL, 0, NULL);
        }
    }
    else
    {
        CsrBtDeviceAddr slaveDeviceAddr = pInst->bdAddrList[pInst->nDevice];

        printf( "Enter OBEX password for \"%04X:%02X:%06X\": ", slaveDeviceAddr.nap,
                slaveDeviceAddr.uap, slaveDeviceAddr.lap  );
        pInst->state = AUTHENTICATING_STATE ;
        pInst->kbm = PASSKEY_STATE;
        pInst->passkeyLength = 0;
        pInst->passkey[0] = 0;
    }
    CsrPmemFree(pMsg->realm);
}

static void handleCsrBtPacPullPbCfm(appInstData *pInst)
{


    if (CONNECTED_STATE == pInst->state)
    {
        CsrBtPacPullPbCfm *pMsg = pInst->pRecvMsg;

        if (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE == pMsg->responseCode)
        {
            if (NULL == pInst->pFile)
            {
                /* empty pb downloaded successful, we create file anyway */
                pInst->pFile = CsrBtFopen(pInst->name, "wb");
            }
            printf("\nPhonebook download complete!\n");
            printf("\tSize of Pb: %d entries (NB: always 0 if CSRMAX entries was different from 0)\n", pMsg->pbSize);
            printf("\tNumber of missed calls: %u\n", pMsg->newMissedCall);
            printf("\tNumber of bytes downloaded: %d\n", pInst->nObjLen);
            printf("\tDownloaded phonebook is stored in file: %s\n", pInst->name);
        }
        else
        {
            printf("Error occurred. OBEX response code: 0x%04x\n", pMsg->responseCode);
        }
    }
    if (NULL != pInst->pFile)
    {
        CsrBtFclose(pInst->pFile);
        pInst->pFile = NULL;
    }
}

static void handleCsrBtPacPullPbInd(appInstData *pInst)
{
    CsrBtPacPullPbInd *pMsg = pInst->pRecvMsg;

    if (CONNECTED_STATE == pInst->state)
    {
        CsrUint32 nWrite;

        if (NULL == pInst->pFile)
        {
            /* first indication, create file */
            pInst->pFile = CsrBtFopen(pInst->name, "wb");
        }

        nWrite = CsrBtFwrite(pMsg->bodyOffset+pMsg->payload, sizeof(CsrUint8), pMsg->bodyLength, pInst->pFile);
        if (nWrite != pMsg->bodyLength)
        {
            /* file error */
            printf("File error, could not write to file!\n");
        }

        pInst->nObjLen += pMsg->bodyLength;
        printf("%d Bytes downloaded, total downloaded: %d Bytes\n", pMsg->bodyLength, pInst->nObjLen);
        CsrBtPacPullPbResSend(FALSE);
    }
    if (NULL != pMsg->payload)
    {
        CsrPmemFree(pMsg->payload);
        pMsg->payload = NULL;
    }
}

static void handleSetFolderCfm(appInstData *pInst)
{
    CsrBtPacSetFolderCfm *pMsg = pInst->pRecvMsg;

    if (CONNECTED_STATE == pInst->state)
    {
        if (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE == pMsg->responseCode)
        {
            printf("Folder set successful!\n\n");
        }
        else
        {
            printf("Error occurred. Obex response code: 0x%04X\n\n", pMsg->responseCode);
        }
    }
}

static void handlePullvCardListCfm(appInstData *pInst)
{
    CsrBtPacPullVcardListCfm *pMsg = pInst->pRecvMsg;

    if (CONNECTED_STATE == pInst->state)
    {
        if (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE == pMsg->responseCode)
        {
            printf("\nV-card listing downloaded complete\n");
            printf("Phonebook size: %u\n", pMsg->pbSize);
            printf("New missed calls: %u\n", pMsg->newMissedCall);
            printf("Number of bytes downloaded: %d\n", pInst->nObjLen);
            printf("Listing stored in file: %s\n", pInst->name);
        }
        else
        {
            printf("Error occurred. Obex response code: 0x%04X\n\n", pMsg->responseCode);
        }
    }
    if (NULL != pInst->pFile)
    {
        CsrBtFclose(pInst->pFile);
        pInst->pFile = NULL;
    }
}

static void handlePullvCardListInd(appInstData *pInst)
{
    CsrBtPacPullVcardListInd *pMsg = pInst->pRecvMsg;

    if (CONNECTED_STATE == pInst->state)
    {
        CsrUint32 nWrite;

        /* first indication, create file */
        if (NULL == pInst->pFile)
        {
            const char *ext = ".vcf";
            const char *dflt = PB_DEFAULT_FOLDER_NAME;

            /* ensure that the name do not include a path.
             * No path must be include since PullvCardEntry uses releatib path information.
             * But just in any case, we still check and do only use the name is a path is inlcuded. */

            CsrStrCpy(pInst->name, getPathStart(pInst->name));

            if ('.' == pInst->name[0] || 0 == pInst->name[0])
            {
                /* create a valid file name */
                CsrStrCpy(pInst->name, dflt);
            }

            CsrStrCat(pInst->name, ext);

            pInst->pFile = CsrBtFopen(pInst->name, "wb");

            if (NULL == pInst->pFile)
            {
                printf("File error, could not open the file named: %s\n",pInst->name);

                if (NULL != pMsg->payload)
                {
                    CsrPmemFree(pMsg->payload);
                    pMsg->payload = NULL;
                }
                return;
            }
        }

        nWrite = CsrBtFwrite(pMsg->bodyOffset + pMsg->payload,
                           sizeof(CsrUint8),
                           pMsg->bodyLength,
                           pInst->pFile);

        if (nWrite != pMsg->bodyLength)
        {
            printf("File error, could not write to file!\n");
        }
        pInst->nObjLen += pMsg->bodyLength;
        CsrBtPacPullVcardListResSend(FALSE);
    }
    if (NULL != pMsg->payload)
    {
        CsrPmemFree(pMsg->payload);
        pMsg->payload = NULL;
    }
}

static void handlePullvCardEntryCfm(appInstData *pInst)
{
    CsrBtPacPullVcardEntryCfm *pMsg = pInst->pRecvMsg;

    if (CONNECTED_STATE == pInst->state)
    {
        if (CSR_BT_OBEX_SUCCESS_RESPONSE_CODE == pMsg->responseCode)
        {
            printf("V-Card Entry downloaded complete\n");
            printf("Number of bytes downloaded: %d\n", pInst->nObjLen);
            printf("Downloaded entry is stored in file: %s\n",pInst->name);
        }
        else
        {
            printf("Error occurred. Obex response code: 0x%04X\n\n", pMsg->responseCode);
        }
    }
    if (NULL != pInst->pFile)
    {
        CsrBtFclose(pInst->pFile);
        pInst->pFile = NULL;
    }
}

static void handlePullvCardEntryInd(appInstData *pInst)
{
    CsrBtPacPullVcardEntryInd *pMsg = pInst->pRecvMsg;

    if (CONNECTED_STATE == pInst->state)
    {
        CsrUint32 nWrite;

        /* first indication, create file */
        if (NULL == pInst->pFile)
        {
            CsrStrCpy(pInst->name, getPathStart(pInst->name));

            pInst->pFile = CsrBtFopen(pInst->name, "wb");
        }
        nWrite = CsrBtFwrite(
            pMsg->bodyOffset + pMsg->payload, sizeof(CsrUint8),
            pMsg->bodyLength, pInst->pFile
            );
        if (nWrite != pMsg->bodyLength)
        {   /* file error */
            printf("File error, could not write to file!\n");
        }
        pInst->nObjLen += pMsg->bodyLength;
        printf("%d Bytes downloaded, total downloaded: %d Bytes\n", pMsg->bodyLength, pInst->nObjLen);
        CsrBtPacPullVcardEntryResSend(FALSE);
    }
    if (NULL != pMsg->payload)
    {
        CsrPmemFree(pMsg->payload);
        pMsg->payload = NULL;
    }
}

static void handlePacAbort(appInstData *pInst)
{
    printf("Server confirmed abort.\n\n");
    if (pInst->pFile)
    {
        CsrBtFclose(pInst->pFile);
        pInst->pFile = NULL;
    }
    playIntro(pInst);
}

static void handlePacDisconnect(appInstData *pInst)
{
    printf("Disconnected!\n\n");
    pInst->state = IDLE_STATE;
    playIntro(pInst);
    pInst->passkeyLength = 0;
    if (pInst->pFile)
    {
        CsrBtFclose(pInst->pFile);
        pInst->pFile = NULL;
    }
}

/*******************************************************************************
 *
 *   function to handle all pac-primitives.
 *
 *******************************************************************************/
static void handlePacPrim(appInstData *pInst)
{
    CsrBtPacPrim *pMsg = pInst->pRecvMsg;

    switch (*pMsg)
    {
        case CSR_BT_PAC_CONNECT_CFM:
            handleCsrBtPacConnectCfm(pInst);
            break;

        case CSR_BT_PAC_AUTHENTICATE_IND:
            handlePacAuthInd(pInst);
            break;

        case CSR_BT_PAC_PULL_PB_CFM:
            handleCsrBtPacPullPbCfm(pInst);
            break;

        case CSR_BT_PAC_PULL_PB_IND:
            handleCsrBtPacPullPbInd(pInst);
            break;

        case CSR_BT_PAC_SET_FOLDER_CFM:
            /* Fall through */
        case CSR_BT_PAC_SET_BACK_FOLDER_CFM:
            /* Fall through */
        case CSR_BT_PAC_SET_ROOT_FOLDER_CFM:
            handleSetFolderCfm(pInst);
            break;

        case CSR_BT_PAC_PULL_VCARD_LIST_CFM:
            handlePullvCardListCfm(pInst);
            break;

        case CSR_BT_PAC_PULL_VCARD_LIST_IND:
            handlePullvCardListInd(pInst);
            break;

        case CSR_BT_PAC_PULL_VCARD_ENTRY_CFM:
            handlePullvCardEntryCfm(pInst);
            break;

        case CSR_BT_PAC_PULL_VCARD_ENTRY_IND:
            handlePullvCardEntryInd(pInst);
            break;

        case CSR_BT_PAC_ABORT_CFM:
            handlePacAbort(pInst);
            break;

        case CSR_BT_PAC_DISCONNECT_IND:
            handlePacDisconnect(pInst);
            break;

        default:
            printf("####### Unhandled pacPrim: %x received\n", *pMsg);
            CsrBtPacFreeUpstreamMessageContents(CSR_BT_PAC_PRIM, pInst->pRecvMsg);
            break;
    }
}

/*************************  KBM PRIMITIVES  ***************************/

/*******************************************************************************
 *
 *   function to handle all keyboard presses (sent as a keyboard event signal).
 *
 *******************************************************************************/
static void searchStart(appInstData *pInst)
{
    /* Find CSRMAX MAX_DISCOVERY_RESULTS_IN_LIST devices */
    pInst->nDeviceInList = 0;
    pInst->bFullDeviceList = FALSE;

    CsrBtSdSearchReqSend(pInst->CsrSchedQid,
                  CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                  6000, /* RSSI buffer timeout */
                  CSR_BT_INFINITE_TIME, /* Search timeout */
                  CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE, /* RSSI threshold */
                  CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device */
                  CSR_BT_OBJECT_TRANSFER_MAJOR_SERVICE_MASK, /* Class of Device mask */
                  CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                  0, /* Filter length */
                  NULL); /* Filter */

    printf("\nSearch in progress. Please wait .....\n");
}

static void pullPbStart(appInstData *pInst)
{
    printf("\nSelect source (1. Phone, 2. SIM [default:phone]):\t");
    pInst->kbm = PB_SOURCE_STATE;
    pInst->nObjLen = 0;
    if (NULL != pInst->pFile)
    {
        CsrBtFclose(pInst->pFile);
        pInst->pFile = NULL;
    }
}

static CsrBool pbSourceState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool bProcessed = FALSE;

    if (cKey == '1' || cKey == RETURN_KEY)
    {
        pInst->src = CSR_BT_PAC_SRC_PHONE;
        bProcessed = TRUE;
    }
    else if (cKey == '2')
    {
        pInst->src = CSR_BT_PAC_SRC_SIM;
        bProcessed = TRUE;
    }
    printf("%c", cKey);
    if (bProcessed)
    {
        pInst->kbm = PB_NAME_STATE;
        pInst->nNameLen = 0;
        CsrMemSet(pInst->name, 0, sizeof(pInst->name));
        printf("\nEnter name of phonebook:\n"
            "\texamples:\n"
            "\t\"pb.vcf\"  = main phone book (default if nothing is entered)\n"
            "\t\"ich.vcf\" = incoming calls history\n"
            "\t\"och.vcf\" = outgoing calls history\n"
            "\t\"mch.vcf\" = missed calls history\n"
            "\t\"cch.vcf\" = combined calls history\n"
            ":\t");
    }
    return bProcessed;
}

static CsrBool pbNameState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool bName = TRUE;

    if (RETURN_KEY == cKey)
    {
        if (pInst->name[0] == '\0')
        {
            sprintf(pInst->name, "%s", "pb.vcf");
            pInst->nNameLen = 6;
        }
        else
        {
            pInst->name[pInst->nNameLen] = '\0';
        }
        printf("\n");
        pInst->kbm = PB_ENTRIES_STATE;
        pInst->nEntriesLen = 0;
        printf("Enter max. number of entries you want (0 - only returns the selected PBs size) (default: 10):\t");
    }
    else if (BACKSPACE_KEY == cKey)
    {
        if (pInst->nNameLen > 0)
        {
            pInst->nNameLen--;
            printf("\b \b");
        }
    }
    else if ((cKey >= 32) && (cKey < 127))
    {
        if (pInst->nNameLen < CSR_BT_MAX_PATH_LENGTH)
        {
            pInst->name[pInst->nNameLen++] = cKey;
            printf("%c", cKey);
        }
    }
    else
    {
        bName = FALSE;
    }
    return bName;
}

static CsrBool pbEntriesState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool bProcessed = TRUE;

    if (RETURN_KEY == cKey)
    {
        CsrUcs2String *pName = NULL;
        CsrUint32 nNameLen;
        CsrUint16 nMaxSize;

        if(pInst->nEntriesLen == 0)
        {
            sprintf(pInst->szEntries, "10");
            pInst->nEntriesLen = 2;
        }
        else
        {
            pInst->szEntries[pInst->nEntriesLen] = '\0';
        }
        printf("\n");
        pInst->kbm = NORMAL_STATE;

        nNameLen = CsrStrLen(pInst->name)*2 + 2;
        if(nNameLen > 2)
        {
            pName = CsrUtf82Ucs2ByteString((CsrUint8*)pInst->name);
        }

        sscanf(pInst->szEntries, "%hu", &nMaxSize);
        CsrBtPacPullPbReqSend(pName,
                         pInst->src,
                         (CsrUint8*)pInst->filter,
                         CSR_BT_PAC_FORMAT_VCARD2_1,
                         nMaxSize,
                         0,
                         FALSE);
        printf("\nDownloading phonebook. Please wait ...!\n");
    }
    else if (BACKSPACE_KEY == cKey)
    {
        if (pInst->nEntriesLen> 0)
        {
            pInst->nEntriesLen--;
            printf("\b \b");
        }
    }
    else if (cKey >= '0' && cKey <= '9')
    {
        if (pInst->nEntriesLen< MAX_ENTRIES)
        {
            pInst->szEntries[pInst->nEntriesLen++] = cKey;
            printf("%c", cKey);
        }
    }
    else
    {
        bProcessed = FALSE;
    }

    return bProcessed;
}

static void setFolderStart(appInstData *pInst)
{
    printf("Enter new path (["PATH_SEP"] for root (default), [..] for back):\t");
    pInst->kbm = FOLDER_NAME_STATE;
    pInst->nPathLen = 0;
    CsrMemSet(pInst->path, 0, sizeof(pInst->path));
}

static CsrBool folderNameState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool bProcessed = TRUE;

    if (RETURN_KEY == cKey)
    {
        pInst->path[pInst->nPathLen] = '\0';
        printf("\n");
        pInst->kbm = NORMAL_STATE;
        if (pInst->path[0] == *PATH_SEP || pInst->nPathLen == 0)
        {
            printf("\nRequest path set to root.\n");
            CsrBtPacSetRootFolderReqSend();
        }
        else if ('.' == pInst->path[0] && '.' == pInst->path[1])
        {
            printf("\nRequest path set one step back.\n");
            CsrBtPacSetBackFolderReqSend();
        }
        else
        {
            CsrUcs2String *pFolder = NULL;
            CsrUint32 nFolderLen;

            printf("\nRequest path set to: %s\n", pInst->path);

            nFolderLen = CsrStrLen(pInst->path)*2 + 2;
            if (nFolderLen > 2)
            {
                pFolder = CsrUtf82Ucs2ByteString((CsrUint8*)pInst->path);
            }
            CsrBtPacSetFolderReqSend(pFolder);
        }
    }
    else if (BACKSPACE_KEY == cKey)
    {
        if (pInst->nPathLen> 0)
        {
            pInst->nPathLen--;
            printf("\b \b");
        }
    }
    else if ((cKey >= 32) && (cKey < 127))
    {
        if (pInst->nPathLen < CSR_BT_MAX_PATH_LENGTH)
        {
            pInst->path[pInst->nPathLen++] = cKey;
            printf("%c", cKey);
        }
    }
    else
    {
        bProcessed = FALSE;
    }
    return bProcessed;
}

static void pullPbListStart(appInstData *pInst)
{
    printf("Enter folder name:\n"
        "\texamples:\n"
        "\t\"pb\" = main phone book\n"
        "\t\"ich\" = incoming calls history\n"
        "\t\"och\" = outgoing calls history\n"
        "\t\"mch\" = missed calls history\n"
        "\t\"cch\" = combined calls history\n"
        "\t\"\" = listing of the folder which is the current directory\n:");
    pInst->kbm = VLIST_NAME_STATE;
    CsrMemSet(pInst->name, 0, sizeof(pInst->name));
    pInst->nNameLen = 0;
    pInst->nObjLen = 0;
}

static CsrBool vlistNameState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool bProcessed = TRUE;

    if (RETURN_KEY == cKey)
    {
        pInst->name[pInst->nNameLen] = '\0';
        printf("\n");
        pInst->kbm = VLIST_ORDER_STATE;
        printf("Enter sorting order [a]lphabetic (default), [i]ndexed or [p]honetical:\t");
    }
    else if (BACKSPACE_KEY == cKey)
    {
        if (pInst->nNameLen> 0)
        {
            pInst->nNameLen--;
            printf("\b \b");
        }
    }
    else if ((cKey >= 32) && (cKey < 127))
    {
        if (pInst->nNameLen < CSR_BT_MAX_PATH_LENGTH)
        {
            pInst->name[pInst->nNameLen++] = cKey;
            printf("%c", cKey);
        }
    }
    else
    {
        bProcessed = FALSE;
    }
    return bProcessed;
}

static CsrBool vlistOrderState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool bProcessed = TRUE;

    if ('i' == cKey || 'I' == cKey)
    {
        pInst->order = CSR_BT_PAC_ORDER_INDEXED;
    }
    else if ('p' == cKey || 'P' == cKey)
    {
        pInst->order = CSR_BT_PAC_ORDER_PHONETICAL;
    }
    else
    {
        pInst->order = CSR_BT_PAC_ORDER_ALPHABETICAL; /* default */
    }
    printf("%c", cKey);

    if (bProcessed)
    {
        printf("\nEnter which attribute to search on ([n]ame (default), n[u]mber or [s]ound):\t");
        pInst->kbm = VLIST_SEARCH_ATT_STATE;
    }
    return bProcessed;
}

static CsrBool vlistSearchAttState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool bProcessed = TRUE;

    if ('u' == cKey || 'U' == cKey)
    {
        pInst->searchAtt = CSR_BT_PAC_SEARCH_ATT_NUMBER;
    }
    else if ('s' == cKey || 'S' == cKey)
    {
        pInst->searchAtt = CSR_BT_PAC_SEARCH_ATT_SOUND;
    }
    else
    {
        pInst->searchAtt = CSR_BT_PAC_SEARCH_ATT_NAME;
    }
    printf("%c", cKey);

    if (bProcessed)
    {
        printf("\nEnter Search string to selected attribute (a \'*\' means all entries (default)):\t");
        pInst->kbm = VLIST_SEARCH_VAL_STATE;
        pInst->nSearchLen = 0;
    }
    return bProcessed;
}

static CsrBool vlistSearchValState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool bProcessed = TRUE;

    if (RETURN_KEY == cKey)
    {
        if(pInst->nSearchLen == 0)
        {
            pInst->search[0] = '*';
            pInst->nSearchLen = 1;
        }
        else
        {
            pInst->search[pInst->nSearchLen] = '\0';
        }
        printf("\n");
        pInst->kbm = VLIST_ENTRIES_STATE;
        printf("Enter max. number of entries in listing (0 for size only) (default value: 10):\t");
        pInst->nEntriesLen = 0;
    }
    else if (BACKSPACE_KEY == cKey)
    {
        if (pInst->nSearchLen > 0)
        {
            pInst->nSearchLen--;
            printf("\b \b");
        }
    }
    else if ((cKey >= 32) && (cKey < 127))
    {
        if (pInst->nSearchLen < MAX_SEARCH)
        {
            pInst->search[pInst->nSearchLen++] = cKey;
            printf("%c", cKey);
        }
    }
    else
    {
        bProcessed = FALSE;
    }
    return bProcessed;
}

static CsrBool vlistEntriesState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool bProcessed = TRUE;

    if (RETURN_KEY == cKey)
    {
        if(pInst->nEntriesLen == 0)
        {
            CsrStrCpy(pInst->szEntries, "10");
            pInst->nEntriesLen = 2;
        }
        else
        {
            pInst->szEntries[pInst->nEntriesLen] = '\0';
        }
        printf("\n");
        pInst->kbm = VLIST_OFFSET_STATE;
        printf("Enter entry start offset (default: 0):\t");
        pInst->nOffsetLen = 0;
    }
    else if (BACKSPACE_KEY == cKey)
    {
        if (pInst->nEntriesLen> 0)
        {
            pInst->nEntriesLen--;
            printf("\b \b");
        }
    }
    else if (cKey >= '0' && cKey <= '9')
    {
        if (pInst->nEntriesLen< MAX_ENTRIES)
        {
            pInst->szEntries[pInst->nEntriesLen++] = cKey;
            printf("%c", cKey);
        }
    }
    else
    {
        bProcessed = FALSE;
    }
    return bProcessed;
}

static CsrBool vlistOffsetState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool bProcessed = TRUE;

    if (RETURN_KEY == cKey)
    {
        CsrUint32 nTmp = 0xffff;
        CsrUint16 nStartOffset;
        CsrUcs2String *pName = NULL, *pSearch;

        if(pInst->nEntriesLen == 0)
        {
            CsrStrCpy(pInst->szOffset, "0");
            pInst->nOffsetLen = 1;
        }
        else
        {
            pInst->szOffset[pInst->nOffsetLen] = '\0';
        }
        printf("\n");
        pInst->kbm = NORMAL_STATE;

        sscanf(pInst->szEntries, "%u", &nTmp);
        pInst->nMaxLstCnt = (CsrUint16)nTmp;
        pInst->nEntriesLen = 0;

        nTmp = 0;
        sscanf(pInst->szOffset,"%u", &nTmp);
        nStartOffset = (CsrUint16)nTmp;

        if (('.' == pInst->name[0] && 0 == pInst->name[1]) || 0 == pInst->nNameLen)
        {
            pName = NULL; /* no search list all entries */
        }
        else
        {
            CsrUint32 nNameLen;

            nNameLen = CsrStrLen(pInst->name)*2 + 2;
            if(nNameLen > 2)
            {
                pName = CsrUtf82Ucs2ByteString((CsrUint8*)pInst->name);
            }
        }

        if (0 == pInst->search[0] || '*' == pInst->search[0])
        {
            pSearch = NULL; /* don't use searchVal */
        }
        else
        {

            pSearch = CsrPmemAlloc(CsrStrLen(pInst->search)+1);
            CsrStrCpy((char *) pSearch, pInst->search);
        }

        CsrBtPacPullVcardListReqSend(pName,
                                pInst->order,
                                pSearch,
                                pInst->searchAtt,
                                pInst->nMaxLstCnt,
                                nStartOffset,
                                FALSE);
    }
    else if (BACKSPACE_KEY == cKey)
    {
        if (pInst->nOffsetLen > 0)
        {
            pInst->nOffsetLen--;
            printf("\b \b");
        }
    }
    else if (cKey >= '0' && cKey <= '9')
    {
        if (pInst->nOffsetLen < MAX_ENTRIES)
        {
            pInst->szOffset[pInst->nOffsetLen++] = cKey;
            printf("%c", cKey);
        }
    }
    else
    {
        bProcessed = FALSE;
    }
    return bProcessed;
}

static void pullPbEntryStart(appInstData *pInst)
{
    pInst->nObjLen = 0;
    printf("Enter entry name:\n"
        "\texamples:\n"
        "\t\"pb.vcf\" = main phone book\n"
        "\t\"0.vcf\" = owners card (only allowed in telecom/pb or sim1/telecom/pb folder!)\n"
        "\t\"xxx.vcf\" = anyother vCard entry listing obtained from the phonebook listing\n"
        ":\t");
    pInst->nNameLen = 0;
    pInst->kbm = VCARD_NAME_STATE;
}

static CsrBool vcardNameState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool bProcessed = TRUE;

    if (RETURN_KEY == cKey)
    {
        CsrUcs2String *pName = NULL;
        CsrUint32 nNameLen;

        if(pInst->nNameLen == 0)
        {
            printf("\nInvalid selection: you must specify an entry name to download\n");
            return TRUE;
        }

        pInst->name[pInst->nNameLen] = '\0';
        printf("\n");
        pInst->kbm = NORMAL_STATE;

        nNameLen = CsrUcs2ByteStrLen((CsrUcs2String*)pInst->name);
        if (nNameLen > 0)
        {
            pName = CsrUtf82Ucs2ByteString((CsrUint8*)pInst->name);
        }

        CsrBtPacPullVcardEntryReqSend(pName, (CsrUint8*)pInst->filter, CSR_BT_PAC_FORMAT_VCARD2_1, FALSE);
        printf("Requesting entry: %s ...\n", pInst->name);
    }
    else if (BACKSPACE_KEY == cKey)
    {
        if (pInst->nNameLen> 0)
        {
            pInst->nNameLen--;
            printf("\b \b");
        }
    }
    else if ((cKey >= 32) && (cKey < 127))
    {
        if (pInst->nNameLen < CSR_BT_MAX_PATH_LENGTH)
        {
            pInst->name[pInst->nNameLen++] = cKey;
            printf("%c", cKey);
        }
    }
    else
    {
        bProcessed = FALSE;
    }
    return bProcessed;
}

static void setAuthStart(appInstData *pInst)
{
    if (pInst->bAuth)
    {
        printf("Authentication is currently ON\n");
    }
    else
    {
        printf("Authentication is currently OFF\n");
    }
    printf("Set Authentication On[1]/Off[0]: ");
    scanf("%hhu", &(pInst->bAuth));
    playIntro(pInst);
}

static void abortStart(appInstData *pInst)
{
    CSR_UNUSED(pInst);
    printf("Abort all operation.\n\n");
    CsrBtPacAbortReqSend();
}

static void disconnectPacStart(appInstData *pInst)
{
    if(pInst->state != CONNECT_STATE)
    {
        CsrBtPacDisconnectReqSend(TRUE);
        printf("\nDisconnecting from server. Please wait .... \n");
    }
    else
    {
        CsrBtPacCancelConnectReqSend();
        printf("\nCancelling connection attempt. Please wait .... \n");
    }
    pInst->state = IDLE_STATE;
    if (pInst->pFile)
    {
        CsrBtFclose(pInst->pFile);
        pInst->pFile = NULL;
    }
}


/*******************************************************************************
 *
 *   a keyboard event is received.
 *   first check for selection of a found device during inquiry if state is
 *   inquiry. If not start proper action as selected.
 *
 *******************************************************************************/
static void handleKeybPrim(appInstData *pInst)
{
    CsrBtKeyPress_t *key;
    CsrBool bProcessed;
    key = (CsrBtKeyPress_t*)pInst->pRecvMsg;

    if( key->key == ESCAPE_KEY )
    {
        printf("\nUser exit...\n");
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif
        return;
    }

    switch (pInst->kbm)
    {
        case PASSKEY_STATE:
            bProcessed = passkeyState(pInst, key->key);
            break;

        case SECURITY_KEYBOARD_STATE:
            handleScKeybPrim(&pInst->scInstData);
            bProcessed = TRUE;
            break;

        case ADDRESS_STATE:
            bProcessed = addressState(pInst, key->key);
            break;

        case FILTER_STATE:
            bProcessed = filterState(pInst, key->key);
            break;

        case PB_SOURCE_STATE:
            bProcessed = pbSourceState(pInst, key->key);
            break;

        case PB_NAME_STATE:
            bProcessed = pbNameState(pInst, key->key);
            break;

        case PB_ENTRIES_STATE:
            bProcessed = pbEntriesState(pInst, key->key);
            break;

        case FOLDER_NAME_STATE:
            bProcessed = folderNameState(pInst, key->key);
            break;

        case VLIST_NAME_STATE:
            bProcessed = vlistNameState(pInst, key->key);
            break;

        case VLIST_ORDER_STATE:
            bProcessed = vlistOrderState(pInst, key->key);
            break;

        case VLIST_SEARCH_ATT_STATE:
            bProcessed = vlistSearchAttState(pInst, key->key);
            break;

        case VLIST_SEARCH_VAL_STATE:
            bProcessed = vlistSearchValState(pInst, key->key);
            break;

        case VLIST_ENTRIES_STATE:
            bProcessed = vlistEntriesState(pInst, key->key);
            break;

        case VLIST_OFFSET_STATE:
            bProcessed = vlistOffsetState(pInst, key->key);
            break;

        case VCARD_NAME_STATE:
            bProcessed = vcardNameState(pInst, key->key);
            break;

        default:
            switch(pInst->state)
            {
                case IDLE_STATE:
                    bProcessed = idleState(pInst, key->key);
                    break;

                case CONNECT_STATE:
                    bProcessed = connectState(pInst, key->key);
                    break;

                case CONNECTED_STATE:
                    bProcessed = connectedState(pInst, key->key);
                    break;

                case SEARCHING_STATE:
                    bProcessed = searchState(pInst, key->key);
                    break;

                default:
                    bProcessed = FALSE;
                    break;
            }
    }
    /* any state */
    if (!bProcessed)
    {
        /*  unexpected primitive received */
        playIntro(pInst);
        printf("####### default in keybPrim handler %x,\n", key->key);
    }
}

static void startConnect(appInstData *pInst)
{
    CsrBtDeviceAddr slaveDeviceAddr = pInst->bdAddrList[pInst->nDevice];

    /* Use bluetooth passkey from bonding obex authentication */
    CsrBtPacConnectReqSend(pInst->CsrSchedQid,
                           PAC_MAX_OBEX_PACKET_SIZE,
                           slaveDeviceAddr,
                           pInst->bAuth,
                           0, NULL,
                           (CsrUint16)CsrStrLen((char *) pInst->passkey),
                           (CsrUint8 *) CsrStrDup((char *) pInst->passkey),
                           NULL,
                           0, /*length*/
                           0, /*count*/
                           0, /*windowSize*/
                           TRUE); /*enableSrm*/

    pInst->state = CONNECT_STATE;
    playIntro(pInst);
    printf("Connecting to PBAP Server. Please wait .... \n");
}

static CsrBool passkeyState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool isPasskey = TRUE;

    if (RETURN_KEY == cKey)
    {
        pInst->passkey[pInst->passkeyLength] = '\0';
        printf("\n");
        pInst->kbm = NORMAL_STATE;

        switch (pInst->state)
        {
            case CONNECT_INIT_STATE:
                    startConnect(pInst);
                break;
            case AUTHENTICATING_STATE:
            case CONNECT_STATE:
            {
                CsrUint8 *key;

                key = CsrMemDup(pInst->passkey, pInst->passkeyLength);

                CsrBtPacAuthenticateResSend(key, pInst->passkeyLength, NULL);
            }

            default:
                printf("Unknown state: %d\n", pInst->state);
                break;
        }
    }
    else if (BACKSPACE_KEY == cKey)
    {
        if (pInst->passkeyLength > 0)
        {
            pInst->passkeyLength--;
            printf("\b \b");
        }
    }
    else if ((cKey >= 32) && (cKey < 127))
    {
        if (pInst->passkeyLength < CSR_BT_PASSKEY_MAX_LEN)
        {
            pInst->passkey[pInst->passkeyLength++] = cKey;
            printf("%c", cKey);
        }
    }
    else
    {
        isPasskey = FALSE;
    }
    return isPasskey;
}

#define VCARD_VERSION   (1 << 0)
#define VCARD_N  (1 << 2) /* N Structured Presentation of Name) */
#define VCARD_TEL   (1 << 7)
#define VCARD_EMAIL (1 << 8)
#define VCARD_TITLE (1 << 12)
#define VCARD_ROLE  (1 << 13)

static CsrBool filterState(appInstData *pInst, CsrUint8 cKey)
{
    CsrUint32    val, tmpval;
    CsrUint8     offset = 0; /* offset into pInst->filter[] */
    char        *s;

    pInst->kbm = NORMAL_STATE;

    /*
     * When you change this, REMEMBER
     * to sync with filterMenu() below.
     */

    switch (cKey) {
    case 'e':
    case 'E':
        val = VCARD_EMAIL;
        s = "email";
        break;

    case 'n':
    case 'N':
        val = VCARD_N;
        s = "name";
        break;

    case 'p':
    case 'P':
        val = VCARD_TEL;
        s = "phone number";
        break;

    case 'r':
    case 'R':
        val = VCARD_ROLE;
        s = "role";
        break;

    case 't':
    case 'T':
        val = VCARD_TITLE;
        s = "title";
        break;

    case 'v':
    case 'V':
        val = VCARD_VERSION;
        s = "VCARD version";
        break;

    default:
        printf("Invalid filter key.\n");
        return TRUE;
    }

    /*
     * Find offset into pInst->filter[] and flip
     * the appropriate bit in that 8 bit quantity.
     * Find offset by shifting value bitpattern 8
     * positions to the right.  While the result
     * is positive (only one bit set), the
     * pInst->filter offset is too small.
     *
     * Note that while CsrUint32 is really too small
     * to store all bits, bits 29 through 63 are
     * reserved and not used for qualification tests.
     */
    tmpval = val;
    while (tmpval >> 8) {
        tmpval >>= 8;
        offset++;
    }
    val >>= 8 * offset;

    /* Now flip/xor the bit. */
    pInst->filter[offset] ^= val;

    playIntro(pInst);
    printf("`%s' bit flipped, now %d.\n", s,
        (pInst->filter[offset] & val) ? 1 : 0);

    return TRUE;
}

static void filterMenu(void)
{
    printf("VCARD filter options\n\n"
        "  e) email (EMAIL)\n"
        "  n) name (N, Structured Presentation of Name)\n"
        "  p) phone number (TEL)\n"
        "  r) organization role (ROLE)\n"
        "  t) title (TITLE)\n"
        "  v) VCARD version no. (VERSION)\n\n");

}
static CsrBool addressState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool isAddr = TRUE;

    if (RETURN_KEY == cKey)
    {
        CsrUint32 n, u, l;
        pInst->szBdAddr[pInst->BdAddrLen] = '\0';
        printf("\n");
        pInst->kbm = NORMAL_STATE;
        pInst->nDevice = 0;
        sscanf(pInst->szBdAddr, "%04X:%02X:%06X", &n, &u, &l);
        pInst->bdAddrList[pInst->nDevice].nap = (CsrUint16)n;
        pInst->bdAddrList[pInst->nDevice].uap = (CsrUint8)u;
        pInst->bdAddrList[pInst->nDevice].lap = (CsrUint24)l;
    }
    else if (BACKSPACE_KEY == cKey)
    {
        if (pInst->BdAddrLen > 0)
        {
            pInst->BdAddrLen--;
            printf("\b \b");
        }
    }
    else if ((cKey >= 32) && (cKey < 127))
    {
        if (pInst->BdAddrLen < 15)
        {
            pInst->szBdAddr[pInst->BdAddrLen++] = cKey;
            printf("%c", cKey);
        }
    }
    else
    {
        isAddr = FALSE;
    }
    return isAddr;
}

static CsrBool idleState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool bProcessed = TRUE;

    switch( cKey)
    {
        case '1': /* Discover new devices */
            searchStart(pInst);
            pInst->state = SEARCHING_STATE;
            break;

        case '2': /* Connect to server */
            {
                if (pInst->bAuth)
                {
                    CsrBtDeviceAddr slaveDeviceAddr = pInst->bdAddrList[pInst->nDevice];

                    printf( "Enter local OBEX password for \"%04X:%02X:%06X\": ", slaveDeviceAddr.nap,
                            slaveDeviceAddr.uap, slaveDeviceAddr.lap  );
                    pInst->kbm = PASSKEY_STATE;
                    pInst->state = CONNECT_INIT_STATE;
                    pInst->passkeyLength = 0;
                    pInst->passkey[0] = 0;
                }
                else
                {
                    startConnect(pInst);
                }
                break;
            }
        case '7': /* Security Controller Configuration */
            handleScKeybPrim(&pInst->scInstData);
            break;

        case 's':
        case 'S':
            setAuthStart(pInst);
            break;

        case 'b':
        case 'B':
            pInst->kbm = ADDRESS_STATE;
            pInst->BdAddrLen = 0;
            printf("Enter bluetooth address (XXXX:XX:XXXXXX [nap:uap:lap]): ");
            break;



        case ' ':
        case RETURN_KEY:
            playIntro(pInst);
            break;
        default:
            printf("\nwarning: not a valid selection to press: %c, try again\n", cKey);
            break;
    }
    return bProcessed;
}

static CsrBool connectState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool bProcessed = TRUE;

    switch( cKey)
    {
        case 'd':
        case 'D':
            disconnectPacStart(pInst);
            break;
        case ' ':
        case RETURN_KEY:
            playIntro(pInst);
            break;
        default:
            printf("\nwarning: not a valid selection to press: %c, try again\n", cKey);
            break;
    }
    return bProcessed;
}

static CsrBool connectedState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool bProcessed = TRUE;

    switch( cKey)
    {
        case '3':
            pullPbStart(pInst);
            break;

        case '4':
            setFolderStart(pInst);
            break;

        case '5':
            pullPbListStart(pInst);
            break;

        case '6':
            pullPbEntryStart(pInst);
            break;

        case 'a':
        case 'A':
            abortStart(pInst);
            break;

        case 'd':
        case 'D':
            disconnectPacStart(pInst);
            break;

        case 'f':
        case 'F':
            filterMenu();
            pInst->kbm = FILTER_STATE;
            break;

        case 'g':
        case 'G':
            pInst->filter[0] = 0x00;
            pInst->filter[1] = 0x00;
            pInst->filter[2] = 0x00;
            pInst->filter[3] = 0x00;
            pInst->filter[4] = 0x00;
            pInst->filter[5] = 0x00;
            pInst->filter[6] = 0x00;
            pInst->filter[7] = 0x00;
            printf("vcard request filter cleared.\n");
            break;

        case 'h':
        case 'H':
            /* get everything */
            pInst->filter[0] = 0xff;
            pInst->filter[1] = 0xff;
            pInst->filter[2] = 0xff;
            pInst->filter[3] = 0x1f;
            pInst->filter[4] = 0x00;
            pInst->filter[5] = 0x00;
            pInst->filter[6] = 0x00;
            pInst->filter[7] = 0x00;
            printf("all non-reserved vcard request filter bits enabled.\n");
            break;

        case ' ':
        case RETURN_KEY:
            playIntro(pInst);
            break;
        default:
            printf("\nwarning: not a valid selection to press: %c, try again\n", cKey);
            break;
    }
    return bProcessed;
}

static CsrBool searchState(appInstData *pInst, CsrUint8 cKey)
{
    CsrBool bProcessed = TRUE;

    if (cKey >= '0' && cKey <= '9')
    {
        if (pInst->nDeviceInList >= (CsrUint32)(cKey - '0'))
        {
            pInst->nDevice = cKey - '0';
            bProcessed = TRUE;
            if(pInst->bFullDeviceList)
            {
                printf(
                    "Selecting device with addr: %04X:%02X:%06X\n",
                    pInst->bdAddrList[pInst->nDevice].nap, pInst->bdAddrList[pInst->nDevice].uap,
                    pInst->bdAddrList[pInst->nDevice].lap
                    );
                scAppSetBondingAddr(&pInst->scInstData,
                                    pInst->bdAddrList[pInst->nDevice]);
                pInst->state = IDLE_STATE;
            }
            else
            {
                CsrBtSdCancelSearchReqSend(pInst->CsrSchedQid);
            }
        }
        else
        {
            printf("You have selected an unknown device! Please try again\n");
        }
    }
    return bProcessed;
}

static char *getPathStart(char *path)
{
    CsrUint32 i;

    for(i=CsrStrLen(path); i>0; i--)
    {
        if((char)path[i] == *"/" || (char)path[i] == *"\\" )
        {
            break;
        }
    }

    return (char*)(path + i);
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
