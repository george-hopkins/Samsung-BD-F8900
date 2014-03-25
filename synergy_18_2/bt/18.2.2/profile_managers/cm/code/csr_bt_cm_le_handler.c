/****************************************************************************

Copyright Cambridge Silicon Radio Limited and its licensors 2011. All rights reserved.

REVISION:      $Revision: #5 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_cm_le.h"

static void csrBtCmLeScanProcess(cmInstanceData_t *cmInst,
                                 CsrBtCmLeScanReq *ctx,
                                 CsrUint8 hciStatus);

static void csrBtCmLeAdvertiseProcess(cmInstanceData_t *cmInst,
                                      CsrBtCmLeAdvertiseReq *ctx,
                                      CsrUint8 hciStatus);

/* Connection parameter update COEX handler */
static void csrBtCmLeParamUpdateCoex(cmInstanceData_t *cmInst,
                                     CsrBtTypedAddr *addr,
                                     CsrUint16 interval,
                                     CsrUint16 latency,
                                     CsrUint16 timeout)
{
    leConnVar *conn;

    /* Find cache entry */
    for(conn = cmInst->leVar.connCache;
        conn != NULL;
        conn = conn->next)
    {
        if (CsrBtAddrEq(&(conn->dmPrim.addrt), addr))
        {
            /* Update connection params if they changed */
            if((conn->dmPrim.ble_conn_params.conn_interval != interval)
               || (conn->dmPrim.ble_conn_params.conn_latency != latency)
               || (conn->dmPrim.ble_conn_params.supervision_timeout != timeout))
            {
                conn->dmPrim.ble_conn_params.conn_interval = interval;
                conn->dmPrim.ble_conn_params.conn_latency = latency;
                conn->dmPrim.ble_conn_params.supervision_timeout = timeout;
                /* accuracy is not modified */
                
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY
                /* Notify COEX update. Note that the LE cache contains the
                 * updated DM_ACL_CONN_HANDLE_IND with the params */
                CsrBtCmPropgateEvent(cmInst,
                                     CsrBtCmPropagateLeConnectionEvent,
                                     CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY,
                                     HCI_SUCCESS,
                                     &(conn->dmPrim),
                                     NULL);
#endif
            }
            break;
        }
    }
}

/* Send scan confirm to app */
static void csrBtCmLeScanCfmSend(CsrSchedQid qid,
                                 CsrBtResultCode resultCode,
                                 CsrBtSupplier resultSupplier)
{
    CsrBtCmLeScanCfm *cfm;
    cfm = (CsrBtCmLeScanCfm*)CsrPmemAlloc(sizeof(CsrBtCmLeScanCfm));
    cfm->type = CSR_BT_CM_LE_SCAN_CFM;
    cfm->resultCode = resultCode;
    cfm->resultSupplier = resultSupplier;
    CsrBtCmPutMessage(qid, cfm);
}

/* Send connection parameter update complete indication */
static void csrBtCmLeConnparamUpdateCmpIndSend(CsrSchedQid qid,
                                               CsrBtTypedAddr *address,
                                               CsrBtResultCode resultCode,
                                               CsrBtSupplier resultSupplier)
{
    CsrBtCmLeConnparamUpdateCmpInd *ind;
    ind = CsrPmemAlloc(sizeof(CsrBtCmLeConnparamUpdateCmpInd));
    ind->type = CSR_BT_CM_LE_CONNPARAM_UPDATE_CMP_IND;
    ind->address = *address;
    ind->resultCode = resultCode;
    ind->resultSupplier = resultSupplier;
    CsrBtCmPutMessage(qid, ind);
}                                               

/* Send whitelist confirm to app */
static void csrBtCmLeWhitelistSetCfmSend(CsrSchedQid qid,
                                         CsrUint8 operation,
                                         CsrBtResultCode resultCode,
                                         CsrBtSupplier resultSupplier)
{
    CsrBtCmLeWhitelistSetCfm *cfm;
    cfm = (CsrBtCmLeWhitelistSetCfm*)CsrPmemAlloc(sizeof(CsrBtCmLeWhitelistSetCfm));
    cfm->type = CSR_BT_CM_LE_WHITELIST_SET_CFM;
    cfm->operation = operation;
    cfm->resultCode = resultCode;
    cfm->resultSupplier = resultSupplier;
    CsrBtCmPutMessage(qid, cfm);
}

/* Send advertise confirm to app */
static void csrBtCmLeAdvertiseCfmSend(cmInstanceData_t *cmInst,
                                      CsrSchedQid qid,
                                      CsrBtResultCode resultCode,
                                      CsrBtSupplier resultSupplier)
{
    CsrBtCmLeAdvertiseCfm *cfm;
    cfm = (CsrBtCmLeAdvertiseCfm*)CsrPmemAlloc(sizeof(CsrBtCmLeAdvertiseCfm));
    cfm->type = CSR_BT_CM_LE_ADVERTISE_CFM;
    cfm->resultCode = resultCode;
    cfm->resultSupplier = resultSupplier;
    CsrBtCmPutMessage(qid, cfm);
}

/* Callback completion handler for connection update data request */
static void csrBtCmLeConnectionUpdateComplete(cmInstanceData_t *cmInst,
                                              struct cmCallbackObjTag *object,
                                              void *context,
                                              void *event)
{
    DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_T *prim;
    CsrBtCmLeConnparamUpdateReq *ctx;

    prim = (DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM_T*)event;
    ctx = (CsrBtCmLeConnparamUpdateReq*)context;

    csrBtCmLeConnparamUpdateCmpIndSend(ctx->appHandle,
                                       &prim->addrt,
                                       (CsrBtResultCode)(prim->status == HCI_SUCCESS
                                                         ? CSR_BT_RESULT_CODE_CM_SUCCESS
                                                         : prim->status),
                                       (CsrBtSupplier)(prim->status == HCI_SUCCESS
                                                       ? CSR_BT_SUPPLIER_CM
                                                       : CSR_BT_SUPPLIER_HCI));
}

