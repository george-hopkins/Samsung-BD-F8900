/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csr_bt_dunc_prim.h"
#include "csr_pmem.h"
#include "csr_bt_at_dte.h"
#include "csr_bt_dunc_lib.h"
#include "csr_bt_ppp_lib.h"
#include "csr_bt_ppp_prim.h"
#include "csr_bt_ipcp.h"
#include "csr_bt_ip_prim.h"
#include "csr_bt_dhcp_prim.h"
#include "csr_bt_icmp_prim.h"
#include "csr_bt_platform.h"

#define MAX_LOGIN_NAME_LENGTH        64
#define MAX_PASSWORD_LENGTH          64

#define PING_DATA_SIZE               32

typedef enum
{
    ok,
    none
} ExpectedResponseType;

typedef enum
{
    idle,
    dialNumberReq,
    ipReq,
    userReq,
    passwordReq
} DuncKeyHandlerStatesType;

typedef enum
{
    notConnectedState,
    commandState,
    onlineCommandState,
    onlineDataState,
    onlineDataNcpConnectingState,
    onlineDataIpConnectedState
} DuncDataDemoStatesType;

typedef struct
{
    void *                      msgP;
    CsrBtNcpInstanceData             * ncpInstData;
    DuncDataDemoStatesType        state;
    DuncKeyHandlerStatesType    keyHandlerState;
    ExpectedResponseType        expectedResponse;
    CsrBool                        doHangup;
    CsrBool                        userEntered;
    CsrBool                        pwdEntered;
    CsrBool                        setupDone;
    CsrUint16                    currentKeyboardIndex;
    CsrUint16                    maxMsgSize;
    CsrUint16                    macAddress[3];
    CsrUint8                        connectEstablishmentNumber;
    CsrUint8                        dialNumLen;
    CsrBool                        dialNumberEntered;
    CsrInt8                        dialNumber[64];
    CsrBool                        dialing;
    CsrUint8                        pingLen;
    CsrBool                        pingEntered;
    CsrUint8                        ping[64];
    CsrUint16                    pingAddressNetworkOrder[2];
    CsrUint8                        pppAuthenticationInProgress;
    CsrBtPppAuthenticateRes            * authenticateRes;
    CsrBool                      callIncoming;
    CsrBool                      connectPpp;
} DuncDataDemoInstanceDataType;


/* forward function declarations */
static void handleKeybPrim(DuncDataDemoInstanceDataType *instData);


static void playIntro(DuncDataDemoInstanceDataType *instData)
{
    switch(instData->state)
    {
        case notConnectedState:
            {
                /* dont print anything - this is done by the control part of the app */
                break;
            }
        case commandState:
            {
                printf("------------------------------------------------------------\n");
                printf("\n         Dialup Networking Profile Client Demo\n");
                printf("\n------------------------------------------------------------\n");
                printf("Hit <ESC> to quit program!\n");
                printf("\n");
                printf("BT State:    CONNECTED\n");
                printf("Modem State: COMMAND STATE\n");
                printf("PPP State:   DISCONNECTED\n");
                printf("\n");
                printf("Options: \n\n");
                printf("    4) Disconnect connection to the GW\n");
                if (instData->dialNumberEntered == TRUE)
                {
                    printf("    5) Enter phone number to dial for the connection (%s)\n", instData->dialNumber);
                }
                else
                {
                    printf("    5) Enter phone number to dial for the connection (none entered yet)\n");
                }

                if (instData->userEntered == TRUE)
                {
                    printf("    6) Enter username for login (%s)\n", instData->authenticateRes->loginName);
                }
                else
                {
                    printf("    6) Enter username for login (none entered yet)\n");
                }

                if (instData->pwdEntered == TRUE)
                {
                    printf("    7) Enter password for login (***********)\n");
                }
                else
                {
                    printf("    7) Enter password for login (none entered yet)\n");
                }

                if (instData->userEntered && instData->pwdEntered && instData->dialNumberEntered)
                {
                    printf("    8) Start dialing (%s)\n", instData->dialNumber);
                }
                else
                {
                    printf("    8) Start dialing (still some options needs to be entered)\n");
                }
                printf("\n------------------------------------------------------------\n");
                break;
            }
        case onlineDataState:
            {
                printf("------------------------------------------------------------\n");
                printf("\n         Dialup Networking Profile Client Demo\n");
                printf("\n------------------------------------------------------------\n");
                printf("Hit <ESC> to quit program!\n");
                printf("\n");
                printf("State:       CONNECTED\n");
                printf("Modem State: ONLINE DATA STATE\n");
                if (instData->connectPpp == TRUE)
                {
                    printf("PPP State:   CONNECTING\n");
                }
                else
                {
                    printf("PPP State:   NOT CONNECTED\n");
                }
                printf("\n");
                printf("Options: \n\n");
                printf("    4) Disconnect connection to the GW\n");
                printf("    9) Send (non-PPP) test data\n");
                printf("    h) Hang-up call\n");
                printf("\n------------------------------------------------------------\n");
                break;
            }
        case onlineDataNcpConnectingState:
            {
                printf("------------------------------------------------------------\n");
                printf("\n         Dialup Networking Profile Client Demo\n");
                printf("\n------------------------------------------------------------\n");
                printf("Hit <ESC> to quit program!\n");
                printf("\n");
                printf("State:       CONNECTED\n");
                printf("Modem State: ONLINE DATA STATE\n");
                printf("PPP State:   NCP CONNECTING\n");
                printf("\n");
                printf("Options: \n\n");
                printf("    4) Disconnect connection to the GW\n");
                printf("\n------------------------------------------------------------\n");
                break;
            }
        case onlineDataIpConnectedState:
            {
                printf("------------------------------------------------------------\n");
                printf("\n         Dialup Networking Profile Client Demo\n");
                printf("\n------------------------------------------------------------\n");
                printf("Hit <ESC> to quit program!\n");
                printf("\n");
                printf("State:       CONNECTED\n");
                printf("Modem State: ONLINE DATA STATE\n");
                printf("PPP State:   IP CONNECTED (IpAddress: %u.%u.%u.%u, PeerAddress: %u.%u.%u.%u)\n",instData->ncpInstData->assignedIpAddress[0], instData->ncpInstData->assignedIpAddress[1], instData->ncpInstData->assignedIpAddress[2], instData->ncpInstData->assignedIpAddress[3],
                       instData->ncpInstData->peerIpAddress[0], instData->ncpInstData->peerIpAddress[1], instData->ncpInstData->peerIpAddress[2], instData->ncpInstData->peerIpAddress[3]);
                printf("\n");
                printf("Options: \n\n");
                printf("    4) Disconnect connection to the GW\n");
                if (instData->pingEntered == FALSE)
                {
                    printf("    i) Enter IP to ping (none entered yet)\n");
                    printf("    p) Ping (none entered yet)\n");
                }
                else
                {
                    printf("    i) Enter IP to ping (%s)\n", instData->ping);
                    printf("    p) Ping (%s)\n", instData->ping);
                }
                printf("    h) Hang-up call\n");


                printf("\n------------------------------------------------------------\n");
                break;
            }
        default:
            {
            }
    }
}


