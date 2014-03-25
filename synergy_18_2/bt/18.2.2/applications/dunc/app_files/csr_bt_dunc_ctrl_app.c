/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include "stdio.h"
#include "string.h"
#include <stdarg.h>
#include <stdlib.h>

#include "csr_env_prim.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_tasks.h"
#include "bluetooth.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_dunc_prim.h"
#include "csr_bt_dunc_lib.h"
#include "csr_bt_dunc_data_app.h"
#include "csr_bt_platform.h"
#include "csr_bt_demoapp.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_sched_init.h"

#define NO_SELECTED                    (255)
#define MAX_DISCOVERY_RESULTS_IN_LIST  (10)

/*  the maximum size that the SAP client is capable of transferring*/
#define MAX_MSG_SIZE                   (2048)

/*    The time to wait for bonding and connection in number of seconds    */
#define MAX_CONNECT_TIME               (10)
#define KEYB_CHECK_TIMEOUT             (200000)

/* The number of possible console output settings */
#define NUMBER_OF_OUTPUTS              (2)

#define TO_UPPER(x)                    ((((x) >= 'a') && ((x) <= 'z')) ? ((x) & 0xDF) : (x))
#define IS_DIGIT_CHAR(x)               ((((x) >= '0') && ((x) <= '9')) ? TRUE : FALSE)

typedef CsrUint8 DuncCtrlDemoStatesType;
#define  idle (DuncCtrlDemoStatesType) 0x00
#define  inquiry (DuncCtrlDemoStatesType) 0x01
#define  pairing (DuncCtrlDemoStatesType) 0x02
#define  connecting (DuncCtrlDemoStatesType) 0x03
#define  connected (DuncCtrlDemoStatesType) 0x04
#define  disconnecting (DuncCtrlDemoStatesType) 0x05

typedef struct
{
    CsrSchedQid                       CsrSchedQid; /* my queue id */
    CsrSchedQid                       extraQid; /* data queue id */
    DuncCtrlDemoStatesType          state;
    DuncCtrlDemoStatesType          prevState;
    CsrBtDeviceAddr                    bdAddr;  /*the default address to use in crowded environment*/
    void *                          msgP;
    CsrUint8                         numInList;
    CsrUint8                         selectedDevice;
    CsrBtDeviceAddr                    bdAddrList[MAX_DISCOVERY_RESULTS_IN_LIST]; /*list of discovered devices*/
    CsrBool                          forwardAllKeys;
    ScAppInstData_t                 scInstData;
} DuncCtrlDemoInstanceDataType;

/* forward declarations for handler functions    */
static void handleKeybPrim(DuncCtrlDemoInstanceDataType *instData);
static void handleDuncPrim(DuncCtrlDemoInstanceDataType *instData);
static void handleSdPrim(DuncCtrlDemoInstanceDataType *instData);

int _kbhit( void );
int _getch( void );

extern BD_ADDR_T defGlobalBdAddr;

