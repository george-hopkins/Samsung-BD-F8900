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

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_bt_tasks.h"
#include "bluetooth.h"
#include "csr_bt_profiles.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_dg_prim.h"
#include "csr_bt_dg_lib.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_iwu_prim.h"
#include "csr_bt_iwu_handler.h"
#include "csr_bt_at_lib.h"
#include "csr_bt_platform.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_sched_init.h"

#ifdef __linux__
#define DEFAULT_EXTRAPORT    "/dev/ttyS1"
#else
#define DEFAULT_EXTRAPORT    "COM2"
#endif
#define KEYB_CHECK_TIMEOUT   (100000)

char extra_ComPort[128]       = DEFAULT_EXTRAPORT;

/* demoapp states */
typedef CsrUint8 dunDemoStates_t;
#define  null (dunDemoStates_t) 0x00
#define  idle (dunDemoStates_t) 0x01
#define  pairing (dunDemoStates_t) 0x02

/* demoapp instance data */
typedef struct
{
    void                *msgP;
    CsrSchedQid           CsrSchedQid;
    ScAppInstData_t     scInstData;
    dunDemoStates_t        DunState;
    dunDemoStates_t        prevDunState;
    CsrBool              discoverable;
} dunDemoInstanceData_t;


void ClearDunState(void)
{
    IWU_UPDATE_DUNGW_STATE_REQ_T * prim;

    prim                          = (IWU_UPDATE_DUNGW_STATE_REQ_T *) CsrPmemAlloc(sizeof(IWU_UPDATE_DUNGW_STATE_REQ_T));
    prim->type                    = IWU_UPDATE_DUNGW_STATE_REQ;
    prim->newState                = idle;
    CsrSchedMessagePut(TESTQUEUE, CSR_BT_IWU_PRIM, prim);

    /*DunState = idle;*/
}

/*    forward declarations for handler functions */
static void handleKeybPrim(dunDemoInstanceData_t *instData);

/* insert a call to this function in the "forever" loop in CsrSched() */
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
    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, NULL);
}

static void playIntro(dunDemoInstanceData_t *instData)
{
    printf("     Welcome to the Dial-up Networking demo: \n\n");
    printf("     Hit <Esc> to exit\n\n");
    printf("     Options:\n");

    printf("          1)  Activate service\n");
    printf("          2)  Deactivate service\n");
    printf("          3)  Release a connection\n");
    printf("          4)  Security Controller Configuration\n");
    if (instData->discoverable)
    {
        printf("          5)  Make non-discoverable\n");
    }
    else
    {
        printf("          5)  Make discoverable\n");
    }
    printf("\n          t)  Terminate on-going call (send ATH command to modem)\n");

    printf("\n");
}

void handleDataPathStatusInd(IWU_PRIM_T *prim)
{
    IWU_DATA_PATH_STATUS_IND_T *dpsPrim;
    char *actions[3] = {"Opened", "Closed", "Lost"};
    char *what;

    dpsPrim = (IWU_DATA_PATH_STATUS_IND_T*)prim;
    what = actions[dpsPrim->status];

    printf("\nData path status indication (dg queue %i): %s\n",
           dpsPrim->dgInstanceQueue, what);

}

static void dungScAppSetFnFini(void *pFiniData)
{
    dunDemoInstanceData_t *instData = (dunDemoInstanceData_t *)pFiniData;

    scAppRestoreState((CsrUint8 *) &instData->prevDunState, (CsrUint8 *) &instData->DunState);
    system(CLS);
    playIntro(instData);
}

static void dungScAppSetFnInit(void *pInitData)
{
    dunDemoInstanceData_t *instData = (dunDemoInstanceData_t *)pInitData;

    scAppSaveState((CsrUint8 *) &instData->prevDunState, (CsrUint8 *) &instData->DunState, pairing);
}

static void* dungScAppSetFnGetMessage(void *pRecvMsgPData)
{
    dunDemoInstanceData_t *instData = (dunDemoInstanceData_t *) pRecvMsgPData;
    void *result   = instData->msgP;
    instData->msgP = NULL;
    return result;
}

/******************************************************************************
 *   init function called by the scheduler upon initialisation.
 *   This function is used to boot the demo application.
 *****************************************************************************/
