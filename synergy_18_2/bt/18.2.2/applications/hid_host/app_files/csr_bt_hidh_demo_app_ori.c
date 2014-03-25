/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "csr_bt_platform.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_hidh_lib.h"
#include "csr_bt_hidh_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_hid_parser.h"
#include "csr_bt_hidh_demo_app.h"
#include "csr_bt_nvs_hid.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bccmd_lib.h"
#if FW_MAJOR_VERSION >= 3
#include "csr_sched_init.h"
#else
#include "platform/csr_sched_init.h"
#endif

#include "csr_bt_sdc_support.h"
#include "csr_bt_demoapp.h"

#define HID_MINOR_DEV_CLASS_POINTER     0x80
#define HID_MINOR_DEV_CLASS_KEYBOARD    0x40

#define HIDH_MAX_SDP_ATTRIBUTE    300 /* maximum SDP attribute size to read */

#define CHANGE_STATE(s) (instData->state=s)

/* Defines for PTS RF-shielding */
#define ENABLE_TX_VARID  0x4007
#define DISABLE_TX_VARID 0x4008

typedef enum
{
    HID_PARSER_VERSION            = 0x0201,
    HID_DEVICE_SUBCLASS,
    HID_COUNTRY_CODE,
    HID_VIRTUAL_CABLE,
    HID_RECONNECT_INITIATE,
    HID_DESCRIPTOR_LIST,
    HID_LANGID_BASE_LIST,
    HID_SDP_DISABLE,
    HID_BATTERY_POWER,
    HID_REMOTE_WAKE,
    HID_PROFILE_VERSION,
    HID_SUPERVISION_TIMEOUT,
    HID_NORMALLY_CONNECTABLE,
    HID_BOOT_DEVICE,
    ENUM_HID_ATTRIBUTE_END
} hidh_sdp_attrib_t;

const char *state_str[HIDH_NOF_STATES] =
{
    "IDLE",
    "SEARCH",
    "SELECT DEVICE",
    "PAIRING",
    "CONNECTED",
    "AWAIT_CONNECT",
    "UNPLUG",
    "RECONNECTING",
    "AWAIT_USER_REG"
};

/* forward declarations for handler functions    */
void handleKeybPrim(CsrUint8 theChar, hidhInstData_t *instData);
void handleHidhPrim(hidhInstData_t *instData, void *msg);
void handleSdPrim(hidhInstData_t *instData, void *msg);
void handleCmPrim(hidhInstData_t *instData, void *msg);
int _kbhit( void );
int _getch( void );

static void hidhSendReport(CsrUint8 reportId, CsrUint8 data)
{
    CsrUint16    reportLen;
    CsrUint8     *reportData;

    reportLen = 3;
    reportData = CsrPmemAlloc(reportLen);

    reportData[0] = 0; /* Reserved */
    reportData[1] = reportId;
    reportData[2] = data;

    CsrBtHidhDataReqSend(0, CSR_BT_HIDH_OUTPUT_REPORT,
                    reportLen, reportData);
}

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
  playMenu:
************************************************************************************/
void playMenu(hidhInstData_t *instData)
{
    printf("\nSynergy BT Demo - Human Interface Device host (HID host)\n");
    printf("Hit <ESC> to quit! Hit <SPACE> to reprint menu!\n\n");
    printf("Current state: %s\n", state_str[instData->state]);
    printf("\nOptions: \n");

    switch(instData->state)
    {
        case HIDH_STATE_IDLE:
            {
                printf("  0)  Start device search\n");
                printf("  1)  Security Controller Configuration\n");
                printf("  2)  Establish a connection\n");
                printf("  3)  Wait for HID device connect\n");
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
                printf("  q)  Read DI service record\n");
#endif
                printf("  w)  Read HID service record\n");
                break;
            }
        case HIDH_STATE_AWAIT_CONNECT:
            {
                printf("  0)  Cancel connect accept\n");
                break;
            }
        case HIDH_STATE_RECONNECTING:
        case HIDH_STATE_CONNECTED:
            {
                printf("  0)  Unplug (Virtual Cable Unplug)\n");
                printf("  1)  Disconnect\n");
                printf("  2)  Suspend\n");
                printf("  3)  Soft device reset\n");
                printf("  4)  Get Report\n");
                printf("  5)  Set Report\n");
                printf("  6)  Get Protocol\n");
                printf("  7)  Set Protocol (toggle report/boot protocol, currently ");
                if (CSR_BT_HIDH_BOOT_PROTOCOL == instData->protocol)
                {
                    printf("BOOT protocol)\n");
                }
                else
                {
                    printf("REPORT protocol)\n");
                }

                printf("  8)  Get Idle\n");
                printf("  9)  Set Idle\n");
                printf("\n");
                printf("  p)  Toggle Extra PTS features - TEST ONLY!\n");
                if (instData->showPtsExtras == TRUE)
                {
                    printf("         e)  Send invalid report ID \n");   /* PTS special */
                    printf("         f)  Send invalid transaction type\n"); /* PTS special */
                    printf("         l)  Send large output report\n");  /* PTS special */
                    printf("         d)  Disable RF-shield \n");        /* PTS special */
                    printf("         s)  Enable RF-shield \n");         /* PTS special */
                    printf("\n");
                }
                printf("\n  u)  Send activate Num Lock report\n");
                printf("  v)  Send activate Caps Lock report\n");
                printf("  w)  Send activate Scroll Lock report\n");
                printf("  z)  Send deactivate all LEDs (Num, Caps, Scroll) report\n");
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
                printf("  q)  Read DI service record\n");
#endif
                printf("  w)  Read SDP service record\n");
                printf("  x)  Send output report\n");
                break;
            }
        default:
            {
                break;
            }
    }
}


/*************************************************************************************
  startConnecting:
************************************************************************************/
void startConnecting(hidhInstData_t *instData)
{
    CsrBtHidhSdpInfo *sdpInfo = NULL;
    CsrCharString       *serviceName;
    CsrUint16        descLen;
    CsrUint8        *desc;

    if( instData->selectedDevice == CSR_BT_UNDEFINED)
    {
        instData->selectedDevice = 0;
    }

    printf("Connecting  - please wait \n\n");

    Nvs_GetHidInfo(&instData->bdAddrList[instData->selectedDevice],
                   &sdpInfo,
                   &serviceName,
                   &descLen,
                   &desc);

    if( (sdpInfo != NULL) && sdpInfo->isValid )
    {
        CsrBtHidhConnectReqSend( instData->ctrl_app_hdl,
                            instData->selectedDevice,
                            instData->bdAddrList[instData->selectedDevice],
                            L2CA_FLUSH_TO_DEFAULT,
                            NULL,
                            NULL,
                            sdpInfo,
                            serviceName,
                            descLen,
                            desc);
    }
    else
    {
        CsrBtHidhConnectReqSend( instData->ctrl_app_hdl, instData->selectedDevice,
                            instData->bdAddrList[instData->selectedDevice],
                            L2CA_FLUSH_TO_DEFAULT,
                            NULL,
                            NULL,
                            NULL,
                            NULL,
                            0,
                            NULL);
    }
}