/**************************************************************************************************
 *
 *    init function called by the scheduler upon initialisation. This function is used to initialise
 *    the demo application by initialising its instance data, printing the options available for the
 *   user and waiting for the user to make a choice
 *
 **************************************************************************************************/
void CsrBtExtraTaskInit(void **gash)
{
    DuncDataDemoInstanceDataType *instData;
    *gash    = (void *) CsrPmemAlloc(sizeof(DuncDataDemoInstanceDataType));
    instData = (DuncDataDemoInstanceDataType *) *gash;
    instData->ncpInstData = (CsrBtNcpInstanceData *) CsrPmemAlloc(sizeof(CsrBtNcpInstanceData));
    /* initialise instance data */
    instData->ncpInstData->demoAppInstanceData  = instData;
    instData->ncpInstData->ncpTimer             = 0;
    instData->connectEstablishmentNumber        = 0;
    instData->dialNumberEntered                 = FALSE;
    instData->state                             = notConnectedState;
    instData->keyHandlerState                   = idle;
    instData->dialNumLen                        = 0;
    instData->pingLen                           = 0;
    instData->pingEntered                       = FALSE;
    instData->setupDone                         = FALSE;
    instData->doHangup                          = FALSE;
    instData->dialing                           = FALSE;
    instData->expectedResponse                  = ok;
    instData->pppAuthenticationInProgress       = 0;
    instData->currentKeyboardIndex              = 0;
    instData->userEntered                       = FALSE;
    instData->pwdEntered                        = FALSE;
    instData->authenticateRes                   = NULL;
    instData->callIncoming                      = FALSE;
    instData->connectPpp                        = TRUE;
    /* register the ipcp protocol */
    PppRegisterNetworkProtocolReqSend(CSR_BT_EXTRA_IFACEQUEUE, CSR_BT_IPCP_NCP_PROTOCOL, CSR_BT_IP_IFACEQUEUE, CSR_BT_IPCP_IP_PROTOCOL);
}

static void pppPrimHandler(DuncDataDemoInstanceDataType *instData)
{
    CsrBtPppPrim *primType;
    primType = (CsrBtPppPrim *) instData->msgP;

    switch(*primType)
    {
        case CSR_BT_PPP_CONNECT_CFM:
            {
                CsrBtPppConnectCfm * connectCfm;
                connectCfm = (CsrBtPppConnectCfm *) instData->msgP;

                if (connectCfm->resultCode      == CSR_BT_RESULT_CODE_BSL_SUCCESS &&
                    connectCfm->resultSupplier  == CSR_BT_SUPPLIER_BSL)
                {
                    CsrBtStartIpCp(instData->ncpInstData);
                    instData->state = onlineDataNcpConnectingState;
                }
                else
                {
                    system(CLS);
                    printf("CSR_BT_PPP_CONNECT_CFM with FAIL, wrong password or login name. Restart demo and try again\n");
                }

                break;
            }

        case CSR_BT_PPP_REGISTER_NETWORK_PROTOCOL_CFM:
            {
                break;
            }

        case CSR_BT_PPP_DATA_IND:
            {
                break;
            }

        case CSR_BT_PPP_NCP_IND:
            {
                CsrBtHandleNcpInd(instData->ncpInstData);
                break;
            }

        case CSR_BT_PPP_DOWN_IND:
            {
                break;
            }

        case CSR_BT_PPP_UP_IND:
            {
                break;
            }

        case CSR_BT_PPP_DISCONNECT_IND:
            {
                CsrBtNcpHandleDisconnectInd(instData->ncpInstData);
                break;
            }

        case CSR_BT_PPP_AUTHENTICATE_IND:
            {
                CsrSchedMessagePut(CSR_BT_PPP_IFACEQUEUE, CSR_BT_PPP_PRIM, instData->authenticateRes);
                break;
            }

        default:
            {
                printf("Unknown PPP prim\n");
            }
    }
}

