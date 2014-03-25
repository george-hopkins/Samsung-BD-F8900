/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_tasks.h"
#include "bluetooth.h"
#include "csr_bt_profiles.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_sapc_prim.h"
#include "csr_bt_sapc_lib.h"
#include "csr_bt_saps_prim.h"
#include "csr_bt_saps_lib.h"
#include "csr_bt_platform.h"
#include "csr_bt_demoapp.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_sched_init.h"

#include "csr_bt_cm_lib.h"

#define KEYB_CHECK_TIMEOUT               (200000)
#define MAX_DISCOVERY_RESULTS_IN_LIST    (10)

/* The maximum size that the SAP client is capable of transferring */
#define MAX_MSG_SIZE                     (2048)

/* The time to wait for bonding and connection in number of seconds */
#define MAX_CONNECT_TIME                 (10)

/* The number of possible console output settings */
#define NUMBER_OF_OUTPUTS                (2)

#define TO_UPPER(x)                      ((((x) >= 'a') && ((x) <= 'z')) ? ((x) & 0xDF) : (x))
#define IS_DIGIT_CHAR(x)                 ((((x) >= '0') && ((x) <= '9')) ? TRUE : FALSE)

/*********************************************************************
 * Application states and instance
 *********************************************************************/
typedef CsrUint8 SapDemoStates_t;
#define  idle (SapDemoStates_t) 0x00
#define  inquiry (SapDemoStates_t) 0x01
#define  pairing (SapDemoStates_t) 0x02
#define  connected (SapDemoStates_t) 0x03
#define  disconnecting (SapDemoStates_t) 0x04

typedef struct
{
    CsrSchedQid                       CsrSchedQid;
    CsrBool                          serverActivated;
    SapDemoStates_t                 state;
    SapDemoStates_t                 prevState;
    CsrBtDeviceAddr                    bdAddr;
    void *                          msgP;
    CsrUint8                         numInList;
    CsrUint8                         selectedDevice;
    CsrBtDeviceAddr                    bdAddrList[MAX_DISCOVERY_RESULTS_IN_LIST];
    CsrUint16                        remoteMaxMsgSize;
    CsrBool                          outputLevel;
    ScAppInstData_t                 scInstData;
#ifdef ENABLE_COMMANDS
    CsrBool                          commandSent;
    CsrBool                          commandExpected;
#endif
} SapDemoInstanceData_t;

/*********************************************************************
 * Forward declarations for handler functions
 *********************************************************************/
void handleKeybPrim(SapDemoInstanceData_t *instData);
void handleSapsPrim(SapDemoInstanceData_t *instData);
void handleSapcPrim(SapDemoInstanceData_t *instData);
void handleSdPrim(SapDemoInstanceData_t *instData);
void printHexString(CsrUint8 * hexString, CsrUint16 length);
int _kbhit( void );
int _getch( void );

/*********************************************************************
 * Keyboard timer fucntion
 *********************************************************************/
void KeyMessageService(CsrUint16 m, void *data)
{
    if(_kbhit())
    {
        CsrBtKeyPress_t *key;
        key = CsrPmemAlloc(sizeof(CsrBtKeyPress_t));
        key->type = KEY_MESSAGE;
        key->key =_getch();
        CsrSchedMessagePut(TESTQUEUE, KEY_MESSAGE, key);
    }
    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, m, data);
}

/*********************************************************************
 * Display menu options
 *********************************************************************/
void playIntro(SapDemoInstanceData_t *instData)
{
    system_cls();
    printf("\n*********************************************************************\n");
    printf("*\n* SIM ACCESS Demo:\n*\n");
    printf("* Hit <ESC> to quit program!\n*\n");
    printf("*\n");
    printf("* Options: \n");
    printf("*    0) Start inquiry for a mobile phone\n");
    printf("*    1) Security Controller Configuration\n");

    if(instData->serverActivated == FALSE)
    {
        printf("*    2) Activate SAP server (server is not activated)\n");
    }
    else
    {
        printf("*    2) De-activate SAP server (server is activated)\n");
        printf("*    3) Start connection between car phone and mobile phone\n");
    }
    switch(instData->outputLevel)
    {
        case FALSE:
            {
                printf("*    a) Display content of ATRs and APDUs (current level: off)\n");
                break;
            }
        case TRUE:
            {
                printf("*    a) Display content of ATRs and APDUs (current level: on)\n");
                break;
            }
        default:
            {

            }
    }

#ifdef ENABLE_COMMANDS
    printf("*    The commands below are only allowed when being connected\n");
    printf("*    b) Send a RESET_SIM_REQ to the mobile phone\n");
    printf("*    c) Send a POWER_SIM_OFF_REQ to the mobile phone\n");
    printf("*    d) Send a POWER_SIM_ON_REQ to the mobile phone\n");
    printf("*    e) Send a TRANSFER_CARD_READER_STATUS_REQ to the mobile phone\n");
    printf("*    f) Send a TRANSFER_PROTOCOL_REQ to the mobile phone\n");
    printf("*    g) Send a STATUS_IND indicating card not accessible to the car phone\n");
    printf("*    h) Send a STATUS_IND indicating card removed to the car phone\n");
    printf("*    i) Send a STATUS_IND indicating card inserted to the car phone\n");
    printf("*    j) Send a STATUS_IND indicating card recovered to the car phone\n");
    printf("*    k) Send a STATUS_IND indicating card reset to the car phone\n");
    printf("*    l) Send a TRANSFER_ATR_REQ to the mobile phone\n");

    printf("*\n");
    printf("*    x) Do not forward the next incoming indication or command\n");
#endif
    printf("*\n");
    printf("*********************************************************************\n");
}