static void* createEnvCleanup(void* vPtr)
{
    CsrCleanupInd *prim;
    CSR_UNUSED(vPtr);
    prim = CsrPmemAlloc(sizeof(CsrCleanupInd));
    prim->type = CSR_CLEANUP_IND;
    prim->phandle = TESTQUEUE;

    return((void*)prim);
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

static void forwardKey(CsrUint16 queue, CsrUint8 theChar)
{
    CsrBtKeyPress_t *key;
    key = CsrPmemAlloc(sizeof(CsrBtKeyPress_t));
    key->type = KEY_MESSAGE;
    key->key = theChar;
    CsrSchedMessagePut(queue, KEY_MESSAGE, key);
}

static void playIntro(DuncCtrlDemoInstanceDataType *instData)
{
    switch(instData->state)
    {
        case idle:
        case inquiry:
            {
                printf("------------------------------------------------------------\n");
                printf("\n         Dialup Networking Profile Client Demo\n");
                printf("\n------------------------------------------------------------\n");
                printf("Hit <ESC> to quit program!\n");
                printf("\n");
                printf("State:       NOT CONNECTED\n");
                printf("Modem Mode:  NOT CONNECTED\n");
                printf("PPP State:   DISCONNECTED\n");
                printf("\n");
                printf("Options: \n\n");
                printf("    0) Start inquiry for a DUN Gateway\n");
                printf("    1) Security Controller Configuration\n");
                    printf("    2) Connect to the Gateway (%04X:%02X:%06X)\n",instData->bdAddrList[instData->selectedDevice].nap, instData->bdAddrList[instData->selectedDevice].uap, instData->bdAddrList[instData->selectedDevice].lap);
                printf("\n------------------------------------------------------------\n");
                break;
            }
        case connecting:
            {
                printf("------------------------------------------------------------\n");
                printf("\n         Dialup Networking Profile Client Demo\n");
                printf("\n------------------------------------------------------------\n");
                printf("Hit <ESC> to quit program!\n");
                printf("\n");
                printf("State:       CONNECTING\n");
                printf("Modem Mode:  NOT CONNECTED\n");
                printf("PPP State:   DISCONNECTED\n");
                printf("\n");
                printf("Options: \n\n");
                printf("    3) Cancel connection establishment\n");
                printf("\n------------------------------------------------------------\n");
                break;
            }
        case connected:
            {
                /* now we are connected - let the data part control the info printing */
                forwardKey(CSR_BT_EXTRA_IFACEQUEUE, ' ');
                break;
            }
        default:
            {

            }
    }


}

static void clearResultList(CsrUint8 *selectedDevice, CsrUint32 maxResult, BD_ADDR_T *bdAddrList)
{
    /* Setup search arrays */
    CsrUint8 i;

    *selectedDevice = 0;
    for (i = 0; i < maxResult; i++)
    {
        bdAddrList[i].lap = defGlobalBdAddr.lap;
        bdAddrList[i].nap = defGlobalBdAddr.nap;
        bdAddrList[i].uap = defGlobalBdAddr.uap;
    }
}
static void stopInquiry(DuncCtrlDemoInstanceDataType *instData)
{
    CsrBtSdCancelSearchReqSend(instData->CsrSchedQid);
    printf("\nCancelling inquiry - please wait ....\n");
    instData->state = idle;
}

static void startInquiry(DuncCtrlDemoInstanceDataType *instData)
{
    instData->selectedDevice = NO_SELECTED;
    instData->numInList = 0;
    instData->state = inquiry;

    CsrBtSdSearchReqSend(instData->CsrSchedQid,
                  CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                  6000, /* RSSI buffer timeout */
                  CSR_BT_INFINITE_TIME, /* Search timeout */
                  CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE, /* RSSI threshold */
                  CSR_BT_DIAL_UP_NETWORKING_PROFILE, /* Class of Device */
                  CSR_BT_DIAL_UP_NETWORKING_PROFILE, /* Class of Device mask */
                  CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                  0,     /* Filter length */
                  NULL); /* Filter */

    printf("\nSearch in progress - press entry number to select device (press \"c\" for cancel)\n");
}

static void duncScAppSetFnFini(void *pFiniData)
{
    DuncCtrlDemoInstanceDataType *instData = (DuncCtrlDemoInstanceDataType *) pFiniData;
    scAppRestoreState((CsrUint8 *) &instData->prevState, (CsrUint8 *) &instData->state);
    playIntro(instData);
}

static void duncScAppSetFnInit(void *pInitData)
{
    DuncCtrlDemoInstanceDataType *instData = (DuncCtrlDemoInstanceDataType *) pInitData;
    scAppSaveState((CsrUint8 *) &instData->prevState, (CsrUint8 *) &instData->state, pairing);
}

static void* duncScAppSetFnGetMessage(void *pRecvMsgPData)
{
    DuncCtrlDemoInstanceDataType *instData = (DuncCtrlDemoInstanceDataType *) pRecvMsgPData;
    void *result                           = instData->msgP;
    instData->msgP                         = NULL;
    return result;
}

/******************************************************************************
 * init function called by the scheduler upon initialisation. This function
 * is used to initialise the demo application by initialising its instance
 * data, printing the options available for the user and waiting for the
 * user to make a choice
 *****************************************************************************/
void CsrBtAppInit(void **gash)
{
    CsrBtDeviceAddr zeroAddr;
    DuncCtrlDemoInstanceDataType *instData;
    *gash    = (void *) CsrPmemAlloc(sizeof(DuncCtrlDemoInstanceDataType));
    instData = (DuncCtrlDemoInstanceDataType *) *gash;

    /* If running in external mode, get a CsrSchedQid */
    instData->CsrSchedQid               = TESTQUEUE;
    instData->extraQid          = CSR_BT_EXTRA_IFACEQUEUE;
    instData->state             = idle;

    CsrBtBdAddrZero(&zeroAddr);
    if (!CsrBtBdAddrEq(&defGlobalBdAddr, &zeroAddr))
    {
    instData->bdAddr.lap        = defGlobalBdAddr.lap; /*here the default address is set to the value given in the "-a" argument*/
    instData->bdAddr.uap        = defGlobalBdAddr.uap; /*here the default address is set to the value given in the "-a" argument*/
    instData->bdAddr.nap        = defGlobalBdAddr.nap; /*here the default address is set to the value given in the "-a" argument*/
    }
    else
    {
        instData->bdAddr.lap        = 0; /*here the default address is set*/
        instData->bdAddr.uap        = 0; /*here the default address is set*/
        instData->bdAddr.nap        = 0; /*here the default address is set*/
    }
    instData->forwardAllKeys    = FALSE;

    /* Register at the security controller */
    CsrBtScActivateReqSend(instData->CsrSchedQid);

    clearResultList(&instData->selectedDevice, MAX_DISCOVERY_RESULTS_IN_LIST, &instData->bdAddrList[0]);

    /* Security App Configuration */
    scAppInitInstanceData(&instData->scInstData);

    scAppSetFnFini(&instData->scInstData, duncScAppSetFnFini, instData);
    scAppSetFnInit(&instData->scInstData, duncScAppSetFnInit, instData);
    scAppSetSecLevelSend(&instData->scInstData, scAppSendProfileSecurityLevelReq, &instData->scInstData);

    scAppSetFnGetMessage(&instData->scInstData, duncScAppSetFnGetMessage, instData);

    scAppSetSecLevelPrim(&instData->scInstData,
                         0,     /* Index 0. Single profile */
                         "DUNC",
                         CSR_BT_DUNC_PRIM, CSR_BT_DUNC_IFACEQUEUE,
                         SC_APP_SET_SEC_LEVEL_NA, CSR_BT_DUNC_SECURITY_OUT_REQ);

    system(CLS);
    playIntro(instData);
    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, instData);
}

