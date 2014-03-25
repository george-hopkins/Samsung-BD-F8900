/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "bluetooth.h"
#include "csr_bt_ss_prim.h"
#include "csr_util.h"
#include "peer_com_linux_u2u.h"
#include "AppAVHandler.h"
#include "csr_pmem.h"
#include "csr_samsung_log.h"


//#define EXTERNAL_DEBUG_PCM

typedef struct CsrBtAppAVDataStructType
{
   CsrUint16 pcm_sampling_rate;
   CsrUint8  pcm_no_channels;
#ifdef EXTERNAL_DEBUG_PCM
   FILE      *localPCMFile;
#endif
}CsrBtAppAVDataType; 



typedef void (* csrBtAvAppStateHandlerType)(csr_api_av_prim_type message, void* data, CsrUint16 length);



static CsrBtAppAVEventHandlerStruct * CsrBtAvAppEventFunctionsPtr=NULL;   
static CsrBtAppAVDataType avInstnceData;
static BD_ADDR_T 		 deviceAvAddrNull; 



/////////  AV CMD /////////////

CsrInt16 csr_av_active_cmd(CsrBtAppAVEventHandlerStruct * BtAvCallbackFunctionsPtr )
{
  DBG_SS_PRINT("csr_av_active_cmd\n");

   if(BtAvCallbackFunctionsPtr == NULL)
	{
		return FALSE;
	}
   CsrBtAvAppEventFunctionsPtr = BtAvCallbackFunctionsPtr;
   

  
  return  csr_bt_client_app_put_message_to_tx_thread(CSR_AV_SS_ID,AV_ACTIVATATE_REQ,NULL, 0 );
}

CsrInt16 csr_av_deactvie_cmd(void )
{
   DBG_SS_PRINT("csr_av_deactvie_cmd ___ss\n");

   
  return csr_bt_client_app_put_message_to_tx_thread(CSR_AV_SS_ID,AV_DEACTIVATATE_REQ,NULL, 0 );
}

CsrInt16 csr_av_connect_cmd(BD_ADDR_T bdaddr )
{
   void * data= CsrPmemAlloc(sizeof(BD_ADDR_T));

   CsrMemCpy(data,&bdaddr, sizeof(BD_ADDR_T));
   
   DBG_SS_PRINT("csr_av_connect_cmd ___ss\n");

    if(CsrMemCmp(&(bdaddr), &(deviceAvAddrNull),sizeof(BD_ADDR_T))==0)
		return CSR_ERR_CLI_BAD_PARAM;
  return csr_bt_client_app_put_message_to_tx_thread(CSR_AV_SS_ID,AV_CONNECT_REQ,data, sizeof(BD_ADDR_T) );
}


CsrInt16 csr_av_disconnect_cmd(BD_ADDR_T bdaddr )
{
   DBG_SS_PRINT("csr_av_disconnect_cmd ___ss");

   
  return csr_bt_client_app_put_message_to_tx_thread(CSR_AV_SS_ID,AV_DISCONNECT_REQ,NULL, 0 );
}

CsrInt16 csr_av_conneting_cancel_cmd(BD_ADDR_T bdaddr )
{

   void * data= CsrPmemAlloc(sizeof(BD_ADDR_T));

   CsrMemCpy(data,&bdaddr, sizeof(BD_ADDR_T));

   DBG_SS_PRINT("csr_av_connecting_cancel_cmd ___ss");

   if(CsrMemCmp(&(bdaddr), &(deviceAvAddrNull),sizeof(BD_ADDR_T))==0)
		return CSR_ERR_CLI_BAD_PARAM;
   
  return csr_bt_client_app_put_message_to_tx_thread(CSR_AV_SS_ID,AV_CONNECTING_CANCEL_REQ,data,  sizeof(BD_ADDR_T) );
}



/////////  AV EVENT /////////////

