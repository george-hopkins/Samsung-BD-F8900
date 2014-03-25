/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#ifdef CSR_BT_INSTALL_CM_GET_LINK_QUALITY
#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"

void CsrBtCmDmHciGetLinkQualityCompleteHandler(cmInstanceData_t *cmData)
{
    DM_HCI_GET_LINK_QUALITY_CFM_T * dmPrim = (DM_HCI_GET_LINK_QUALITY_CFM_T *) cmData->recvMsgP;
    CsrBtCmGetLinkQualityCfm      * cmPrim = (CsrBtCmGetLinkQualityCfm *)CsrPmemAlloc(sizeof(CsrBtCmGetLinkQualityCfm));

    cmPrim->type        = CSR_BT_CM_GET_LINK_QUALITY_CFM;
    cmPrim->deviceAddr  = dmPrim->bd_addr;
    cmPrim->linkQuality = dmPrim->link_quality;

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

void CsrBtCmGetLinkQualityReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmGetLinkQualityReq * cmPrim = (CsrBtCmGetLinkQualityReq *) cmData->recvMsgP;
    cmData->dmVar.appHandle           = cmPrim->appHandle;
    dm_hci_get_link_quality(&cmPrim->deviceAddr, NULL);
}
#endif
