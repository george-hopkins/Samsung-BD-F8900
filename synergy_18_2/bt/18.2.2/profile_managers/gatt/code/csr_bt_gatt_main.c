/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2010

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #3 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_gatt_private.h"

static const CsrBtGattHandlerType csrBtDispatchGatt[CSR_BT_GATT_PRIM_DOWNSTREAM_COUNT] =
{
    CsrBtGattRegisterReqHandler,                 /* CSR_BT_GATT_REGISTER_REQ */  
    CsrBtGattUnregisterReqHandler,               /* CSR_BT_GATT_UNREGISTER_REQ */ 
    CsrBtGattDbAllocReqHandler,                  /* CSR_BT_GATT_DB_ALLOC_REQ */ 
    CsrBtGattDbDeallocReqHandler,                /* CSR_BT_GATT_DB_DEALLOC_REQ */
    CsrBtGattDbAddReqHandler,                    /* CSR_BT_GATT_DB_ADD_REQ */
    CsrBtGattDbRemoveReqHandler,                 /* CSR_BT_GATT_DB_REMOVE_REQ */
    CsrBtGattDbAccessResHandler,                 /* CSR_BT_GATT_DB_ACCESS_RES */
    CsrBtGattEventSendReqHandler,                /* CSR_BT_GATT_EVENT_SEND_REQ */
    CsrBtGattCentralReqHandler,                  /* CSR_BT_GATT_CENTRAL_REQ */
    CsrBtGattPeripheralReqHandler,               /* CSR_BT_GATT_PERIPHERAL_REQ */
    CsrBtGattDisconnectReqHandler,               /* CSR_BT_GATT_DISCONNECT_REQ */ 
    CsrBtGattScanReqHandler,                     /* CSR_BT_GATT_SCAN_REQ */
    CsrBtGattAdvertiseReqHandler,                /* CSR_BT_GATT_ADVERTISE_REQ */     
    CsrBtGattDiscoverServicesReqHandler,         /* CSR_BT_GATT_DISCOVER_SERVICES_REQ */
    CsrBtGattDiscoverCharacReqHandler,           /* CSR_BT_GATT_DISCOVER_CHARAC_REQ */  
    CsrBtGattDiscoverCharacDescriptorsReqHandler,/* CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_REQ */
    CsrBtGattFindInclServicesReqHandler,         /* CSR_BT_GATT_FIND_INCL_SERVICES_REQ */
    CsrBtGattReadReqHandler,                     /* CSR_BT_GATT_READ_REQ */
    CsrBtGattReadByUuidReqHandler,               /* CSR_BT_GATT_READ_BY_UUID_REQ */
    CsrBtGattReadMultiReqHandler,                /* CSR_BT_GATT_READ_MULTI_REQ */
    CsrBtGattWriteReqHandler,                    /* CSR_BT_GATT_WRITE_REQ */
    CsrBtGattCancelReqHandler,                   /* CSR_BT_GATT_CANCEL_REQ */
    CsrBtGattWriteClientConfigurationReqHandler, /* CSR_BT_GATT_WRITE_CLIENT_CONFIGURATION_REQ */
    CsrBtGattBredrConnectReqHandler,             /* CSR_BT_GATT_BREDR_CONNECT_REQ */
    CsrBtGattBredrAcceptReqHandler,              /* CSR_BT_GATT_BREDR_ACCEPT_REQ */
    CsrBtGattParamScanReqHandler,                /* CSR_BT_GATT_PARAM_SCAN_REQ */
    CsrBtGattParamAdvertiseReqHandler,           /* CSR_BT_GATT_PARAM_ADVERTISE_REQ */
#ifdef CSR_BT_INSTALL_GATT_PARAM_CONNECTION
    CsrBtGattParamConnectionReqHandler,          /* CSR_BT_GATT_PARAM_CONNECTION_REQ */
#else
    NULL,                                        /* CSR_BT_GATT_PARAM_CONNECTION_REQ */
#endif
    CsrBtGattWhitelistAddReqHandler,             /* CSR_BT_GATT_WHITELIST_ADD_REQ */
    CsrBtGattWhitelistClearReqHandler,           /* CSR_BT_GATT_WHITELIST_CLEAR_REQ */
    CsrBtGattWhitelistReadReqHandler,            /* CSR_BT_GATT_WHITELIST_READ_REQ */
    CsrBtGattParamConnUpdateReqHandler,          /* CSR_BT_GATT_PARAM_CONN_UPDATE_REQ */
    CsrBtGattSubscriptionReqHandler,             /* CSR_BT_GATT_SUBSCRIPTION_REQ */
    CsrBtGattSetEventMaskReqHandler,             /* CSR_BT_GATT_SET_EVENT_MASK_REQ */
    CsrBtGattParamConnUpdateResHandler,          /* CSR_BT_GATT_PARAM_CONN_UPDATE_RES */
    CsrBtGattSecurityReqHandler,                 /* CSR_BT_GATT_SECURITY_REQ */
};

