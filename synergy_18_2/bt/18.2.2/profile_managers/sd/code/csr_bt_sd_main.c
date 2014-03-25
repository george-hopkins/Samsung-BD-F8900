/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#include "csr_bt_sd_main.h"

static const SdStateHandlerType sdStateHandlers[NUM_OF_DOWNSTREAM_APP_STATES][CSR_BT_SD_ALL_DOWNSTREAM_COUNT] =
{
    /* SD_STATE_APP_IDLE */
    {
#ifdef CSR_BT_INSTALL_SD_MEMORY_CONFIG
        CsrBtSdMemoryConfigReqIdleState,                 /* CSR_BT_SD_MEMORY_CONFIG_REQ                     */
#else
        NULL,                                            /* CSR_BT_SD_MEMORY_CONFIG_REQ                     */
#endif
#ifdef CSR_BT_INSTALL_SD_SEARCH_CONFIG
        CsrBtSdSearchConfigReqIdleState,                 /* CSR_BT_SD_SEARCH_CONFIG_REQ                     */
#else
        NULL,                                            /* CSR_BT_SD_SEARCH_CONFIG_REQ                     */
#endif
        CsrBtSdReadDeviceInfoReqAllState,                /* CSR_BT_SD_READ_DEVICE_INFO_REQ                  */
        CsrBtSdSearchReqIdleState,                       /* CSR_BT_SD_SEARCH_REQ                            */
        CsrBtSdCancelSearchReqAllState,                  /* CSR_BT_SD_CANCEL_SEARCH_REQ                     */
        CsrBtSdReadAvailableServicesReqIdleState,        /* CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ           */
        CsrBtSdCancelReadAvailableServicesReqAllState,   /* CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ    */
        CsrBtSdReadDeviceListReqAllState,                /* CSR_BT_SD_READ_DEVICE_LIST_REQ                  */
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
        CsrBtSdReadServiceRecordReqIdleState,            /* CSR_BT_SD_READ_SERVICE_RECORD_REQ               */
        CsrBtSdCancelReadServiceRecordReqAllState,       /* CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ        */
        CsrBtSdRegisterServiceRecordReqIdleState,        /* CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ           */
        CsrBtSdUnregisterServiceRecordReqIdleState,      /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ         */
#else
        NULL,                                            /* CSR_BT_SD_READ_SERVICE_RECORD_REQ               */
        NULL,                                            /* CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ        */
        NULL,                                            /* CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ           */
        NULL,                                            /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ         */
#endif  /* EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE */
        CsrBtSdCleanupReqIdleState,                      /* CSR_BT_SD_CLEANUP_REQ                           */
        CsrBtSdSynchronizeReqIdleState,                  /* CSR_BT_SD_SYNCHRONIZE_REQ                       */
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
        CsrBtSdReadServicesReqIdleState,                 /* CSR_BT_SD_READ_SERVICES_REQ                     */
        CsrBtSdReadServicesCancelReqAllState,            /* CSR_BT_SD_READ_SERVICES_CANCEL_REQ              */
#else
        NULL,                                            /* CSR_BT_SD_READ_SERVICE_RECORD_REQ               */
        NULL,                                            /* CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ        */
#endif  /* EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE */
        CsrBtSdRegisterAvailableServiceReqAllState,      /* CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ        */
        CsrBtSdUpdateDeviceReqAllState,                  /* CSR_BT_SD_UPDATE_DEVICE_REQ                     */
    },
    /* SD_STATE_APP_INQ */
    {
#ifdef CSR_BT_INSTALL_SD_MEMORY_CONFIG
        CsrBtSdUtilSaveMessage,                          /* CSR_BT_SD_MEMORY_CONFIG_REQ                     */
#else
        NULL,                                            /* CSR_BT_SD_MEMORY_CONFIG_REQ                     */
#endif
#ifdef CSR_BT_INSTALL_SD_SEARCH_CONFIG
        CsrBtSdUtilSaveMessage,                          /* CSR_BT_SD_SEARCH_CONFIG_REQ                     */
#else
        NULL,                                            /* CSR_BT_SD_SEARCH_CONFIG_REQ                     */
#endif
        CsrBtSdReadDeviceInfoReqAllState,                /* CSR_BT_SD_READ_DEVICE_INFO_REQ                  */
        CsrBtSdSearchReqInqState,                        /* CSR_BT_SD_SEARCH_REQ                            */
        CsrBtSdCancelSearchReqAllState,                  /* CSR_BT_SD_CANCEL_SEARCH_REQ                     */
        CsrBtSdSaveReadAvailableServicesReqHandler,      /* CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ           */
        CsrBtSdCancelReadAvailableServicesReqAllState,   /* CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ    */
        CsrBtSdReadDeviceListReqAllState,                /* CSR_BT_SD_READ_DEVICE_LIST_REQ                  */
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
        CsrBtSdReadServiceRecordReqInqSave,              /* CSR_BT_SD_READ_SERVICE_RECORD_REQ               */
        CsrBtSdCancelReadServiceRecordReqAllState,       /* CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ        */
        CsrBtSdRegisterServiceRecordReqSave,             /* CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ           */
        CsrBtSdUnregisterServiceRecordReqSave,           /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ         */
#else
        NULL,                                            /* CSR_BT_SD_READ_SERVICE_RECORD_REQ               */
        NULL,                                            /* CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ        */
        NULL,                                            /* CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ           */
        NULL,                                            /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ         */
#endif  /* EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE */
        CsrBtSdUtilSaveMessage,                          /* CSR_BT_SD_CLEANUP_REQ                           */
        CsrBtSdUtilSaveMessage,                          /* CSR_BT_SD_SYNCHRONIZE_REQ                       */
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
        CsrBtSdReadServicesReqInqSave,                   /* CSR_BT_SD_READ_SERVICES_REQ                     */
        CsrBtSdReadServicesCancelReqAllState,            /* CSR_BT_SD_READ_SERVICES_CANCEL_REQ              */
#else
        NULL,                                            /* CSR_BT_SD_READ_SERVICE_RECORD_REQ               */
        NULL,                                            /* CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ        */
#endif  /* EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE */
        CsrBtSdRegisterAvailableServiceReqAllState,      /* CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ        */
        CsrBtSdUpdateDeviceReqAllState,                  /* CSR_BT_SD_UPDATE_DEVICE_REQ                     */
    },
    /* SD_STATE_APP_OTHER */
    {
#ifdef CSR_BT_INSTALL_SD_MEMORY_CONFIG
        CsrBtSdUtilSaveMessage,                          /* CSR_BT_SD_MEMORY_CONFIG_REQ                     */
#else
        NULL,                                            /* CSR_BT_SD_MEMORY_CONFIG_REQ                     */
#endif
#ifdef CSR_BT_INSTALL_SD_SEARCH_CONFIG
        CsrBtSdUtilSaveMessage,                          /* CSR_BT_SD_SEARCH_CONFIG_REQ                     */
#else
        NULL,                                            /* CSR_BT_SD_SEARCH_CONFIG_REQ                     */
#endif
        CsrBtSdReadDeviceInfoReqAllState,                /* CSR_BT_SD_READ_DEVICE_INFO_REQ                  */
        CsrBtSdUtilSaveMessage,                          /* CSR_BT_SD_SEARCH_REQ                            */
        CsrBtSdCancelSearchReqAllState,                  /* CSR_BT_SD_CANCEL_SEARCH_REQ                     */
        CsrBtSdSaveReadAvailableServicesReqHandler,      /* CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ           */
        CsrBtSdCancelReadAvailableServicesReqAllState,   /* CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ    */
        CsrBtSdReadDeviceListReqAllState,                /* CSR_BT_SD_READ_DEVICE_LIST_REQ                  */
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
        CsrBtSdReadServiceRecordReqInqSave,              /* CSR_BT_SD_READ_SERVICE_RECORD_REQ               */
        CsrBtSdCancelReadServiceRecordReqAllState,       /* CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ        */
        CsrBtSdRegisterServiceRecordReqSave,             /* CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ           */
        CsrBtSdUnregisterServiceRecordReqSave,           /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ         */
#else
        NULL,                                            /* CSR_BT_SD_READ_SERVICE_RECORD_REQ               */
        NULL,                                            /* CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ        */
        NULL,                                            /* CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ           */
        NULL,                                            /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ         */
#endif  /* EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE */
        CsrBtSdUtilSaveMessage,                          /* CSR_BT_SD_CLEANUP_REQ                           */
        CsrBtSdUtilSaveMessage,                          /* CSR_BT_SD_SYNCHRONIZE_REQ                       */
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
        CsrBtSdReadServicesReqInqSave,                   /* CSR_BT_SD_READ_SERVICES_REQ                     */
        CsrBtSdReadServicesCancelReqAllState,            /* CSR_BT_SD_READ_SERVICES_CANCEL_REQ              */
#else
        NULL,                                            /* CSR_BT_SD_READ_SERVICE_RECORD_REQ               */
        NULL,                                            /* CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ        */
#endif  /* EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE */
        CsrBtSdRegisterAvailableServiceReqAllState,      /* CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ        */
        CsrBtSdUpdateDeviceReqAllState,                  /* CSR_BT_SD_UPDATE_DEVICE_REQ                     */
    },
    /* SD_STATE_APP_INIT */
    {
#ifdef CSR_BT_INSTALL_SD_MEMORY_CONFIG
        CsrBtSdUtilSaveMessage,                          /* CSR_BT_SD_MEMORY_CONFIG_REQ                     */
#else
        NULL,                                            /* CSR_BT_SD_MEMORY_CONFIG_REQ                     */
#endif
#ifdef CSR_BT_INSTALL_SD_SEARCH_CONFIG
        CsrBtSdUtilSaveMessage,                          /* CSR_BT_SD_SEARCH_CONFIG_REQ                     */
#else
        NULL,                                            /* CSR_BT_SD_SEARCH_CONFIG_REQ                     */
#endif
        CsrBtSdUtilSaveMessage,                          /* CSR_BT_SD_READ_DEVICE_INFO_REQ                  */
        CsrBtSdUtilSaveMessage,                          /* CSR_BT_SD_SEARCH_REQ                            */
        CsrBtSdCancelSearchReqAllState,                  /* CSR_BT_SD_CANCEL_SEARCH_REQ                     */
        CsrBtSdSaveReadAvailableServicesReqHandler,      /* CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ           */
        CsrBtSdCancelReadAvailableServicesReqAllState,   /* CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ    */
        CsrBtSdUtilSaveMessage,                          /* CSR_BT_SD_READ_DEVICE_LIST_REQ                  */
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
        CsrBtSdReadServiceRecordReqInqSave,              /* CSR_BT_SD_READ_SERVICE_RECORD_REQ               */
        CsrBtSdCancelReadServiceRecordReqAllState,       /* CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ        */
        CsrBtSdRegisterServiceRecordReqSave,             /* CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ           */
        CsrBtSdUnregisterServiceRecordReqSave,           /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ         */
#else
        NULL,                                            /* CSR_BT_SD_READ_SERVICE_RECORD_REQ               */
        NULL,                                            /* CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ        */
        NULL,                                            /* CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ           */
        NULL,                                            /* CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ         */
#endif  /* EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE */
        CsrBtSdUtilSaveMessage,                          /* CSR_BT_SD_CLEANUP_REQ                           */
        CsrBtSdUtilSaveMessage,                          /* CSR_BT_SD_SYNCHRONIZE_REQ                       */
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
        CsrBtSdReadServicesReqInqSave,                   /* CSR_BT_SD_READ_SERVICES_REQ                     */
        CsrBtSdReadServicesCancelReqAllState,            /* CSR_BT_SD_READ_SERVICES_CANCEL_REQ              */
#else
        NULL,                                            /* CSR_BT_SD_READ_SERVICE_RECORD_REQ               */
        NULL,                                            /* CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ        */
#endif  /* EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE */
        CsrBtSdRegisterAvailableServiceReqAllState,      /* CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ        */
        CsrBtSdUpdateDeviceReqAllState,                  /* CSR_BT_SD_UPDATE_DEVICE_REQ                     */
    },
};

