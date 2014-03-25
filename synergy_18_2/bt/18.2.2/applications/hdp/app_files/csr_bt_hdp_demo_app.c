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

#include "csr_bt_hdp_lib.h"
#include "csr_bt_hdp_prim.h"
#include "csr_bt_sc_prim.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_demoapp.h"
#include "csr_bt_sc_demo_app.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_hdp_demo_util.h"

static CsrCharString service[] = "Health";
static CsrCharString provider[] = "C S R";
static CsrCharString mdep_desc[] = "Mdep description";

CsrUint8 mdep1;
CsrUint8 mdep2;

#define AARQ_APDU  0xE2
#define AARE_APDU  0xE3
#define PRST_APDU  0xE7
#define RLRQ_APDU  0xE4
#define RLRE_APDU  0xE5

#define IEEE_DEV_CONFIG_ID_DEFAULT      0x00

#define IEEE_ACCEPTED_CONFIG            0x00
#define IEEE_ACCEPTED_UNKNOWN_CONFIG    0x03

#define REMOTE_OPERATION_RSP            0x02
#define REMOTE_OPERATION_INVOKE         0x01

#define GET                             0x03
#define CONFIRMED_EVENT_REPORT          0x01
    
/* Forward declarations for handler functions */
void hdpHandleKeybPrim(CsrUint8 theChar, hdpDemoGlobalInstanceData_t *hdpData);
void handleHdpPrim(hdpDemoGlobalInstanceData_t *hdpData);
void handleSdPrim(hdpDemoGlobalInstanceData_t *hdpData);
void handleCmPrim(hdpDemoGlobalInstanceData_t *hdpData);

int _kbhit( void );
int _getch( void );


static void hdpPrintBDAddress(CsrBtDeviceAddr *bd)
{
   printf("%04X:%02X:%06X\n",bd->nap,bd->uap,bd->lap);
}

/******************************************************************************
  KeyMessageService:
*****************************************************************************/
void hdpKeyMessageService(CsrUint16 m ,void *data)
{
    if (_kbhit())
    {
        CsrBtKeyPress_t *key;
        key = CsrPmemAlloc(sizeof(CsrBtKeyPress_t));
        key->type = KEY_MESSAGE;
        key->key =_getch();
        CsrSchedMessagePut(TESTQUEUE, KEY_MESSAGE, key);
    }
    CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
}

/******************************************************************************
  playIntro:
*****************************************************************************/
void hdpdemoPlayIntro(hdpDemoGlobalInstanceData_t *hdpData)
{

    printf("\nHit <ESC> to quit program!\n\n");
    printf("Local Device address :");
    hdpPrintBDAddress(&hdpData->bdLocal);

    printf("\nOptions: \n");

    if (!hdpData->doneInquiry)
    {
        printf("  0)  Start inquiry\n");
    }

    printf("  1)  Security Controller Configuration\n");
    printf("  2)  Register HDP service record\n");
    printf("  3)  Get Capablities of peer device\n");
    printf("  4)  Make device connectable\n");
    printf("  5)  Connect reliable channel\n");
    printf("  6)  Connect a virtual channel\n");
    printf("  7)  Disconnect virtual channel\n");
    printf("  8)  Disconnect Associate Channel\n");
    printf("  9)  Send Data\n");
    printf("  A)  Send a suspend request\n");
    printf("  B)  Deactivate HDP\n");
    printf("  C)  Unregister the HDP service record\n");
    printf("  D)  Resume the previous connection\n");
    printf("  E)  Perform an Echo Test\n");
    printf("  F)  IEEE Data Test -1 \n");
    printf("  G)  IEEE Data Test -2 (re-Associate follow after Test-1)\n"); 
    printf("  M)  Initiate Bonding\n");   
}

/******************************************************************************
  startActivate:
*****************************************************************************/
void hdpStartRegister(hdpDemoGlobalInstanceData_t *hdpData)
{
    printf("\nRegistering  the following Service record :");
    CsrBtHdpRegisterReqSend(hdpData->phandle,
                            SECL_NONE, 
                            CsrStrDup(service),
                            CsrStrDup(mdep_desc),
                            CsrStrDup(provider),
                            0, 
                            HDP_DEMO_APP_NO_END_POINTS, 
                            CSR_BT_HDP_SUPPORT_RECONNECT_INITIATION | CSR_BT_HDP_SUPPORT_RECONNECT_ACCEPTANCE);
}

void hdpSendRegisterRsp(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrBtMdepId        mdep_id;
    CsrBtMdepDataType datatype;
    CsrBtMdepRole      role;
    CsrBool             resuse = FALSE;
    CsrBtHdpRegisterInd *pMsg = (CsrBtHdpRegisterInd *)hdpData->recvMsgP;
    static CsrUint8 toggleRole = HDP_DEMO_APP_SOURCE;
    
    mdep_id = pMsg->mdepId;
    mdep_id = (resuse == FALSE )?pMsg->mdepId + 1 : pMsg->mdepId;
    datatype = CSR_BT_HDP_DEVICE_TYPE_BP;
    
    if (hdpData->role == HDP_DEMO_APP_BOTH_ROLES ) 
    {
        if ( toggleRole == HDP_DEMO_APP_SOURCE ) 
        {  
            role = HDP_DEMO_APP_SOURCE;
            toggleRole = HDP_DEMO_APP_SINK;
        }
        else
        {
            role = HDP_DEMO_APP_SINK;
            toggleRole = HDP_DEMO_APP_SINK;            
        }
    }
    else
    {
        role = hdpData->role;
    }
    printf("\nMdep Id %d, Datatype = %d, Role = %d",mdep_id,datatype,role);
    CsrBtHdpRegisterRspSend(hdpData->phandle,datatype,role,NULL, resuse);
}

void hdpGetCapab(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrBtDeviceAddr addr;
    addr = hdpData->bdAddrList[hdpData->selectedRemoteDeviceIdx];
    CsrBtHdpGetCapabReqSend(hdpData->phandle,
                            addr, 
                            0xFF,
                            0x00,
                            CSR_BT_HDP_ROLE_ANY);
}

void hdpConnectAssociateChannel(hdpDemoGlobalInstanceData_t *hdpData,CsrUint8 id)
{
   CsrBtDeviceAddr addr;
   CsrBtMdepRole role;
   addr = hdpData->bdAddrList[hdpData->selectedRemoteDeviceIdx];
   role = hdpData->mdepRole[id]; 
   CsrBtHdpConnectAssociateChannelReqSend(hdpData->phandle,
                                     hdpData->instanceId,
                                     addr, 
                                     hdpData->mdepId[id],
                                     hdpData->mdepDataType[id],
                                     role,                                     
                                     488);
   printf("\nSent CsrBtHdpConnectAssociateChannelReq");
    
}

void hdpConnectVirtualChannel(hdpDemoGlobalInstanceData_t *hdpData)
{
   CsrBtDeviceAddr addr;
   addr = hdpData->bdAddrList[hdpData->selectedRemoteDeviceIdx];
   CsrBtHdpConnectVirtualChannelReqSend(hdpData->phandle,hdpData->assocChId[hdpData->idxAssocChId],2,200);
   printf("\n Connect Virtual Channel Req sent\n");

}

void hdpDisconnectVirtualChannel(hdpDemoGlobalInstanceData_t *hdpData)
{
   printf("Disconnect Virtual Channel Request Sent\n");
   CsrBtHdpDisconnectVirtualChannelReqSend(hdpData->phandle,hdpData->virtualChId);
}

void hdpDisconnectAssociateChannel(hdpDemoGlobalInstanceData_t *hdpData)
{
   printf("\nSending a CsrBtHdpDisconnectAssociateChannelReq\n");
   printf(" on Assoc Ch Id - %x",hdpData->assocChId[hdpData->idxAssocChId]);
   CsrBtHdpDisconnectAssociateChannelReqSend(hdpData->phandle,hdpData->assocChId[hdpData->idxAssocChId]);
}

void hdpDataSend(hdpDemoGlobalInstanceData_t *hdpData)
{
   CsrUint8 *data = CsrPmemAlloc(100);
   int i;   
   for (i = 0; i < 100; i++)
   {
      data[i] = (CsrUint8)(i % 256);
   }
   printf("\n Data sent on Ch id - %x",hdpData->assocChId[hdpData->idxAssocChId]);
   CsrBtHdpDataReqSend(hdpData->phandle, hdpData->assocChId[hdpData->idxAssocChId],data,100);
}

