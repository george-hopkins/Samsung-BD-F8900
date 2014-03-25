/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "csr_pmem.h"
#include "csr_bt_profiles.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_hcrp_lib.h"
#include "csr_bt_hcrp_prim.h"
#include "csr_bt_platform.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_demoapp.h"
#include "csr_sched_init.h"

#define KEYB_CHECK_TIMEOUT                (250000)
#define MAX_ACCEPT_CONNECT_TIME           (100)
#define MAX_DISCOVERY_RESULTS_IN_LIST     (10)
#define HCRP_FILE                         "hcrp_file.dat"
#define IEEE1284                          "MFG:HEWLETT-PACKARD;MDL:DESKJET 950C;CLASS:PRINTER;CMD:MLC,PCL,PML;"

/* demoapp states */
typedef CsrUint8 hcrpDemoStates_t;
#define  null (hcrpDemoStates_t) 0x00
#define  idle (hcrpDemoStates_t) 0x01
#define  connecting (hcrpDemoStates_t) 0x02
#define  acceptConnection (hcrpDemoStates_t) 0x03
#define  connected (hcrpDemoStates_t) 0x04
#define  disconnecting (hcrpDemoStates_t) 0x05
#define  parked (hcrpDemoStates_t) 0x06
#define  inquiry (hcrpDemoStates_t) 0x07
#define  pairing (hcrpDemoStates_t) 0x08

/* demoapp instance data */
typedef struct
{
    CsrSchedQid           CsrSchedQid;
    hcrpDemoStates_t    state;
    hcrpDemoStates_t    prevState;
    void*               msgP;
    CsrBtDeviceAddr        bdAddrList[MAX_DISCOVERY_RESULTS_IN_LIST];
    CsrUint8             numInList;
    CsrUint8             selectedDevice;
    CsrUint16            payloadLength;
    CsrUint8*            payload;
    CsrFileHandle                *file;
    ScAppInstData_t     scInstData;
} hcrpDemoInstanceData_t;

/* function prototypes for local functions. */
static void handleHcrpPrim(hcrpDemoInstanceData_t *instData);
static void handleKeybPrim(hcrpDemoInstanceData_t *instData);
static void handleCmPrim(hcrpDemoInstanceData_t *instData);
static void handleGetLptStatusInd(CsrUint16 pduId, CsrUint16 transId);
static void handleGetIeee1284Ind(CsrUint16 pduId, CsrUint16 transId);
static void handleDataL2CaConnectInd(hcrpDemoInstanceData_t *instData);
static void handleDataL2CaDisconnectInd(hcrpDemoInstanceData_t *instData);
static void handleDataL2DataInd(hcrpDemoInstanceData_t *instData);

void KeyMessageService(CsrUint16 m, void *data)
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

/************************* local utility functions    **************************/

void playIntro()
{
    printf("\n\n");
    printf("Options: \n\n");
    printf("  1) Activate HCRP server\n");
    printf("  2) Deactive HCRP server\n");
    printf("  3) Security Controller Configuration\n");
    printf("\n");
}

static void hcrpScAppSetFnFini(void *pFiniData)
{
    hcrpDemoInstanceData_t *instData = (hcrpDemoInstanceData_t *) pFiniData;
    scAppRestoreState((CsrUint8 *) &instData->prevState, (CsrUint8 *) &instData->state);
    system_cls();
    playIntro();
}

static void hcrpScAppSetFnInit(void *pInitData)
{
    hcrpDemoInstanceData_t *instData = (hcrpDemoInstanceData_t *) pInitData;
    scAppSaveState((CsrUint8 *) &instData->prevState, (CsrUint8 *) &instData->state, pairing);
}

static void* hcrpScAppSetFnGetMessage(void *pRecvMsgPData)
{
    hcrpDemoInstanceData_t *instData = (hcrpDemoInstanceData_t *) pRecvMsgPData;
    void *result                     = instData->msgP;
    instData->msgP                   = NULL;
    return result;
}

void CsrBtAppInit(void** gash)
{
    hcrpDemoInstanceData_t *instData;

    *gash = (void*) CsrPmemAlloc(sizeof(hcrpDemoInstanceData_t));
    instData = (hcrpDemoInstanceData_t*) *gash;
    instData->state = idle;

    /* Application handle */
    instData->CsrSchedQid = TESTQUEUE;

    printf("\nHCRP Server Demo:\n\n");
    printf("Hit <ESC> to quit program!\n\n");

    playIntro();
    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, NULL);

    /* Security App Configuration */
    CsrBtScActivateReqSend(instData->CsrSchedQid);
    scAppInitInstanceData(&instData->scInstData);

    scAppSetFnFini(&instData->scInstData, hcrpScAppSetFnFini, instData);
    scAppSetFnInit(&instData->scInstData, hcrpScAppSetFnInit, instData);
    scAppSetSecLevelSend(&instData->scInstData, scAppSendProfileSecurityLevelReq, &instData->scInstData);

    scAppSetFnGetMessage(&instData->scInstData, hcrpScAppSetFnGetMessage, instData);

    scAppSetSecLevelPrim(&instData->scInstData,
                         0,     /* Index 0. Single profile */
                         "HCRP", CSR_BT_HCRP_PRIM,
                         CSR_BT_HCRP_IFACEQUEUE,
                         CSR_BT_HCRP_SECURITY_IN_REQ, SC_APP_SET_SEC_LEVEL_NA);
}