#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    CsrUint16 msgType=0;
    void *msgData=NULL;
    DuncCtrlDemoInstanceDataType * instanceData = *gash;

    scAppDeinitInstanceData(&instanceData->scInstData);

    /* get a message from the demoapplication message queue.
     * The message is returned in prim and the event type in eventType
     */
    while ( CsrSchedMessageGet(&msgType, &msgData) )
    {
        switch (msgType)
        {
            /*It was a dialup networking profile client primitive*/
            case CSR_BT_DUNC_PRIM:
            {
                CsrBtDuncFreeUpstreamMessageContents(msgType, msgData);
                break;
            }
            case CSR_BT_SD_PRIM:
            {
                CsrBtSdFreeUpstreamMessageContents(msgType, msgData);
                break;
            }
            case CSR_BT_SC_PRIM:
            {
                CsrBtScFreeUpstreamMessageContents(msgType, msgData);
                break;
            }

            default:
            {
            }
        }
        CsrPmemFree(msgData);
    }

    CsrPmemFree(instanceData);
}
#endif

/******************************************************************************
 * this is the demo application handler function. All primitives sent to the
 * demo application will be received in here. The scheduler ensures that
 * the function is activated when a signal is put on the demo application
 * signal queue. The gash double pointer parameter is the instance data
 * available for this task running in the scheduler
 *****************************************************************************/
