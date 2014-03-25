/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "csr_pmem.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_bnep_prim.h"
#include "csr_bt_bsl_prim.h"
#include "csr_bt_bsl_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_internet_primitives.h"
#include "csr_bt_ip_prim.h"
#include "csr_bt_icmp_prim.h"
#include "csr_bt_dhcp_prim.h"
#include "csr_bt_platform.h"
#include "csr_bt_demoapp.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_sched_init.h"

/* Misc settings */
#define PING_DATA_SIZE                          32
#define MAX_KEYBOARD_STRING_SIZE                20
#define KEYB_CHECK_TIMEOUT                      200000
#define MAX_NUMBER_OF_SEARCH_RESULTS            10

/* Default IP address */
#define DEFAULT_IP_ADDRESS_0                    0xA8C0
#define DEFAULT_IP_ADDRESS_1                    0x2088
#define DEFAULT_SUBNET_MASK_0                   0xFFFF
#define DEFAULT_SUBNET_MASK_1                   0x00FF
#define DEFAULT_GATEWAY_IP_ADDRESS_0            0xA8C0
#define DEFAULT_GATEWAY_IP_ADDRESS_1            0xFE88
#define DEFAULT_PING_IP_ADDRESS_0               0xA8C0
#define DEFAULT_PING_IP_ADDRESS_1               0x2888

/* States */
#define INITIALISING_STATE                      0x0000
#define IDLE_STATE                              0x0001
#define CONNECTABLE_STATE                       0x0002
#define CONNECTED_STATE                         0x0003
#define SELECT_SERVER_STATE                     0x0004
#define ADDRESS_CONFIGURATION_STATE             0x0005
#define DIRECT_CONNECTION                       0x0006
#define ENTER_PASS_KEY_STATE                    0x0007

/* Configuration sub-states */
#define DHCP_RESPONSE_PENDING_SUB_STATE         0x0000
#define IP_ADDRESS_PENDING_SUB_STATE            0x0001
#define SUBNET_MASK_PENDING_SUB_STATE           0x0002
#define GATEWAY_IP_ADDRESS_PENDING_SUB_STATE    0x0003

/* Device discovery sub-states */
#define INQUIRY_SUB_STATE                       0x0000
#define CANCEL_INQUIRY_SUB_STATE                0x0001
#define PANU_SEARCH_SUB_STATE                   0x0002
#define GN_SEARCH_SUB_STATE                     0x0003
#define NAP_SEARCH_SUB_STATE                    0x0004
#define CONNECT_PENDING_SUB_STATE               0x0005
#define CANCEL_PENDING_SUB_STATE                0x0006
#define AWAITINGSELECTION_SUB_STATE             0x0007

/* Connected sub-states */
#define CONNECTING_SUB_STATE                    0x0000
#define DISCONNECTING_SUB_STATE                 0x0001
#define CONNECTED_SUB_STATE                     0x0002
#define PING_ADDRESS_PENDING_SUB_STATE          0x0003
#define PING_PENDING_SUB_STATE                  0x0004
#define DHCP_PENDING_SUB_STATE                  0x0005

extern BD_ADDR_T defGlobalBdAddr;

typedef struct
{
    CsrUint8 state;
    CsrUint8 subState;
    CsrUint8 prevSubState;
    void    *message;
    CsrUint16 id;
    CsrUint16 macAddress[3];
    CsrUint16 ipAddress[2];
    CsrUint16 subnetMask[2];
    CsrUint16 gatewayIpAddress[2];
    CsrBool useDhcp;
    CsrUint16 pingIpAddress[2];
    CsrUint8 currentKeyboardStringIndex;
    CsrUint8 keyboardStringBuffer[MAX_KEYBOARD_STRING_SIZE];
    CsrUint8 selectedDeviceIndex;
    CsrUint16 numInList;
    CsrBtDeviceAddr deviceList[MAX_NUMBER_OF_SEARCH_RESULTS];
    CsrBtDeviceAddr defaultDevAddr;
    ScAppInstData_t scInstData;
} DemoAppInstanceData;

void handleKeybPrim(DemoAppInstanceData * instanceData, CsrUint8 theKey);
int _kbhit( void );
int _getch( void );

static void showUsage(DemoAppInstanceData * instanceData)
{
    CsrBtDeviceAddr zeroAddr;
    CsrBtBdAddrZero(&zeroAddr);

    switch (instanceData->state)
    {
        case INITIALISING_STATE:
            printf("\nPan Demo:\n\n");
            printf("Hit <ESC> to quit program!\n\n");
            printf("Initialising MAC address, please, wait...\n");
            break;

        case IDLE_STATE:
            printf("\nIdle state\n\n");
            printf("Options:\n\n");
            printf("A for address configuration\n");
            printf("P to activate PANU server\n");
            printf("G to activate GN server\n");
            printf("N to activate NAP server\n");
            printf("S to search for servers\n");
            printf("U to configure Security Controller\n");

            if (!CsrBtBdAddrEq(&instanceData->defaultDevAddr, &zeroAddr) )
            {
                printf("D to establish connection to: [%04X:%02X:%06X]\n",
                       instanceData->defaultDevAddr.nap,
                       instanceData->defaultDevAddr.uap,
                       instanceData->defaultDevAddr.lap);
            }
            break;

        case CONNECTABLE_STATE:
            printf("\nConnectable server state\n\n");
            printf("Options:\n\n");
            printf("C to cancel server mode\n\n");
            break;

        case CONNECTED_STATE:
            if (instanceData->subState == CONNECTING_SUB_STATE)
            {
                system_cls();
                printf("\nConnecting directly to: %04X:%02X:%06X, press C to cancel\n",
                       instanceData->defaultDevAddr.nap,
                       instanceData->defaultDevAddr.uap,
                       instanceData->defaultDevAddr.lap);
            }
            else
            {
                printf("\nConnected state\n\n");
                printf("Options:\n\n");
                printf("P to PING a node\n");
                printf("D to disconnect\n\n");
            }
            break;

        case SELECT_SERVER_STATE:
            printf("\nSelect server state\n\n");
            printf("Options:\n\n");
            printf("C to cancel server search\n\n");
            printf("Press a number from the list below to select a server\n");
            break;

        case ADDRESS_CONFIGURATION_STATE:
            printf("\nAddress configuration state\n\n");
            printf("Options:\n\n");
            printf("C to cancel address configuration\n\n");
            printf("Use DHCP [Y], or Specify IP Addresses [N]? ");
            break;

        case DIRECT_CONNECTION:
            system_cls();
            printf("\nDirect connection state\n");
            printf("Target device: [%04X:%02X:%06X]\n",
                   instanceData->defaultDevAddr.nap,
                   instanceData->defaultDevAddr.uap,
                   instanceData->defaultDevAddr.lap);
            printf("Options:\n\n");
            printf("P)  Connect to PANU\n");
            printf("N)  Connect to NAP\n");
            printf("G)  Connect to GN\n");
            printf("C)  Cancel direct connection\n");
            break;

        default:
            printf("Unknown state: %d\n", instanceData->state);
            break;

    }
}

