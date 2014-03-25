/**********************************************************************
* File Name   : Bluetooth_SD.c
* Description : This file define functions and variables for Service Discovery.
* Author 	  :	Kim Jin Cheol	(Connectivity Team, HYUNDAI MOBIS)
* Updates			:
***********************************************************************/

#include "csr_bt_sd_lib.h"
#include "csr_bt_declare_tasks.h"
#include "csr_bt_sd_prim.h"
#include "Bluetooth_Common.h"
#include "Bluetooth_SD.h"
#include "csr_pmem.h"



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

	inst->searchFilter = 0; // RENDERING_MAJOR_SERVICE_MASK;// |
					//CAPTURING_MAJOR_SERVICE_MASK|
					//OBJECT_TRANSFER_MAJOR_SERVICE_MASK;
	inst->searchTime = 0;
	inst->searchNumMax = MAX_SEARCH_LIST_NUM;
	inst->selectedIndex = 0;
	inst->searchListNum =0;

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
void handleSdPrim(SdAppInst  *inst)
{

   CsrPrim    *primType;
   primType = (CsrPrim *) inst->recvMsgP;
   
	switch (*primType)
	{
		case CSR_BT_SD_SEARCH_RESULT_IND:
		{
			CsrBtSdSearchResultInd *prim = (CsrBtSdSearchResultInd *)inst->recvMsgP;
			break;
		}

		case CSR_BT_SD_CLOSE_SEARCH_IND:
		{
			break;
		}

		case CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM:
		{
            CsrBtSdReadAvailableServicesCfm *prim;
			//CsrUint16 nameLen;
            CsrUint8 *name;


	
            prim = (CsrBtSdReadAvailableServicesCfm*)inst->recvMsgP;


//                    stateChange(inst, STATE_IDLE);
//                    playMenu(inst);
			if(prim->resultSupplier == CSR_BT_SUPPLIER_SD && prim->resultCode == CSR_BT_RESULT_CODE_SD_SUCCESS)
			{
				CsrUint16	 noOfServices;
				CsrUint32	 * serviceList;

				/*nameLen = */CsrBtSdDecodeFriendlyName(prim->info, prim->infoLen, &name);
				CL_API_LOGPrintf("Service search for address %04X:%02X:%06X, name %s completed:\n",
				       prim->deviceAddr.nap,
				       prim->deviceAddr.uap,
				       prim->deviceAddr.lap,
				       name);
				CsrPmemFree(name);

                CL_API_LOGPrintf("CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM prim->info %x, prim->infoLen %d\n",prim->info, prim->infoLen );
				if(CsrBtSdIsServicePresent(prim->info, prim->infoLen, CSR_BT_HFG_PROFILE_UUID))
				{
				    CL_API_LOGPrintf("CSR_BT_HFG_PROFILE_UUID service available\n");
				}
				else
				{
				    CL_API_LOGPrintf("CSR_BT_HFG_PROFILE_UUID not available\n");
				}



				noOfServices = CsrBtSdReadServiceList(prim->info, prim->infoLen, &serviceList);


				while(noOfServices > 0)
				{
					CsrUint8 serviceLsb;
					noOfServices--;

					serviceLsb = (CsrUint8) serviceList[noOfServices]-1; /*only take lsb of service and substract 1 to index the table correctly*/

					switch(serviceList[noOfServices] & 0xFFFFFF00)
					{
						case 0x00001100:
						{
							CL_API_LOGPrintf(" CSR_BT_SD_READ_AVAILABLE CSR_BT_GAP_SERVICE_11 available %lx\n",serviceList[noOfServices]);
							

							break;
						}

						case 0x00001200:
						{
							CL_API_LOGPrintf(" CSR_BT_SD_READ_AVAILABLE CSR_BT_GAP_SERVICE_12 available %lx\n",serviceList[noOfServices]);

							break;
						}

						case 0x00001300:
						{
							CL_API_LOGPrintf(" CSR_BT_SD_READ_AVAILABLE CSR_BT_GAP_SERVICE_12 available %lx\n",serviceList[noOfServices]);

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

			CL_API_LOGPrintf("Read available services cancelled\n");
			break;
		}

		default:
		{
			CL_API_LOGPrintf("Unhandled SD primitive: 0x%04X\n", *primType);
			break;
		}
	}
	CsrPmemFree(inst->recvMsgP);
}

