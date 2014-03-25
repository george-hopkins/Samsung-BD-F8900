/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_L2CA_MODULE

#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_util.h"
#include "csr_exceptionhandler.h"

static void CsrBtCmL2caRegisterCfmSend(CsrSchedQid appHandle, psm_t psm, CsrUint16 modeMask,
                                       CsrBtResultCode resultCode, CsrBtSupplier resultSupplier,
                                       CsrUint16 context)
{
    CsrBtCmL2caRegisterCfm * prim   = (CsrBtCmL2caRegisterCfm *)CsrPmemAlloc(sizeof(CsrBtCmL2caRegisterCfm));
    prim->type                      = CSR_BT_CM_L2CA_REGISTER_CFM;
    prim->localPsm                  = psm;
    prim->mode_mask                 = modeMask;
    prim->resultCode                = resultCode;
    prim->resultSupplier            = resultSupplier;
    prim->context                   = context;
    CsrBtCmPutMessage(appHandle, prim);
}

void CsrBtCmL2caRegisterReqHandler(cmInstanceData_t *cmData)
{ /* Request to register a PSM channel in the L2CAP layer, so it can multiplex
     between protocols. */
    CsrBtCmL2caRegisterReq  * prim  = (CsrBtCmL2caRegisterReq *) cmData->recvMsgP;

    cmData->smVar.appHandle = prim->phandle;
    cmData->smVar.context   = prim->context;

    /* Register a PSM to l2cap */
    L2CA_RegisterReq(prim->localPsm,
                     CSR_BT_CM_IFACEQUEUE,
                     prim->mode_mask,
                     0, /*flags*/
                     0); /* reg_ctx */
}

#ifdef CSR_BT_INSTALL_CM_PRI_L2CA_UNREGISTER
void CsrBtCmL2caUnRegisterReqHandler(cmInstanceData_t *cmData)
{ /* Request to unregister a dynamic PSM channel in the L2CAP layer                 */
    CsrBtCmL2caUnregisterReq  * prim = (CsrBtCmL2caUnregisterReq *) cmData->recvMsgP;

    L2CA_UnRegisterReq(prim->localPsm, CSR_BT_CM_IFACEQUEUE);
}

void CsrBtCmL2caUnRegisterCfmHandler(cmInstanceData_t *cmData)
{ /* Confirmation event to previous PSM register request.                           */
    L2CA_UNREGISTER_CFM_T * prim = (L2CA_UNREGISTER_CFM_T *) cmData->recvMsgP;

    if ((L2CA_RESULT_T)prim->result != L2CA_RESULT_SUCCESS)
    {
        CsrBtCmStateEventException(L2CAP_PRIM, L2CA_UNREGISTER_CFM, cmData->globalState);
    }
}
#endif

void CsrBtCmL2caRegisterCfmHandler(cmInstanceData_t *cmData)
{ /* Confirmation event to previous PSM register request. Send
     CSR_BT_CM_L2CA_REGISTER_CFM to the application and restore the local service manager
     queue                                                                          */
    L2CA_REGISTER_CFM_T * prim = (L2CA_REGISTER_CFM_T *) cmData->recvMsgP;

    if ((L2CA_RESULT_T)prim->result == L2CA_RESULT_SUCCESS)
    {
        CsrBtCmL2caRegisterCfmSend(cmData->smVar.appHandle,
                                   prim->psm_local, prim->mode_mask,
                                   CSR_BT_RESULT_CODE_CM_SUCCESS, CSR_BT_SUPPLIER_CM,
                                   cmData->smVar.context);
    }
    else
    {
        CsrBtCmL2caRegisterCfmSend(cmData->smVar.appHandle,
                                   prim->psm_local, prim->mode_mask,
                                   (CsrBtResultCode) prim->result, CSR_BT_SUPPLIER_L2CAP_MISC,
                                   cmData->smVar.context);
    }
    CsrBtCmServiceManagerLocalQueueHandler(cmData);
}

#endif /* #ifndef EXCLUDE_CSR_BT_L2CA_MODULE */
