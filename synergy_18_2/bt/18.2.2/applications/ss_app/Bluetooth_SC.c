/**********************************************************************
* File Name   : Bluetooth_SC.c
* Description : This file define functions and variables for Security Controller.
* Updates			:
***********************************************************************/
#include "csr_bt_declare_tasks.h"
#include "csr_bt_sc_lib.h"
#include "csr_bt_sc_prim.h"
#include "Bluetooth_Common.h"
#include "csr_bt_sd_lib.h"
#include "Bluetooth_SC.h"
#include "csr_pmem.h"
#include "AppGaplib.h"

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
//	CsrUint8		 thePasskey[4]={'0','0','0','0'};

	*gash = CsrPmemAlloc(sizeof(ScAppInst));
	CsrMemSet(*gash, 0, sizeof(ScAppInst));
	inst = (ScAppInst *)*gash;

	inst->qid = TESTQUEUE;
#if 0	
	inst->pairingFromPeer=FALSE;

	inst->bondingDevAddr.lap=0; //For Passkey Indication from remote.
	inst->bondingDevAddr.uap=0;
	inst->bondingDevAddr.nap=0;

	inst->passkeyPosition=0;
	inst->passkeyLength=0;


	CsrMemSet(inst->passkey,0,sizeof(PASSKEY_MAX_LEN));
#endif
	CsrBtScActivateReqSend(inst->qid);
//	CsrBtScSetAuthRequirementsReqSend(HCI_MITM_REQUIRED_GENERAL_BONDING);
//	CsrBtScSetIoCapabilityReqSend(HCI_IO_CAP_NO_INPUT_NO_OUTPUT);

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
void handleScPrim(ScAppInst  *inst,CsrPrim    *primType)
{

	switch (*primType)
	{
		/* The SC must have the passkey to use for the pairing procedure */
		case CSR_BT_SC_PASSKEY_IND:
		{
 
            CsrBtScPasskeyInd *prim;
			CsrBtDeviceName             name;
			CsrUint8         thePasskey[4]={'0','0','0','0'};

			prim = (CsrBtScPasskeyInd *)primType;
			CsrStrNCpyZero((CsrCharString *)name, (const CsrCharString *)prim->deviceName, sizeof(CsrBtDeviceName));

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0," receive CSR_BT_SC_PASSKEY_IND.."));			
			CsrAppBtScPassKeyInd(&(prim->deviceAddr),&name,&(prim->paired),prim->cod);

		//	CsrBtScPasskeyResSend(TRUE,prim->deviceAddr,4,thePasskey,TRUE,FALSE);
			CsrPmemFree(prim->deviceName);
			prim->deviceName=NULL;

			break;
		}


	    case CSR_BT_SC_SSP_PASSKEY_IND:
		{
 
            CsrBtScSspPasskeyInd *prim;
			prim = (CsrBtScSspPasskeyInd *)primType;
	
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0," receive CSR_BT_SC_SSP_PASSKEY_IND."));
			CsrBtScSspPairingResSend(prim->deviceAddr,TRUE,TRUE,FALSE);
			CsrPmemFree(prim->deviceName);
			prim->deviceName=NULL;

			break;
		}



	    case CSR_BT_SC_SSP_COMPARE_IND:
		{
 
            CsrBtScSspCompareInd *prim;
			prim = (CsrBtScSspCompareInd *)primType;
	
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"receive CSR_BT_SC_SSP_COMPARE_IND.. numericValue [%ld]",prim->numericValue));
			CsrBtScSspCompareResSend(TRUE,prim->deviceAddr,TRUE,FALSE);
			CsrPmemFree(prim->deviceName);
			prim->deviceName=NULL;

			break;
		}
		case CSR_BT_SC_SSP_NOTIFICATION_IND:
		{
			CsrBtScSspKeypressNotificationInd *prim;
			prim = (CsrBtScSspKeypressNotificationInd *)primType;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"receive CSR_BT_SC_SSP_NOTIFICATION_IND..notificationType[%ld]" ,prim->notificationType));

			CsrBtScSspNotificationResSend(TRUE, prim->deviceAddr, TRUE, FALSE);

			break;
		}

	    case CSR_BT_SC_SSP_JUSTWORKS_IND:
		{
 
            CsrBtScSspJustworksInd *prim;
			CsrBtDeviceName             name;
			
			prim = (CsrBtScSspJustworksInd *)primType;
			CsrUtf8StrCpy(name, prim->deviceName);
			CsrAppBtScSspJustworksInd(&(prim->deviceAddr),&(name),&prim->paired,prim->cod);
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"receive CSR_BT_SC_SSP_JUSTWORKS_IND.."));
			
		//	CsrBtScSspJustWorksResSend(TRUE,prim->deviceAddr,TRUE,FALSE);
			CsrPmemFree(prim->deviceName);
			prim->deviceName=NULL;

			break;
		}
		case CSR_BT_SC_REBOND_IND:
		{
 
            CsrBtScRebondInd *prim;
			prim = (CsrBtScRebondInd *)primType;
	
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"receive CSR_BT_SC_REBOND_IND.."));
			CsrBtScRebondResSend(prim->deviceAddr,TRUE);
			CsrPmemFree(prim->deviceName);
			prim->deviceName=NULL;

			break;
		}	
		case CSR_BT_SC_BOND_CFM:
		{
			/* Bonding completed. Result is indicated. If failed then exit */
		    CsrBtScBondCfm *prim = (CsrBtScBondCfm *)primType;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"receive CSR_BT_SC_BOND_CFM..\n"));
			//CsrAppBtScBondCfm(prim->deviceAddr,);
		    
	
			break;
		}

		case CSR_BT_SC_AUTHORISE_IND:
		{
			CsrBtScAuthoriseInd    *prim;
			CsrBtDeviceName             name;
			
			prim = (CsrBtScAuthoriseInd *)primType;

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"receive CSR_BT_SC_AUTHORISE_IND..\n"));
			CsrUtf8StrCpy(name, prim->deviceName);
			CsrAppBtScAuthorizeInd(&(prim->deviceAddr),&name,&(prim->serviceId));
			CsrPmemFree(prim->deviceName);
			//CsrBtScAuthoriseResSend(TRUE,prim->deviceAddr);
			prim->deviceName=NULL;
			break;
		}
		case CSR_BT_SC_AUTHORISE_EXT_IND:
		{
			CsrBtScAuthoriseExtInd    *prim;
			CsrBtDeviceName             name;
			prim = (CsrBtScAuthoriseExtInd *)primType;

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"receive CSR_BT_SC_AUTHORISE_EXT_IND..\n"));
			CsrUtf8StrCpy(name, prim->deviceName);
			CsrAppBtScAuthorizeInd(&(prim->deviceAddr),&(name),&(prim->serviceId));
			CsrPmemFree(prim->deviceName);
			prim->deviceName=NULL;
			break;
		}

		case CSR_BT_SC_BOND_IND:
		{
		    CsrBtScBondInd *prim= (CsrBtScBondInd *) primType;
			CsrBtScDbDeviceRecord record;
			CsrBtScDbDeviceRecord *send_record=NULL;
		//	CsrBool boolresult=FALSE;

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Bond indication received from device %04X:%02X:%06X addr type [%x]",
				prim->deviceAddr.nap,
				prim->deviceAddr.uap,
				prim->deviceAddr.lap,
				prim->addressType));				
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"receive CSR_BT_SC_BOND_IND result [%x].. supplie[%x]",prim->resultCode ,prim->resultSupplier));
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Cod value [%x],addedToScDbList [%d]",prim->cod,prim->addedToScDbList));