/*********************************************************************
 * Stop search
 ********************************************************************/
void stopInquiry(SapDemoInstanceData_t *instData)
{
    CsrBtSdCancelSearchReqSend(instData->CsrSchedQid);
    printf("\nCancelling inquiry - please wait ....\n");
    instData->state = idle;
}

/*********************************************************************
 * Start search
 ********************************************************************/
void startInquiry(SapDemoInstanceData_t *instData)
{
    instData->state = inquiry;
    instData->numInList = 0;

    CsrBtSdSearchReqSend(instData->CsrSchedQid,
                  CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                  6000, /* RSSI buffer timeout */
                  CSR_BT_INFINITE_TIME, /* Search timeout */
                  CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE, /* RSSI threshold */
                  CSR_BT_TELEPHONY_MAJOR_SERVICE_MASK, /* Class of Device */
                  CSR_BT_TELEPHONY_MAJOR_SERVICE_MASK, /* Class of Device mask */
                  CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                  0, /* Filter length */
                  NULL); /* Filter */

    printf("\nSearch in progress - please wait (press \"c\" for cancel).....\n");
}

static void sapScAppSetFnFini(void *pFiniData)
{
    SapDemoInstanceData_t *instData = (SapDemoInstanceData_t *) pFiniData;
    scAppRestoreState((CsrUint8 *) &instData->prevState, (CsrUint8 *) &instData->state);
    system_cls();
    playIntro(instData);
}

static void sapScAppSetFnInit(void *pInitData)
{
    SapDemoInstanceData_t *instData = (SapDemoInstanceData_t *) pInitData;
    scAppSaveState((CsrUint8 *) &instData->prevState, (CsrUint8 *) &instData->state, pairing);
}

static void* sapScAppSetFnGetMessage(void *pRecvMsgPData)
{
    SapDemoInstanceData_t *instData = (SapDemoInstanceData_t *) pRecvMsgPData;
    void *result                    = instData->msgP;
    instData->msgP                  = NULL;
    return result;
}

/*********************************************************************
 * Init function called by the scheduler upon initialisation. This
 * function is used to boot the demo application by printing the
 * options available for the user and waiting for the user to make a
 * choice
 *********************************************************************/
void CsrBtAppInit(void **gash)
{
    extern BD_ADDR_T defGlobalBdAddr;
    CsrBtDeviceAddr zeroAddr;

    SapDemoInstanceData_t *instData;
    *gash    = (void *) CsrPmemAlloc(sizeof(SapDemoInstanceData_t));
    instData = (SapDemoInstanceData_t *) *gash;

    instData->state = idle;

    CsrBtBdAddrZero(&zeroAddr);
    if (!CsrBtBdAddrEq(&defGlobalBdAddr, &zeroAddr))
    {
        instData->bdAddr.lap = defGlobalBdAddr.lap;
        instData->bdAddr.nap = defGlobalBdAddr.nap;
        instData->bdAddr.uap = defGlobalBdAddr.uap;
    }
    else
    {
        instData->bdAddr.lap = 0;
        instData->bdAddr.nap = 0;
        instData->bdAddr.uap = 0;
    }

    instData->selectedDevice = 255; /*none is yet chosen*/
    instData->remoteMaxMsgSize = 0;
    instData->outputLevel = TRUE;
    instData->serverActivated = FALSE;

    playIntro(instData);

    /* Application handle */
    instData->CsrSchedQid = TESTQUEUE;

    /* Security App Configuration */
    CsrBtScActivateReqSend(instData->CsrSchedQid);
    scAppInitInstanceData(&instData->scInstData);

    scAppSetFnFini(&instData->scInstData, sapScAppSetFnFini, instData);
    scAppSetFnInit(&instData->scInstData, sapScAppSetFnInit, instData);
    scAppSetSecLevelSend(&instData->scInstData, scAppSendProfileSecurityLevelReq, &instData->scInstData);

    scAppSetFnGetMessage(&instData->scInstData, sapScAppSetFnGetMessage, instData);

    scAppSetSecLevelPrim(&instData->scInstData,
                         0,     /* Index 0. Single profile */
                         "SAPC", CSR_BT_SAPC_PRIM,
                         CSR_BT_SAPC_IFACEQUEUE,
                         SC_APP_SET_SEC_LEVEL_NA, CSR_BT_SAPC_SECURITY_OUT_REQ);

    scAppSetSecLevelPrim(&instData->scInstData,
                         1,     /* Index 1. Single profile */
                         "SAPS", CSR_BT_SAPS_PRIM,
                         CSR_BT_SAPS_IFACEQUEUE,
                         CSR_BT_SAPS_SECURITY_IN_REQ, SC_APP_SET_SEC_LEVEL_NA);


    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, NULL);
    {
        ScAppSecLvlInstData_t *secLevelInst;
        ScAppDemoSecurityReq  *req;

        secLevelInst = &instData->scInstData.secLevelData[instData->scInstData.secLevelIdx % instData->scInstData.secLevelCnt];
        req          = &secLevelInst->req[secLevelInst->securityIncoming];
        req->secLevel = CSR_BT_SEC_SPECIFY|CSR_BT_SEC_ENCRYPTION|CSR_BT_SEC_AUTHENTICATION|CSR_BT_SEC_MITM;
        instData->scInstData.fnSend(instData->scInstData.pSendData);
    }
}

