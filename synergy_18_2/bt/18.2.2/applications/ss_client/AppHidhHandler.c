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
#include "csr_pmem.h"
#include "peer_com_linux_u2u.h"
#include "AppHIDHHandler.h"
#include "csr_samsung_log.h"


typedef void (* csrBtHIDHAppStateHandlerType)(csr_api_hidh_prim_type message, void* data, CsrUint16 length);


static CsrBtAppHIDHEventHandlerStruct * CsrBtHIDHAppEventFunctionsPtr=NULL;   



//////////////HIDH CMD
CsrInt16 csr_hidh_active_cmd(CsrBtAppHIDHEventHandlerStruct * btHidhCallbackFunctionsPtr )
{
  DBG_SS_PRINT("csr_hidh_active_cmd\n");

   if(btHidhCallbackFunctionsPtr == NULL)
	{
		return FALSE;
	}
    CsrBtHIDHAppEventFunctionsPtr = btHidhCallbackFunctionsPtr;
   
  return csr_bt_client_app_put_message_to_tx_thread(CSR_HIDH_SS_ID,HIDH_ACTIVATATE_REQ,NULL, 0 );
}




CsrInt16 csr_hidh_deactive_cmd(void)
{
  DBG_SS_PRINT("csr_hidh_deactive_cmd\n");
   
  return csr_bt_client_app_put_message_to_tx_thread(CSR_HIDH_SS_ID,HIDH_DEACTIVATATE_REQ,NULL, 0 );
}

CsrInt16 csr_hidh_connect_cmd(CsrAppBtHidhSdpInfoData *infodata )
{
	void *send_data;
	void *server_data_prt;
	CsrUint16 send_data_len;
	CsrUint16 serviceName_len=0;
	CsrUint8 charData='\0'; //null char
	
	if(infodata->serviceName)
	   serviceName_len=CsrStrLen(infodata->serviceName);
	
	send_data_len=sizeof(BD_ADDR_T)+sizeof(CsrBtHidhSdpInfo)+serviceName_len+ sizeof(CsrUint8)+sizeof(CsrUint16)+infodata->descriptorLength;

	send_data=	CsrPmemAlloc(send_data_len);
	server_data_prt=send_data;

	CsrMemCpy(server_data_prt, &infodata->deviceAddr, sizeof(CsrBtDeviceAddr));
	server_data_prt+=sizeof(CsrBtDeviceAddr);

	if(infodata->sdpInfo)	
	{
	    CsrMemCpy(server_data_prt, infodata->sdpInfo, sizeof(CsrBtHidhSdpInfo));
	}
	else
		CsrMemSet(server_data_prt, 0x00, sizeof(CsrBtHidhSdpInfo));
	server_data_prt+=sizeof(CsrBtHidhSdpInfo);
	
	if(serviceName_len)
		CsrMemCpy(server_data_prt, infodata->serviceName, serviceName_len);
	server_data_prt+=serviceName_len;

	
	CsrMemCpy(server_data_prt, &charData, sizeof(CsrUint8));
	server_data_prt+=sizeof(CsrUint8);

	CsrMemCpy(server_data_prt, &infodata->descriptorLength, sizeof(CsrUint16));

	server_data_prt+=sizeof(CsrUint16);
	
	if(infodata->descriptorLength)
		CsrMemCpy(server_data_prt, infodata->descriptor, infodata->descriptorLength);

   
   DBG_SS_PRINT("csr_hidh_connect_cmd\n");

  
  return  csr_bt_client_app_put_message_to_tx_thread(CSR_HIDH_SS_ID,HIDH_CONNECT_REQ,send_data, send_data_len );
}

