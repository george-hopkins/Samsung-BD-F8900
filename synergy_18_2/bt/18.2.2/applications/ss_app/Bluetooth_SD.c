/**********************************************************************
* File Name   : Bluetooth_SD.c
* Description : This file define functions and variables for Service Discovery.
* Updates			:
***********************************************************************/

#include "csr_bt_sd_lib.h"
#include "csr_bt_declare_tasks.h"
#include "csr_bt_sd_prim.h"
#include "Bluetooth_Common.h"
#include "Bluetooth_SD.h"
#include "csr_pmem.h"
#include "AppGaplib.h" 

#define BT_VID_SAMSUNG 0x04E8
#define BT_PID_SAMSUNG_AD_SUPPORT_TV 0x8080


/*****************************************************************************
* Function Name : init_sd()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
void init_sd(void **gash)
{
	SdAppInst *inst;

	*gash = CsrPmemAlloc(sizeof(SdAppInst));
	CsrMemSet(*gash, 0, sizeof(SdAppInst));
	inst = (SdAppInst *)*gash;

	inst->qid = TESTQUEUE;
	inst->state =CSR_BT_GAP_SD_STATE_IDLE;



	//Bluetooth_InitSearchList();
}

/*****************************************************************************
* Function Name : deinit_sd()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
void deinit_sd(void **gash)
{
	SdAppInst *inst;

	inst = (SdAppInst *)*gash;

	CsrPmemFree(inst);

}


/*****************************************************************************
* Function Name : handleSdPrim()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
void handleSdPrim(SdAppInst  *inst,CsrPrim    *primType)
{

   
	switch (*primType)
	{
		case CSR_BT_SD_SEARCH_RESULT_IND:
		{
			CsrBtSdSearchResultInd *prim = (CsrBtSdSearchResultInd *)primType;
			CsrUint8 * vender_id;
			CsrUint32 vidpid=0;

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SD_SEARCH_RESULT_IND\n"));

			
			if (inst->state == CSR_BT_GAP_SD_STATE_SEARCHING)
			{ /* The applicatoin is in the middle of a search procedure 						  */

				CsrUint8 *friendlyName;
				CsrBtDeviceName devicefriendlyName;
				

				CsrBtSdDecodeFriendlyName(prim->info, prim->infoLen, &friendlyName);

				if(friendlyName)
		       	{
		       	    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"csrBtSdSearchResultIndFunc name %s \n",friendlyName));
		        	CsrMemCpy(&devicefriendlyName,friendlyName,sizeof(CsrBtDeviceName));
					devicefriendlyName[CSR_BT_MAX_FRIENDLY_NAME_LEN]='\0';
		       	}
			    else
			   	{
			   	   CsrMemSet(&devicefriendlyName,0x00,sizeof(CsrBtDeviceName));
			   	   CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"csrBtSdSearchResultIndFunc name null"));
			   	}
					/* Update the CSR_BT_GAP_SD_SEARCH_DEVICES_UI, the UI which showing remote devices found the
					search/inquiry procedure  
					*/
				
				CsrBtSdInfoGetTag(prim->infoLen,prim->info,&vender_id,CSR_BT_EIR_DATA_TYPE_SM_TK_VALUE);
				if(vender_id)
				{
				    
					vidpid=(((vender_id[2])|(vender_id[3]<<8))<<16) |((vender_id[4])|vender_id[5]<<8);
					CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"csrBtSdSearchResultIndFunc vid %04x pid %04x",((vender_id[2])|(vender_id[3]<<8)),((vender_id[4])|vender_id[5]<<8)));
#if 0					
				    if((((vender_id[2])|(vender_id[3]<<8))== BT_VID_SAMSUNG ) && (((vender_id[4])|vender_id[5]<<8) ==BT_PID_SAMSUNG_AD_SUPPORT_TV))
				    {
				       isTv=TRUE;
					}