void startInquiry(void)
{
    CsrBtSdSearchReqSend(TESTQUEUE,
                  CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING, /* searchConfiguration */
                  6000, /* RSSI buffer timeout */
                  CSR_BT_INFINITE_TIME, /* Search timeout */
                  CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE, /* RSSI threshold */
                  CSR_BT_NETWORKING_MAJOR_SERVICE_MASK, /* Class of Device */
                  CSR_BT_NETWORKING_MAJOR_SERVICE_MASK, /* Class of Device mask */
                  CSR_BT_SD_ACCESS_CODE_GIAC, /* Access code */
                  0, /* Filter length */
                  NULL); /* Filter */
}

void stopInquiry(void)
{
    CsrBtSdCancelSearchReqSend(TESTQUEUE);
}

void KeyMessageService(CsrUint16 notUsed,void *data)
{
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

static void panScAppSetFnFini(void *pFiniData)
{
    DemoAppInstanceData *instData = (DemoAppInstanceData *) pFiniData;
    scAppRestoreState(&instData->prevSubState, &instData->state);
    system_cls();
    showUsage(instData);
}

static void panScAppSetFnInit(void *pInitData)
{
    DemoAppInstanceData *instData = (DemoAppInstanceData *) pInitData;
    scAppSaveState(&instData->prevSubState, &instData->state, ENTER_PASS_KEY_STATE);
}

static void* panScAppSetFnGetMessage(void *pRecvMsgPData)
{
    DemoAppInstanceData *instData = (DemoAppInstanceData *) pRecvMsgPData;
    void *result                  = instData->message;
    instData->message             = NULL;
    return result;
}

void CsrBtAppInit(void **gash)
{
    DemoAppInstanceData * instanceData;
    CsrBtDeviceAddr zeroAddr;
    instanceData = (DemoAppInstanceData *) CsrPmemAlloc(sizeof(DemoAppInstanceData));
    *gash = (void *) instanceData;

    CsrBtBdAddrZero(&zeroAddr);
    CsrBtBdAddrZero(&(instanceData->defaultDevAddr));
    instanceData->state = INITIALISING_STATE;
    instanceData->useDhcp = FALSE;
    instanceData->ipAddress[0] = DEFAULT_IP_ADDRESS_0;
    instanceData->ipAddress[1] = DEFAULT_IP_ADDRESS_1;
    instanceData->subnetMask[0] = DEFAULT_SUBNET_MASK_0;
    instanceData->subnetMask[1] = DEFAULT_SUBNET_MASK_1;
    instanceData->gatewayIpAddress[0] = DEFAULT_GATEWAY_IP_ADDRESS_0;
    instanceData->gatewayIpAddress[1] = DEFAULT_GATEWAY_IP_ADDRESS_1;
    instanceData->pingIpAddress[0] = DEFAULT_PING_IP_ADDRESS_0;
    instanceData->pingIpAddress[1] = DEFAULT_PING_IP_ADDRESS_1;

    if (!CsrBtBdAddrEq(&defGlobalBdAddr, &zeroAddr) )
    {
        CsrBtBdAddrCopy( &(instanceData->defaultDevAddr), &defGlobalBdAddr );
        instanceData->selectedDeviceIndex = 0;
        instanceData->numInList= 0;
    }

    showUsage(instanceData);

    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, KeyMessageService, 0, NULL);

    CsrBtBslRegisterReqSend(TESTQUEUE, CSR_BT_IP_IFACEQUEUE);

    CsrBtSdSearchConfigReqSend(TESTQUEUE, CSR_BT_SD_SEARCH_USE_STANDARD , 6000,
                    MAX_NUMBER_OF_SEARCH_RESULTS);

    CsrBtCmReadLocalBdAddrReqSend(TESTQUEUE);

    /* Security App Configuration */
    CsrBtScActivateReqSend(TESTQUEUE);
    scAppInitInstanceData(&instanceData->scInstData);

    scAppSetFnFini(&instanceData->scInstData, panScAppSetFnFini, instanceData);
    scAppSetFnInit(&instanceData->scInstData, panScAppSetFnInit, instanceData);
    scAppSetSecLevelSend(&instanceData->scInstData, scAppSendProfileSecurityLevelReq, &instanceData->scInstData);

    scAppSetFnGetMessage(&instanceData->scInstData, panScAppSetFnGetMessage, instanceData);

    scAppSetSecLevelPrim(&instanceData->scInstData,
                         0,     /* Index 0. Single profile */
                         "BSL", CSR_BT_BSL_PRIM,
                         CSR_BT_BSL_IFACEQUEUE,
                         CSR_BT_BSL_SECURITY_IN_REQ, CSR_BT_BSL_SECURITY_OUT_REQ);
}

#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    CsrUint16    eventClass=0;
    void        *msg=NULL;
    DemoAppInstanceData * instanceData;

    instanceData = (DemoAppInstanceData *) *gash;

    scAppDeinitInstanceData(&instanceData->scInstData);

    while ( CsrSchedMessageGet(&eventClass , &msg) )
    {
        if (eventClass == CSR_BT_BSL_PRIM)
        {
            CsrPrim    *primType;

            primType = (CsrPrim *) msg;
            switch (*primType )
            {
                case CSR_BT_BSL_PRIM:
                    CsrBtBslFreeUpstreamMessageContents(eventClass, msg);
                    break;
                case CSR_BT_SC_PRIM:
                    CsrBtScFreeUpstreamMessageContents(eventClass, msg);
                    break;
                case CSR_BT_SD_PRIM:
                    CsrBtSdFreeUpstreamMessageContents(eventClass, msg);
                    break;
                case CSR_BT_CM_PRIM:
                    CsrBtCmFreeUpstreamMessageContents(eventClass, msg);
                    break;
                default:
                    break;
            }
        }
    }
    CsrPmemFree(instanceData);
}
#endif

