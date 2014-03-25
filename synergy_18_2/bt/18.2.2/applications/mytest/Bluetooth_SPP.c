/****************************************************************************

(c) Cambridge Silicon Radio Limited 2009

All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_synergy.h"

#include <string.h>
#include "csr_util.h"
#include "csr_pmem.h"
#include "csr_exceptionhandler.h"

#include "csr_app_lib.h"

#include "csr_bt_platform.h"
#include "csr_bt_gap_app_lib.h"
#include "csr_bt_util.h"
#include "csr_bt_spp_lib.h"
#include "Bluetooth_SPP.h"
#include "Bluetooth_Common.h"

static char * csrBtSppAppReturnSppInstName(CsrUint16 instance)
{
    char *buf;
    buf = (char *)CsrPmemAlloc(CsrStrLen("SPP instance xxxx")+1);

    //CsrBtSprintf(buf, "SPP instance %04X\n", instance);
    return buf;
}


/******************************************************************************
CsrBtSppAppStartActivate:
*****************************************************************************/
void CsrBtSppAppStartActivate(CsrSppAppGlobalInst *sppData)
{
    CsrBtSppAppInstData    *ptr;

    ptr = &sppData->sppInstance;
	CL_API_LOGPrintf("send CsrBtSppAppStartActivate \n");

    ptr->serverOrClient        = SERVER;
    CsrBtSppActivateReqSend(sppData->queueId, sppData->phandle, MAX_ACTIVATE_TIME, CSR_BT_SPP_SERVER_ROLE, (CsrCharString *) CsrStrDup(ptr->comPortName));
}

/******************************************************************************
CsrBtSppAppStartConnecting:
*****************************************************************************/
void CsrBtSppAppStartConnecting(CsrSppAppGlobalInst *sppData)
{
    CsrBtSppAppInstData    *ptr;

    ptr        = &sppData->sppInstance;

    ptr->serverOrClient     = CLIENT;
    CsrBtSppConnectReqSend(sppData->queueId,
        sppData->phandle,
        (sppData->selectedDeviceAddr),
        FALSE,
        NULL,
        CSR_BT_SPP_CLIENT_ROLE);
}

/******************************************************************************
CsrBtSppAppStartDisconnect:
*****************************************************************************/
void CsrBtSppAppStartDisconnect(CsrSppAppGlobalInst *sppData)
{
    CsrBtSppAppInstData    *ptr;
    ptr = &sppData->sppInstance;

    CsrBtSppDisconnectReqSend(sppData->queueId, ptr->serverChannel);
}

/******************************************************************************
CsrBtSppAppStartDeactivate:
*****************************************************************************/
void CsrBtSppAppStartDeactivate(CsrSppAppGlobalInst *sppData)
{
    CsrBtSppDeactivateReqSend(sppData->queueId, sppData->phandle);
}

/******************************************************************************
CsrBtSppAppSendingTheSelectedFile:
*****************************************************************************/
void CsrBtSppAppSendingTheSelectedFile(CsrSppAppGlobalInst *sppData, CsrBtSppAppInstData *instData)
{
    CsrUint32                bytesToRead;

    if( (instData->currentFileSize - instData->currentFilePosition) <= instData->framesize)
    {
        bytesToRead = instData->currentFileSize - instData->currentFilePosition;
    }
    else
    {
        bytesToRead = instData->framesize;
    }

    if(bytesToRead > 0)
    {
        CsrUint16                bytesRead;
        CsrUint8                    * body;

        body = (CsrUint8 *) CsrPmemAlloc(bytesToRead);
        bytesRead = CsrBtFread(body, 1, bytesToRead, instData->currentFileHandle);
        instData->currentFilePosition += bytesRead;
        CsrBtSppDataReqSend(sppData->queueId, instData->serverChannel, bytesRead, body);

        CL_API_LOGPrintf("Sending %d bytes of %d, Sent %d ; remaining: %d",
            bytesRead,
            instData->currentFileSize,
            instData->currentFilePosition,
            instData->currentFileSize - instData->currentFilePosition);
    }
    if(instData->currentFilePosition >= instData->currentFileSize)
    {
        CsrBtFclose(instData->currentFileHandle);
        CL_API_LOGPrintf( "File transfer operation finished successfully. %d bytes sent",instData->currentFilePosition);
        instData->currentFilePosition    = 0;
        instData->currentFileHandle        = NULL;
        instData->CsrBtSppAppSending                = FALSE;

     //   CsrBtSppAppHandleSppCsrUiSendFile(sppData, FALSE);
    }
}