void CsrBtAppInit(void **gash)
{
    dunDemoInstanceData_t  *instData;

    *gash                = (void *) CsrPmemAlloc(sizeof(dunDemoInstanceData_t));
    instData            = (dunDemoInstanceData_t *) *gash;
    instData->msgP        = NULL;

    instData->CsrSchedQid       = TESTQUEUE;
    instData->discoverable = TRUE;
    ClearDunState();
    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, NULL);

    /* Security App Configuration */
    CsrBtScActivateReqSend(instData->CsrSchedQid);
    scAppInitInstanceData(&instData->scInstData);

    scAppSetFnFini(&instData->scInstData, dungScAppSetFnFini, instData);
    scAppSetFnInit(&instData->scInstData, dungScAppSetFnInit, instData);
    scAppSetSecLevelSend(&instData->scInstData, scAppSendProfileSecurityLevelReq, &instData->scInstData);

    scAppSetFnGetMessage(&instData->scInstData, dungScAppSetFnGetMessage, instData);

    scAppSetSecLevelPrim(&instData->scInstData,
                         0,     /* Index 0. Single profile */
                         "DG", CSR_BT_DG_PRIM,
                         CSR_BT_DG_IFACEQUEUE,
                         CSR_BT_DG_SECURITY_IN_REQ, SC_APP_SET_SEC_LEVEL_NA);
}

/******************************************************************************
 *   The CsrBtAppDeinit function called by the scheduler upon de-initialisation.
 *****************************************************************************/
#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    void                    * msg=NULL;
    dunDemoInstanceData_t    * instData;
    CsrUint16                eventClass=0;

    instData    = (dunDemoInstanceData_t *) *gash;

    scAppDeinitInstanceData(&instData->scInstData);

    while (CsrSchedMessageGet(&eventClass, &msg))
    {
        switch(eventClass){
            case CSR_BT_DG_PRIM:
            {
                CsrBtDgFreeUpstreamMessageContents(eventClass, msg);
                break;
            }
            case CSR_BT_SC_PRIM:
            {
                CsrBtScFreeUpstreamMessageContents(eventClass, msg);
                break;
            }
            case CSR_BT_CM_PRIM:
            {
                CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
                break;
            }
            default:
            {
                break;
            }
        }
        CsrPmemFree(msg);
    }
    CsrPmemFree(instData);
}
#endif


/******************************************************************************
 *    this is the demo application handler function. All primitives sent
 *    to the demo application will be received in here. The scheduler
 *    ensures that the function is activated when a signal is put on the
 *    demo application signal queue.
 *****************************************************************************/
void CsrBtAppHandler(void **gash)
{
    CsrUint16                eventType=0;
    void                    *prim=NULL;
    dunDemoInstanceData_t    *instData;

    instData = (dunDemoInstanceData_t *) *gash;
    /*  get a message from the demoapplication message queue.
     * The message is returned in prim and the event type in eventType */
    CsrSchedMessageGet(&eventType, &prim);
    instData->msgP = prim;

    switch (eventType)
    {
        case CSR_BT_SC_PRIM:
            {
                handleScPrim(&instData->scInstData);
                break;
            }
        case KEY_MESSAGE:
            {
                handleKeybPrim(instData);
                break;
            }
        case CSR_BT_DG_PRIM :
            {
                CsrPrim *prim = (CsrPrim *) instData->msgP;
                switch(*prim)
                {
                    case CSR_BT_DG_SECURITY_IN_CFM:
                        handleScSecLevelPrim(&instData->scInstData);
                        break;

                    default:
                        printf("####### default in testhandler %x,\n",eventType);
                        break;
                }
                break;
            }
        case CSR_BT_IWU_PRIM :
            {
                CsrPrim *primType;
                primType = (CsrPrim*)prim;
                switch(*primType)
                {
                    case IWU_DATA_PATH_STATUS_IND:
                        handleDataPathStatusInd(prim);
                        break;

                    case IWU_UPDATE_DUNGW_STATE_REQ:
                    {
                        instData->DunState = ((IWU_UPDATE_DUNGW_STATE_REQ_T *)prim)->newState;
                        break;
                    }

                    default:
                        playIntro(instData);
                        break;
                }
                break;
            }
        case CSR_BT_CM_PRIM:
            {
                CsrPrim *prim = (CsrPrim *) instData->msgP;
                switch(*prim)
                {
                    case CSR_BT_CM_WRITE_SCAN_ENABLE_CFM:
                    {
                        playIntro(instData);
                        break;
                    }
                    default:
                    {
                        printf("Unhandled CM prim 0x%2x received\n", *prim);
                        CsrBtCmFreeUpstreamMessageContents(eventType, instData->msgP);
                        break;
                    }
                }
                break;
            }
        default:
            {
                system(CLS);
                printf("####### default in testhandler %x\n",eventType);
                break;
            }
    }    /*    end switch(eventType) */

    /* free the received signal again. If the signal for some reason
     * must be stored in the application the pointer (prim) must be
     * set to NULL in order not to free it here */
    CsrPmemFree(instData->msgP);
}