#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void** gash)
{
    CsrUint16 eventType=0;
    void* prim=NULL;
    hcrpDemoInstanceData_t *instData;

    instData = (hcrpDemoInstanceData_t*) *gash;

    scAppDeinitInstanceData(&instData->scInstData);

    /* get a message from the demoapplication message queue. The
     * message is returned in prim and the event type in eventType
 */
    while (CsrSchedMessageGet(&eventType, &prim))
    {
        switch(eventType)
        {
            case CSR_BT_HCRP_PRIM:
            {
                CsrBtHcrpFreeUpstreamMessageContents(eventType, prim);
                break;
            }
            case CSR_BT_SC_PRIM:
            {
                CsrBtScFreeUpstreamMessageContents(eventType, prim);
                break;
            }
            case CSR_BT_CM_PRIM:
            {
                CsrBtCmFreeUpstreamMessageContents(eventType, prim);
                break;
            }
        }
    }

    CsrPmemFree(instData);
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
    CsrUint16 eventType=0;
    void* prim=NULL;
    hcrpDemoInstanceData_t *instData;

    instData = (hcrpDemoInstanceData_t*) *gash;
    /* get a message from the demoapplication message queue. The message is returned in prim
          and the event type in eventType */
    CsrSchedMessageGet(&eventType, &prim);
    instData->msgP = prim;

    switch (eventType)
    {
        case CSR_BT_HCRP_PRIM:
        {
            CsrPrim *prim = (CsrPrim *) instData->msgP;
            switch(*prim)
            {
                case CSR_BT_HCRP_SECURITY_IN_CFM:
                    handleScSecLevelPrim(&instData->scInstData);
                    break;

                default:
                    handleHcrpPrim(instData);
                    break;
            }
            break;
        }

        case CSR_BT_SC_PRIM:
            handleScPrim(&instData->scInstData);
            break;

        case CSR_BT_CM_PRIM:
            handleCmPrim(instData);
            break;

        case KEY_MESSAGE:
            handleKeybPrim(instData);
            break;

        default:
            printf("####### csr_bt_hcrp_demo_app.c: default in testhandler %x,\n",eventType);
            break;
    }

    /* free the received signal again. If the signal for some reason
     * must be stored in the application the pointer (prim) must be
     * set to NULL in order not to free it here
 */
    CsrPmemFree(instData->msgP);
}

/*****************************    HCRP PRIMITIVES    ******************************/

static void handleHcrpPrim(hcrpDemoInstanceData_t *instData)
{
    CsrBtHcrpPrim *pMsg = instData->msgP;

    switch((CsrBtHcrpPrim)*pMsg)
    {
        case CSR_BT_HCRP_GET_LPT_STATUS_IND:
            {
                CsrBtHcrpGetLptStatusInd *hcrpGetLptStatusInd = (CsrBtHcrpGetLptStatusInd*)pMsg;

                printf("HCRP_GET_LPT_STATUS_REQ:\n");
                handleGetLptStatusInd(hcrpGetLptStatusInd->pduId,
                                      hcrpGetLptStatusInd->transId);
                break;
            }

        case CSR_BT_HCRP_GET_IEEE_1284_IND:
            {
                CsrBtHcrpGetIeee1284Ind *hcrpGetIeee1284Ind = (CsrBtHcrpGetIeee1284Ind*) pMsg;

                printf("HCRP_GET_IEEE_1284_REQ:\n");
                handleGetIeee1284Ind(hcrpGetIeee1284Ind->pduId,
                                     hcrpGetIeee1284Ind->transId);
                break;
            }

        case CSR_BT_HCRP_L2CA_CONNECT_IND:
            /* HCRP initialisation stage 8 - the HCRP data channel connection was
               successful. */
            printf("HCRP_DATA_L2CA_CONNECT_IND:\n");
            handleDataL2CaConnectInd(instData);
            break;

        case CSR_BT_HCRP_L2CA_DISCONNECT_IND:
            printf("HCRP_DATA_L2CA_DISCONNECT_IND:\n");
            handleDataL2CaDisconnectInd(instData);
            break;

        case CSR_BT_HCRP_L2CA_DATA_IND:
            {
                CsrBtHcrpL2caDataInd *hcrpDataL2CaDataInd = (CsrBtHcrpL2caDataInd*) pMsg;

                printf("HCRP_DATA_L2CA_DATA_IND:\n");

                instData->payloadLength = hcrpDataL2CaDataInd->payloadLength;
                instData->payload = hcrpDataL2CaDataInd->payload;

                handleDataL2DataInd(instData);
                break;
            }

        default:
            printf("####### csr_bt_hcrp_demo_app.c: Unhandled HCRP signal: %x received\n", *pMsg);
            CsrBtHcrpFreeUpstreamMessageContents(CSR_BT_HCRP_PRIM, pMsg);
            break;
    }
}

