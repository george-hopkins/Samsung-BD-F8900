/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"

void CsrBtCmDmHciReadLocalVersionCompleteHandler(cmInstanceData_t *cmData)
{
    DM_HCI_READ_LOCAL_VER_INFO_CFM_T    *dmPrim;

    dmPrim = (DM_HCI_READ_LOCAL_VER_INFO_CFM_T *) cmData->recvMsgP;

    if (dmPrim->status == HCI_SUCCESS)
    {
        cmData->dmVar.lmpVersion = dmPrim->lmp_version;
    }
    else
    {
        /* We require Bluetooth 1.2 */
        cmData->dmVar.lmpVersion = CSR_BT_BLUETOOTH_VERSION_1P2;
    }

    /* Next step in startup-sequence */
    dm_hci_read_local_features(NULL);
}

void CsrBtCmReadLocalVersionReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmReadLocalVersionCfm * prim;
    CsrBtCmReadLocalVersionReq * cmPrim;

    cmPrim           = (CsrBtCmReadLocalVersionReq *) cmData->recvMsgP;
    prim             = (CsrBtCmReadLocalVersionCfm *)CsrPmemAlloc(sizeof(CsrBtCmReadLocalVersionCfm));
    prim->type       = CSR_BT_CM_READ_LOCAL_VERSION_CFM;
    prim->lmpVersion = cmData->dmVar.lmpVersion;

    CsrBtCmPutMessage(cmPrim->appHandle, prim);
    CsrBtCmDmLocalQueueHandler();
}