void CsrBtSdInit(void **gash)
{
    SdMainInstance_t *mainInst;

    *gash                               = zpnew(SdMainInstance_t);
    mainInst                            = (SdMainInstance_t *) *gash;
    mainInst->phandle                   = CSR_SCHED_QID_INVALID;
    SD_CHANGE_STATE(mainInst->appState, SD_STATE_APP_INIT);

    /* Device List specific */
    CsrCmnListInit(&mainInst->dl, 0, NULL, CsrBtSdUtilDeviceInfoDeinit);
    mainInst->memoryConfig              = CSR_BT_SD_MEMORY_CONFIG_DEFAULT;
    mainInst->dlMax                     = CSR_BT_SD_MEMORY_LIST_MAX_DEFAULT;
    mainInst->dlInfoMax                 = CSR_BT_SD_MEMORY_LIST_INFO_MAX_DEFAULT;

    /* Search (inquiry/name) specific */
    CsrCmnListInit(&mainInst->searchInst, 0, NULL, CsrBtSdUtilSearchInstDeinit);
    mainInst->rrnActiveTime             = CSR_BT_SD_READ_NAME_TIMEOUT_DEFAULT;
    mainInst->inquiryTxPowerLevel       = CSR_BT_SD_DEFAULT_INQUIRY_TX_POWER_LEVEL;

    mainInst->sdpSearchData             = CsrBtUtilSdcInit(CsrBtSdReadSdpResultHandler, CSR_BT_SD_IFACEQUEUE);

    /* Insert the CSR_BT_PNP_INFORMATION_UUID directly, e.g request the SD to look for a
       DI profile when reading available service from a remote device */
#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
    mainInst->uuid32List                = CsrPmemAlloc(sizeof(CsrBtUuid32) * SD_POOL_SIZE_FOR_UUID32_LIST);
    mainInst->uuid32ListSize            = SD_POOL_SIZE_FOR_UUID32_LIST;
    mainInst->uuid32List[0]             = CSR_BT_PNP_INFORMATION_UUID;
    mainInst->uuid32ListCount           = 1;
#endif

    /* And register ourselves with the CM in order to receive all SD
     * related messages */
    CsrBtCmRegisterHandlerReqSend(CSR_BT_CM_HANDLER_TYPE_SD,
                                  CSR_BT_SD_IFACEQUEUE,
                                  0); /* flags */

    SD_CHANGE_STATE(mainInst->processState, SD_STATE_PROC_SC_READ_DEVICE_RECORD);
#ifdef CSR_BT_LE_ENABLE
    CsrBtGattRegisterReqSend(CSR_BT_SD_IFACEQUEUE, 0); /* context, not used */
    
    CsrBtScReadDeviceRecordWithHdlPairsReqSend(CSR_BT_SD_IFACEQUEUE, 
                                               SD_SC_READ_DEVICE_RECORD_BYTES_MAX);
#else 
    CsrBtScReadDeviceRecordReqSend(CSR_BT_SD_IFACEQUEUE, 
                                   SD_SC_READ_DEVICE_RECORD_BYTES_MAX);
    
#endif /* CSR_BT_LE_ENABLE */
}