/*************************************************************************************
  startConnectAccept:
************************************************************************************/
void startConnectAccept(hidhInstData_t *instData)
{
    CsrBtHidhSdpInfo *sdpInfo = NULL;
    CsrCharString       *serviceName;
    CsrUint16        descLen;
    CsrUint8        *desc;

    if( instData->selectedDevice == CSR_BT_UNDEFINED)
    {
        instData->selectedDevice = 0;
    }

    Nvs_GetHidInfo(&instData->bdAddrList[instData->selectedDevice],
                   &sdpInfo,
                   &serviceName,
                   &descLen,
                   &desc);

    if( (sdpInfo != NULL) && sdpInfo->isValid )
    {
        CsrBtHidhConnectAcceptReqSend( TESTQUEUE, 0,
                                  instData->bdAddrList[instData->selectedDevice],
                                  L2CA_FLUSH_TO_DEFAULT,
                                  NULL,
                                  NULL,
                                  sdpInfo,
                                  serviceName,
                                  descLen,
                                  desc);

        CHANGE_STATE(HIDH_STATE_AWAIT_CONNECT);
    }
    else
    {
        CsrPmemFree(sdpInfo);
        CsrPmemFree(serviceName);
        CsrPmemFree(desc);
        printf("Device information not available, unable to accept connection\n");
        playMenu(instData);
    }
}


/*************************************************************************************
  startSearch:
************************************************************************************/
void startSearch(hidhInstData_t *instData)
{
    instData->selectedDevice = CSR_BT_UNDEFINED;
    instData->numInList    = 0;
    instData->isSearch = TRUE;

    CsrBtSdSearchReqSend(instData->ctrl_app_hdl,
                    CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                    6000, /* RSSI buffer timeout */
                    CSR_BT_INFINITE_TIME, /* Search timeout */
                    CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE, /* RSSI threshold */
                    0, /* Class of Device */
                    0, /* Class of Device mask */
                    CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                    0, /* Filter length */
                    NULL); /* Filter */

    printf("\nSearch in progress - please wait..\n");
    printf("Press  'C' to finish searching and selecting a device\n");
    CHANGE_STATE(HIDH_STATE_SEARCH);
}

/*************************************************************************************
  initInstanceData:
************************************************************************************/
void initInstanceData(hidhInstData_t *instData)
{
    CsrUint8    i;
    extern BD_ADDR_T defGlobalBdAddr;
    CsrBtDeviceAddr zeroAddr;

    instData->state                 = HIDH_STATE_IDLE;
    instData->numInList             = 0;
    instData->selectedDevice        = CSR_BT_UNDEFINED;
    instData->action                = NULL_ACTION;
    instData->device                = NULL;
    instData->nonZeroIdleRate       = FALSE;
    instData->isEncrypted           = FALSE;
    instData->showPtsExtras         = FALSE;
    instData->protocol              = CSR_BT_HIDH_REPORT_PROTOCOL;
    instData->scMsgPending          = FALSE;

    CsrMemSet(&zeroAddr, 0, sizeof(CsrBtDeviceAddr));
    if (!CsrBtBdAddrEq(&defGlobalBdAddr, &zeroAddr))
    {
        for (i = 0; i < MAX_DISCOVERY_RESULTS_IN_LIST; i++)
        {
            instData->bdAddrList[i].lap = defGlobalBdAddr.lap;
            instData->bdAddrList[i].nap = defGlobalBdAddr.nap;
            instData->bdAddrList[i].uap = defGlobalBdAddr.uap;
        }
        instData->selectedDevice = 0;
    }
    else
    {
        for (i = 0; i < MAX_DISCOVERY_RESULTS_IN_LIST; i++)
        {
            instData->bdAddrList[i].lap = 0;
            instData->bdAddrList[i].nap = 0;
            instData->bdAddrList[i].uap = 0;
        }
    }

    instData->hidServiceRecordHandle = 0;;

    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, NULL);
    playMenu(instData);
}


void hidhScAppSetFnFini(void *pFiniData)
{
    hidhInstData_t *instData = (hidhInstData_t *) pFiniData;
    scAppRestoreState((CsrUint8 *) &instData->prevState, (CsrUint8 *) &instData->state);
    playMenu(instData);
}

void hidhScAppSetFnInit(void *pInitData)
{
    hidhInstData_t *instData = (hidhInstData_t *) pInitData;
    scAppSaveState((CsrUint8 *) &instData->prevState, (CsrUint8 *) &instData->state, HIDH_STATE_PAIRING);
}

void* hidhScAppSetFnGetMessage(void *pRecvMsgPData)
{
    hidhInstData_t *instData = (hidhInstData_t *) pRecvMsgPData;
    void *result        = instData->recvMsgP;
    instData->recvMsgP  = NULL;
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
    hidhInstData_t *instData;

    /*  allocate and initialise instance data space  */
    *gash = (void *) CsrPmemAlloc(sizeof(hidhInstData_t));
    instData = (hidhInstData_t *) *gash;

    initInstanceData(instData);
    Nvs_Init();

    /* Get control application handle */
    instData->ctrl_app_hdl = TESTQUEUE;

    /* Register user application handle if NOT in split-mode */
    CsrBtHidhRegisterUserReqSend(CSR_BT_HID_ALL_DEVICE_SLOTS,
                            instData->ctrl_app_hdl);
    CHANGE_STATE(HIDH_STATE_AWAIT_USER_REG);

    /* Security App Configuration */
    scAppInitInstanceData(&instData->scInstData);

    scAppSetFnFini(&instData->scInstData, hidhScAppSetFnFini, instData);
    scAppSetFnInit(&instData->scInstData, hidhScAppSetFnInit, instData);
    scAppSetSecLevelSend(&instData->scInstData, scAppSendProfileSecurityLevelReq, &instData->scInstData);

    scAppSetFnGetMessage(&instData->scInstData, hidhScAppSetFnGetMessage, instData);

    scAppSetSecLevelPrim(&instData->scInstData,
                         0,
                         "HIDH",
                         CSR_BT_HIDH_PRIM, CSR_BT_HIDH_IFACEQUEUE,
                         CSR_BT_HIDH_SECURITY_IN_REQ, CSR_BT_HIDH_SECURITY_OUT_REQ);

    CsrBtScActivateReqSend(instData->ctrl_app_hdl);
}