CsrInt16 csr_hidh_connect_accecpt_cmd(CsrAppBtHidhSdpInfoData *infodata )
{
	void *send_data;
	void *server_data_prt;
	CsrUint16 send_data_len;
	CsrUint16 serviceName_len=0;
	CsrUint8 charData='\0'; //null char
	
	if(infodata->serviceName)
	   serviceName_len=CsrStrLen(infodata->serviceName);
	
	send_data_len=sizeof(BD_ADDR_T)+sizeof(CsrBtHidhSdpInfo)+serviceName_len+ sizeof(CsrUint8)+sizeof(CsrUint16)+infodata->descriptorLength;

	send_data=	CsrPmemAlloc(send_data_len);
	server_data_prt=send_data;

	CsrMemCpy(server_data_prt, &infodata->deviceAddr, sizeof(CsrBtDeviceAddr));
	server_data_prt+=sizeof(CsrBtDeviceAddr);

	if(infodata->sdpInfo)	
	{
	    CsrMemCpy(server_data_prt, infodata->sdpInfo, sizeof(CsrBtHidhSdpInfo));
	}
	else
		return CSR_ERR_CLI_BAD_PARAM;
	server_data_prt+=sizeof(CsrBtHidhSdpInfo);
	
	if(serviceName_len)
		CsrMemCpy(server_data_prt, infodata->serviceName, serviceName_len);
	server_data_prt+=serviceName_len;

	
	CsrMemCpy(server_data_prt, &charData, sizeof(CsrUint8));
	server_data_prt+=sizeof(CsrUint8);

	CsrMemCpy(server_data_prt, &infodata->descriptorLength, sizeof(CsrUint16));

	server_data_prt+=sizeof(CsrUint16);
	
	if(infodata->descriptorLength)
		CsrMemCpy(server_data_prt, infodata->descriptor, infodata->descriptorLength);

   
   DBG_SS_PRINT("csr_hidh_connect_accecpt_cmd\n");

  
  return  csr_bt_client_app_put_message_to_tx_thread(CSR_HIDH_SS_ID,HIDH_CONNECT_ACCEPCT_REQ,send_data, send_data_len );
}





CsrInt16 csr_hidh_disconnect_cmd(BD_ADDR_T bdaddr )
{
   DBG_SS_PRINT("csr_hidh_disconnect_cmd\n");

   
  return csr_bt_client_app_put_message_to_tx_thread(CSR_HIDH_SS_ID,HIDH_DISCONNECT_REQ,NULL, 0 );
}


CsrInt16 csr_hidh_control_cmd(BD_ADDR_T bdaddr,CsrBtHidhCtrl operation )
{

   void *data;
   CsrUint16 data_len;
   
   data_len=sizeof(CsrBtHidhCtrl);
   DBG_SS_PRINT("csr_hidh_control_cmd \n");
   
   data=CsrPmemAlloc(data_len);
   CsrMemCpy(data,&operation,data_len);


   
  return csr_bt_client_app_put_message_to_tx_thread(CSR_HIDH_SS_ID,HIDH_CONTROL_REQ,data, data_len );
}



CsrInt16 csr_hidh_data_cmd(CsrBtHidhReport         reportType,CsrUint16    reportLen,CsrUint8     *reportData)
{

   void *data;
   CsrUint16 data_len=sizeof(CsrBtHidhReport)+sizeof(CsrUint16)+reportLen;
   
   DBG_SS_PRINT("csr_hidh_data_cmd \n");
   
   data=CsrPmemAlloc(data_len);
   
   CsrMemCpy(data,&reportType,sizeof(CsrBtHidhReport));
   CsrMemCpy(data+sizeof(CsrBtHidhReport),&reportLen,sizeof(CsrUint16));
   CsrMemCpy(data+sizeof(CsrBtHidhReport)+sizeof(CsrUint16),&reportData,reportLen);

  return csr_bt_client_app_put_message_to_tx_thread(CSR_HIDH_SS_ID,HIDH_SEND_DATA_REQ,data, data_len );
}




//////////////HIDH EVENT

static void csrBtHIDHActivateCfmHandler(csr_api_hidh_prim_type message, void* data, CsrUint16 length)
{
    if(CsrBtHIDHAppEventFunctionsPtr->csrBtHidhAtivateCfmFunc == NULL)
    {
        DBG_SS_PRINT("csrBtHIDHActivateCfmHandler Event function not defined\n");
    }
    else
    {
        CsrBtHIDHAppEventFunctionsPtr->csrBtHidhAtivateCfmFunc();
    }
}

static void csrBtHIDHDeActivateCfmHandler(csr_api_hidh_prim_type message, void* data, CsrUint16 length)
{


    if(CsrBtHIDHAppEventFunctionsPtr->csrBtHidhDeActivateCfmFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtHIDHDeActivateCfmHandler Event function not defined\n");
    }
    else
    {
        CsrBtHIDHAppEventFunctionsPtr->csrBtHidhDeActivateCfmFunc();
    }
}