/******************************************************************************
CsrBtSppAppOpenTheSelectedFile:
*****************************************************************************/
void CsrBtSppAppOpenTheSelectedFile(CsrSppAppGlobalInst *sppData)
{
    CsrBtSppAppInstData    *ptr;

    ptr = &sppData->sppInstance;

    ptr->currentFilePosition = 0;

    if (ptr->currentFileHandle == NULL)
    {
        int        result;
        CsrBtStat_t statBuffer;

        CsrMemSet(&statBuffer, 0, sizeof(CsrBtStat_t));
        statBuffer.st_size    = 0;
        result                = CsrBtStat(ptr->fileName, &statBuffer);
        if(!result)
        {
            if((ptr->currentFileHandle = CsrBtFopen(ptr->fileName, "rb")) == NULL)
            {
                CL_API_LOGPrintf( "Fail. could not open the file %s", ptr->fileName);
            }
            else
            {
                ptr->currentFileSize = statBuffer.st_size;
                ptr->CsrBtSppAppSending         = TRUE;
                CsrBtSppAppSendingTheSelectedFile(sppData, ptr);

                CL_API_LOGPrintf( "Starting transfering the file %s", ptr->fileName);
            }
        }
        else
        {
            CL_API_LOGPrintf( "Fail. could not open the file %s", ptr->fileName);
        }
    }
    else
    {
        if(ptr->currentFileHandle != NULL)
        {
            CsrBtFclose(ptr->currentFileHandle);
            ptr->currentFileHandle    = NULL;
        }
        CL_API_LOGPrintf( "Fail.currentFileHandle is not NULL");
    }
}

/******************************************************************************
CsrBtSppAppSendingNextPieceofFile:
*****************************************************************************/
void CsrBtSppAppSendingNextPieceofFile(CsrSppAppGlobalInst *sppData, CsrBtSppAppInstData *instData)
{
    if(instData->currentFileHandle != NULL)
    {
        CsrBtSppAppSendingTheSelectedFile(sppData, instData);
    }
    else
    {
        instData->currentFilePosition = 0;
    }
}


/******************************************************************************
CsrBtSppAppCancelFileTransfer:
*****************************************************************************/
void CsrBtSppAppCancelFileTransfer(CsrSppAppGlobalInst *sppData)
{
    CsrBtSppAppInstData    *ptr;

    ptr = &sppData->sppInstance;

    if(ptr->currentFileHandle != NULL)
    {
        CsrBtFclose(ptr->currentFileHandle);
        ptr->currentFilePosition    = 0;
        ptr->currentFileHandle        = NULL;
        ptr->CsrBtSppAppSending                = FALSE;
        CL_API_LOGPrintf( "The file transferring for file %s is cancelled", ptr->fileName);

    }
    else
    {
        CL_API_LOGPrintf( "The file transferring could not be cancelled");
    }
}