/* Callback completion handler for connection update data request */
static void csrBtCmLeConnectionSetComplete(cmInstanceData_t *cmInst,
                                           struct cmCallbackObjTag *object,
                                           void *context,
                                           void *event)
{
    DM_SET_BLE_CONNECTION_PARAMETERS_CFM_T *prim;
    CsrBtCmLeConnparamReq *ctx;
    CsrBtCmLeConnparamCfm *cfm;

    prim = (DM_SET_BLE_CONNECTION_PARAMETERS_CFM_T*)event;
    ctx = (CsrBtCmLeConnparamReq*)context;
    cfm = (CsrBtCmLeConnparamCfm*)CsrPmemAlloc(sizeof(CsrBtCmLeConnparamCfm));   
    cfm->type = CSR_BT_CM_LE_CONNPARAM_CFM;
    
    if(prim->status == HCI_SUCCESS)
    {
        cfm->resultSupplier = CSR_BT_SUPPLIER_CM;
        cfm->resultCode = CSR_BT_RESULT_CODE_CM_SUCCESS;
    }
    else
    {
        cfm->resultSupplier = CSR_BT_SUPPLIER_HCI;
        cfm->resultCode = prim->status;
    }
    
    CsrBtCmPutMessage(ctx->appHandle, cfm);    
}

/* Callback completion handler for whitelist */
static void csrBtCmLeWhitelistSetComplete(cmInstanceData_t *cmInst,
                                           struct cmCallbackObjTag *object,
                                           void *context,
                                           void *event)
{
    CsrBtCmLeWhitelistSetReq *ctx;
    CsrBtResultCode resultCode;
    CsrBtSupplier resultSupplier;
    ctx = (CsrBtCmLeWhitelistSetReq*)context;

    if(ctx->operation == CSR_BT_CM_LE_WHITELIST_ADD)
    {
        DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM_T *prim;
        prim = (DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM_T*)event;
        resultCode = (CsrBtResultCode)((prim->status == HCI_SUCCESS)
                                       ? CSR_BT_RESULT_CODE_CM_SUCCESS
                                       : prim->status);
    }
    else
    {
        DM_HCI_ULP_CLEAR_WHITE_LIST_CFM_T *prim;
        prim = (DM_HCI_ULP_CLEAR_WHITE_LIST_CFM_T*)event;
        resultCode = (CsrBtResultCode)((prim->status == HCI_SUCCESS)
                                       ? CSR_BT_RESULT_CODE_CM_SUCCESS
                                       : prim->status);
    }

    resultSupplier = ((resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS)
                      ? CSR_BT_SUPPLIER_CM
                      : CSR_BT_SUPPLIER_HCI);
    csrBtCmLeWhitelistSetCfmSend(ctx->appHandle,
                                 ctx->operation,
                                 resultCode,
                                 resultSupplier);

    /* Queue system will free event and ctx */
}

/* Callback completion handler for scan request */
static void csrBtCmLeSetScanEnableComplete(cmInstanceData_t *cmInst,
                                           struct cmCallbackObjTag *object,
                                           void *context,
                                           void *event)
{
    DM_HCI_ULP_SET_SCAN_ENABLE_CFM_T *prim;
    CsrBtCmLeScanReq *ctx;
    CsrUint8 mode;
    prim = (DM_HCI_ULP_SET_SCAN_ENABLE_CFM_T*)event;
    ctx = (CsrBtCmLeScanReq*)context;

    mode = (CsrUint8)((ctx->paramChange & CSR_BT_CM_LE_PARCHG_INTERNAL)
                      ? CSR_BT_CM_LE_MODE_OFF
                      : ctx->mode);

    if (mode != cmInst->leVar.scanMode)
    {
        /* If we are actively trying to connect, do not change COEX
         * mode */
        if(!((cmInst->leVar.scanMode == CSR_BT_CM_LE_MODE_ON)
             && (cmInst->leVar.scanType == CSR_BT_CM_LE_SCANTYPE_INITIATING)))
        {
            cmInst->leVar.scanMode = mode;
        }

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY
        CsrBtCmPropgateEvent(cmInst,
                             CsrBtCmPropagateLeScanEvent,
                             CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY,
                             prim->status,
                             &mode,
                             NULL);
#endif
    }

    /* Internal toggle to allow param change. */
    if(!(ctx->paramChange & CSR_BT_CM_LE_PARCHG_INTERNAL))
    {
        ctx->mode = CSR_BT_CM_LE_MODE_IGNORE;
    }
    csrBtCmLeScanProcess(cmInst, ctx, prim->status);
    object->context = NULL;
}

/* Callback completion handler for scan request */
static void csrBtCmLeSetScanParametersComplete(cmInstanceData_t *cmInst,
                                               struct cmCallbackObjTag *object,
                                               void *context,
                                               void *event)
{
    DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM_T *prim;
    CsrBtCmLeScanReq *ctx;
    prim = (DM_HCI_ULP_SET_SCAN_ENABLE_CFM_T*)event;
    ctx = (CsrBtCmLeScanReq*)context;

    /* Sub-step complete */
    ctx->paramChange &= ~(CSR_BT_CM_LE_PARCHG_PAR);
    csrBtCmLeScanProcess(cmInst, ctx, prim->status);
    object->context = NULL;
}