#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    CsrUint16 msg_type;
    void *msg_data;
    hidhInstData_t *instData;

    instData = (hidhInstData_t *) (*gash);

    scAppDeinitInstanceData(&instData->scInstData);

    /*    get a message from the demoapplication message queue. The message is returned in prim
          and the event type in eventType                                                        */
    while ( CsrSchedMessageGet(&msg_type, &msg_data) )
    {
        switch (msg_type)
        {
            case CSR_BT_HIDH_PRIM:
            {
                CsrBtHidhFreeUpstreamMessageContents(msg_type, msg_data);
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
               CsrBccmdPrim *primType = (CsrBccmdPrim *) msg_data;

                if (*primType == CSR_BCCMD_CFM)
                {
                    ;
                }
                else
                {
                    printf("####### csr_bt_hidh_demo_app.c: unhandle  bccmd message %x,\n", *primType);
                }
                CsrBccmdFreeUpstreamMessageContents(CSR_BCCMD_PRIM, msg_data);
                break;
            }


        }
        CsrPmemFree(msg_data);
    }

    CsrPmemFree(instData->device);
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
    hidhInstData_t *instData;
    CsrUint16    eventType;
    void        *msg;
    /*    get a message from the demoapplication message queue. The message is returned in prim
          and the event type in eventType                                                        */
    instData = (hidhInstData_t *) (*gash);
    CsrSchedMessageGet(&eventType, &msg);

    instData->recvMsgP = msg;

    switch (eventType)
    {
        case CSR_BT_HIDH_PRIM:
            {
                CsrPrim *prim = (CsrPrim *) msg;
                switch(*prim)
                {
                    case CSR_BT_HIDH_SECURITY_OUT_CFM:
                    case CSR_BT_HIDH_SECURITY_IN_CFM:
                        handleScSecLevelPrim(&instData->scInstData);
                        break;

                    default:
                        handleHidhPrim(instData, msg);
                        break;
                }
                break;
            }
        case CSR_BT_SC_PRIM:
            {
                CsrPrim *prim = (CsrPrim *) msg;

                if (instData->scMsgPending)
                {
                    switch(*prim)
                    {
                        case CSR_BT_SC_ENCRYPTION_CFM:
                        {
                            CsrBtScEncryptionCfm *scPrim = (CsrBtScEncryptionCfm*) msg;
                            printf("\nEncryption mode changed to %u for device (%04X:%02X:%06X) with result 0x%02X from supplier 0x%02X\n",
                                           scPrim->encEnabled,
                                           scPrim->deviceAddr.nap,
                                           scPrim->deviceAddr.uap,
                                           scPrim->deviceAddr.lap,
                                           scPrim->resultCode,
                                           scPrim->resultSupplier);
                            break;
                        }

                        default:
                        {
                            printf("Unhandled SC primitive: 0x%04X\n", *prim);
                            break;
                        }
                    }
                    instData->scMsgPending = FALSE;
                }
                else
                {
                    handleScPrim(&instData->scInstData);
                }
                break;
            }
        case CSR_BT_SD_PRIM:
            {
                handleSdPrim(instData, msg);
                break;
            }
        case CSR_BT_CM_PRIM:
            {
                handleCmPrim(instData, msg);
                break;
            }
        case CSR_BCCMD_PRIM:
        {
           CsrBccmdPrim *primType = (CsrBccmdPrim *) msg;

            if (*primType == CSR_BCCMD_CFM)
            {
                ;
            }
            else
            {
                printf("####### csr_bt_hidh_demo_app.c: unhandle  bccmd message %x,\n", *primType);
            }
            CsrBccmdFreeUpstreamMessageContents(CSR_BCCMD_PRIM, msg);
            break;
        }
        case KEY_MESSAGE:
            {
                CsrBtKeyPress_t *key;
                key = (CsrBtKeyPress_t*)msg;

                handleKeybPrim(key->key, instData);
                break;
            }
        default:
            {
                printf("####### unhandled event in A2DP handler %x,\n",eventType);
            }
    }
    /*    free the received signal again. If the signal for some reason must be stored in the application
          the pointer (prim) must be set to NULL in order not to free it here
    */
    CsrPmemFree(instData->recvMsgP);
}

void printOutHidData(CsrUint8 *data,
                     CsrUint16 dataLen)
{
    CsrUint16 i, startDataIndex = 0;

    if (1 == data[1])
    {
        printf("Keyboard(%02x): ", data[1]);
        startDataIndex = 2;
    }
    else if (2 == data[1])
    {
        printf("Mouse(%02x): buttons(%02x), X(%i), Y(%i) ", data[1], data[2], (CsrInt8)data[3], (CsrInt8)data[4]);
        startDataIndex = 5;
    }
    for(i=startDataIndex; i<dataLen;i++)
    {
        printf("%02x ", data[i]);
    }
    printf("\n%d bytes received\n", dataLen);

}

void printOutHandshakeResultCode(CsrBtResultCode resultCode)
{
    switch (resultCode)
    {
        case CSR_BT_RESULT_CODE_HIDH_SUCCESS:
            {
                printf("CSR_BT_RESULT_CODE_HIDH_SUCCESS");
                break;
            }
        case CSR_BT_RESULT_CODE_HIDH_NOT_READY:
            {
                printf("CSR_BT_RESULT_CODE_HIDH_NOT_READY");
                break;
            }
        case CSR_BT_RESULT_CODE_HIDH_INVALID_REPORT_ID:
            {
                printf("CSR_BT_RESULT_CODE_HIDH_INVALID_REPORT_ID");
                break;
            }
        case CSR_BT_RESULT_CODE_HIDH_UNSUPPORTED:
            {
                printf("CSR_BT_RESULT_CODE_HIDH_UNSUPPORTED");
                break;
            }
        case CSR_BT_RESULT_CODE_HIDH_INVALID_PARAMETER:
            {
                printf("CSR_BT_RESULT_CODE_HIDH_INVALID_PARAMETER");
                break;
            }
        case CSR_BT_RESULT_CODE_HIDH_UNKNOWN:
            {
                printf("CSR_BT_RESULT_CODE_HIDH_UNKNOWN");
                break;
            }
        case CSR_BT_RESULT_CODE_HIDH_FATAL:
            {
                printf("CSR_BT_RESULT_CODE_HIDH_FATAL");
                break;
            }
        default:
            {
                printf("Unknown result code");
                break;
            }
    }
}