void CsrBtSdHandler(void **gash)
{
    CsrUint16 eventClass=0;
    SdMainInstance_t *mainInst;

    mainInst = (SdMainInstance_t *) *gash;
    if (mainInst->restoreMsgFlag == FALSE)
    {
        CsrSchedMessageGet(&eventClass , &mainInst->recvMsgP);
    }
    else
    {
        if(!CsrMessageQueuePop(&mainInst->saveQueue, &eventClass, &mainInst->recvMsgP))
        {
            CsrSchedMessageGet(&eventClass , &mainInst->recvMsgP);
        }
        mainInst->restoreMsgFlag = FALSE;
    }

    switch (eventClass)
    {
        case CSR_BT_SC_PRIM:
        {
            CsrBtScPrim primType = *(CsrBtScPrim *)mainInst->recvMsgP;

            switch (primType)
            {
                case CSR_BT_SC_READ_DEVICE_RECORD_IND:
                {
                    CsrBtSdScReadDeviceRecordIndHandler(mainInst);
                    break;
                }

                case CSR_BT_SC_READ_DEVICE_RECORD_CFM:
                {
                    CsrBtSdScReadDeviceRecordCfmHandler(mainInst);
                    break;
                }

                case CSR_BT_SC_UPDATE_TRUST_LEVEL_IND:
                {
                    CsrBtSdScUpdateTrustLevelIndHandler(mainInst);
                    break;
                }

                default:
                {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    CsrGeneralException("SD", mainInst->appState, primType, "Unknown SC prim or undefined state");
#endif
                    CsrBtScFreeUpstreamMessageContents(eventClass, mainInst->recvMsgP);
                    break;
                }
            }
            break;
        }
        case CSR_BT_SD_PRIM:
        {
            CsrBtSdPrim primType = *(CsrBtSdPrim *) mainInst->recvMsgP;

            if ((CsrUint16)(primType < CSR_BT_SD_ALL_DOWNSTREAM_COUNT) &&
                (mainInst->appState < NUM_OF_DOWNSTREAM_APP_STATES)      &&
                (sdStateHandlers[mainInst->appState][primType] != NULL))
            {
                sdStateHandlers[mainInst->appState][primType](mainInst);
            }
            else if (primType != CSR_BT_SD_HOUSE_CLEANING)
            { /*State/Event ERROR! */
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrGeneralException("SD", mainInst->appState, primType, "Unknown SD prim or undefined state");
#endif
                CsrBtSdFreeDownstreamMessageContents(eventClass, mainInst->recvMsgP);
            }
            break;
        }
        case CSR_BT_CM_PRIM:
        {
            CsrBtCmPrim primType = *(CsrBtCmPrim *) mainInst->recvMsgP;

            switch (primType)
            {/* Switch is used since all the pritives are distributed all over the place */
                case CSR_BT_CM_INQUIRY_RESULT_IND:
                {
                    CsrBtSdCmInquiryResultIndHandler(mainInst);
                    break;
                }

                case CSR_BT_CM_READ_REMOTE_NAME_CFM:
                {
                    CsrBtSdCmReadRemoteNameCfmHandler(mainInst);
                    break;
                }

                case CSR_BT_CM_READ_REMOTE_NAME_IND:
                {
                    CsrBtSdCmReadRemoteNameIndHandler(mainInst);
                    break;
                }

                case CSR_BT_CM_SET_EVENT_FILTER_COD_CFM:
                {
                    CsrBtSdCmSetEventFilterCodCfm(mainInst);
                    break;
                }

                case CSR_BT_CM_CLEAR_EVENT_FILTER_CFM:
                {
                    CsrBtSdCmClearEventFilterCfm(mainInst);
                    break;
                }

#ifndef EXCLUDE_CSR_BT_SD_SERVICE_RECORD_MODULE
                case CSR_BT_CM_SDS_REGISTER_CFM:
                {
                    CsrBtSdCmSdsRegisterCfmHandler(mainInst);
                    break;
                }

                case CSR_BT_CM_SDS_UNREGISTER_CFM:
                {
                    CsrBtSdCmSdsUnregisterCfmHandler(mainInst);
                    break;
                }
#endif
                case CSR_BT_CM_INQUIRY_CFM:
                {
                    CsrBtSdCmInquiryCfmHandler(mainInst);
                    break;
                }
                default:
                {
                    if (CsrBtUtilSdcVerifyCmMsg(mainInst->recvMsgP))
                    {
                        CsrBtUtilSdcCmMsgHandler(mainInst, mainInst->sdpSearchData, mainInst->recvMsgP);
                    }
                    else
                    {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                        CsrGeneralException("SD", mainInst->appState, primType, "Unknown CM prim or undefined state");
#endif
                        CsrBtCmFreeUpstreamMessageContents(CSR_BT_CM_PRIM, mainInst->recvMsgP);
                    }
                    break;
                }
            }
            break;
        }
#ifdef CSR_BT_LE_ENABLE
        case CSR_BT_GATT_PRIM:
        {
            CsrBtSdGattHandler(mainInst);
            CsrBtGattFreeUpstreamMessageContents(eventClass, mainInst->recvMsgP);
            break;
        }
#endif
        case CSR_SCHED_PRIM:
        {
            CsrEnvPrim primType = *(CsrEnvPrim *) mainInst->recvMsgP;

            if (primType == CSR_CLEANUP_IND)
            {
                CsrBtSdEnvCleanupHandler(mainInst);
            }
            else
            {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                CsrGeneralException("SD", mainInst->appState, primType, "Unknown CSR_SCHED_PRIM or undefined state");
#endif
            }
            break;
        }
        default:
        {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
            CsrGeneralException("SD", mainInst->appState, eventClass, "Unknown primitive class received in main");
#endif
            break;
        }

    }
    CsrPmemFree(mainInst->recvMsgP);
}

