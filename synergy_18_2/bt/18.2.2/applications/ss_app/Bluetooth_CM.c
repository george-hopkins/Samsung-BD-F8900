/**********************************************************************
* File Name   : Bluetooth_CM.c
* Description : This file contains functions and variables for
*                Connection Manager.
* Updates			:
***********************************************************************/
#include "csr_pmem.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_declare_tasks.h"
#include "Bluetooth_CM.h"
#include "Bluetooth_Common.h"
#include "AppGaplib.h"
/*****************************************************************************
* Function Name : init_cm()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
CsrBool isCSRChipInit=FALSE;
void init_cm(void **gash)
{
	CmAppInst *inst;

	*gash = CsrPmemAlloc(sizeof(CmAppInst));
	CsrMemSet(*gash, 0, sizeof(CmAppInst));
	inst = (CmAppInst *)*gash;
	inst->qid = TESTQUEUE;
	inst->isInit=FALSE;
	isCSRChipInit=FALSE;

//	CsrBtCmReadLocalVersionReqSend(inst->qid);
	CsrBtCmSetEventMaskReqSend(inst->qid, CSR_BT_CM_EVENT_MASK_SUBSCRIBE_BLUECORE_INITIALIZED, CSR_BT_CM_EVENT_MASK_COND_ALL);

    CsrBtCmReadLocalBdAddrReqSend(inst->qid);
    CsrBtCmReadLocalNameReqSend(inst->qid);
	CsrBtCmWriteScanEnableReqSend(inst->qid,TRUE,TRUE);

	/* This message is ONLY needed in this demo application, and should NOT be used in real life products */
	CsrBtCmWriteMajorMinorCodReqSend(inst->qid , CSR_BT_AV_MAJOR_DEVICE_MASK, CSR_BT_WEARABLE_HEADSET_AV_MINOR_DEVICE_CLASS);
	CsrBtCmWriteServiceCodReqSend(inst->qid,CSR_BT_AUDIO_MAJOR_SERVICE_MASK|CSR_BT_RENDERING_MAJOR_SERVICE_MASK);
}


/*****************************************************************************
* Function Name : deinit_cm()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
void deinit_cm(void **gash)
{
	CmAppInst *inst;
	

	inst = (CmAppInst *)*gash;
	CsrPmemFree(inst);
}


/*****************************************************************************
* Function Name : handleCmPrim()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
void handleCmPrim(CmAppInst  *inst,CsrPrim    *primType)
{


    switch (*primType)
    {
        case CSR_BT_CM_SET_EVENT_MASK_CFM:
		{
	//		CsrBtCmSetEventMaskCfm *prim = (CsrBtCmReadLocalBdAddrCfm * )primType;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_CM_SET_EVENT_MASK_CFM"));
			break;
        }
        case CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM :
        {
            CsrBtCmReadLocalBdAddrCfm *prim = (CsrBtCmReadLocalBdAddrCfm * )primType;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM %04X:%02X:%06X",
				prim->deviceAddr.nap,
				prim->deviceAddr.uap,
				prim->deviceAddr.lap));
			CsrMemCpy(&(inst->localDeviceAddr), &(prim->deviceAddr), sizeof(CsrBtDeviceAddr));
			CsrAppBtGapCmLocalBdaddrCfm(&prim->deviceAddr);
            
			break;
        }
        break;
	    case  CSR_BT_CM_SET_LOCAL_NAME_CFM:
	    {
	    
//	    	CsrBtCmSetLocalNameCfm *prim =  (CsrBtCmSetLocalNameCfm *)primType;

	    	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_CM_SET_LOCAL_NAME_CFM"));
            
			break;
	    }

		case CSR_BT_CM_READ_LOCAL_NAME_CFM:
		{
			CsrBtCmReadLocalNameCfm *prim =  (CsrBtCmReadLocalNameCfm *)primType;
			CsrUtf8StrCpy(inst->localName, prim->localName);
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_CM_READ_LOCAL_NAME_CFM"));
			CsrAppBtGapCmLocalNameCfm(&(inst->localName));
			CsrPmemFree(prim->localName);
			break;
		}
		case CSR_BT_CM_BLUECORE_INITIALIZED_IND:
		{
//			CsrBtCmBluecoreInitializedInd *prim = (CsrBtCmBluecoreInitializedInd *)primType;
			CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_CM_BLUECORE_INITIALIZED_IND"));
			inst->isInit=TRUE;
			isCSRChipInit=TRUE;
			CsrAppBtGapInitInd();
			break;
		}
	    case CSR_BT_CM_WRITE_COD_CFM :
	    {
	        CsrBtCmWriteCodCfm *prim = (CsrBtCmWriteCodCfm *)primType;

	        if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
	            prim->resultSupplier == CSR_BT_SUPPLIER_CM )
	        {
	          CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_CM_WRITE_COD_CFM success"));
	        }
	        else
	        {
	          CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_CM_WRITE_COD_CFM fail resultCode [%x]resultSupplier [%x]", prim->resultCode,prim->resultSupplier));
	        }
			break;
	    }
	    case  CSR_BT_CM_WRITE_SCAN_ENABLE_CFM:
	    {	    
//	    	CsrBtCmWriteScanEnableCfm *prim =  (CsrBtCmWriteScanEnableCfm *)primType;
	    	CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_CM_WRITE_SCAN_ENABLE_CFM"));
			break;
	    }
	    break;
	    case CSR_BT_CM_ROLE_DISCOVERY_CFM:
        {
        //    CsrBtCmRoleDiscoveryCfm *prim = (CsrBtCmRoleDiscoveryCfm *)primType;
            CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_CM_ROLE_DISCOVERY_CFM"));
            break;
        }
		case CSR_BT_CM_WRITE_PAGE_TO_CFM:
		{
			 CsrBtCmWritePageToCfm *prim = (CsrBtCmRoleDiscoveryCfm *)primType;
            CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"CSR_BT_CM_WRITE_PAGE_TO_CFM resultCode [%x]resultSupplier [%x]", prim->resultCode,prim->resultSupplier));
			break;
		}
	    default:
        {
			CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, primType);
            CSR_LOG_TEXT_DEBUG((CSR_SS_AP_LOG_ID,0,"####### default in CmPrim handler %x",*primType));
            /*  unexpected primitive received   */
        }
	 }   /*  end switch(primType)    */
}