/*********************************************************************
 * Shutdown application. Called when the scheduler "CsrSchedStop" is
 * run.
 ********************************************************************/
#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    CsrUint16 msg_type=0;
    void *msg_data=NULL;
    SapDemoInstanceData_t *instData;

    instData = (SapDemoInstanceData_t*)(*gash);

    scAppDeinitInstanceData(&instData->scInstData);

    /*get the messages placed at the test queue and free the necessary memory*/
    while(CsrSchedMessageGet(&msg_type, &msg_data))
    {
        switch (msg_type)
        {
            case CSR_BT_SAPC_PRIM:
                {
                    CsrBtSapcFreeUpstreamMessageContents(msg_type, msg_data);
                    break;
                }
            case CSR_BT_SAPS_PRIM:
                {
                    CsrBtSapsFreeUpstreamMessageContents(msg_type, msg_data);
                    break;
                }
            case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(msg_type, msg_data);
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
            default:
                {
                    break;
                }
        }
        CsrPmemFree(msg_data);
    }
    CsrPmemFree(instData);
}
#endif

/*********************************************************************
 * This is the demo application handler function. All primitives sent
 * to the demo application will be received in here. The scheduler
 * ensures that the function is activated when a signal is put on the
 * demo application signal queue. The gash double pointer parameter is
 * the instance data available for this task running in the scheduler
 *********************************************************************/
void CsrBtAppHandler(void **gash)
{
    CsrUint16    eventType=0;
    void        *prim=NULL;
    SapDemoInstanceData_t *instData;
    instData = (SapDemoInstanceData_t *) *gash;

    /* Get a message from the demoapplication message queue. The
     * message is returned in prim and the event type in
     * eventType
     */
    CsrSchedMessageGet(&eventType, &prim);
    instData->msgP = prim;

    /* Five event types must be handled. SC for bonding, SD for
     * inquiry and SAPS/SAPC for SAP profile related messages.
     * Finally, we also handle keypresses
     */
    switch (eventType)
    {
        case CSR_BT_SC_PRIM:
            {
                /*It was a security controller related primitive*/
                handleScPrim(&instData->scInstData);
                break;
            }

        case CSR_BT_SAPC_PRIM:
            {
                CsrPrim *prim = (CsrPrim *) instData->msgP;
                switch(*prim)
                {
                    case CSR_BT_SAPC_SECURITY_OUT_CFM:
                        handleScSecLevelPrim(&instData->scInstData);
                        break;

                    default:
                        /*It was a sim access profile client primitive*/
                        handleSapcPrim(instData);
                        break;
                }
                break;
            }
        case CSR_BT_SAPS_PRIM:
            {
                CsrPrim *prim = (CsrPrim *) instData->msgP;
                switch(*prim)
                {
                    case CSR_BT_SAPS_SECURITY_IN_CFM:
                        handleScSecLevelPrim(&instData->scInstData);
                        break;

                    default:
                        /*It was a sim access profile server primitive*/
                        handleSapsPrim(instData);
                        break;
                }
                break;
            }

        case CSR_BT_SD_PRIM:
            {
                /*It was a service discovery primitive*/
                handleSdPrim(instData);
                break;
            }

        case KEY_MESSAGE:
            {
                /*It was a key pressed primitive*/
                handleKeybPrim(instData);
                break;
            }
        default:
            {
                /* Unexpected primitive received    */
                printf("####### default in testhandler %x,\n",eventType);
            }
    }

    /* Free the received signal again. If the signal for some reason
     * must be stored in the application the pointer (prim) must be
     * set to NULL in order not to free it here
     */
    CsrPmemFree(instData->msgP);
}

/*********************************************************************
 * SD event is received.
 *********************************************************************/
