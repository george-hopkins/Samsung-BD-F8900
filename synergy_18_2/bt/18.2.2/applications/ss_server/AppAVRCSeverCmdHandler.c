/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include <stdio.h>
#include "csr_types.h"
#include "csr_bt_ss_prim.h"
#include "csr_bt_avrcp_lib.h"
#include "csr_bt_avrcp_prim.h"
#include "csr_samsung_log.h"
#include "Bluetooth_AVRC.h"



CsrBool handle_avrc_server_cmd_handler( CsrUint16 cmd, void *data, AvrcAppInst *inst)
{
//	printf("handle_avrc_server_cmd_handler : cmd <%04x>, \n", cmd);

   switch(cmd)
   {
     case AVRC_ACTIVATATE_REQ:
	 {
 	
		CsrBtAvrcpRoleDetails tgFeatures;
		CsrBtAvrcpRoleDetails ctFeatures;

		CsrBtAvrcpConfigRoleSupport(&ctFeatures,                                         /* Pointer to details */
		CSR_BT_AVRCP_CONFIG_ROLE_STANDARD,                        /* Role config */
		CSR_BT_AVRCP_CONFIG_SR_VERSION_14,                                   /* AVRCP version */
		CSR_BT_AVRCP_CONFIG_SR_FEAT_CAT1_PLAY_REC,                     /* Features */
		(CsrCharString*)CsrStrDup("Samsung"),                                         /* Provider name */
		(CsrCharString*)CsrStrDup("AVRCP CT"));                                   /* Service name */
#ifdef EXCLUDE_CSR_BT_AVRCP_TG_MODULE
         CsrBtAvrcpConfigRoleNoSupport(&tgFeatures);
#else		
		 CsrBtAvrcpConfigRoleSupport(&tgFeatures,                                         /* Pointer to details */
                               CSR_BT_AVRCP_CONFIG_ROLE_STANDARD,                        /* Role config */
                               CSR_BT_AVRCP_CONFIG_SR_VERSION_14,                          /* AVRCP version */
                               CSR_BT_AVRCP_CONFIG_SR_FEAT_CAT1_PLAY_REC,                 /* Features */
                                    (CsrCharString*)CsrStrDup("Samsung"),                                         /* Provider name */
                                    (CsrCharString*)CsrStrDup("AVRCP TG")); 
#endif	
		 CsrBtAvrcpConfigReqSend(inst->qid,              /* Phandle */
                       CSR_BT_AVRCP_CONFIG_GLOBAL_STANDARD,    /* Global configuration */
                       CSR_BT_AV_PROFILE_DEFAULT_MTU_SIZE,                        /* MTU */
                       tgFeatures,                  /* Target specific configuration */
                       ctFeatures); 

	 	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_avrc_server_cmd_handler AVRC_ACTIVATATE_REQ"));

     }
	 break;
	 
	 case AVRC_DEACTIVATATE_REQ:
	 {
	 	  CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_avrc_server_cmd_handler AVRC_DEACTIVATATE_REQ"));
	 	  CsrBtAvrcpDeactivateReqSend();
     }
     break;
	 case AVRC_CT_PASS_THROUGH_REQ:
	 {
	 	 CsrUint8   opId;
         CsrUint8   state;
		    
		CsrMemCpy(&opId,  data , sizeof(CsrUint8));
		CsrMemCpy(&state, data + sizeof(CsrUint8), sizeof(CsrUint8)); 

		CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_avrc_server_cmd_handler AVRC_CT_PASS_THROUGH_REQ opId %x,state %x",opId,state));
		if(inst->state==SSAvStateAvrcpConnected)
		{

	    	CsrBtAvrcpCtPassThroughReqSend(inst->qid,
	                              inst->connectionId,
	                              opId,
	                              state);
		}
		else
	    {
	        CSR_LOG_TEXT_ERROR((CSR_SS_AP_LOG_ID,0, "handle_avrc_server_cmd_handler AVRC_CT_PASS_THROUGH_REQ state err %d",inst->state));
	    }
	}
     break;
	 case AVRC_CONNECT_REQ:
	 {
	 	CsrBtDeviceAddr        deviceAddr;
		CsrMemCpy(&deviceAddr,data,sizeof(CsrBtDeviceAddr));
	 	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_avrc_server_cmd_handler AVRC_CONNECT_REQ"));
		/* connect */
		if(inst->state==SSAvStateAvrcpIdle)
		{
		   inst->state=SSAvStateAvrcpConnecting;
			CsrBtAvrcpConnectReqSend(deviceAddr);
		}
		else
	    {
	        CsrAppServerAVRCConnetCfm(&deviceAddr,(CsrBtResultCode)inst->state ,CSR_BT_SUPPLIER_SS_UI);
	        CSR_LOG_TEXT_ERROR((CSR_SS_AP_LOG_ID,0, "handle_avrc_server_cmd_handler AVRC_CONNECT_REQ state err %d",inst->state));
	    }
		
     }
	 break;
	 case AVRC_DISCONNECT_REQ:
	 {
	 	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_avrc_server_cmd_handler AVRC_DISCONNECT_REQ"));
		
		if(inst->state==SSAvStateAvrcpConnected)
		{
		    inst->state=SSAvStateAvrcpDsiconnecting;
			CsrBtAvrcpDisconnectReqSend(inst->connectionId);
		}
		else
	    {
	        CsrAppServerAVRCDisconectCfm(&(inst->remoteAddr),inst->state,CSR_BT_SUPPLIER_SS_UI);
	        CSR_LOG_TEXT_ERROR((CSR_SS_AP_LOG_ID,0, "handle_avrc_server_cmd_handler AVRC_DISCONNECT_REQ state err %d",inst->state));
	    }
	
     }
	 break;
	 case AVRC_CONNECTING_CANCEL_REQ:
	 {
	 	CsrBtDeviceAddr        deviceAddr;
		CsrMemCpy(&deviceAddr,data,sizeof(CsrBtDeviceAddr));
	 	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_avrc_server_cmd_handler AVRC_CONNECTING_CANCEL_REQ"));
		
		if(inst->state==SSAvStateAvrcpConnecting)
		{
		    inst->state=SSAvStateAvrcpDsiconnecting;
			CsrBtAvrcpCancelConnectReqSend(deviceAddr);
		}
		else
	    {
	        CsrAppServerAVRCConnetCfm(&deviceAddr,(CsrBtResultCode)inst->state ,CSR_BT_SUPPLIER_SS_UI);
	        CSR_LOG_TEXT_ERROR((CSR_SS_AP_LOG_ID,0, "handle_avrc_server_cmd_handler AVRC_CONNECTING_CANCEL_REQ state err %d",inst->state));
	    }
	
     }
	 break;
	 
	  case AVRC_CT_SET_VOLUME_REQ:
	  {
		CsrUint8	 volume;
		CsrUint8	 isMute;
		 
		CsrMemCpy(&volume,  data , sizeof(CsrUint8));
		CsrMemCpy(&isMute, data + sizeof(CsrUint8), sizeof(CsrUint8)); 

		CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_avrc_server_cmd_handler AVRC_CT_SET_VOLUME_REQ volume %x,state %x",volume,isMute));
		if(inst->state==SSAvStateAvrcpConnected)
		{
			CsrBtAvrcpCtSetSSVolumeReqSend(inst->qid,
							   inst->connectionId,
							   volume,
							   isMute);
		}
		else
	    {
	        CSR_LOG_TEXT_ERROR((CSR_SS_AP_LOG_ID,0, "handle_avrc_server_cmd_handler AVRC_CT_SET_VOLUME_REQ state err %d",inst->state));
	    }
	 }
	  break;

	// 
     default:
	{
		/* Unexpected primitive received */
		CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,"handle_avrc_server_cmd_handler Unexpected primitive received  %x\n",cmd));
	}
	 break;
   }
   return TRUE;
}