#ifdef ENABLE_SHUTDOWN
/****************************************************************************
    This function is called by the scheduler to perform a graceful shutdown
    of a scheduler task.
    This function must:
    1)  empty the input message queue and free any allocated memory in the
        messages.
    2)  free any instance data that may be allocated.
****************************************************************************/
void CsrBtSdDeinit(void **gash)
{
    CsrBool              lastMsg;
    CsrUint16            msg_type=0;
    void                *msg_data=NULL;
    SdMainInstance_t    *mainInst;

    mainInst    = (SdMainInstance_t *) (*gash);
    lastMsg     = TRUE;

    while ( lastMsg )
    {
        if ( !CsrMessageQueuePop(&mainInst->saveQueue, &msg_type, &msg_data) )
        {
            lastMsg = CsrSchedMessageGet(&msg_type , &msg_data);
        }

        if (lastMsg)
        {
            switch (msg_type)
            {
                case CSR_BT_CM_PRIM:
                {
                    CsrBtCmFreeUpstreamMessageContents(msg_type, msg_data);
                    break;
                }
                case CSR_BT_SD_PRIM:
                {
                    CsrBtSdFreeDownstreamMessageContents(msg_type, msg_data);
                    break;
                }
                case CSR_BT_SC_PRIM:
                {
                    CsrBtScFreeUpstreamMessageContents(msg_type, msg_data);
                    break;
                }
#ifdef CSR_BT_LE_ENABLE
                case CSR_BT_GATT_PRIM:
                {
                    CsrBtGattFreeUpstreamMessageContents(CSR_BT_GATT_PRIM, mainInst->recvMsgP);
                    break;
                }
#endif
                case CSR_SCHED_PRIM:
                {
                    /*Just ignore */
                    break;
                }
                default:
                {
#ifndef EXCLUDE_CSR_EXCEPTION_HANDLER_MODULE
                    CsrGeneralException("SD", 0, msg_type, "Unknown PRIM In deinit");
#endif
                    break;
                }
            }
            CsrPmemFree(msg_data);
        }
    }

    CsrPmemFree(mainInst->sdpRecord);
    CsrPmemFree(mainInst->serviceHandleList);
    CsrPmemFree(mainInst->uuid32List);

    CsrBtUtilSdcRfcDeinit(&mainInst->sdpSearchData);
    CsrCmnListDeinit(&mainInst->dl);
    CsrCmnListDeinit(&mainInst->searchInst);

    CsrPmemFree(mainInst);
}
#endif /*ENABLE_SHUTDOWN */

/*=============================== END OF FILE ================================*/