/* Drive scan parameter changes */
static void csrBtCmLeScanProcess(cmInstanceData_t *cmInst,
                                 CsrBtCmLeScanReq *ctx,
                                 CsrUint8 hciStatus)
{
    DM_UPRIM_T *prim;

    /* Stop on error, but allow initial 'turn off' to proceed even if
     * it fails */
    if((hciStatus != HCI_SUCCESS)
       && !(ctx->paramChange & CSR_BT_CM_LE_PARCHG_INTERNAL))
    {
        csrBtCmLeScanCfmSend(ctx->appHandle,
                             hciStatus,
                             CSR_BT_SUPPLIER_HCI);
        CsrPmemFree(ctx);
        return;
    }
    ctx->paramChange &= ~(CSR_BT_CM_LE_PARCHG_INTERNAL);

    if(ctx->paramChange & CSR_BT_CM_LE_PARCHG_PAR)
    {
        /* Need to store scan params for COEX */
        cmInst->leVar.scanInterval = ctx->scanInterval;
        cmInst->leVar.scanWindow = ctx->scanWindow;

        /* If we are actively trying to connect, do not change COEX
         * scan type */
        if(!((cmInst->leVar.scanMode == CSR_BT_CM_LE_MODE_ON)
             && (cmInst->leVar.scanType == CSR_BT_CM_LE_SCANTYPE_INITIATING)))
        {
            cmInst->leVar.scanType = ctx->scanType;
        }

        dm_hci_ulp_set_scan_parameters_req(ctx->scanType,
                                           ctx->scanInterval,
                                           ctx->scanWindow,
                                           HCI_ULP_ADDRESS_PUBLIC,
                                           ctx->scanningFilterPolicy,
                                           &prim);
        CsrBtCmCallbackSendSimpleBlockDm(cmInst,
                                         DM_HCI_ULP_SET_SCAN_PARAMETERS_CFM,
                                         ctx,
                                         prim,
                                         csrBtCmLeSetScanParametersComplete);
    }
    else if(ctx->mode == CSR_BT_CM_LE_MODE_ON)
    {
        dm_hci_ulp_set_scan_enable_req(HCI_ULP_SCAN_ENABLED,
                                       ctx->filterDuplicates,
                                       &prim);
        CsrBtCmCallbackSendSimpleBlockDm(cmInst,
                                         DM_HCI_ULP_SET_SCAN_ENABLE_CFM,
                                         ctx,
                                         prim,
                                         csrBtCmLeSetScanEnableComplete);
    }
    else
    {
        /* If we are actively trying to connect, send COEX from here
         * as we won't enable standard advertising mode */
        /* If we are actively trying to connect, do not change COEX
         * mode */
        if((cmInst->leVar.scanMode == CSR_BT_CM_LE_MODE_ON)
           && (cmInst->leVar.scanType == CSR_BT_CM_LE_SCANTYPE_INITIATING))
        {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY
            CsrBtCmPropgateEvent(cmInst,
                                 CsrBtCmPropagateLeScanEvent,
                                 CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY,
                                 HCI_SUCCESS,
                                 &(cmInst->leVar.scanMode),
                                 NULL);
#endif
        }

        csrBtCmLeScanCfmSend(ctx->appHandle,
                             CSR_BT_RESULT_CODE_CM_SUCCESS,
                             CSR_BT_SUPPLIER_CM);
        CsrPmemFree(ctx);
    }
}

/* Callback completion handler for set advertise enable request */
static void csrBtCmLeSetAdvertiseEnableComplete(cmInstanceData_t *cmInst,
                                                struct cmCallbackObjTag *object,
                                                void *context,
                                                void *event)
{
    DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM_T *prim;
    CsrBtCmLeAdvertiseReq *ctx;
    CsrUint8 mode;
    prim = (DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM_T*)event;
    ctx = (CsrBtCmLeAdvertiseReq*)context;

    mode = (CsrUint8)((ctx->paramChange & CSR_BT_CM_LE_PARCHG_INTERNAL)
                      ? CSR_BT_CM_LE_MODE_OFF
                      : ctx->mode);

    if (mode != cmInst->leVar.advMode)
    {
        cmInst->leVar.advMode = mode;

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY
        CsrBtCmPropgateEvent(cmInst,
                             CsrBtCmPropagateLeAdvertisingEvent,
                             CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY,
                             prim->status,
                             &(ctx->mode),
                             NULL);
#endif
    }

    /* Internal toggle to allow param change. */
    if(!(ctx->paramChange & CSR_BT_CM_LE_PARCHG_INTERNAL))
    {
        ctx->mode = CSR_BT_CM_LE_MODE_IGNORE;
    }
    csrBtCmLeAdvertiseProcess(cmInst, ctx, prim->status);
    object->context = NULL;
}

/* Callback completion handler for set advertising data request */
static void csrBtCmLeSetAdvertisingParametersComplete(cmInstanceData_t *cmInst,
                                                      struct cmCallbackObjTag *object,
                                                      void *context,
                                                      void *event)
{
    DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM_T *prim;
    CsrBtCmLeAdvertiseReq *ctx;
    prim = (DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM_T*)event;
    ctx = (CsrBtCmLeAdvertiseReq*)context;

    /* Sub-step complete */
    ctx->paramChange &= ~(CSR_BT_CM_LE_PARCHG_PAR);
    csrBtCmLeAdvertiseProcess(cmInst, ctx, prim->status);
    object->context = NULL;
}

/* Callback completion handler for set advertising data request */
static void csrBtCmLeSetAdvertiseAdDataComplete(cmInstanceData_t *cmInst,
                                                struct cmCallbackObjTag *object,
                                                void *context,
                                                void *event)
{
    DM_HCI_ULP_SET_ADVERTISING_DATA_CFM_T *prim;
    CsrBtCmLeAdvertiseReq *ctx;
    prim = (DM_HCI_ULP_SET_ADVERTISING_DATA_CFM_T*)event;
    ctx = (CsrBtCmLeAdvertiseReq*)context;

    ctx->paramChange &= ~(CSR_BT_CM_LE_PARCHG_DATA_AD);
    csrBtCmLeAdvertiseProcess(cmInst, ctx, prim->status);
    object->context = NULL;
}

/* Callback completion handler for set scan response data request */
static void csrBtCmLeSetAdvertiseSrDataComplete(cmInstanceData_t *cmInst,
                                                struct cmCallbackObjTag *object,
                                                void *context,
                                                void *event)
{
    DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM_T *prim;
    CsrBtCmLeAdvertiseReq *ctx;
    prim = (DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM_T*)event;
    ctx = (CsrBtCmLeAdvertiseReq*)context;

    ctx->paramChange &= ~(CSR_BT_CM_LE_PARCHG_DATA_SR);
    csrBtCmLeAdvertiseProcess(cmInst, ctx, prim->status);
    object->context = NULL;
}

