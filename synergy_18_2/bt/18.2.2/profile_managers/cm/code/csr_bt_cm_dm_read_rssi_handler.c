/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#ifdef CSR_BT_INSTALL_CM_READ_RSSI
#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"

void CsrBtCmDmHciReadRssiCompleteHandler(cmInstanceData_t *cmData)
{
    DM_HCI_READ_RSSI_CFM_T * dmPrim = (DM_HCI_READ_RSSI_CFM_T *) cmData->recvMsgP;
    CsrBtCmReadRssiCfm     * cmPrim = (CsrBtCmReadRssiCfm *)CsrPmemAlloc(sizeof(CsrBtCmReadRssiCfm));

    cmPrim->type        = CSR_BT_CM_READ_RSSI_CFM;
    cmPrim->deviceAddr  = dmPrim->addrt.addr;
    cmPrim->rssi        = dmPrim->rssi;
    cmPrim->addressType = dmPrim->addrt.type;

    if (dmPrim->status == HCI_SUCCESS)
    {
        cmPrim->resultCode      = CSR_BT_RESULT_CODE_CM_SUCCESS;
        cmPrim->resultSupplier  = CSR_BT_SUPPLIER_CM;
    }
    else
    {
        cmPrim->resultCode      = (CsrBtResultCode) dmPrim->status;
        cmPrim->resultSupplier  = CSR_BT_SUPPLIER_HCI;
    }

    CsrBtCmPutMessage(cmData->dmVar.appHandle, cmPrim);
    CsrBtCmDmLocalQueueHandler();
}

void CsrBtCmReadRssiReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmReadRssiReq * cmPrim = (CsrBtCmReadRssiReq *) cmData->recvMsgP;
    TYPED_BD_ADDR_T ad;
    cmData->dmVar.appHandle     = cmPrim->appHandle;
    ad.addr = cmPrim->deviceAddr;
    ad.type = cmPrim->addressType;
    dm_hci_read_rssi(&ad, NULL);
}
#endif
