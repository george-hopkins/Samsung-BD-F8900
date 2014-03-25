/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2011

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_callback_q.h"
#include "csr_bt_cm_util.h"

#ifdef CSR_BT_INSTALL_CM_READ_TX_POWER_LEVEL
void CsrBtCmDmHciReadTxPowerLevelCompleteHandler(cmInstanceData_t *cmData)
{
    DM_HCI_READ_TX_POWER_LEVEL_CFM_T    * dmPrim;
    CsrBtCmReadTxPowerLevelCfm            * cmPrim;

    dmPrim    = (DM_HCI_READ_TX_POWER_LEVEL_CFM_T *) cmData->recvMsgP;
    cmPrim    = (CsrBtCmReadTxPowerLevelCfm *)CsrPmemAlloc(sizeof(CsrBtCmReadTxPowerLevelCfm));

    cmPrim->type        = CSR_BT_CM_READ_TX_POWER_LEVEL_CFM;

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
    cmPrim->deviceAddr    = dmPrim->addrt.addr;
    cmPrim->powerLevel    = dmPrim->pwr_level;
    cmPrim->addressType   = dmPrim->addrt.type;
    CsrBtCmPutMessage(cmData->dmVar.appHandle, cmPrim);
    CsrBtCmDmLocalQueueHandler();
}

void CsrBtCmReadTxPowerLevelReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmReadTxPowerLevelReq    * cmPrim;
    TYPED_BD_ADDR_T ad;
    cmPrim = (CsrBtCmReadTxPowerLevelReq *) cmData->recvMsgP;
    
    cmData->dmVar.appHandle        = cmPrim->appHandle;
    ad.addr = cmPrim->deviceAddr;
    ad.type = cmPrim->addressType;
    dm_hci_read_tx_power_level(&ad, cmPrim->levelType, NULL);
}
#endif

#ifdef CSR_BT_LE_ENABLE
/* Upstream sender */
static void csrBtCmReadAdvertisingChTxPowerCfmSend(CsrSchedQid qid,
                                                   CsrInt8 txPower,
                                                   CsrUint16 context,
                                                   CsrBtResultCode resultCode,
                                                   CsrBtSupplier resultSupplier)
{
    CsrBtCmReadAdvertisingChTxPowerCfm *prim;
    prim = CsrPmemAlloc(sizeof(CsrBtCmReadAdvertisingChTxPowerCfm));
    prim->type = CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_CFM;
    prim->txPower = txPower;
    prim->context = context;
    prim->resultCode = resultCode;
    prim->resultSupplier = resultSupplier;
    CsrBtCmPutMessage(qid, prim);    
}
#endif

#ifdef CSR_BT_LE_ENABLE
/* Callback completion */
static void csrBtCmReadAdvertisingChTxPowerComplete(cmInstanceData_t *cmInst,
                                                    struct cmCallbackObjTag *object,
                                                    void *context,
                                                    void *event)
{
    DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM_T *prim;
    CsrBtCmReadAdvertisingChTxPowerReq *ctx;

    prim = (DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM_T*)event;
    ctx = (CsrBtCmReadAdvertisingChTxPowerReq*)context;

    csrBtCmReadAdvertisingChTxPowerCfmSend(ctx->appHandle,
                                           prim->tx_power,
                                           ctx->context,
                                           (CsrBtResultCode)(prim->status == HCI_SUCCESS
                                                             ? CSR_BT_RESULT_CODE_CM_SUCCESS
                                                             : prim->status),
                                           (CsrBtSupplier)(prim->status == HCI_SUCCESS
                                                           ? CSR_BT_SUPPLIER_CM
                                                           : CSR_BT_SUPPLIER_HCI));
}
#endif

#ifdef CSR_BT_LE_ENABLE
/* Downstream handler */
void CsrBtCmReadAdvertisingChTxPowerReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmReadAdvertisingChTxPowerReq *req;
    req = cmData->recvMsgP;

    if ((cmData->globalState != notReady_s)
        && (cmData->dmVar.lmpVersion >= CSR_BT_BLUETOOTH_VERSION_4P0))
    {
        DM_UPRIM_T *prim;
        dm_hci_ulp_read_advertising_channel_tx_power_req(&prim);
        CsrBtCmCallbackSendSimpleBlockDm(cmData,
                                         DM_HCI_ULP_READ_ADVERTISING_CHANNEL_TX_POWER_CFM,
                                         req,
                                         prim,
                                         csrBtCmReadAdvertisingChTxPowerComplete);
        cmData->recvMsgP = NULL;
    }
    else
    {
        csrBtCmReadAdvertisingChTxPowerCfmSend(req->appHandle,
                                               0, /* level */
                                               req->context,
                                               CSR_BT_RESULT_CODE_CM_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE,
                                               CSR_BT_SUPPLIER_CM);
    }
}
#endif