/* Drive LE advertise request */
static void csrBtCmLeAdvertiseProcess(cmInstanceData_t *cmInst,
                                      CsrBtCmLeAdvertiseReq *ctx,
                                      CsrUint8 hciStatus)
{
    DM_UPRIM_T *prim;

    /* Stop on error, but allow initial 'turn off' to proceed even if
     * it fails */
    if((hciStatus != HCI_SUCCESS)
       && !(ctx->paramChange & CSR_BT_CM_LE_PARCHG_INTERNAL))
    {
        csrBtCmLeAdvertiseCfmSend(cmInst,
                                  ctx->appHandle,
                                  hciStatus,
                                  CSR_BT_SUPPLIER_HCI);
        CsrPmemFree(ctx);
        return;
    }
    ctx->paramChange &= ~(CSR_BT_CM_LE_PARCHG_INTERNAL);

    if(ctx->paramChange & CSR_BT_CM_LE_PARCHG_DATA_AD)
    {
        dm_hci_ulp_set_advertising_data_req(ctx->advertisingDataLength,
                                            ctx->advertisingData,
                                            &prim);
        CsrBtCmCallbackSendSimpleBlockDm(cmInst,
                                         DM_HCI_ULP_SET_ADVERTISING_DATA_CFM,
                                         ctx,
                                         prim,
                                         csrBtCmLeSetAdvertiseAdDataComplete);
        CsrPmemFree(ctx->advertisingData);
        ctx->advertisingData = NULL;
    }
    else if(ctx->paramChange & CSR_BT_CM_LE_PARCHG_DATA_SR)
    {
        dm_hci_ulp_set_scan_response_data_req(ctx->scanResponseDataLength,
                                              ctx->scanResponseData,
                                              &prim);
        CsrBtCmCallbackSendSimpleBlockDm(cmInst,
                                         DM_HCI_ULP_SET_SCAN_RESPONSE_DATA_CFM,
                                         ctx,
                                         prim,
                                         csrBtCmLeSetAdvertiseSrDataComplete);
        CsrPmemFree(ctx->scanResponseData);
        ctx->scanResponseData = NULL;
    }
    else if(ctx->paramChange & CSR_BT_CM_LE_PARCHG_PAR)
    {
        /* Need to store advertise params for COEX */
        cmInst->leVar.advType = ctx->advertisingType;
        cmInst->leVar.advIntervalMin = ctx->advIntervalMin;
        cmInst->leVar.advIntervalMax = ctx->advIntervalMax;
        cmInst->leVar.advChannelMap = ctx->advertisingChannelMap;

        dm_hci_ulp_set_advertising_parameters_req(ctx->advIntervalMin,
                                                  ctx->advIntervalMax,
                                                  ctx->advertisingType,
                                                  HCI_ULP_ADDRESS_PUBLIC,
                                                  &ctx->directAddress,
                                                  ctx->advertisingChannelMap,
                                                  ctx->advertisingFilterPolicy,
                                                  &prim);
        CsrBtCmCallbackSendSimpleBlockDm(cmInst,
                                         DM_HCI_ULP_SET_ADVERTISING_PARAMETERS_CFM,
                                         ctx,
                                         prim,
                                         csrBtCmLeSetAdvertisingParametersComplete);
    }
    else if(ctx->mode == CSR_BT_CM_LE_MODE_ON)
    {
        dm_hci_ulp_set_advertise_enable_req(HCI_ULP_ADVERTISING_ENABLED,
                                            &prim);
        CsrBtCmCallbackSendSimpleBlockDm(cmInst,
                                         DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM,
                                         ctx,
                                         prim,
                                         csrBtCmLeSetAdvertiseEnableComplete);
    }
    else
    {
        /* If we are actively trying to connect, send COEX from here
         * as we won't enable standard advertising mode */
        if((cmInst->leVar.advMode == CSR_BT_CM_LE_MODE_ON)
           && ((cmInst->leVar.advType == HCI_ULP_ADVERT_CONNECTABLE_UNDIRECTED)
               || (cmInst->leVar.advType == HCI_ULP_ADVERT_CONNECTABLE_DIRECTED)))
        {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY
            CsrBtCmPropgateEvent(cmInst,
                                 CsrBtCmPropagateLeAdvertisingEvent,
                                 CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY,
                                 HCI_SUCCESS,
                                 &(cmInst->leVar.advMode),
                                 NULL);
#endif
        }

        csrBtCmLeAdvertiseCfmSend(cmInst,
                                  ctx->appHandle,
                                  CSR_BT_RESULT_CODE_CM_SUCCESS,
                                  CSR_BT_SUPPLIER_CM);
        CsrPmemFree(ctx);
    }
}