static void duncPrimHandler(DuncDataDemoInstanceDataType *instData)
{
    CsrBtDuncPrim *primType;
    primType = (CsrBtDuncPrim *)instData->msgP;
    switch(*primType)
    {
        case CSR_BT_DUNC_DATA_IND:
            {
                CsrBtDuncDataInd *duncPrim;
                duncPrim = (CsrBtDuncDataInd *)instData->msgP;
                CsrBtDuncDataResSend(CSR_BT_DUNC_IFACEQUEUE);

                switch(instData->state)
                {
                    case notConnectedState:
                        {
                            /* this should not be possible */
                            printf("Error: Received data in not connected state\n");
                            CsrPmemFree(duncPrim->data);
                            break;
                        }
                    case commandState:
                        {
                            /* must be an AT command response - assumes always being ok response */
                            if (instData->setupDone != TRUE)
                            {
                                /* we are still performing setup - look for an "OK" and the send next AT setup command */
                                /* we have to NULL terminate the string before working on it */
                                CsrInt8 * tmpString = (CsrInt8 *) CsrPmemAlloc(duncPrim->dataLength +1);
                                CsrMemCpy(tmpString, duncPrim->data, duncPrim->dataLength);
                                tmpString[duncPrim->dataLength] = '\0';
                                if (strstr((char *) tmpString, "OK\0") != NULL)
                                {
                                    if( ATSetup(instData->connectEstablishmentNumber) == TRUE )
                                    {
                                        instData->setupDone = TRUE;
                                    }
                                    instData->connectEstablishmentNumber++;
                                }
                                else if (strstr((char *) tmpString, "RING\0") != NULL)
                                {
                                    printf("\nRING command received. Press A to accept call or R to reject call\n");
                                    instData->callIncoming = TRUE;
                                    instData->connectPpp = FALSE;
                                }
                                else
                                {
                                }
                                CsrPmemFree(tmpString);
                            }
                            else
                            {
                                if(instData->dialing == FALSE && instData->connectPpp == TRUE)
                                {
                                    /* setup is done, now dial */
                                    ATDial(instData->dialNumber);
                                    instData->dialing = TRUE;
                                }
                                else if (instData->callIncoming == TRUE)
                                {
                                    CsrInt8 * tmpString = (CsrInt8 *) CsrPmemAlloc(duncPrim->dataLength +1);
                                    CsrMemCpy(tmpString, duncPrim->data, duncPrim->dataLength);
                                    tmpString[duncPrim->dataLength] = '\0';
                                    if (strstr((char *) tmpString, "ATA\0") != NULL)
                                    {
                                        /* ATA command is being is sent (incoming call is being accepted) */
                                        instData->callIncoming = FALSE;
                                    }
                                    else if (strstr((char *) tmpString, "ATH\0") != NULL)
                                    {
                                        /* ATH command is being is sent (incoming call is being rejected) */
                                        instData->callIncoming = FALSE;
                                    }
                                    else
                                    {
                                        printf("Unknown command (%s) was received during incoming call setup\n", tmpString);
                                    }
                                }
                                else
                                {
                                    /* we are trying to establish a connection - check for CONNECT response */
                                    /* we have to NULL terminate the string before working on it */
                                    CsrInt8 * tmpString = (CsrInt8 *) CsrPmemAlloc(duncPrim->dataLength +1);
                                    CsrMemCpy(tmpString, duncPrim->data, duncPrim->dataLength);
                                    tmpString[duncPrim->dataLength] = '\0';
                                    if (strstr((char *) tmpString, "CONNECT\0") != NULL)
                                    {
                                        CsrBtDuncStatusInd * prim;
                                        CsrBtDeviceAddr addr;
                                        addr.nap = 0x0000;
                                        addr.uap = 0x00;
                                        addr.lap = 0x000000;

                                        if (instData->connectPpp == TRUE)
                                        {
                                            /* send a PPP connect request to start the PPP layer, the addr parameter is not used in DUN demo only in LAP demo,
                                               therefore it is just zeroes */
                                            PppConnectReqSend(CSR_BT_EXTRA_IFACEQUEUE, addr);
                                            instData->expectedResponse = none;
                                        }
                                        /* we are connected - go to onlineData state */
                                        instData->state = onlineDataState;
                                        system(CLS);
                                        playIntro(instData);
                                        printf("\nReceived CONNECT response\n");

                                        /* send a "fake" CSR_BT_DUNC_STATUS_IND to the PPP informing that it can now use the connection */
                                        prim = (CsrBtDuncStatusInd *)CsrPmemAlloc(sizeof(CsrBtDuncStatusInd));
                                        prim->type = CSR_BT_DUNC_STATUS_IND;
                                        prim->connected = TRUE;
                                        prim->maxMsgSize = instData->maxMsgSize;
                                        CsrSchedMessagePut(CSR_BT_PPP_IFACEQUEUE, CSR_BT_DUNC_PRIM, prim);
                                    }
                                    else if(strstr((char *) tmpString, "OK\0") != NULL)
                                    {
                                        /* just ignore */
                                    }
                                    else
                                    {
                                        if (strstr((char *) tmpString, "NO CARRIER\0") != NULL)
                                        {
                                            printf("ERROR: NO CARRIER!\n");
                                        }
                                        else if (strstr((char *) tmpString, "BUSY\0") != NULL)
                                        {
                                            system(CLS);
                                            printf("ERROR: LINE IS BUSY!\n");
                                            playIntro(instData);
                                            instData->setupDone = FALSE;
                                            instData->connectEstablishmentNumber = 0;
                                            instData->dialing = FALSE;
                                        }
                                        else if (strstr((char *) tmpString, "NO DIALTONE\0") != NULL)
                                        {
                                            system(CLS);
                                            printf("ERROR: NO DIALTONE!\n");
                                            playIntro(instData);
                                            instData->setupDone = FALSE;
                                            instData->connectEstablishmentNumber = 0;
                                            instData->dialing = FALSE;
                                        }
                                        else
                                        {
                                            system(CLS);
                                            printf("ERROR: Could not connect, check cable and dial number and try again...!\n");
                                            playIntro(instData);
                                            instData->setupDone = FALSE;
                                            instData->connectEstablishmentNumber = 0;
                                            instData->dialing = FALSE;

                                        }
                                    }
                                    CsrPmemFree(tmpString);
                                }
                            }
                            CsrPmemFree(duncPrim->data);
                            break;
                        }
                    case onlineCommandState:
                        {
                            /* can be online commands or ATO for going into onlineDataState */
                            CsrPmemFree(duncPrim->data);
                            break;
                        }
                    case onlineDataState:
                        {
                            if (instData->connectPpp == TRUE)
                            {
                                switch(instData->expectedResponse)
                                {
                                    case none:
                                        {
                                            /* most likely data for PPP, but can also be +++AT */
                                            CsrBtDuncDataInd * dataInd;
                                            dataInd = (CsrBtDuncDataInd *) CsrPmemAlloc(sizeof(CsrBtDuncDataInd));
                                            CsrMemCpy((void *)dataInd, (void *)instData->msgP, sizeof(CsrBtDuncDataInd));
                                            CsrSchedMessagePut(CSR_BT_PPP_IFACEQUEUE, CSR_BT_DUNC_PRIM, dataInd);
                                            break;
                                        }
                                    default:
                                        printf("Unexpected response!!!\n");
                                }
                            }
                            else
                            {
                                CsrUint8 i = 0;
                                CsrBtDuncDataInd * dataInd;
                                dataInd = (CsrBtDuncDataInd *) CsrPmemAlloc(sizeof(CsrBtDuncDataInd));
                                CsrMemCpy((void *)dataInd, (void *)instData->msgP, sizeof(CsrBtDuncDataInd));
                                printf("\nData received: ");
                                for (i = 0; i < dataInd->dataLength; i++)
                                {
                                    printf("%c", (*((dataInd->data) + i)));
                                }
                            }
                            break;
                        }
                    case onlineDataNcpConnectingState:
                        {
                            CsrBtDuncDataInd * dataInd;
                            dataInd = (CsrBtDuncDataInd *) CsrPmemAlloc(sizeof(CsrBtDuncDataInd));
                            CsrMemCpy((void *)dataInd, (void *)instData->msgP, sizeof(CsrBtDuncDataInd));
                            CsrSchedMessagePut(CSR_BT_PPP_IFACEQUEUE, CSR_BT_DUNC_PRIM, dataInd);
                            break;
                        }
                    case onlineDataIpConnectedState:
                        {
                            if(instData->doHangup)
                            {
                                CsrInt8 * tmpString = (CsrInt8 *) CsrPmemAlloc(duncPrim->dataLength +1);
                                CsrMemCpy(tmpString, duncPrim->data, duncPrim->dataLength);
                                tmpString[duncPrim->dataLength] = '\0';
                                if (strstr((char *) tmpString, "OK\0") != NULL)
                                {
                                    CsrBtDuncDisconnectReqSend(CSR_BT_DUNC_IFACEQUEUE);
                                    instData->doHangup = FALSE;
                                }
                            }
                            else
                            {
                                CsrBtDuncDataInd * dataInd;
                                dataInd = (CsrBtDuncDataInd *) CsrPmemAlloc(sizeof(CsrBtDuncDataInd));
                                CsrMemCpy((void *)dataInd, (void *)instData->msgP, sizeof(CsrBtDuncDataInd));
                                CsrSchedMessagePut(CSR_BT_PPP_IFACEQUEUE, CSR_BT_DUNC_PRIM, dataInd);

                            }
                            break;
                        }
                    default:
                        {
                            /* this should never happen */
                            printf("Undefined state in data application part!!!\n");
                            CsrPmemFree(duncPrim->data);
                        }
                }
                break;
            }

        case CSR_BT_DUNC_DATA_CFM:
            {
                /* forward it to the PPP-layer */
                if((instData->state == onlineDataState) || (instData->state == onlineDataNcpConnectingState) || (instData->state == onlineDataIpConnectedState))
                {
                    CsrBtDuncDataCfm * prim;

                    prim = (CsrBtDuncDataCfm *)CsrPmemAlloc(sizeof(CsrBtDuncDataCfm));
                    prim->type = CSR_BT_DUNC_DATA_CFM;
                    CsrSchedMessagePut(CSR_BT_PPP_IFACEQUEUE, CSR_BT_DUNC_PRIM, prim);
                }
                break;
            }

        case CSR_BT_DUNC_CONTROL_IND:
            {
                /*printf("Received CSR_BT_DUNC_CONTROL_IND\n");*/
                break;
            }

        case CSR_BT_DUNC_PORTNEG_IND:
            {
                CsrBtDuncPortnegInd *prim;
                prim = (CsrBtDuncPortnegInd *)instData->msgP;

                /* accept port settings from remote device */
                CsrBtDuncPortnegResSend(prim->duncInstanceId, &(prim->portPar));
                break;
            }

        case CSR_BT_DUNC_STATUS_IND:
            {
                CsrBtDuncStatusInd *prim;
                prim = (CsrBtDuncStatusInd *)instData->msgP;
                if (prim->connected == TRUE)
                {
                    instData->state = commandState;
                    instData->maxMsgSize = prim->maxMsgSize;
                    playIntro(instData);
                }
                else if (prim->connected == FALSE)
                {
                    instData->dialNumberEntered  = FALSE;
                    instData->pingEntered = FALSE;
                    instData->pwdEntered = FALSE;
                    instData->userEntered = FALSE;
                    instData->connectEstablishmentNumber = 0;
                    instData->setupDone = FALSE;
                    instData->state = notConnectedState;
                }
                break;
            }

        case CSR_BT_DUNC_REGISTER_DATA_PATH_HANDLE_CFM:
            {
                /* just ignore */
                break;
            }
        default:
            {
                printf("default in data_app switch: 0x%04x\n", *primType);
                CsrBtDuncFreeUpstreamMessageContents(*primType, instData->msgP);
                break;
            }
    }
}