static const CsrBtGattAttHandlerType csrBtDispatchAtt[CSR_BT_GATT_ATT_PRIM_UP_COUNT] =
{
    CsrBtGattAttRegisterCfmHandler,                 /* ATT_REGISTER_CFM */  
    NULL,                                           /* ATT_UNREGISTER_CFM (Never unregister Att)*/ 
    NULL,                                           /* ATT_ADD_DB_CFM (Robinson only)*/ 
    CsrBtGattAttAddCfmHandler,                      /* ATT_ADD_CFM */
    CsrBtGattAttRemoveCfmHandler,                   /* ATT_REMOVE_CFM */
    CsrBtGattAttConnectCfmHandler,                  /* ATT_CONNECT_CFM */
    CsrBtGattAttConnectIndHandler,                  /* ATT_CONNECT_IND */
    NULL,                                           /* ATT_DISCONNECT_CFM (Robinson only) */
    CsrBtGattAttDisconnectIndHandler,               /* ATT_DISCONNECT_IND */
    CsrBtGattAttExchangeMtuCfmHandler,              /* ATT_EXCHANGE_MTU_CFM */ 
    CsrBtGattAttExchangeMtuIndHandler,              /* ATT_EXCHANGE_MTU_IND */
    CsrBtGattAttFindInfoCfmHandler,                 /* ATT_FIND_INFO_CFM */     
    CsrBtGattAttFindByTypeValueCfmHandler,          /* ATT_FIND_BY_TYPE_VALUE_CFM */
    CsrBtGattAttReadByTypeCfmHandler,               /* ATT_READ_BY_TYPE_CFM */
    CsrBtGattAttReadCfmHandler,                     /* ATT_READ_CFM */   
    CsrBtGattAttReadBlobCfmHandler,                 /* ATT_READ_BLOB_CFM */
    CsrBtGattAttReadMultiCfmHandler,                /* ATT_READ_MULTI_CFM */
    CsrBtGattAttReadByGroupTypeCfmHandler,          /* ATT_READ_BY_GROUP_TYPE_CFM */
    CsrBtGattAttWriteCfmHandler,                    /* ATT_WRITE_CFM */
    CsrBtGattAttPrepareWriteCfmHandler,             /* ATT_PREPARE_WRITE_CFM */
    CsrBtGattAttExecuteWriteCfmHandler,             /* ATT_EXECUTE_WRITE_CFM */
    CsrBtGattAttHandleValueCfmHandler,              /* ATT_HANDLE_VALUE_CFM */
    CsrBtGattAttHandleValueIndHandler,              /* ATT_HANDLE_VALUE_IND */
    CsrBtGattAttAccessIndHandler                    /* ATT_ACCESS_IND */
};

void CsrBtGattInit(void **gash)
{
    GattMainInst *inst = CsrPmemZalloc(sizeof(GattMainInst));
    *gash              = inst;
    CsrBtGattInitHandler(inst);   
}