static void csrBtHIDHConnectIndHandler(csr_api_hidh_prim_type message, void* data, CsrUint16 length)
{


    if(CsrBtHIDHAppEventFunctionsPtr->csrBtHidhConnectIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtHIDHConnectIndHandler Event function not defined\n");
    }
    else
    {
        CsrBtHIDHAppEventFunctionsPtr->csrBtHidhConnectIndFunc(*((CsrBtDeviceAddr*) data));
    }
}
                                           



static void csrBtHIDHConnectCfmHandler(csr_api_hidh_prim_type message, void* data, CsrUint16 length)
{


    if(CsrBtHIDHAppEventFunctionsPtr->csrBtHidhConnectCfmFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtHIDHonectCfmHandler Event function not defined\n");
    }
    else
    {

        CsrBtResultCode       resultCode;
		CsrBtSupplier   resultSupplier;
		CsrAppBtHidhSdpInfoData  infodata;
		CsrUint16   str_data_len;
		void *data_prt;
		
		data_prt=data;
		CsrMemCpy(&resultCode,data_prt,sizeof(CsrBtResultCode));
		data_prt+=sizeof(CsrBtResultCode);
		CsrMemCpy(&resultSupplier,data_prt,sizeof(CsrBtSupplier));
		data_prt+=sizeof(CsrBtSupplier);
		CsrMemCpy(&infodata.deviceAddr,data_prt,sizeof(BD_ADDR_T));
		data_prt+=sizeof(BD_ADDR_T);
		
		infodata.sdpInfo=data_prt;
		if(infodata.sdpInfo->isValid==FALSE)
			infodata.sdpInfo=NULL;
		data_prt+=sizeof(CsrBtHidhSdpInfo);
		
		str_data_len=CsrStrLen(data_prt);

		if(str_data_len)
		{
			infodata.serviceName=data_prt;
		}
		else
		{
			infodata.serviceName=NULL;
		}
		data_prt+=(str_data_len+1);

		CsrMemCpy(&infodata.descriptorLength,data_prt,sizeof(CsrUint16));
		data_prt+=sizeof(CsrUint16);
		
		if(infodata.descriptorLength)
		{
		    infodata.descriptor=data_prt;
		}
		else
		{
			infodata.descriptor=NULL;
		}
		
        CsrBtHIDHAppEventFunctionsPtr->csrBtHidhConnectCfmFunc(resultCode,resultSupplier,&infodata);
    }
}


static void csrBtHIDHDisconectIndHandler(csr_api_hidh_prim_type message, void* data, CsrUint16 length)
{


    if(CsrBtHIDHAppEventFunctionsPtr->csrBtHidhDisconnectIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtHIDHDisconectIndHandler Event function not defined\n");
    }
    else
    {	
       BD_ADDR_T  Bdaddr;
	   CsrUint16 reasonCode;
	   CsrUint16 reasonSupplier;
	   
        CsrMemCpy(&Bdaddr,data,sizeof(BD_ADDR_T));
		CsrMemCpy(&reasonCode,data+sizeof(BD_ADDR_T),sizeof(CsrUint16));
		CsrMemCpy(&reasonSupplier,data+sizeof(BD_ADDR_T)+sizeof(CsrUint16),sizeof(CsrUint16));
		
        CsrBtHIDHAppEventFunctionsPtr->csrBtHidhDisconnectIndFunc(Bdaddr,reasonCode,reasonSupplier);
    }
}

static void csrBtHIDHDataIndHandler(csr_api_hidh_prim_type message, void* data, CsrUint16 length)
{


    if(CsrBtHIDHAppEventFunctionsPtr->csrBtHidhDataIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtHIDHDisconectIndHandler Event function not defined\n");
    }
    else
    {	
        CsrBtHIDHAppEventFunctionsPtr->csrBtHidhDataIndFunc(length,(CsrUint8*) data);
    }
}

static void csrBtHIDHStatusIndHandler(csr_api_hidh_prim_type message, void* data, CsrUint16 length)
{


    if(CsrBtHIDHAppEventFunctionsPtr->csrBtHidhStatusIndFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtHIDHDisconectIndHandler Event function not defined\n");
    }
    else
    {	
        CsrBtHIDHAppEventFunctionsPtr->csrBtHidhStatusIndFunc(*(CsrBtHidhStatus*) data);
    }
}