void csrBtAvActivateCfmHandler(  csr_api_av_prim_type message, void* data, CsrUint16 length)
{
    if(CsrBtAvAppEventFunctionsPtr->csrBtAvAtivateCfmFunc == NULL)
    {
        DBG_SS_PRINT("csrBtAvActivateCfmHandler Event function not defined\n");
    }
    else
    {
        CsrBtAvAppEventFunctionsPtr->csrBtAvAtivateCfmFunc();
    }
}

static void csrBtAvDeActivateCfmHandler(  csr_api_av_prim_type message, void* data, CsrUint16 length)
{


    if(CsrBtAvAppEventFunctionsPtr->csrBtAvDeActivateCfmFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAvDeActivateCfmHandler Event function not defined\n");
    }
    else
    {
        CsrBtAvAppEventFunctionsPtr->csrBtAvDeActivateCfmFunc();
    }
}

static void csrBtAvConnectIndHandler(  csr_api_av_prim_type message, void* data, CsrUint16 length)
{


    if(CsrBtAvAppEventFunctionsPtr->csrBtAvConnectIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAvConnectIndFunc Event function not defined\n");
    }
    else
    {
 //      BD_ADDR_T bdaddr;
#ifdef EXTERNAL_DEBUG_PCM    
        avInstnceData.localPCMFile=fopen("/mtd_rwarea/pcm_dump.wav", "wb");
#endif
 //        CsrMemCpy(&(bdaddr),data,sizeof(BD_ADDR_T));
        
        CsrBtAvAppEventFunctionsPtr->csrBtAvConnectIndFunc(*((BD_ADDR_T*)data));
    }
}
                                            

static void csrBtAvConfigIndHandler(  csr_api_av_prim_type message, void* data, CsrUint16 length)
{
   

    if(CsrBtAvAppEventFunctionsPtr->csrBtAvConfigIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAvConfigIndFunc Event function not defined\n");
    }
    else
    {
      
		CsrMemCpy(&(avInstnceData.pcm_sampling_rate), data, sizeof(CsrUint16));
		CsrMemCpy(&(avInstnceData.pcm_no_channels), data + sizeof(CsrUint16), sizeof(CsrUint8));
		CsrBtAvAppEventFunctionsPtr->csrBtAvConfigIndFunc(avInstnceData.pcm_sampling_rate,avInstnceData.pcm_no_channels);
	}
}
static void csrBtAvConfigCfmHandler(  csr_api_av_prim_type message, void* data, CsrUint16 length)
{
   

    if(CsrBtAvAppEventFunctionsPtr->csrBtAvConfigCfmFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAvConfigIndFunc Event function not defined\n");
    }
    else
    {
      
		CsrMemCpy(&(avInstnceData.pcm_sampling_rate), data, sizeof(CsrUint16));
		CsrMemCpy(&(avInstnceData.pcm_no_channels), data + sizeof(CsrUint16), sizeof(CsrUint8));
		CsrBtAvAppEventFunctionsPtr->csrBtAvConfigCfmFunc(avInstnceData.pcm_sampling_rate,avInstnceData.pcm_no_channels);
	}
}



static void csrBtAvPcmStreamDataIndHandler(  csr_api_av_prim_type message, void* data, CsrUint16 length)
{
    if(CsrBtAvAppEventFunctionsPtr->csrBtAvPcmStreamDataIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAvPcmStreamDataIndHandler Event function not defined\n");
    }
    else
    {
#ifdef EXTERNAL_DEBUG_PCM 
      {
		fwrite(data,length,1,avInstnceData.localPCMFile);
       }
        
#endif
        CsrBtAvAppEventFunctionsPtr->csrBtAvPcmStreamDataIndFunc(data, length, avInstnceData.pcm_sampling_rate, avInstnceData.pcm_no_channels);
    }
}

