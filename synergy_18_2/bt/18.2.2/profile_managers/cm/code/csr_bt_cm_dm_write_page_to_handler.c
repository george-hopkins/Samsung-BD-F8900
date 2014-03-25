/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"

void CsrBtCmDmHciWritePageToCompleteHandler(cmInstanceData_t * cmData)
{
#ifdef CSR_BT_INSTALL_CM_WRITE_PAGE_SCAN
    DM_HCI_WRITE_PAGE_TIMEOUT_CFM_T * dmPrim;

    dmPrim = (DM_HCI_WRITE_PAGE_TIMEOUT_CFM_T *) cmData->recvMsgP;

    if(cmData->globalState == notReady_s)
#endif
    {
        /* Continue CM initialisation even if the request failed */
        cmData->dmVar.fallbackPerformed = FALSE;
        dm_hci_write_pagescan_activity(CSR_BT_PAGE_SCAN_INTERVAL_DEFAULT, CSR_BT_PAGE_SCAN_WINDOW_DEFAULT, NULL);
    }
#ifdef CSR_BT_INSTALL_CM_WRITE_PAGE_TO
    else
    { /* The request came from the application */
        CsrBtCmWritePageToCfm * prim = (CsrBtCmWritePageToCfm *)CsrPmemAlloc(sizeof(CsrBtCmWritePageToCfm));
        prim->type                   = CSR_BT_CM_WRITE_PAGE_TO_CFM;

        if (dmPrim->status == HCI_SUCCESS)
        {
            prim->resultCode         = CSR_BT_RESULT_CODE_CM_SUCCESS;
            prim->resultSupplier     = CSR_BT_SUPPLIER_CM;
        }
        else
        {
            prim->resultCode         = (CsrBtResultCode) dmPrim->status;
            prim->resultSupplier     = CSR_BT_SUPPLIER_HCI;
        }
        CsrBtCmPutMessage(cmData->dmVar.appHandle, prim);
        CsrBtCmDmLocalQueueHandler();
    }
#endif    
}

#ifdef CSR_BT_INSTALL_CM_WRITE_PAGE_TO
void CsrBtCmDmWritePageToReqHandler(cmInstanceData_t * cmData)
{
    CsrBtCmWritePageToReq * cmPrim;

    cmPrim = (CsrBtCmWritePageToReq *) cmData->recvMsgP;

    cmData->dmVar.appHandle = cmPrim->appHandle;
    dm_hci_write_page_to(cmPrim->pageTimeout, NULL);
}
#endif
