/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include <stdio.h>
#include "csr_types.h"
#include "csr_bt_ss_prim.h"
#include "csr_bt_hidh_lib.h"
#include "csr_bt_hidh_prim.h"
#include "csr_samsung_log.h"
#include "Bluetooth_HIDH.h"
#include "AppHIDHidhData.h"
#define CHANGE_STATE(s) (inst->state=s)


CsrBool handle_hidh_server_cmd_handler( CsrUint16 cmd, void *data, HidhAppInst *inst)	
{
//	printf("handle_hidh_server_cmd_handler : cmd <%04x>, \n", cmd);

   switch(cmd)
   {
     case HIDH_ACTIVATATE_REQ:
	 {
	 	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"HIDH_ACTIVATATE_REQ"));
	 	/* Register user application handle if NOT in split-mode */
		CsrBtHidhRegisterUserReqSend(CSR_BT_HID_ALL_DEVICE_SLOTS,
		                    inst->qid);

		//CsrBtHidhSecurityInReqSend(inst->qid,CSR_BT_SEC_NONE);
		//CsrBtHidhSecurityOutReqSend(inst->qid,CSR_BT_SEC_NONE);
		CHANGE_STATE(HIDH_STATE_AWAIT_USER_REG);

     }
	 break;
	 
	 case HIDH_CONNECT_REQ:
	 {
	 	 CsrAppBtHidhSdpInfoData  infodata;
	 	 if(inst->state==HIDH_STATE_IDLE)
	 	 {

			 CsrUint16	 str_data_len;
			 void *data_prt;
			 CsrBtHidhSdpInfo        *sdpInfodata;
			 
			 data_prt=data;

			 CsrMemCpy(&infodata.deviceAddr,data_prt,sizeof(BD_ADDR_T));
			 data_prt+=sizeof(BD_ADDR_T);
			 
			  sdpInfodata=data_prt;
			 
			 if(sdpInfodata->isValid==FALSE)
				 infodata.sdpInfo=NULL;
			 else
			 {
	             infodata.sdpInfo = (CsrBtHidhSdpInfo *) CsrPmemAlloc(sizeof(CsrBtHidhSdpInfo));
				  CsrMemCpy(infodata.sdpInfo,data_prt,sizeof(CsrBtHidhSdpInfo));
			 }
			 data_prt+=sizeof(CsrBtHidhSdpInfo);
			 
			 str_data_len=CsrStrLen(data_prt);

			 if(str_data_len)
			 {
				 infodata.serviceName=data_prt;
				 infodata.serviceName = (CsrBtHidhSdpInfo *) CsrPmemAlloc(str_data_len+1);
				 CsrStrCpy(infodata.serviceName,(CsrCharString *)data_prt);
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
				 infodata.descriptor=(CsrBtHidhSdpInfo *) CsrPmemAlloc(infodata.descriptorLength);
				 CsrMemCpy(infodata.descriptor,data_prt,infodata.descriptorLength);
			 }
			 else
			 {
				 infodata.descriptor=NULL;
			 }


	//	    if( inst->selectedDevice == CSR_BT_UNDEFINED)
		    {
		        inst->selectedDevice = 0;
		    }
		//	inst->bdAddrList[inst->selectedDevice]=deviceAddr;


		    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"HIDH_CONNECT_REQ  send - please wait \n\n"));


		    if( (infodata.sdpInfo != NULL) && infodata.sdpInfo->isValid )
		    {
		        CsrBtHidhConnectReqSend( inst->qid,
		                            inst->selectedDevice,
		                            infodata.deviceAddr,
		                            L2CA_FLUSH_TO_DEFAULT,
		                            NULL,
		                            NULL,
		                            infodata.sdpInfo,
		                            infodata.serviceName,
		                            infodata.descriptorLength,
		                            infodata.descriptor);
		    }
		    else    
		    {
		        CsrBtHidhConnectReqSend( inst->qid, inst->selectedDevice,
		                            infodata.deviceAddr,
		                            L2CA_FLUSH_TO_DEFAULT,
		                            NULL,
		                            NULL,
		                            NULL,
		                            NULL,
		                            0,
		                            NULL);
		    }
	 	}
		else
		{

		   CsrMemCpy(&infodata.deviceAddr,data,sizeof(BD_ADDR_T));
		   infodata.sdpInfo=NULL;
		   infodata.serviceName=NULL;
		   infodata.descriptorLength=0;
		   infodata.descriptor=NULL;
		   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"HIDH_CONNECT_REQ send err %d",inst->state));
		   CsrAppServerHidhConnetCfm((CsrBtResultCode)inst->state ,CSR_BT_SUPPLIER_SS_UI,&infodata);

		}
	
     }
	 break;
	 case HIDH_CONNECT_ACCEPCT_REQ:
	 {
	 	 if(inst->state==HIDH_STATE_IDLE)
	 	 {
			 CsrAppBtHidhSdpInfoData  infodata;
			 CsrUint16	 str_data_len;
			 void *data_prt;
			 CsrBtHidhSdpInfo        *sdpInfodata;
			 
			 data_prt=data;

			 CsrMemCpy(&infodata.deviceAddr,data_prt,sizeof(BD_ADDR_T));
			 data_prt+=sizeof(BD_ADDR_T);
			 
			  sdpInfodata=data_prt;
			 
			 if(sdpInfodata->isValid==FALSE)
				 infodata.sdpInfo=NULL;
			 else
			 {
	             infodata.sdpInfo = (CsrBtHidhSdpInfo *) CsrPmemAlloc(sizeof(CsrBtHidhSdpInfo));
				  CsrMemCpy(infodata.sdpInfo,data_prt,sizeof(CsrBtHidhSdpInfo));
			 }
			 data_prt+=sizeof(CsrBtHidhSdpInfo);
			 
			 str_data_len=CsrStrLen(data_prt);

			 if(str_data_len)
			 {
				 infodata.serviceName=data_prt;
				 infodata.serviceName = (CsrBtHidhSdpInfo *) CsrPmemAlloc(str_data_len+1);
				 CsrStrCpy(infodata.serviceName,(CsrCharString *)data_prt);
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
				 infodata.descriptor=(CsrBtHidhSdpInfo *) CsrPmemAlloc(infodata.descriptorLength);
				 CsrMemCpy(infodata.descriptor,data_prt,infodata.descriptorLength);
			 }
			 else
			 {
				 infodata.descriptor=NULL;
			 }


	//	    if( inst->selectedDevice == CSR_BT_UNDEFINED)
		    {
		        inst->selectedDevice = 0;
		    }
		//	inst->bdAddrList[inst->selectedDevice]=deviceAddr;


		    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"HIDH_CONNECT_ACCEPCT_REQ send  - please wait \n\n"));


		    if( (infodata.sdpInfo != NULL) && infodata.sdpInfo->isValid )
		    {
		       CHANGE_STATE(HIDH_STATE_AWAIT_CONNECT);
		        CsrBtHidhConnectAcceptReqSend( inst->qid,
		                            inst->selectedDevice,
		                            infodata.deviceAddr,
		                            L2CA_FLUSH_TO_DEFAULT,
		                            NULL,
		                            NULL,
		                            infodata.sdpInfo,
		                            infodata.serviceName,
		                            infodata.descriptorLength,
		                            infodata.descriptor);
		    }
	 	}
	    else
		{
		   CsrBtDeviceAddr          deviceAddr; 
		   
		   CsrMemCpy(&deviceAddr,data,sizeof(BD_ADDR_T));
		   CsrAppServerHidhConectAccectCfm(&deviceAddr,(CsrBtResultCode)inst->state ,CSR_BT_SUPPLIER_SS_UI);
		   
		   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"HIDH_CONNECT_ACCEPCT_REQ send err %d",inst->state));
		}
     }
	 	break;
	 case HIDH_DISCONNECT_REQ:
	 {
		if((inst->state==HIDH_STATE_CONNECTED)||(inst->state== HIDH_STATE_RECONNECTING))
	 	{
	 		CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"HIDH_DISCONNECT_REQ  - please wait \n\n"));
	 		CsrBtHidhDisconnectReqSend(0);
		}
		else
		{
		   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"HIDH_DISCONNECT_REQ send err %d",inst->state));
		   CsrAppServerHidhDisconnectCfm(&inst->connectedAddr,inst->state,CSR_BT_SUPPLIER_SS_UI);
		}
	 }
	 break;
	 case HIDH_DEACTIVATATE_REQ:
	 {
	 	  CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"HIDH_DEACTIVATATE_REQ  - please wait \n\n"));
	 }
	 break;
	 case HIDH_CONTROL_REQ:
	 {
	 	if((inst->state==HIDH_STATE_CONNECTED)||(inst->state== HIDH_STATE_RECONNECTING))
	 	{
	 	     CsrBtHidhCtrl           operation;
			 CsrMemCpy(&operation,data,sizeof(CsrBtHidhCtrl));
			 CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"HIDH_CONTROL_REQ  - please wait \n\n"));
	 		 CsrBtHidhControlReqSend(0,operation);
	 	}
		else
		{
		   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"HIDH_CONTROL_REQ send err %d",inst->state));
		}
	 	
	 }
	 break;
	 case HIDH_SEND_DATA_REQ:
	 {
	 	CsrBtHidhReport         reportType;   
	    CsrUint16    reportLen;
	    CsrUint8     *reportData;
		if((inst->state==HIDH_STATE_CONNECTED)||(inst->state== HIDH_STATE_RECONNECTING))
	 	{

			CsrMemCpy(&reportType,	data , sizeof(CsrBtHidhReport));
			CsrMemCpy(&reportLen, data + sizeof(CsrBtHidhReport), sizeof(CsrUint16)); 
			reportData = CsrPmemAlloc(reportLen);
			CsrMemCpy(reportData, data + sizeof(CsrBtHidhReport)+ sizeof(CsrUint16), reportLen); 
		    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"HIDH_SEND_DATA_REQ  - please wait \n\n"));		

		    CsrBtHidhDataReqSend(0, reportType,
		                    reportLen, reportData);
	    }
		else
		{
		   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"HIDH_SEND_DATA_REQ send err %d",inst->state));
		}
	 }
	  break;
	 default:
	{
		/* Unexpected primitive received */
		CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,"handle_hidh_server_cmd_handler Unexpected primitive received  %x",cmd));
	}
	break;
   }

   return TRUE;
}