void CsrBtAppHandler(void **gash)
{
    DemoAppInstanceData * instanceData;
    CsrUint16    eventType=0;
    void        *prim=NULL;

    instanceData = (DemoAppInstanceData *) *gash;

    /*    get a message from the demoapplication message queue. The message is returned in prim
          and the event type in eventType                                                        */
    CsrSchedMessageGet(&eventType, &prim);
    instanceData->message = prim;

    switch (eventType)
    {
        case CSR_BT_BSL_PRIM:
            {
                CsrUint16 *primType;

                primType = prim;
                switch (*primType)
                {
                    case CSR_BT_BSL_SECURITY_IN_CFM:
                    case CSR_BT_BSL_SECURITY_OUT_CFM:
                        {
                            handleScSecLevelPrim(&instanceData->scInstData);
                            break;
                        }

                    case CSR_BT_BSL_CONNECT_IND:
                        {
                            CsrBtBslConnectInd * connectInd;

                            connectInd = (CsrBtBslConnectInd *) prim;

                            if((connectInd->resultCode == CSR_BT_RESULT_CODE_BSL_SUCCESS) &&
                               (connectInd->resultSupplier == CSR_BT_SUPPLIER_BSL))
                            {
                                instanceData->id = connectInd->id;

                                if (instanceData->subState == DISCONNECTING_SUB_STATE)
                                {
                                    CsrBtBslDisconnectReq * disconnectReq;

                                    disconnectReq = (CsrBtBslDisconnectReq *) CsrPmemAlloc(sizeof(CsrBtBslDisconnectReq));
                                    disconnectReq->type = CSR_BT_BSL_DISCONNECT_REQ;
                                    disconnectReq->id = instanceData->id;

                                    CsrSchedMessagePut(CSR_BT_BSL_IFACEQUEUE, CSR_BT_BSL_PRIM, disconnectReq);
                                }
                                else
                                {
                                    instanceData->state = CONNECTED_STATE;
                                    printf("Connected!\n");

                                    if (instanceData->useDhcp)
                                    {
                                        CsrBtDhcpActivateClientReq * dhcpActivateReq;

                                        printf("Performing DHCP procedure...\n");

                                        dhcpActivateReq = (CsrBtDhcpActivateClientReq *) CsrPmemAlloc(sizeof(CsrBtDhcpActivateClientReq));
                                        dhcpActivateReq->type = CSR_BT_DHCP_ACTIVATE_CLIENT_REQ;
                                        dhcpActivateReq->applicationQueue = TESTQUEUE;
                                        dhcpActivateReq->ethernetAddress[0] = instanceData->macAddress[0];
                                        dhcpActivateReq->ethernetAddress[1] = instanceData->macAddress[1];
                                        dhcpActivateReq->ethernetAddress[2] = instanceData->macAddress[2];

                                        CsrSchedMessagePut(CSR_BT_DHCP_IFACEQUEUE, CSR_BT_DHCP_PRIM, dhcpActivateReq);

                                        instanceData->subState = DHCP_PENDING_SUB_STATE;
                                    }
                                    else
                                    {
                                        CsrBtIpAddressReq * addressReq;

                                        addressReq = (CsrBtIpAddressReq *) CsrPmemAlloc(sizeof(CsrBtIpAddressReq));
                                        addressReq->type = CSR_BT_IP_ADDRESS_REQ;
                                        addressReq->ipAddress[0] = instanceData->ipAddress[0];
                                        addressReq->ipAddress[1] = instanceData->ipAddress[1];
                                        addressReq->subnetMask[0] = instanceData->subnetMask[0];
                                        addressReq->subnetMask[1] = instanceData->subnetMask[1];
                                        addressReq->gatewayIpAddress[0] = instanceData->gatewayIpAddress[0];
                                        addressReq->gatewayIpAddress[1] = instanceData->gatewayIpAddress[1];

                                        CsrSchedMessagePut(CSR_BT_IP_IFACEQUEUE, CSR_BT_IP_PRIM, addressReq);

                                        printf("Addresses manual configured.\n");

                                        instanceData->subState = CONNECTED_SUB_STATE;
                                    }
                                    showUsage(instanceData);
                                }
                            }
                            else
                            {
                                printf("Error: %d",connectInd->resultCode);
                                instanceData->state = IDLE_STATE;
                                showUsage(instanceData);
                            }
                            break;
                        }

                    case CSR_BT_BSL_ACTIVATE_CFM:
                        {
                            CsrBtBslActivateCfm * activateCfm;

                            activateCfm = (CsrBtBslActivateCfm *) prim;

                            if((activateCfm->resultCode == CSR_BT_RESULT_CODE_BSL_SUCCESS) &&
                               (activateCfm->resultSupplier == CSR_BT_SUPPLIER_BSL))
                            {
                                /* do nothing */
                            }
                            else
                            {
                                instanceData->state = IDLE_STATE;
                                showUsage(instanceData);
                            }
                            break;
                        }

                    case CSR_BT_BSL_DISCONNECT_IND:
                            {
                                if (instanceData->subState != DISCONNECTING_SUB_STATE)
                                {
                                    CsrBtBslDisconnectInd * disconnectInd;
                                    CsrBtBslDisconnectRes * disconnectRes;

                                    disconnectInd = (CsrBtBslDisconnectInd *) prim;
                                    disconnectRes = (CsrBtBslDisconnectRes *) CsrPmemAlloc(sizeof(CsrBtBslDisconnectRes));
                                    disconnectRes->type = CSR_BT_BSL_DISCONNECT_RES;
                                    disconnectRes->id = disconnectInd->id;
                                    CsrSchedMessagePut(CSR_BT_BSL_IFACEQUEUE, CSR_BT_BSL_PRIM, disconnectRes);
                                }
                                instanceData->state = IDLE_STATE;
                                showUsage(instanceData);
                                break;
                            }
                    case CSR_BT_BSL_STATUS_IND:
                        {
                            CsrBtBslStatusInd * statusInd;

                            statusInd = (CsrBtBslStatusInd *) prim;
                            printf("CSR_BT_BSL_STATUS_IND: status=%02X, event=%02X\n", statusInd->status, statusInd->event);
                            break;
                        }

                    default:
                        {
                            CsrBtBslFreeUpstreamMessageContents(eventType, prim);
                            printf("CSR_BT_BSL_PRIM default event: %04X?\n", *primType);
                        }
                }

                break;
            }

        case CSR_BT_SC_PRIM:
            {
                handleScPrim(&instanceData->scInstData);
                break;
            }

        case CSR_BT_CM_PRIM:
            {
                CsrUint16 *primType;

                primType = (CsrUint16 *) prim;
                switch (*primType)
                {
                    case CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM:
                        {
                            CsrBtCmReadLocalBdAddrCfm * readBdAddrCfm;
                            CsrBtIpMacAddressReq * macAddressReq;
                            ETHER_ADDR thisAddress;

                            readBdAddrCfm = (CsrBtCmReadLocalBdAddrCfm *) prim;
                            thisAddress = CsrBtConvBdAddr2EtherBig(&readBdAddrCfm->deviceAddr);

                            macAddressReq = (CsrBtIpMacAddressReq *) CsrPmemAlloc(sizeof(CsrBtIpMacAddressReq));
                            macAddressReq->type = CSR_BT_IP_MAC_ADDRESS_REQ;
                            macAddressReq->macAddress[0] = thisAddress.w[0];
                            macAddressReq->macAddress[1] = thisAddress.w[1];
                            macAddressReq->macAddress[2] = thisAddress.w[2];
                            CsrSchedMessagePut(CSR_BT_IP_IFACEQUEUE, CSR_BT_IP_PRIM, macAddressReq);

                            instanceData->macAddress[0] = thisAddress.w[0];
                            instanceData->macAddress[1] = thisAddress.w[1];
                            instanceData->macAddress[2] = thisAddress.w[2];

                            instanceData->state = IDLE_STATE;
                            showUsage(instanceData);
                            break;
                        }
                    default:
                        CsrBtCmFreeUpstreamMessageContents(eventType, prim);
                        break;
                }
                break;
            }

        case CSR_BT_SD_PRIM:
            {
                CsrPrim *primType;

                primType = (CsrPrim *) prim;
                switch (*primType)
                {
                    case CSR_BT_SD_SEARCH_RESULT_IND:
                    {
                        CsrBtSdSearchResultInd * discoverResultInd;
                        discoverResultInd = (CsrBtSdSearchResultInd *) prim;

                        if ((instanceData->state == SELECT_SERVER_STATE) && (instanceData->subState == INQUIRY_SUB_STATE))
                        {
                            CsrUint16 nameLen;
                            CsrUint8 *name;

                            instanceData->deviceList[instanceData->numInList] = discoverResultInd->deviceAddr;

                            printf("\n%u: ", instanceData->numInList);
                            nameLen = CsrBtSdDecodeFriendlyName(discoverResultInd->info, discoverResultInd->infoLen, &name);
                            if (nameLen > 0)
                            {
                                printf("%s\n   ", name);
                                CsrPmemFree(name);
                            }
                            printf("%04X:%02X:%06X\n   ",
                                   discoverResultInd->deviceAddr.nap,
                                   discoverResultInd->deviceAddr.uap,
                                   discoverResultInd->deviceAddr.lap);
                            printf("%s\n",CodDecodeDeviceClass(discoverResultInd->deviceClass));

                            instanceData->numInList++;
                        }
                        CsrPmemFree(discoverResultInd->info);
                        break;
                    }

                    case CSR_BT_SD_CLOSE_SEARCH_IND:
                        if ((instanceData->state == SELECT_SERVER_STATE) && (instanceData->subState == CANCEL_PENDING_SUB_STATE))
                        {
                            instanceData->state = IDLE_STATE;
                            showUsage(instanceData);
                        }
                        else if ((instanceData->state == SELECT_SERVER_STATE) && (instanceData->subState == INQUIRY_SUB_STATE))
                        {
                            instanceData->subState = AWAITINGSELECTION_SUB_STATE;
                        }
                        break;
                    default:
                        CsrBtSdFreeUpstreamMessageContents(eventType, prim);
                        break;
                    }
                break;
            }

        case KEY_MESSAGE:
            {
                CsrBtKeyPress_t *key;
                key = (CsrBtKeyPress_t*)prim;

                handleKeybPrim(instanceData, key->key);
                break;
            }

        case CSR_BT_ICMP_PRIM:
            {
                CsrUint16 *primType;

                primType = prim;
                if (*primType == CSR_BT_ICMP_PING_CFM)
                {
                    CsrBtIcmpPingCfm * pingCfm;

                    pingCfm = (CsrBtIcmpPingCfm *) prim;
                    if (pingCfm->result == CSR_BT_ICMP_RESULT_SUCCESS)
                    {
                        printf("PING success (%u bytes in %u usecs.)!\n\n",
                               PING_DATA_SIZE,
                               pingCfm->responseTime);
                    }
                    else
                    {
                        printf("PING failed - result code=%04X\n\n", pingCfm->result);
                    }
                }

                instanceData->subState = CONNECTED_SUB_STATE;
                break;
            }

        case CSR_BT_DHCP_PRIM:
            {
                CsrUint16 *primType;

                primType = prim;
                if (*primType == CSR_BT_DHCP_ACTIVATE_CLIENT_CFM)
                {
                    CsrBtDhcpActivateClientCfm * dhcpActivateCfm;

                    dhcpActivateCfm = (CsrBtDhcpActivateClientCfm *) prim;
                    if ((dhcpActivateCfm->result == CSR_BT_DHCP_RESULT_CONFIGURED)||(dhcpActivateCfm->result == CSR_BT_DHCP_RESULT_ALREADY_ACTIVE))
                    {
                        printf("Addresses configured to %u.%u.%u.%u.\n\n",
                               CSR_BT_LAN_WORD(dhcpActivateCfm->ipAddress[0]) >> 8,
                               CSR_BT_LAN_WORD(dhcpActivateCfm->ipAddress[0]) & 0x00FF, CSR_BT_LAN_WORD(dhcpActivateCfm->ipAddress[1]) >> 8,
                               CSR_BT_LAN_WORD(dhcpActivateCfm->ipAddress[1]) & 0x00FF);
                        instanceData->subState = CONNECTED_SUB_STATE;
                    }
                    else
                    {
                        CsrBtBslDisconnectReq * disconnectReq;

                        printf("DHCP addresses configuration failed (%04X), disconnecting...\n",
                               dhcpActivateCfm->result);

                        disconnectReq = (CsrBtBslDisconnectReq *) CsrPmemAlloc(sizeof(CsrBtBslDisconnectReq));
                        disconnectReq->type = CSR_BT_BSL_DISCONNECT_REQ;
                        disconnectReq->id = instanceData->id;

                        CsrSchedMessagePut(CSR_BT_BSL_IFACEQUEUE, CSR_BT_BSL_PRIM, disconnectReq);

                        instanceData->subState = DISCONNECTING_SUB_STATE;
                    }
                    }
                break;
            }

        default:
            {
                /*    unexpected primitive received    */
                printf("####### default in testhandler %x,\n", eventType);
            }
    }    /*    end switch(eventType)    */

    /*    free the received signal again. If the signal for some reason must be stored in the application
          the pointer (prim) must be set to NULL in order not to free it here                                */
    CsrPmemFree(instanceData->message);
        }

