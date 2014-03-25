/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_l2cap.h"

static void csrBtCmDmWriteAutoFlushTimeoutMsgSend(CsrBtDeviceAddr *deviceAddr, CsrUint16 flushTo)
{
    CsrBtCmDmWriteAutoFlushTimeoutReq    *cmPrim;

    cmPrim          = pnew(CsrBtCmDmWriteAutoFlushTimeoutReq);
    cmPrim->type    = CSR_BT_CM_DM_WRITE_AUTO_FLUSH_TIMEOUT_REQ;
    cmPrim->flushTo = flushTo;
    bd_addr_copy(&cmPrim->deviceAddr, deviceAddr);
    CsrBtCmPutMessage(CSR_BT_CM_IFACEQUEUE, cmPrim);
}

void CsrBtCmWriteAutoFlushTimeout(cmInstanceData_t *cmData, CsrBtDeviceAddr *deviceAddr)
{
    CsrUint16 smallestFlushTo = returnL2capSmallestFlushTo(cmData, deviceAddr);
    aclTable *acl = returnAclTable(cmData, deviceAddr);
    CsrUint16 flushTo;

    if(acl != NULL && acl->flushTo != smallestFlushTo &&
      cmData->dmVar.lmpVersion >= CSR_BT_BLUETOOTH_VERSION_2P1)
    {
        acl->flushTo = smallestFlushTo;
        flushTo = acl->flushTo == L2CA_FLUSH_TO_DEFAULT ? 0 : MILLI_TO_BB_SLOTS(acl->flushTo);
        csrBtCmDmWriteAutoFlushTimeoutMsgSend(&acl->deviceAddr, flushTo);
    }
}

void CsrBtCmDmHciWriteAutoFlushTimeoutCompleteHandler(cmInstanceData_t * cmData)
{
    CsrBtCmDmLocalQueueHandler();
}

void CsrBtCmDmWriteAutoFlushTimeoutReqHandler(cmInstanceData_t * cmData)
{
    CsrBtCmDmWriteAutoFlushTimeoutReq * cmPrim;

    cmPrim = (CsrBtCmDmWriteAutoFlushTimeoutReq *) cmData->recvMsgP;

    cmData->dmVar.appHandle = CSR_BT_CM_IFACEQUEUE;
    dm_hci_write_auto_flush_timeout(&cmPrim->deviceAddr, cmPrim->flushTo, NULL);
}

#endif

