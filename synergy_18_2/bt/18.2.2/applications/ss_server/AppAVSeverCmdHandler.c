/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include <stdio.h>
#include "csr_types.h"
#include "csr_bt_ss_prim.h"
#include "csr_bt_av_lib.h"
#include "csr_bt_av_prim.h"
#include "csr_samsung_log.h"
#include "Bluetooth_AV.h"


CsrBool handle_av_server_cmd_handler( CsrUint16 cmd, void *data, Av2AppInst *inst)	
{
//	printf("handle_av_server_cmd_handler : cmd <%04x>, \n", cmd);
   switch(cmd)
   {
     case AV_ACTIVATATE_REQ:
	 {
	 	  CsrBtAvActivateReqSend(inst->qid,CSR_BT_AV_SINK);
     }
	 break;
	 case AV_DEACTIVATATE_REQ:
	 {
	 	  CsrBtAvDeactivateReqSend(CSR_BT_AV_AUDIO_SINK);
     }
	 break;
	 case AV_CONNECT_REQ:
	 {
	 	  BD_ADDR_T  deviceAddr;
		  CsrMemCpy(&deviceAddr,data,sizeof(BD_ADDR_T));
	 	  if(inst->state ==SsAvStateAvDisconnected)
	 	  {

			  inst->state =SsAvStateAvConnecting;
		 	  CsrBtAvConnectReqSend(inst->qid,deviceAddr,CSR_BT_AV_AUDIO_SOURCE);
	 	  }
		  else
		  {
		    CSR_LOG_TEXT_ERROR((CSR_SS_AP_LOG_ID,0, "handle_av_server_cmd_handler AV_CONNECT_REQ state err %d",inst->state));
			
			CsrAppServerAvConnetCfm(&deviceAddr,(CsrBtResultCode)inst->state ,CSR_BT_SUPPLIER_SS_UI);
		  }
     }
	 break;
	 case AV_CONNECTING_CANCEL_REQ:
	{
		 BD_ADDR_T  deviceAddr;
		 CsrMemCpy(&deviceAddr,data,sizeof(BD_ADDR_T));
	 	  if(inst->state ==SsAvStateAvConnecting)
	 	  {

			  inst->state =SsAvStateAvDisconnectiong;
		 	  CsrBtAvCancelConnectReqSend(deviceAddr);
	 	  }
		  else
		  {
		    CSR_LOG_TEXT_ERROR((CSR_SS_AP_LOG_ID,0, "handle_av_server_cmd_handler AV_CONNECT_REQ state err %d",inst->state));
			CsrAppServerAvConnetCfm(&deviceAddr,(CsrBtResultCode)inst->state ,CSR_BT_SUPPLIER_SS_UI);
		  }
     } 	

	break;
	 case AV_DISCONNECT_REQ:
	 {

		  switch(inst->state)
		  {
		    case SsAvStateAvConnected:
			case SsAvStateAvConnectedOpen:
			case SsAvStateAvConnectedStreaming:	
				inst->state =SsAvStateAvDisconnectiong;
	 	  	    CsrBtAvDisconnectReqSend(inst->connectionId);
			   break;
			default:
				CsrAppServerAvDisconnectCfm(&(inst->remoteAddr),inst->state,CSR_BT_SUPPLIER_SS_UI);
				CSR_LOG_TEXT_ERROR((CSR_SS_AP_LOG_ID,0, "handle_av_server_cmd_handler AV_DISCONNECT_REQ err %d",inst->state));
				break;
		  }
	 	  
     }
	 break;
	 default:
	{
		/* Unexpected primitive received */
		CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,"handle_av_server_cmd_handler Unexpected primitive received  %x\n",cmd));
	}
	break;
   }

   return TRUE;
}