/* Handle scan request from app */
void CsrBtCmLeScanReqHandler(cmInstanceData_t *cmInst)
{
    CsrBtCmLeScanReq *req;
    req = (CsrBtCmLeScanReq*)cmInst->recvMsgP;

    if (cmInst->dmVar.lmpVersion < CSR_BT_BLUETOOTH_VERSION_4P0)
    {
        csrBtCmLeScanCfmSend(req->appHandle,
                             CSR_BT_RESULT_CODE_CM_UNSUPPORTED_FEATURE,
                             CSR_BT_SUPPLIER_CM);
        CsrBtCmFreeDownstreamMessageContents(CSR_BT_CM_PRIM,
                                             req);
        return;
    }

    /* COEX notification from GATT */
    if(req->mode & CSR_BT_CM_LE_MODE_COEX_NOTIFY)
    {
        CsrUint8 mode;
        mode = req->mode & ~(CSR_BT_CM_LE_MODE_COEX_NOTIFY);

        /* Connection attempt as peripheral/slave/advertiser */
        if ((cmInst->leVar.scanMode != CSR_BT_CM_LE_MODE_ON)
            && (cmInst->leVar.scanType != CSR_BT_CM_LE_SCANTYPE_INITIATING))
        {
            cmInst->leVar.scanMode = mode;
            cmInst->leVar.scanType = CSR_BT_CM_LE_SCANTYPE_INITIATING;
            
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY
            CsrBtCmPropgateEvent(cmInst,
                                 CsrBtCmPropagateLeScanEvent,
                                 CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY,
                                 HCI_SUCCESS,
                                 &mode,
                                 NULL);
#endif
        }
    }
    else
    {
        /* Start scan command processor by turning it off */
        DM_UPRIM_T *prim;
        dm_hci_ulp_set_scan_enable_req(HCI_ULP_SCAN_DISABLED,
                                       HCI_ULP_FILTER_DUPLICATES_DISABLED,
                                       &prim);
        req->paramChange |= CSR_BT_CM_LE_PARCHG_INTERNAL;
        CsrBtCmCallbackSendSimpleBlockDm(cmInst,
                                         DM_HCI_ULP_SET_SCAN_ENABLE_CFM,
                                         req,
                                         prim,
                                         csrBtCmLeSetScanEnableComplete);
        cmInst->recvMsgP = NULL;
    }
}

/* Handle advertise request from app */
void CsrBtCmLeAdvertiseReqHandler(cmInstanceData_t *cmInst)
{
    CsrBtCmLeAdvertiseReq *req;
    req = (CsrBtCmLeAdvertiseReq*)cmInst->recvMsgP;

    if (cmInst->dmVar.lmpVersion < CSR_BT_BLUETOOTH_VERSION_4P0)
    {
        csrBtCmLeAdvertiseCfmSend(cmInst,
                                  req->appHandle,
                                  CSR_BT_RESULT_CODE_CM_UNSUPPORTED_FEATURE,
                                  CSR_BT_SUPPLIER_CM);
        CsrBtCmFreeDownstreamMessageContents(CSR_BT_CM_PRIM,
                                             req);
        return;
    }

    /* COEX notification from GATT */
    if(req->mode & CSR_BT_CM_LE_MODE_COEX_NOTIFY)
    {
        CsrUint8 mode;
        mode = req->mode & ~(CSR_BT_CM_LE_MODE_COEX_NOTIFY);

        /* Connection attempt as peripheral/slave/advertiser */
        if ((cmInst->leVar.advMode != mode)
            || (cmInst->leVar.advType != req->advertisingType))
        {
            /* If we are actively trying to connect, do not change
             * COEX mode */
            if(!( (cmInst->leVar.advMode == CSR_BT_CM_LE_MODE_ON)
                  && ((cmInst->leVar.advType == HCI_ULP_ADVERT_CONNECTABLE_UNDIRECTED)
                      || (cmInst->leVar.advType == HCI_ULP_ADVERT_CONNECTABLE_DIRECTED))) )
            {
                cmInst->leVar.advMode = mode;
                cmInst->leVar.advType = req->advertisingType;
            }

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY
            CsrBtCmPropgateEvent(cmInst,
                                 CsrBtCmPropagateLeAdvertisingEvent,
                                 CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY,
                                 HCI_SUCCESS,
                                 &mode,
                                 NULL);
#endif
        }
    }
    else
    {
        /* Avoid sending out empty scan response packets */
        DM_UPRIM_T *prim;

        if((req->scanResponseDataLength == 0)
           && (req->advertisingType == HCI_ULP_ADVERT_DISCOVERABLE)
           && (req->paramChange & CSR_BT_CM_LE_PARCHG_DATA_SR))
        {
            req->advertisingType = HCI_ULP_ADVERT_NON_CONNECTABLE;
        }


        /* If we are setting up a connection, the DM handles the
         * pause-and-resume for us */
        if ((cmInst->leVar.advMode == CSR_BT_CM_LE_MODE_ON)
            && ((cmInst->leVar.advType == HCI_ULP_ADVERT_CONNECTABLE_UNDIRECTED)
                || (cmInst->leVar.advType == HCI_ULP_ADVERT_CONNECTABLE_DIRECTED)))
        {
            req->mode = CSR_BT_CM_LE_MODE_IGNORE;
            csrBtCmLeAdvertiseProcess(cmInst, req, HCI_SUCCESS);
        }
        else
        {
            /* Start advertise command processor by turning it off */
            req->paramChange |= CSR_BT_CM_LE_PARCHG_INTERNAL;
            dm_hci_ulp_set_advertise_enable_req(HCI_ULP_ADVERTISING_DISABLED,
                                                &prim);
            CsrBtCmCallbackSendSimpleBlockDm(cmInst,
                                             DM_HCI_ULP_SET_ADVERTISE_ENABLE_CFM,
                                             req,
                                             prim,
                                             csrBtCmLeSetAdvertiseEnableComplete);
        }

        /* Always consumed */
        cmInst->recvMsgP = NULL;
    }
}