/******************************************************************************
 * a keyboard event is received.
 * first check for selection of a found device during inquiry if state
 * is inquiry. If not start proper action as selected.
 *****************************************************************************/
static void handleKeybPrim(dunDemoInstanceData_t *instData)
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

    if (instData->DunState == pairing)
    {
        handleScKeybPrim(&instData->scInstData);
    }
    else
    {
        switch (key->key)
        {
            case '1':
                {
                    IWU_ACTIVATE_REQ_T    *prim;

                    prim = (IWU_ACTIVATE_REQ_T *)CsrPmemAlloc(sizeof(IWU_ACTIVATE_REQ_T));
                    prim->type = IWU_ACTIVATE_REQ;
                    CsrSchedMessagePut(CSR_BT_IWU_IFACEQUEUE, CSR_BT_IWU_PRIM, prim);
                    break;
                }
            case '2':
                {
                    IWU_DEACTIVATE_REQ_T    *prim;

                    prim = (IWU_DEACTIVATE_REQ_T *)CsrPmemAlloc(sizeof(IWU_DEACTIVATE_REQ_T));
                    prim->type = IWU_DEACTIVATE_REQ;
                    CsrSchedMessagePut(CSR_BT_IWU_IFACEQUEUE, CSR_BT_IWU_PRIM, prim);
                    break;
                }
            case '3':
                {
                    IWU_RELEASE_REQ_T    *prim;

                    prim = (IWU_RELEASE_REQ_T *)CsrPmemAlloc(sizeof(IWU_RELEASE_REQ_T));
                    prim->type = IWU_RELEASE_REQ;
                    CsrSchedMessagePut(CSR_BT_IWU_IFACEQUEUE, CSR_BT_IWU_PRIM, prim);
                    break;
                }
            case '4':
                {
                    handleScKeybPrim(&instData->scInstData);
                    break;
                }
            case 't':
            case 'T':
                {
                    CsrBtAtDgAtcHInd    *prim;

                    prim = (CsrBtAtDgAtcHInd *)CsrPmemAlloc(sizeof(CsrBtAtDgAtcHInd));
                    prim->type = CSR_BT_AT_DG_ATC_H_IND;
                    CsrSchedMessagePut(CSR_BT_IWU_IFACEQUEUE, CSR_BT_AT_PRIM, prim);
                    break;
                }
            case '5':
                {
                    if (instData->discoverable)
                    {
                        CsrBtCmWriteScanEnableReqSend(instData->CsrSchedQid,
                                                 TRUE,
                                                 FALSE);
                        instData->discoverable = FALSE;
                        printf("Made non-discoverable\n");
                    }
                    else
                    {
                        CsrBtCmWriteScanEnableReqSend(instData->CsrSchedQid,
                                                 FALSE,
                                                 FALSE);
                        instData->discoverable = TRUE;
                        printf("Made discoverable\n");
                    }
                    break;
                }
            default:
                {
                    printf("Please try again. Key not valid\n");
                    break;
                }
        }
    }
}

void applicationUsage(char *col, char *des, char *opt)
{
    strcat(col, "[-D <dev>] ");
    sprintf(opt, "\t-D <dev>\t\tThe serial port for the modem. Default: '%s'\n",
            extra_ComPort);
    strcat(des, opt);
}

char* applicationExtraOptions(void)
{
    return "d:D:";
}

char* applicationCmdLineParse(int ch, char *optionArgument)
{
    switch(ch)
    {
        case 'D':
        case 'd':
            {
                if (sscanf(optionArgument, "%s",extra_ComPort) != 1)
                {
                    return "Invalid 'D'/'d' argument";
                }
                return NULL;
                break;
            }
        default:
            /*should not be possible*/
            return "Unknown option in applicationCmdLineParse()";
            break;
    }

    return NULL;
}

void applicationSetup(void)
{
}