void CsrBtAppHandler(void **gash)
{
    CsrUint16    eventType=0;
    DuncCtrlDemoInstanceDataType *instData;
    instData = (DuncCtrlDemoInstanceDataType *) *gash;

    /* get a message from the ctrl demo application message queue.
     * The message is returned in prim and the event type in eventType
     */
    CsrSchedMessageGet(&eventType, &instData->msgP);

    /* four event types must be handled. SC for bonding, SD for inquiry
     * and DUNCC for DUNC profile related messages
     */
    switch (eventType)
    {
        case CSR_SCHED_PRIM:
            {
                CsrEnvPrim *primType;
                primType = (CsrEnvPrim *) instData->msgP;
                if(*primType == CSR_CLEANUP_IND)
                {
                    printf("Environment cleanup indication received\n");
                }
                break;
            }
            /*It was a security controller related primitive*/
        case CSR_BT_SC_PRIM:
            {
                handleScPrim(&instData->scInstData);
                break;
            }
            /*It was a dialup networking profile client primitive*/
        case CSR_BT_DUNC_PRIM:
            {
                CsrBtDuncPrim *msg = (CsrBtDuncPrim *) instData->msgP;
                switch(*msg)
                {
                    case CSR_BT_DUNC_SECURITY_OUT_CFM:
                        handleScSecLevelPrim(&instData->scInstData);
                        break;

                    default:
                        handleDuncPrim(instData);
                        break;
                    }
                break;
            }
            /*It was a service discovery primitive*/
        case CSR_BT_SD_PRIM:
            {
                handleSdPrim(instData);
                break;
            }
            /*It was a key pressed primitive*/
        case KEY_MESSAGE:
            {
                handleKeybPrim(instData);
                instData->msgP = NULL;
                break;
            }
        default:
            {
                /*    unexpected primitive received    */
                printf("####### default in testhandler %x,\n",eventType);
            }
    }    /*    end switch(eventType)    */

    /* free the received signal again. If the signal for some reason
     * must be stored in the application the pointer (prim) must be
     * set to NULL in order not to free it here
     */
    CsrPmemFree(instData->msgP);
}

/******************************************************************************
 * an SD event is received.
 *****************************************************************************/
static void handleSdPrim(DuncCtrlDemoInstanceDataType *instData)
{
    CsrPrim    *primType;

    primType = (CsrPrim *)instData->msgP;
    switch (*primType)
    {
        /*    Inq result received    */
        case CSR_BT_SD_SEARCH_RESULT_IND:
            {
                CsrBtSdSearchResultInd *prim = (CsrBtSdSearchResultInd *) instData->msgP;
                if (instData->state == inquiry)
                {
                    CsrUint16 nameLen;
                    CsrUint8 *name;

                    instData->bdAddrList[instData->numInList] = prim->deviceAddr;

                    printf("\n%u: ", instData->numInList);
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

                    instData->numInList++;
                }
                CsrPmemFree(prim->info);
                break;
            }
            /* Inquiry confirm, indicating that the specified search
             * criteria were fullfilled
             */
        case CSR_BT_SD_CLOSE_SEARCH_IND:
                instData->state = idle;
                printf("\nActivating entry %i device addr: %04X:%02X:%06X\n",
                       instData->selectedDevice,
                       instData->bdAddrList[instData->selectedDevice].nap,
                       instData->bdAddrList[instData->selectedDevice].uap,
                       instData->bdAddrList[instData->selectedDevice].lap);
                system(CLS);
                playIntro(instData);
                break;
        default:
            {
                printf("####### default in SdPrim handler %x,\n",*primType);
                /*    unexpected primitive received    */
                CsrBtSdFreeUpstreamMessageContents(*primType, instData->msgP);
            }
    }    /*    end switch(primType)    */
}