static void csrBtAvCloseIndHandler(  csr_api_av_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtAvAppEventFunctionsPtr->csrBtAvCloseIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAvCloseIndHandler Event function not defined\n");
    }
    else
    {
#ifdef EXTERNAL_DEBUG_PCM      
        fclose(avInstnceData.localPCMFile);
#endif
        CsrBtAvAppEventFunctionsPtr->csrBtAvCloseIndFunc();
    }
}

static void csrBtAvCloseCfmHandler(  csr_api_av_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtAvAppEventFunctionsPtr->csrBtAvCloseCfmFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAvCloseCfmHandler Event function not defined\n");
    }
    else
    {
   //    CsrBool result;
#ifdef EXTERNAL_DEBUG_PCM      
        fclose(avInstnceData.localPCMFile);
#endif
     //   CsrMemCpy(&result,data,sizeof(CsrBool));
        CsrBtAvAppEventFunctionsPtr->csrBtAvCloseCfmFunc(*((CsrBool *)data));
    }
}


static void csrBtAvConnectCfmHandler(  csr_api_av_prim_type message, void* data, CsrUint16 length)
{


    if(CsrBtAvAppEventFunctionsPtr->csrBtAvConnectCfmFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAvConnectIndFunc Event function not defined\n");
    }
    else
    {
		BD_ADDR_T Bdaddr;
        CsrBtResultCode 	   avResultCode;
		CsrBtSupplier			avResultSupplier;
		
#ifdef EXTERNAL_DEBUG_PCM    
				avInstnceData.localPCMFile=fopen("/mtd_rwarea/pcm_dump.wav", "wb");
#endif

		CsrMemCpy(&Bdaddr,data,sizeof(BD_ADDR_T));
		CsrMemCpy(&avResultCode,data+sizeof(BD_ADDR_T),sizeof(CsrBtResultCode));
		CsrMemCpy(&avResultSupplier,data+sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode),sizeof(CsrBtSupplier));



		CsrBtAvAppEventFunctionsPtr->csrBtAvConnectCfmFunc(Bdaddr,avResultCode,avResultSupplier);
    }
}

static void csrBtAvDisconnectIndHandler(  csr_api_av_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtAvAppEventFunctionsPtr->csrBtAvDisconectIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAvCloseIndHandler Event function not defined\n");
    }
    else
    {
       BD_ADDR_T  Bdaddr;
	   CsrUint16 reasonCode;
	   CsrUint16 reasonSupplier;
	   
#ifdef EXTERNAL_DEBUG_PCM      
        fclose(avInstnceData.localPCMFile);
#endif
        CsrMemCpy(&Bdaddr,data,sizeof(BD_ADDR_T));
		CsrMemCpy(&reasonCode,data+sizeof(BD_ADDR_T),sizeof(CsrUint16));
		CsrMemCpy(&reasonSupplier,data+sizeof(BD_ADDR_T)+sizeof(CsrUint16),sizeof(CsrUint16));

        CsrBtAvAppEventFunctionsPtr->csrBtAvDisconectIndFunc(Bdaddr,reasonCode,reasonSupplier);
    }
}


static void csrBtAvOpenIndHandler(  csr_api_av_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtAvAppEventFunctionsPtr->csrBtAvOpenIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAvOpenIndFunc Event function not defined\n");
    }
    else
    {

		CsrUint8 filter_supported;

		CsrMemCpy(&filter_supported,data,sizeof(filter_supported));


        CsrBtAvAppEventFunctionsPtr->csrBtAvOpenIndFunc(filter_supported);
    }
}

static void csrBtAvOpenCfmHandler(  csr_api_av_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtAvAppEventFunctionsPtr->csrBtAvOpenCfmFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAvOpenCfmFunc Event function not defined\n");
    }
    else
    {

		CsrUint8 filter_supported;

		CsrMemCpy(&filter_supported,data,sizeof(filter_supported));


        CsrBtAvAppEventFunctionsPtr->csrBtAvOpenCfmFunc(filter_supported);
    }
}