/******************************************************************************
csrBtSppAppTimeOut:
*****************************************************************************/
static void csrBtSppAppTimeOut(CsrSchedQid queueId, CsrSppAppGlobalInst *sppData)
{
    CsrBtSppAppInstData *instData;
    double avgSpeed;
    double elapsed;
    double wrapAroundTime;
    double totalTime;

    instData = &sppData->sppInstance;
    instData->timerFlag        = FALSE;
    if(instData->writingFileHandle != NULL)
    {
        CsrBtFclose(instData->writingFileHandle);
        instData->writingFileHandle    = NULL;

        elapsed = instData->lastTime - instData->CsrBtSppAppStartTime;
        wrapAroundTime = (double) instData->nWrapArounds * WRAPAROUND_TIME_SEC;
        totalTime = wrapAroundTime + elapsed/1000000;
        if(instData->byteCounter > 0 && (elapsed > 0 || wrapAroundTime > 0))
        {
            avgSpeed = instData->byteCounter * 8.0 / (totalTime * 1000);
            CL_API_LOGPrintf( "%s has been copied to disk.", instData->outputFileName);
            CL_API_LOGPrintf( "The transfer speed was %f Kbits/Sec.", avgSpeed);
            CL_API_LOGPrintf( "Amount of bytes transferred is  %u Kbytes", instData->byteCounter/1024);
            instData->byteCounter    = 0;
            instData->nWrapArounds   = 0;
        }
    }
}

/******************************************************************************
CsrBtSppAppDataIndHandler:
*****************************************************************************/
void CsrBtSppAppDataIndHandler(CsrSppAppGlobalInst *sppData)
{
    CsrBtSppDataInd            *myPrim;
    CsrBtSppAppInstData    *instData;
    TIME                      currentTime;

    myPrim = (CsrBtSppDataInd *) sppData->recvMsgP;

    currentTime = CsrTimeGet(NULL);

    instData = &sppData->sppInstance;
    if (instData == NULL)
    {
        CsrGeneralException("CSR_BT_SPP_APP", 0, 0,
            "No instData return when receiving a CSR_BT_SPP_DATA_IND exit demo");
    }
    CsrBtSppDataResSend(sppData->queueId, instData->serverChannel);

    if(instData->writingFileHandle == NULL)
    {
        if((instData->writingFileHandle = CsrBtFopen(instData->outputFileName, "wb")) == NULL)
        {
            CL_API_LOGPrintf( "Fail. could not open the file for writing");
        }
        else
        {
            instData->byteCounter    = myPrim->payloadLength;
            instData->CsrBtSppAppStartTime        = currentTime;
            instData->timerFlag        = TRUE;
            instData->timeId        = CsrSchedTimerSet(TIME_WITH_NO_DATA, (void (*) (CsrUint16, void *)) csrBtSppAppTimeOut, (CsrSchedQid) sppData->queueId, (void *) sppData);
            CsrBtFwrite(myPrim->payload, sizeof(CsrUint8), myPrim->payloadLength, instData->writingFileHandle);

            CL_API_LOGPrintf( "Data transfer initated from peer");
        }
    }
    else
    {
        CsrSchedTimerCancel(instData->timeId,NULL,NULL);
        instData->byteCounter    = myPrim->payloadLength + instData->byteCounter;
        instData->timerFlag        = TRUE;
        instData->timeId        = CsrSchedTimerSet(TIME_WITH_NO_DATA, (void (*) (CsrUint16, void *)) csrBtSppAppTimeOut, (CsrSchedQid) sppData->queueId, (void *) sppData);
        CsrBtFwrite(myPrim->payload, sizeof(CsrUint8), myPrim->payloadLength, instData->writingFileHandle);

        if (instData->lastTime > currentTime)
        {
            ++instData->nWrapArounds;
        }
    }
    instData->lastTime    = currentTime;
    CsrPmemFree(myPrim->payload);
}



/******************************************************************************
CsrBtSppAppInitInstanceData:
*****************************************************************************/
void CsrBtSppAppInitInstanceData(CsrSppAppGlobalInst *sppData)
{
    CsrMemSet(sppData,0,sizeof(CsrSppAppGlobalInst));

    sppData->sppInstance.serverChannel             = 0xff;
}