#endif					
				}
				CsrAppBtGapSdSearchInd(&prim->deviceAddr,prim->deviceClass,&devicefriendlyName,vidpid,prim->rssi);
				inst->devicesInSearchList++;

				if(inst->devicesInSearchList >= inst->devicesInSearchMax)
				{
				    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CsrBtSdCancelSearchReqSend devicesInSearchList [%d] devicesInSearchMax[%d]",inst->devicesInSearchList,inst->devicesInSearchMax));
					inst->state = CSR_BT_GAP_SD_STATE_CANCEL_SEARCH;
		        	CsrBtSdCancelSearchReqSend(inst->qid);
				}
	
				CsrPmemFree(friendlyName);
			}

			CsrPmemFree(prim->info);
			break;
		}
	
		case CSR_BT_SD_CLOSE_SEARCH_IND:
		{
			CsrBtSdCloseSearchInd *prim = (CsrBtSdCloseSearchInd *)primType;

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SD_CLOSE_SEARCH_IND [%d] [%d]\n",prim->resultCode,prim->resultSupplier));
			
			inst->state = CSR_BT_GAP_SD_STATE_IDLE;
			CsrAppBtGapSdSearchClose();
			break;
		}
		case CSR_BT_SD_SYNCHRONIZE_CFM:
		{
			CsrBtSdSynchronizeCfm *prim = (CsrBtSdCloseSearchInd *)primType;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SD_CLOSE_SEARCH_IND totalNumOfDevices %ld\n", prim->totalNumOfDevices));
			break;
		}
#if 0
		case CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM:
		{
            CsrBtSdReadAvailableServicesCfm *prim;
			//CsrUint16 nameLen;
            CsrUint8 *name;


	
            prim = (CsrBtSdReadAvailableServicesCfm*)primType;


//                    stateChange(inst, STATE_IDLE);
//                    playMenu(inst);
			if(prim->resultSupplier == CSR_BT_SUPPLIER_SD && prim->resultCode == CSR_BT_RESULT_CODE_SD_SUCCESS)
			{
				CsrUint16	 noOfServices;
				CsrUint32	 * serviceList;

				/*nameLen = */CsrBtSdDecodeFriendlyName(prim->info, prim->infoLen, &name);
				CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Service search for address %04X:%02X:%06X, name %s completed:\n",
				       prim->deviceAddr.nap,
				       prim->deviceAddr.uap,
				       prim->deviceAddr.lap,
				       name));
				CsrPmemFree(name);

                CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM prim->info %x, prim->infoLen %d\n",prim->info, prim->infoLen) );
				if(CsrBtSdIsServicePresent(prim->info, prim->infoLen, CSR_BT_HFG_PROFILE_UUID))
				{
				    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_HFG_PROFILE_UUID service available\n"));
				}
				else
				{
				    CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_HFG_PROFILE_UUID not available\n"));
				}



				noOfServices = CsrBtSdReadServiceList(prim->info, prim->infoLen, &serviceList);


				while(noOfServices > 0)
				{
					//CsrUint8 serviceLsb;
					noOfServices--;

				//	serviceLsb = (CsrUint8) serviceList[noOfServices]-1; /*only take lsb of service and substract 1 to index the table correctly*/

					switch(serviceList[noOfServices] & 0xFFFFFF00)
					{
						case 0x00001100:
						{
							CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0," CSR_BT_SD_READ_AVAILABLE CSR_BT_GAP_SERVICE_11 available %lx\n",serviceList[noOfServices]));
							

							break;
						}

						case 0x00001200:
						{
							CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0," CSR_BT_SD_READ_AVAILABLE CSR_BT_GAP_SERVICE_12 available %lx\n",serviceList[noOfServices]));

							break;
						}

						case 0x00001300:
						{
							CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0," CSR_BT_SD_READ_AVAILABLE CSR_BT_GAP_SERVICE_12 available %lx\n",serviceList[noOfServices]));

							break;
						}

						default:
						{
							break;
						}
					}

				}

				CsrPmemFree(serviceList);
			}
		   
            CsrPmemFree(prim->info);
            break;
        }
		case CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM:
		{
			//stateChange(inst, STATE_IDLE);

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Read available services cancelled\n"));
			break;
		}
#endif
		default:
		{
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Unhandled SD primitive: 0x%04X\n", *primType));
			CsrBtSdFreeUpstreamMessageContents(CSR_BT_SD_PRIM, primType);
			break;
		}
	}
}


