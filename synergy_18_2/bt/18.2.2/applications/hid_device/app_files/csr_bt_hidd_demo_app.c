/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "csr_bt_platform.h"
#include "csr_bt_profiles.h"
#include "bluetooth.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_hidd_lib.h"
#include "csr_bt_hidd_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_hid_service_record.h"
#include "csr_bt_sc_demo_app.h"

#include "csr_bt_sd_prim.h"
#include "csr_bt_sd_lib.h"
#include "csr_bccmd_lib.h"
#include "csr_sched_init.h"


#define CHANGE_STATE(s) (instData->state=s)

#define HID_MINOR_DEV_CLASS_POINTER     0x80
#define HID_MINOR_DEV_CLASS_KEYBOARD    0x40

#define HID_MOUSE                       100
#define HID_KEYBOARD                    200

/*    The time to wait for bonding and connection in number of seconds */
#define MAX_BONDING_TIME                (60)
#define KEYB_CHECK_TIMEOUT              (100000)

#define CSR_BT_UNDEFINED                  (0xFF)

#define NULL_ACTION                     (0)
#define GET_PROTOCOL_ACTION             (1)
#define GET_IDLE_ACTION                 (2)

#define NOF_DI_SERVICE_DESCRIPTIONS      5

/* Defines for PTS RF-shielding */
#define ENABLE_TX_VARID 0x4007
#define DISABLE_TX_VARID 0x4008

/* Input reports */
/* Mouse  - matches the descriptor in csr_bt_hid_service_record.h */
#define MOUSE_REPORT_LENGTH             5

static CsrUint8  EMPTY_MOUSE_REPORT[]    = {0x02,0x00,0x00,0x00,0x00};
static CsrUint8  BUTTON1_REPORT[]        = {0x02,0x01,0x00,0x00,0x00};
static CsrUint8  BUTTON2_REPORT[]        = {0x02,0x02,0x00,0x00,0x00};
static CsrUint8  BUTTON3_REPORT[]        = {0x02,0x04,0x00,0x00,0x00};

static CsrUint8  SCROLL_UP_REPORT[]      = {0x02,0x00,0x00,0x00,0x01};
static CsrUint8  SCROLL_DOWN_REPORT[]    = {0x02,0x00,0x00,0x00,0xFF};

static CsrUint8  UP_REPORT[]             = {0x02,0x00,0x00,0xFF,0x00};
static CsrUint8  DOWN_REPORT[]           = {0x02,0x00,0x00,0x01,0x00};
static CsrUint8  LEFT_REPORT[]           = {0x02,0x00,0xFF,0x00,0x00};
static CsrUint8  RIGHT_REPORT[]          = {0x02,0x00,0x01,0x00,0x00};