static void startPing(DuncDataDemoInstanceDataType *instData)
{
    CsrBtIcmpPingReq * pingReq;

    pingReq = (CsrBtIcmpPingReq *) CsrPmemAlloc(sizeof(CsrBtIcmpPingReq));
    pingReq->type = CSR_BT_ICMP_PING_REQ;
    pingReq->applicationQueue = CSR_BT_EXTRA_IFACEQUEUE;
    pingReq->destinationAddress[0] = instData->pingAddressNetworkOrder[0];
    pingReq->destinationAddress[1] = instData->pingAddressNetworkOrder[1];
    pingReq->dataLength = PING_DATA_SIZE;
    CsrSchedMessagePut(CSR_BT_ICMP_IFACEQUEUE, CSR_BT_ICMP_PRIM, pingReq);
}

static void icmpPrimHandler(DuncDataDemoInstanceDataType * instData)
{
    if (instData->state == onlineDataIpConnectedState)
    {
        CsrUint16 * primType;

        primType = instData->msgP;
        if (*primType == CSR_BT_ICMP_PING_CFM)
        {
            CsrBtIcmpPingCfm * pingCfm;

            pingCfm = (CsrBtIcmpPingCfm *) instData->msgP;

            if (pingCfm->result == CSR_BT_ICMP_RESULT_SUCCESS)
            {
                printf("PING success (%u bytes in %u usecs.)!\n\n", PING_DATA_SIZE, pingCfm->responseTime);
            }
            else if (pingCfm->result == CSR_BT_ICMP_RESULT_UNREACHABLE)
            {
                system(CLS);
                printf("PING failed - destination unreachable\n");
                playIntro(instData);
            }
            else
            {
                system(CLS);
                printf("PING failed - result code=%04X\n\n", pingCfm->result);
                playIntro(instData);
            }
        }
    }
}