void CsrBtGattInterfaceHandler(void **gash)
{
    CsrUint16    eventClass=0;
    GattMainInst *inst = *gash;

    if (CsrSchedMessageGet(&eventClass, &inst->msg))
    {
        CsrPrim type = *(CsrPrim *)inst->msg;

        switch (eventClass)
        {
            case CSR_BT_GATT_PRIM:
            {
                if (type < CSR_BT_GATT_PRIM_DOWNSTREAM_COUNT)
                {
                    if(!csrBtDispatchGatt[type](inst))
                    {
                        CsrStateEventException("No Gatt owner", eventClass, type, 0);
                    }
                }
                else
                {
                    CsrStateEventException("GATT handler - unknown GATT primitive", eventClass, type, 0);
                }

                if (inst->msg)
                {
                    CsrBtGattFreeDownstreamMessageContents(eventClass, inst->msg);
                }
                break;
            }
            case CSR_BT_ATT_PRIM:
            {
                if ((type - ATT_PRIM_UP) < CSR_BT_GATT_ATT_PRIM_UP_COUNT &&
                    csrBtDispatchAtt[(CsrUint16)(type - ATT_PRIM_UP)])
                {
                    csrBtDispatchAtt[(CsrUint16)(type - ATT_PRIM_UP)](inst);
                }
                else
                {
                    CsrStateEventException("GATT handler - unknown ATT primitive", eventClass, type, 0);
                }
                attlib_free((ATT_UPRIM_T *)inst->msg);
                inst->msg = NULL;
                break;
            }
            case CSR_BT_CM_PRIM:
            {
                if (CsrBtUtilSdcVerifyCmMsg(inst->msg))
                { /* Running SDP Search */
                    CsrBtUtilSdcCmMsgHandler(inst, inst->sdpSearchData, inst->msg);
                    /* Note the function CsrBtUtilSdcCmMsgHandler 
                       is responsible for calling 
                       CsrBtCmFreeUpstreamMessageContents */
                    ;
                }
                else
                {
                    CsrBtGattDispatchCm(inst);
                    
                    if (inst->msg)
                    {                    
                        CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, inst->msg);
                    }
                }
                break;
            }
            case CSR_BT_SC_PRIM:
            {
                CsrBtGattDispatchSc(inst);
                CsrBtScFreeUpstreamMessageContents(CSR_BT_SC_PRIM, inst->msg);
                break;
            }
            case DM_PRIM:
            {
                if(*(CsrUint16*)inst->msg == DM_SM_REGISTER_CFM)
                {
                    /* Ignore */
                }
                else
                {
                    CsrStateEventException("GATT handler - unknown ATT primitive", eventClass, type, 0);
                }
                dm_free_upstream_primitive((DM_UPRIM_T*)inst->msg);
                inst->msg = NULL;
                break;
            }
            case CSR_BT_SD_PRIM:
            {
                if (type == CSR_BT_SD_READ_DEVICE_INFO_CFM)
                {
                    CsrBtGattSdReadDeviceInfoCfmHandler(inst);
                }
                else
                {
                    CsrStateEventException("GATT handler - unknown SD primitive", eventClass, type, 0);
                }
                CsrBtSdFreeUpstreamMessageContents(CSR_BT_SD_PRIM, inst->msg);
                CsrBtSdPrivateFreeUpstreamMessageContents(CSR_BT_SD_PRIM, inst->msg); 
                break;
            }
            default:
                CsrStateEventException("GATT handler - unknown event class/type", eventClass, type, 0);
                break;
        }
        CsrPmemFree(inst->msg);
    }
}

#ifdef ENABLE_SHUTDOWN
void CsrBtGattDeinit(void **gash)
{
    CsrUint16    eventClass=0;
    GattMainInst *inst;
    inst = *gash;
    
    while (CsrSchedMessageGet(&eventClass, &inst->msg))
    {
        
        switch (eventClass)
        {
            case CSR_BT_GATT_PRIM:
                CsrBtGattFreeDownstreamMessageContents(eventClass, inst->msg);
                break;

            case CSR_BT_ATT_PRIM:
                attlib_free((ATT_UPRIM_T *)inst->msg);
                inst->msg = NULL;
                break;

            case CSR_BT_CM_PRIM:
                CsrBtCmFreeUpstreamMessageContents(eventClass, inst->msg);
                break;

            case CSR_BT_SC_PRIM:
                CsrBtScFreeUpstreamMessageContents(CSR_BT_SC_PRIM, inst->msg);
                break;

            case DM_PRIM:
                dm_free_upstream_primitive((DM_UPRIM_T*)inst->msg);
                inst->msg = NULL;
                break;
            case CSR_BT_SD_PRIM:
                CsrBtSdFreeUpstreamMessageContents(CSR_BT_SD_PRIM, inst->msg);
                CsrBtSdPrivateFreeUpstreamMessageContents(CSR_BT_SD_PRIM, inst->msg);
                break;
            default:
                CsrStateEventException("GATT deinit - unknown event class", eventClass, 0, 0);
                break;
        }
        CsrPmemFree(inst->msg);
    }

    CsrCmnListDeinit(&(inst->queue));
    CsrCmnListDeinit(&(inst->appInst));
    CsrCmnListDeinit(&(inst->prepare));
    CsrCmnListDeinit(&inst->connInst);
    CsrCmnListDeinit(&(inst->sdpRecordInst));
    CsrBtUtilSdcRfcDeinit(&inst->sdpSearchData);
    CsrPmemFree(inst->whitelist);
    CsrPmemFree(inst->localName);
    CsrPmemFree(inst);
    *gash = NULL;
}
#endif