/**************************************************************************************************
Function to handle all hidh-primitives.
**************************************************************************************************/
void handleHidhPrim(hidhInstData_t *instData, void *msg)
{
    CsrBtHidhPrim    *primType;

    primType = (CsrBtHidhPrim *) msg;

    switch (*primType)
    {
        case CSR_BT_HIDH_DATA_IND:
            {
                CsrBtHidhDataInd        *prim = (CsrBtHidhDataInd *) msg;

                if( prim->reportType != CSR_BT_HIDH_OTHER)
                {
                    printf("Report(%i): ", prim->reportType);
                    printOutHidData(prim->data, prim->dataLen);
                }
                else
                {
                    if( instData->action == GET_PROTOCOL_ACTION)
                    {
                        instData->protocol = prim->data[1];
                        if( instData->protocol )
                        {
                            printf("Get protocol returned: Report\n");
                        }
                        else
                        {
                            printf("Get protocol returned: Boot");
                        }
                    }
                    else if( instData->action == GET_IDLE_ACTION)
                    {
                        printf("Get idle returned: %i", prim->data[1]);
                    }
                    instData->action = NULL_ACTION;

                }
                printf("\n");

                CsrPmemFree(prim->data);
                break;
            }
        case CSR_BT_HIDH_REGISTER_USER_CFM:
            {
                CsrBtHidhRegisterUserCfm *prim = (CsrBtHidhRegisterUserCfm*)msg;

                if(prim->resultCode == CSR_BT_RESULT_CODE_HIDH_SUCCESS &&
                   prim->resultSupplier == CSR_BT_SUPPLIER_HIDH)
                {
                    printf("Received a successful REGISTER_USER confirmation for deviceId %i\n",
                           prim->slotId);
                    CHANGE_STATE(HIDH_STATE_IDLE);
                }
                else
                {
                    printf("Received a REGISTER_USER error with code 0x%02X (supplier 0x%02X)\n", prim->resultCode, prim->resultSupplier);
                }
                break;
            }

        case CSR_BT_HIDH_CONNECT_ACCEPT_IND:
            {
                CsrBtHidhSdpInfo *sdpInfo = NULL;
                CsrCharString *serviceName;
                CsrUint16 descLen;
                CsrUint8 *desc;

                Nvs_GetHidInfo(&instData->bdAddrList[instData->selectedDevice],
                               &sdpInfo,
                               &serviceName,
                               &descLen,
                               &desc);

                if( (sdpInfo != NULL) && (sdpInfo->deviceSubclass & HID_MINOR_DEV_CLASS_KEYBOARD))
                {
                    /* the device is a keyboard => encrypt connection */
                    CsrBtScEncryptionReqSend(instData->ctrl_app_hdl,
                                        instData->bdAddrList[instData->selectedDevice], TRUE);
                    instData->scMsgPending = TRUE;
                }
                CsrPmemFree(sdpInfo);
                CsrPmemFree(serviceName);
                CsrPmemFree(desc);

                CHANGE_STATE(HIDH_STATE_CONNECTED);

                playMenu(instData);
                break;
            }
        case CSR_BT_HIDH_CONNECT_CFM:
            {
                CsrBtHidhConnectCfm        *prim = (CsrBtHidhConnectCfm *) msg;

                if (prim->resultCode == CSR_BT_RESULT_CODE_HIDH_SUCCESS &&
                    prim->resultSupplier == CSR_BT_SUPPLIER_HIDH)
                {
                    if( (prim->sdpInfo != NULL) && (prim->sdpInfo->deviceSubclass & HID_MINOR_DEV_CLASS_KEYBOARD))
                    {
                        /* the device is a keyboard => encrypt connection */
                        CsrBtScEncryptionReqSend(instData->ctrl_app_hdl,
                                            instData->bdAddrList[instData->selectedDevice],
                                            TRUE);
                        instData->scMsgPending = TRUE;
                    }
                    CHANGE_STATE(HIDH_STATE_CONNECTED);
                }
                else
                {
                    printf("Establishment failed, remote device not responding...\n");
                }
                playMenu(instData);

                if(prim->sdpInfo)
                {
                    printf("\nHID device plugged in: %s\n", prim->serviceName);
                    printf("Vendor ID: %X, Product ID: %X, Version: %i.%i.%i\n",
                           prim->sdpInfo->vendorId,
                           prim->sdpInfo->productId,
                           prim->sdpInfo->deviceVersion >> 8,
                           (prim->sdpInfo->deviceVersion & 0x00F0) >> 4,
                           prim->sdpInfo->deviceVersion & 0x000F );

                    Nvs_PutHidInfo(&prim->deviceAddr,
                                   prim->sdpInfo,
                                   prim->serviceName,
                                   prim->descriptorLength,
                                   prim->descriptor);

                }
                CsrPmemFree(prim->descriptor);
                CsrPmemFree(prim->serviceName);
                CsrPmemFree(prim->sdpInfo);

                break;
            }
        case CSR_BT_HIDH_CONNECT_ACCEPT_CFM:
            {
                CsrBtHidhConnectAcceptCfm        *prim = (CsrBtHidhConnectAcceptCfm *) msg;

                if(prim->resultCode != CSR_BT_RESULT_CODE_HIDH_SUCCESS ||
                   prim->resultSupplier != CSR_BT_SUPPLIER_HIDH)
                {
                    printf("Connect accept call rejected with result: 0x%02X (supplier 0x%02X)\n", prim->resultCode, prim->resultSupplier);
                }
                else
                {
                    printf( "Waiting for HID device (%04X:%02X:%06X) to connect...  \n\n",
                            prim->deviceAddr.nap,
                            prim->deviceAddr.uap,
                            prim->deviceAddr.lap );
                }
                playMenu(instData);
                break;
            }
        case CSR_BT_HIDH_DISCONNECT_CFM:
        case CSR_BT_HIDH_DISCONNECT_IND:
            {
                CHANGE_STATE(HIDH_STATE_IDLE);

                playMenu(instData);
                break;
            }
        case CSR_BT_HIDH_CANCEL_CONNECT_ACCEPT_CFM:
            {
                if( instData->state == HIDH_STATE_AWAIT_CONNECT )
                {
                    CsrBtHidhCancelConnectAcceptCfm    *prim = (CsrBtHidhCancelConnectAcceptCfm *) msg;

                    if(prim->resultCode == CSR_BT_RESULT_CODE_HIDH_SUCCESS && prim->resultSupplier == CSR_BT_SUPPLIER_HIDH )
                    {
                        CHANGE_STATE(HIDH_STATE_IDLE);
                    }
                    else
                    {
                        printf("Failed to cancel connect accept, resultCode: 0x%04X, resultSupplier: 0x%04X\n", prim->resultCode, prim->resultSupplier);
                    }

                    playMenu(instData);
                }
                break;
            }
        case CSR_BT_HIDH_CONTROL_IND:
            {
                CsrBtHidhControlInd    *prim = (CsrBtHidhControlInd *) msg;

                if( prim->operation == CSR_BT_HIDH_VC_UNPLUG)
                {
                    CsrBtHidhDisconnectReqSend(0);
                    Nvs_RemoveHidInfo(&instData->bdAddrList[instData->selectedDevice]);
                }
                break;
            }
        case CSR_BT_HIDH_HANDSHAKE_IND:
            {
                CsrBtHidhHandshakeInd    *prim = (CsrBtHidhHandshakeInd *) msg;

                printf("Received HANDSHAKE, result code: 0x%02X (", prim->resultCode);
                printOutHandshakeResultCode(prim->resultCode);
                printf(")\n");

                if(prim->resultCode != CSR_BT_RESULT_CODE_HIDH_SUCCESS)
                {
                    instData->action = NULL_ACTION;
                }

                break;
            }
        case CSR_BT_HIDH_STATUS_IND:
            {
                CsrBtHidhStatusInd    *prim = (CsrBtHidhStatusInd *) msg;

                if( prim->status == CSR_BT_HIDH_STATUS_CONNECTED )
                {
                    if(instData->state == HIDH_STATE_RECONNECTING)
                    {
                        CHANGE_STATE(HIDH_STATE_CONNECTED);
                        playMenu(instData);
                        if( instData->isEncrypted )
                        {
                            /* the connection was encrypted before the drop-out, start encryption again */
                            CsrBtScEncryptionReqSend(instData->ctrl_app_hdl, instData->bdAddrList[instData->selectedDevice], TRUE);
                            instData->scMsgPending = TRUE;
                        }
                    }

                    if( prim->descriptor != NULL)
                    {
                        instData->device = CsrBtHidParse( prim->descriptor,
                                                     prim->descriptorLength);

                    }
                }
                else if(prim->status == CSR_BT_HIDH_STATUS_RECONNECTING)
                {
                    CHANGE_STATE(HIDH_STATE_RECONNECTING);
                    playMenu(instData);
                }

                CsrPmemFree(prim->descriptor);
                CsrPmemFree(prim->serviceName);
                CsrPmemFree(prim->sdpInfo);

                break;
            }

        case CSR_BT_HIDH_SECURITY_IN_CFM:
        case CSR_BT_HIDH_SECURITY_OUT_CFM:
            {
                handleScSecLevelPrim(&instData->scInstData);
                break;
            }
        default:
            {
                printf("## HID host app. unhandled HIDH prim 0x%04x,\n", *primType);
                CsrBtHidhFreeUpstreamMessageContents(CSR_BT_HIDH_PRIM, msg);
            }
    }
}