/******************************************************************************
 * function for start connecting to GW
 *****************************************************************************/
static void startConnect(DuncCtrlDemoInstanceDataType *instData)
{
    if(instData->selectedDevice == NO_SELECTED)
    {
        CsrBtDuncConnectReqSend(CSR_BT_DUNC_IFACEQUEUE, instData->CsrSchedQid,
                           instData->bdAddr, TRUE);
    }
    else
    {
        CsrBtDuncConnectReqSend(CSR_BT_DUNC_IFACEQUEUE, instData->CsrSchedQid,
                           instData->bdAddrList[instData->selectedDevice],
                           TRUE);
    }

#ifndef _NO_DATA_PATH_
    CsrBtDuncRegisterDataPathHandleReqSend(CSR_BT_DUNC_IFACEQUEUE,
                                      instData->extraQid);
#endif

    instData->state = connecting;
    system(CLS);
    playIntro(instData);
}

/******************************************************************************
 * a keyboard event is received.
 * first check for selection of a found device during inquiry if state
 * is inquiry. If not start proper action as selected.
 * NOTE: This keyboard handler is common for both the data and
 * control applications.
 *****************************************************************************/
static void handleKeybPrim(DuncCtrlDemoInstanceDataType *instData)
{
    CsrBtKeyPress_t *key;
    key = (CsrBtKeyPress_t*)instData->msgP;

    if (key->key == ESCAPE_KEY)
    {
        printf("\nUser exit...\n");
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
        return;
#else
        exit(0);
#endif
    }

    if (instData->state == inquiry)
    {
        if(key->key == 'c' || key->key == 'C')
        {
            stopInquiry(instData);
            /* use default device address */
            instData->selectedDevice = NO_SELECTED;
            return;
        }
        instData->selectedDevice = key->key - '0';
        if ( instData->selectedDevice >= instData->numInList )
        {
            instData->selectedDevice = 0;
            CsrBtBdAddrCopy(&instData->bdAddrList[instData->selectedDevice], &instData->bdAddr);
        }

        instData->state = idle;
            printf("Selecting device with addr: %04X:%02X:%06X\n", instData->bdAddrList[instData->selectedDevice].nap, instData->bdAddrList[instData->selectedDevice].uap, instData->bdAddrList[instData->selectedDevice].lap);
        scAppSetBondingAddr(&instData->scInstData,
                            instData->bdAddrList[instData->selectedDevice]);
        stopInquiry(instData);
        return;
    }

    if (instData->state == pairing)
    {
        handleScKeybPrim(&instData->scInstData);
        return;
    }

#ifndef _NO_DATA_PATH_
    if ((instData->forwardAllKeys == TRUE) && (instData->state == connected))
    {
        if (key->key == RETURN_KEY)
        {
            instData->forwardAllKeys = FALSE;
        }
        CsrSchedMessagePut(CSR_BT_EXTRA_IFACEQUEUE, KEY_MESSAGE, key);

        return;
    }
#endif
    switch(key->key)
    {
        /*discover new GWs*/
        case '0':
            {
                if ( instData->state == idle )
                {
                    instData->state = inquiry;
                    startInquiry(instData);
                    instData->numInList = 0;
                    instData->selectedDevice = 0;
                }
                break;
            }
            /*Security Controller Configuration */
        case '1':
            {
                if (instData->state == idle)
                {
                    handleScKeybPrim(&instData->scInstData);
                }
                else
                {
                    printf("Illegal selection! Please make another choice...\n");
                }

                break;
            }
            /*connect to the GW*/
        case '2':
            {
                if (instData->state == idle)
                {
                    startConnect(instData);
                }
                else
                {
                    printf("Illegal selection! Please make another choice...\n");
                }
                break;
            }
            /*cancel connect*/
        case '3':
            {
                if (instData->state == connecting)
                {
                    CsrBtDuncCancelConnectReqSend(CSR_BT_DUNC_IFACEQUEUE);
                }
                else
                {
                    printf("Illegal selection! Please make another choice...\n");
                }
                break;
            }
            /* disconnect */
        case '4':
            {
                if (instData->state == connected)
                {
                    CsrBtDuncDisconnectReqSend(CSR_BT_DUNC_IFACEQUEUE);
                }
                else
                {
                    printf("Illegal selection! Please make another choice...\n");
                }
                break;
            }
            /****************************************
             *  Key handling for the data app part  *
             ****************************************/
#ifndef _NO_DATA_PATH

        case '5': /* enter dial number */
        case '6': /* enter user */
        case '7': /* enter password */
            /* enter IP to ping */
        case 'i':
        case 'I':
            {
                if (instData->state == connected)
                {
                    instData->forwardAllKeys = TRUE;
                    /* forward the key to the data app part */
                    forwardKey(CSR_BT_EXTRA_IFACEQUEUE, key->key);
                }
                break;
            }
            /* refresh screen */
        case ' ':
            {
                playIntro(instData);
                break;
            }

        case '8':  /* dial */
        case '9':  /* send test data */
            /* accept incoming call */
        case 'a':
        case 'A':
            /* start ping */
        case 'p':
        case 'P':
        case 'h':
        case 'H':
            /* Reject incoming call */
        case 'r':
        case 'R':
            {
                if (instData->state == connected)
                {
                    /* forward the key to the data app part */
                    forwardKey(CSR_BT_EXTRA_IFACEQUEUE, key->key);
                }
                break;
            }
#endif
            /****************************************/

        case 'e':
            {
                CsrSchedMessageBroadcast(CSR_SCHED_TASK_ID, createEnvCleanup, NULL);
                break;
            }
        default:
            {
                printf("####### default in keybPrim ctrl handler %x,\n", key->key );
                /*    unexpected primitive received    */
            }
    }    /*    end switch    */
}