/* Control LE whitelist */
void CsrBtCmLeWhitelistSetReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmLeWhitelistSetReq *prim;
    DM_UPRIM_T *dmPrim;
    CsrUint16 expect;
    prim = (CsrBtCmLeWhitelistSetReq*)cmData->recvMsgP;
    dmPrim = NULL;
    expect = 0;

    if (cmData->dmVar.lmpVersion < CSR_BT_BLUETOOTH_VERSION_4P0)
    {
        csrBtCmLeWhitelistSetCfmSend(prim->appHandle,
                                     prim->operation,
                                     CSR_BT_RESULT_CODE_CM_UNSUPPORTED_FEATURE,
                                     CSR_BT_SUPPLIER_CM);
        CsrBtCmFreeDownstreamMessageContents(CSR_BT_CM_PRIM,
                                             prim);
        return;
    }

    switch(prim->operation)
    {
        case CSR_BT_CM_LE_WHITELIST_ADD:
        {
            dm_hci_ulp_add_device_to_white_list_req(&prim->address,
                                                    &dmPrim);
            expect = DM_HCI_ULP_ADD_DEVICE_TO_WHITE_LIST_CFM;
            break;
        }

        case CSR_BT_CM_LE_WHITELIST_CLEAR:
            dm_hci_ulp_clear_white_list_req(&dmPrim);
            expect = DM_HCI_ULP_CLEAR_WHITE_LIST_CFM;
            break;

        default:
            dmPrim = NULL;
            break;
    }

    if(dmPrim)
    {
        CsrBtCmCallbackSendSimpleBlockDm(cmData,
                                         expect,
                                         cmData->recvMsgP,
                                         dmPrim,
                                         csrBtCmLeWhitelistSetComplete);
        cmData->recvMsgP = NULL;
    }
    else
    {
        csrBtCmLeScanCfmSend(prim->appHandle,
                             CSR_BT_RESULT_CODE_CM_UNACCEPTABLE_PARAMETER,
                             CSR_BT_SUPPLIER_CM);
    }
}

void CsrBtCmLeConnparamReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmLeConnparamReq *req;
    DM_UPRIM_T *prim;
    req = (CsrBtCmLeConnparamReq*)cmData->recvMsgP;
    
    if (cmData->dmVar.lmpVersion < CSR_BT_BLUETOOTH_VERSION_4P0)
    {
        CsrBtCmLeConnparamCfm *cfm;
        cfm = (CsrBtCmLeConnparamCfm*)CsrPmemAlloc(sizeof(CsrBtCmLeConnparamCfm));
        cfm->type = CSR_BT_CM_LE_CONNPARAM_CFM;
        cfm->resultSupplier = CSR_BT_SUPPLIER_CM;
        cfm->resultCode = CSR_BT_RESULT_CODE_CM_UNSUPPORTED_FEATURE;
        CsrBtCmPutMessage(req->appHandle, cfm);
        CsrBtCmFreeDownstreamMessageContents(CSR_BT_CM_PRIM,
                                             req);
        return;
    }

    dm_set_ble_connection_parameters_req(req->scanInterval,
                                         req->scanWindow,
                                         req->connIntervalMin,
                                         req->connIntervalMax,
                                         req->connLatency,
                                         req->supervisionTimeout,
                                         req->connAttemptTimeout,
                                         req->advIntervalMin,
                                         req->advIntervalMax,
                                         req->connLatencyMax,
                                         req->supervisionTimeoutMin,
                                         req->supervisionTimeoutMax,
                                         &prim);
    CsrBtCmCallbackSendSimpleBlockDm(cmData,
                                     DM_SET_BLE_CONNECTION_PARAMETERS_CFM,
                                     req,
                                     prim,
                                     csrBtCmLeConnectionSetComplete);        
    cmData->recvMsgP = NULL;
}

void CsrBtCmLeConnparamUpdateReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmLeConnparamUpdateReq *req;
    DM_UPRIM_T *prim;

    req = (CsrBtCmLeConnparamUpdateReq*)cmData->recvMsgP;

    if (cmData->dmVar.lmpVersion < CSR_BT_BLUETOOTH_VERSION_4P0)
    {
        csrBtCmLeConnparamUpdateCmpIndSend(req->appHandle,
                                           &(req->address),
                                           CSR_BT_RESULT_CODE_CM_UNSUPPORTED_FEATURE,
                                           CSR_BT_SUPPLIER_CM);
        CsrBtCmFreeDownstreamMessageContents(CSR_BT_CM_PRIM,
                                             req);
        return;
    }

    /* Change par ameters for a connection */
    dm_ble_update_connection_update_req(&req->address,
                                        req->connIntervalMin,
                                        req->connIntervalMax,
                                        req->connLatency,
                                        req->supervisionTimeout,
                                        req->minimumCeLength,
                                        req->maximumCeLength,
                                        &prim);
    CsrBtCmCallbackSendSimpleBlockDm(cmData,
                                     DM_BLE_UPDATE_CONNECTION_PARAMETERS_CFM,
                                     req,
                                     prim,
                                     csrBtCmLeConnectionUpdateComplete);
    cmData->recvMsgP = NULL;
}

void CsrBtCmLeAcceptConnparamUpdateResHandler(cmInstanceData_t *cmData)
{
    CsrBtCmLeAcceptConnparamUpdateRes *req = 
                    (CsrBtCmLeAcceptConnparamUpdateRes*) cmData->recvMsgP;
    
    dm_ble_connection_par_update_rsp(req->l2caSignalId,
                                     req->address,
                                     req->connIntervalMin,
                                     req->connIntervalMax,
                                     req->connLatency,
                                     req->supervisionTimeout,
                                     (CsrUint16)(req->accept ? L2CAP_CONNECTION_PARAMETER_UPDATE_ACCEPT
                                                             : L2CAP_CONNECTION_PARAMETER_UPDATE_REJECT),
                                     NULL);
}

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY
void CsrBtCmLeAclConnHandleIndHandler(cmInstanceData_t *cmData)
{
    DM_ACL_CONN_HANDLE_IND_T *prim = (DM_ACL_CONN_HANDLE_IND_T*) cmData->recvMsgP;

    /* Scan/advertise off */
    CsrBtCmPropgateEvent(cmData,
                         CsrBtCmPropagateLeConnStopScanAdvEvent,
                         CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY,
                         prim->status,
                         NULL,
                         NULL);
#endif

    cmData->leVar.scanMode = CSR_BT_CM_LE_MODE_OFF;
    cmData->leVar.advMode = CSR_BT_CM_LE_MODE_OFF;

    /* Connection on */
    if (prim->status == HCI_SUCCESS)
    {
#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY
        leConnVar *conn;

        CsrBtCmPropgateEvent(cmData,
                             CsrBtCmPropagateLeConnectionEvent,
                             CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY,
                             prim->status,
                             prim,
                             NULL);
#endif

        /* Add connection to LE cache */
        conn = CsrPmemAlloc(sizeof(leConnVar));
        CsrMemCpy(&(conn->dmPrim), prim, sizeof(DM_ACL_CONN_HANDLE_IND_T));

        conn->next = cmData->leVar.connCache;
        cmData->leVar.connCache = conn;
    }
}

