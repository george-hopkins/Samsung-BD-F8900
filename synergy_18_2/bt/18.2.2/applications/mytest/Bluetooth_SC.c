/**********************************************************************
* File Name   : Bluetooth_SC.c
* Description : This file define functions and variables for Security Controller.
* Author 	  :	Kim Jin Cheol	(Connectivity Team, HYUNDAI MOBIS)
* Updates			:
***********************************************************************/
#include "csr_bt_declare_tasks.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sc_prim.h"
#include "Bluetooth_Common.h"
#include "csr_bt_sd_lib.h"
#include "Bluetooth_SC.h"
#include "csr_pmem.h"





/*****************************************************************************
* Function Name : init_sc()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
void init_sc(void **gash)
{
	ScAppInst *inst;

	*gash = CsrPmemAlloc(sizeof(ScAppInst));
	CsrMemSet(*gash, 0, sizeof(ScAppInst));
	inst = (ScAppInst *)*gash;

	inst->qid = TESTQUEUE;
	inst->pairingFromPeer=FALSE;

	inst->bondingDevAddr.lap=0; //For Passkey Indication from remote.
	inst->bondingDevAddr.uap=0;
	inst->bondingDevAddr.nap=0;

	inst->passkeyPosition=0;
	inst->passkeyLength=0;


	CsrMemSet(inst->passkey,0,sizeof(PASSKEY_MAX_LEN));

	CsrBtScActivateReqSend(inst->qid);
//	CsrBtScSetAuthRequirementsReqSend(HCI_MITM_REQUIRED_GENERAL_BONDING);
//	CsrBtScSetIoCapabilityReqSend(HCI_IO_CAP_DISPLAY_YES_NO);


	//inst->scDbListNum = 0;
	//inst->selectedIndex = 0;
	//memset(inst->scDbList,0,sizeof(scDbDeviceRecord_t)*MAX_NUM_OF_SCDB);
	//ScActivateReqSend(inst->qid);
}

/*****************************************************************************
* Function Name : deinit_sc()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
void deinit_sc(void **gash)
{
	ScAppInst *inst;

	inst = (ScAppInst *)*gash;

	CsrPmemFree(inst);
}

/*****************************************************************************
* Function Name : handleScPrim()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
void handleScPrim(ScAppInst  *inst)
{
    CsrPrim    *primType;
    primType = (CsrPrim *) inst->recvMsgP;

	switch (*primType)
	{
		/* The SC must have the passkey to use for the pairing procedure */
		case CSR_BT_SC_PASSKEY_IND:
		{
 
            CsrBtScPasskeyInd *prim;
			CsrUint8         thePasskey[CSR_BT_PASSKEY_MAX_LEN]={'0','0','0','0'};

			CL_API_LOGPrintf(" receive CSR_BT_SC_PASSKEY_IND..\r\n");
			
			prim = (CsrBtScPasskeyInd *)inst->recvMsgP;

			CsrBtScPasskeyResSend(TRUE,prim->deviceAddr,4,thePasskey,TRUE,TRUE);
			CsrPmemFree(prim->deviceName);
			prim->deviceName=NULL;

			break;
		}


	    case CSR_BT_SC_SSP_PASSKEY_IND:
		{
 
            CsrBtScSspPasskeyInd *prim;
			prim = (CsrBtScSspPasskeyInd *)inst->recvMsgP;
	
			CL_API_LOGPrintf(" receive CSR_BT_SC_SSP_PASSKEY_IND..\r\n");
			CsrBtScSspPairingResSend(prim->deviceAddr,TRUE,TRUE,TRUE);
			CsrPmemFree(prim->deviceName);
			prim->deviceName=NULL;

			break;
		}



	    case CSR_BT_SC_SSP_COMPARE_IND:
		{
 
            CsrBtScSspCompareInd *prim;
			prim = (CsrBtScSspCompareInd *)inst->recvMsgP;
	
			CL_API_LOGPrintf("receive CSR_BT_SC_SSP_COMPARE_IND..\n");
			CsrBtScSspCompareResSend(TRUE,prim->deviceAddr,TRUE,TRUE);
			CsrPmemFree(prim->deviceName);
			prim->deviceName=NULL;

			break;
		}
		case CSR_BT_SC_SSP_NOTIFICATION_IND:
		{
			CsrBtScSspKeypressNotificationInd *prim;
			prim = (CsrBtScSspKeypressNotificationInd *)inst->recvMsgP;
			CL_API_LOGPrintf("receive CSR_BT_SC_SSP_NOTIFICATION_IND..\n");

			CsrBtScSspNotificationResSend(TRUE, prim->deviceAddr, TRUE, TRUE);

			break;
		}

	    case CSR_BT_SC_SSP_JUSTWORKS_IND:
		{
 
            CsrBtScSspJustworksInd *prim;
			prim = (CsrBtScSspJustworksInd *)inst->recvMsgP;
	
			CL_API_LOGPrintf("\r\n receive CSR_BT_SC_SSP_JUSTWORKS_IND..\n");
			CsrBtScSspCompareResSend(TRUE,prim->deviceAddr,TRUE,TRUE);
			CsrPmemFree(prim->deviceName);
			prim->deviceName=NULL;

			break;
		}
		case CSR_BT_SC_REBOND_IND:
		{
 
            CsrBtScRebondInd *prim;
			prim = (CsrBtScRebondInd *)inst->recvMsgP;
	
			CL_API_LOGPrintf("\r\n receive CSR_BT_SC_REBOND_IND..\n");
			CsrBtScRebondResSend(prim->deviceAddr,TRUE);
			CsrPmemFree(prim->deviceName);
			prim->deviceName=NULL;

			break;
		}	
		case CSR_BT_SC_BOND_CFM:
		{
			/* Bonding completed. Result is indicated. If failed then exit */
		    CsrBtScBondCfm *prim;
		    prim = (CsrBtScBondCfm *)inst->recvMsgP;
		    
			CL_API_LOGPrintf("receive CSR_BT_SC_BOND_CFM..\n");
			break;
		}

		case CSR_BT_SC_AUTHORISE_IND:
		{
			CsrBtScAuthoriseInd    *prim;
			prim = (CsrBtScAuthoriseInd *)inst->recvMsgP;

			CL_API_LOGPrintf("receive CSR_BT_SC_AUTHORISE_IND..\n");
			CsrBtScAuthoriseResSend(TRUE, prim->deviceAddr);
	        CsrPmemFree(prim->deviceName);
			//CsrBtScAuthoriseResSend(TRUE,prim->deviceAddr);
			prim->deviceName=NULL;
			break;
		}
		case CSR_BT_SC_AUTHORISE_EXT_IND:
		{
			CsrBtScAuthoriseExtInd    *prim;
			prim = (CsrBtScAuthoriseExtInd *)inst->recvMsgP;

			CL_API_LOGPrintf("receive CSR_BT_SC_AUTHORISE_EXT_IND..\n");
			CsrBtScAuthoriseResSend(TRUE, prim->deviceAddr);
			CsrPmemFree(prim->deviceName);
			prim->deviceName=NULL;
			break;
		}

		case CSR_BT_SC_BOND_IND:
		{
		    CsrBtScBondInd *prim;
		    prim    = (CsrBtScBondInd *) inst->recvMsgP;


			CL_API_LOGPrintf("\r\n Bond indication received from device %04X:%02X:%06X\n",
				prim->deviceAddr.nap,
				prim->deviceAddr.uap,
				prim->deviceAddr.lap);				
			CL_API_LOGPrintf("receive CSR_BT_SC_BOND_IND result [%x]..\n",prim->resultCode);

				inst->bondingDevAddr=prim->deviceAddr;

			//CsrBtSdReadAvailableServicesReqSend(inst->qid, prim->deviceAddr, CSR_BT_SD_SERVICE_USE_STANDARD, 0, NULL);
			break;
		}
        case CSR_BT_SC_DEBUG_MODE_CFM:
		{
			CsrBtScDebugModeCfm *prim =(CsrBtScDebugModeCfm *) inst->recvMsgP;
			CL_API_LOGPrintf("\r\n CSR_BT_SC_DEBUG_MODE_CFM ealble[%x] \n",  prim->enabled);
			break;
        }
		case CSR_BT_SC_DEBOND_CFM:
		{
			CsrBtScDebondCfm *prim =(CsrBtScDebondCfm *) inst->recvMsgP;
			CL_API_LOGPrintf("\r\n CSR_BT_SC_DEBOND_CFM %x \n",  prim->resultCode);
			break;
        }	