void handleSdPrim(SapDemoInstanceData_t *instData)
{
    CsrPrim    *primType;

    primType = (CsrPrim *)instData->msgP;
    switch (*primType)
    {
        case CSR_BT_SD_SEARCH_RESULT_IND:
            {
                /* Inq result received */
                CsrBtSdSearchResultInd *prim = (CsrBtSdSearchResultInd *) instData->msgP;
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
                       instData->bdAddrList[instData->numInList].nap,
                       instData->bdAddrList[instData->numInList].uap,
                       instData->bdAddrList[instData->numInList].lap);
                printf("%s\n",CodDecodeDeviceClass(prim->deviceClass));

                instData->numInList++;

                CsrPmemFree(prim->info);
                break;
            }
        case CSR_BT_SD_CLOSE_SEARCH_IND:
            {
                printf("Inquiry stopped \n");
                printf("\nActivating entry %i device addr: %04X:%02X:%06X\n", instData->selectedDevice, instData->bdAddrList[instData->selectedDevice].nap, instData->bdAddrList[instData->selectedDevice].uap, instData->bdAddrList[instData->selectedDevice].lap);
                instData->state = idle;
                playIntro(instData);
                break;
            }
        default:
            {
                /* unexpected primitive received */
                printf("####### default in SdPrim handler %x,\n",primType);
                CsrBtSdFreeUpstreamMessageContents(CSR_BT_SD_PRIM, instData->msgP);
            }
    }
}

/*********************************************************************
 * A keyboard event is received.  first check for selection of a found
 * device during inquiry if state is inquiry. If not start proper
 * action as selected.
 *********************************************************************/
void handleKeybPrim(SapDemoInstanceData_t *instData)
{
    CsrBtKeyPress_t *key;
    key = (CsrBtKeyPress_t*)instData->msgP;

    if (key->key == ESCAPE_KEY)
    {
        printf("\nUser exit...\n");
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif
        return;
    }

    if (instData->state == inquiry)
    {
        if(key->key == 'c' || key->key == 'C')
        {
            stopInquiry(instData);
            instData->selectedDevice = 255; /*use default device address*/
            return;
        }
        instData->selectedDevice = key->key - '0';
        if ( instData->selectedDevice >= instData->numInList )
        {
            printf("Activating default device with addr: %04X:%02X:%06X\n",
                   instData->bdAddr.nap,
                   instData->bdAddr.uap,
                   instData->bdAddr.lap);
            scAppSetBondingAddr(&instData->scInstData, instData->bdAddr);
        }
        else
        {
            printf("Selecting device with addr: %04X:%02X:%06X\n",
                   instData->bdAddrList[instData->selectedDevice].nap,
                   instData->bdAddrList[instData->selectedDevice].uap,
                   instData->bdAddrList[instData->selectedDevice].lap);
            scAppSetBondingAddr(&instData->scInstData,
                                instData->bdAddrList[instData->selectedDevice]);
        }
        instData->state = idle;
        stopInquiry(instData);
        return;
    }

    if(instData->state == pairing)
    {
        handleScKeybPrim(&instData->scInstData);
        return;
    }

    switch(key->key)
    {
        case '0':
            {
                /*discover new devices*/
                if ( instData->state == idle )
                {
                    instData->state = inquiry;
                    startInquiry(instData);
                    instData->numInList = 0;
                    instData->selectedDevice = 0;
                }
                break;
            }

        case '1':
            {
                /*pairing with the mobile phone*/
                if ( instData->state == idle )
                {
                    handleScKeybPrim(&instData->scInstData);
                }
                break;
            }

        case '2':
            {
                /*activate server*/
                if(instData->serverActivated == FALSE)
                {
                    CsrBtSapsActivateReqSend(instData->CsrSchedQid);
                }
                else
                {
                    CsrBtSapsDeactivateReqSend();
                }
                break;
            }

        case '3':
            {
                /*ready for connection from the car phone*/
                printf("Ready for connection - connect from the car phone\n");
                break;
            }

        case 'a':
        case 'A':
            {
                /*enable/disable displaying the content of the ATRs and APDUs*/
                if(instData->outputLevel==FALSE)
                {
                    instData->outputLevel = TRUE;
                }
                else
                {
                    instData->outputLevel = FALSE;
                }
                playIntro(instData);
                break;
            }
#ifdef ENABLE_COMMANDS
        case 'b':
        case 'B':
            {
                printf("A RESET_SIM_REQ is just sent to the mobile phone initiated by the user\n");
                CsrBtSapcResetSimReqSend();
                instData->commandSent=TRUE;
                break;
            }
        case 'c':
        case 'C':
            {
                printf("A POWER_SIM_OFF_REQ is just sent to the mobile phone initiated by the user\n");
                CsrBtSapcPowerSimOffReqSend();
                instData->commandSent=TRUE;
                break;
            }
        case 'd':
        case 'D':
            {
                printf("A POWER_SIM_ON_REQ is just sent to the mobile phone initiated by the user\n");
                CsrBtSapcPowerSimOnReqSend();
                instData->commandSent=TRUE;
                break;
            }
        case 'e':
        case 'E':
            {
                printf("A TRANSFER_CARD_READER_STATUS_REQ is just sent to the mobile phone initiated by the user\n");
                CsrBtSapcTransferCardReaderStatusReqSend();
                instData->commandSent=TRUE;
                break;
            }
        case 'f':
        case 'F':
            {
                printf("A TRANSFER_PROTOCOL_REQ is just sent to the mobile phone initiated by the user\n");
                CsrBtSapcSetTransferProtocolReqSend(1);
                instData->commandSent=TRUE;
                break;
            }
        case 'g':
        case 'G':
            {
                printf("A STATUS_IND indicating card not accessible is sent to the car phone initiated by the user\n");
                CsrBtSapsStatusIndSend(CSR_BT_CARD_NOT_ACCESSIBLE);
                break;
            }
        case 'h':
        case 'H':
            {
                printf("A STATUS_IND indicating card removed is sent to the car phone initiated by the user\n");
                CsrBtSapsStatusIndSend(CSR_BT_CARD_REMOVED);
                break;
            }
        case 'i':
        case 'I':
            {
                printf("A STATUS_IND indicating card inserted is sent to the car phone initiated by the user\n");
                CsrBtSapsStatusIndSend(CSR_BT_CARD_INSERTED);
                break;
            }
        case 'j':
        case 'J':
            {
                printf("A STATUS_IND indicating card recovered is sent to the car phone initiated by the user\n");
                CsrBtSapsStatusIndSend(CSR_BT_CARD_RECOVERED);
                break;
            }
        case 'k':
        case 'K':
            {
                printf("A STATUS_IND indicating card reset is sent to the car phone initiated by the user\n");
                CsrBtSapsStatusIndSend(CSR_BT_CARD_RESET);
                break;
            }
        case 'l':
        case 'L':
            {
                printf("A TRANSFER_ATR_REQ is sent to the mobile phone initiated by the user\n");
                CsrBtSapcTransferAtrReqSend();
                break;
            }
        case 'x':
        case 'X':
            {
                instData->commandExpected = TRUE;
                printf("The next incoming command or indication will not be forwarded\n");
                break;
            }
#endif
        default:
            {
                /* Unexpected primitive received */
                system_cls();
                printf("####### default in keybPrim handler %x,\n", key->key );
            }
    }
}