/**************************************************************************************************
 *
 *    this is the demo application handler function. All primitives sent to the demo application will
 *    be received in here. The scheduler ensures that the function is activated when a signal is put
 *    on the demo application signal queue. The gash double pointer parameter is the instance data
 *    available for this task running in the scheduler
 *
 **************************************************************************************************/
void CsrBtExtraTaskHandler(void **gash)
{
    CsrUint16    eventType=0;
    void        *prim=NULL;
    DuncDataDemoInstanceDataType *instData;
    instData = (DuncDataDemoInstanceDataType *) *gash;

    /*    get a message from the data demo application message queue. The message is returned in prim
          and the event type in eventType */
    CsrSchedMessageGet(&eventType, &prim);
    instData->msgP = prim;
    instData->ncpInstData->receivedMessage = instData->msgP;
    /*    four event types must be handled. SC for bonding, CM for pairing and DUNCC for DUNC
          profile related messages */
    switch (eventType)
    {
        /* it was dunc primitive */
        case CSR_BT_DUNC_PRIM:
            {
                duncPrimHandler(instData);
                break;
            }

        case CSR_BT_PPP_PRIM:
            {
                pppPrimHandler(instData);
                break;
            }

        case CSR_BT_ICMP_PRIM:
            {
                icmpPrimHandler(instData);
                break;
            }
            /*It was a key pressed primitive*/
        case KEY_MESSAGE:
            {
                handleKeybPrim(instData);
                break;
            }

        default:
            {
                printf("####### Wrong Primitive type received\n");
                break;
            }
    }
    if (instData->msgP != NULL)
    {
        CsrPmemFree(instData->msgP);
        instData->msgP = NULL;
    }
}

