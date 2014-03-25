/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"

#ifdef CSR_BT_INSTALL_CM_DUT_MODE
void CsrBtCmSendDeviceUnderTestComplete(CsrSchedQid appHandle, CsrUint8 status, CsrUint8 step)
{
    CsrBtCmEnableDutModeCfm        *prim;

    prim                = (CsrBtCmEnableDutModeCfm *)CsrPmemAlloc(sizeof(CsrBtCmEnableDutModeCfm));
    prim->type          = CSR_BT_CM_ENABLE_DUT_MODE_CFM;
    prim->stepNumber    = step;

    if (status == HCI_SUCCESS)
    {
        prim->resultCode     = CSR_BT_RESULT_CODE_CM_SUCCESS;
        prim->resultSupplier = CSR_BT_SUPPLIER_CM;
    }
    else
    {
        prim->resultCode     = (CsrBtResultCode) status;
        prim->resultSupplier = CSR_BT_SUPPLIER_HCI;
    }
    CsrBtCmPutMessage(appHandle, prim);
    CsrBtCmDmLocalQueueHandler();
}

void CsrBtCmDeviceUnderTestReqHandler(cmInstanceData_t *cmData)
{ /* This event indicates that the application desired to the device */
    CsrBtCmEnableDutModeReq *cmPrim;
    CONDITION_T condition;

    cmPrim    = (CsrBtCmEnableDutModeReq *) cmData->recvMsgP;
    condition.auto_accept            = HCI_COND_SETUP_AUTO_ACCEPT_FLAG_RS_OFF;
    dm_hci_set_event_filter(CONNECTION_FILTER, ALL_CONNECTION, &condition, NULL);
    cmData->dmVar.deviceUnderTest    = TRUE;
    cmData->dmVar.appHandle          = cmPrim->appHandle;

}

void CsrBtCmDmHciDeviceUnderTestCompleteHandler(cmInstanceData_t *cmData)
{
    DM_HCI_ENABLE_DUT_MODE_CFM_T    *dmPrim;

    dmPrim        = (DM_HCI_ENABLE_DUT_MODE_CFM_T *)cmData->recvMsgP;
    CsrBtCmSendDeviceUnderTestComplete(cmData->dmVar.appHandle, dmPrim->status, 3);
}

void CsrBtCmDeviceUnderTestDisableReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmDisableDutModeReq *cmPrim = (CsrBtCmDisableDutModeReq *)cmData->recvMsgP;
    CsrBtCmDisableDutModeCfm *prim;

    cmData->dmVar.deviceUnderTest    = FALSE;
    
    prim                 = (CsrBtCmDisableDutModeCfm *)CsrPmemAlloc(sizeof(CsrBtCmDisableDutModeCfm));
    prim->type           = CSR_BT_CM_DISABLE_DUT_MODE_CFM;
    prim->resultCode     = CSR_BT_RESULT_CODE_CM_SUCCESS;
    prim->resultSupplier = CSR_BT_SUPPLIER_CM;

    CsrBtCmPutMessage(cmPrim->appHandle, prim);    
    CsrBtCmDmLocalQueueHandler();
}
#endif