/******************************************************************************
* Init function called by the scheduler upon initialisation.
* This function is used to boot the demo application.
*****************************************************************************/
void init_spp(void **gash)
{

    CsrSppAppGlobalInst    *sppData;

    /* allocate and initialise instance data space */
    *gash = (void *) CsrPmemAlloc(sizeof(CsrSppAppGlobalInst));
    sppData = (CsrSppAppGlobalInst *) *gash;
    CsrBtSppAppInitInstanceData(sppData);

    sppData->phandle = CsrSchedTaskQueueGet() ;

    sppData->initialized = FALSE;


    CsrBtSppGetInstancesQidReqSend(sppData->phandle);

}

#ifdef ENABLE_SHUTDOWN
void deinit_spp(void **gash)
{
    CsrUint16 msg_type=0;
    void *msg_data=NULL;
    CsrSppAppGlobalInst *sppData;

    sppData = (CsrSppAppGlobalInst *) (*gash);
    /* get a message from the demoapplication message queue. The message is returned in prim
    and the event type in eventType */
    while ( CsrSchedMessageGet(&msg_type, &msg_data) )
    {
        switch (msg_type)
        {
        case CSR_BT_SPP_PRIM:
            CsrBtSppFreeUpstreamMessageContents(msg_type, msg_data);
            break;
        }
        CsrPmemFree(msg_data);
    }

    if (sppData->sppInstance.timerFlag)
    {
        CsrSchedTimerCancel(sppData->sppInstance.timeId, NULL, NULL);
    }

    if (sppData->sppInstance.currentFileHandle != NULL)
    {
        CsrBtFclose(sppData->sppInstance.currentFileHandle);
    }

    if (sppData->sppInstance.writingFileHandle != NULL)
    {
        CsrBtFclose(sppData->sppInstance.writingFileHandle);
    }

    if (sppData->serviceNameList)
    {
        CsrPmemFree(sppData->serviceNameList);
        sppData->serviceNameList = NULL;
    }

    if (sppData->profileName)
    {
        CsrPmemFree(sppData->profileName);
        sppData->profileName = NULL;
    }

    CsrPmemFree(sppData->appHandles);

    CsrPmemFree(sppData);
}
#else
void CsrBtSppAppDeinit(void **gash)
{
    CSR_UNUSED(gash);
}
#endif



