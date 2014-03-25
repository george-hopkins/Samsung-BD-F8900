/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_util.h"

void CsrBtCmDmHciReadBdAddrCompleteHandler(cmInstanceData_t *cmData)
{ /* This event is the confirmation that the host controller was read
     (or try to) the local device address. Build and send
     CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM to the application */
    CsrBtCmReadLocalBdAddrCfm        *prim;
    DM_HCI_READ_BD_ADDR_CFM_T    *dmPrim;

    dmPrim        = (DM_HCI_READ_BD_ADDR_CFM_T *)cmData->recvMsgP;
    prim        = (CsrBtCmReadLocalBdAddrCfm *)CsrPmemAlloc(sizeof(CsrBtCmReadLocalBdAddrCfm));

    prim->type    = CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM;
    if(dmPrim->status == HCI_SUCCESS)
    {
        prim->deviceAddr = dmPrim->bd_addr;
    }
    else
    {
        bd_addr_zero(&(prim->deviceAddr));
    }
    CsrBtCmPutMessage(cmData->dmVar.appHandle, prim);
    CsrBtCmDmLocalQueueHandler();
}

void CsrBtCmReadBdAddrReqHandler(cmInstanceData_t *cmData)
{ /* This event indicates that the application desired to know the local
     device address. */
    CsrBtCmReadLocalBdAddrReq *cmPrim;

    cmPrim    = (CsrBtCmReadLocalBdAddrReq *) cmData->recvMsgP;

    cmData->dmVar.appHandle        = cmPrim->phandle;
    dm_hci_read_bd_addr(NULL);
}
