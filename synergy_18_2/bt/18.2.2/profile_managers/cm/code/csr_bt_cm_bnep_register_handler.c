/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE

#include "csr_bt_cm_bnep.h"
#include "csr_bt_cm_util.h"

/*************************************************************************************
  CsrBtCmBnepRegisterReqHandler
************************************************************************************/
void CsrBtCmBnepRegisterReqHandler(cmInstanceData_t *cmData)
{ /* This event allows the above profile to register a protocol handle with
    BNEP. */
    CsrBtCmBnepRegisterReq  * cmPrim;

    cmPrim                        = (CsrBtCmBnepRegisterReq *) cmData->recvMsgP;
    cmData->bnepVar.appHandle    = cmPrim->phandle;
    CsrBtBnepExtendedRegisterReqSend(cmPrim->disableExtended, cmPrim->disableStack, cmPrim->manualBridge,
                                CSR_BT_CM_IFACEQUEUE, cmPrim->deviceAddr);
    CsrBtCmServiceManagerLocalQueueHandler(cmData);
}
#endif