/******************************************************************************
 * a DUNC prim is received from the profile. Determine what it is and
 * perform the correct action
 *****************************************************************************/
static void handleDuncPrim(DuncCtrlDemoInstanceDataType *instData)
{
    CsrBtDuncPrim *primType;
    primType = (CsrBtDuncPrim *)instData->msgP;
    switch(*primType)
    {
        case CSR_BT_DUNC_CONNECT_CFM:
            {
                CsrBtDuncConnectCfm * duncPrim;
                duncPrim = (CsrBtDuncConnectCfm *) instData->msgP;
                if (duncPrim->resultSupplier == CSR_BT_SUPPLIER_DUNC &&
                    duncPrim->resultCode == CSR_BT_RESULT_CODE_DUNC_SUCCESS)
                {
                    instData->state = connected;
                    system(CLS);
                    playIntro(instData);
                }
                else
                {
                    instData->state = idle;
                    system(CLS);
                    printf("Connect failed with err: 0x%04x (supplier 0x%04x)\n",
                        duncPrim->resultCode, duncPrim->resultSupplier);
                    playIntro(instData);
                }
                break;
            }

        case CSR_BT_DUNC_DISCONNECT_IND:
            {
                CsrBtDuncDisconnectInd *duncPrim;
                duncPrim = (CsrBtDuncDisconnectInd *) instData->msgP;
                if (duncPrim->localTerminated == TRUE)
                {
                    instData->state = idle;
                    system(CLS);
                    playIntro(instData);
                }
                else
                {
                    printf("Could not disconnect\n");
                }
                break;
            }
        default:
            {
                printf("Has received an unexpected CSR_BT_DUNC_PRIM: %d!!\n",
                       *primType);
            }
    }
}

void applicationUsage(char *col, char *des, char *opt)
{
}

char* applicationExtraOptions(void)
{
    return "";
}

char* applicationCmdLineParse(int ch, char *optionArgument)
{
    return NULL;
}

void applicationSetup(void)
{
}