#ifdef ENABLE_SHUTDOWN
void CsrBtExtraTaskDeinit(void **gash)
{
    CsrUint16 msgType=0;
    void *msgData=NULL;
    DuncDataDemoInstanceDataType * instanceData = *gash;
    /*    get a message from the demoapplication message queue. The message is returned in prim
          and the event type in eventType */
    while ( CsrSchedMessageGet(&msgType, &msgData) )
    {
        switch (msgType)
        {
            /*It was a dialup networking profile client primitive*/
            case CSR_BT_DUNC_PRIM:
                {
                    CsrBtDuncFreeUpstreamMessageContents(msgType, msgData);
                }
            case CSR_BT_PPP_PRIM:
                {
                    CsrBtPppPrim * primType = msgData;
                    switch(*primType)
                    {
                        case CSR_BT_PPP_DATA_IND:
                            {
                                CsrBtPppDataInd * dataInd = msgData;
                                CsrPmemFree(dataInd->payload);
                                break;
                            }
                        default:
                            {
                            }
                    }
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

static void pppAuthenticationKeyHandler(DuncDataDemoInstanceDataType *instData, CsrUint8 theChar)
{
    if (theChar == RETURN_KEY)
    {
        if (instData->keyHandlerState == userReq)
        {
            instData->authenticateRes->loginName[instData->currentKeyboardIndex] = '\0';
            instData->keyHandlerState = idle;
            instData->userEntered = TRUE;
            system(CLS);
            playIntro(instData);
        }
        else
        {
            instData->authenticateRes->password[instData->currentKeyboardIndex] = '\0';
            instData->keyHandlerState = idle;
            instData->pwdEntered = TRUE;
            system(CLS);
            playIntro(instData);
        }
    }
    else if (theChar == BACKSPACE_KEY)
    {
        if (instData->currentKeyboardIndex > 0)
        {
            instData->currentKeyboardIndex--;
            if (instData->keyHandlerState == userReq)
            {
                instData->authenticateRes->loginName[instData->currentKeyboardIndex] = '\0';
            }
            else
            {
                instData->authenticateRes->password[instData->currentKeyboardIndex] = '\0';
            }
            printf("%c %c", theChar, theChar);
        }
    }
    else if ((theChar >= 32) && (theChar < 127))    /* NOTICE: only ASCII support at the moment! */
    {
        if (instData->keyHandlerState == userReq)
        {
            if (instData->currentKeyboardIndex < MAX_LOGIN_NAME_LENGTH)
            {
                instData->authenticateRes->loginName[instData->currentKeyboardIndex++] = theChar;
                printf("%c", theChar);
            }
        }
        else
        {
            if (instData->currentKeyboardIndex < MAX_PASSWORD_LENGTH)
            {
                instData->authenticateRes->password[instData->currentKeyboardIndex++] = theChar;
                printf("*");
            }
        }
    }
    else
    {
        ;
    }
}

static void dialNumberHandler(DuncDataDemoInstanceDataType *instData, CsrUint8 theChar)
{
    if (theChar == RETURN_KEY)
    {
        if (instData->dialNumLen != 0)
        {
            instData->dialNumber[instData->dialNumLen] = '\0';
        }
        else
        {
            printf("\nYou must enter a number to dial!\n");
        }
        instData->keyHandlerState = idle;
        instData->dialNumberEntered = TRUE;
        system(CLS);
        playIntro(instData);
    }
    else if (theChar == BACKSPACE_KEY)
    {
        if (instData->dialNumLen > 0)
        {
            instData->dialNumLen--;
            printf("%c %c", theChar, theChar);
        }
    }
    else if ((theChar >= 32) && (theChar < 127))
    {
        if (instData->dialNumLen < 16)
        {
            instData->dialNumber[instData->dialNumLen++] = theChar;
            printf("%c", theChar);
        }
    }
    else
    {
        ;
    }

}

static void ipReqHandler(DuncDataDemoInstanceDataType *instData, CsrUint8 theChar)
{
    if (theChar == RETURN_KEY)
    {
        CsrUint16 addr1;
        CsrUint16 addr2;
        CsrUint16 addr3;
        CsrUint16 addr4;
        instData->ping[instData->pingLen] = '\0';

        if (sscanf((char *) instData->ping, "%hu.%hu.%hu.%hu", &addr1, &addr2, &addr3, &addr4) == 4)    /* NOTICE: only class C support at the moment! */
        {
            instData->pingAddressNetworkOrder[0] = CSR_BT_LAN_WORD((addr1 << 8) | addr2);
            instData->pingAddressNetworkOrder[1] = CSR_BT_LAN_WORD((addr3 << 8) | addr4);
            instData->pingEntered = TRUE;
        }
        else
        {
            printf("\nERROR: Invalid IP address, please, correct...\n\n");
            printf("Enter ping address: %s", instData->ping);
        }
        instData->keyHandlerState = idle;
        system(CLS);
        playIntro(instData);
    }
    else if (theChar == BACKSPACE_KEY)
    {
        if (instData->pingLen > 0)
        {
            instData->pingLen--;
            printf("%c %c", theChar, theChar);
        }
    }
    else if ((theChar >= 32) && (theChar < 127))
    {
        if (instData->pingLen < 16)
        {
            instData->ping[instData->pingLen++] = theChar;
            printf("%c", theChar);
        }
    }
    else
    {
        ;
    }
}

/**************************************************************************************************
 *
 *    a keyboard event is received.
 *    first check for selection of a found device during inquiry if state is inquiry. If not start
 *    proper action as selected.
 *    NOTE: This keyboard handler is common for both the data and control applications.
 **************************************************************************************************/
static void handleKeybPrim(DuncDataDemoInstanceDataType *instData)
{
    CsrBtKeyPress_t *key;
    key = (CsrBtKeyPress_t*)instData->msgP;

    if( instData->pppAuthenticationInProgress )
    {
        pppAuthenticationKeyHandler(instData, key->key);
        return;
    }

    switch(instData->keyHandlerState)
    {
        case idle:
            {
                /* do nothing */
                break;
            }
        case dialNumberReq:
            {
                dialNumberHandler(instData, key->key);
                return;

            }
        case ipReq:
            {
                ipReqHandler(instData, key->key);
                return;
            }
        case userReq:
            {
                pppAuthenticationKeyHandler(instData, key->key);
                return;
            }
        case passwordReq:
            {
                pppAuthenticationKeyHandler(instData, key->key);
                return;
            }
        default:
            {
            }
    }

    switch(key->key)
    {
        /* refresh screen */
        case ' ':
            {
                playIntro(instData);
                break;
            }
        /* enter dial number */
        case '5':
            {
                if (instData->state == commandState)
                {
                    instData->dialNumberEntered = FALSE;
                    instData->dialNumLen = 0;
                    instData->keyHandlerState = dialNumberReq;
                    printf("Enter the number to dial:\n");
                }
                else
                {
                    printf("Illegal selection! Please make another choice...\n");
                }
                break;
            }
            /* enter user name */
        case '6':
            {
                if (instData->state == commandState)
                {
                    if (instData->authenticateRes == NULL)
                    {
                        instData->authenticateRes = (CsrBtPppAuthenticateRes *) CsrPmemAlloc(sizeof(CsrBtPppAuthenticateRes));
                        instData->authenticateRes->type = CSR_BT_PPP_AUTHENTICATE_RES;
                        instData->authenticateRes->loginName = CsrPmemAlloc(MAX_LOGIN_NAME_LENGTH + 1);
                        instData->authenticateRes->loginName[0] = '\0';
                        instData->authenticateRes->password = CsrPmemAlloc(MAX_PASSWORD_LENGTH + 1);
                        instData->authenticateRes->password[0] = '\0';
                    }
                    instData->currentKeyboardIndex = 0;
                    instData->keyHandlerState = userReq;
                    printf("Enter login name:\n");
                }
                else
                {
                    printf("Illegal selection! Please make another choice...\n");
                }
                break;
            }
            /* enter password */
        case '7':
            {
                if (instData->state == commandState)
                {
                    if (instData->authenticateRes == NULL)
                    {
                        instData->authenticateRes = (CsrBtPppAuthenticateRes *) CsrPmemAlloc(sizeof(CsrBtPppAuthenticateRes));
                        instData->authenticateRes->type = CSR_BT_PPP_AUTHENTICATE_RES;
                        instData->authenticateRes->loginName = CsrPmemAlloc(MAX_LOGIN_NAME_LENGTH + 1);
                        instData->authenticateRes->loginName[0] = '\0';
                        instData->authenticateRes->password = CsrPmemAlloc(MAX_PASSWORD_LENGTH + 1);
                        instData->authenticateRes->password[0] = '\0';
                    }
                    instData->currentKeyboardIndex = 0;
                    instData->keyHandlerState = passwordReq;
                    printf("Enter password for login name:\n");
                }
                else
                {
                    printf("Illegal selection! Please make another choice...\n");
                }
                break;
            }
            /* dial */
        case '8':
            {
                if (instData->state == commandState)
                {
                    if(!(instData->dialNumberEntered && instData->pwdEntered && instData->userEntered))
                    {
                        printf("You have to enter dialnumber, user name and password before dialing...!\n");
                        break;
                    }
                    printf("Dialing...\n");
                    if( ATSetup(instData->connectEstablishmentNumber) == TRUE )
                    {
                        instData->setupDone = TRUE;
                    }
                    instData->connectEstablishmentNumber++;
                }
                else
                {
                    printf("Illegal selection! Please make another choice...\n");
                }
                break;
            }
            /* Send test data */
        case '9':
            {
                printf("Sending test data...\n");
                SendTestData();
                break;
            }
            /* accept incoming call */
        case 'a':
        case 'A':
            {
                if (instData->state == commandState && instData->callIncoming == TRUE)
                {
                    printf("Accepting call...\n");
                    ATAnswer();
                    instData->setupDone = TRUE;
                    instData->dialing = TRUE;
                }
                else
                {
                    printf("Illegal selection! Please make another choice...\n");
                }
                break;
            }
            /* enter IP to ping */
        case 'i':
        case 'I':
            {
                if (instData->state == onlineDataIpConnectedState)
                {
                    instData->pingEntered = FALSE;
                    instData->pingLen = 0;
                    instData->keyHandlerState = ipReq;
                    printf("Enter the IP address to ping:\n");
                }
                else
                {
                    printf("Illegal selection! Please make another choice...\n");
                }
                break;
            }
            /* start ping */
        case 'p':
        case 'P':
            {
                if (instData->state == onlineDataIpConnectedState)
                {
                    printf("Pinging...\n");
                    startPing(instData);
                }
                else
                {
                    printf("Illegal selection! Please make another choice...\n");
                }
                break;
            }
            /* hang up call */
        case 'h':
        case 'H':
            {
                if (instData->state == onlineDataIpConnectedState || instData->state == onlineDataState)
                {
                    instData->doHangup = TRUE;
                    ATHangup();
                    printf("Hanging up...\n");
                }
                else
                {
                    printf("Illegal selection! Please make another choice...\n");
                }
                break;
            }
        case 'r':
        case 'R':
            {
                if (instData->state == commandState && instData->callIncoming == TRUE)
                {
                    instData->doHangup = TRUE;
                    printf("Rejecting call...\n");
                    SendHookOn(0, NULL);
                }
                else
                {
                    printf("Illegal selection! Please make another choice...\n");
                }
                break;
            }
        default:
            {
                printf("####### default in keybPrim data handler %x,\n", key->key );
                /*    unexpected primitive received */
            }
    }
}

/* -------------------- IPCP call back functions -------------------- */

void StartIpStack(CsrBtNcpInstanceData * ncpInstData)
{
    if (ncpInstData->assignedIpAddress[0] || ncpInstData->assignedIpAddress[1] || ncpInstData->assignedIpAddress[2] || ncpInstData->assignedIpAddress[3])
    {
        CsrBtIpAddressReq * addressReq;

        addressReq = (CsrBtIpAddressReq *) CsrPmemAlloc(sizeof(CsrBtIpAddressReq));
        addressReq->type = CSR_BT_IP_ADDRESS_REQ;
        addressReq->ipAddress[0] = CSR_BT_LAN_WORD((((CsrUint16) ncpInstData->assignedIpAddress[0]) << 8) + ((CsrUint16) ncpInstData->assignedIpAddress[1]));
        addressReq->ipAddress[1] = CSR_BT_LAN_WORD((((CsrUint16) ncpInstData->assignedIpAddress[2]) << 8) + ((CsrUint16) ncpInstData->assignedIpAddress[3]));
        addressReq->gatewayIpAddress[0] = CSR_BT_LAN_WORD((((CsrUint16) ncpInstData->peerIpAddress[0]) << 8) + ((CsrUint16) ncpInstData->peerIpAddress[1]));
        addressReq->gatewayIpAddress[1] = CSR_BT_LAN_WORD((((CsrUint16) ncpInstData->peerIpAddress[2]) << 8) + ((CsrUint16) ncpInstData->peerIpAddress[3]));
        addressReq->subnetMask[0] = 0;    /* not used with PPP */
        addressReq->subnetMask[1] = 0;    /* not used with PPP */

        CsrSchedMessagePut(CSR_BT_IP_IFACEQUEUE, CSR_BT_IP_PRIM, addressReq);
        ((DuncDataDemoInstanceDataType *)ncpInstData->demoAppInstanceData)->state = onlineDataIpConnectedState;
        system(CLS);
        playIntro(ncpInstData->demoAppInstanceData);
    }
    else
    {
        CsrBtDhcpActivateClientReq * dhcpReq;

        dhcpReq = (CsrBtDhcpActivateClientReq *) CsrPmemAlloc(sizeof(CsrBtDhcpActivateClientReq));
        dhcpReq->type = CSR_BT_DHCP_ACTIVATE_CLIENT_REQ;
        dhcpReq->applicationQueue = TESTQUEUE;

        CsrSchedMessagePut(CSR_BT_DHCP_IFACEQUEUE, CSR_BT_DHCP_PRIM, dhcpReq);
    }
}

void CloseDownIpStack(CsrBtNcpInstanceData * instanceData)
{
    CsrBtIpShutdownReq * shutdownReq;

    shutdownReq = (CsrBtIpShutdownReq *) CsrPmemAlloc(sizeof(CsrBtIpShutdownReq));
    shutdownReq->type = CSR_BT_IP_SHUTDOWN_REQ;
    CsrSchedMessagePut(CSR_BT_IP_IFACEQUEUE, CSR_BT_IP_PRIM, shutdownReq);
}