void hdpResumeReqSend(hdpDemoGlobalInstanceData_t *hdpData)
{    
    printf("\nSending a CsrBtHdpResumeReq\n");
    printf(" on Assoc Ch Id - %x",hdpData->assocChId[hdpData->idxAssocChId]);
    CsrBtHdpResumeReqSend(hdpData->phandle, hdpData->assocChId[hdpData->idxAssocChId]);
}

void hdpSuspend(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrBtHdpSuspendReqSend(hdpData->phandle,hdpData->assocChId[hdpData->idxAssocChId]);
}

void hdpUnregister(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrBtHdpUnregisterReqSend(hdpData->phandle);
}

void hdpBond(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrBtDeviceAddr addr;
    addr = hdpData->bdAddrList[hdpData->selectedRemoteDeviceIdx];
    CsrBtScBondReqSend(hdpData->phandle, addr);
}
void hdpEchoTest(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrBtDeviceAddr addr;
    addr = hdpData->bdAddrList[hdpData->selectedRemoteDeviceIdx];
    CsrBtHdpConnectAssociateChannelReqSend(hdpData->phandle,
                                           hdpData->instanceId,
                                           addr, 
                                           CSR_BT_HDP_ECHO_TEST_MDEP,
                                           hdpData->mdepDataType[0],
                                           hdpData->mdepRole[0],
                                           CSR_BT_HDP_MAX_PACKET_LEN);
    hdpData->state = connecting;
}
/******************************************************************************
  startInquiry:
*****************************************************************************/
void hdpStartInquiry(hdpDemoGlobalInstanceData_t *hdpData)
{

    hdpData->state        = inquiry;
    hdpData->numInList            = 0;

    CsrBtSdSearchReqSend(hdpData->phandle,
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
    printf("Press  'C' to stop the inquiry and select a device\n");
}

/******************************************************************************
  initInstanceData:
*****************************************************************************/
void initInstanceData(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrUint8    i;
    extern BD_ADDR_T defGlobalBdAddr;

    BD_ADDR_T zeroAddr={0,0,0};

    hdpData->recvMsgP         = NULL;
    hdpData->state            = init;
    hdpData->keyState         = init;
    hdpData->numInList        = 0;
    hdpData->instanceId       = 0;
    hdpData->idxAssocChId     = 0;

    for (i  =0 ; i < MAX_MDEP ; i++ )
    {
        hdpData->assocChId[i]        = 0;
        hdpData->mdepId[i]           = 0;
        hdpData->mdepDataType[i]     = 0;
        hdpData->mdepRole[i]         = 0;
    }

    if (!CsrBtBdAddrEq(&defGlobalBdAddr, &zeroAddr))
    {
        for (i = 0; i < MAX_DISCOVERY_RESULTS_IN_LIST; i++)
        {
            hdpData->bdAddrList[i].lap = defGlobalBdAddr.lap;
            hdpData->bdAddrList[i].nap = defGlobalBdAddr.nap;
            hdpData->bdAddrList[i].uap = defGlobalBdAddr.uap;
        }

        hdpData->selectedRemoteDeviceIdx = 0;
        hdpData->doneInquiry      = TRUE;
    }
    else
    {
     for (i = 0; i < MAX_DISCOVERY_RESULTS_IN_LIST; i++)
     {
         hdpData->bdAddrList[i].lap = 0;
         hdpData->bdAddrList[i].nap = 0;
         hdpData->bdAddrList[i].uap = 0;
     }
        hdpData->doneInquiry      = FALSE;
    }


    hdpData->phandle = TESTQUEUE;

}


static void hdpScAppSetFnFini(void *pFiniData)
{
    hdpDemoGlobalInstanceData_t *instData = (hdpDemoGlobalInstanceData_t *) pFiniData;
    scAppRestoreState((CsrUint8 *) &instData->prevState, (CsrUint8 *) &instData->state);
    hdpdemoPlayIntro(instData);
}

static void hdpScAppSetFnInit(void *pInitData)
{
   hdpDemoGlobalInstanceData_t *instData = (hdpDemoGlobalInstanceData_t *) pInitData;
   scAppSaveState((CsrUint8 *) &instData->prevState, (CsrUint8 *) &instData->state, pairing);    
}

static void* hdpScAppSetFnGetMessage(void *pRecvMsgPData)
{
    hdpDemoGlobalInstanceData_t *instData = (hdpDemoGlobalInstanceData_t *) pRecvMsgPData;
    void *result                          = instData->recvMsgP;
    instData->recvMsgP                    = NULL;
    return result;
}


/******************************************************************************
 * Init function called by the scheduler upon initialisation.
 * This function is used to boot the demo application.
 *****************************************************************************/
void CsrBtAppInit(void **gash)
{
    hdpDemoGlobalInstanceData_t    *hdpData;

    CsrBtCmWriteCodReq *prim;
    CsrUint32 cod = 0x000900;

    extern BD_ADDR_T defGlobalBdAddr;
    /* allocate and initialise instance data space */
    *gash = (void *) CsrPmemAlloc(sizeof(hdpDemoGlobalInstanceData_t));
    hdpData = (hdpDemoGlobalInstanceData_t *) *gash;
    initInstanceData(hdpData);

    /* Register at the security controller */
    CsrBtScActivateReqSend(hdpData->phandle);
  
    /* Security App Configuration */
    scAppInitInstanceData(&hdpData->scInstData);
    
    scAppSetFnFini(&hdpData->scInstData, hdpScAppSetFnFini, hdpData);
    scAppSetFnInit(&hdpData->scInstData, hdpScAppSetFnInit, hdpData);
    scAppSetSecLevelSend(&hdpData->scInstData, scAppSendProfileSecurityLevelReq, &hdpData->scInstData);
   
    scAppSetFnGetMessage(&hdpData->scInstData, hdpScAppSetFnGetMessage, hdpData);
   
    scAppSetSecLevelPrim(&hdpData->scInstData,
                         0,     /* Index 0. Single profile */
                         "HDP",
                         CSR_BT_HDP_PRIM, CSR_BT_HDP_IFACEQUEUE,
                         CSR_BT_HDP_SECURITY_IN_REQ, CSR_BT_HDP_SECURITY_OUT_REQ);    

    CsrBtCmReadLocalBdAddrReqSend(hdpData->phandle);

    prim = CsrPmemAlloc(sizeof(CsrBtCmWriteCodReq));
    prim->type = CSR_BT_CM_WRITE_COD_REQ;
    prim->appHandle = hdpData->phandle;
    prim->updateFlags = (CSR_BT_CM_WRITE_COD_UPDATE_FLAG_SERVICE_CLASS |
                         CSR_BT_CM_WRITE_COD_UPDATE_FLAG_MAJOR_MINOR_CLASS);

    prim->serviceClassOfDevice = cod;
    prim->majorClassOfDevice = cod;
    prim->minorClassOfDevice = cod;

    CsrSchedMessagePut(CSR_BT_CM_IFACEQUEUE, CSR_BT_CM_PRIM, prim);

    printf("Starting up, Please wait....\n");
}

#ifdef ENABLE_SHUTDOWN
void CsrBtAppDeinit(void **gash)
{
    CsrUint16 msg_type=0;
    void *msg_data=NULL;
    hdpDemoGlobalInstanceData_t *hdpData;

    hdpData = (hdpDemoGlobalInstanceData_t *) (*gash);
    /* get a message from the demoapplication message queue. The message is returned in prim
       and the event type in eventType */
    while ( CsrSchedMessageGet(&msg_type, &msg_data) )
    {
        switch (msg_type)
        {
            case CSR_BT_HDP_PRIM:
                /*free handler*/
                break;
            case CSR_BT_SC_PRIM:
                CsrBtScFreeUpstreamMessageContents(msg_type, msg_data);
                break;
            case CSR_BT_SD_PRIM:
                CsrBtSdFreeUpstreamMessageContents(msg_type, msg_data);
                break;
        }
        CsrPmemFree(msg_data);
    }

    CsrPmemFree(hdpData);
}
#endif

/******************************************************************************
 * This is the demo application handler function. All primitives sent
 * to the demo application will be received in here. The scheduler
 * ensures that the function is activated when a signal is put on the
 * demo application signal queue.
 *****************************************************************************/
void CsrBtAppHandler(void **gash)
{
   hdpDemoGlobalInstanceData_t *hdpData;
   CsrUint16    eventType=0;
   void        *msg=NULL;

   hdpData = (hdpDemoGlobalInstanceData_t *) (*gash);
   CsrSchedMessageGet(&eventType, &msg);

   hdpData->recvMsgP = msg;

   switch (eventType)
   {
   case CSR_BT_HDP_PRIM:
      handleHdpPrim(hdpData);
      break;

   case CSR_BT_SC_PRIM:
       handleScPrim(&hdpData->scInstData);
       break;

   case CSR_BT_SD_PRIM:
       handleSdPrim(hdpData);
       break;

   case CSR_BT_CM_PRIM :
      handleCmPrim(hdpData);
      break;

   case KEY_MESSAGE:
      {
          CsrBtKeyPress_t *key;
          key = (CsrBtKeyPress_t*)hdpData->recvMsgP;

          hdpHandleKeybPrim(key->key, hdpData);
          break;
      }

   default:

       /* unexpected primitive received */
       printf("####### default in testhandler %x,\n",eventType);

   }
   CsrPmemFree(hdpData->recvMsgP);
}

/******************************************************************************
 * Function to handle all hdp-primitives.
 ******************************************************************************/


void handleHdpPrimIeeeAgent(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrPrim    *primType = (CsrPrim *) hdpData->recvMsgP;  


    if (*primType == CSR_BT_HDP_DATA_IND )
    {
        CsrBtHdpDataInd *pMsg = (CsrBtHdpDataInd *)hdpData->recvMsgP;
        CsrUint8 *pData = pMsg->data;           
        switch(hdpData->ieeeState)
        {
            case associate :
            {
                if ( (pData[0] == AARE_APDU) &&
                     (pData[5] == IEEE_ACCEPTED_UNKNOWN_CONFIG ) )
                {
                    printf("\n Received AARE_APDU with ACCEPTED_UNKNOWN_CONFIG");
                    hdpdemoSendApduConfig(hdpData);
                    hdpData->ieeeState = configuring;
                }
                else if ( (pData[0] == AARE_APDU) &&
                          (pData[5] == IEEE_ACCEPTED_CONFIG ) )
                {

                    printf("\n Received AARE_APDU with ACCEPTED_CONFIG");
                    hdpdemoDataMeasurement(hdpData);                    
                    hdpData->ieeeState = disassociating;
                }
                else
                {
                    printf("\n Received ADPU : %x with %x",pData[0],pData[5]);    
                }
            }
            break;

            case configuring :
            {
                if ( (pData[0]  == PRST_APDU) &&
                     ( pData[8] == REMOTE_OPERATION_RSP ) &&
                     ( pData[9] == CONFIRMED_EVENT_REPORT ) )
                {
                    CsrUint8 len = pData[3] + 3; /* add the offset of length from begining */

                    printf("\n Received PRST_APDU{Respone|Confirmed Event Report} ");
                    if ( ( pData[len-1] == 0 ) &&
                         ( pData[len] == 0 )  ) /*accepted-config */
                    {
                        hdpData->ieeeState = operating/*mds*/;
                        hdpdemoDataMeasurement(hdpData);
                        hdpData->ieeeState = disassociating;
                        printf("with ACCEPTED_CONFIG");                        
                    }
                    else
                    {
                        printf("\n Received ADPU : %x in state %d",pData[0],hdpData->ieeeState);    
                    }
                }
                else if ( (pData[0]  == PRST_APDU) &&
                     ( pData[8] == REMOTE_OPERATION_INVOKE ) &&
                     ( pData[9] == GET ) )
                {
                    printf("\n Received Data{Invoke|Get handle = 0} ");
                    hdpdemoSendDataMDSAttrib(hdpData);
                    /* hdpData->ieeeState = operating; */
                }                
                else
                {
                    printf("\n Received ADPU : %x in state %d",pData[0],hdpData->ieeeState);    

                    printf("\n####### Error in Agent configuration");
                }
                
            }
            break;        

            case mds :
            {
                if ( (pData[0]  == PRST_APDU) &&
                     ( pData[8] == REMOTE_OPERATION_INVOKE ) &&
                     ( pData[9] == GET ) )
                {
                    printf("\n Received Data{Invoke|Get handle = 0} ");
                    hdpdemoSendDataMDSAttrib(hdpData);
                    hdpData->ieeeState = operating;
                }
                else
                {
                    printf("\n####### Error in Get MDS attributes");
                }
                
            }
            break;

            case disassociating :
            {
                if ( (pData[0]  == PRST_APDU) &&
                     ( pData[8] == REMOTE_OPERATION_RSP ) &&
                     ( pData[9] == CONFIRMED_EVENT_REPORT ) )
                {
                    printf("\n Received Data{Response|Confirmed Event Report} ");
                    hdpdemoRlrqApdu(hdpData);
                    hdpData->ieeeState = disassociate;

                }
                else
                {
                    printf("\n Received ADPU : %x in state %d",pData[0],hdpData->ieeeState);    
                    
                    printf("\n####### Error in operating state");
                }
                
            }
            break;            

            case disassociate :
            {
                if ( (pData[0]  == RLRE_APDU)  )
                {
                    printf("\n Received RLRE_APDU");
                    hdpData->ieeeState = pass;
                    printf("\n Dissasociated");
                }
                else
                {
                    printf("\n Received ADPU : %x in state %d",pData[0],hdpData->ieeeState);    

                    printf("\n####### Error");
                }
                
            }
            break;            

            default :
                printf("\n####### default in handleHdpPrimIeeeAgent %x,\n",pData[0]);
        }

    }
    else if (*primType == CSR_BT_HDP_DATA_CFM )
    {
        CsrBtHdpDataCfm *pMsg = (CsrBtHdpDataCfm *)hdpData->recvMsgP;

        printf("\n Received CsrBtHdpDataCfm with resultCode = %x and resultSupplier = %x",
                                                    pMsg->resultCode,pMsg->resultSupplier);
        switch (hdpData->ieeeState)
        {
            case operating  :
            {
                if ( pMsg->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS )
                {

                    hdpdemoDataMeasurement(hdpData);
                    hdpData->ieeeState = disassociating;
                }
                else
                {
                    printf("\n####### Error in Get MDS attributes");
                }
            }
            break; 
                       
            
            default :
                break; 
        }
    }
    else
    {
        
    }
}


void handleHdpPrimIeeeManager(hdpDemoGlobalInstanceData_t *hdpData)
{


    CsrPrim    *primType = (CsrPrim *) hdpData->recvMsgP;  


    if (*primType == CSR_BT_HDP_DATA_IND )
    {
        CsrBtHdpDataInd *pMsg = (CsrBtHdpDataInd *)hdpData->recvMsgP;
        CsrUint8 *pData = pMsg->data;         
        switch(hdpData->ieeeState)
        {
            case associate :
            {
                CsrUint16 devConfigId;
                
                devConfigId = pData[44];
                devConfigId = ( devConfigId << 8 ) | pData[45];                

                if ( (pData[0] == AARQ_APDU) &&
                      (hdpData->devConfigId != devConfigId ) )
                {
                    printf("\n Received AARQ_APDU with Unknown config %x",devConfigId);
                    hdpData->devConfigId = devConfigId;
                    hdpdemoSendAareApdu(hdpData,FALSE);
                    hdpData->ieeeState = w4config;
                }
                else if( (pData[0] == AARQ_APDU) &&
                      (hdpData->devConfigId == devConfigId ) )
                {
                    printf("\n Received AARQ_APDU with known config %x",hdpData->devConfigId );
                    hdpdemoSendAareApdu(hdpData,TRUE);
                    hdpData->ieeeState = operating;
                }
                else
                {
                    printf("\n Received ADPU : %x with %x",pData[0],pData[5]);

                }
            }
            break;

            case w4config :
            {
                if ( (pData[0]  == PRST_APDU) &&
                     ( pData[8] == REMOTE_OPERATION_INVOKE ) &&
                     ( pData[9] == CONFIRMED_EVENT_REPORT ) )
                {
                    printf("\n Received Configuration");
                    hdpdemoSendApduConfigRsp(hdpData);
                    hdpData->ieeeState = operating  /* mds*/;

                }
                else
                {
                    printf("\n Received ADPU : %x in state %d",pData[0],hdpData->ieeeState);    

                    printf("\n####### Error in received configuration");
                }
                
            }
            break;  

            case mds_recv :
            {
                if ( (pData[0]  == PRST_APDU) &&
                     ( pData[8] == REMOTE_OPERATION_RSP ) &&
                     ( pData[9] == GET ) )
                {
                    printf("\n Received MDS Attributes");
                    hdpData->ieeeState = operating;
                }
                else
                {
                    printf("\n Received ADPU : %x in state %d",pData[0],hdpData->ieeeState);    

                    printf("\n####### Error in receiving attributes");
                }
                
            }
            break;              

            case operating :
            {
                if ( (pData[0]  == PRST_APDU) &&
                     ( pData[8] == REMOTE_OPERATION_INVOKE ) &&
                     ( pData[9] == CONFIRMED_EVENT_REPORT ) )
                {
                    printf("\nReceived Measurement");
                    hdpdemoDataMeasurementCfm(hdpData);
                    hdpData->ieeeState = disassociating;

                }
                else
                {
                    printf("\n Received ADPU : %x in state %d",pData[0],hdpData->ieeeState);    
                    
                    printf("\n####### Error in operating state");
                }
                
            }
            break;            

            case disassociating :
            {
                if ( (pData[0]  == RLRQ_APDU)  )
                {
                    printf("\nReceived RLRQ_APDU");
                    hdpdemoRlreApdu(hdpData);
                    hdpData->ieeeState = disassociate;
                    printf("\n Dissasociated");
                }
                else
                {
                    printf("\nReceived APDU %x",pData[0]);
                    printf("\n####### Error");
                }
                
            }
            break;            

            default :
                printf("\n####### default in handleHdpPrimIeeeManager %x,\n",pData[0]);
        }

    }
    else if (*primType == CSR_BT_HDP_DATA_CFM )
    {
        CsrBtHdpDataCfm *pMsg = (CsrBtHdpDataCfm *)hdpData->recvMsgP;

        printf("\n Received CsrBtHdpDataCfm with resultCode = %x and resultSupplier = %x",
                                                    pMsg->resultCode,pMsg->resultSupplier);
        switch (hdpData->ieeeState)
        {
            case mds  :
            {
                if ( pMsg->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS )
                {

                    hdpdemoSendDataMDSGet(hdpData);
                    hdpData->ieeeState = mds_recv;
                }
                else
                {
                    printf("\n####### Error in Get MDS attributes");
                }
            }
            break; 

            case disassociate :
            {
                if ( pMsg->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS )
                {
                    hdpData->ieeeState = pass;
                }
                else
                {
                    printf("\n####### Error in disassociating");
                }
            }
            break;

            default :
                break; 
        }
    }
    else
    {
        
    }
}

void handleHdpPrim(hdpDemoGlobalInstanceData_t *hdpData)
{
   CsrPrim    *primType;

   primType = (CsrPrim *) hdpData->recvMsgP;

   switch (hdpData->state)
   {
      case registering :
         
         switch (*primType)
         {
            case CSR_BT_HDP_REGISTER_IND:
               hdpSendRegisterRsp(hdpData);
               break;
               
            case CSR_BT_HDP_REGISTER_CFM :
            {
               CsrBtHdpRegisterCfm *msg = (CsrBtHdpRegisterCfm *)hdpData->recvMsgP;
               hdpData->state = registered;
               printf("\nService Record Registered\nResult Code %d",msg->resultCode);
               hdpdemoPlayIntro(hdpData);
               CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
               break;
            }
            case CSR_BT_HDP_DEACTIVATE_CFM:
             {
               CsrBtHdpDeactivateCfm *prim = (CsrBtHdpDeactivateCfm *)hdpData->recvMsgP;
               printf("HDP deactivated: Result 0x%04X, Supplier 0x%04X\n",prim->resultCode, prim->resultSupplier);
               hdpdemoPlayIntro(hdpData);
               CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
               break;
            }
            default:
               printf("####### default in hdp prim handler 0x%04x in state %d\n",*primType,hdpData->state);
               /* release unexpected primitive received */
            
         }
         break;
      case registered : 
         if ( *primType == CSR_BT_HDP_ACTIVATE_CFM )
         {
               hdpData->state = connectable;
               printf("Device is connectable\n");
               hdpdemoPlayIntro(hdpData);
               CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
         }
         else if (*primType == CSR_BT_HDP_UNREGISTER_CFM )
         {
            CsrBtHdpUnregisterCfm *prim = (CsrBtHdpUnregisterCfm*)hdpData->recvMsgP;
            printf("CSR_BT_HDP_UNREGISTER_CFM Result 0x%04X, Supplier 0x%04X\n",prim->resultCode, prim->resultSupplier);            
         }
         else
         {
             printf("####### default in hdp prim handler 0x%04x in state %d\n",*primType,hdpData->state);
         }
         break;
      case getcapab:
 
         switch (*primType)
         {
            case CSR_BT_HDP_CTRL_GET_CAPAB_IND:
            {
               CsrBtHdpMdep *temp;
               CsrBtHdpCtrlGetCapabInd *prim = (CsrBtHdpCtrlGetCapabInd *)hdpData->recvMsgP;
               CsrUint16 i = 0;


               printf("\nThe results retreived were\n");
               printf("Instance Id %x\n", prim->hdpInstanceId);
               printf("Provider Name - [%s]\n", prim->providerName);
               printf("Service Name  - [%s]\n", prim->serviceName);
               printf("Supported Features Length %d\n",prim->supportedFeatureListLen);
               hdpData->instanceId = prim->hdpInstanceId;

               for(i=0;i<prim->supportedFeatureListLen;i++)
               {
                  temp = prim->supportedFeatureList + i;

                  hdpData->mdepId[i] = temp->id;
                  hdpData->mdepDataType[i] = temp->dataType;
                  hdpData->mdepRole[i] = temp->role;
                  printf("MDEP REFERENCE - %d\n",i);
                  printf("MDEP ID=%d\n",temp->id);
                  printf("MDEP DataType = %d\n",temp->dataType);
                  printf("MDEP Role = %d\n",temp->role);
               }
               temp = prim->supportedFeatureList;
               CsrPmemFree(temp);
               CsrPmemFree(prim->providerName);
               CsrPmemFree(prim->serviceDescrip);
               CsrPmemFree(prim->serviceName);

               break;
            }
            case CSR_BT_HDP_CTRL_GET_CAPAB_CFM:
            {
               CsrBtHdpCtrlGetCapabCfm *msg = (CsrBtHdpCtrlGetCapabCfm *)hdpData->recvMsgP;
               printf("Confirm Received for Get Capablities Result 0x%04X, Supplier 0x%04X\n",msg->resultCode, msg->resultSupplier);

               hdpdemoPlayIntro(hdpData);
               CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
               break;
               
            }
            case CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND:
            {
               CsrBtHdpAcceptConnectAssociateChannelInd *prim = (CsrBtHdpAcceptConnectAssociateChannelInd *)hdpData->recvMsgP;

               printf("\nReceived a CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND");
               printf("\n from device :");
               hdpPrintBDAddress(&prim->deviceAddr);
               printf("\n for Mdep Id - %d",prim->mdepId);
               printf("\n Asso channel Id = %x",prim->assocChannelId); 

               printf("\nSending a HdpAcceptConnectAssociateChannelRes");               
               CsrBtHdpAcceptConnectAssociateChannelResSend(hdpData->phandle, 
                                                            CSR_BT_RESULT_CODE_HDP_SUCCESS,
                                                            200,
                                                            prim->assocChannelId);
               hdpData->state = connectable;
            break;
            }
            break;
            default:
                printf("####### default in hdp prim handler 0x%04x in state %d\n",*primType,hdpData->state);
         }
         break;
      case connecting:
         {
          switch (*primType)
          {
            case CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM:
            {
               CsrBtHdpConnectAssociateChannelCfm *prim = (CsrBtHdpConnectAssociateChannelCfm *)hdpData->recvMsgP;
               printf("\nReceived a CsrBtHdpConnectAssociateChannelCfm");
               printf("\nwith Result 0x%04X, Supplier 0x%04X\n",prim->resultCode, prim->resultSupplier);
               printf("\nfrom device : ");
               hdpPrintBDAddress(&prim->deviceAddr);
               printf("\n for Mdep id - %d",prim->mdepId);
               printf("\n of Datatype - %d",prim->dataType);
               printf("\n hdpInstanceId - %x",prim->hdpInstanceId);
               printf("\n Assoc chan Id - %x",prim->assocChannelId );  
               printf("\n");
              
               hdpData->assocChId[hdpData->idxAssocChId] = prim->assocChannelId;
               if(prim->mdepId == CSR_BT_HDP_ECHO_TEST_MDEP && (prim->resultCode == 0x0080 || prim->resultCode == 0x0000))
               { 
                 hdpDataSend(hdpData);
               }
               hdpdemoPlayIntro(hdpData);
               CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
               hdpData->state = connected;
               break;
            }
            case CSR_BT_HDP_DEACTIVATE_CFM:
            {
               CsrBtHdpDeactivateCfm *prim = (CsrBtHdpDeactivateCfm *)hdpData->recvMsgP;
               printf("HDP deactivated: Result 0x%04X, Supplier 0x%04X\n",prim->resultCode, prim->resultSupplier);
               hdpdemoPlayIntro(hdpData);
               CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
               break;
            }
            default:
                printf("####### default in hdp prim handler 0x%04x in state %d\n",*primType,hdpData->state);
          }
         } 
         break;
      case connectable:
         {
          switch (*primType)
          {
            case CSR_BT_HDP_ACTIVATE_CFM:
            {
               hdpData->state = connectable;
               printf("Device is connectable\n");
               hdpdemoPlayIntro(hdpData);
               CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
               break;
            }
            case CSR_BT_HDP_DEACTIVATE_CFM:
            {
               CsrBtHdpDeactivateCfm *prim = (CsrBtHdpDeactivateCfm *)hdpData->recvMsgP;
               printf("HDP deactivated: Result 0x%04X, Supplier 0x%04X\n",prim->resultCode, prim->resultSupplier);
               hdpdemoPlayIntro(hdpData);
               CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
               break;
            }
            case CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND:
            {
               CsrBtHdpAcceptConnectAssociateChannelInd *prim = (CsrBtHdpAcceptConnectAssociateChannelInd *)hdpData->recvMsgP;

               printf("\nReceived a CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND");
               printf("\n from device :");
               hdpPrintBDAddress(&prim->deviceAddr);
               printf("\n for Mdep Id - %d",prim->mdepId);
               printf("\n Asso channel Id = %x",prim->assocChannelId); 

               printf("\nSending a HdpAcceptConnectAssociateChannelRes");               
               CsrBtHdpAcceptConnectAssociateChannelResSend(hdpData->phandle, 
                                                            CSR_BT_RESULT_CODE_HDP_SUCCESS,
                                                            200,
                                                            prim->assocChannelId);
               break;
            }
            case CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND:
            {
               CsrBtHdpConnectAssociateChannelInd *prim = (CsrBtHdpConnectAssociateChannelInd *)hdpData->recvMsgP;    

               printf("\nReceived a CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND");
               printf("\nwith Result 0x%04X, Supplier 0x%04X\n",prim->resultCode, prim->resultSupplier);              
               printf("\nfrom device : ");
               hdpPrintBDAddress(&prim->deviceAddr);
               printf("\n for Mdep id - %d",prim->mdepId);
               printf("\n of Datatype - %d",prim->dataType);
               printf("\n hdpInstanceId - %x",prim->hdpInstanceId);
               printf("\n Assoc chan Id - %x",prim->assocChannelId );
               hdpData->assocChId[hdpData->idxAssocChId] = prim->assocChannelId;
               hdpData->state = connected;
               printf("\n");
               hdpdemoPlayIntro(hdpData);
               CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
               break;
            }
            case CSR_BT_HDP_CTRL_GET_CAPAB_IND:
            {
               CsrBtHdpMdep *temp;
               CsrBtHdpCtrlGetCapabInd *prim = (CsrBtHdpCtrlGetCapabInd *)hdpData->recvMsgP;
               CsrUint16 i = 0;
               /* upf */ 
               printf("\nThe results retreived were\n");
               printf("Instance Id %x\n", prim->hdpInstanceId);
               printf("Provider Name - [%s]\n", prim->providerName);
               printf("Service Name  - [%s]\n", prim->serviceName);
               
               hdpData->instanceId = prim->hdpInstanceId;
               

               for(i=0;( i<prim->supportedFeatureListLen )  
                        ;i++)
               {
                  temp = prim->supportedFeatureList + i;

                  hdpData->mdepId[i] = temp->id;
                  hdpData->mdepDataType[i] = temp->dataType;
                  hdpData->mdepRole[i] = temp->role;
                  printf("MDEP REFERENCE - %d\n",i);  
                  printf("MDEP ID=%d\n",temp->id);
                  printf("MDEP DataType = %d\n",temp->dataType);
                  printf("MDEP Role = %d\n",temp->role);                                    
               }
               temp = prim->supportedFeatureList;
               CsrPmemFree(temp);
               CsrPmemFree(prim->providerName);
               CsrPmemFree(prim->serviceDescrip);
               CsrPmemFree(prim->serviceName);
               break;
            }
            case CSR_BT_HDP_CTRL_GET_CAPAB_CFM:
            {
               CsrBtHdpCtrlGetCapabCfm *msg = (CsrBtHdpCtrlGetCapabCfm *)hdpData->recvMsgP;
               printf("Confirm Received for Get Capablities Result 0x%04X, Supplier 0x%04X\n",msg->resultCode, msg->resultSupplier);

               hdpdemoPlayIntro(hdpData);
               CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
               break;
            }
            case CSR_BT_HDP_RESUME_IND:
            {
                CsrBtHdpResumeInd *prim = (CsrBtHdpResumeInd *) hdpData->recvMsgP;
                printf("\nCSR_BT_HDP_RESUME_IND received for Association Ch ID %x",
                        prim->chId);
                printf("\nSending a CsrBtHdpResumeReq");
                printf("\n on Assoc Ch Id - %x\n",prim->chId);
                CsrBtHdpResumeResSend(hdpData->phandle, prim->chId, TRUE,0x2A);                         
                hdpdemoPlayIntro(hdpData);
                CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
                break;
            }            
            default:
                printf("####### default in hdp prim handler 0x%04x in state %d\n",*primType,hdpData->state);
          }
         }
        break;
       case connected:
         {
          switch (*primType)
          {
            case CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_IND:
            {
               int mclId,mdlId;
               CsrBtHdpAcceptConnectVirtualChannelInd *prim = (CsrBtHdpAcceptConnectVirtualChannelInd *)hdpData->recvMsgP;
               printf("CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_IND received\n");
               mdlId = (prim->virtualChannelId & CSR_BT_HDP_INSTANCE_MDL_MASK);
               mclId = (prim->virtualChannelId >> 16 & CSR_BT_HDP_INSTANCE_MDL_MASK);
               hdpData->virtualChId = prim->virtualChannelId;
               CsrBtHdpAcceptConnectVirtualChannelResSend(hdpData->phandle,
                                                          prim->virtualChannelId,
                                                          2, 
                                                          200,
                                                          CSR_BT_RESULT_CODE_HDP_SUCCESS);
               break;
            }
            case CSR_BT_HDP_DEACTIVATE_CFM:
            {
               CsrBtHdpDeactivateCfm *prim = (CsrBtHdpDeactivateCfm *)hdpData->recvMsgP;
               printf("HDP deactivated: Result 0x%04X, Supplier 0x%04X\n",prim->resultCode, prim->resultSupplier);
               hdpdemoPlayIntro(hdpData);
               CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
               hdpData->state = selecting;
               break;
            }   
            case CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND:
            {
               CsrBtHdpAcceptConnectAssociateChannelInd *prim = (CsrBtHdpAcceptConnectAssociateChannelInd *)hdpData->recvMsgP;
     
               printf("\nReceived a CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND");
               printf("\n from device :");
               hdpPrintBDAddress(&prim->deviceAddr);
               printf("\n for Mdep Id - %d",prim->mdepId);
               printf("\n Asso channel Id = %x",prim->assocChannelId); 

               printf("\nSending a HdpAcceptConnectAssociateChannelRes");
               CsrBtHdpAcceptConnectAssociateChannelResSend(hdpData->phandle, 
                                                            CSR_BT_RESULT_CODE_HDP_SUCCESS, 
                                                            200,
                                                            prim->assocChannelId);
               break;
            }
            case CSR_BT_HDP_CTRL_GET_CAPAB_IND:
            {
               CsrBtHdpMdep *temp;
               CsrBtHdpCtrlGetCapabInd *prim = (CsrBtHdpCtrlGetCapabInd *)hdpData->recvMsgP;
               CsrUint16 i = 0;

               /* upf */ 
               printf("\nThe results retreived were\n");
               printf("Instance Id %x\n", prim->hdpInstanceId);
               printf("Provider Name - [%s]\n", prim->providerName);
               printf("Service Name  - [%s]\n", prim->serviceName);             
               hdpData->instanceId = prim->hdpInstanceId;
               
               /* upf-32*/
               for(i=0;( i<prim->supportedFeatureListLen ) 
                        ;i++)
               {
                  temp = prim->supportedFeatureList + i;

                  hdpData->mdepId[i] = temp->id;
                  hdpData->mdepDataType[i] = temp->dataType;
                  hdpData->mdepRole[i] = temp->role;
                  printf("MDEP REFERENCE - %d\n",i);  
                  printf("MDEP ID=%d\n",temp->id);
                  printf("MDEP DataType = %d\n",temp->dataType);
                  printf("MDEP Role = %d\n",temp->role);                                    
               }
               temp = prim->supportedFeatureList;
               CsrPmemFree(temp);
               CsrPmemFree(prim->providerName);
               CsrPmemFree(prim->serviceDescrip);
               CsrPmemFree(prim->serviceName);
               break;
            }
            case CSR_BT_HDP_CTRL_GET_CAPAB_CFM:
            {
               CsrBtHdpCtrlGetCapabCfm *msg = (CsrBtHdpCtrlGetCapabCfm *)hdpData->recvMsgP;
               printf("Confirm Received for Get Capablities Result 0x%04X, Supplier 0x%04X\n",msg->resultCode, msg->resultSupplier);

               hdpdemoPlayIntro(hdpData);
               CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
               break;
            }
             case CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND:
            {
               CsrBtHdpConnectAssociateChannelInd *prim = (CsrBtHdpConnectAssociateChannelInd *)hdpData->recvMsgP;    
               /* upf */
               printf("\nReceived a CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND");
               printf("\nwith Result 0x%04X, Supplier 0x%04X\n",prim->resultCode, prim->resultSupplier);              
               printf("\nfrom device");
               hdpPrintBDAddress(&prim->deviceAddr);
               printf("\n for Mdep id %d",prim->mdepId);
               printf("\n of Datatype %d",prim->dataType);
               printf("\n hdpInstanceId - %x",prim->hdpInstanceId);
               printf("\n Assoc chan Id - %x",prim->assocChannelId );
               hdpData->assocChId[hdpData->idxAssocChId] = prim->assocChannelId;
               hdpData->state = connected;
               printf("\n");
               hdpdemoPlayIntro(hdpData);
               CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
               break;
            }
            case CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_IND:
            {
               printf("CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_IND\nOperation Complete\n");

               break;
            }
            case CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_CFM:
            {
               CsrBtHdpConnectVirtualChannelCfm *prim = (CsrBtHdpConnectVirtualChannelCfm *)hdpData->recvMsgP;    
               printf("CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_CFM received with result 0x%04X (supplier 0x%04X)\nOperation Complete\n",prim->resultCode, prim->resultSupplier);
               hdpData->virtualChId = prim->virtualChannelId;
               break;
            }

            case CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_IND:
            {
               CsrBtHdpDisconnectVirtualChannelInd *prim = (CsrBtHdpDisconnectVirtualChannelInd *)hdpData->recvMsgP;    
               printf("CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_IND received with result 0x%04X (supplier 0x%04X) and Virtual Ch ID %d\nOperation Complete\n", prim->resultCode, prim->resultSupplier,prim->virtualChId);
               break;
            }

            case CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_CFM:
            {
               CsrBtHdpDisconnectVirtualChannelCfm *prim = (CsrBtHdpDisconnectVirtualChannelCfm *)hdpData->recvMsgP;    
               printf("CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_CFM received with result 0x%04X (supplier 0x%04X) and Virtual Ch ID %d\nOperation Complete\n",prim->resultCode, prim->resultSupplier,prim->virtualChId);
               break;
            }

            case CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND:
            {
               CsrBtHdpDisconnectAssociateChannelInd *prim = (CsrBtHdpDisconnectAssociateChannelInd *)hdpData->recvMsgP;    
               printf("\nReceived a CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND");
               printf("\n   with result 0x%04X (supplier 0x%04X)",prim->resultCode, prim->resultSupplier);
               printf("\n   on Association Ch ID %x",prim->assocChId);
               printf(" \nOperation Complete\n");
               hdpData->state = suspend;
               break;
            }

            case CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM:
            {
               CsrBtHdpDisconnectAssociateChannelCfm *prim = (CsrBtHdpDisconnectAssociateChannelCfm *)hdpData->recvMsgP;    
               printf("\nReceived a CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM");
               printf("\n   with result 0x%04X (supplier 0x%04X)",prim->resultCode, prim->resultSupplier);
               printf("\n   on Assoc Ch Id - %x",prim->assocChId);
               printf("\n Operation Complete\n");
               break;
            }

            case CSR_BT_HDP_DATA_IND:
            {
               CsrBtHdpDataInd *prim = (CsrBtHdpDataInd *)hdpData->recvMsgP;    
               CsrUint8 *pData = prim->data;
               printf("\nReceived CSR_BT_HDP_DATA_IND ");
               printf("\n    length  %d\n",prim->length);
               printf("\n    data[0] - %x, data[%d] - %x",pData[0],prim->length,pData[prim->length - 1]); 
               CsrPmemFree(pData);
               break;
            }
            case CSR_BT_HDP_DATA_CFM:
            {
               CsrBtHdpDataCfm *prim = (CsrBtHdpDataCfm *)hdpData->recvMsgP;   
               printf("\nReceived CSR_BT_HDP_DATA_CFM ");
               printf("\n    with Result 0x%04X (supplier 0x%04X)\n",prim->resultCode, prim->resultSupplier);
               break;
            }
            case CSR_BT_HDP_UNREGISTER_CFM :
            {
               CsrBtHdpUnregisterCfm *prim = (CsrBtHdpUnregisterCfm*)hdpData->recvMsgP;
               printf("CSR_BT_HDP_UNREGISTER_CFM result= 0x%04X (supplier 0x%04X)\n",prim->resultCode, prim->resultSupplier);  
               hdpData->state = idle;
               break;
            }          
            case CSR_BT_HDP_RESUME_IND:
            {
                CsrBtHdpResumeInd *prim = (CsrBtHdpResumeInd *) hdpData->recvMsgP;
                printf("\nCSR_BT_HDP_RESUME_IND received with result 0x%04X (supplier 0x%04X)  \
                       \n for Association Ch ID %x",
                        prim->resultCode,
                        prim->resultSupplier,
                        prim->chId);
                printf("\nSending a CsrBtHdpResumeRes");
                printf("\n on Assoc Ch Id - %x\n",prim->chId);
                CsrBtHdpResumeResSend(hdpData->phandle, prim->chId, TRUE, 0x2A);                         
                hdpdemoPlayIntro(hdpData);
                CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
                break;
            }

            default:
                printf("####### default in hdp prim handler 0x%04x in state %d\n",*primType,hdpData->state);
          }
         }
        break;
      case suspend:
         switch (*primType )  
         {
            case CSR_BT_HDP_SUSPEND_CFM:
            {
                CsrBtHdpSuspendCfm *prim = (CsrBtHdpSuspendCfm *) hdpData->recvMsgP;
                printf("CSR_BT_HDP_SUSPEND_CFM received with result 0x%04X (supplier 0x%04X) \
                        \n and Association Ch ID %x\nOperation Complete\n",
                        prim->resultCode,
                        prim->resultSupplier,
                        prim->assocChId);
                hdpdemoPlayIntro(hdpData);
                CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
                break;

            }
            case CSR_BT_HDP_RESUME_CFM:
            {
                CsrBtHdpResumeCfm *prim = (CsrBtHdpResumeCfm *) hdpData->recvMsgP;
                printf("\nCSR_BT_HDP_RESUME_CFM received with result 0x%04X (supplier 0x%04X) \
                         \nand Association Ch ID %x\nOperation Complete\n",
                        prim->resultCode,
                        prim->resultSupplier,
                        prim->assocChId);
                hdpData->assocChId[hdpData->idxAssocChId] = prim->assocChId;
                hdpdemoPlayIntro(hdpData);
                CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
                break;
            }
            case CSR_BT_HDP_RESUME_IND:
            {
                CsrBtHdpResumeInd *prim = (CsrBtHdpResumeInd *) hdpData->recvMsgP;
                printf("\nCSR_BT_HDP_RESUME_IND received with result 0x%04X (supplier 0x%04X)  \
                       \n for Association Ch ID %x",
                        prim->resultCode,
                        prim->resultSupplier,
                        prim->chId);
                printf("\nSending a CsrBtHdpResumeRes");
                printf("\n on Assoc Ch Id - %x\n",prim->chId);
                CsrBtHdpResumeResSend(hdpData->phandle, prim->chId, TRUE,0x2A);                         
                hdpdemoPlayIntro(hdpData);
                CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
                break;
            }            
            case CSR_BT_HDP_DEACTIVATE_CFM:
            {
               CsrBtHdpDeactivateCfm *prim = (CsrBtHdpDeactivateCfm *)hdpData->recvMsgP;
               printf("HDP deactivated: Result 0x%04X (supplier 0x%04X) \n",prim->resultCode, prim->resultSupplier);
               hdpdemoPlayIntro(hdpData);
               CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
               break;
            }

            case CSR_BT_HDP_DATA_CFM:
            {
               CsrBtHdpDataCfm *prim = (CsrBtHdpDataCfm *)hdpData->recvMsgP;   
               printf("Result 0x%04X (supplier 0x%04X)\n",prim->resultCode, prim->resultSupplier);
               break;           
            }

            case CSR_BT_HDP_UNREGISTER_CFM :
            {
               CsrBtHdpUnregisterCfm *prim = (CsrBtHdpUnregisterCfm*)hdpData->recvMsgP;
               printf("CSR_BT_HDP_UNREGISTER_CFM result= 0x%04X (supplier 0x%04X)\n",prim->resultCode, prim->resultSupplier);            
               hdpData->state = idle;
               break;            
            }        
            case CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND:
            {
               CsrBtHdpConnectAssociateChannelInd *prim = (CsrBtHdpConnectAssociateChannelInd *)hdpData->recvMsgP;    
               /* upf */
               printf("\nReceived a CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND");
               printf("\nfrom device");
               hdpPrintBDAddress(&prim->deviceAddr);
               printf("\nresult= 0x%04X (supplier 0x%04X)",prim->resultCode, prim->resultSupplier);
               printf("\n for Mdep id %d",prim->mdepId);
               printf("\n of Datatype %d",prim->dataType);
               printf("\n hdpInstanceId - %x",prim->hdpInstanceId);
               printf("\n Assoc chan Id - %x",prim->assocChannelId );
               printf("\n");
               hdpdemoPlayIntro(hdpData);
               CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);

               break;
            }
            case CSR_BT_HDP_CTRL_GET_CAPAB_IND:
            {
               CsrBtHdpMdep *temp;
               CsrBtHdpCtrlGetCapabInd *prim = (CsrBtHdpCtrlGetCapabInd *)hdpData->recvMsgP;
               CsrUint16 i = 0;


               printf("\nThe results retreived were\n");
               printf("Instance Id %x\n", prim->hdpInstanceId);
               printf("Provider Name - [%s]\n", prim->providerName);
               printf("Service Name  - [%s]\n", prim->serviceName);
               printf("Supported Features Length %d\n",prim->supportedFeatureListLen);
               hdpData->instanceId = prim->hdpInstanceId;

               for(i=0;i<prim->supportedFeatureListLen;i++)
               {
                  temp = prim->supportedFeatureList + i;

                  hdpData->mdepId[i] = temp->id;
                  hdpData->mdepDataType[i] = temp->dataType;
                  hdpData->mdepRole[i] = temp->role;
                  printf("MDEP REFERENCE - %d\n",i);
                  printf("MDEP ID=%d\n",temp->id);
                  printf("MDEP DataType = %d\n",temp->dataType);
                  printf("MDEP Role = %d\n",temp->role);
               }
               temp = prim->supportedFeatureList;
               CsrPmemFree(temp);
               CsrPmemFree(prim->providerName);
               CsrPmemFree(prim->serviceDescrip);
               CsrPmemFree(prim->serviceName);

               break;
            }
            case CSR_BT_HDP_CTRL_GET_CAPAB_CFM:
            {
               CsrBtHdpCtrlGetCapabCfm *msg = (CsrBtHdpCtrlGetCapabCfm *)hdpData->recvMsgP;
               printf("Confirm Received for Get Capablities Result 0x%04X, Supplier 0x%04X\n",msg->resultCode, msg->resultSupplier);

               hdpdemoPlayIntro(hdpData);
               CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
               break;
               
            }

            default:
                printf("####### default in hdp prim handler 0x%04x in state %d\n",*primType,hdpData->state);
               break;
         }
         break;
      case ieee_agent :
        
        handleHdpPrimIeeeAgent(hdpData);
        if (hdpData->ieeeState == pass )
        {
            hdpData->state = connected;
            hdpdemoPlayIntro(hdpData);
            CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
        }
        break;

      case ieee_manager :
        
        handleHdpPrimIeeeManager(hdpData);
      
        if (hdpData->ieeeState == pass )
        {
            hdpData->state = connected;
            hdpdemoPlayIntro(hdpData);
            CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
        }
        break;

      default :
         printf("####### default in hdp : invalid state %04x,\n",hdpData->state);
         break;
            
   }

}