#if 0		
		case CSR_BT_SC_SET_SECURITY_MODE_CFM:
		{

			CL_API_LOGPrintf("CSR_BT_SC_SET_SECURITY_MODE_CFM \n");
			break;
        }	

		case CSR_BT_SC_ENCRYPTION_CFM:
	    {

			CL_API_LOGPrintf("CSR_BT_SC_ENCRYPTION_CFM \n");
			break;
        }
		case CSR_BT_SC_READ_DEVICE_RECORD_IND:
	    {

			CL_API_LOGPrintf("CSR_BT_SC_READ_DEVICE_RECORD_IND \n");
			break;
        }
		case CSR_BT_SC_READ_DEVICE_RECORD_CFM:
		{

			CL_API_LOGPrintf("CSR_BT_SC_READ_DEVICE_RECORD_CFM \n");
			break;
        }
		case CSR_BT_SC_UPDATE_TRUST_LEVEL_IND:
		{

			CL_API_LOGPrintf("CSR_BT_SC_UPDATE_TRUST_LEVEL_IND \n");
			break;
        }	
		case CSR_BT_SC_AUTHENTICATE_CFM:
		{

			CL_API_LOGPrintf("CSR_BT_SC_AUTHENTICATE_CFM \n");
			break;
        }
		case CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM:
		{

			CL_API_LOGPrintf("CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM \n");
			break;
        }
		case CSR_BT_SC_SSP_PAIRING_IND:
		{

			CL_API_LOGPrintf("CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM \n");
			break;
        }
		case CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND:
		{

			CL_API_LOGPrintf("CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND \n");
			break;
        }
		case CSR_BT_SC_MODE_IND:
		{

			CL_API_LOGPrintf("CSR_BT_SC_MODE_IND \n");
			break;
        }	
		case CSR_BT_SC_DATABASE_CFM:
		{

			CL_API_LOGPrintf("CSR_BT_SC_DATABASE_CFM \n");
			break;
        }	

		case CSR_BT_SC_CANCEL_ACCEPT_BOND_IND:
		{

			CL_API_LOGPrintf("CSR_BT_SC_CANCEL_ACCEPT_BOND_IND \n");
			break;
        }
			
		case CSR_BT_SC_LE_SECURITY_IND:
		{

			CL_API_LOGPrintf("CSR_BT_SC_LE_SECURITY_IND \n");
			break;
        }
		case CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM:
		{

			CL_API_LOGPrintf("CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM \n");
			break;
        }
			
			
#endif
		default:
		{
			/* Unexpected primitive received */
			CsrBtScFreeUpstreamMessageContents(CSR_BT_SC_PRIM, inst->recvMsgP);
		
		}
	}
    CsrPmemFree(inst->recvMsgP);
    inst->recvMsgP=NULL;

}


