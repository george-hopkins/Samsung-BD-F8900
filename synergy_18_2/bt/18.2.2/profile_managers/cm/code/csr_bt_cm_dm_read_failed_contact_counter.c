/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_l2cap.h"

#ifdef CSR_BT_INSTALL_CM_READ_FAILED_CONTACT_COUNTER
void CsrBtCmReadFailedContactCounterReqHandler(cmInstanceData_t * cmData)
{
    CsrBtCmReadFailedContactCounterReq * cmPrim;

    cmPrim = (CsrBtCmReadFailedContactCounterReq *) cmData->recvMsgP;

    cmData->dmVar.appHandle = cmPrim->appHandle;
    dm_hci_failed_contact_counter(&cmPrim->deviceAddr, NULL);
}

static void csrBtCmReadFailedContactCounterCfmSend(CsrSchedQid                  appHandle,
                                                   CsrBtDeviceAddr      deviceAddr,
                                                   CsrBtResultCode      resultCode,
                                                   CsrBtSupplier  resultSupplier,
                                                   CsrUint16             failedContactCount)
{
    CsrBtCmReadFailedContactCounterCfm        *prim;

    prim                        = (CsrBtCmReadFailedContactCounterCfm *) CsrPmemAlloc(sizeof(CsrBtCmReadFailedContactCounterCfm));
    prim->type                  = CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_CFM;
    prim->deviceAddr            = deviceAddr;
    prim->failedContactCount    = failedContactCount;
    prim->resultCode            = resultCode;
    prim->resultSupplier        = resultSupplier;;
    CsrBtCmPutMessage(appHandle, prim);
}

void CsrBtCmDmHciReadFailedContactCounterCompleteHandler(cmInstanceData_t * cmData)
{
    DM_HCI_READ_FAILED_CONTACT_COUNT_CFM_T     *dmPrim;

    dmPrim = (DM_HCI_READ_FAILED_CONTACT_COUNT_CFM_T *) cmData->recvMsgP;

    if (dmPrim->status == HCI_SUCCESS)
    {
        csrBtCmReadFailedContactCounterCfmSend(cmData->dmVar.appHandle, dmPrim->bd_addr,
            CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM, dmPrim->failed_contact_count);
    }
    else
    {
        csrBtCmReadFailedContactCounterCfmSend(cmData->dmVar.appHandle, dmPrim->bd_addr,
            (CsrBtResultCode)dmPrim->status, CSR_BT_SUPPLIER_HCI, dmPrim->failed_contact_count);
    }
    CsrBtCmDmLocalQueueHandler();
}
#endif