/**************************************************************************************************
Function to handle all sd-primitives.
**************************************************************************************************/
void handleSdPrim(hidhInstData_t *instData, void *msg)
{
    CsrBtSdPrim    *primType;

    primType = (CsrBtSdPrim *)msg;

    switch (*primType)
    {
        case CSR_BT_SD_SEARCH_RESULT_IND:
            {
                CsrBtSdSearchResultInd *prim = (CsrBtSdSearchResultInd *)msg;
                if (instData->state == HIDH_STATE_SEARCH)
                {
                    CsrUint16 nameLen;
                    CsrUint8 *name;

                    if( instData->numInList < MAX_DISCOVERY_RESULTS_IN_LIST )
                    {
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

                        if (instData->numInList == MAX_DISCOVERY_RESULTS_IN_LIST)
                        {
                            /* Reached maximum result list entries. Cancelling search */
                            CsrBtSdCancelSearchReqSend(instData->ctrl_app_hdl);
                        }
                    }
                }
                CsrPmemFree(prim->info);
                break;
            }
        case CSR_BT_SD_CLOSE_SEARCH_IND:
            {
                if( instData->numInList > 0 )
                {
                    CHANGE_STATE(HIDH_STATE_SELECT_DEVICE);
                    printf("Select a device! ");
                }
                else
                {
                    CHANGE_STATE(HIDH_STATE_IDLE);
                    playMenu(instData);
                }
                break;
            }
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
        case CSR_BT_SD_READ_SERVICE_RECORD_IND:
            {
                CsrBtSdReadServiceRecordInd *prim = (CsrBtSdReadServiceRecordInd *) msg;
                CsrBtSdDiServiceRecordV13Struct v13;

                printf("CSR_BT_SD_READ_SERVICE_RECORD_IND with length %d\n", prim->dataLen);

                CsrBtSdExtractDiServiceRecordV13Data(prim->data, prim->dataLen, &v13);
                CmnDiPrintServiceRecordV13(&v13);

                CsrPmemFree(prim->data);
                break;
            }
        case CSR_BT_SD_READ_SERVICE_RECORD_CFM:
            {
                CsrBtSdReadServiceRecordCfm *prim = (CsrBtSdReadServiceRecordCfm *) msg;

                if (prim->resultSupplier == CSR_BT_SUPPLIER_SD)
                {
                    if (prim->resultCode == CSR_BT_RESULT_CODE_SD_SUCCESS)
                    {
                        printf("CSR_BT_SD_READ_SERVICE_RECORD_CFM SUCCESS\n");
                    }
                    else if (prim->resultCode == CSR_BT_RESULT_CODE_SD_SEARCH_CANCELLED)
                    {
                        printf("CSR_BT_SD_READ_SERVICE_RECORD_CFM CANCELLED\n");
                    }
                    else
                    {
                        printf("CSR_BT_SD_READ_SERVICE_RECORD_CFM ERROR\n");
                    }
                }
                else
                {
                    printf("CSR_BT_SD_READ_SERVICE_RECORD_CFM ERROR\n");
                }
                break;
            }
#endif
        default:
            {    /*    unexpected primitive received    */
                printf("## HID host app. unhandled SD prim %x,\n", *primType);
                CsrBtSdFreeUpstreamMessageContents(*primType, msg);
            }
    }
}