/* Keyboard - matches the descriptor in csr_bt_hid_service_record.h */
#define KEYBOARD_REPORT_LENGTH          9
static CsrUint8  EMPTY_KEYBOARD_REPORT[] = {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

static CsrUint8  SHIFT_C_REPORT[]        = {0x01,0x02,0x00,0x06,0x00,0x00,0x00,0x00,0x00};
static CsrUint8  SHIFT_S_REPORT[]        = {0x01,0x02,0x00,0x16,0x00,0x00,0x00,0x00,0x00};
static CsrUint8  SHIFT_R_REPORT[]        = {0x01,0x02,0x00,0x15,0x00,0x00,0x00,0x00,0x00};

static CsrUint8  SHIFT_REPORT[]          = {0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static CsrUint8  CTRL_REPORT[]           = {0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
static CsrUint8  ALT_REPORT[]            = {0x01,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

static CsrUint8  BACKSPACE_REPORT[]      = {0x01,0x00,0x00,0x2a,0x00,0x00,0x00,0x00,0x00};
static CsrUint8  SPACE_REPORT[]          = {0x01,0x00,0x00,0x2c,0x00,0x00,0x00,0x00,0x00};
static CsrUint8  ARROW_UP_REPORT[]       = {0x01,0x00,0x00,0x52,0x00,0x00,0x00,0x00,0x00};


static CsrUint8  SHIFT_b_REPORT[]        = {0x01,0x00,0x00,0x05,0x00,0x00,0x00,0x00,0x00};
static CsrUint8  SHIFT_l_REPORT[]        = {0x01,0x00,0x00,0x0f,0x00,0x00,0x00,0x00,0x00};
static CsrUint8  SHIFT_u_REPORT[]        = {0x01,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00};
static CsrUint8  SHIFT_e_REPORT[]        = {0x01,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00};
static CsrUint8  SHIFT_t_REPORT[]        = {0x01,0x00,0x00,0x17,0x00,0x00,0x00,0x00,0x00};
static CsrUint8  SHIFT_o_REPORT[]        = {0x01,0x00,0x00,0x12,0x00,0x00,0x00,0x00,0x00};
static CsrUint8  SHIFT_h_REPORT[]        = {0x01,0x00,0x00,0x0b,0x00,0x00,0x00,0x00,0x00};
static CsrUint8  SHIFT_ENTER_REPORT[]    = {0x01,0x00,0x00,0x28,0x00,0x00,0x00,0x00,0x00};

#define LONG_REPORT_LENGTH              2500

/* Demo app states */
typedef CsrUint8 hiddStates_t;
#define idle_s (hiddStates_t) 0x00
#define active_s (hiddStates_t) 0x01
#define pairing_s (hiddStates_t) 0x02
#define connected_s (hiddStates_t) 0x03
#define reconnecting_s (hiddStates_t) 0x04
#define disconnected_s (hiddStates_t) 0x05
#define num_of_states (hiddStates_t) 0x06

const char *state_str[num_of_states] =
{
    "IDLE",
    "ACTIVE",
    "PAIRING",
    "CONNECTED",
    "RE-CONNECTING",
    "DISCONNECTED"
};

/* Low power modes */
#define ACTIVE         0
#define SNIFF          1
#define DISCONNECTED   2
#define NUM_OF_MODES   3


const char *mode_str[NUM_OF_MODES] =
{
    "ACTIVE",
    "SNIFF",
    "DISCONNECTED"
};

typedef struct
{
    CsrBool          valid;
    CsrBool          primaryRecord;
    CsrUint32        serviceDescriptionHandle;
}hiddServiceHandleStruct;

/* instance data */
typedef struct
{
    hiddStates_t    state;
    hiddStates_t    prevState;
    void           *recvMsgP;
    CsrSchedQid       appHdl;
    CsrUint16        sendMsgType;
    CsrUint8         lowPowerMode;
    CsrUint8         protocol;
    CsrUint8         idleRate;
    CsrUint8         deviceType;
    CsrUint8         countBluetooth;
    CsrBool          disconnected;
    CsrBool          sendEmpty;
    CsrBool          writeBluetooth;
    CsrBool          showPtsExtras;
    CsrBool          longControlReport;
    CsrBool          inquiryScanDisabled;
    CsrBtDeviceAddr    bondingDevAddr;
    ScAppInstData_t scInstData;

#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
    CsrUint16                   sdUnregisterTemp;
    CsrBool                     sdUnregisterState;
    CsrBool                     sdTempPR;
    hiddServiceHandleStruct    sdEntry[NOF_DI_SERVICE_DESCRIPTIONS];
#endif
} hiddInstData_t;


/* forward declarations for handler functions */
void handleKeybPrim(CsrUint8 theChar, hiddInstData_t *instData);
void handleHiddPrim(hiddInstData_t *instData, void *msg);
int _kbhit( void );
int _getch( void );

#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
static void printOutServiceHandles(hiddInstData_t *instData)
{
    CsrUint16 x;

    for (x=0; x<NOF_DI_SERVICE_DESCRIPTIONS; x++)
    {
        printf("Entry %i: ", x);
        printf("valid (%i), ", instData->sdEntry[x].valid);
        printf("Primary Record (%i) \n", instData->sdEntry[x].primaryRecord);
    }
    printf("\n");
}

static CsrBool doesPrimaryRecordExist(hiddInstData_t *instData)
{
    CsrBool retVal = FALSE;
    CsrUint16 x;

    for (x=NOF_DI_SERVICE_DESCRIPTIONS; x>0; x--)
    {
        if (TRUE == instData->sdEntry[x-1].valid)
        {
            if (TRUE == instData->sdEntry[x-1].primaryRecord)
            {
                retVal = TRUE;
            }
        }
    }
    return retVal;
}

static CsrUint16 nofServiceHandleEntries(hiddInstData_t *instData)
{
    CsrUint16 x, nofEntries = 0;

    for (x=NOF_DI_SERVICE_DESCRIPTIONS; x>0; x--)
    {
        if (TRUE == instData->sdEntry[x-1].valid)
        {
            nofEntries++;
        }
    }
    return nofEntries;
}


static CsrBool insertServiceHandle(hiddInstData_t *instData, CsrUint32 serviceHandle, CsrUint16 *index)
{
    CsrBool retVal = FALSE;
    CsrUint16 x, nofEntries = 0, nofPR = 0, foundEmptyIndex = 0;

    for (x=NOF_DI_SERVICE_DESCRIPTIONS; x>0; x--)
    {
        if (TRUE == instData->sdEntry[x-1].valid)
        {
            nofEntries++;
            if (TRUE == instData->sdEntry[x-1].primaryRecord)
            {
                nofPR++;
            }
        }
        else
        {
            foundEmptyIndex = x-1;
        }
    }

    if (nofEntries != NOF_DI_SERVICE_DESCRIPTIONS)
    {
        *index = foundEmptyIndex;

        instData->sdEntry[foundEmptyIndex].valid = TRUE;
        instData->sdEntry[foundEmptyIndex].primaryRecord = instData->sdTempPR;
        instData->sdEntry[foundEmptyIndex].serviceDescriptionHandle = serviceHandle;

        retVal = TRUE;
    }

    return retVal;
}
#endif

/*************************************************************************************
  KeyMessageService:
************************************************************************************/
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

/*************************************************************************************
  showMenu functions:
************************************************************************************/
void showHeader(hiddInstData_t *instData)
{
    printf("\nSynergy BT Demo - Human Interface Device device (HID device)\n");
    printf("Hit <ESC> to quit! Hit <SPACE> to reprint menu!\n\n");
    printf("Current state: %s\n", state_str[instData->state]);
}

void playMenu(hiddInstData_t *instData)
{
    showHeader(instData);
    printf("\nOptions: \n");

    switch(instData->state)
    {
    case idle_s:
        {
            printf("      Activate\n");
            if((instData->state == idle_s) || (instData->state == active_s) || (instData->state == reconnecting_s))
            {
                BD_ADDR_T zeroAddr;
                CsrBtBdAddrZero(&zeroAddr);
                if(!CsrBtBdAddrEq(&(instData->bondingDevAddr), &zeroAddr))
                {
                    if(instData->disconnected)
                    {
                        if(instData->deviceType == HID_MOUSE)
                        {
                            printf("      a)  MOUSE \n");
                            printf("      c)  MOUSE with known address: %04x:%04x:%06x\n",
                                instData->bondingDevAddr.nap, instData->bondingDevAddr.uap, instData->bondingDevAddr.lap);
                        }
                        else if(instData->deviceType == HID_KEYBOARD)
                        {
                            printf("      b)  KEYBOARD \n");
                            printf("      d)  KEYBOARD with known address: %04x:%02x:%06x\n",
                                instData->bondingDevAddr.nap, instData->bondingDevAddr.uap, instData->bondingDevAddr.lap);
                        }

                    }
                    else
                    {
                        printf("      a)  MOUSE \n");
                        printf("      b)  KEYBOARD \n");
                        printf("      c)  MOUSE with known address: %04x:%02x:%06x\n",
                            instData->bondingDevAddr.nap, instData->bondingDevAddr.uap, instData->bondingDevAddr.lap);
                        printf("      d)  KEYBOARD with known address: %04x:%02x:%06x\n",
                            instData->bondingDevAddr.nap, instData->bondingDevAddr.uap, instData->bondingDevAddr.lap);
                    }
                }
                else
                {
                    if(instData->disconnected)
                    {
                        if(instData->deviceType == HID_MOUSE)
                        {
                            printf("      a)  MOUSE \n");
                        }
                        else if(instData->deviceType == HID_KEYBOARD)
                        {
                            printf("      b)  KEYBOARD \n");
                        }
                    }
                    else
                    {
                        printf("      a)  MOUSE \n");
                        printf("      b)  KEYBOARD \n");
                    }
                }
            }
            printf("      f)  Security Controller Configuration\n");
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
            printf("      G)  Register a DI service record with TRUE primary Record\n");
            printf("      g)  Register a DI service record with FALSE primary Record\n");
            printf("      H)  Printout DI service record list\n");
            printf("      h)  Unregister a DI service record\n");
#endif
            break;
        }
    case active_s:
        {
            if(instData->disconnected)
            {
                printf("  0)  Reactivate\n");
            }
            printf("  1)  Deactivate\n");
            break;
        }
    case reconnecting_s:
        {
            printf("  1)  Deactivate\n");
            break;
        }
    case connected_s:
        {
            printf("  1)  Deactivate\n");
            printf("  2)  Unplug\n");
            printf("  3)  Send input report (e.g. mouse movements or keyboard entries)\n\n");
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
            printf("     G)  Register a DI service record with TRUE primary Record\n");
            printf("     g)  Register a DI service record with FALSE primary Record\n");
            printf("     H)  Printout DI service record list\n");
            printf("     h)  Unregister a DI service record\n\n");
#endif
            printf("  Current low power mode is: %s. Change mode to: \n",mode_str[instData->lowPowerMode]);
            printf("  5)    Active mode\n");
            printf("  6)    Sniff mode\n");
            printf("  7)    Disconnected mode\n");
            printf("\n");
            printf("  p)    Toggle extra PTS features - FOR TEST ONLY!\n");
            if (instData->showPtsExtras == TRUE)
            {
                printf("\n");
                printf("            l)     Send long input report on interrupt channel\n");
                if (instData->longControlReport == FALSE)
                {
                    printf("            L)     Send long input report on control channel\n");
                    printf("                   when receiving a Get Report command from Host\n");
                }
                else
                {
                    printf("            L)     Send short input report on control channel\n");
                    printf("                   when receiving a Get Report command from Host\n");
                }
                printf("            m)     Enable RF-shield\n");
                printf("            n)     Disable RF-shield\n");
                printf("            z)     Toggle inquiry scan (currently %s)\n", instData->inquiryScanDisabled ? "disabled" : "enabled");
                printf("\n");
            }
            break;
        }
    case disconnected_s:
        {
            printf("  1)  Deactivate\n\n");

            printf("  Current low power mode is: %s. Change mode to: \n",mode_str[instData->lowPowerMode]);
            printf("  5)    Active mode\n");
            printf("  6)    Sniff mode\n");
            printf("  7)    Disconnected mode\n");

            break;
        }
    default:
        {
            break;
        }
    }
}

void getReport(hiddInstData_t *instData)
{
    showHeader(instData);
    if(instData->deviceType == HID_MOUSE)
    {
        printf("  z)  Empty \n");
        printf("  x)  Move up \n");
        printf("  s)  Move down \n");
        printf("  q)  Move left \n");
        printf("  w)  Move right \n");
        printf("  e)  Button 1 (left) \n");
        printf("  r)  Button 2 (right) \n");
        printf("  t)  Button 3 \n");
        printf("  y)  Scroll up \n");
        printf("  u)  Scroll down \n");
        printf("\nPress <SPACE> to go back to main menu\n");
    }
    else if(instData->deviceType == HID_KEYBOARD)
    {
        printf("  x)  Empty \n");
        printf("  z)  C \n");
        printf("  s)  S \n");
        printf("  r)  R \n");
        printf("  q)  SHIFT \n");
        printf("  w)  CTRL \n");
        printf("  e)  ALT \n");
        printf("  t)  BACKSPACE \n");
        printf("  y)  SPACE \n");
        printf("  u)  Arrow UP \n");
        printf("  o)  'bluetooth' + ENTER \n");
        printf("\nPress <SPACE> to go back to main menu\n");
    }
}

void sendBluetoothResponse(hiddInstData_t *instData)
{
    CsrUint8* report = NULL;
    CsrUint8 tmpDataLen = KEYBOARD_REPORT_LENGTH;
    CsrUint8* tmpData = CsrPmemZalloc(tmpDataLen+1);

    switch(instData->countBluetooth)
    {
    case 0:
        {
            report = SHIFT_b_REPORT;
        }
        break;
    case 1:
        {
            report = SHIFT_l_REPORT;
        }
        break;
    case 2:
        {
            report = SHIFT_u_REPORT;
        }
        break;
    case 3:
        {
            report = SHIFT_e_REPORT;
        }
        break;
    case 4:
        {
            report = SHIFT_t_REPORT;
        }
        break;
    case 5:
        {
            report = SHIFT_o_REPORT;
        }
        break;
    case 6:
        {
            report = SHIFT_o_REPORT;
        }
        break;
    case 7:
        {
            report = SHIFT_t_REPORT;
        }
        break;
    case 8:
        {
            report = SHIFT_h_REPORT;
        }
        break;
    case 9:
        {
            report = SHIFT_ENTER_REPORT;
        }
        break;
    default:
        {
            instData->writeBluetooth = FALSE;
        }
        break;
    }
    if(report)
    {

        CsrMemCpy(&(tmpData[1]), report, tmpDataLen);
        instData->sendEmpty = TRUE;
        instData->countBluetooth++;
        CsrBtHiddDataReqSend(CSR_BT_HIDD_IFACEQUEUE, (CsrUint16)(tmpDataLen+1), tmpData);
    }
}

void sendResponse(hiddInstData_t *instData, CsrUint8 theChar)
{
    CsrUint8* tmpData = NULL;
    CsrUint8* report = NULL;
    CsrUint16 tmpDataLen = 0;

    if(instData->deviceType == HID_MOUSE)
    {
        tmpDataLen = MOUSE_REPORT_LENGTH;
        switch(theChar)
        {
        case 'z':
            {
                report = EMPTY_MOUSE_REPORT;
                break;
            }
        case 'x':
            {
                report = UP_REPORT;
                break;
            }
        case 's':
            {
                report = DOWN_REPORT;
                break;
            }
        case 'q':
            {
                report = LEFT_REPORT;
                break;
            }
        case 'w':
            {
                report = RIGHT_REPORT;
                break;
            }
        case 'e':
            {
                report = BUTTON1_REPORT;
                break;
            }
        case 'r':
            {
                report = BUTTON2_REPORT;
                break;
            }
        case 't':
            {
                report = BUTTON3_REPORT;
                break;
            }
        case 'y':
            {
                report = SCROLL_UP_REPORT;
                break;
            }
        case 'u':
            {
                report = SCROLL_DOWN_REPORT;
                break;
            }

        default:
            {
                break;
            }
        }
    }
    else if(instData->deviceType == HID_KEYBOARD)
    {
        tmpDataLen = KEYBOARD_REPORT_LENGTH;
        switch(theChar)
        {
        case 'x':
            {
                report = EMPTY_KEYBOARD_REPORT;
                break;
            }
        case 'z':
            {
                instData->sendEmpty = TRUE;
                report = SHIFT_C_REPORT;
                break;
            }
        case 's':
            {
                instData->sendEmpty = TRUE;
                report = SHIFT_S_REPORT;
                break;
            }
        case 'r':
            {
                instData->sendEmpty = TRUE;
                report = SHIFT_R_REPORT;
                break;
            }
        case 'q':
            {
                report = SHIFT_REPORT;
                break;
            }
        case 'w':
            {
                report = CTRL_REPORT;
                break;
            }
        case 'e':
            {
                report = ALT_REPORT;
                break;
            }
        case 't':
            {
                instData->sendEmpty = TRUE;
                report = BACKSPACE_REPORT;
                break;
            }
        case 'y':
            {
                instData->sendEmpty = TRUE;
                report = SPACE_REPORT;
                break;
            }
        case 'u':
            {
                instData->sendEmpty = TRUE;
                report = ARROW_UP_REPORT;
                break;
            }

        default:
            {
                break;
            }
        }
    }

    tmpData = CsrPmemZalloc(tmpDataLen+1);
    if(report)
    {
        CsrMemCpy(&(tmpData[1]), report, tmpDataLen);
    }
    if(instData->sendMsgType == CSR_BT_HIDD_CONTROL_RES)
    {
        CsrBtHiddControlResSend(CSR_BT_HIDD_IFACEQUEUE, CSR_BT_HIDD_DATA, CSR_BT_HIDD_INPUT_REPORT, (CsrUint16)(tmpDataLen+1), tmpData);
    }
    else if(instData->sendMsgType == CSR_BT_HIDD_DATA_REQ)
    {
        CsrBtHiddDataReqSend(CSR_BT_HIDD_IFACEQUEUE, (CsrUint16)(tmpDataLen+1), tmpData);
    }
    else
    {
        printf("\n\nUnexpected sendResponse: %02Xh \n", instData->sendMsgType);
    }
}


/*************************************************************************************
  hiddControlIndHandler:
************************************************************************************/
void hiddControlIndHandler(hiddInstData_t *instData, void* message)
{
    CsrBtHiddControlInd *prim;
    char *tmpTransactionType;
    char *tmpParameter;

    prim = (CsrBtHiddControlInd *) message;

    tmpTransactionType = CsrPmemZalloc(50);
    tmpTransactionType = NULL;
    tmpParameter = CsrPmemZalloc(50);
    tmpParameter = NULL;

    switch(prim->transactionType)
    {
    case CSR_BT_HIDD_HANDSHAKE:
        tmpTransactionType = "CSR_BT_HIDD_HANDSHAKE";
        break;
    case CSR_BT_HIDD_CONTROL:
        {
            tmpTransactionType = "CSR_BT_HIDD_CONTROL";

            switch(prim->parameter)
            {
            case CSR_BT_HIDD_NOP:
                tmpParameter = "CSR_BT_HIDD_NOP";
                break;
            case CSR_BT_HIDD_HARD_RESET:
                tmpParameter = "CSR_BT_HIDD_HARD_RESET";
                break;
            case CSR_BT_HIDD_SOFT_RESET:
                tmpParameter = "CSR_BT_HIDD_SOFT_RESET";
                break;
            case CSR_BT_HIDD_SUSPEND:
                tmpParameter = "CSR_BT_HIDD_SUSPEND";
                break;
            case CSR_BT_HIDD_EXIT_SUSPEND:
                tmpParameter = "CSR_BT_HIDD_EXIT_SUSPEND";
                break;
            case CSR_BT_HIDD_VC_UNPLUG:
                tmpParameter = "CSR_BT_HIDD_VC_UNPLUG";
                printf("\n\nUnplug not expected in CSR_BT_HIDD_CONTROL_IND");
                break;
            default:
                printf("\n\nUnexpected Parameter:%02Xh \n", prim->parameter);
                break;
            }
        }
        break;
    case CSR_BT_HIDD_GET_REPORT:
        {
            CsrUint16 tmpDataLen = 0;
            CsrUint8* tmpData = NULL;
            CsrUint8* report = NULL;
            CsrUint8 success = TRUE;

            tmpTransactionType = "CSR_BT_HIDD_GET_REPORT";
            instData->sendMsgType = CSR_BT_HIDD_CONTROL_RES;

            if(instData->deviceType == HID_MOUSE)
            {
                if(prim->dataLen <= 0 || prim->data[1] != 0x02)
                {
                    success = FALSE;

                }
                else
                {

                    if (instData->longControlReport == FALSE)
                    {
                        tmpDataLen = MOUSE_REPORT_LENGTH;
                        report = EMPTY_MOUSE_REPORT;
                    }
                    else
                    {
                        CsrUint16 i = 0;
                        CsrUint8  t_data[LONG_REPORT_LENGTH];

                        tmpDataLen = LONG_REPORT_LENGTH;
                        for (i = 0; i < LONG_REPORT_LENGTH; i++)
                        {
                            t_data[i] = i % 256;
                        }
                        report = t_data;
                    }
                }
            }
            else if(instData->deviceType == HID_KEYBOARD)
            {
                if(prim->dataLen <= 0 || prim->data[1] != 0x01)
                {
                    success = FALSE;

                }
                else
                {
                    if (instData->longControlReport == FALSE)
                    {
                        tmpDataLen = KEYBOARD_REPORT_LENGTH;
                        report = EMPTY_KEYBOARD_REPORT;
                        }
                    else
                    {
                        CsrUint16 i = 0;
                        CsrUint8  t_data[LONG_REPORT_LENGTH];

                        tmpDataLen = LONG_REPORT_LENGTH;
                        for (i = 1; i < LONG_REPORT_LENGTH + 1; i++)
                        {
                            t_data[i] = i % 256;
                        }
                        report = t_data;
                    }
                }
            }


            tmpData = CsrPmemZalloc(tmpDataLen+1);
            if(report)
            {
                CsrMemCpy(&(tmpData[1]), report, tmpDataLen);
            }
            if(success)
            {
                printf("\nSending %d bytes\n", (tmpDataLen + 1));
                CsrBtHiddControlResSend(CSR_BT_HIDD_IFACEQUEUE, CSR_BT_HIDD_DATA, CSR_BT_HIDD_INPUT_REPORT, (CsrUint16)(tmpDataLen+1), tmpData);
            }
            else
            {
                CsrBtHiddControlResSend(CSR_BT_HIDD_IFACEQUEUE, CSR_BT_HIDD_HANDSHAKE, CSR_BT_HIDD_ERR_INVALID_REPORT_ID, 0, NULL);
            }


        }
        break;
    case CSR_BT_HIDD_SET_REPORT:
        {
            CsrBtHiddParameterType result = CSR_BT_HIDD_SUCCESS;

            tmpTransactionType = "CSR_BT_HIDD_SET_REPORT";

            if(prim->parameter == CSR_BT_HIDD_INPUT_REPORT)
            {
                tmpParameter = "CSR_BT_HIDD_INPUT_REPORT";
            }
            else if(prim->parameter == CSR_BT_HIDD_OUTPUT_REPORT)
            {
                tmpParameter = "CSR_BT_HIDD_OUTPUT_REPORT";
            }
            else if(prim->parameter == CSR_BT_HIDD_FEATURE_REPORT)
            {
                tmpParameter = "CSR_BT_HIDD_FEATURE_REPORT";
            }
            else if (prim->parameter == CSR_BT_HIDD_OTHER)
            {
                tmpParameter = "CSR_BT_HIDD_OTHER";
            }
            else
            {
                result = CSR_BT_HIDD_ERR_INVALID_REPORT_ID;
            }
            CsrBtHiddControlResSend(CSR_BT_HIDD_IFACEQUEUE, CSR_BT_HIDD_HANDSHAKE, result, 0, NULL);
            if(prim->data)
            {
                CsrUint8 i;
                printf("\nDataLength: %i, data: ", prim->dataLen-1);
                for(i = 1; i < prim->dataLen; i++)
                {
                    printf("%02x ",prim->data[i]);
                }
            }
        }
        break;
    case CSR_BT_HIDD_GET_PROTOCOL:
        {
            CsrUint8* tmpData;
            tmpTransactionType = "CSR_BT_HIDD_GET_PROTOCOL";

            tmpData = CsrPmemZalloc(1+1);
            CsrMemCpy(&(tmpData[1]), &(instData->protocol), 1);
            CsrBtHiddControlResSend(CSR_BT_HIDD_IFACEQUEUE, CSR_BT_HIDD_DATA, 0, 1+1, tmpData);
        }
        break;
    case CSR_BT_HIDD_SET_PROTOCOL:
        {
            tmpTransactionType = "CSR_BT_HIDD_SET_PROTOCOL";

            instData->protocol = prim->parameter;

            if(prim->parameter == CSR_BT_HIDD_BOOT_PROTOCOL)
            {
                tmpParameter = "CSR_BT_HIDD_BOOT_PROTOCOL";
            }
            else if(prim->parameter == CSR_BT_HIDD_REPORT_PROTOCOL)
            {
                tmpParameter = "CSR_BT_HIDD_REPORT_PROTOCOL";
            }

            CsrBtHiddControlResSend(CSR_BT_HIDD_IFACEQUEUE, CSR_BT_HIDD_HANDSHAKE, CSR_BT_HIDD_SUCCESS, 0, NULL);
        }
        break;
    case CSR_BT_HIDD_GET_IDLE:
        {
            CsrUint8* tmpData;
            tmpTransactionType = "CSR_BT_HIDD_GET_IDLE";

            tmpData = CsrPmemZalloc(1+1);
            CsrMemCpy(&(tmpData[1]), &(instData->idleRate), 1);
            CsrBtHiddControlResSend(CSR_BT_HIDD_IFACEQUEUE, CSR_BT_HIDD_DATA, 0, 1+1, tmpData);
        }
        break;
    case CSR_BT_HIDD_SET_IDLE:
        {
            tmpTransactionType = "CSR_BT_HIDD_SET_IDLE";

            instData->idleRate = prim->data[1];

            CsrBtHiddControlResSend(CSR_BT_HIDD_IFACEQUEUE, CSR_BT_HIDD_HANDSHAKE, CSR_BT_HIDD_SUCCESS, 0, NULL);
        }
        break;
    case CSR_BT_HIDD_DATA:
        {
            tmpTransactionType = "CSR_BT_HIDD_DATA";
        }
        break;
    case CSR_BT_HIDD_DATC:
        {
            tmpTransactionType = "CSR_BT_HIDD_DATC";
        }
        break;
    default:
        {
            printf("\n\nUnexpected TransactionType");
            CsrBtHiddControlResSend(CSR_BT_HIDD_IFACEQUEUE, CSR_BT_HIDD_HANDSHAKE, CSR_BT_HIDD_ERR_UNSUPPORTED, 0, NULL);
        }
        break;
    }
    printf("\n\nTransactionType: %s, Parameter: %s", tmpTransactionType, tmpParameter);

}

/*************************************************************************************
  activate:
************************************************************************************/
void activate(hiddInstData_t *instData, BD_ADDR_T bdAddr, CsrUint8 deviceType)
{
    CsrUint16    hidLength = 0;
    CsrUint8     *hidSdpRecord = NULL;
    BD_ADDR_T    zeroAddr;

    if(deviceType == HID_MOUSE)
    {
        hidLength = sizeof(hid_mouse_service_record);
        hidSdpRecord = (CsrUint8 *) CsrPmemAlloc(hidLength);
        CsrMemCpy(hidSdpRecord, hid_mouse_service_record, hidLength);
    }
    else if(deviceType == HID_KEYBOARD)
    {
        hidLength = sizeof(hid_keyboard_service_record);
        hidSdpRecord = (CsrUint8 *) CsrPmemAlloc(hidLength);
        CsrMemCpy(hidSdpRecord, hid_keyboard_service_record, hidLength);
    }

    showHeader(instData);
    printf("\n\n Activating...   please wait\n");

    CsrBtBdAddrZero(&zeroAddr);
    if(!CsrBtBdAddrEq(&bdAddr, &zeroAddr))
    {
        CsrPmemFree(hidSdpRecord);
        hidSdpRecord = NULL;
        hidLength = 0;
    }

    CsrBtHiddActivateReqSend(CSR_BT_HIDD_IFACEQUEUE,
                        instData->appHdl,
                        NULL,
                        NULL,
                        0xFFFF,
                        bdAddr,
                        0,
                        NULL,
                        hidLength,
                        hidSdpRecord);
}

/*************************************************************************************
  deactivate:
************************************************************************************/
void deactivate(hiddInstData_t *instData)
{
    CsrBtHiddDeactivateReqSend(CSR_BT_HIDD_IFACEQUEUE);
}

/*************************************************************************************
  unplug:
************************************************************************************/
void unplug(hiddInstData_t *instData)
{
    CsrBtHiddUnplugReqSend(CSR_BT_HIDD_IFACEQUEUE,instData->bondingDevAddr);
}

/*************************************************************************************
  changeLowPowerMode:
************************************************************************************/
void changeLowPowerMode(hiddInstData_t *instData, CsrUint8 nextMode)
{
    CsrUint8 mode = 0;
    if(nextMode == ACTIVE)
    {
        mode = CSR_BT_HIDD_ACTIVE_MODE;
    }
    else if (nextMode == SNIFF)
    {
        mode = CSR_BT_HIDD_SNIFF_MODE;
    }
    else if(nextMode == DISCONNECTED)
    {
        mode = CSR_BT_HIDD_DISCONNECT_MODE;
    }

    CsrBtHiddModeChangeReqSend(CSR_BT_HIDD_IFACEQUEUE, mode);
}


/*************************************************************************************
  initInstanceData:
************************************************************************************/
void initInstanceData(hiddInstData_t *instData)
{
    extern BD_ADDR_T defGlobalBdAddr;

    instData->state               = idle_s;
    instData->lowPowerMode        = ACTIVE;
    instData->protocol            = CSR_BT_HIDD_REPORT_PROTOCOL;
    instData->idleRate            = 0;
    instData->deviceType          = HID_MOUSE;
    instData->appHdl              = 0;
    instData->disconnected        = FALSE;
    instData->sendEmpty           = FALSE;
    instData->writeBluetooth      = FALSE;
    instData->countBluetooth      = 0;
    instData->showPtsExtras       = FALSE;
    instData->longControlReport   = FALSE;
    instData->inquiryScanDisabled = FALSE;

#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
    {
        CsrUint8 x;

        instData->sdTempPR            = FALSE;
        instData->sdUnregisterTemp    = 0xffff;
        instData->sdUnregisterState   = FALSE;
        for (x=0; x<NOF_DI_SERVICE_DESCRIPTIONS; x++)
        {
            instData->sdEntry[x].valid = FALSE;
            instData->sdEntry[x].primaryRecord = FALSE;
            instData->sdEntry[x].serviceDescriptionHandle = 0;
        }
    }
#endif

    CsrBtBdAddrCopy(&instData->bondingDevAddr, &defGlobalBdAddr);

    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, NULL);
    playMenu(instData);
}


static void hiddScAppSetFnFini(void *pFiniData)
{
    hiddInstData_t *instData = (hiddInstData_t *) pFiniData;
    scAppRestoreState((CsrUint8 *) &instData->prevState, (CsrUint8 *) &instData->state);
    playMenu(instData);
}

static void hiddScAppSetFnInit(void *pInitData)
{
    hiddInstData_t *instData = (hiddInstData_t *) pInitData;
    scAppSaveState((CsrUint8 *) &instData->prevState, (CsrUint8 *) &instData->state, pairing_s);
}

static void* hiddScAppSetFnGetMessage(void *pRecvMsgPData)
{
    hiddInstData_t *instData = (hiddInstData_t *) pRecvMsgPData;
    void *result             = instData->recvMsgP;
    instData->recvMsgP       = NULL;
    return result;
}

/**************************************************************************************************
 *
 *    init function called by the scheduler upon initialisation. This function is used to boot
 *    the demo application.
 *
 **************************************************************************************************/
void CsrBtAppInit(void **gash)
{
    hiddInstData_t *instData;

    /*  allocate and initialise instance data space */
    *gash = (void *) CsrPmemAlloc(sizeof(hiddInstData_t));
    instData = (hiddInstData_t *) *gash;

    initInstanceData(instData);

    /* Get application handle */
    instData->appHdl = TESTQUEUE;

    /* Security App Configuration */
    scAppInitInstanceData(&instData->scInstData);

    scAppSetFnFini(&instData->scInstData, hiddScAppSetFnFini, instData);
    scAppSetFnInit(&instData->scInstData, hiddScAppSetFnInit, instData);
    scAppSetSecLevelSend(&instData->scInstData, scAppSendProfileSecurityLevelReq, &instData->scInstData);

    scAppSetFnGetMessage(&instData->scInstData, hiddScAppSetFnGetMessage, instData);

    scAppSetSecLevelPrim(&instData->scInstData,
                         0,     /* Index 0. Single profile */
                         "HIDD",
                         CSR_BT_HIDD_PRIM, CSR_BT_HIDD_IFACEQUEUE,
                         CSR_BT_HIDD_SECURITY_IN_REQ, CSR_BT_HIDD_SECURITY_OUT_REQ);
}

#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    CsrUint16 msg_type=0;
    void *msg_data=NULL;
    hiddInstData_t *instData;

    instData = (hiddInstData_t *) (*gash);

    scAppDeinitInstanceData(&instData->scInstData);

    /* get a message from the demoapplication message queue. The
       message is returned in prim and the event type in eventType
    */
    while(CsrSchedMessageGet(&msg_type, &msg_data) )
    {
        switch (msg_type)
        {
            case CSR_BT_HIDD_PRIM:
            {
                CsrBtHiddFreeUpstreamMessageContents(msg_type, msg_data);
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
            case CSR_BT_CM_PRIM:
            {
                CsrBtCmFreeUpstreamMessageContents(msg_type, msg_data);
                break;
            }
            case CSR_BCCMD_PRIM:
            {
                CsrBccmdFreeUpstreamMessageContents(CSR_BCCMD_PRIM, msg_data);
                break;
            }

        }
        CsrPmemFree(msg_data);
    }
    CsrPmemFree(instData);
}
#endif

/**************************************************************************************************
 *
 *    This is the demo application handler function. All primitives sent to the demo application will
 *    be received in here. The scheduler ensures that the function is activated when a signal is put
 *    on the demo application signal queue.
 *
 **************************************************************************************************/
void CsrBtAppHandler(void **gash)
{
    hiddInstData_t *instData;
    CsrUint16        eventType=0;
    void           *msg=NULL;

    /* get a message from the demoapplication message queue. The
       message is returned in prim and the event type in eventType.
    */
    instData = (hiddInstData_t *) (*gash);
    CsrSchedMessageGet(&eventType, &instData->recvMsgP);

    msg = instData->recvMsgP;

    switch (eventType)
    {
        case CSR_BT_HIDD_PRIM:
            {
                CsrPrim *prim = (CsrPrim *) msg;
                switch(*prim)
                {
                    case CSR_BT_HIDD_SECURITY_OUT_CFM:
                    case CSR_BT_HIDD_SECURITY_IN_CFM:
                        handleScSecLevelPrim(&instData->scInstData);
                        break;

                    default:
                        handleHiddPrim(instData, msg);
                        break;
                }
                break;
            }
        case CSR_BT_SC_PRIM:
            {
                handleScPrim(&instData->scInstData);
                break;
            }
        case KEY_MESSAGE:
            {
                CsrBtKeyPress_t *key;
                key = (CsrBtKeyPress_t*)msg;

                handleKeybPrim(key->key, instData);
                break;
            }
        case CSR_BT_SD_PRIM:
            {
                CsrPrim * sdPrim = msg;

                switch (*sdPrim)
                {
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
                    case CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM:
                    {
                        CsrBtSdRegisterServiceRecordCfm *prim = (CsrBtSdRegisterServiceRecordCfm *) msg;

                        if (prim->resultCode == CSR_BT_RESULT_CODE_SD_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_SD)
                        {
                            CsrUint16 index;
                            if (TRUE == insertServiceHandle(instData, prim->serviceHandle, &index))
                            {
                                printf("CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM SUCCESS in index (%i)\n", index);
                                printOutServiceHandles(instData);
                            }
                            else
                            {
                                printf("CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM ERROR, no more internal space in instance data to save the servicehandle\n");
                            }
                        }
                        else
                        {
                            printf("CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM ERROR\n");
                        }
                        break;
                    }
                    case CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM:
                    {
                        CsrBtSdUnregisterServiceRecordCfm *prim = (CsrBtSdUnregisterServiceRecordCfm *) msg;

                        if (prim->resultCode == CSR_BT_RESULT_CODE_SD_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_SD)
                        {
                            printf("CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM SUCCESS for index (%i)\n", instData->sdUnregisterTemp);
                            instData->sdEntry[instData->sdUnregisterTemp].valid                    = FALSE;
                            instData->sdEntry[instData->sdUnregisterTemp].primaryRecord            = FALSE;
                            instData->sdEntry[instData->sdUnregisterTemp].serviceDescriptionHandle = 0;
                            instData->sdUnregisterTemp = 0;
                            printOutServiceHandles(instData);
                        }
                        else
                        {
                            printf("CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM ERROR\n");
                        }
                        break;
                    }
#endif

                    default:
                    {
                        printf("Unknown sdPrim type in CsrBtAppHandler: %02X\n\n", *sdPrim);
                        CsrBtSdFreeUpstreamMessageContents(eventType, msg);
                        break;
                    }
                }
                break;
            }
        case CSR_BT_CM_PRIM:
            {
                CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, msg);
                break;
            }
            case CSR_BCCMD_PRIM:
            {
                CsrBccmdFreeUpstreamMessageContents(CSR_BCCMD_PRIM, msg);
                break;
            }
        default:
            {
                playMenu(instData);
                printf("####### unhandled event in hidd handler %x,\n",eventType);
            }
    }
    /*    free the received signal again. If the signal for some reason must be stored in the application
          the pointer (prim) must be set to NULL in order not to free it here */
    CsrPmemFree(instData->recvMsgP);
}

/**************************************************************************************************
Function to handle all hidd-primitives.
**************************************************************************************************/
void handleHiddPrim(hiddInstData_t *instData, void *msg)
{
    CsrBtHiddPrim    *primType;

    primType = (CsrBtHiddPrim *) msg;

    switch (*primType)
    {
    case CSR_BT_HIDD_ACTIVATE_CFM:
        {
            CsrBtHiddActivateCfm *prim = (CsrBtHiddActivateCfm *) msg;
            if(prim->resultCode == CSR_BT_RESULT_CODE_HIDD_SUCCESS &&
               prim->resultSupplier == CSR_BT_SUPPLIER_HIDD)
            {
                printf("\n\n Activate OK ");
                CHANGE_STATE(active_s);
                playMenu(instData);
            }
            else
            {
                showHeader(instData);
                printf("\n\n Fail to Activate, result code is 0x%02X (supplier 0x%02X)\n", prim->resultCode, prim->resultSupplier);
            }
        }
        break;
    case CSR_BT_HIDD_DEACTIVATE_CFM:
        {
            CsrBtHiddDeactivateCfm *prim = (CsrBtHiddDeactivateCfm *) msg;
            if(prim->resultCode == CSR_BT_RESULT_CODE_HIDD_SUCCESS &&
               prim->resultSupplier == CSR_BT_SUPPLIER_HIDD)
            {
                printf("\n\n Deactivate OK ");
                CHANGE_STATE(idle_s);
                instData->disconnected = FALSE;
                instData->lowPowerMode = ACTIVE;
                playMenu(instData);
            }
            else
            {
                showHeader(instData);
                printf("\n\n Fail to Deactivate, result code is 0x%02X (supplier 0x%02X)\n", prim->resultCode, prim->resultSupplier);
            }
        }
        break;
    case CSR_BT_HIDD_CONTROL_IND:
        {
            hiddControlIndHandler(instData, msg);
        }
        break;
    case CSR_BT_HIDD_STATUS_IND:
        {
            CsrBtHiddStatusInd *prim = (CsrBtHiddStatusInd *) msg;

            if(prim->status == CSR_BT_HIDD_CONNECTED)
            {
                CHANGE_STATE(connected_s);
                if(!CsrBtBdAddrEq(&(instData->bondingDevAddr), &(prim->deviceAddr)))
                {
                    CsrBtBdAddrCopy(&(instData->bondingDevAddr), &(prim->deviceAddr));
                }
            }
            else if(prim->status == CSR_BT_HIDD_DISCONNECTED)
            {
                instData->disconnected = TRUE;
                CHANGE_STATE(active_s);
            }
            else if(prim->status == CSR_BT_HIDD_RECONNECTING)
            {
                instData->disconnected = TRUE;
                CHANGE_STATE(reconnecting_s);
            }
            else if(prim->status == CSR_BT_HIDD_CONNECT_FAILED)
            {
                instData->disconnected = TRUE;
                CHANGE_STATE(active_s);
            }
            else if(prim->status == CSR_BT_HIDD_UNREGISTER_FAILED)
            {
                instData->disconnected = TRUE;
                CHANGE_STATE(active_s);
            }
            else
            {
                printf("\n\n Unknown CSR_BT_HIDD_STATUS_IND");
            }
            playMenu(instData);
        }
        break;
    case CSR_BT_HIDD_DATA_CFM:
        {
            if(instData->sendEmpty)
            {
                CsrUint8* tmpData = NULL;
                CsrUint16 tmpDataLen = KEYBOARD_REPORT_LENGTH;
                CsrUint8* report = EMPTY_KEYBOARD_REPORT;

                tmpData = CsrPmemZalloc(tmpDataLen+1);

                CsrMemCpy(&(tmpData[1]), report, tmpDataLen);

                CsrBtHiddDataReqSend(CSR_BT_HIDD_IFACEQUEUE, (CsrUint16)(tmpDataLen+1), tmpData);
                instData->sendEmpty = FALSE;
            }
            else if(instData->writeBluetooth)
            {
                sendBluetoothResponse(instData);
            }
            else
            {
                printf("\n CSR_BT_HIDD_DATA_CFM");
            }
        }
        break;
    case CSR_BT_HIDD_DATA_IND:
        {
            CsrBtHiddDataInd *prim = (CsrBtHiddDataInd *) msg;
            CsrUint16 i;
            printf("\nHIDD_DATA_IND ReportType: %02x, ReportLength: %i, ReportData: ", prim->reportType, prim->reportLen-1);
            for(i = 1; i < prim->reportLen; i++)
            {
                printf("%02x ",prim->report[i]);
            }
        }
        break;
    case CSR_BT_HIDD_UNPLUG_IND:
        {
            CsrBtHiddUnplugInd *prim = (CsrBtHiddUnplugInd *) msg;
            if(prim->resultCode == CSR_BT_RESULT_CODE_HIDD_SUCCESS &&
               prim->resultSupplier == CSR_BT_SUPPLIER_HIDD)
            {
                instData->disconnected = TRUE;
                CHANGE_STATE(active_s);
                CsrBtBdAddrZero(&(instData->bondingDevAddr));
                instData->lowPowerMode = ACTIVE;
                playMenu(instData);
            }
        }
        break;
    case CSR_BT_HIDD_UNPLUG_CFM:
        {
            CsrBtHiddUnplugCfm *prim = (CsrBtHiddUnplugCfm *) msg;
            if(prim->resultCode == CSR_BT_RESULT_CODE_HIDD_SUCCESS &&
               prim->resultSupplier == CSR_BT_SUPPLIER_HIDD)
            {
                instData->disconnected = TRUE;
                CHANGE_STATE(active_s);
                CsrBtBdAddrZero(&(instData->bondingDevAddr));
                instData->lowPowerMode = ACTIVE;
                playMenu(instData);
            }

        }
        break;
    case CSR_BT_HIDD_MODE_CHANGE_IND:
        {
            CsrBtHiddModeChangeInd *prim = (CsrBtHiddModeChangeInd *) msg;
            if(prim->resultCode == CSR_BT_RESULT_CODE_HIDD_SUCCESS &&
               prim->resultSupplier == CSR_BT_SUPPLIER_HIDD)
            {
                if(prim->mode == CSR_BT_HIDD_DISCONNECT_MODE)
                {
                    instData->lowPowerMode = DISCONNECTED;
                    CHANGE_STATE(disconnected_s);
                    playMenu(instData);
                }
                else if(prim->mode == CSR_BT_HIDD_ACTIVE_MODE)
                {
                    instData->lowPowerMode = ACTIVE;
                    if(instData->state != connected_s)
                    {
                        CHANGE_STATE(connected_s);
                        playMenu(instData);
                    }
                    else
                    {
                        printf("\n\nLow power mode changes to: %s\n", mode_str[instData->lowPowerMode]);
                    }
                }
                else if(prim->mode == CSR_BT_HIDD_SNIFF_MODE)
                {
                    instData->lowPowerMode = SNIFF;
                    if(instData->state != connected_s)
                    {
                        CHANGE_STATE(connected_s);
                        playMenu(instData);
                    }
                    else
                    {
                        printf("\n\nLow power mode changes to: %s\n", mode_str[instData->lowPowerMode]);
                    }
                }
            }
        }
        break;
    default:
        {
            printf("\n\nHID device app. unhandled HIDD prim 0x%04x,\n", *primType);
            CsrBtHiddFreeUpstreamMessageContents(*primType, msg);
        }
        break;
    }
}

/**************************************************************************************************
 *
 *    a keyboard event is received.
 *    first check for selection of a found device during inquiry if state is inquiry. If not start
 *    proper action as selected.
 *
 **************************************************************************************************/
void handleKeybPrim(CsrUint8 theChar, hiddInstData_t *instData)
{
    CsrBool primaryRecordTemp = FALSE;

    if (theChar == ESCAPE_KEY)
    {
        printf("\nUser exit...\n");
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif
        return;
    }
    else if(theChar == ' ')
    {
        playMenu(instData);
        return;
    }

    if(instData->state == pairing_s)
    {
        handleScKeybPrim(&instData->scInstData);
    }
    else
    {
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
        if(TRUE == instData->sdUnregisterState)
        {
            instData->sdUnregisterState = FALSE;
            if ((theChar<='9') && (theChar>='0'))
            {
                if (TRUE == instData->sdEntry[(theChar-'0')].valid)
                {
                    if ((TRUE == instData->sdEntry[(theChar-'0')].primaryRecord) && (1 != nofServiceHandleEntries(instData)))
                    {
                        printf("Not a valid choice (Cannot delete the primary record entry!)!\n");
                        playMenu(instData);
                    }
                    else
                    {
                        instData->sdUnregisterTemp = (theChar-'0');
                        CsrBtSdUnregisterServiceRecordReqSend(TESTQUEUE, 0, instData->sdEntry[instData->sdUnregisterTemp].serviceDescriptionHandle);
                    }
                }
                else
                {
                    printf("Not a valid choice (The index is not registered!)!\n");
                    playMenu(instData);
                }
            }
            else
            {
                printf("Not a valid choice (Wrong keypress!)!\n");
                playMenu(instData);
            }
        }
        else
#endif
        {
            switch(theChar)
            {
                case '0':
                    {
                        if((instData->state == idle_s) || (instData->state == active_s) || (instData->state == reconnecting_s))
                        {
                            BD_ADDR_T zeroAddr;
                            CsrBtBdAddrZero(&zeroAddr);
                            if(!CsrBtBdAddrEq(&(instData->bondingDevAddr), &zeroAddr))
                            {
                                if(instData->disconnected)
                                {
                                    if(instData->deviceType == HID_MOUSE)
                                    {
                                        printf("      a)  MOUSE \n");
                                        printf("      c)  MOUSE with known address:%x:%x:%x\n",
                                               instData->bondingDevAddr.nap, instData->bondingDevAddr.uap, instData->bondingDevAddr.lap);
                                    }
                                    else if(instData->deviceType == HID_KEYBOARD)
                                    {
                                        printf("      b)  KEYBOARD \n");
                                        printf("      d)  KEYBOARD with known address:%x:%x:%x\n",
                                               instData->bondingDevAddr.nap, instData->bondingDevAddr.uap, instData->bondingDevAddr.lap);
                                    }

                                }
                                else
                                {
                                    printf("      a)  MOUSE \n");
                                    printf("      b)  KEYBOARD \n");
                                    printf("      c)  MOUSE with known address:%x:%x:%x\n",
                                           instData->bondingDevAddr.nap, instData->bondingDevAddr.uap, instData->bondingDevAddr.lap);
                                    printf("      d)  KEYBOARD with known address:%x:%x:%x\n",
                                           instData->bondingDevAddr.nap, instData->bondingDevAddr.uap, instData->bondingDevAddr.lap);
                                }
                            }
                            else
                            {
                                if(instData->disconnected)
                                {
                                    if(instData->deviceType == HID_MOUSE)
                                    {
                                        printf("      a)  MOUSE \n");
                                    }
                                    else if(instData->deviceType == HID_KEYBOARD)
                                    {
                                        printf("      b)  KEYBOARD \n");
                                    }
                                }
                                else
                                {
                                    printf("      a)  MOUSE \n");
                                    printf("      b)  KEYBOARD \n");
                                }
                            }
                        }
                        break;
                    }
                case 'a':
                    {
                        if((instData->state == idle_s) || (instData->state == active_s) || (instData->state == reconnecting_s))
                        {
                            BD_ADDR_T zeroAddr;
                            CsrBtBdAddrZero(&zeroAddr);
                            instData->deviceType = HID_MOUSE;
                            activate(instData, zeroAddr, instData->deviceType);
                        }
                        break;
                    }
                case 'b':
                    {
                        if((instData->state == idle_s) || (instData->state == active_s) || (instData->state == reconnecting_s))
                        {
                            BD_ADDR_T zeroAddr;
                            CsrBtBdAddrZero(&zeroAddr);
                            instData->deviceType = HID_KEYBOARD;
                            activate(instData, zeroAddr, instData->deviceType);
                        }
                        break;
                    }
                case 'c':
                    {
                        if((instData->state == idle_s) || (instData->state == active_s) || (instData->state == reconnecting_s))
                        {
                            instData->deviceType = HID_MOUSE;
                            activate(instData, instData->bondingDevAddr, instData->deviceType);
                        }
                        break;
                    }
                case 'd':
                    {
                        if((instData->state == idle_s) || (instData->state == active_s) || (instData->state == reconnecting_s))
                        {
                            instData->deviceType = HID_KEYBOARD;
                            activate(instData, instData->bondingDevAddr, instData->deviceType);
                        }
                        break;
                    }
                case 'f':
                    {
                        if((instData->state == idle_s))
                        {
                            handleScKeybPrim(&instData->scInstData);
                        }
                        break;
                    }
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
                case 'G':
                    {
                        if (FALSE == doesPrimaryRecordExist(instData))
                        {
                            primaryRecordTemp = TRUE;
                        }
                        else
                        {
                            printf("A primary Record already exist, parameter changed to FALSE!\n");
                        }
                        /* No breakpoint needed here, need to fall through !!! */
                    }
                case 'g':
                    {
                        CsrUint8 *tempString1, *tempString2, *tempString3;

                        if (NOF_DI_SERVICE_DESCRIPTIONS > nofServiceHandleEntries(instData))
                        {
                            if ((TRUE != doesPrimaryRecordExist(instData)) && (TRUE != primaryRecordTemp))
                            {
                                printf("A primary Record does not exist, parameter changed to TRUE!\n");
                                primaryRecordTemp = TRUE;
                            }
                            tempString1 = CsrPmemAlloc(75);
                            tempString2 = CsrPmemAlloc(25);
                            tempString3 = CsrPmemAlloc(50);

                            CsrMemCpy(tempString1, "This is a test for the Device identification description.", 57);
                            CsrMemCpy(tempString2, "http://client.doc", 17);
                            CsrMemCpy(tempString3, "https://documentation.whatever", 30);

                            instData->sdTempPR = primaryRecordTemp;
                            CsrBtSdRegisterDiServiceRecordV13(TESTQUEUE, 10, 10, 30, primaryRecordTemp, 1,
                                                         tempString1, 57,
                                                         tempString2, 17,
                                                         tempString3, 30);
                        }
                        else
                        {
                            printf("No more internal memory for new service records\n");
                        }
                        break;
                    }
                case 'H':
                    {
                        printOutServiceHandles(instData);
                        break;
                    }
                case 'h':
                    {
                        printOutServiceHandles(instData);
                        printf("Enter the service handle index to unregister:\n");
                        instData->sdUnregisterState = TRUE;
                        break;
                    }
#endif
                case '1' :
                    {
                        deactivate(instData);
                        break;
                    }
                case '2' :
                    {
                        if(instData->state == connected_s)
                        {
                            unplug(instData);
                        }
                        break;
                    }
                case '3' :
                    {
                        if(instData->state == connected_s)
                        {
                            instData->sendMsgType = CSR_BT_HIDD_DATA_REQ;
                            getReport(instData);
                        }
                        break;
                    }
                case '5':
                    {
                        changeLowPowerMode(instData, ACTIVE);
                        break;
                    }
                case '6':
                    {
                        changeLowPowerMode(instData, SNIFF);
                        break;
                    }
                case '7':
                    {
                        changeLowPowerMode(instData, DISCONNECTED);
                        break;
                    }
                case 'z':
                    {
                        if (instData->showPtsExtras)
                        {
                            instData->inquiryScanDisabled = instData->inquiryScanDisabled ? FALSE : TRUE;
                            CsrBtCmWriteScanEnableReqSend(TESTQUEUE, instData->inquiryScanDisabled, FALSE);
                        }
                        break;
                    }
                case 'x':
                case 's':
                case 'q':
                case 'w':
                case 'e':
                case 'r':
                case 't':
                case 'y':
                case 'u':
                    {
                        sendResponse(instData, theChar);
                        break;
                    }
                case 'o':
                    {
                        instData->writeBluetooth = TRUE;
                        sendBluetoothResponse(instData);
                        break;
                    }
                    break;
                case 'p':
                    {
                        if(instData->state == connected_s && instData->showPtsExtras == FALSE)
                        {
                            instData->showPtsExtras = TRUE;
                        }
                        else
                        {
                            instData->showPtsExtras = FALSE;
                        }
                        showHeader(instData);
                        playMenu(instData);
                        break;
                    }
                case 'L':
                    {
                        /* Send long reports on control channel */
                        if (instData->showPtsExtras == TRUE && instData->longControlReport == FALSE)
                        {
                            instData->longControlReport = TRUE;
                            printf("Reply to a Get Report command will now be a long report\n");
                        }
                        else
                        {
                            instData->longControlReport = FALSE;
                            printf("Reply to a Get Report command will now be a short report\n");
                        }
                        break;
                    }
                case 'l':
                    {
                        /* Send long reports on interrupt channel */
                        if (instData->showPtsExtras == TRUE)
                        {
                            CsrUint16 i = 0;
                            CsrUint8* tmpData = NULL;

                            tmpData = CsrPmemZalloc(LONG_REPORT_LENGTH+1);

                            for (i = 1; i < LONG_REPORT_LENGTH + 1; i++)
                            {
                                tmpData[i] = i%256;
                            }
                            printf("Sending %d bytes of data\n", LONG_REPORT_LENGTH);
                            CsrBtHiddDataReqSend(CSR_BT_HIDD_IFACEQUEUE, (CsrUint16)(LONG_REPORT_LENGTH+1), tmpData);
                        }
                        break;
                    }
                case 'm':
                    {
                        /* Enable RF shield */
                        if (instData->showPtsExtras == TRUE)
                        {
                            CsrBccmdWriteReqSend(TESTQUEUE, DISABLE_TX_VARID, 0, 0, NULL);
                            printf("RF shield enabled\n");
                        }
                        break;
                    }
                case 'n':
                    {
                        /* Disable RF shield */
                        if (instData->showPtsExtras == TRUE)
                        {
                            CsrBccmdWriteReqSend(TESTQUEUE, ENABLE_TX_VARID, 0, 0, NULL);
                            printf("RF shield disabled\n");
                        }
                        break;
                    }
                default:
                    {
                        /*    unexpected key pressed */
                        printf("## HID device application key press %c,\n",theChar);
                        break;
                    }
            }
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
