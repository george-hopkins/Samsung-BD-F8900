/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_prim.h"
#include "dm_prim.h"
#include "dmlib.h"
#include "csr_bt_cm_private_lib.h"
#include "csr_bt_cm_util.h"
#include "csr_bt_cm_callback_q.h"
#include "csr_bt_cm_events_handler.h"

/* Upstream sender */
static void csrBtCmReadEncryptionKeySizeSend(CsrSchedQid appHandle,
                                             CsrBtTypedAddr address,
                                             CsrUint16 keySize,
                                             CsrUint16 context,
                                             CsrBtResultCode resultCode,
                                             CsrBtSupplier resultSupplier)
{
    CsrBtCmReadEncryptionKeySizeCfm *prim;
    prim = CsrPmemAlloc(sizeof(CsrBtCmReadEncryptionKeySizeCfm));
    prim->type = CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_CFM;
    prim->address = address;
    prim->keySize = keySize;
    prim->context = context;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtCmPutMessage(appHandle, prim);
}

/* Callback completion */
#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_3P0
static void csrBtCmReadEncryptionKeySizeComplete(cmInstanceData_t *cmInst,
                                                 struct cmCallbackObjTag *object,
                                                 void *context,
                                                 void *event)
{
    DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM_T *prim;
    CsrBtCmReadEncryptionKeySizeReq *ctx;

    prim = (DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM_T*)event;
    ctx = (CsrBtCmReadEncryptionKeySizeReq*)context;

    csrBtCmReadEncryptionKeySizeSend(ctx->appHandle,
                                     ctx->address,
                                     prim->key_size,
                                     ctx->context,
                                     (CsrBtResultCode)(prim->status == HCI_SUCCESS
                                                       ? CSR_BT_RESULT_CODE_CM_SUCCESS
                                                       : prim->status),
                                     (CsrBtSupplier)(prim->status == HCI_SUCCESS
                                                     ? CSR_BT_SUPPLIER_CM
                                                     : CSR_BT_SUPPLIER_HCI));
}
#endif

/* Downstream handler */
void CsrBtCmReadEncryptionKeySizeReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmReadEncryptionKeySizeReq *req;
    req = (CsrBtCmReadEncryptionKeySizeReq*)cmData->recvMsgP;

#if CSR_BT_BT_VERSION >= CSR_BT_BLUETOOTH_VERSION_3P0
    if ((cmData->globalState != notReady_s)
        && (cmData->dmVar.lmpVersion >= CSR_BT_BLUETOOTH_VERSION_3P0))
    {
        DM_UPRIM_T *prim;
        dm_hci_read_encryption_key_size(&CsrBtAddrAddr(req->address),
                                        &prim);
        CsrBtCmCallbackSendSimpleBlockDm(cmData,
                                         DM_HCI_READ_ENCRYPTION_KEY_SIZE_CFM,
                                         req,
                                         prim,
                                         csrBtCmReadEncryptionKeySizeComplete);        
        cmData->recvMsgP = NULL;
    }
    else
#endif
    {
        csrBtCmReadEncryptionKeySizeSend(req->appHandle,
                                         req->address,
                                         0,
                                         req->context,
                                         CSR_BT_RESULT_CODE_CM_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE,
                                         CSR_BT_SUPPLIER_CM);
    }
}

/* Downstream handler */
#ifdef CSR_BT_LE_ENABLE
void CsrBtCmSmSetEncryptionKeySizeReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmSmSetEncryptionKeySizeReq *prim;
    prim = cmData->recvMsgP;

    dm_sm_init_req_le_enc(DM_SM_INIT_ENC_SIZE_CONF,
                          0, /* mode (not specified) */
                          0, /* security_level_default (not specified) */
                          0, /* config (not specified) */
                          0, /* write_auth_enable (not specified) */
                          0, /* mode3_enc (not specified) */
                          NULL, /* sm_key_state (not specified) */
                          0, /* sm_div_state (not specified) */
                          0, /* sm_sign_counter (not specified) */
                          prim->maxKeySize,
                          prim->minKeySize,
                          NULL);
}
#endif