/**************************************************************************************************
Function to handle all CM-primitives.
**************************************************************************************************/
void handleCmPrim(hidhInstData_t *instData, void *msg)
{
    CsrBtCmPrim    *primType;

    primType = (CsrBtCmPrim *)msg;

    switch (*primType)
    {
        case CSR_BT_CM_SDC_SEARCH_CFM:
            {
                CsrBtCmSdcSearchCfm *prim = (CsrBtCmSdcSearchCfm*) msg;

                if( instData->hidServiceRecordHandle )
                {
                    instData->sdpAttrib = CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER;
                    CsrBtCmSdcAttributeReqSend( instData->hidServiceRecordHandle, instData->sdpAttrib,
                                                HIDH_MAX_SDP_ATTRIBUTE);
                }
                else
                {
                    printf("instData->hidServiceRecordHandle == NULL\n");
                }

                break;
            }

        case CSR_BT_CM_SDC_ATTRIBUTE_CFM:
            {
                CsrBtCmSdcAttributeCfm *prim = (CsrBtCmSdcAttributeCfm*) msg;

                if(prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
                   prim->resultSupplier == CSR_BT_SUPPLIER_CM)
                {
                    switch (instData->sdpAttrib)
                    {
                        case CSR_BT_SERVICE_NAME_ATTRIBUTE_IDENTIFIER:
                            instData->sdpAttrib = HID_PARSER_VERSION;
                            break;

                        case HID_PARSER_VERSION:
                            printf(".");
                            instData->sdpAttrib++;
                            break;

                        case HID_DEVICE_SUBCLASS:
                            printf(".");
                            instData->sdpAttrib++;
                            break;

                        case HID_COUNTRY_CODE:
                            printf(".");
                            instData->sdpAttrib++;
                            break;

                        case HID_VIRTUAL_CABLE:
                            printf(".");
                            instData->sdpAttrib++;
                            break;

                        case HID_RECONNECT_INITIATE:
                            printf(".");
                            instData->sdpAttrib++;
                            break;

                        case HID_DESCRIPTOR_LIST:
                            printf(".");
                            instData->sdpAttrib++;
                            break;

                        case HID_LANGID_BASE_LIST:
                            printf(".");
                            instData->sdpAttrib++;
                            break;

                        case HID_SDP_DISABLE:
                            printf(".");
                            instData->sdpAttrib++;
                            break;

                        case HID_BATTERY_POWER:
                            printf(".");
                            instData->sdpAttrib++;
                            break;

                        case HID_REMOTE_WAKE:
                            printf(".");
                            instData->sdpAttrib++;
                            break;

                        case HID_PROFILE_VERSION:
                            printf(".");
                            instData->sdpAttrib++;
                            break;

                        case HID_SUPERVISION_TIMEOUT:
                            printf(".");
                            instData->sdpAttrib++;
                            break;

                        case HID_NORMALLY_CONNECTABLE:
                            printf(".");
                            instData->sdpAttrib++;
                            break;

                        case HID_BOOT_DEVICE:
                            printf(".");
                            CsrBtCmSdcCloseReqSend(TESTQUEUE);
                            instData->sdpAttrib = 0xffff;
                            break;

                        default:
                            printf("Unsupported SDP attribute\n");
                            instData->sdpAttrib = 0xffff;
                            break;
                    }

                    if (instData->sdpAttrib != 0xffff)
                    {
                        CsrBtCmSdcAttributeReqSend( instData->hidServiceRecordHandle, instData->sdpAttrib,
                                                    HIDH_MAX_SDP_ATTRIBUTE);
                    }
                }
                else
                {
                    printf("Could not read Service Record\n");
                }

                break;
            }

        case CSR_BT_CM_SDC_SEARCH_IND:
            {
                CsrBtCmSdcSearchInd *prim = (CsrBtCmSdcSearchInd*) msg;

                if( prim->service == CSR_BT_HID_PROFILE_UUID)
                {
                    instData->hidServiceRecordHandle = *prim->serviceHandleList;
                }
                else
                {
                    printf("Invalid UUID = 0x%2x\n",prim->service);
                }

                break;
            }

        case CSR_BT_CM_SDC_CLOSE_IND:
            {
                CsrBtCmSdcCloseInd *prim = (CsrBtCmSdcCloseInd*) msg;

                printf("\nDone reading SDP record\n");
                break;
            }

        default:
            {    /*    unexpected primitive received    */
                printf("## HID host app. unhandled CM prim %x,\n", *primType);
                CsrBtCmFreeUpstreamMessageContents(*primType, msg);
            }
    }
}


/**************************************************************************************************
 *
 *    a keyboard event is received.
 *    first check for selection of a found device during inquiry if state is inquiry. If not start
 *    proper action as selected.
 *
 **************************************************************************************************/