/******************************************************************************
* Function to handle all spp-primitives.
******************************************************************************/
void handleSppPrim(CsrSppAppGlobalInst *sppData)
{
    CsrPrim *primType;
    CsrBtSppAppInstData *instData;

    primType = (CsrPrim *) sppData->recvMsgP;
    switch (*primType)
    {
        case CSR_BT_SPP_ACTIVATE_CFM:
        {
            instData = &sppData->sppInstance;

 //           CsrBtSppAppHandleSppCsrUiActivateDeactivate(sppData, TRUE);
            CL_API_LOGPrintf( "CSR_BT_SPP_ACTIVATE_CFM\n");
            break;
        }
        case CSR_BT_SPP_CONNECT_IND:
        {
            CsrBtSppConnectInd        *myPrim;
            CsrUint8    modemSetting    = 0;
            CsrUint8 breakSignal        = 0;

            myPrim = (CsrBtSppConnectInd *) sppData->recvMsgP;
            instData = &sppData->sppInstance;
            instData->serverChannel = myPrim->serverChannel;

            if (myPrim->resultSupplier == CSR_BT_SUPPLIER_SPP &&
                myPrim->resultCode == CSR_BT_RESULT_CODE_SPP_SUCCESS)
            {
                instData->framesize        = myPrim->profileMaxFrameSize;
                instData->CsrBtSppAppSending        = FALSE;
                if(instData->serverOrClient == CLIENT)
                {
                    if(CSR_BT_SPP_CLIENT_ROLE == CSR_BT_DTE)
                    {
                        modemSetting = CSR_BT_MODEM_RTS_MASK | CSR_BT_MODEM_DTR_MASK;
                    }
                    else
                    {
                        modemSetting = CSR_BT_MODEM_DSR_MASK | CSR_BT_MODEM_CTS_MASK;
                    }
                }
                else
                {
                    if(CSR_BT_SPP_SERVER_ROLE == CSR_BT_DCE)
                    {
                        modemSetting = CSR_BT_MODEM_DSR_MASK | CSR_BT_MODEM_CTS_MASK;
                    }
                    else
                    {
                        modemSetting = CSR_BT_MODEM_RTS_MASK | CSR_BT_MODEM_DTR_MASK;
                    }
                }
                CsrBtSppControlReqSend(sppData->queueId, instData->serverChannel, modemSetting, breakSignal);

                CL_API_LOGPrintf( "Successfully Connected with: %04X:%02X:%06X\n", myPrim->deviceAddr.nap, myPrim->deviceAddr.uap, myPrim->deviceAddr.lap);
            }
            else
            {
                CL_API_LOGPrintf( "Failed to connect with: %04X:%02X:%06X, resultCode=%d, resultSupplier=%d\n", myPrim->deviceAddr.nap, myPrim->deviceAddr.uap, myPrim->deviceAddr.lap, myPrim->resultCode, myPrim->resultSupplier);
            }
//            CsrBtSppAppHandleSppCsrUiConnectDisconnect(sppData, TRUE,(CsrBool)((myPrim->resultCode == CSR_BT_RESULT_CODE_SPP_SUCCESS && myPrim->resultSupplier == CSR_BT_SUPPLIER_SPP) ? TRUE : FALSE));
            break;
        }
        case CSR_BT_SPP_DEACTIVATE_CFM:
        {
            instData = &sppData->sppInstance;

//            CsrBtSppAppHandleSppCsrUiActivateDeactivate(sppData, FALSE);
            CL_API_LOGPrintf("CSR_BT_SPP_DEACTIVATE_CFM\n");
            break;
        }
        case CSR_BT_SPP_DATA_IND:
        {
            CsrBtSppAppDataIndHandler(sppData);
            break;
        }
        case CSR_BT_SPP_DATA_CFM:
        {
            instData = &sppData->sppInstance;

            CsrBtSppAppSendingNextPieceofFile(sppData, instData);
            break;
        }
        case CSR_BT_SPP_CONTROL_IND:
        {
            break;
        }
        case CSR_BT_SPP_DISCONNECT_IND:
        {
            CsrBtSppDisconnectInd    *myPrim;

            myPrim = (CsrBtSppDisconnectInd *) sppData->recvMsgP;
            instData = &sppData->sppInstance;

            instData->CsrBtSppAppSending        = FALSE;
            instData->byteCounter    = 0;
            if(instData->currentFileHandle != NULL)
            {
                CsrBtFclose(instData->currentFileHandle);
                instData->currentFileHandle = NULL;
            }
            if(instData->writingFileHandle != NULL)
            {
                CsrSchedTimerCancel(instData->timeId,NULL,NULL);
                instData->timerFlag        = FALSE;
                CsrBtFclose(instData->writingFileHandle);
                instData->writingFileHandle    = NULL;
            }

            CL_API_LOGPrintf( "Disconnected from: %04X:%02X:%06X\n", myPrim->deviceAddr.nap, myPrim->deviceAddr.uap, myPrim->deviceAddr.lap);
//            CsrBtSppAppHandleSppCsrUiConnectDisconnect(sppData, FALSE, TRUE);

            break;
        }
        case CSR_BT_SPP_PORTNEG_IND:
        {
            RFC_PORTNEG_VALUES_T m;
            CsrBtSppPortnegInd *myPrim;

            myPrim = (CsrBtSppPortnegInd *) sppData->recvMsgP;
            CsrBtPortParDefault(&m);
            CsrBtSppPortnegResSend(myPrim->queueId, myPrim->serverChannel, &m);
            break;
        }
        case CSR_BT_SPP_SERVICE_NAME_IND :
        {
            CsrBtSppServiceNameInd    *myPrim;

            myPrim        = (CsrBtSppServiceNameInd *) sppData->recvMsgP;
            sppData->serviceNameList        = myPrim->serviceNameList;
            sppData->serviceNameListSize    = myPrim->serviceNameListSize;

//            CsrBtSppAppHandleSppCsrUiServiceNameList(sppData, myPrim->serviceNameListSize, myPrim->serviceNameList);

            break;
        }

        case CSR_BT_SPP_GET_INSTANCES_QID_CFM:
        {
            CsrBtSppGetInstancesQidCfm *myPrim;
            CsrUint8 i;

            myPrim = (CsrBtSppGetInstancesQidCfm *) sppData->recvMsgP;

            sppData->numberOfSppInstances = myPrim->phandlesListSize;

            CsrPmemFree(sppData->appHandles);
            sppData->appHandles = (CsrBtSppAppProfileHandleList *)CsrPmemAlloc(sizeof(CsrBtSppAppProfileHandleList) * sppData->numberOfSppInstances);

            for(i=0;i<sppData->numberOfSppInstances;i++)
            {
                sppData->appHandles[i].sppAppHandle = CSR_SCHED_QID_INVALID;
                sppData->appHandles[i].sppProfileHandle = myPrim->phandlesList[i];
            }

            CsrPmemFree(myPrim->phandlesList);

            sppData->queueId = sppData->appHandles[0].sppProfileHandle;
            sppData->appHandles[0].sppAppHandle = sppData->phandle;

            sppData->profileName = csrBtSppAppReturnSppInstName(0);
            CL_API_LOGPrintf( (const char *)PROFILE_NAME(sppData->profileName));
            //CsrBtSprintf(sppData->sppInstance.comPortName,"COM%d\n",0);
            //CsrBtSprintf(sppData->sppInstance.outputFileName,"CSR_BT_SPP%02d.txt\n",0);

//            CsrBtSppAppHandleSppSetMainMenuHeader(sppData);

            sppData->initialized = TRUE;
  			CsrBtSppAppStartActivate(sppData);

  //          csrBtSppAppSendSppAppHouseCleaning(sppData);

            break;
        }

        case CSR_BT_SPP_STATUS_IND:
        {
            CsrBtSppStatusInd *myPrim;

            myPrim  = (CsrBtSppStatusInd*)sppData->recvMsgP;

            if(myPrim->connect)
            {
                CL_API_LOGPrintf( "Data status indication: Connect\n");
            }
            else
            {
                CL_API_LOGPrintf( "Data status indication: Disconnect\n");
            }
            break;
        }

        case CSR_BT_SPP_REGISTER_DATA_PATH_HANDLE_CFM:
        {
            CL_API_LOGPrintf( "Data path handle registered\n");
            break;
        }

        case CSR_BT_SPP_DATA_PATH_STATUS_IND:
        {
            CsrBtSppDataPathStatusInd *prim;
            char *events[4] = { "Opened", "Closed", "Lost", "Undefined" };
            char *what;

            prim = (CsrBtSppDataPathStatusInd*)sppData->recvMsgP;
            if(prim->status <= 2)
            {
                what = events[prim->status];
            }
            else
            {
                what = events[3];
            }
            CL_API_LOGPrintf( "Data path application status indication: %s\n", what);
            break;
        }

        default:
        {
            CL_API_LOGPrintf( "####### default in spp prim handler 0x%04x,\n",
                *primType);
            CsrBtSppFreeUpstreamMessageContents(CSR_BT_SPP_PRIM, sppData->recvMsgP);
            /* unexpected primitive received */
        }
    }
}

