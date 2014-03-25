/**********************************************************************
* File Name   : Bluetooth_CM.c
* Description : This file contains functions and variables for
*                Connection Manager.
* Author 	  :	Kim Jin Cheol	(Connectivity Team, HYUNDAI MOBIS)
* Updates			:
***********************************************************************/
#include "csr_pmem.h"
#include "csr_bt_cm_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_declare_tasks.h"
#include "Bluetooth_CM.h"
#include "Bluetooth_Common.h"


/*****************************************************************************
* Function Name : init_cm()
******************************************************************************
* Desription    :
* Parameter     :
* Return        :
******************************************************************************/
void init_cm(void **gash)
{
	CmAppInst *inst;

	*gash = CsrPmemAlloc(sizeof(CmAppInst));
	CsrMemSet(*gash, 0, sizeof(CmAppInst));
	inst = (CmAppInst *)*gash;
	
	inst->qid = TESTQUEUE;
	inst->recvMsgP = NULL;
	
    CsrBtCmSetLocalNameReqSend(inst->qid , (CsrUtf8String *) CsrStrDup("MY_TEST GET"));
	CsrBtCmWriteScanEnableReqSend(inst->qid ,FALSE,FALSE);
	CsrBtCmReadLocalBdAddrReqSend(inst->qid);

	    /* This message is ONLY needed in this demo application, and should NOT be used in real life products */
    //CsrBtCmWriteMajorMinorCodReqSend(TESTQUEUE, CSR_BT_AV_MAJOR_DEVICE_MASK|CSR_BT_PHONE_MAJOR_DEVICE_MASK, CSR_BT_HANDSFREE_AV_MINOR_DEVICE_CLASS);
	CsrBtCmWriteMajorMinorCodReqSend(inst->qid , CSR_BT_AV_MAJOR_DEVICE_MASK|CSR_BT_RENDERING_MAJOR_SERVICE_MASK, CSR_BT_HANDSFREE_AV_MINOR_DEVICE_CLASS);
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
void handleCmPrim(CmAppInst  *inst)
{
    CsrPrim    *primType;
    primType = (CsrPrim *) inst->recvMsgP;

    switch (*primType)
    {
        case CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM :
        {
            CsrBtCmReadLocalBdAddrCfm *prim = (CsrBtCmReadLocalBdAddrCfm * )inst->recvMsgP;
            CL_API_LOGPrintf("CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM BDADDR %04X:%02X:%06X\n",prim->deviceAddr.nap,prim->deviceAddr.uap,prim->deviceAddr.lap);
			break;
        }
        break;
	    case  CSR_BT_CM_SET_LOCAL_NAME_CFM:
	    {
	    
	    	CsrBtCmSetLocalNameCfm *prim =  (CsrBtCmSetLocalNameCfm *)inst->recvMsgP;
	    	CL_API_LOGPrintf("CSR_BT_CM_SET_LOCAL_NAME_CFM\n");
			break;
	    }
	    case CSR_BT_CM_WRITE_COD_CFM :
	    {
	        CsrBtCmWriteCodCfm *prim = (CsrBtCmWriteCodCfm *)inst->recvMsgP;
			CL_API_LOGPrintf("CSR_BT_CM_WRITE_COD_CFM\n");

	        if (prim->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS &&
	            prim->resultSupplier == CSR_BT_SUPPLIER_CM )
	        {
	          //  printf("\nCOD - Health Major device class\n");
	        }
	        else
	        {
	           // printf("\nError- Setting Major device class\n");
	        }
			break;
	    }
	    case  CSR_BT_CM_WRITE_SCAN_ENABLE_CFM:
	    {	    
	    	CsrBtCmWriteScanEnableCfm *prim =  (CsrBtCmWriteScanEnableCfm *)inst->recvMsgP;
	    	CL_API_LOGPrintf("CSR_BT_CM_WRITE_SCAN_ENABLE_CFM\n");
			break;
	    }
	    break;
	    case CSR_BT_CM_ROLE_DISCOVERY_CFM:
        {
            CsrBtCmRoleDiscoveryCfm *prim = (CsrBtCmRoleDiscoveryCfm *)inst->recvMsgP;
            CL_API_LOGPrintf("CSR_BT_CM_WRITE_SCAN_ENABLE_CFM %x\n", prim->role);
            break;
        }   
	    default:
	        {
	            CL_API_LOGPrintf("####### default in CmPrim handler %x\n",*primType);
	            /*  unexpected primitive received   */
	        }
	 }   /*  end switch(primType)    */

}

/*****************************************************************************
*
*	Connection Management (CM)Command Function
*
*****************************************************************************/


/*****************************************************************************
* Function Name : Bluetooth_ScanEnable()
******************************************************************************
* Desription    : 
* Parameter     : 
* Return        :
******************************************************************************/
void	Bluetooth_ScanEnable(void)
{		
	CsrBtCmWriteScanEnableReqSend(TESTQUEUE, FALSE, FALSE);
}

/*****************************************************************************
* Function Name : Bluetooth_ScanDisable()
******************************************************************************
* Desription    : 
* Parameter     : 
* Return        :
******************************************************************************/
void	Bluetooth_ScanDisable(void)
{
	/*phandle: protocol handle
     disableInquiryScan:    TRUE disable inquiry scan
     disablePageScan:    TRUE disable page scan*/
	CsrBtCmWriteScanEnableReqSend(TESTQUEUE, TRUE, TRUE);
}


//#endif