/*********************************************************************
 * A SAPC prim is received originating from the mobile
 * phone. Determine what it is and perform the correct action
 *********************************************************************/
void handleSapcPrim(SapDemoInstanceData_t *instData)
{
    CsrPrim *primType;
    primType = (CsrPrim *)instData->msgP;
    switch(*primType)
    {
        case CSR_BT_SAPC_CONNECT_CFM:
            {
                CsrBtSapcConnectCfm * sapcPrim;
                sapcPrim = (CsrBtSapcConnectCfm *) instData->msgP;
                if (sapcPrim->resultSupplier == CSR_BT_SUPPLIER_SAPC)
                {
                    switch(sapcPrim->resultCode)
                    {
                        case CSR_BT_RESULT_CODE_SAP_CONNECTION_SUCCESS:
                            {
                                printf("Has established a connection to the server (SAPS_CONNECT_CFM)\n");
                                instData->remoteMaxMsgSize = sapcPrim->maxMsgSize;
                                break;
                            }
                        case CSR_BT_RESULT_CODE_SAP_CONNECTION_OK_ONGOING_CALL:
                            {
                                printf("Has established a connection to the server but a call is ongoing: wait for SIM reset (SAPS_CONNECT_CFM)\n");
                                instData->remoteMaxMsgSize = sapcPrim->maxMsgSize;
                                break;
                            }

                        case CSR_BT_RESULT_CODE_SAP_CONNECTION_ATTEMPT_FAILED:
                            {
                                printf("Error, SAP Server unable to establish connection\n");
#ifdef ENABLE_SHUTDOWN
                                CsrSchedStop();
#else
                                exit(1);
#endif
                                return;
                            }
                        case CSR_BT_RESULT_CODE_SAP_CONNECTION_MAX_MSG_SIZE_NOT_SUPPORTED:
                            {
                                printf("Error, SAP Server does not support maximum message size");
#ifdef ENABLE_SHUTDOWN
                                CsrSchedStop();
#else
                                exit(1);
#endif
                                return;

                                break;
                            }
                        case CSR_BT_RESULT_CODE_SAP_CONNECTION_MSG_SIZE_TO_SMALL:
                            {
                                printf("Error, maximum message size by SAP Client is too small");
#ifdef ENABLE_SHUTDOWN
                                CsrSchedStop();
#else
                                exit(1);
#endif
                                return;

                                break;
                            }
                        default:
                            {
                                printf("Unknown result code in the received CSR_BT_SAPC_CONNECT_CFM message!\n");
#ifdef ENABLE_SHUTDOWN
                                CsrSchedStop();
#else
                                exit(1);
#endif
                                return;
                            }
                    }
                    break;
                }
                else
                {
                    printf("Unknown result supplier (%X) / result code (%X) in the received CSR_BT_SAPC_CONNECT_CFM message!\n", sapcPrim->resultSupplier, sapcPrim->resultCode);
#ifdef ENABLE_SHUTDOWN
                    CsrSchedStop();
#else
                    exit(1);
#endif
                }
            }
        case CSR_BT_SAPC_STATUS_IND:
            {
                CsrBtSapcStatusInd *sapcPrim;
                sapcPrim = (CsrBtSapcStatusInd *) instData->msgP;
                printf("Has received a CSR_BT_SAPC_STATUS_IND from the mobile phone\n");
                if( instData->outputLevel == TRUE)
                {
                    printf("....containing STATUS_CHANGE: 0x%x\n",sapcPrim->statusChange);
                }
                /*Send a CSR_BT_SAPS_CONNECT_RES*/
                if(instData->state != connected)
                {
                    CsrBtSapsConnectResSend(CSR_BT_RESULT_CODE_SAP_CONNECTION_SUCCESS, instData->remoteMaxMsgSize, sapcPrim->statusChange);
                    instData->state = connected;
                    /*now a connection is established*/
                    /*Return the passkey length requirement back to default*/
                    scAppSetMinimumPasskeylength(&instData->scInstData, 1);
                }
                else
                {
#ifdef ENABLE_COMMANDS
                    if(instData->commandExpected)
                    {/* Report emidiately */
                        printf("Status change 0x%x received\n",
                               sapcPrim->statusChange);
                    }
                    else
                    { /* forward to sap server */
#endif
                        CsrBtSapsStatusIndSend(sapcPrim->statusChange);
#ifdef ENABLE_COMMANDS
                    }
                    instData->commandExpected = FALSE;
#endif
                }
                break;
            }
        case CSR_BT_SAPC_TRANSFER_ATR_CFM:
            {
                CsrBtSapcTransferAtrCfm *sapcPrim;
                sapcPrim = (CsrBtSapcTransferAtrCfm *) primType;
                printf("Has received a CSR_BT_SAPC_TRANSFER_ATR_CFM from the mobile phone\n");
                if( instData->outputLevel == TRUE)
                {
                    printf("....containing ATR: 0x");
                    printHexString(sapcPrim->atr,sapcPrim->atrLength);
                }
                CsrBtSapsTransferAtrResSend(sapcPrim->resultCode,sapcPrim->atr,sapcPrim->atrLength);
                break;
            }
        case CSR_BT_SAPC_TRANSFER_APDU_CFM:
            {
                CsrBtSapcTransferApduCfm *sapcPrim;
                sapcPrim = (CsrBtSapcTransferApduCfm *) primType;
                printf("Has received a CSR_BT_SAPC_TRANSFER_APDU_CFM from the mobile phone\n");
                if( instData->outputLevel == TRUE)
                {
                    printf("....containing the response APDU: 0x");
                    printHexString(sapcPrim->responseApdu,sapcPrim->responseApduLength);
                }
                CsrBtSapsTransferApduResSend(sapcPrim->resultCode, sapcPrim->responseApdu, sapcPrim->responseApduLength);
                break;
            }
        case CSR_BT_SAPC_DISCONNECT_IND:
            {
                CsrBtSapcDisconnectInd *sapcPrim;
                sapcPrim = (CsrBtSapcDisconnectInd *) primType;
                if(sapcPrim->reasonSupplier == CSR_BT_SUPPLIER_SAPC &&
                   sapcPrim->reasonCode == CSR_BT_RESULT_CODE_SAP_IMMEDIATE_CONNECTION_TERM_BY_PEER)
                {
                    printf("Has received an CSR_BT_SAPC_DISCONNECT_IND with reason %X, supplier: %X from the mobile phone\n",
                           sapcPrim->reasonCode, sapcPrim->reasonSupplier);
                    printf("....containing an immediate requirement, hence ending session \n");
                    if(instData->state != disconnecting)
                    {
                        CsrBtSapsDisconnectReqSend(CSR_BT_IMMEDIATE_DISCONNECT);
                        instData->state = disconnecting;
                    }
                    else
                    {
                        instData->state = idle;
                        playIntro(instData);
                    }
                }
                else if(sapcPrim->reasonSupplier == CSR_BT_SUPPLIER_SAPC &&
                        sapcPrim->reasonCode == CSR_BT_RESULT_CODE_SAP_GRACEFUL_CONNECTION_TERM_BY_PEER)
                {
                    printf("Has received a CSR_BT_SAPC_DISCONNECT_IND with reason %X, supplier: %X from the mobile phone\n",
                           sapcPrim->reasonCode, sapcPrim->reasonSupplier);
                    printf("....containing a graceful disconnect\n");
                    CsrBtSapsDisconnectReqSend(CSR_BT_GRACEFUL_DISCONNECT);
                }
                break;
            }
        case CSR_BT_SAPC_RESET_SIM_CFM:
            {
                CsrBtSapcResetSimCfm *sapcPrim;
                sapcPrim = (CsrBtSapcResetSimCfm *) primType;
                printf("Has received a CSR_BT_SAPC_RESET_SIM_CFM from the mobile phone");
                printf("....containing the following result code: 0x%x\n",sapcPrim->resultCode);
#ifdef ENABLE_COMMANDS
                if (instData->commandSent == FALSE)
                {
#endif
                    CsrBtSapsResetSimResSend(sapcPrim->resultCode);
#ifdef ENABLE_COMMANDS
                }
                instData->commandSent = FALSE;
#endif

                break;
            }
        case CSR_BT_SAPC_POWER_SIM_ON_CFM:
            {
                CsrBtSapcPowerSimOnCfm *sapcPrim;
                sapcPrim = (CsrBtSapcPowerSimOnCfm *) primType;
                printf("Has received a CSR_BT_SAPC_POWER_SIM_ON_CFM from the mobile phone");
                printf("....containing the following result code: 0x%x\n",sapcPrim->resultCode);
#ifdef ENABLE_COMMANDS
                if (instData->commandSent == FALSE)
                {
#endif
                    CsrBtSapsPowerSimOnResSend(sapcPrim->resultCode);
#ifdef ENABLE_COMMANDS
                }
                instData->commandSent = FALSE;
#endif
                break;
            }
        case CSR_BT_SAPC_POWER_SIM_OFF_CFM:
            {
                CsrBtSapcPowerSimOffCfm *sapcPrim;
                sapcPrim = (CsrBtSapcPowerSimOffCfm *) primType;
                printf("Has received a CSR_BT_SAPC_POWER_SIM_OFF_CFM from the mobile phone");
                printf("....containing the following result code: 0x%x\n",sapcPrim->resultCode);
#ifdef ENABLE_COMMANDS
                if (instData->commandSent == FALSE)
                {
#endif
                    CsrBtSapsPowerSimOffResSend(sapcPrim->resultCode);
#ifdef ENABLE_COMMANDS
                }
                instData->commandSent = FALSE;
#endif
                break;
            }
        case CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_CFM:
            {
                CsrBtSapcTransferCardReaderStatusCfm *sapcPrim;
                sapcPrim = (CsrBtSapcTransferCardReaderStatusCfm *) primType;
                printf("Has received a CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_CFM from the mobile phone\n");
                printf("....containing the following result code: 0x%x\n",sapcPrim->resultCode);
                printf("    and the following card reader status: 0x%x\n",sapcPrim->cardReaderStatus);
#ifdef ENABLE_COMMANDS
                if (instData->commandSent == FALSE)
                {
#endif
                    CsrBtSapsTransferCardReaderStatusResSend(sapcPrim->resultCode, sapcPrim->cardReaderStatus);
#ifdef ENABLE_COMMANDS
                }
                instData->commandSent = FALSE;
#endif
                break;
            }
        case CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_CFM:
            {
                CsrBtSapcSetTransferProtocolCfm *sapcPrim;
                sapcPrim = (CsrBtSapcSetTransferProtocolCfm *)primType;
                printf("Has received a CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_CFM from the mobile phone\n");
                printf("....containing the following result code: 0x%x\n", sapcPrim->resultCode);

#ifdef ENABLE_COMMANDS
                if (instData->commandSent == FALSE)
                {
#endif
                    CsrBtSapsSetTransferProtocolResSend(sapcPrim->resultCode);
#ifdef ENABLE_COMMANDS
                }
                instData->commandSent = FALSE;
#endif
                break;
            }
        default:
            {
                printf("Has received a SAPC_PRIM but it was not right: %d!!\n",*primType);
                CsrBtSapcFreeUpstreamMessageContents(CSR_BT_SAPC_PRIM, instData->msgP);
#ifdef ENABLE_SHUTDOWN
                CsrSchedStop();
#else
                exit(1);
#endif
                return;

            }
    }
}