void handleKeybPrim(DemoAppInstanceData * instanceData, CsrUint8 theKey)
{
    CsrBtDeviceAddr zeroAddr;
    CsrBtBdAddrZero(&zeroAddr);
    if (theKey == ESCAPE_KEY)
    {
        printf("\nUser exit...\n");
#ifdef ENABLE_SHUTDOWN
        CsrSchedStop();
#else
        exit(0);
#endif
        return;
    }

    switch (instanceData->state)
    {
        case INITIALISING_STATE:
            break;

        case ENTER_PASS_KEY_STATE:
            handleScKeybPrim(&instanceData->scInstData);
            break;

        case IDLE_STATE:
            switch (theKey)
            {
                case 'a':
                case 'A':
                    instanceData->state = ADDRESS_CONFIGURATION_STATE;
                    instanceData->subState = DHCP_RESPONSE_PENDING_SUB_STATE;
                    instanceData->currentKeyboardStringIndex = 0;
                    showUsage(instanceData);
                    break;

                case 'p':
                case 'P':
                    if (!instanceData->useDhcp)
                    {
                        CsrBtIpAddressReq * addressReq;

                        addressReq = (CsrBtIpAddressReq *) CsrPmemAlloc(sizeof(CsrBtIpAddressReq));
                        addressReq->type = CSR_BT_IP_ADDRESS_REQ;
                        addressReq->ipAddress[0] = instanceData->ipAddress[0];
                        addressReq->ipAddress[1] = instanceData->ipAddress[1];
                        addressReq->subnetMask[0] = instanceData->subnetMask[0];
                        addressReq->subnetMask[1] = instanceData->subnetMask[1];
                        addressReq->gatewayIpAddress[0] = instanceData->gatewayIpAddress[0];
                        addressReq->gatewayIpAddress[1] = instanceData->gatewayIpAddress[1];

                        CsrSchedMessagePut(CSR_BT_IP_IFACEQUEUE, CSR_BT_IP_PRIM, addressReq);
                    }

                    CsrBtBslActivateReqSend(FALSE, CSR_BT_BSL_PANU_ROLE ,CSR_BT_BSL_PANU_ROLE);

                    instanceData->state = CONNECTABLE_STATE;
                    instanceData->subState = CONNECT_PENDING_SUB_STATE;
                    showUsage(instanceData);

                    printf("PANU server activated, awaiting connection...\n");
                    break;

                case 'g':
                case 'G':
                    if (!instanceData->useDhcp)
                    {
                        CsrBtIpAddressReq * addressReq;

                        addressReq = (CsrBtIpAddressReq *) CsrPmemAlloc(sizeof(CsrBtIpAddressReq));
                        addressReq->type = CSR_BT_IP_ADDRESS_REQ;
                        addressReq->ipAddress[0] = instanceData->ipAddress[0];
                        addressReq->ipAddress[1] = instanceData->ipAddress[1];
                        addressReq->subnetMask[0] = instanceData->subnetMask[0];
                        addressReq->subnetMask[1] = instanceData->subnetMask[1];
                        addressReq->gatewayIpAddress[0] = instanceData->gatewayIpAddress[0];
                        addressReq->gatewayIpAddress[1] = instanceData->gatewayIpAddress[1];

                        CsrSchedMessagePut(CSR_BT_IP_IFACEQUEUE, CSR_BT_IP_PRIM, addressReq);
                    }

                    CsrBtBslActivateReqSend(FALSE, CSR_BT_BSL_GN_ROLE ,CSR_BT_BSL_PANU_ROLE);

                    instanceData->state = CONNECTABLE_STATE;
                    instanceData->subState = CONNECT_PENDING_SUB_STATE;
                    showUsage(instanceData);

                    printf("GN server activated, awaiting connection...\n");
                    break;

                case 'n':
                case 'N':
                    if (!instanceData->useDhcp)
                    {
                        CsrBtIpAddressReq * addressReq;

                        addressReq = (CsrBtIpAddressReq *) CsrPmemAlloc(sizeof(CsrBtIpAddressReq));
                        addressReq->type = CSR_BT_IP_ADDRESS_REQ;
                        addressReq->ipAddress[0] = instanceData->ipAddress[0];
                        addressReq->ipAddress[1] = instanceData->ipAddress[1];
                        addressReq->subnetMask[0] = instanceData->subnetMask[0];
                        addressReq->subnetMask[1] = instanceData->subnetMask[1];
                        addressReq->gatewayIpAddress[0] = instanceData->gatewayIpAddress[0];
                        addressReq->gatewayIpAddress[1] = instanceData->gatewayIpAddress[1];

                        CsrSchedMessagePut(CSR_BT_IP_IFACEQUEUE, CSR_BT_IP_PRIM, addressReq);
                    }

                    CsrBtBslActivateReqSend(FALSE, CSR_BT_BSL_NAP_ROLE ,CSR_BT_BSL_PANU_ROLE);

                    instanceData->state = CONNECTABLE_STATE;
                    instanceData->subState = CONNECT_PENDING_SUB_STATE;
                    showUsage(instanceData);

                    printf("NAP server activated, awaiting connection...\n");
                    break;

                case 's':
                case 'S':
                    instanceData->numInList = 0;
                    instanceData->selectedDeviceIndex = 0;
                    startInquiry();
                    instanceData->state = SELECT_SERVER_STATE;
                    instanceData->subState = INQUIRY_SUB_STATE;
                    showUsage(instanceData);
                    break;
                    /*    insert correct address and role to connect without search    */
                case 'd':
                case 'D':
                    if (!CsrBtBdAddrEq(&instanceData->defaultDevAddr, &zeroAddr) )
                    {
                        instanceData->state = DIRECT_CONNECTION;
                        showUsage(instanceData);
                    }
                    break;

                case 'u':
                case 'U':
                    handleScKeybPrim(&instanceData->scInstData);
                    break;
            }
            break;

        case ADDRESS_CONFIGURATION_STATE:
            switch (theKey)
            {
                case 'c':
                case 'C':
                    instanceData->state = IDLE_STATE;
                    showUsage(instanceData);
                    break;

                case 'y':
                case 'Y':
                    instanceData->useDhcp = TRUE;
                    instanceData->state = IDLE_STATE;
                    showUsage(instanceData);
                    break;

                case 'n':
                case 'N':
                    printf("N\n\n");
                    printf("Current IP address: %u.%u.%u.%u\n",
                           CSR_BT_LAN_WORD(instanceData->ipAddress[0]) >> 8, CSR_BT_LAN_WORD(instanceData->ipAddress[0]) & 0x00FF,
                           CSR_BT_LAN_WORD(instanceData->ipAddress[1]) >> 8, CSR_BT_LAN_WORD(instanceData->ipAddress[1]) & 0x00FF);

                    printf("Enter IP address (empty to use current): ");
                    instanceData->subState = IP_ADDRESS_PENDING_SUB_STATE;
                    instanceData->currentKeyboardStringIndex = 0;
                    break;

                case RETURN_KEY:
                    switch (instanceData->subState)
                    {
                        case DHCP_RESPONSE_PENDING_SUB_STATE:
                            break;

                        case IP_ADDRESS_PENDING_SUB_STATE:
                            if (instanceData->currentKeyboardStringIndex)
                            {
                                int addr1;
                                int addr2;
                                int addr3;
                                int addr4;

                                instanceData->keyboardStringBuffer[instanceData->currentKeyboardStringIndex] = '\0';
                                if (sscanf((char *) instanceData->keyboardStringBuffer, "%u.%u.%u.%u", &addr1, &addr2, &addr3, &addr4) == 4)
                                {
                                    instanceData->ipAddress[0] = CSR_BT_LAN_WORD((((CsrUint16) addr1) << 8) | ((CsrUint16) addr2));
                                    instanceData->ipAddress[1] = CSR_BT_LAN_WORD((((CsrUint16) addr3) << 8) | ((CsrUint16) addr4));

                                    printf("\n\n");
                                    printf("Current subnet mask: %u.%u.%u.%u\n",
                                           CSR_BT_LAN_WORD(instanceData->subnetMask[0]) >> 8,
                                           CSR_BT_LAN_WORD(instanceData->subnetMask[0]) & 0x00FF,
                                           CSR_BT_LAN_WORD(instanceData->subnetMask[1]) >> 8, CSR_BT_LAN_WORD(instanceData->subnetMask[1]) & 0x00FF);

                                    printf("Enter subnet mask (empty to use current): ");
                                    instanceData->subState = SUBNET_MASK_PENDING_SUB_STATE;
                                    instanceData->currentKeyboardStringIndex = 0;
                                }
                                else
                                {
                                    printf("\nInvalid address, please, try again...\n\n");
                                    printf("Current IP address: %u.%u.%u.%u\n",
                                           CSR_BT_LAN_WORD(instanceData->ipAddress[0]) >> 8,
                                           CSR_BT_LAN_WORD(instanceData->ipAddress[0]) & 0x00FF,
                                           CSR_BT_LAN_WORD(instanceData->ipAddress[1]) >> 8,
                                           CSR_BT_LAN_WORD(instanceData->ipAddress[1]) & 0x00FF);
                                    printf("Enter IP address (empty to use current): ");
                                    instanceData->subState = IP_ADDRESS_PENDING_SUB_STATE;
                                    instanceData->currentKeyboardStringIndex = 0;
                                }
                            }
                            else
                            {
                                printf("\n\n");
                                printf("Current subnet mask: %u.%u.%u.%u\n",
                                       CSR_BT_LAN_WORD(instanceData->subnetMask[0]) >> 8,
                                       CSR_BT_LAN_WORD(instanceData->subnetMask[0]) & 0x00FF,
                                       CSR_BT_LAN_WORD(instanceData->subnetMask[1]) >> 8,
                                       CSR_BT_LAN_WORD(instanceData->subnetMask[1]) & 0x00FF);

                                printf("Enter subnet mask (empty to use current): ");
                                instanceData->subState = SUBNET_MASK_PENDING_SUB_STATE;
                                instanceData->currentKeyboardStringIndex = 0;
                            }
                            break;

                        case SUBNET_MASK_PENDING_SUB_STATE:
                            if (instanceData->currentKeyboardStringIndex)
                            {
                                int addr1;
                                int addr2;
                                int addr3;
                                int addr4;

                                instanceData->keyboardStringBuffer[instanceData->currentKeyboardStringIndex] = '\0';
                                if (sscanf((char *) instanceData->keyboardStringBuffer, "%u.%u.%u.%u", &addr1, &addr2, &addr3, &addr4) == 4)
                                {
                                    instanceData->subnetMask[0] = CSR_BT_LAN_WORD((((CsrUint16) addr1) << 8) | ((CsrUint16) addr2));
                                    instanceData->subnetMask[1] = CSR_BT_LAN_WORD((((CsrUint16) addr3) << 8) | ((CsrUint16) addr4));

                                    printf("\n\n");
                                    printf("Current gateway IP address: %u.%u.%u.%u\n",
                                           CSR_BT_LAN_WORD(instanceData->gatewayIpAddress[0]) >> 8,
                                           CSR_BT_LAN_WORD(instanceData->gatewayIpAddress[0]) & 0x00FF,
                                           CSR_BT_LAN_WORD(instanceData->gatewayIpAddress[1]) >> 8,
                                           CSR_BT_LAN_WORD(instanceData->gatewayIpAddress[1]) & 0x00FF);

                                    printf("Enter gateway IP address (empty to use current): ");
                                    instanceData->subState = GATEWAY_IP_ADDRESS_PENDING_SUB_STATE;
                                    instanceData->currentKeyboardStringIndex = 0;
                                }
                                else
                                {
                                    printf("\nInvalid address, please, try again...\n\n");
                                    printf("Current subnet mask: %u.%u.%u.%u\n",
                                           CSR_BT_LAN_WORD(instanceData->subnetMask[0]) >> 8,
                                           CSR_BT_LAN_WORD(instanceData->subnetMask[0]) & 0x00FF,
                                           CSR_BT_LAN_WORD(instanceData->subnetMask[1]) >> 8,
                                           CSR_BT_LAN_WORD(instanceData->subnetMask[1]) & 0x00FF);

                                    printf("Enter subnet mask (empty to use current): ");
                                    instanceData->subState = SUBNET_MASK_PENDING_SUB_STATE;
                                    instanceData->currentKeyboardStringIndex = 0;
                                }
                            }
                            else
                            {
                                printf("\n\n");
                                printf("Current gateway IP address: %u.%u.%u.%u\n",
                                       CSR_BT_LAN_WORD(instanceData->gatewayIpAddress[0]) >> 8,
                                       CSR_BT_LAN_WORD(instanceData->gatewayIpAddress[0]) & 0x00FF,
                                       CSR_BT_LAN_WORD(instanceData->gatewayIpAddress[1]) >> 8,
                                       CSR_BT_LAN_WORD(instanceData->gatewayIpAddress[1]) & 0x00FF);

                                printf("Enter gateway IP address (empty to use current): ");
                                instanceData->subState = GATEWAY_IP_ADDRESS_PENDING_SUB_STATE;
                                instanceData->currentKeyboardStringIndex = 0;
                            }
                            break;

                        case GATEWAY_IP_ADDRESS_PENDING_SUB_STATE:
                            if (instanceData->currentKeyboardStringIndex)
                            {
                                int addr1;
                                int addr2;
                                int addr3;
                                int addr4;

                                instanceData->keyboardStringBuffer[instanceData->currentKeyboardStringIndex] = '\0';
                                if (sscanf((char *) instanceData->keyboardStringBuffer, "%u.%u.%u.%u", &addr1, &addr2, &addr3, &addr4) == 4)
                                {
                                    instanceData->gatewayIpAddress[0] = CSR_BT_LAN_WORD((((CsrUint16) addr1) << 8) | ((CsrUint16) addr2));
                                    instanceData->gatewayIpAddress[1] = CSR_BT_LAN_WORD((((CsrUint16) addr3) << 8) | ((CsrUint16) addr4));
                                    instanceData->state = IDLE_STATE;
                                    showUsage(instanceData);
                                }
                                else
                                {
                                    printf("\nInvalid address, please, try again...\n\n");
                                    printf("Current gateway IP address: %u.%u.%u.%u\n",
                                           CSR_BT_LAN_WORD(instanceData->gatewayIpAddress[0]) >> 8,
                                           CSR_BT_LAN_WORD(instanceData->gatewayIpAddress[0]) & 0x00FF,
                                           CSR_BT_LAN_WORD(instanceData->gatewayIpAddress[1]) >> 8,
                                           CSR_BT_LAN_WORD(instanceData->gatewayIpAddress[1]) & 0x00FF);

                                    printf("Enter gateway IP address (empty to use current): ");
                                    instanceData->subState = GATEWAY_IP_ADDRESS_PENDING_SUB_STATE;
                                    instanceData->currentKeyboardStringIndex = 0;
                                }
                            }
                            else
                            {
                                instanceData->state = IDLE_STATE;
                                showUsage(instanceData);
                            }
                            break;
                    }
                    break;

                case BACKSPACE_KEY:
                    if (instanceData->currentKeyboardStringIndex)
                    {
                        printf("\b \b");
                        instanceData->currentKeyboardStringIndex--;
                    }
                    break;

                default:
                    if (instanceData->currentKeyboardStringIndex < MAX_KEYBOARD_STRING_SIZE - 1)
                    {
                        if ((theKey >= '0') && (theKey <= '9'))
                        {
                            printf("%c", theKey);
                            instanceData->keyboardStringBuffer[instanceData->currentKeyboardStringIndex++] = theKey;
                        }
                        else if (theKey == '.')
                        {
                            printf("%c", theKey);
                            instanceData->keyboardStringBuffer[instanceData->currentKeyboardStringIndex++] = theKey;
                        }
                    }
                    break;
            }
            break;

        case CONNECTED_STATE:
                switch (theKey)
                {
                    case 'c':
                    case 'C':
                    {
                        if (instanceData->subState == CONNECTING_SUB_STATE)
                        {
                            CsrBtBslCancelConnectReqSend(&instanceData->defaultDevAddr);
                        }
                        break;
                    }
                    case 'd':
                    case 'D':
                        if (instanceData->subState != DISCONNECTING_SUB_STATE)
                        {
                            if (instanceData->subState != CONNECTING_SUB_STATE)
                            {
                                CsrBtBslDisconnectReq * disconnectReq;
                                CsrBtDhcpDeactivateClientInd * deactiInd;

                                disconnectReq = (CsrBtBslDisconnectReq *) CsrPmemAlloc(sizeof(CsrBtBslDisconnectReq));
                                disconnectReq->type = CSR_BT_BSL_DISCONNECT_REQ;
                                disconnectReq->id = instanceData->id;

                                CsrSchedMessagePut(CSR_BT_BSL_IFACEQUEUE, CSR_BT_BSL_PRIM, disconnectReq);

                                deactiInd = (CsrBtDhcpDeactivateClientInd *) CsrPmemAlloc(sizeof(CsrBtDhcpDeactivateClientInd));
                                deactiInd->type = CSR_BT_DHCP_DEACTIVATE_CLIENT_REQ;
                                CsrSchedMessagePut(CSR_BT_DHCP_IFACEQUEUE, CSR_BT_DHCP_PRIM, deactiInd);

                                instanceData->subState = DISCONNECTING_SUB_STATE;

                            }
                            else
                            {
                                instanceData->subState = DISCONNECTING_SUB_STATE;
                            }
                        }
                        break;

                    case 'p':
                    case 'P':
                        if (instanceData->subState == CONNECTED_SUB_STATE)
                        {
                            printf("Current PING IP address: %u.%u.%u.%u\n",
                                   CSR_BT_LAN_WORD(instanceData->pingIpAddress[0]) >> 8,
                                   CSR_BT_LAN_WORD(instanceData->pingIpAddress[0]) & 0x00FF,
                                   CSR_BT_LAN_WORD(instanceData->pingIpAddress[1]) >> 8,
                                   CSR_BT_LAN_WORD(instanceData->pingIpAddress[1]) & 0x00FF);

                            printf("Enter PING IP address (empty to use current): ");
                            instanceData->currentKeyboardStringIndex = 0;
                            instanceData->subState = PING_ADDRESS_PENDING_SUB_STATE;
                        }
                        break;

                    case RETURN_KEY:
                        if (instanceData->subState == PING_ADDRESS_PENDING_SUB_STATE)
                        {
                            if (instanceData->currentKeyboardStringIndex)
                            {
                                int addr1;
                                int addr2;
                                int addr3;
                                int addr4;

                                instanceData->keyboardStringBuffer[instanceData->currentKeyboardStringIndex] = '\0';
                                if (sscanf((char *) instanceData->keyboardStringBuffer, "%u.%u.%u.%u", &addr1, &addr2, &addr3, &addr4) == 4)
                                {
                                    CsrBtIcmpPingReq * pingReq;

                                    instanceData->pingIpAddress[0] = CSR_BT_LAN_WORD((((CsrUint16) addr1) << 8) | ((CsrUint16) addr2));
                                    instanceData->pingIpAddress[1] = CSR_BT_LAN_WORD((((CsrUint16) addr3) << 8) | ((CsrUint16) addr4));

                                    printf("\n\n");
                                    printf("Pinging %u.%u.%u.%u, please, wait...\n",
                                           CSR_BT_LAN_WORD(instanceData->pingIpAddress[0]) >> 8,
                                           CSR_BT_LAN_WORD(instanceData->pingIpAddress[0]) & 0x00FF,
                                           CSR_BT_LAN_WORD(instanceData->pingIpAddress[1]) >> 8,
                                           CSR_BT_LAN_WORD(instanceData->pingIpAddress[1]) & 0x00FF);

                                    pingReq = (CsrBtIcmpPingReq *) CsrPmemAlloc(sizeof(CsrBtIcmpPingReq));
                                    pingReq->type = CSR_BT_ICMP_PING_REQ;
                                    pingReq->applicationQueue = TESTQUEUE;
                                    pingReq->destinationAddress[0] = instanceData->pingIpAddress[0];
                                    pingReq->destinationAddress[1] = instanceData->pingIpAddress[1];
                                    pingReq->dataLength = PING_DATA_SIZE;
                                    CsrSchedMessagePut(CSR_BT_ICMP_IFACEQUEUE, CSR_BT_ICMP_PRIM, pingReq);

                                    instanceData->subState = PING_PENDING_SUB_STATE;
                                }
                                else
                                {
                                    printf("\nInvalid address, please, try again...\n\n");
                                    printf("Current PING IP address: %u.%u.%u.%u\n",
                                           CSR_BT_LAN_WORD(instanceData->pingIpAddress[0]) >> 8,
                                           CSR_BT_LAN_WORD(instanceData->pingIpAddress[0]) & 0x00FF,
                                           CSR_BT_LAN_WORD(instanceData->pingIpAddress[1]) >> 8,
                                           CSR_BT_LAN_WORD(instanceData->pingIpAddress[1]) & 0x00FF);

                                    printf("Enter PING IP address (empty to use current): ");
                                    instanceData->currentKeyboardStringIndex = 0;
                                    instanceData->subState = PING_ADDRESS_PENDING_SUB_STATE;
                                }
                            }
                            else
                            {
                                CsrBtIcmpPingReq * pingReq;

                                printf("\n\n");
                                printf("Pinging %u.%u.%u.%u, please, wait...\n",
                                       CSR_BT_LAN_WORD(instanceData->pingIpAddress[0]) >> 8,
                                       CSR_BT_LAN_WORD(instanceData->pingIpAddress[0]) & 0x00FF,
                                       CSR_BT_LAN_WORD(instanceData->pingIpAddress[1]) >> 8,
                                       CSR_BT_LAN_WORD(instanceData->pingIpAddress[1]) & 0x00FF);

                                pingReq = (CsrBtIcmpPingReq *) CsrPmemAlloc(sizeof(CsrBtIcmpPingReq));
                                pingReq->type = CSR_BT_ICMP_PING_REQ;
                                pingReq->applicationQueue = TESTQUEUE;
                                pingReq->destinationAddress[0] = instanceData->pingIpAddress[0];
                                pingReq->destinationAddress[1] = instanceData->pingIpAddress[1];
                                pingReq->dataLength = PING_DATA_SIZE;
                                CsrSchedMessagePut(CSR_BT_ICMP_IFACEQUEUE, CSR_BT_ICMP_PRIM, pingReq);

                                instanceData->subState = PING_PENDING_SUB_STATE;
                            }
                        }
                        break;

                    case BACKSPACE_KEY:
                        if (instanceData->currentKeyboardStringIndex)
                        {
                            printf("\b \b");
                            instanceData->currentKeyboardStringIndex--;
                        }
                        break;

                    default:
                        if (instanceData->currentKeyboardStringIndex < MAX_KEYBOARD_STRING_SIZE - 1)
                        {
                            if ((theKey >= '0') && (theKey <= '9'))
                            {
                                printf("%c", theKey);
                                instanceData->keyboardStringBuffer[instanceData->currentKeyboardStringIndex++] = theKey;
                            }
                            else if (theKey == '.')
                            {
                                printf("%c", theKey);
                                instanceData->keyboardStringBuffer[instanceData->currentKeyboardStringIndex++] = theKey;
                            }
                        }
                        break;
                }
            break;

        case CONNECTABLE_STATE:
                switch (theKey)
                {
                    case 'c':
                    case 'C':
                        CsrBtBslActivateReqSend(FALSE, CSR_BT_BSL_NO_ROLE ,CSR_BT_BSL_PANU_ROLE);
                        instanceData->state = IDLE_STATE;
                        showUsage(instanceData);
                        break;
                }
            break;

        case SELECT_SERVER_STATE:
                switch (theKey)
                {
                    case 'c':
                    case 'C':
                        if (instanceData->subState == AWAITINGSELECTION_SUB_STATE)
                        {
                            instanceData->state = IDLE_STATE;
                            showUsage(instanceData);
                        }
                        else if (instanceData->subState == INQUIRY_SUB_STATE)
                        {
                            CsrBtSdCancelSearchReqSend(TESTQUEUE);
                            printf("Cancelling, please, wait...\n");
                            instanceData->subState = CANCEL_PENDING_SUB_STATE;
                        }
                        break;
                    default:
                        if ((theKey >= '0') && (theKey <= '9'))
                        {
                            if ((theKey - '0') < instanceData->numInList)
                            {

                                instanceData->selectedDeviceIndex = theKey - '0';
                                printf("\nDevice %d selected\n",instanceData->selectedDeviceIndex);
                                CsrBtBdAddrCopy( &(instanceData->defaultDevAddr), &(instanceData->deviceList[theKey - '0']));
                                if (instanceData->subState == AWAITINGSELECTION_SUB_STATE)
                                {
                                    instanceData->state = IDLE_STATE;
                                    showUsage(instanceData);
                                }
                                else if (instanceData->subState == INQUIRY_SUB_STATE)
                                {
                                    CsrBtSdCancelSearchReqSend(TESTQUEUE);
                                    instanceData->subState = CANCEL_PENDING_SUB_STATE;
                                }
                                scAppSetBondingAddr(&instanceData->scInstData,
                                                    instanceData->deviceList[instanceData->selectedDeviceIndex]);
                            }
                        }
                        break;
            }
            break;

        case DIRECT_CONNECTION:
            switch (theKey)
            {
                case 'p':
                case 'P':
                    CsrBtBslConnectReqSend(instanceData->defaultDevAddr, CSR_BT_BSL_PANU_ROLE, CSR_BT_BSL_PANU_ROLE);
                    instanceData->state = CONNECTED_STATE;
                    instanceData->subState = CONNECTING_SUB_STATE;
                    showUsage(instanceData);
                    break;

                case 'n':
                case 'N':
                    CsrBtBslConnectReqSend(instanceData->defaultDevAddr, CSR_BT_BSL_PANU_ROLE, CSR_BT_BSL_NAP_ROLE);
                    instanceData->state = CONNECTED_STATE;
                    instanceData->subState = CONNECTING_SUB_STATE;
                    showUsage(instanceData);
                    break;

                case 'g':
                case 'G':
                    CsrBtBslConnectReqSend(instanceData->defaultDevAddr, CSR_BT_BSL_PANU_ROLE, CSR_BT_BSL_GN_ROLE);
                    instanceData->state = CONNECTED_STATE;
                    instanceData->subState = CONNECTING_SUB_STATE;
                    showUsage(instanceData);
                    break;

                case 'c':
                case 'C':
                    instanceData->state = IDLE_STATE;
                    showUsage(instanceData);
                    break;

                default:
                    /* discard the input */
                    break;
            }
            break;
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
