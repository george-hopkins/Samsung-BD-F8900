/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include <stdio.h>
#include "csr_types.h"
#include "csr_bt_ss_prim.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_sd_lib.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_samsung_log.h"
#include "csr_bt_sc_db.h"
#include "csr_util.h"
#include "csr_pmem.h"
#include "csr_result.h"
#include "Bluetooth_CM.h"
#include "Bluetooth_SC.h"
#include "Bluetooth_SD.h"
#include "AppGaplib.h"

extern CsrResult CsrTransportReset();


CsrBool handle_cm_server_cmd_handler( CsrUint16 cmd, void *data, CmAppInst *inst)
{
	//printf("handle_cm_server_cmd_handler : cmd <%04x>,\n", cmd);

   switch(cmd)
   {
     case GAP_SET_SCAN_ENABLE_REQ:
	 {
		CsrBool     disableInquiryScan;
		CsrBool     disablePageScand; 

		CsrMemCpy(&disableInquiryScan,  data , sizeof(disableInquiryScan));
		CsrMemCpy(&disablePageScand, data + sizeof(disableInquiryScan), sizeof(disablePageScand)); 
		CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_cm_server_cmd_handler GAP_SET_SCAN_ENABLE_REQ"));

		CsrBtCmWriteScanEnableReqSend(inst->qid,disableInquiryScan,disablePageScand);
     }
	 break;
	 case GAP_CONNECTABLE_REQ:
	 {
	 	  CsrBool     connectAble;
		  CsrMemCpy(&connectAble,  data , sizeof(connectAble));
	 	  CsrBtCmConnectableReqSend(inst->qid,connectAble);
		  CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_cm_server_cmd_handler GAP_CONNECTABLE_REQ"));
     }
	 break;
	 case GAP_LOCAL_BDADDR_REQ:
	 {
	 	 CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_cm_server_cmd_handler GAP_LOCAL_BDADDR_REQ"));
	 	CsrBtCmReadLocalBdAddrReqSend(inst->qid);
	 }
	 break;
	 case GAP_LOCAL_NAME_REQ:
	 {
	 	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_cm_server_cmd_handler GAP_LOCAL_NAME_REQ"));
	 	CsrBtCmReadLocalNameReqSend(inst->qid);
	 }
	 break;
	case GAP_SET_DBG_LEVEL_REQ:
		{

			CsrUint32 level;
//			CsrCharString *value;
			
            CsrMemCpy(&level,  data , sizeof(level));

#if defined(CSR_LOG_ENABLE)
		    CsrLogLevelEnvironmentSet(0x00);
	        CsrLogLevelTaskSetAll(0x00);
	        CsrLogLevelTextSetAll(level);
#endif			
				
		}
		break;
	case GAP_SS_INIT_REQ:
		{
			extern CsrUint32 SSLogLevelText;
			extern CsrUint32 SSIsPaic;
			CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0, "csr_bt_app_hci VER [%s]",SYNERGY_SS_VER));
			//CsrBtCmReadCodReqSend(CSR_BT_GAP_APP_IFACEQUEUE);
			CsrAppBtGapGetDbgLevel(SSLogLevelText);
			//CsrBtScDebugModeReqSend(CSR_BT_GAP_APP_IFACEQUEUE,TRUE);
			if(SSIsPaic)
			{	
				CsrAppBtGapError();
			}
			if(inst->isInit)
			{
			    CsrAppBtGapInitInd();
			}
			
		}
		break;
	case GAP_SET_LOCAL_NAME_REQ:
		 {
			CsrUtf8String			 * pName;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_cm_server_cmd_handler GAP_SET_LOCAL_NAME_REQ"));
			pName=(CsrUtf8String *)CsrPmemAlloc(sizeof(CsrBtDeviceName));
			CsrMemCpy(pName,  data , sizeof(CsrBtDeviceName));
			CsrBtCmSetLocalNameReqSend(inst->qid,pName);
		 }
		 break;	
	case GAP_SS_RESET_REQ:
	    {
			CsrTransportReset();
		}
		break;
	case GAP_CM_WRITE_PAGE_TO_REQ:
	    {
            CsrUint16        timeout;
			CsrMemCpy(&timeout, data, sizeof(CsrUint16)); 
		    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_cm_server_cmd_handler GAP_CM_WRITE_PAGE_TO_REQ"));
			CsrBtCmWritePageToReqSend(inst->qid,timeout);
		}
	    break;
	default:
		{
			/* Unexpected primitive received */
			CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,"handle_cm_server_cmd_handler Unexpected primitive received  %x\n",cmd));
			break;
		}
	break;
   }

   return TRUE;
}
CsrBool handle_sc_server_cmd_handler( CsrUint16 cmd, void *data,  ScAppInst *inst)
{ 
	switch(cmd)
   {

		case GAP_SC_PASSKEY_RES:
		 {
			CsrBool 		 accept;
			CsrBtDeviceAddr    theDeviceAddr;
			CsrUint8		 thePasskeyLength;
			CsrUint8		 thePasskey[CSR_BT_PASSKEY_MAX_LEN];
			CsrBool 		 addDevice=FALSE;
			CsrBool 		 authorised;
	
             int i;
	
			CsrMemCpy(&accept,	data , sizeof(accept));
			CsrMemCpy(&theDeviceAddr, data + sizeof(accept), sizeof(theDeviceAddr)); 
			CsrMemCpy(&thePasskeyLength, data + sizeof(accept)+ sizeof(theDeviceAddr), sizeof(thePasskeyLength)); 
			CsrMemCpy(&thePasskey, data + sizeof(accept)+ sizeof(theDeviceAddr)+sizeof(thePasskeyLength), sizeof(thePasskey)); 
			CsrMemCpy(&authorised, data + sizeof(accept)+ sizeof(theDeviceAddr)+sizeof(thePasskeyLength)+sizeof(thePasskey), sizeof(authorised)); 
			
	
			if(accept)
				addDevice=TRUE;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_sc_server_cmd_handler GAP_SC_PASSKEY_RES BDADDR %04X:%02X:%06X\n",theDeviceAddr.nap, theDeviceAddr.uap, theDeviceAddr.lap));

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "GAP_SC_PASSKEY_RES accept %d,authorised %d addDevice %d",accept,authorised,addDevice));
			CsrBtScPasskeyResSend(accept, theDeviceAddr,thePasskeyLength,thePasskey,addDevice,authorised);
		 }
		 break;

		case GAP_SC_DEREGISGER_SCDB_REQ:
		{
			CsrBtDeviceAddr 	deviceAddr;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_sc_server_cmd_handler GAP_SC_DEREGISGER_SCDB_REQ"));
			CsrMemCpy(&deviceAddr,	data , sizeof(CsrBtDeviceAddr));
			CsrBtScDebondReqSend(inst->qid,deviceAddr);
		
		}
		break;
		
		case GAP_SC_AUTHRIZE_RSP:
		{
			 CsrBool isAuthrized;
			 BD_ADDR_T            bd_addr;
			 CsrMemCpy(&isAuthrized,	data , sizeof(CsrBool));
			 CsrMemCpy(&bd_addr,	data+ sizeof(CsrBool) , sizeof(BD_ADDR_T));
			 CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "GAP_SC_AUTHRIZE_RSP isAuthrized %d BDADDR %04X:%02X:%06X\n",isAuthrized,bd_addr.nap, bd_addr.uap, bd_addr.lap));
			 CsrBtScAuthoriseResSend(isAuthrized, bd_addr);
		}
		
	    case GAP_SC_ENCRYPTION_REQ:
		{
			CsrBool isEncryptionMod;
			 BD_ADDR_T            bd_addr;
			 CsrMemCpy(&isEncryptionMod,	data , sizeof(CsrBool));
			 CsrMemCpy(&bd_addr,	data+ sizeof(CsrBool) , sizeof(BD_ADDR_T));
			 CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "GAP_SC_ENCRYPTION_REQ isAuthriisEncryptionModzed %d BDADDR %04X:%02X:%06X\n",isEncryptionMod,bd_addr.nap, bd_addr.uap, bd_addr.lap));
			CsrBtScEncryptionReqSend(inst->qid, bd_addr, isEncryptionMod);
		}
		break;
		
        case GAP_SC_SSP_JUSTWORKS_RES:
		{
		   CsrBool                 accept;
           CsrBtDeviceAddr         deviceAddr;
           CsrBool                 authorised;
           CsrBool                 addDevice=FALSE;
		   
			CsrMemCpy(&accept,	data , sizeof(accept));
			CsrMemCpy(&deviceAddr, data + sizeof(accept), sizeof(deviceAddr)); 
			CsrMemCpy(&authorised, data + sizeof(accept)+ sizeof(deviceAddr), sizeof(authorised));
			
			if(accept)
				addDevice=TRUE;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_sc_server_cmd_handler GAP_SC_SSP_JUSTWORS_RES accept %d,authorised %d addDevice %d",accept,authorised,addDevice));
			CsrBtScSspJustWorksResSend(accept,deviceAddr,addDevice,authorised);
		}
		break;
		
		default:
		{
			/* Unexpected primitive received */
			CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,"handle_sc_server_cmd_handler Unexpected primitive received  %x\n",cmd));
			break;
		}
	}
	return TRUE;

}
CsrBool handle_sd_server_cmd_handler( CsrUint16 cmd, void *data, SdAppInst *inst)
{
	switch(cmd)
	{
	    case GAP_SD_REGISTER_SCDB_REQ:
		{
			CsrBtScDbDeviceRecord *     pTheRecord=(CsrBtScDbDeviceRecord * )data;
			//		CsrBtDevicePropertiesType *deviceProperties;
			//		 deviceProperties = CsrPmemAlloc(sizeof(*deviceProperties));
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_sd_server_cmd_handler GAP_SD_REGISTER_SCDB_REQ"));
			CsrBtScDbWrite(&(pTheRecord->deviceAddr), pTheRecord);
			//		CsrBtScCopyDeviceProperties(deviceProperties,pTheRecord);
			//		CsrBtScUpdateDeviceReqSend(CSR_BT_GAP_APP_IFACEQUEUE,deviceProperties);
			CsrBtSdSynchronizeReqSend(inst->qid);
		}
	 	break;

		case GAP_SD_SEARCH_REQ:
		{
		
			CsrUint16 maxnum;
			CsrUint32 searchTime;
			CsrUint32 bufferTime;
			CsrBool skipNameRequest;
			CsrUint32 searchConfig;
			CsrBtClassOfDevice Cod;
			
			CsrMemCpy(&maxnum,  data , sizeof(maxnum));
			CsrMemCpy(&searchTime, data + sizeof(maxnum), sizeof(searchTime));
			CsrMemCpy(&bufferTime, data +  sizeof(maxnum)+ sizeof(searchTime), sizeof(bufferTime));
			CsrMemCpy(&Cod, data +  sizeof(maxnum)+ sizeof(searchTime)+sizeof(bufferTime), sizeof(Cod));
			CsrMemCpy(&skipNameRequest, data +  sizeof(maxnum)+ sizeof(searchTime)+sizeof(bufferTime)+sizeof(Cod), sizeof(skipNameRequest));

			if(inst->state==CSR_BT_GAP_SD_STATE_IDLE)
			{

				inst->state = CSR_BT_GAP_SD_STATE_SEARCHING;
				inst->devicesInSearchList = 0;
				inst->devicesInSearchMax=maxnum;

			
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_sd_server_cmd_handler GAP_SD_SEARCH_REQ search[%ld] buffer[%ld] cod [%x], name [%d]\n",searchTime,bufferTime,Cod,skipNameRequest));
				
				if(skipNameRequest)
					searchConfig=CSR_BT_SD_SEARCH_SHOW_UNKNOWN_DEVICE_NAMES | CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING|CSR_BT_SD_SEARCH_DISABLE_NAME_READING;
				else	
					searchConfig= CSR_BT_SD_SEARCH_CONTINUE_AFTER_RSSI_SORTING;

			      CsrBtSdSearchReqSend(inst->qid,
		                searchConfig, 							/* searchConfiguration */
		                bufferTime,                     		/* RSSI buffer timeout */
		                searchTime,                        		/* Search timeout */
		                CSR_BT_SD_RSSI_THRESHOLD_DONT_CARE,   	/* RSSI threshold */
		                Cod,      								/* Class of Device */
		                Cod,      								/* Class of Device mask */
		                CSR_BT_SD_ACCESS_CODE_GIAC,           	/* Access code */
		                0,                             			/* Filter length */
		                NULL);
			}
			else
			{
			   CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0, "handle_sd_server_cmd_handler GAP_SD_SEARCH_REQ  err state[%d]",inst->state));
			}
		}
		break;
		case GAP_SD_SEARCH_CANCEL_REQ:
		{
			if(inst->state==CSR_BT_GAP_SD_STATE_SEARCHING)
			{
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0, "handle_sd_server_cmd_handler GAP_SD_SEARCH_CANCEL_REQ"));
				inst->state = CSR_BT_GAP_SD_STATE_CANCEL_SEARCH;
				CsrBtSdCancelSearchReqSend(inst->qid);
			}
			else
			{
			   CSR_LOG_TEXT_WARNING((CSR_SS_AP_LOG_ID,0, "handle_sd_server_cmd_handler GAP_SD_SEARCH_CANCEL_REQ  err state[%d]",inst->state));
			}
		}
		break;
		default:
		{
			/* Unexpected primitive received */
			CSR_LOG_TEXT_CRITICAL((CSR_SS_AP_LOG_ID,0,"handle_sd_server_cmd_handler Unexpected primitive received  %x\n",cmd));
			break;
		}
	}
	 return TRUE;
}