void handleKeybPrim(CsrUint8 theChar, hidhInstData_t *instData)
{
    if (theChar == ESCAPE_KEY)
    {
        printf("\nUser exit...\n");

        Nvs_Flush();

#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif
    }
    else if(theChar == ' ')
    {
        playMenu(instData);
        return;
    }

    else
    {
        switch(instData->state)
        {
            case HIDH_STATE_IDLE :
                {
                    switch(theChar)
                    {
                        case '0':
                            {
                                startSearch(instData);
                                break;
                            }
                        case '1' :
                            {
                                handleScKeybPrim(&instData->scInstData);
                                break;
                            }
                        case '2' :
                            {
                                startConnecting(instData);
                                break;
                            }
                        case '3' :
                            {
                                CsrUint8 i;

                                instData->isSearch = FALSE;
                                instData->numInList = Nvs_GetHidDeviceList(3, instData->bdAddrList);

                                if( instData->numInList )
                                {
                                    printf("Select from list of known devices:\n");
                                    for(i=0;i<instData->numInList;i++)
                                    {
                                        printf(" %i) %04X:%02X:%06X\n",
                                               i,
                                               instData->bdAddrList[i].nap,
                                               instData->bdAddrList[i].uap,
                                               instData->bdAddrList[i].lap);
                                    }
                                    printf("\n");
                                    CHANGE_STATE(HIDH_STATE_SELECT_DEVICE);
                                }
                                else
                                {
                                    printf("No known devices\n\n");
                                    playMenu(instData);
                                }
                                break;
                            }
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
                        case 'q':
                        case 'Q':
                            {
                                printf("Read DI service record ... \n");
                                CsrBtSdReadDiServiceRecordV13(TESTQUEUE, instData->bdAddrList[instData->selectedDevice]);
                                break;
                            }
#endif

                        case 'w':
                        case 'W':
                            {
                                CsrBtUuid32 *service = (CsrBtUuid32 *)CsrPmemAlloc(sizeof(CsrBtUuid32) * 1);

                                printf("Read SDP service record ... \n");
                                /* do service search first */
                                service[0] = CSR_BT_HID_PROFILE_UUID;

                                CsrBtCmSdcSearchReqSend(TESTQUEUE,
                                                        instData->bdAddrList[instData->selectedDevice],
                                                        service, 1);

                                break;
                            }
                    }
                    break;
                }
            case HIDH_STATE_SEARCH :
                {
                    switch(theChar)
                    {
                        case 'c':
                        case 'C':
                            {
                                printf("\nCancelling discovery - please wait ....\n");
                                CsrBtSdCancelSearchReqSend(instData->ctrl_app_hdl);
                                break;
                            }
                        default:
                            {
                                break;
                            }
                    }
                    break;
                }
            case HIDH_STATE_PAIRING :
                {
                    handleScKeybPrim(&instData->scInstData);
                    break;
                }
            case HIDH_STATE_SELECT_DEVICE:
                {
                    if( (theChar - '0') < instData->numInList)
                    {
                        instData->selectedDevice = (theChar - '0');
                        if( instData->isSearch )
                        {
                            CHANGE_STATE(HIDH_STATE_IDLE);
                            playMenu(instData);
                        }
                        else
                        {
                            startConnectAccept(instData);
                        }
                        scAppSetBondingAddr(&instData->scInstData,
                                            instData->bdAddrList[instData->selectedDevice]);
                    }
                    else
                    {
                        if( instData->numInList != 0 )
                        {
                            printf("Bad selection. Try again\n");
                        }
                    }
                    break;
                }
            case HIDH_STATE_RECONNECTING:
            case HIDH_STATE_CONNECTED:
                {
                    switch(theChar)
                    {
                        case '0' :
                            {
                                printf("Unplugging...\n\n");
                                CsrBtHidhControlReqSend(0, CSR_BT_HIDH_VC_UNPLUG);
                                Nvs_RemoveHidInfo(&instData->bdAddrList[instData->selectedDevice]);
                                break;
                            }

                        case '1' :
                            {
                                printf("Disconnecting...\n\n");
                                CsrBtHidhDisconnectReqSend(0);
                                break;
                            }

                        case '2' :
                            {
                                printf("Suspending...\n\n");
                                CsrBtHidhControlReqSend(0, CSR_BT_HIDH_SUSPEND);
                                break;
                            }
                        case '3' :
                            {
                                printf("Resetting(soft)...\n\n");
                                CsrBtHidhControlReqSend(0, CSR_BT_HIDH_SOFT_RESET);
                                break;
                            }

                        case '4' :
                            {
                                struct CsrBtHidReport    *report = instData->device->reportType[CSR_BT_HID_INPUT_REP].reportIndex[0];

                                printf("Get report...\n\n");
                                CsrBtHidhGetReportReqSend( 0, CSR_BT_HIDH_INPUT_REPORT, (CsrUint8) report->id, (CsrUint16) (report->size/8));
                                break;
                            }
                        case '5' :
                            {
                                CsrUint16            reportLen, i;
                                CsrUint8            *reportData;
                                struct CsrBtHidReport *report;

                                report    = instData->device->reportType[CSR_BT_HID_INPUT_REP].reportIndex[0];
                                reportLen = report->size/8 + 1;

                                i = 1;
                                if(report->id != 0)
                                {
                                    reportLen++;
                                    reportData = CsrPmemZalloc(reportLen);
                                    reportData[i++] = report->id;
                                }
                                else
                                {
                                    reportData = CsrPmemAlloc(reportLen);
                                }

                                printf("Set report...\n\n");

                                CsrBtHidhSetReportReqSend(0, CSR_BT_HIDH_INPUT_REPORT, reportLen, reportData);
                                break;
                            }
                        case '6' :
                            {
                                printf("Get protocol...\n\n");
                                CsrBtHidhGetProtocolReqSend(0);
                                instData->action = GET_PROTOCOL_ACTION;
                                break;
                            }
                        case '7' :
                            {
                                printf("Setting protocol to ");
                                if( instData->protocol == CSR_BT_HIDH_BOOT_PROTOCOL)
                                {
                                    printf("REPORT protocol\n\n");
                                    instData->protocol      = CSR_BT_HIDH_REPORT_PROTOCOL;
                                    CsrBtHidhSetProtocolReqSend(0, CSR_BT_HIDH_REPORT_PROTOCOL);
                                }
                                else
                                {
                                    printf("BOOT protocol\n\n");
                                    instData->protocol      = CSR_BT_HIDH_BOOT_PROTOCOL;
                                    CsrBtHidhSetProtocolReqSend(0, CSR_BT_HIDH_BOOT_PROTOCOL);
                                }
                                break;
                            }

                        case '8' :
                            {
                                printf("Get idle...\n\n");
                                CsrBtHidhGetIdleReqSend(0);
                                instData->action = GET_IDLE_ACTION;
                                break;
                            }
                        case '9' :
                            {
                                printf("Set idle...\n\n");
                                if(instData->nonZeroIdleRate)
                                {
                                    CsrBtHidhSetIdleReqSend(0, 0);
                                    instData->nonZeroIdleRate = FALSE;
                                }
                                else
                                {
                                    CsrBtHidhSetIdleReqSend(0, 250); /* get report at least once per second */
                                    instData->nonZeroIdleRate = TRUE;
                                }
                                break;
                            }
                        case 'u':
                            {
                                printf("Send set activate Num Lock report ...\n\n");
                                hidhSendReport(1, 1);
                                break;
                            }

                        case 'v':
                            {
                                printf("Send set activate Caps Lock report ...\n\n");
                                hidhSendReport(1, 2);
                                break;
                            }


                        case 'w':
                            {
                                printf("Send set activate scroll Lock report ...\n\n");
                                hidhSendReport(1, 4);
                                break;
                            }
                        case 'z':
                            {
                                printf("Send set deactivate all LEDs report ...\n\n");
                                hidhSendReport(1, 0);
                                break;
                            }

                        case 'p': /* Fall through */
                        case 'P':
                            {
                                if (instData->showPtsExtras == TRUE)
                                {
                                    instData->showPtsExtras = FALSE;
                                }
                                else
                                {
                                    instData->showPtsExtras = TRUE;
                                }
                                playMenu(instData);
                                break;
                            }
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
                        case 'q':
                        case 'Q':
                            {
                                printf("Read DI service record ... \n");
                                CsrBtSdReadDiServiceRecordV13(TESTQUEUE, instData->bdAddrList[instData->selectedDevice]);
                                break;
                            }
#endif
                        case 's' : /* Fall through */
                        case 'S' :
                            {
                                if (instData->showPtsExtras == TRUE)
                                {
                                    CsrBccmdWriteReqSend(TESTQUEUE, DISABLE_TX_VARID, 0, 0, NULL);
                                    printf("RF shield enabled\n");
                                }
                                break;
                            }
                        case 'd':    /* Fall through */
                        case 'D':
                            {
                                if (instData->showPtsExtras == TRUE)
                                {
                                    CsrBtHidhSdpInfo *sdpInfo = NULL;
                                    CsrCharString *serviceName;
                                    CsrUint16 descLen;
                                    CsrUint8 *desc;

                                    CsrBccmdWriteReqSend(TESTQUEUE, ENABLE_TX_VARID, 0, 0, NULL);
                                    Nvs_GetHidInfo(&instData->bdAddrList[instData->selectedDevice],
                                                   &sdpInfo,
                                                   &serviceName,
                                                   &descLen,
                                                   &desc);

                                    if( (sdpInfo != NULL) && sdpInfo->isValid )
                                    {
                                        L2CA_QOS_T *qos = CsrPmemAlloc(sizeof(L2CA_QOS_T));

                                        printf("\nHID device: %s\n", serviceName);
                                        printf("Vendor ID: %X, Product ID: %X, Version: %i.%i.%i\n",
                                               sdpInfo->vendorId,
                                               sdpInfo->productId,
                                               sdpInfo->deviceVersion >> 8,
                                               (sdpInfo->deviceVersion & 0x00F0) >> 4,
                                               sdpInfo->deviceVersion & 0x000F );

                                        qos->service_type = L2CA_QOS_TYPE_GUARANTEED;
                                        qos->token_rate = 800;
                                        qos->token_bucket = 8;
                                        qos->peak_bw = 800;
                                        qos->latency = 10000;
                                        qos->delay_var = L2CA_QOS_DEFAULT_DELAY_VAR;

                                        CsrBtHidhConnectAcceptReqSend( TESTQUEUE,
                                                                  1,
                                                                  instData->bdAddrList[instData->selectedDevice],
                                                                  L2CA_FLUSH_TO_DEFAULT,
                                                                  NULL,
                                                                  qos,
                                                                  sdpInfo,
                                                                  serviceName,
                                                                  descLen,
                                                                  desc);
                                    }
                                    else
                                    {
                                        CsrPmemFree(sdpInfo);
                                        CsrPmemFree(serviceName);
                                        CsrPmemFree(desc);
                                    }
                                    printf("RF shield disabled\n");
                                }
                                break;
                            }

                        case 'e':
                        case 'E':
                            {
                                CsrUint16            reportLen, i;
                                CsrUint8            *reportData;
                                struct CsrBtHidReport *report;

                                report    = instData->device->reportType[CSR_BT_HID_INPUT_REP].reportIndex[0];
                                reportLen = report->size/8 + 1;

                                i = 1;
                                if(report->id != 0)
                                {
                                    reportLen++;
                                    reportData = CsrPmemZalloc(reportLen);
                                    reportData[i++] = report->id;
                                }
                                else
                                {
                                    reportData = CsrPmemAlloc(reportLen);
                                }

                                printf("Send report with invalid ID ...\n\n");

                                CsrBtHidhSetReportReqSend(0, 0x04, reportLen, reportData);
                                break;
                            }

                        case 'f':
                        case 'F':
                        {/* Send invalid transaction type */
                            CsrUint8 *data = CsrPmemAlloc(1);

                            printf("Sent invalid transaction type (0xf)\n");
                            CsrBtHidhSetReportReqSend(0, 0xF0, 1, data);
                            break;
                        }

                        case 'x' :
                            {
                                CsrUint16             reportLen, i;
                                CsrUint8             *reportData;
                                struct CsrBtHidReport  *report;

                                report = instData->device->reportType[CSR_BT_HID_OUTPUT_REP].reportIndex[0];

                                if( report != 0)
                                {
                                    reportLen = report->size/8 + 1;

                                    i = 1;
                                    if(report->id != 0)
                                    {
                                        reportLen++;
                                        reportData = CsrPmemZalloc(reportLen);
                                        reportData[i++] = report->id;
                                    }
                                    else
                                    {
                                        reportData = CsrPmemAlloc(reportLen);
                                    }

                                    printf("Sending output report...\n\n");

                                    CsrBtHidhDataReqSend(0, CSR_BT_HIDH_OUTPUT_REPORT, reportLen, reportData);
                                }
                                else
                                {
                                    printf("Not possible to send output report, no output on device!\n\n");
                                }
                                break;
                            }
                        case 'l' : /* Fall through */
                        case 'L' :
                            {
                                if (instData->showPtsExtras == TRUE)
                                {
                                    CsrUint16    reportLen;
                                    CsrUint8        *reportData;
                                    struct CsrBtHidReport    *report;
                                    CsrUint16 i = 1;

                                    report = instData->device->reportType[CSR_BT_HID_OUTPUT_REP].reportIndex[0];

                                    if( report != 0)
                                    {
                                        reportLen = 2500;
                                        reportLen++;
                                        reportData = CsrPmemZalloc(reportLen);

                                        reportData[i] = report->id;
                                        for (; i<reportLen; i++)
                                        {
                                            reportData[i] = i%256;
                                        }
                                        printf("Sending large output report...\n");
                                        CsrBtHidhDataReqSend(0, CSR_BT_HIDH_OUTPUT_REPORT, reportLen, reportData);
                                    }
                                    else
                                    {
                                        printf("Not possible to send output report, no output on device!\n\n");
                                    }
                                }
                                break;
                            }

                    }
                    break;
                }
            case HIDH_STATE_AWAIT_CONNECT:
                {
                    switch(theChar)
                    {
                        case '0' :
                            {
                                CsrBtHidhCancelConnectAcceptReqSend(0);
                                printf("Cancelling connect accept - please wait!\n");
                                break;
                            }
                    }
                    break;
                }
            case HIDH_STATE_AWAIT_USER_REG:
                {
                    printf("\nUser input detected before receiving successful REGISTER_USER confirmation. \
                        \nUser input ignored.\n\n");
                    break;
                }
            default :
                {
                    printf("Bad selection, or not allowed right now\n");
                    break;
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