//			inst->bondingDevAddr=prim->deviceAddr;
			
			if(prim->addedToScDbList)
			{
				CsrBtScDbRead(&(prim->deviceAddr),&record);
				send_record=&record;
			}	
			CsrAppBtScBondInd(&(prim->addedToScDbList),&(prim->deviceAddr),prim->resultCode,prim->resultSupplier,send_record);	
			break;
		}
        case CSR_BT_SC_DEBUG_MODE_CFM:
		{
			CsrBtScDebugModeCfm *prim =(CsrBtScDebugModeCfm *) primType;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SC_DEBUG_MODE_CFM ealble[%x]",  prim->enabled));
			break;
        }
		case CSR_BT_SC_DEBOND_CFM:
		{
			CsrBtScDebondCfm *prim =(CsrBtScDebondCfm *) primType;
	//			CsrBool boolresult=FALSE;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SC_DEBOND_CFM resultCode[%x] ,resultSupplier [%x]",  prim->resultCode, prim->resultSupplier));

			CsrAppBtScDeBondCfm(prim->deviceAddr,prim->resultCode,prim->resultSupplier);
			break;
        }
	
		case CSR_BT_SC_SET_SECURITY_MODE_CFM:
		{

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SC_SET_SECURITY_MODE_CFM \n"));
			break;
        }	

		case CSR_BT_SC_ENCRYPTION_CFM:
	    {
			CsrBtScEncryptionCfm *prim = (CsrBtScEncryptionCfm*) primType;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SC_ENCRYPTION_CFM encEnabled[%d] resultCode[%x] ,resultSupplier [%x]",prim->encEnabled,  prim->resultCode, prim->resultSupplier));
			
			CsrAppBtScEncryptionCfm(prim->deviceAddr,prim->resultCode,prim->resultSupplier);

		
			break;
        }
#if 0			
		case CSR_BT_SC_READ_DEVICE_RECORD_IND:
	    {

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SC_READ_DEVICE_RECORD_IND \n"));
			break;
        }
		case CSR_BT_SC_READ_DEVICE_RECORD_CFM:
		{

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SC_READ_DEVICE_RECORD_CFM \n"));
			break;
        }
		case CSR_BT_SC_UPDATE_TRUST_LEVEL_IND:
		{

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SC_UPDATE_TRUST_LEVEL_IND \n"));
			break;
        }	
		case CSR_BT_SC_AUTHENTICATE_CFM:
		{

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SC_AUTHENTICATE_CFM \n"));
			break;
        }
		case CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM:
		{

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM \n"));
			break;
        }
		case CSR_BT_SC_SSP_PAIRING_IND:
		{

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM \n"));
			break;
        }
		case CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND:
		{

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND \n"));
			break;
        }
		case CSR_BT_SC_MODE_IND:
		{

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SC_MODE_IND \n"));
			break;
        }	
		case CSR_BT_SC_DATABASE_CFM:
		{

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SC_DATABASE_CFM \n"));
			break;
        }	

		case CSR_BT_SC_CANCEL_ACCEPT_BOND_IND:
		{

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SC_CANCEL_ACCEPT_BOND_IND \n"));
			break;
        }
			
		case CSR_BT_SC_LE_SECURITY_IND:
		{

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SC_LE_SECURITY_IND \n"));
			break;
        }
		case CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM:
		{

			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM \n"));
			break;
        }
			
#endif
		default:
		{
			/* Unexpected primitive received */
			CsrBtScFreeUpstreamMessageContents(CSR_BT_SC_PRIM, primType);
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"Unhandled SC primitive: 0x%04x", *primType));
		}
	}
    

}