static void handleGetLptStatusInd(CsrUint16 pduId, CsrUint16 transId)
{
    CsrBtHcrpGetLptStatusResSend(pduId, transId, 0x18);
}

static void handleGetIeee1284Ind(CsrUint16 pduId, CsrUint16 transId)
{
    CsrCharString *str;

    str = (CsrCharString*)CsrPmemAlloc(CsrStrLen(IEEE1284) + 1);
    CsrStrCpy((char*)str, IEEE1284);

    CsrBtHcrpGetIeee1284ResSend(pduId, transId, (CsrUtf8String*)str);
}

static void handleDataL2CaConnectInd(hcrpDemoInstanceData_t *instData)
{
    instData->file = CsrBtFopen(HCRP_FILE, "wb");

    if (instData->file != NULL)
    {
        printf(HCRP_FILE" opened for write\n");
    }
    else
    {
        printf("Could not open "HCRP_FILE" for write\n");
    }
}

static void handleDataL2CaDisconnectInd(hcrpDemoInstanceData_t *instData)
{
    CsrBtFclose(instData->file);
    instData->file = NULL;
}

static void handleDataL2DataInd(hcrpDemoInstanceData_t *instData)
{
    CsrUint32 wr;

    if((instData->payload != NULL) &&
       (instData->payloadLength > 0) &&
       (instData->file != NULL))
    {
        wr = CsrBtFwrite(instData->payload,
                       1,
                       instData->payloadLength,
                       instData->file);

        printf("Wrote %i of %i bytes\n", wr, instData->payloadLength);
        CsrPmemFree(instData->payload);
        instData->payload = NULL;
        instData->payloadLength = 0;
    }
}

/*****************************    CM PRIMITIVES    ******************************/

static void handleCmPrim(hcrpDemoInstanceData_t *instData)
{
    CsrPrim *primType;

    primType = (CsrPrim*) instData->msgP;

    switch (*primType)
    {
        default:
            printf("####### csr_bt_hcrp_demo_app.c: default in CmPrim handler %x,\n", *primType);
            CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, instData->msgP);
            break;
    }
}

/**************************************************************************************************
 *
 *    function to handle all keyboard presses (sent as a keyboard event signal).
 *
 **************************************************************************************************/

void activate(hcrpDemoInstanceData_t *instData)
{
    instData->state = acceptConnection;
    CsrBtHcrpActivateReqSend(instData->CsrSchedQid,
                        (CsrUint8*)IEEE1284);
    printf("Activated...\n");
}

void deactivate(hcrpDemoInstanceData_t *instData)
{
    instData->state = disconnecting;
    CsrBtHcrpDeactivateReqSend();
    printf("Deactivated...\n");
}


/**************************************************************************************************
 *
 *    a keyboard event is received.
 *    first check for selection of a found device during inquiry if state is inquiry. If not start
 *    proper action as selected.
 *
 **************************************************************************************************/
static void handleKeybPrim(hcrpDemoInstanceData_t *instData)
{
    CsrBtKeyPress_t *key;
    key = (CsrBtKeyPress_t*)instData->msgP;

    if (instData->state == pairing)
    {
        handleScKeybPrim(&instData->scInstData);
    }
    else
    {
        switch(key->key)
        {
            case '1':
                {
                    activate(instData);
                    break;
                }

            case '2':
                {
                    deactivate(instData);
                    break;
                }

            case '3':
                {
                    handleScKeybPrim(&instData->scInstData);
                    break;
                }

            case ESCAPE_KEY:
                {
                    printf("\nUser exit...\n");
#ifdef ENABLE_SHUTDOWN
                    CsrSchedStop();
#else
                    exit(0);
#endif
                    return;
                }

            default:
                printf("####### csr_bt_hcrp_demo_app.c: default in keybPrim handler %x,\n", *((CsrUint8*) instData->msgP));
                break;
        }
    }
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
