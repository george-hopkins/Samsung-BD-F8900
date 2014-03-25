/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"
#ifdef CSR_BT_INSTALL_CM_READ_CLOCK

void CsrBtCmDmHciReadClockCompleteHandler(cmInstanceData_t *cmData)
{
    CsrBtCmReadClockCfm * cmPrim;
    DM_HCI_READ_CLOCK_CFM_T * dmPrim;

    cmPrim = (CsrBtCmReadClockCfm *)CsrPmemAlloc(sizeof(CsrBtCmReadClockCfm));
    dmPrim = (DM_HCI_READ_CLOCK_CFM_T *) cmData->recvMsgP;

    cmPrim->type     = CSR_BT_CM_READ_CLOCK_CFM;
    cmPrim->accuracy = dmPrim->accuracy;
    cmPrim->clock    = dmPrim->clock;
    cmPrim->bd_addr  = dmPrim->bd_addr;

    if (dmPrim->status == HCI_SUCCESS)
    {
        cmPrim->resultCode     = CSR_BT_RESULT_CODE_CM_SUCCESS;
        cmPrim->resultSupplier = CSR_BT_SUPPLIER_CM;
    }
    else
    {
        cmPrim->resultCode     = (CsrBtResultCode) dmPrim->status;
        cmPrim->resultSupplier = CSR_BT_SUPPLIER_HCI;
    }

    CsrBtCmPutMessage(cmData->dmVar.appHandle, cmPrim);
    CsrBtCmDmLocalQueueHandler();
}

void CsrBtCmReadClockReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmReadClockReq * cmPrim;

    cmPrim                  = (CsrBtCmReadClockReq *) cmData->recvMsgP;
    cmData->dmVar.appHandle = cmPrim->appHandle;
    dm_hci_read_clock(cmPrim->whichClock, &cmPrim->bd_addr, NULL);
}
#endif