void handleCmPrim(hdpDemoGlobalInstanceData_t *hdpData)
{
    CsrPrim    *primType;

    primType = (CsrPrim *) hdpData->recvMsgP;

    switch (*primType)
    {
    case CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM :
        {
            CsrBtCmReadLocalBdAddrCfm *prim = (CsrBtCmReadLocalBdAddrCfm * )hdpData->recvMsgP;

            hdpData->bdLocal = prim->deviceAddr;
            hdpData->state = idle;
            hdpdemoPlayIntro(hdpData);
            CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
        }
        break;

    case CSR_BT_CM_WRITE_COD_CFM :
        {
            CsrBtCmWriteCodCfm *pCfm = (CsrBtCmWriteCodCfm *)hdpData->recvMsgP;
            if (pCfm->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
                pCfm->resultSupplier == CSR_BT_SUPPLIER_CM )
            {
                printf("\nCOD - Health Major device class\n");
            }
            else
            {
                printf("\nError- Setting Major device class\n");
            }
        }
        break;

    default:
        printf("####### default in cm prim handler 0x%04x,\n",*primType);
        /* release unexpected primitive received */

    }
}


/******************************************************************************
 * Function to handle all SD-primitives.
 ******************************************************************************/
void handleSdPrim(hdpDemoGlobalInstanceData_t *hdpData)
{
   CsrPrim    *primType;

   primType = (CsrPrim *) hdpData->recvMsgP;
   switch (*primType)
   {
   /* Inq result received */
   case CSR_BT_SD_SEARCH_RESULT_IND:
   {
       CsrBtSdSearchResultInd *prim = (CsrBtSdSearchResultInd *)hdpData->recvMsgP;
       if(hdpData->state == inquiry)
       {
           CsrUint16 nameLen;
           CsrUint8 *name;

           hdpData->bdAddrList[hdpData->numInList] = prim->deviceAddr;

           printf("\n%u: ", hdpData->numInList);
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

           if (hdpData->numInList > MAX_DISCOVERY_RESULTS_IN_LIST - 1)
           {
               CsrBtSdCancelSearchReqSend(hdpData->phandle);
               printf("\nReached MAX_DISCOVERY_RESULTS_IN_LIST. Cancelling inquiry - please wait ....\n");
           }
           else
           {
               hdpData->numInList++;
           }
       }
       CsrPmemFree(prim->info);
       break;
   }
   case CSR_BT_SD_CLOSE_SEARCH_IND:

      hdpData->state = selecting;
      printf("Select [Index] of Remote Device :\n");
      CsrSchedTimerSet(KEYB_CHECK_TIMEOUT, hdpKeyMessageService, 0, NULL);
      break;

   default:

      printf("####### default in SdPrim handler %x,\n",*primType);
      CsrBtSdFreeUpstreamMessageContents(CSR_BT_SD_PRIM, hdpData->recvMsgP);
      /* unexpected primitive received */

   }
}