static void csrBtHIDHConnectAcceptIndHandler(csr_api_hidh_prim_type message, void* data, CsrUint16 length)
{


    if(CsrBtHIDHAppEventFunctionsPtr->csrBtHidhConnectAcceptIndHandler == NULL)
    {
        DBG_SS_PRINT( "csrBtHIDHDisconectIndHandler Event function not defined\n");
    }
    else
    {
    	BD_ADDR_T Bdaddr;
        CsrBtResultCode 	   avResultCode;
		CsrBtSupplier			avResultSupplier;
		
		CsrMemCpy(&Bdaddr,data,sizeof(BD_ADDR_T));
		CsrMemCpy(&avResultCode,data+sizeof(BD_ADDR_T),sizeof(CsrBtResultCode));
		CsrMemCpy(&avResultSupplier,data+sizeof(BD_ADDR_T)+sizeof(CsrBtResultCode),sizeof(CsrBtSupplier));
		
        CsrBtHIDHAppEventFunctionsPtr->csrBtHidhConnectAcceptIndHandler(Bdaddr,avResultCode,avResultSupplier);
    }
}

static void csrBtHIDHDisconectCfmHandler(csr_api_hidh_prim_type message, void* data, CsrUint16 length)
{


    if(CsrBtHIDHAppEventFunctionsPtr->csrBtHidhDisconnectCfmFunc == NULL)
    {
        DBG_SS_PRINT( "csrBtHIDHDisconectIndHandler Event function not defined\n");
    }
    else
    {	
       BD_ADDR_T  Bdaddr;
	   CsrUint16 reasonCode;
	   CsrUint16 reasonSupplier;
	   
        CsrMemCpy(&Bdaddr,data,sizeof(BD_ADDR_T));
		CsrMemCpy(&reasonCode,data+sizeof(BD_ADDR_T),sizeof(CsrUint16));
		CsrMemCpy(&reasonSupplier,data+sizeof(BD_ADDR_T)+sizeof(CsrUint16),sizeof(CsrUint16));
        CsrBtHIDHAppEventFunctionsPtr->csrBtHidhDisconnectCfmFunc(Bdaddr,reasonCode,reasonSupplier);
    }
}


static const csrBtHIDHAppStateHandlerType csrBtHIDHAppStateHandler[HIDH_UPSTREAM_CNT] =
{
    csrBtHIDHActivateCfmHandler,                      /* HIDH_ACTIVATATE_CFM                	*/
    csrBtHIDHDeActivateCfmHandler,                    /* HIDH_DEACTIVATATE_CFM            	*/
    csrBtHIDHConnectIndHandler,                       /* HIDH_CONNECT_IND                      	*/
    csrBtHIDHConnectCfmHandler,                       /*HIDH_CONNECT_CFM		*/
    csrBtHIDHDisconectIndHandler,                     /* HIDH_DISCONNECT_IND*/
    csrBtHIDHDataIndHandler,						  /*HIDH_DATA_IND*/
    csrBtHIDHStatusIndHandler,						  /*HIDH_STATUS_IND*/
    csrBtHIDHConnectAcceptIndHandler,                  /*HIDH_CONNECT_ACCEPT_CFM*/
    csrBtHIDHDisconectCfmHandler                       /*HIDH_DISCONNECT_CFM*/
};


void CsrBtHIDHAppEventHandler(  CsrUint16 message, void* data, CsrUint16 length)
{
    if((message >= HIDH_UPSTREAM_BASE) && (message< HIDH_UPSTREM_MAX) && (csrBtHIDHAppStateHandler[(CsrUint16)(message- HIDH_UPSTREAM_BASE)] != NULL)&&(CsrBtHIDHAppEventFunctionsPtr !=  NULL))
    {
        csrBtHIDHAppStateHandler[(CsrUint16)(message - HIDH_UPSTREAM_BASE)]( (csr_api_hidh_prim_type)message, data,length);
    }
    else
    {
       DBG_SS_PRINT("Received CSR_HIDH primitive is unknown or private\n");
    }
}