static void csrBtAvDisconnectCfmHandler(  csr_api_av_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtAvAppEventFunctionsPtr->csrBtAvDisconectCfmFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAvCloseIndHandler Event function not defined\n");
    }
    else
    {
       BD_ADDR_T  Bdaddr;
	   CsrUint16 reasonCode;
	   CsrUint16 reasonSupplier;
	   
#ifdef EXTERNAL_DEBUG_PCM      
        fclose(avInstnceData.localPCMFile);
#endif
        CsrMemCpy(&Bdaddr,data,sizeof(BD_ADDR_T));
		CsrMemCpy(&reasonCode,data+sizeof(BD_ADDR_T),sizeof(CsrUint16));
		CsrMemCpy(&reasonSupplier,data+sizeof(BD_ADDR_T)+sizeof(CsrUint16),sizeof(CsrUint16));
        CsrBtAvAppEventFunctionsPtr->csrBtAvDisconectCfmFunc(Bdaddr,reasonCode,reasonSupplier);
    }
}


static void csrBtAvStartIndHandler(  csr_api_av_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtAvAppEventFunctionsPtr->csrBtAvStartIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAvStartIndHandler Event function not defined\n");
    }
    else
    {
        CsrBtAvAppEventFunctionsPtr->csrBtAvStartIndFunc();
    }
}


static void csrBtAvSuspendIndHandler(  csr_api_av_prim_type message, void* data, CsrUint16 length)
{

    if(CsrBtAvAppEventFunctionsPtr->csrBtAvSuspendIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtAvSuspendIndHandler Event function not defined\n");
    }
    else
    {
        CsrBtAvAppEventFunctionsPtr->csrBtAvSuspendIndFunc();
    }
}


static const csrBtAvAppStateHandlerType csrBtAvAppStateHandler[AV_UPSTREAM_CNT] =
{
    csrBtAvActivateCfmHandler,                      /* AV_ACTIVATATE_CFM                	*/
    csrBtAvDeActivateCfmHandler,                    /* AV_DEACTIVATATE_CFM            	*/
    csrBtAvConnectIndHandler,                         /* AV_CONNECT_IND                      	*/
    csrBtAvConfigIndHandler,                          /*AV_CONFIG_IND				*/
    csrBtAvPcmStreamDataIndHandler,                   /* AV_PCM_STREAM_DATA_IND       	*/
    csrBtAvCloseIndHandler,                         /* AV_CLOSE_IND       				*/
    csrBtAvCloseCfmHandler,							/*AV_CLOSE_CFM*/
    csrBtAvConnectCfmHandler,                         /*AV_CONNECT_CFM*/
    csrBtAvDisconnectIndHandler,						/*AV_DISCONNECT_IND*/
    csrBtAvOpenIndHandler,								/*AV_OPEN_IND*/
    csrBtAvConfigCfmHandler,                          /*AV_CONFIG_CFM				*/
    csrBtAvOpenCfmHandler,								/*AV_OPEN_CFM*/
    csrBtAvDisconnectCfmHandler,                          /*AV_DISCONNECT_CFM,*/
    csrBtAvStartIndHandler,								/*AV_START_IND*/
	csrBtAvSuspendIndHandler							/*AV_SUSPEND_IND*/
};



void CsrBtAvAppEventHandler(  CsrUint16 message, void* data, CsrUint16 length)
{
    if((message >= AV_UPSTREAM_BASE) && (message< AV_UPSTREM_MAX) && (csrBtAvAppStateHandler[(CsrUint16)(message- AV_UPSTREAM_BASE)] != NULL)&&(CsrBtAvAppEventFunctionsPtr != NULL))
    {
        csrBtAvAppStateHandler[(CsrUint16)(message - AV_UPSTREAM_BASE)]( (csr_api_av_prim_type)message, data,length);
    }
    else
    {
       DBG_SS_PRINT("Received CSR_AV primitive is unknown or private\n");
    }
}