void CsrBtCmLeAcceptConnparamUpdateIndHandler(cmInstanceData_t *cmData)
{
    DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_T * dmPrim;
    dmPrim = (DM_BLE_ACCEPT_CONNECTION_PAR_UPDATE_IND_T *) cmData->recvMsgP;

    if(cmData->gattHandle != CSR_SCHED_QID_INVALID)
    {
        CsrBtCmLeAcceptConnparamUpdateInd *ind = (CsrBtCmLeAcceptConnparamUpdateInd*)
                                                  CsrPmemAlloc(sizeof(CsrBtCmLeAcceptConnparamUpdateInd));

        ind->type               = CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_IND;
        ind->address            = dmPrim->bd_addrt; 
        ind->l2caSignalId       = dmPrim->signal_id;
        ind->connIntervalMin    = dmPrim->conn_interval_min;
        ind->connIntervalMax    = dmPrim->conn_interval_max;
        ind->connLatency        = dmPrim->conn_latency;
        ind->supervisionTimeout = dmPrim->supervision_timeout;
        CsrSchedMessagePut(cmData->gattHandle, CSR_BT_CM_PRIM, ind);
    }
    else
    {
        dm_ble_connection_par_update_rsp(dmPrim->signal_id,
                                         dmPrim->bd_addrt,
                                         dmPrim->conn_interval_min,
                                         dmPrim->conn_interval_max,
                                         dmPrim->conn_latency,
                                         dmPrim->supervision_timeout,
                                         L2CAP_CONNECTION_PARAMETER_UPDATE_REJECT,
                                         NULL);
    }
}

void CsrBtCmLeAclClosedIndHandler(cmInstanceData_t *cmData)
{
    DM_ACL_CLOSED_IND_T *prim = (DM_ACL_CLOSED_IND_T*)cmData->recvMsgP;
    leConnVar *conn;
    leConnVar **cppn;

#ifdef CSR_BT_INSTALL_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY
    CsrBtCmPropgateEvent(cmData,
                         CsrBtCmPropagateLeConnectionEvent,
                         CSR_BT_CM_EVENT_MASK_SUBSCRIBE_LOW_ENERGY,
                         HCI_SUCCESS,
                         prim,
                         NULL);
#endif

    /* Remove connection from LE cache. Use double indirection */
    for(cppn = &(cmData->leVar.connCache);
        (conn = *cppn) != NULL;
        cppn = &(conn->next))
    {
        if (CsrBtAddrEq(&(conn->dmPrim.addrt), &(prim->addrt)))
        {
            *cppn = conn->next;
            CsrPmemFree(conn);
            return;
        }
    }
}

/* LE advertise report handler */
void CsrBtCmLeReportIndHandler(cmInstanceData_t *cmInst,
                               DM_HCI_ULP_ADVERTISING_REPORT_IND_T *report)
{
    if(cmInst->gattHandle != CSR_SCHED_QID_INVALID)
    {
        CsrBtCmLeReportInd *ind;
        CsrUint8 length;
        length = report->length_data;

        ind = (CsrBtCmLeReportInd*)CsrPmemAlloc(sizeof(CsrBtCmLeReportInd));
        ind->type = CSR_BT_CM_LE_REPORT_IND;
        ind->eventType = report->event_type;
        ind->address = report->current_addrt;
        ind->permanentAddress = report->permanent_addrt;
        ind->lengthData = length;
        ind->rssi = report->rssi;
        CsrMemCpy(ind->data, report->data, length);
        CsrMemSet(&ind->data[length], 0, CSR_BT_CM_LE_MAX_REPORT_LENGTH - length);

        CsrBtCmPutMessage(cmInst->gattHandle, ind);
    }
}

void CsrBtCmLeConnectionUpdateCmpIndHandler(cmInstanceData_t *cmInst,
                                            DM_HCI_ULP_CONNECTION_UPDATE_COMPLETE_IND_T* dmPrim)
{
    /* COEX */
    csrBtCmLeParamUpdateCoex(cmInst,
                             &(dmPrim->addrt),
                             dmPrim->conn_interval,
                             dmPrim->conn_latency,
                             dmPrim->supervision_timeout);
}

/* De-init code for LE instance data */
void CsrBtCmLeDeinit(cmInstanceData_t *cmData)
{
    while(cmData->leVar.connCache)
    {
        leConnVar *next = cmData->leVar.connCache->next;
        CsrPmemFree(cmData->leVar.connCache);
        cmData->leVar.connCache = next;
    }
}

void CsrBtCmLePhysicalLinkStatusReqHandler(cmInstanceData_t *cmData)
{
    CsrBtCmLePhysicalLinkStatusReq * prim = (CsrBtCmLePhysicalLinkStatusReq *) cmData->recvMsgP;
    aclTable *acl = NULL;
   
    if (!prim->radioType &&
        (returnAclConnectionElement(cmData, CsrBtAddrAddr(prim->address), &acl) != CM_ERROR) &&
        (acl != NULL))
    {
        CsrUint8    oldMode = CsrBtCmDmReturnActualMode(cmData, CsrBtAddrAddr(prim->address));
        acl->gattConnectionActive = prim->status;
         /* Attempt to enter sniff mode. Other connections'
         * preferred modes take priority, so this may not have
         * any effect */
        if (prim->status)
        {
            CsrUint8    newMode = CsrBtCmDmReturnRequestedMode(cmData, CsrBtAddrAddr(prim->address));
            if (newMode != oldMode)
            {
                CsrBtCmDmModeChangeReqMsgSend(LE_PLAYER,
                                              0, /* index, not used */
                                              CSR_BT_SNIFF_MODE,
                                              CsrBtAddrAddr(prim->address));
            }
        }
    }
}