/*********************************************************************
 * A SAPS prim is received originating from the car phone. Determine
 * what it is and perform the correct action
 *********************************************************************/
void handleSapsPrim(SapDemoInstanceData_t *instData)
{
    CsrPrim *primType;
    primType = (CsrPrim *)instData->msgP;
    switch(*primType)
    {
        case CSR_BT_SAPS_ACTIVATE_CFM:
            {
                instData->serverActivated = TRUE;
                printf("SAP server activated!\n");
                playIntro(instData);
                break;
            }
        case CSR_BT_SAPS_DEACTIVATE_CFM:
            {
                instData->serverActivated = FALSE;
                printf("SAP server de-activated!\n");
                playIntro(instData);
                break;
            }
        case CSR_BT_SAPS_CONNECT_IND:
            {
                CsrBtSapsConnectInd *prim;
                prim = (CsrBtSapsConnectInd *) primType;
                printf("Has received a CSR_BT_SAPS_CONNECT_IND - connecting to the mobile phone (accept it on the mobile)\n");
                /*connect to the sapc with the maxmsgsize proposed by the carkit*/
                if(instData->selectedDevice == 255)
                {
                    /*using default*/
                    CsrBtSapcConnectReqSend(instData->CsrSchedQid,
                                       prim->maxMsgSize,
                                       instData->bdAddr);
                }
                else
                {
                    /*using selected*/
                    CsrBtSapcConnectReqSend(instData->CsrSchedQid,
                                       prim->maxMsgSize,
                                       instData->bdAddrList[instData->selectedDevice]);
                }
                /* Ensure that SC knows to require a minimum length of 16
                   for a SAP connection */
                scAppSetMinimumPasskeylength(&instData->scInstData, 16);
                break;
            }
        case CSR_BT_SAPS_TRANSFER_ATR_IND:
            {
                printf("Has received a CSR_BT_SAPS_TRANSFER_ATR_IND from the carkit client\n");
                CsrBtSapcTransferAtrReqSend();
                break;
            }
        case CSR_BT_SAPS_TRANSFER_APDU_IND:
            {
                CsrBtSapsTransferApduInd *prim;
                CsrUint8 * tmpP;
                tmpP = CsrPmemAlloc(4 * sizeof(CsrUint8));
                prim = (CsrBtSapsTransferApduInd *) primType;
                printf("Has received a CSR_BT_SAPS_TRANSFER_APDU_IND from the carkit client\n");
                if( instData->outputLevel == TRUE)
                {
                    printf("....containing the command APDU: 0x");
                    printHexString(prim->apduCommand,prim->apduCommandLength);
                }
                CsrBtSapcTransferApduReqSend(prim->apduCommand,prim->apduCommandLength, FALSE);
                break;
            }
        case CSR_BT_SAPS_DISCONNECT_IND:
            {
                CsrBtSapsDisconnectInd *prim;
                prim = (CsrBtSapsDisconnectInd *) primType;
                printf("Has received a CSR_BT_SAPS_DISCONNECT_IND from the carkit client with with reason %X, supplier: %X\n", prim->reasonCode, prim->reasonSupplier);
                if (instData->state != disconnecting)
                {
                    CsrBtSapcDisconnectReqSend();
                    instData->state = disconnecting;
                }
                else
                {
                    instData->state = idle;
                    playIntro(instData);
                }
                break;
            }
        case CSR_BT_SAPS_RESET_SIM_IND:
            {
                printf("Has received a CSR_BT_SAPS_RESET_SIM_IND from the carkit client\n");
#ifdef ENABLE_COMMANDS
                if(instData->commandExpected)
                {/* Respond emidiately */
                    CsrBtSapsResetSimResSend(CSR_BT_RESULT_CODE_SAP_PROTOCOL_SUCCESS);
                }
                else
                { /* forward to sap client */
#endif
                    CsrBtSapcResetSimReqSend();
#ifdef ENABLE_COMMANDS
                }
                instData->commandExpected = FALSE;
#endif
                break;
            }
        case CSR_BT_SAPS_POWER_SIM_ON_IND:
            {
                printf("Has received a SAPS_POWER_SIM_ON from the carkit client\n");
#ifdef ENABLE_COMMANDS
                if(instData->commandExpected)
                {/* Respond emidiately */
                    CsrBtSapsPowerSimOnResSend(CSR_BT_RESULT_CODE_SAP_PROTOCOL_SUCCESS);
                }
                else
                { /* forward to sap client */
#endif
                    CsrBtSapcPowerSimOnReqSend();
#ifdef ENABLE_COMMANDS
                }
                instData->commandExpected = FALSE;
#endif
                break;
            }
        case CSR_BT_SAPS_POWER_SIM_OFF_IND:
            {
                printf("Has received a SAPS_POWER_SIM_OFF from the carkit client\n");
#ifdef ENABLE_COMMANDS
                if(instData->commandExpected)
                {/* Respond emidiately */
                    CsrBtSapsPowerSimOffResSend(CSR_BT_RESULT_CODE_SAP_PROTOCOL_SUCCESS);
                }
                else
                { /* forward to sap client */
#endif
                    CsrBtSapcPowerSimOffReqSend();
#ifdef ENABLE_COMMANDS
                }
                instData->commandExpected = FALSE;
#endif
                break;
            }
        case CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_IND:
            {
                printf("Has received a CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_IND from the carkit client\n");
#ifdef ENABLE_COMMANDS
                if(instData->commandExpected)
                { /* Respond emidiately */
                    CsrBtSapsTransferCardReaderStatusResSend(CSR_BT_RESULT_CODE_SAP_PROTOCOL_SUCCESS, 0);
                }
                else
                { /* forward to sap client */
#endif
                    CsrBtSapcTransferCardReaderStatusReqSend();
#ifdef ENABLE_COMMANDS
                }
                instData->commandExpected = FALSE;
#endif
                break;
            }
        case CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_IND:
            {
                CsrBtSapsSetTransferProtocolInd *prim;
                prim = (CsrBtSapsSetTransferProtocolInd *)primType;

                printf("Has received a CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_IND using protocol %d from the carkit client\n",prim->protocol);
#ifdef ENABLE_COMMANDS
                if(instData->commandExpected)
                {/* Respond emidiately */
                    CsrBtSapsSetTransferProtocolResSend(CSR_BT_RESULT_CODE_SAP_PROTOCOL_SUCCESS);
                }
                else
                { /* forward to sap client */
#endif
                    CsrBtSapcSetTransferProtocolReqSend(prim->protocol);
#ifdef ENABLE_COMMANDS
                }
                instData->commandExpected = FALSE;
#endif
                break;
            }
        default:
            {
                printf("Has received a SAPS_PRIM but it was not right: %d!!\n", *primType);
                CsrBtSapsFreeUpstreamMessageContents(CSR_BT_SAPS_PRIM, instData->msgP);
#ifdef ENABLE_SHUTDOWN
                CsrSchedStop();
#else
                exit(1);
#endif
                return;
            }
    }
}

/*********************************************************************
 *  printHexString
 *********************************************************************/
void printHexString(CsrUint8 * hexString, CsrUint16 length)
{
    int i;
    for (i=0;i<length;i++)
    {
        printf("%x",*hexString);
        hexString++;
    }
    printf("\n");
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