/******************************************************************************
 * A keyboard event is received.
 * First check for selection of a found device during inquiry if
 * state is inquiry. If not start proper action as selected.
 *****************************************************************************/
void hdpHandleKeybPrim(CsrUint8 theChar, hdpDemoGlobalInstanceData_t *hdpData)
{
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

   switch(hdpData->state)
   {
   case idle :
       {
           switch(theChar)
           {
           case ' ':
               hdpdemoPlayIntro(hdpData);
               break;

           case '0':
               if (!hdpData->doneInquiry)
               {
                   hdpStartInquiry(hdpData);
               }
               break;

           case '1' :

               break;

           case '2' :

               printf("\n Select the Service record to advertise :");
               printf("\n 0) Source");
               printf("\n 1) Sink");
               printf("\n 2) 1 Sink, 1 Source ");
               printf("\n Select - 0, 1 or 2 :");
               hdpData->state = registering;

               break;

           case '3' :

               hdpGetCapab(hdpData);
               hdpData->state = getcapab;

               break;

           case '4' :         
           case '5' :         
           case '6' :
           case '7' :
           case '8' :
           default :

               printf("Bad selection, or not allowed right now, state: %d\n", hdpData->state);
               break;         
           }
       }
       break;

   case pairing:
       {
           handleScKeybPrim(&hdpData->scInstData);
       }
       break;

   case inquiry :
       {

           if ((theChar == 'c') || (theChar == 'C'))
           {
               hdpData->state= idle;
               printf("\nCancelling inquiry - please wait ....\n");
               CsrBtSdCancelSearchReqSend(hdpData->phandle);
           }
           else
           {
               /* Do nothing */
           }
       }
       break;

   case selecting:
       {

           if ((theChar >= '0') && (theChar <= '9'))
           {
               hdpData->selectedRemoteDeviceIdx = theChar - '0';
               printf("Selected Remote Device to Connect :");
               hdpPrintBDAddress(&hdpData->bdAddrList[hdpData->selectedRemoteDeviceIdx]);
               hdpData->state = registered;
               hdpdemoPlayIntro(hdpData);
           }
           else
           {
               printf("Invalid selection");
               printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
           }

       }
       break;
   case  registering :
       {
           printf("%c",theChar);

           if ((theChar >= '0') && (theChar <= '2'))
           {
               hdpData->role = theChar - '0';
               hdpStartRegister(hdpData);            
           }
           else
           {
               printf("\nInvalid selection - Select - 0, 1 or 2 :");
           }
       }
       break;

   case registered :
       {
           switch(theChar)
           {
           case '4' :
               CsrBtHdpActivateReqSend(hdpData->phandle,HDP_NUM_CONNECTIONS);
               break;
           case '0':

               if (!hdpData->doneInquiry)
               {
                   hdpStartInquiry(hdpData);
               }
               break;
           case '3' :

               hdpGetCapab(hdpData);
               hdpData->state = getcapab;
               break;
           case '5':

               printf("\nEnter the MDEP reference:");
               hdpData->state = connecting;
               break;

           case 'b':
           case 'B':

               CsrBtHdpDeactivateReqSend(hdpData->phandle);
               break;

           case 'C':
           case 'c':   
               hdpUnregister(hdpData);
               break;           
           case 'E':
           case 'e':
               hdpEchoTest(hdpData);
               hdpData->state = connecting;
               break;
           case 'M':
           case 'm':
               hdpBond(hdpData);
               break;
           default :

               printf("Bad selection, or not allowed right now, state: %d\n", hdpData->state);
               break;   
           }
       }
       break;

   case connecting :  
       {
           printf("%c",theChar);
           hdpData->idxAssocChId = theChar - '0';
           printf("\n connecting to ...");
           printf("\n Mdep id - [%d]", hdpData->mdepId[hdpData->idxAssocChId]);
           printf("\n Mdep DataType - [%d] ", hdpData->mdepDataType[hdpData->idxAssocChId]);
           printf("\n Mdep Role - [%d]", hdpData->mdepRole[hdpData->idxAssocChId]);
           hdpConnectAssociateChannel(hdpData,hdpData->idxAssocChId);
       }
       break;

   case connectable:
       {
           switch(theChar)
           {
           case 'M':
           case 'm':
               hdpBond(hdpData);
               break;
           case '3' :
               hdpGetCapab(hdpData);
               hdpData->state = getcapab;
               break;        
           case '4' :
               CsrBtHdpActivateReqSend(hdpData->phandle,HDP_NUM_CONNECTIONS);
               break;
           case '5':

               printf("\nEnter the MDEP reference:");
               hdpData->state = connecting;         
               break;

           case 'b' :
           case 'B':
               CsrBtHdpDeactivateReqSend(hdpData->phandle);
               break;
           case 'C':
           case 'c':   
               hdpUnregister(hdpData);
               break;           

           case 'E':
           case 'e':
               hdpEchoTest(hdpData);
               break;

           default :
               printf("Bad selection, or not allowed right now, state: %d\n", hdpData->state);
               break;   
           }
       }
       break;

   case getcapab:
       {
           switch(theChar)
           {

           case '5':

               printf("\nEnter the MDEP reference:");
               hdpData->state = connecting;
               break;

           case 'E':
           case 'e':
               hdpEchoTest(hdpData);
               break;
           case 'M':
           case 'm':
               hdpBond(hdpData);
               break;

           default:
               printf("Bad selection, or not allowed right now, state: %d\n", hdpData->state);
               break;   
           }
       }
       break;

   case connected:
       {
           switch(theChar)
           {
           case '5':

               printf("\nEnter the MDEP reference:");
               hdpData->state = connecting;
               break;

           case '6':
               hdpConnectVirtualChannel(hdpData);
               break;

           case '7':
               hdpDisconnectVirtualChannel(hdpData);
               break;

           case '8':

               printf(" \nEnter the Ch id to be disconnected :");
               hdpData->prevState = hdpData->state;
               hdpData->state = entryNum;
               hdpData->prevSelection = theChar;            

               break;

           case '9':

               printf(" \nEnter the Ch id for  data to be Tx on:");
               hdpData->prevState = hdpData->state;
               hdpData->state = entryNum;
               hdpData->prevSelection = theChar;

               break;

           case 'A':
           case 'a':
               printf(" \nEnter the ref id  of of the MDEP to be suspended:");
               hdpData->prevState = hdpData->state;
               hdpData->state = entryNum;
               hdpData->prevSelection = theChar;
               break;
           case 'C':
           case 'c':   
               hdpUnregister(hdpData);
               break;

           case 'b' :
           case 'B':
               CsrBtHdpDeactivateReqSend(hdpData->phandle);
               break;

           case 'E':
           case 'e':
               hdpEchoTest(hdpData);
               break;

           case 'f':
           case 'F':
               printf("\nEnsure that peer is in IEEE mode");
               printf("\nconfigure the IEEE Manager befoe IEEE Agent");
               printf("\n Select 1) IEEE Agent 2) IEEE Manager :");
               hdpData->prevState = hdpData->state;
               hdpData->state = entryNum;
               hdpData->prevSelection = theChar;            
               break;
           case 'g':
           case 'G':
               printf("\nEnsure that peer is in IEEE mode");
               printf("\nconfigure the IEEE Manager befoe IEEE Agent");
               printf("\n Select 1) IEEE Agent 2) IEEE Manager :");
               hdpData->prevState = hdpData->state;
               hdpData->state = entryNum;
               hdpData->prevSelection = theChar;            
               break;
           default:
               printf("Bad selection, or not allowed right now, state: %d\n", hdpData->state);
               break;  
           }
       }
       break;

   case suspend:   
       {
           switch(theChar) 
           {
           case '9':
               hdpDataSend(hdpData);
               break;    
           case 'd':
           case 'D':
               printf("\nEnter the MDEP reference:");
               hdpData->state = entryNum;
               hdpData->prevState = suspend;
               hdpData->prevSelection = theChar;            
               break;
           case 'B':
           case 'b':   
               hdpUnregister(hdpData);
               break;           
           default:
               printf("Bad selection, or not allowed right now, state: %d\n", hdpData->state);
               break; 
           }
       }
       break;

   case entryNum :
       {
           hdpData->idxAssocChId = theChar - '0';
           hdpData->state = hdpData->prevState;
           printf("%c" ,theChar);
           switch(hdpData->prevSelection)
           {
           case '9':
               hdpDataSend(hdpData); 
               break;

           case '8':
               hdpDisconnectAssociateChannel(hdpData);
               break;

           case 'A':
           case 'a':
               hdpSuspend(hdpData);
               hdpData->state = suspend;
               break;

           case 'd':
           case 'D':
               hdpResumeReqSend(hdpData);
               break;
           case 'f':
           case 'F': 
           case 'g':
           case 'G':                 
               if (theChar == '1')
               {
                   hdpData->state = ieee_agent;
                   printf("\n Enter the channel ref for IEEE test :");
               }
               else if (theChar == '2')
               {
                   hdpData->state = ieee_manager;
                   printf("\n Enter the channel ref for IEEE test :");
               }
               else
               {
                   hdpData->state = entryNum;
                   printf("\n Invalid selection - Select - 1 or 2 :");
               }
               break;               
           default:
               printf("Bad selection, or not allowed right now, state: %d\n", hdpData->state);
               break;              
           }
       }
       break;

   case ieee_agent:
       {
           printf("%c" ,theChar);
           hdpData->idxAssocChId = theChar - '0';        
           if (hdpData->prevSelection == 'f' ||
               hdpData->prevSelection == 'F' )
           {
               hdpData->devConfigId = 0;
           }
           hdpData->ieeeState = associate;
           hdpdemoSendAarqApdu(hdpData);

       }
       break;

   case ieee_manager:
       {
           printf("%c\n" ,theChar);        
           hdpData->idxAssocChId = theChar - '0';        
           if (hdpData->prevSelection == 'f' ||
               hdpData->prevSelection == 'F' )
           {
               hdpData->devConfigId = 0;
           }        
           hdpData->ieeeState = associate;
       }
       break;    

   default :
       printf("Bad selection, or not allowed right now, state: %d\n", hdpData->state);
       break;
   }
}

void applicationUsage(char *col, char *des, char *opt)
{
}

char* applicationCmdLineParse(int optionChar, char *optionArgument)
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