static void csrBtCmLeReceiverTestComplete(cmInstanceData_t *cmInst,
                                          struct cmCallbackObjTag *object,
                                          void *context,
                                          void *event)
{
    DM_HCI_ULP_RECEIVER_TEST_CFM_T *prim;
    CsrBtCmLeTransmitterTestReq *ctx;
    CsrBtCmLeTransmitterTestCfm *cfm;
    
    prim = (DM_HCI_ULP_RECEIVER_TEST_CFM_T*)event;
    ctx = (CsrBtCmLeTransmitterTestReq*)context;
    cfm = (CsrBtCmLeTransmitterTestCfm*)CsrPmemAlloc(sizeof(CsrBtCmLeTransmitterTestCfm));

    cfm->type = CSR_BT_CM_LE_RECEIVER_TEST_CFM;
    cfm->resultCode = (CsrBtResultCode)(prim->status == HCI_SUCCESS
                                        ? CSR_BT_RESULT_CODE_CM_SUCCESS
                                        : prim->status);
    cfm->resultSupplier = (CsrBtSupplier)(prim->status == HCI_SUCCESS
                                          ? CSR_BT_SUPPLIER_CM
                                          : CSR_BT_SUPPLIER_HCI);

    CsrBtCmPutMessage(ctx->appHandle, cfm);
}

void CsrBtCmLeReceiverTestReqHandler(cmInstanceData_t *cmInst)
{
    CsrBtCmLeReceiverTestReq *req;
    DM_UPRIM_T *prim;

    req = (CsrBtCmLeReceiverTestReq*)cmInst->recvMsgP;
    cmInst->recvMsgP = NULL;

    dm_hci_ulp_receiver_test_req(req->rxFrequency,
                                 &prim);

    CsrBtCmCallbackSendSimpleBlockDm(cmInst,
                                     DM_HCI_ULP_RECEIVER_TEST_CFM,
                                     req, /* ctx */
                                     prim,
                                     csrBtCmLeReceiverTestComplete);
}

static void csrBtCmLeTransmitterTestComplete(cmInstanceData_t *cmInst,
                                             struct cmCallbackObjTag *object,
                                             void *context,
                                             void *event)
{
    DM_HCI_ULP_TRANSMITTER_TEST_CFM_T *prim;
    CsrBtCmLeTransmitterTestReq *ctx;
    CsrBtCmLeTransmitterTestCfm *cfm;
    
    prim = (DM_HCI_ULP_TRANSMITTER_TEST_CFM_T*)event;
    ctx = (CsrBtCmLeTransmitterTestReq*)context;
    cfm = (CsrBtCmLeTransmitterTestCfm*)CsrPmemAlloc(sizeof(CsrBtCmLeTransmitterTestCfm));

    cfm->type = CSR_BT_CM_LE_TRANSMITTER_TEST_CFM;
    cfm->resultCode = (CsrBtResultCode)(prim->status == HCI_SUCCESS
                                        ? CSR_BT_RESULT_CODE_CM_SUCCESS
                                        : prim->status);
    cfm->resultSupplier = (CsrBtSupplier)(prim->status == HCI_SUCCESS
                                          ? CSR_BT_SUPPLIER_CM
                                          : CSR_BT_SUPPLIER_HCI);

    CsrBtCmPutMessage(ctx->appHandle,
                       cfm);
}

void CsrBtCmLeTransmitterTestReqHandler(cmInstanceData_t *cmInst)
{
    CsrBtCmLeTransmitterTestReq *req;
    DM_UPRIM_T *prim;

    req = (CsrBtCmLeTransmitterTestReq*)cmInst->recvMsgP;
    cmInst->recvMsgP = NULL;

    dm_hci_ulp_transmitter_test_req(req->txFrequency,
                                    req->lengthOfTestData,
                                    req->packetPayload,
                                    &prim);

    CsrBtCmCallbackSendSimpleBlockDm(cmInst,
                                     DM_HCI_ULP_TRANSMITTER_TEST_CFM,
                                     req, /* ctx */
                                     prim,
                                     csrBtCmLeTransmitterTestComplete);
}

static void csrBtCmLeTestEndComplete(cmInstanceData_t *cmInst,
                                     struct cmCallbackObjTag *object,
                                     void *context,
                                     void *event)
{
    DM_HCI_ULP_TEST_END_CFM_T *prim;
    CsrBtCmLeTestEndReq *ctx;
    CsrBtCmLeTestEndCfm *cfm;
    
    prim = (DM_HCI_ULP_TEST_END_CFM_T*)event;
    ctx = (CsrBtCmLeTestEndReq*)context;
    cfm = (CsrBtCmLeTestEndCfm*)CsrPmemAlloc(sizeof(CsrBtCmLeTestEndCfm));

    cfm->type = CSR_BT_CM_LE_TEST_END_CFM;
    cfm->numberOfPackets = prim->number_of_packets;
    cfm->resultCode = (CsrBtResultCode)(prim->status == HCI_SUCCESS
                                        ? CSR_BT_RESULT_CODE_CM_SUCCESS
                                        : prim->status);
    cfm->resultSupplier = (CsrBtSupplier)(prim->status == HCI_SUCCESS
                                          ? CSR_BT_SUPPLIER_CM
                                          : CSR_BT_SUPPLIER_HCI);

    CsrBtCmPutMessage(ctx->appHandle,
                       cfm);   
}

void CsrBtCmLeTestEndReqHandler(cmInstanceData_t *cmInst)
{
    CsrBtCmLeTestEndReq *req;
    DM_UPRIM_T *prim;

    req = (CsrBtCmLeTestEndReq*)cmInst->recvMsgP;
    cmInst->recvMsgP = NULL;

    dm_hci_ulp_test_end_req(&prim);

    CsrBtCmCallbackSendSimpleBlockDm(cmInst,
                                     DM_HCI_ULP_TEST_END_CFM,
                                     req, /* ctx */
                                     prim,
                                     csrBtCmLeTestEndComplete);
}
