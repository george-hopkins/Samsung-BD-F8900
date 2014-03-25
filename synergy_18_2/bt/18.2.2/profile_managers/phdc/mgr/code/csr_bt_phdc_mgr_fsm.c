/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_MODULE

#include "csr_bt_phdc_mgr_fsm.h"
#include "csr_bt_phdc_common.h"
#include "csr_bt_tasks.h"
#include "csr_bt_phdc_mgr_out.h"
#include "csr_bt_phdc_mgr_prim.h"
#include "csr_bt_hdp_lib.h"
#include "csr_bt_phdc_mgr_util.h"
#include "csr_pmem.h"

#define PHDC_MGR_DATA_CONFIG_OBJECT_REPORT_OFFSET   25
#define PHDC_MGR_DATA_CONFIG_OBJECT_REPORT_HEADER   8   /* 2+2+2+2 for type, handle, length and count */

#define PHDC_MGR_DATA_VAR_COUNT_OFFSET              27
#define PHDC_MGR_DATA_VAR_FIRST_OFFSET              31
#define PHDC_MGR_DATA_VAR_MP_PERSON_ID_OFFSET       31
#define PHDC_MGR_DATA_REPORT_HEADER_OFFSET          4
#define PHDC_MGR_DATA_VAR_MP_EXTRA_OFFSET           6


static CsrUint16 mapRlReason[] = {  CSR_BT_PHDC_MGR_RLRQ_REASON_NORMAL                  /* RELEASE_REQUEST_REASON_NORMAL*/,
                                    CSR_BT_PHDC_MGR_RLRQ_REASON_NO_MORE_CONFIGURATIONS  /* RELEASE_REQUEST_REASON_NO_MORE_CONFIG*/,
                                    CSR_BT_PHDC_MGR_RLRQ_REASON_CONFIGURATION_CHANGED   /* RELEASE_REQUEST_REASON_CONFIG_CHANGED*/};

static CsrUint16 mapAbrtReason[] = {CSR_BT_PHDC_MGR_RESULT_CODE_ABORT_REASON_UNDEFINED,
                                    CSR_BT_PHDC_MGR_RESULT_CODE_ABORT_REASON_BUFFER_OVERFLOW,
                                    CSR_BT_PHDC_MGR_RESULT_CODE_ABORT_REASON_RESPONSE_TMEOUT,
                                    CSR_BT_PHDC_MGR_RESULT_CODE_ABORT_REASON_CONFIGURATION_TMEOUT};

typedef struct
{
    CsrBtPhdcMgrInstance    *pInstance;
    void                    *argP;
} CsrBtPhdcFsmParam;


void csrBtPhdcMgrFsmRun(CsrBtPhdcMgrInstance *pInstance,
                        fsm_event_t event,
                        void *param2)
{
    fsm_result_t fsmResult;
    CsrBtPhdcFsmParam param;

    param.pInstance = pInstance;

    fsmResult = fsm_16bit_run(&csr_bt_phdc_mgr_fsm, 
                              &pInstance->state, 
                              event, 
                              &param,
                              param2);

    CSR_BT_PHDC_M_ASSERT(FSM_RESULT_OK == fsmResult);

    (void)fsmResult;
}

void csrBtPhdcMgrDeviceFsmRun(CsrBtPhdcMgrInstance *pInstance,
                              PhdcMgrDeviceListEntry *pDevice,
                              fsm_event_t event,
                              void *param2)
{
    fsm_result_t fsmResult;
    CsrBtPhdcFsmParam param;

    param.pInstance = pInstance;
    param.argP = pDevice;

    fsmResult = fsm_16bit_run(&csr_bt_phdc_mgr_device_fsm, 
                              &pDevice->state, 
                              event, 
                              &param,
                              param2);

    CSR_BT_PHDC_M_ASSERT(FSM_RESULT_OK == fsmResult);

    (void)fsmResult;
}

            
/* local functions */
static void PhdcMgrTimedEvent(CsrUint16 event, void *pv_arg1)
{
    CsrBtPhdcFsmParam *param;
    fsm_result_t fsmResult;
    PhdcMgrDeviceListEntry *pDevice;
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    
    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pDevice = (PhdcMgrDeviceListEntry *)param->argP;
    pDevice->timerId = 0;

    fsmResult = fsm_16bit_run(&csr_bt_phdc_mgr_device_fsm, 
                              &pDevice->state, 
                              event, 
                              param,
                              NULL);

    CSR_BT_PHDC_M_ASSERT(FSM_RESULT_OK == fsmResult);    

    (void)fsmResult;

    CsrPmemFree(pv_arg1);

}

static void csrBtPhdcMgrCancelTimer(PhdcMgrDeviceListEntry *pDevice)
{
    if (pDevice->timerId)
    {
        if (CsrSchedTimerCancel(pDevice->timerId, NULL, NULL))
        {
            CsrPmemFree(pDevice->fsmParam);
        }
        pDevice->timerId = 0;
    }
}

static void csrBtPhdcMgrSetTimer(CsrBtPhdcFsmParam *param, 
                                 PhdcMgrDeviceListEntry* *pDevice, 
                                 CsrUint32 time, 
                                 fsm_event_t event)
{
    CsrBtPhdcFsmParam *pFsmParam;
    
    pFsmParam = CsrPmemAlloc(sizeof(*pFsmParam));
    *pFsmParam = *param;
    pFsmParam->argP = *pDevice;
    (*pDevice)->fsmParam = (void *)pFsmParam;
    
    (*pDevice)->timerId = CsrSchedTimerSet(time, PhdcMgrTimedEvent, event, (void *) pFsmParam);
}

static CsrBool PhdcRemoveConfigObjElement(CsrCmnListElm_t *elem, void *data)
{
    CSR_UNUSED(elem);
    CSR_UNUSED(data);
    return TRUE;
}

static void csrBtPhdcMgrFreeDeviceMds(CsrBtPhdcMgrInstance *pInstance, PhdcMgrDeviceListEntry *pDevice)
{
    CsrPmemFree(pDevice->pMds->systemId);
    if (pDevice->pMds->pSpec != NULL )
    {
        if (pDevice->pMds->pSpec->length > 0 )
        {
            CsrPmemFree(pDevice->pMds->pSpec->value);
        }
        CsrPmemFree(pDevice->pMds->pSpec);
    }
    if (pDevice->pMds->pSavedApdu)
    {
        CsrBtPhdcMgrFreeDecodeAarq(pDevice->pMds->pSavedApdu);
        pDevice->pMds->pSavedApdu = NULL;
    }
    CsrCmnListDeinit(&pDevice->pMds->configObjList);
    if(pInstance)
    {
        PHDC_MGR_DEVICE_LIST_REMOVE_DEVICE(pInstance->deviceList, pDevice);
    }

}

static CsrBool PhdcRemoveDeviceListElement(CsrCmnListElm_t *elem, void *data)
{
    PhdcMgrDeviceListEntry *pDevice = (PhdcMgrDeviceListEntry *) elem;

    CSR_UNUSED(data);

    CsrCmnListIterateAllowRemove(&pDevice->pMds->configObjList,
                                 PhdcRemoveConfigObjElement,
                                 NULL);
    
    
    csrBtPhdcMgrFreeDeviceMds(NULL, pDevice);

    csrBtPhdcMgrCancelTimer(pDevice);

    return TRUE;
}


static fsm_event_t csrBtPhdcMgrErrorHandler(CsrBtPhdcMgrInstance *pInstance, PhdcMgrDeviceListEntry *pDevice, APDU *pApdu, CsrUint8 currentState)
{
    CsrUint16 choice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    csrBtPhdcMgrCancelTimer(pDevice);
    /* check for rlrq, rlre or abrt */
    switch (pApdu->choice)
    {
        case RLRQ_CHOSEN: 
        {
            if(currentState == CSR_BT_PHDC_MGR_DEVICE_ST_CONFIG_WAITING || 
               currentState == CSR_BT_PHDC_MGR_DEVICE_ST_CONFIG_CHECKING)
            {
                pDevice->reason = mapRlReason[pApdu->u.rlrq.reason];
                CsrBtPhdc20601RlreEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId,pApdu->u.rlrq.reason );
                returnEvent = PHDC_MGR_DEVICE_EV_INT_RELEASE_IND;
            }
            else if (currentState == CSR_BT_PHDC_MGR_DEVICE_ST_UNASSOCIATED)
            {
                pDevice->reason = mapAbrtReason[ABORT_REASON_UNDEFINED];
                CsrBtPhdc20601AbrtEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId,ABORT_REASON_UNDEFINED);
                returnEvent = PHDC_MGR_DEVICE_EV_INT_ABORT;
            }
            else
            {
                pDevice->reason = mapRlReason[RELEASE_REQUEST_REASON_NORMAL];
                CsrBtPhdc20601RlreEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId,RELEASE_REQUEST_REASON_NORMAL );
                returnEvent = PHDC_MGR_DEVICE_EV_INT_RELEASE_IND;
            }
            break;
        }
        case AARQ_CHOSEN: 
        case AARE_CHOSEN: 
        case RLRE_CHOSEN: 
        {
            pDevice->reason = mapAbrtReason[ABORT_REASON_UNDEFINED];
            CsrBtPhdc20601AbrtEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId, ABORT_REASON_UNDEFINED);
            returnEvent = PHDC_MGR_DEVICE_EV_INT_ABORT;
            break;
        }      

        case ABRT_CHOSEN: 
        {
            if(currentState == CSR_BT_PHDC_MGR_DEVICE_ST_DISASSOCIATING || 
               currentState == CSR_BT_PHDC_MGR_DEVICE_ST_OPERATING)
            {
                CsrBtPhdcMgrDisassociateIndSend(pInstance->qId,
                                                pDevice->dataChannelId,  
                                                &pDevice->bdAddr, 
                                                CSR_BT_PHDC_MGR_RESULT_CODE_RCVD_ABRT, 
                                                CSR_BT_SUPPLIER_PHDC);               
            }
            else
            {
                pDevice->aarqOngoing = AARQ_ONGOING_FALSE;
                CsrBtPhdcMgrSendAssociateCompleteIndSend(pInstance->qId, 
                                                         CSR_BT_PHDC_MGR_INVALID_VAL, 
                                                         &pDevice->bdAddr, 
                                                         pDevice->mdepId, 
                                                         CSR_BT_PHDC_MGR_INVALID_VAL, 
                                                         pDevice->maxPacketLength,
                                                         pDevice->dataChannelId, 
                                                         CSR_BT_PHDC_MGR_RESULT_CODE_RCVD_ABRT, 
                                                         CSR_BT_SUPPLIER_PHDC);        
            }
            returnEvent = PHDC_MGR_DEVICE_EV_INT_ABORT_IND;

            break;
        }

        case PRST_CHOSEN:
        {               
            if(currentState == CSR_BT_PHDC_MGR_DEVICE_ST_OPERATING)
            {
                /* 
                ROIV_CMIP_GET_CHOSEN
                ROIV_CMIP_SET_CHOSEN
                ROIV_CMIP_CONFIRMED_SET_CHOSEN
                ROIV_CMIP_ACTION_CHOSEN
                ROIV_CMIP_CONFIRMED_ACTION_CHOSEN
                */
                CsrBtPhdcMgrUpdateInvokeIdAck(pApdu,pDevice);
                    
                if (CsrBtPhdcMgrCanTrasmit(pDevice) )
                {
                    CsrBtPhdc20601RoerEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE,
                                                 pDevice->dataChannelId,
                                                 pDevice->invokeIdAck,
                                                 NO_SUCH_OBJECT_INSTANCE);
                    returnEvent = PHDC_MGR_DEVICE_EV_INT_DATA_ERROR;
                }
                else
                {
                    CSR_BT_PHDC_M_ASSERT(FALSE);
                    returnEvent = PHDC_MGR_DEVICE_EV_INT_ABORT_IND;/* Just to go to Unassociated directly */
                }                
            }
            else
            {
                CsrBtPhdc20601AbrtEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId, ABORT_REASON_UNDEFINED);
                pDevice->reason = mapAbrtReason[ABORT_REASON_UNDEFINED];
                returnEvent = PHDC_MGR_DEVICE_EV_INT_ABORT;
            }
            break;
        }
        default :
        {
            /* Anything else then abort */
            pDevice->reason = mapAbrtReason[ABORT_REASON_UNDEFINED];
            CsrBtPhdc20601AbrtEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId, ABORT_REASON_UNDEFINED);
            returnEvent = PHDC_MGR_DEVICE_EV_INT_ABORT;
        
            break; 
        }
    }
    
    CsrBtPhdcMgrFreeDecodeAnyApdu(pApdu,&choice);
    if (pDevice->pMds)
    {
        if (pDevice->pMds->pSavedApdu == pApdu)
        {
            pDevice->pMds->pSavedApdu = NULL;
        }
    }

    
    return returnEvent;
}

/* Idle */
FSM_ACTION_FN(CsrBtPhdcMgrFsm_IdleSPhdcConfigureEndpointReq)
{
    CsrBtPhdcMgrConfigureEndpointReq *pReq;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
        
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pReq = (CsrBtPhdcMgrConfigureEndpointReq *)pInstance->pRecvMsg;

    if (pInstance->qId == CSR_BT_PHDC_MGR_INVALID_VAL &&
        pInstance->bActivated == FALSE)
    {
        pInstance->qId = pReq->qId;
        pInstance->noActiveConnections = pReq->numOfActiveDeviceConnections;

        CsrBtHdpRegisterReqSend(CSR_BT_PHDC_MGR_IFACEQUEUE,
                                pReq->secLevel,
                                pReq->serviceName,
                                pReq->serviceDescription,
                                pReq->providerName,
                                pReq->sniffTimeOut,
                                pReq->numOfMdep,
                                pReq->supportedProcedures);
        pReq->serviceName = NULL;
        pReq->serviceDescription = NULL;
        pReq->providerName = NULL;
    }
    else
    {
        /* !!! Should maintain state */
        CsrBtPhdcMgrConfigureEndpointCfmSend(pReq->qId,
                                             CSR_BT_PHDC_MGR_RESULT_CODE_ALREADY_CONFIGURED,
                                             CSR_BT_SUPPLIER_PHDC);
    }
    return FSM_EVENT_NULL;
}

/* Registering */
FSM_ACTION_FN(CsrBtPhdcMgrFsm_RegisteringSHdpRegisterInd)
{
    CsrBtHdpRegisterInd *pInd;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
        
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pInd = (CsrBtHdpRegisterInd *)pInstance->pRecvMsg;

    CsrBtPhdcMgrConfigureEndpointIndSend(pInstance->qId, pInd->mdepId);    
    return FSM_EVENT_NULL;
}

FSM_ACTION_FN(CsrBtPhdcMgrFsm_RegisteringSPhdcConfigureEndpointRes)
{
    CsrBtPhdcMgrConfigureEndpointRes *pRes;
    PhdcMgrDeviceDataTypeEntry *pTypeEntry;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
       
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pRes = (CsrBtPhdcMgrConfigureEndpointRes *)pInstance->pRecvMsg;
    
    pTypeEntry = PHDC_MGR_TYPE_SPEC_FIND_FROM_DATA_TYPE(pInstance->typeSpec, &pRes->datatype);

    if (pTypeEntry == NULL)
    {
        pTypeEntry = PHDC_MGR_TYPE_SPEC_ADD_TYPE(pInstance->typeSpec);

        pTypeEntry->dataType = pRes->datatype;
        pTypeEntry->deviceSpec = PhdcMgrMapDataTypeToIEEE(pRes->datatype);
        CSR_BT_PHDC_M_ASSERT(pTypeEntry->deviceSpec != CSR_BT_PHDC_MGR_INVALID_VAL);
    }
    
    CsrBtHdpRegisterRspSend(CSR_BT_PHDC_MGR_IFACEQUEUE,
                            pRes->datatype,
                            pRes->role,
                            pRes->description,
                            pRes->reuseMdepId);
    pRes->description = NULL;
    return FSM_EVENT_NULL;
}

FSM_ACTION_FN(CsrBtPhdcMgrFsm_RegisteringSHdpRegisterCfm)
{
    CsrBtHdpRegisterCfm *pCfm;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pCfm = (CsrBtHdpRegisterCfm *)pInstance->pRecvMsg;

    if (pCfm->resultSupplier == CSR_BT_SUPPLIER_HDP && 
        pCfm->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS )
    {
        CsrBtHdpActivateReqSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pInstance->noActiveConnections)
    }
    else
    {
        /* !!! Error handling */
        CsrBtPhdcMgrConfigureEndpointCfmSend(pInstance->qId, pCfm->resultCode, pCfm->resultSupplier);
    }
    return FSM_EVENT_NULL;
}

/* Activating */
FSM_ACTION_FN(CsrBtPhdcMgrFsm_ActivatingSHdpActivateCfm)
{
    CsrBtHdpActivateCfm *pCfm;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pCfm = (CsrBtHdpActivateCfm *)pInstance->pRecvMsg;

    if (pCfm->resultSupplier == CSR_BT_SUPPLIER_HDP && 
        pCfm->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS)
    {
        pInstance->bActivated = TRUE;
        CsrBtPhdcMgrConfigureEndpointCfmSend(pInstance->qId, CSR_BT_PHDC_MGR_RESULT_CODE_SUCCESS, 
                                             CSR_BT_SUPPLIER_PHDC);
    }
    else
    {
        CsrBtPhdcMgrConfigureEndpointCfmSend(pInstance->qId, pCfm->resultCode, pCfm->resultSupplier);
    }
    return FSM_EVENT_NULL;
}

/* Deactivating */
FSM_ACTION_FN(CsrBtPhdcMgrFsm_DeactivatingSHdpDeactivateCfm)
{
    CsrBtHdpDeactivateCfm *pCfm;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    
    pCfm = (CsrBtHdpDeactivateCfm *)pInstance->pRecvMsg;

    if (pCfm->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS ||
        pCfm->resultSupplier == CSR_BT_SUPPLIER_HDP)
    {
        pInstance->bActivated = FALSE;
        CsrBtHdpUnregisterReqSend(CSR_BT_PHDC_MGR_IFACEQUEUE); 
    }
    else
    {
        CsrBtPhdcMgrUnregisterCfmSend(pInstance->qId, pCfm->resultCode, pCfm->resultSupplier);
    }
    return FSM_EVENT_NULL;
}

FSM_ACTION_FN(CsrBtPhdcMgrFsm_DeactivatingSHdpDisconnectAssociateChannelCfm)
{
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;

    if ( pInstance->bActivated == TRUE )
    {
        CsrBtHdpDeactivateReqSend(CSR_BT_PHDC_MGR_IFACEQUEUE);
    }
    else
    {
        CsrBtHdpUnregisterReqSend(CSR_BT_PHDC_MGR_IFACEQUEUE);           
    }

    return FSM_EVENT_NULL;
}

/* Unregistering */
FSM_ACTION_FN(CsrBtPhdcMgrFsm_UnregisteringSHdpUnregisterCfm)
{
    CsrBtHdpUnregisterCfm *pCfm;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pCfm = (CsrBtHdpUnregisterCfm *)pInstance->pRecvMsg;

    if (pCfm->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS ||
        pCfm->resultSupplier == CSR_BT_SUPPLIER_HDP)
    {
        CsrCmnListIterateAllowRemove(&pInstance->deviceList, 
                                     PhdcRemoveDeviceListElement, 
                                     NULL);

        CsrCmnListIterateAllowRemove(&pInstance->typeSpec, 
                                     PhdcRemoveConfigObjElement, 
                                     NULL);
        
        CsrBtPhdcMgrUnregisterCfmSend(pInstance->qId, 
                                      CSR_BT_PHDC_MGR_RESULT_CODE_SUCCESS, 
                                      CSR_BT_SUPPLIER_PHDC);
        
        pInstance->qId = CSR_BT_PHDC_MGR_INVALID_VAL;
    }
    else
    {
        CsrBtPhdcMgrUnregisterCfmSend(pInstance->qId, 
                                      pCfm->resultCode, 
                                      pCfm->resultSupplier);
    }
    return FSM_EVENT_NULL;
}

/* Any state */
FSM_ACTION_FN(CsrBtPhdcMgrFsm_AnyStatePhdcUnregisterReq)
{
    CsrBtPhdcMgrUnregisterReq *pReq;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    PhdcMgrDeviceListEntry *pDevice = NULL;
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pReq = (CsrBtPhdcMgrUnregisterReq *)pInstance->pRecvMsg;

    if(pInstance->deviceList.count)
    {
        pDevice = (PhdcMgrDeviceListEntry *)(( *(pInstance)).deviceList).first;

        if(pDevice && (pDevice->state != CSR_BT_PHDC_MGR_DEVICE_ST_DISCONNECTED) )
        {
            /* Disconnect BT link */
            CsrBtHdpDisconnectAssociateChannelReqSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId);
        }
        else
        {
            CsrBtHdpDeactivateReqSend(CSR_BT_PHDC_MGR_IFACEQUEUE);
        }
    }
    else
    {
        if (pReq->qId == pInstance->qId)  /* new connection from the device*/
        {
            if ( pInstance->bActivated == TRUE )
            {
                CsrBtHdpDeactivateReqSend(CSR_BT_PHDC_MGR_IFACEQUEUE);
            }
            else
            {
                CsrBtHdpUnregisterReqSend(CSR_BT_PHDC_MGR_IFACEQUEUE);           
            }
        }
        else
        {
            /* The error could be because it is a request from not a registered Qid */
            CsrBtPhdcMgrUnregisterCfmSend(pReq->qId,
                                          CSR_BT_PHDC_MGR_RESULT_CODE_UNKNOWN,
                                          CSR_BT_SUPPLIER_PHDC);
        }
    }
    return FSM_EVENT_NULL;
}


FSM_ACTION_FN(CsrBtPhdcMgrFsm_AnyStateHdpDisconnectAssociateChannelInd)
{
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    
    if ( pInstance->bActivated == TRUE )
    {
        CsrBtHdpDeactivateReqSend(CSR_BT_PHDC_MGR_IFACEQUEUE);
    }
    else
    {
        CsrBtHdpUnregisterReqSend(CSR_BT_PHDC_MGR_IFACEQUEUE);           
    }

    return FSM_EVENT_NULL;
}

FSM_ACTION_FN(CsrBtPhdcMgrFsm_AnyStateHdpCtrlGetCapabInd)
{
    CsrBtHdpCtrlGetCapabInd *pInd;
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pInd =  (CsrBtHdpCtrlGetCapabInd *)pInstance->pRecvMsg;    
    pDevice = PHDC_MGR_DEVICE_LIST_FIND_FROM_BD(pInstance->deviceList, &pInd->deviceAddr);
            
    if(pDevice != NULL)
    {
        pDevice->psmIdentifier = pInd->hdpInstanceId;
    }  
    
    if(pInstance->bGetCapab)
    {
        CsrBtPhdcMgrGetDeviceCapabIndSend(pInstance->qId,
                                          &pInd->deviceAddr,
                                          pInd->hdpInstanceId,
                                          pInd->serviceName,
                                          pInd->providerName,
                                          pInd->supportedFeatureListLen,
                                          pInd->supportedFeatureList);
    }
    else
    {
        CsrPmemFree(pInd->serviceName);
        CsrPmemFree(pInd->serviceDescrip);
        CsrPmemFree(pInd->providerName);
        CsrPmemFree(pInd->supportedFeatureList);
    }
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcMgrFsm_AnyStatePhdcGetDeviceCapabReq)
{
    CsrBtPhdcMgrGetDeviceCapabReq *pReq;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pReq =  (CsrBtPhdcMgrGetDeviceCapabReq *)pInstance->pRecvMsg;

    if (pInstance->bGetCapab == FALSE)
    {
        pInstance->bGetCapab = TRUE;
        CsrBtHdpGetCapabReqSend(CSR_BT_PHDC_MGR_IFACEQUEUE,
                                pReq->deviceAddr,
                                pReq->mdepDataTypeMask,
                                0x00,
                                CSR_BT_HDP_SOURCE_ROLE);
    }
    else 
    {
        CsrBtPhdcMgrGetDeviceCapabCfmSend(pInstance->qId,
                                         &pReq->deviceAddr,
                                         CSR_BT_PHDC_MGR_RESULT_CODE_BUSY,
                                         CSR_BT_SUPPLIER_PHDC);
    }
    return FSM_EVENT_NULL;
}

FSM_ACTION_FN(CsrBtPhdcMgrFsm_AnyStateHdpCtrlGetCapabCfm)
{
    CsrBtHdpCtrlGetCapabCfm *pCfm;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pCfm = (CsrBtHdpCtrlGetCapabCfm *)pInstance->pRecvMsg;

    if ( pCfm->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS )
    {
        pCfm->resultCode = CSR_BT_PHDC_MGR_RESULT_CODE_SUCCESS;
        pCfm->resultSupplier = CSR_BT_SUPPLIER_PHDC;
    }
    pInstance->bGetCapab = FALSE;
    CsrBtPhdcMgrGetDeviceCapabCfmSend(pInstance->qId,
                                     &pCfm->deviceAddr,
                                     pCfm->resultCode,
                                     pCfm->resultSupplier);
    return FSM_EVENT_NULL;
}

FSM_ACTION_FN(CsrBtPhdcMgrFsm_AnyStateHdpResumeInd)
{
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    PhdcMgrDeviceListEntry *pDevice = NULL;
    CsrBtHdpResumeInd *pInd;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);
    
    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pInd = (CsrBtHdpResumeInd *)pInstance->pRecvMsg;

    /* Setup device from current information and send ResumeRes*/
    pDevice = PHDC_MGR_DEVICE_LIST_FIND_FROM_BD(pInstance->deviceList, &pInd->deviceAddr);
                    
    if(pDevice)
    {
        pDevice->dataChannelId = pInd->chId;
        CsrBtHdpResumeResSend(CSR_BT_PHDC_MGR_IFACEQUEUE,pInd->chId,TRUE,pDevice->maxPacketLength);
    }
    else
    {
        CsrBtHdpResumeResSend(CSR_BT_PHDC_MGR_IFACEQUEUE,pInd->chId,FALSE,CSR_BT_PHDC_MGR_INVALID_VAL);
    }

    return FSM_EVENT_NULL;
}

/* Device states */
/* Disconnected */
FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_DisconnectedSHdpAcceptConnectAssociateChannelInd)
{
    PhdcMgrDeviceListEntry *pDevice;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    CsrBtHdpAcceptConnectAssociateChannelInd *pInd;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pInd =  (CsrBtHdpAcceptConnectAssociateChannelInd *)pInstance->pRecvMsg; 
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
    
    if (!pDevice->mdepId || pDevice->mdepId == pInd->mdepId ) 
    {
        pDevice->bdAddr = pInd->deviceAddr;
        pDevice->dataChannelId = pInd->assocChannelId;
        pDevice->mdepId = pInd->mdepId;

        CsrBtPhdcMgrSendAcceptBtLinkIndSend(pInstance->qId, pDevice->bdAddr);
    }
    else
    {
        /* @todo implies
            i)  a MD_CREATE_MDL_REQ for the same MDL or
            ii) if we have advertised 2 service records it
                  for the a  control PSM  or
            iii) received for a different MDL or
            iv) received in error

            For the timebeing send an error*/
        CsrBtHdpAcceptConnectAssociateChannelResSend(CSR_BT_PHDC_MGR_IFACEQUEUE,
                                                     CSR_BT_RESULT_CODE_HDP_UNSPECIFIED_ERROR,
                                                     CSR_BT_PHDC_MGR_INVALID_VAL,
                                                     pInd->assocChannelId);

    }
    
    return FSM_EVENT_NULL;
}

FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_DisconnectedSPhdcAcceptBtLinkRes)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    CsrBtPhdcMgrAcceptBtLinkRes *pRes;
    CsrUint8 response;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pRes = (CsrBtPhdcMgrAcceptBtLinkRes *) pInstance->pRecvMsg;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
    
    response = ( pRes->accept == TRUE  ) ?  CSR_BT_RESULT_CODE_HDP_SUCCESS : 
                                            CSR_BT_RESULT_CODE_HDP_UNSPECIFIED_ERROR;
    
    CsrBtHdpAcceptConnectAssociateChannelResSend(CSR_BT_PHDC_MGR_IFACEQUEUE, response, pRes->maxPacketLength, pDevice->dataChannelId);
        
    if (pRes->accept == FALSE )
    {
        csrBtPhdcMgrFreeDeviceMds(pInstance, pDevice);
    }
    
    return returnEvent;    
}

FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_DisconnectedSHdpConnectAssociateChannelInd)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    CsrBtHdpConnectAssociateChannelInd *pInd;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pInd =  (CsrBtHdpConnectAssociateChannelInd *)pInstance->pRecvMsg;    
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
                
    if (pInd->resultSupplier == CSR_BT_SUPPLIER_HDP &&
        pInd->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS)
    {
        pDevice->maxPacketLength = pInd->maxPacketLength;
        pDevice->dataChannelId = pInd->assocChannelId;
        pDevice->mdepId = pInd->mdepId;

        pDevice->psmIdentifier = pInd->hdpInstanceId;
        pDevice->bdAddr.lap = pInd->deviceAddr.lap;
        pDevice->bdAddr.uap = pInd->deviceAddr.uap;
        pDevice->bdAddr.nap = pInd->deviceAddr.nap;
    }
    else
    {
        csrBtPhdcMgrFreeDeviceMds(pInstance, pDevice);
    }
    return returnEvent;
}


FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_DisconnectedSPhdcSetupBtLinkReq)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    CsrBtPhdcMgrSetupBtLinkReq *pReq;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pReq =  (CsrBtPhdcMgrSetupBtLinkReq *)pInstance->pRecvMsg;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
    
    if (!pDevice->mdepId || pDevice->mdepId == pReq->mdepId )
    {
        pDevice->bdAddr          = pReq->deviceAddr;
        pDevice->mdepId          = pReq->mdepId;
        pDevice->maxPacketLength = pReq->maxPacketLength;
        
        CsrBtHdpConnectAssociateChannelReqSend(CSR_BT_PHDC_MGR_IFACEQUEUE,
                                           pReq->psmIdentifier,
                                           pReq->deviceAddr, 
                                           pReq->mdepId,
                                           pReq->mdepDataType,
                                           CSR_BT_HDP_MDEP_SINK,
                                           pReq->maxPacketLength);       

    }
    else
    {
        /* if the device exists, then the application must do a resume to restart
           the connection with it. If the agent does not support reconnect
           procedure than it dissassociate from the manager and hence
           no instance will be saved */

        CsrBtPhdcMgrSetupBtLinkCfmSend(pInstance->qId,
                                            CSR_BT_PHDC_MGR_INVALID_VAL,
                                            &pReq->deviceAddr,
                                            pReq->mdepId,
                                            pReq->maxPacketLength,
                                            CSR_BT_PHDC_MGR_INVALID_VAL,
                                            CSR_BT_PHDC_MGR_RESULT_CODE_INVALID_OP,
                                            CSR_BT_SUPPLIER_PHDC);
    }
    
    return returnEvent;
}



FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_DisconnectingSHdpDisconnectAssociateChannelCfm)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    CsrBtHdpDisconnectAssociateChannelCfm *pCfm;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pCfm =  (CsrBtHdpDisconnectAssociateChannelCfm *)pInstance->pRecvMsg;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
          
    if ( pCfm->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS )
    {
        pCfm->resultCode = CSR_BT_PHDC_MGR_RESULT_CODE_SUCCESS;
        pCfm->resultSupplier = CSR_BT_SUPPLIER_PHDC;
    }

    if (pDevice->isDisUserIntiated)
    {
        pDevice->isDisUserIntiated = FALSE;
        
        CsrBtPhdcMgrDisassociateCfmSend(pInstance->qId, 
                                    pDevice->dataChannelId, 
                                    &pDevice->bdAddr, 
                                    pCfm->resultCode, 
                                    pCfm->resultSupplier);
    }
    else
    {
        CsrBtPhdcMgrBtLinkRelIndSend(pInstance->qId, pDevice->dataChannelId);        
    }

    csrBtPhdcMgrFreeDeviceMds(pInstance, pDevice);

    return returnEvent;
}

/* Connecting */
FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_ConnectingSHdpConnectAssociateChannelCfm)
{
    PhdcMgrDeviceListEntry *pDevice;
    CsrUint32 psmId;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    CsrBtHdpConnectAssociateChannelCfm *pCfm;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pCfm =  (CsrBtHdpConnectAssociateChannelCfm *)pInstance->pRecvMsg;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
    psmId = pDevice->psmIdentifier;
    
    if (pCfm->resultSupplier == CSR_BT_SUPPLIER_HDP &&
        pCfm->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS)
    {
        pDevice->maxPacketLength    = pCfm->maxPacketLength;
        pDevice->dataChannelId      = pCfm->assocChannelId;
        pDevice->mdepId             = pCfm->mdepId;

        pCfm->resultCode            = CSR_BT_PHDC_MGR_RESULT_CODE_SUCCESS;
        pCfm->resultSupplier        = CSR_BT_SUPPLIER_PHDC;

        csrBtPhdcMgrSetTimer(param, &pDevice, PHDC_MGR_TIMEOUT_INACTIVTY, PHDC_MGR_DEVICE_EV_INT_TO_INACTIVITY); 
    }
    else
    {
        csrBtPhdcMgrFreeDeviceMds(pInstance, pDevice);
    }

    CsrBtPhdcMgrSetupBtLinkCfmSend(pInstance->qId, 
                                    psmId, 
                                    &pCfm->deviceAddr, 
                                    pCfm->mdepId, 
                                    pCfm->maxPacketLength, 
                                    pCfm->assocChannelId, 
                                    pCfm->resultCode, 
                                    pCfm->resultSupplier);

    return returnEvent;
}

/* Unassociated */
FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_UnassociatedSIntTimedOutInactivity)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;

    CsrBtHdpDisconnectAssociateChannelReqSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId);

    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_UnassociatedSHdpDataInd)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    CsrBtHdpDataInd *pInd;
    APDU *pApdu = NULL;
    Associate_result result;
    CsrUint16 choice;
    CsrBool removeSavedApdu = FALSE;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pInd =  (CsrBtHdpDataInd *)pInstance->pRecvMsg;    
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
    
    csrBtPhdcMgrCancelTimer(pDevice);

    CsrBtPhdcMderDecodeApdu(pInd->data, &pApdu);
    /* if aare or rlrq is received send a abort 
    if rlre or abrt is received ignore the event */
    if ( pApdu->choice == AARQ_CHOSEN ) 
    {
        pDevice->pMds->pSavedApdu = pApdu;
        result = CsrBtPhdcMgrIsAssociationComptabile(&pApdu->u.aarq, pDevice->pMds);

        if ( result == ACCEPTED || 
            result == ACCEPTED_UNKNOWN_CONFIG )
        {
            pDevice->aarqOngoing = AARQ_ONGOING_STARTED;
            pDevice->pMds->configStatus = ACCEPTED_UNKNOWN_CONFIG;

            /* Sent indication to app - wait for response */
            CsrBtPhdcMgrAcceptAssociateIndSend(pInstance->qId, 
                                                pDevice->pMds->devConfigId, 
                                                pDevice->pMds->systemId, 
                                                &pDevice->bdAddr);
        }
        else
        {
            /* the agent may not disconnect the BT link as it would
            try a few more configurations. However, start a timer
            to sense inactivity for a long time */                    
            CsrBtPhdc20601AareEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId, pDevice->pMds->pDataProto, result);
            
            csrBtPhdcMgrSetTimer(param, &pDevice, PHDC_MGR_TIMEOUT_INACTIVTY, PHDC_MGR_DEVICE_EV_INT_TO_INACTIVITY); 

            CsrBtPhdcMgrFreeDecodeAnyApdu(pApdu,&choice);
            removeSavedApdu = TRUE;
            returnEvent = PHDC_MGR_DEVICE_EV_INT_AWAITING_CFM;
        }
    }
    else 
    {
        pDevice->aarqOngoing = AARQ_ONGOING_FAILED;
        returnEvent = csrBtPhdcMgrErrorHandler(pInstance, pDevice, pApdu, CSR_BT_PHDC_MGR_DEVICE_ST_UNASSOCIATED);
        removeSavedApdu = TRUE;
    }

    CsrPmemFree(pInd->data);
    if ((pDevice->pMds) && (removeSavedApdu))
    {
        if (pDevice->pMds->pSavedApdu == pApdu)
        {
            pDevice->pMds->pSavedApdu = NULL;
        }
    }
    
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_UnassociatedSPhdcAcceptAssociateRes)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    CsrBtPhdcMgrAcceptAssociateRes *pRes;
    Associate_result result;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pRes =  (CsrBtPhdcMgrAcceptAssociateRes *)pInstance->pRecvMsg;    
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
    
    if (pRes->accept == TRUE )
    {
        PhdAssociationInformation *pAssocInfo = NULL;
        pAssocInfo = (PhdAssociationInformation *)(pDevice->pMds->pDataProto->data_proto_info.value);
        result = pDevice->pMds->configStatus;

        CsrMemCpy(pAssocInfo->systemId.value, 
                &pRes->systemId[0], 
                pAssocInfo->systemId.length);                                                        
    }
    else
    {
        result = REJECTED_UNAUTHORIZED;
        returnEvent = PHDC_MGR_DEVICE_EV_INT_AWAITING_CFM;
    }
    
    CsrBtPhdc20601AareEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId, pDevice->pMds->pDataProto, result);

    CsrBtPhdcMgrFreeDecodeAarq(pDevice->pMds->pSavedApdu);
    pDevice->pMds->pSavedApdu = NULL;

    if ( result == ACCEPTED )
    {
        pDevice->aarqOngoing = AARQ_ONGOING_FALSE;
        CsrBtPhdcMgrSendAssociateCompleteIndSend(pInstance->qId, 
                                            CSR_BT_PHDC_MGR_INVALID_VAL/*CsrUint32 psmIdentifier*/, 
                                            &pDevice->bdAddr, 
                                            pDevice->mdepId, 
                                            CSR_BT_PHDC_MGR_INVALID_VAL/*CsrUint16 mdepDataType*/, 
                                            pDevice->maxPacketLength,
                                            pDevice->dataChannelId, 
                                            result, 
                                            CSR_BT_SUPPLIER_PHDC);

        returnEvent = PHDC_MGR_DEVICE_EV_INT_KNOWN_CONFIG;
    }
        
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_UnassociatedSHdpDataCfm)
{
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    PhdcMgrDeviceListEntry *pDevice;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
          
    csrBtPhdcMgrSetTimer(param, &pDevice, PHDC_MGR_TIMEOUT_CONFIGURATION, PHDC_MGR_DEVICE_EV_INT_TO_CONFIG); 

    return returnEvent;

}


FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_UnassociatedSHdpDisconnectAssociateChannelInd)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    CsrBtHdpDisconnectAssociateChannelInd *pInd;
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pInd =  (CsrBtHdpDisconnectAssociateChannelInd *)pInstance->pRecvMsg;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
    
    csrBtPhdcMgrCancelTimer(pDevice);
    
    if ( pInd->resultSupplier == CSR_BT_SUPPLIER_HDP &&
        pInd->resultCode     == CSR_BT_RESULT_CODE_HDP_SUCCESS )
    {
        pInd->resultSupplier = CSR_BT_SUPPLIER_PHDC;
        pInd->resultCode     = CSR_BT_PHDC_MGR_RESULT_CODE_SUCCESS;
    }

    if (pDevice->isDisUserIntiated == TRUE)
    {
        pDevice->isDisUserIntiated = FALSE;
        
        CsrBtPhdcMgrDisassociateCfmSend(pInstance->qId,
                                        pDevice->dataChannelId, 
                                        &pDevice->bdAddr, 
                                        pInd->resultCode, 
                                        pInd->resultSupplier);

        CsrBtPhdcMgrBtLinkRelIndSend(pInstance->qId, pDevice->dataChannelId);    
    }
    else
    {
        
        if(pDevice->aarqOngoing == AARQ_ONGOING_STARTED)
        {
            pDevice->aarqOngoing = AARQ_ONGOING_FALSE;
            CsrBtPhdcMgrSendAssociateCompleteIndSend(pInstance->qId, 
                                                     pDevice->psmIdentifier, 
                                                     &pDevice->bdAddr, 
                                                     pDevice->mdepId, 
                                                     CSR_BT_PHDC_MGR_INVALID_VAL, 
                                                     pDevice->maxPacketLength,
                                                     pDevice->dataChannelId, 
                                                     CSR_BT_PHDC_MGR_RESULT_CODE_UNKNOWN, 
                                                     CSR_BT_SUPPLIER_PHDC);
        }
        /* BtLink released */
        CsrBtPhdcMgrBtLinkRelIndSend(pInstance->qId, pDevice->dataChannelId);

    }

    return returnEvent;
}



/* ConfigWaiting */
FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_ConfigWaitingSIntTimedOutConfig)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;

    CsrBtPhdc20601AbrtEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId, ABORT_REASON_CONFIGURATION_TIMEOUT);
    pDevice->reason = mapAbrtReason[ABORT_REASON_CONFIGURATION_TIMEOUT];

    return returnEvent;
}


FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_ConfigWaitingSHdpDataInd)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    CsrBtHdpDataInd *pInd;
    APDU *pApdu;
    CsrUint8 i;
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pInd =  (CsrBtHdpDataInd *)pInstance->pRecvMsg;   
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
            
    CsrBtPhdcMderDecodeApdu(pInd->data, &pApdu);

    if ( CsrBtPhdcMgrIsMDC_NOTI_CONFIG(pApdu) && CsrBtPhdcMgrUpdateDevConfig(pApdu,pDevice->pMds))
    {
        ConfigResult accepted; 
        CsrUint16 dataLen, noObjects;
        CsrUint8 *data, *msgPointer = pInd->data;
    
        csrBtPhdcMgrCancelTimer(pDevice);
        
        CsrBtPhdcMgrUpdateInvokeIdAck(pApdu,pDevice);

        accepted = CsrBtPhdcMgrIsSupportedStandardConfig(pDevice->pMds->devConfigId);

        /* Find ConfigObjects and send to app */
        noObjects = CsrBtPhdcMgrGetNoOfConfigObj(pApdu);
        
        if(accepted != ACCEPTED_CONFIG)
        {
            /* Are all ConfigObject classes supported? */
            for ( i = 0 ; i < noObjects ; i++ )
            {
                ConfigObject *pObj = CsrBtPhdcMgrGetConfigObject(pApdu,i);
                if( pObj->obj_class != MDC_MOC_VMO_METRIC_NU)
                {
                    accepted = UNSUPPORTED_CONFIG;
                    break;
                }
                else
                {
                    accepted = ACCEPTED_CONFIG;
                }
            }
        }

        if( accepted == ACCEPTED_CONFIG )
        {
            msgPointer += PHDC_MGR_DATA_CONFIG_OBJECT_REPORT_OFFSET;
            accepted = UNSUPPORTED_CONFIG;

            /* Send config information to app */
            for ( i = 0 ; i < noObjects ; i++ )
            {
                ConfigObject *pObj;
                PhdcMgrConfigObjectEntry *pObjEntry;
                                
                pObj = CsrBtPhdcMgrGetConfigObject(pApdu,i);
        
                dataLen = pObj->attributes.length + PHDC_MGR_DATA_CONFIG_OBJECT_REPORT_HEADER; 

                pObjEntry = PHDC_MGR_OBJECT_LIST_ADD_OBJECT(pDevice->pMds->configObjList);

                pObjEntry->objHandle = pObj->obj_handle;
                pObjEntry->unitCode  = CsrBtPhdcMgrGetUnitCode(pObj);
                pObjEntry->objType = CsrBtPhdcMgrGetObjectType(pObj);
                pObjEntry->pAttrValMap = CsrBtPhdcMgrGetAttrValMap(pObj);/*allocates memory*/
                
                /* Known ObjectType? */
                if (pObjEntry->objType == ( PHDC_MGR_MOVE_LF_16(MDC_PART_SCADA) | MDC_PRESS_BLD_NONINV ) )
                {
                    pObjEntry->pMetricIdList = CsrBtPhdcMgrGetMetricIdList(pObj);/*allocates memory*/
                    accepted = ACCEPTED_CONFIG;
                }
                else if (pObjEntry->objType == (PHDC_MGR_MOVE_LF_16(MDC_PART_SCADA) | MDC_MASS_BODY_ACTUAL) ||
                         pObjEntry->objType == (PHDC_MGR_MOVE_LF_16(MDC_PART_SCADA) | MDC_LEN_BODY_ACTUAL) ||
                         pObjEntry->objType == (PHDC_MGR_MOVE_LF_16(MDC_PART_SCADA) | MDC_RATIO_MASS_BODY_LEN_SQ) ||
                         pObjEntry->objType == (PHDC_MGR_MOVE_LF_16(MDC_PART_SCADA) | MDC_PULS_RATE_NON_INV) )
                {
                    accepted = ACCEPTED_CONFIG;
                }
            
                /* Send ConfigObject to app */
                data = CsrPmemZalloc(dataLen);
                CsrMemCpy(data, msgPointer, dataLen);

                CsrBtPhdcMgrExtConfigObjIndSend(pInstance->qId, 
                                                pDevice->pMds->devConfigId,
                                                &pDevice->bdAddr, 
                                                pObj->obj_handle, 
                                                dataLen,
                                                data,
                                                CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_OPCODE_NEW);
                    
                msgPointer += dataLen;
            }
        }

        /* Now send response to agent */
        if(accepted == UNSUPPORTED_CONFIG || accepted == STANDARD_CONFIG_UNKNOWN)
        {                    
            CsrBtPhdc20601ConfigRspSend(CSR_BT_PHDC_MGR_IFACEQUEUE, 
                                        pDevice->dataChannelId, 
                                        pDevice->invokeIdAck,
                                        pDevice->pMds->devConfigId, 
                                        UNSUPPORTED_CONFIG);

            returnEvent = PHDC_MGR_DEVICE_EV_INT_UNKNOWN_CONFIG;
        }
        else /*if(accepted == ACCEPTED_CONFIG)*/
        {
            /* Success */
            CsrBtPhdc20601GetMdsSend(CSR_BT_PHDC_MGR_IFACEQUEUE, 
                                    pDevice->dataChannelId,
                                    CsrBtPhdcMgrGetInvokeIdTx(pDevice));                    
        }
        
        
    }
    else
    {
        if( pApdu->choice == RLRQ_CHOSEN && pApdu->u.rlrq.reason == RELEASE_REQUEST_REASON_NO_MORE_CONFIG)
        {
            pDevice->aarqOngoing = AARQ_ONGOING_FALSE;

            CsrBtPhdcMgrSendAssociateCompleteIndSend(pInstance->qId, 
                                                    pDevice->psmIdentifier, 
                                                    &pDevice->bdAddr, 
                                                    pDevice->mdepId, 
                                                    CSR_BT_PHDC_MGR_INVALID_VAL, 
                                                    pDevice->maxPacketLength,
                                                    pDevice->dataChannelId, 
                                                    CSR_BT_PHDC_MGR_RESULT_CODE_CONFIG_UNSUPPORTED_CONFIG, 
                                                    CSR_BT_SUPPLIER_PHDC);
        }
        returnEvent = csrBtPhdcMgrErrorHandler(pInstance, pDevice, pApdu, CSR_BT_PHDC_MGR_DEVICE_ST_CONFIG_WAITING);
        pApdu = NULL;
    }

    if (pApdu)
    {
        CsrBtPhdcMgrFreeDecodePrstNotifyConfig(pApdu);
    }
    
    CsrPmemFree(pInd->data);
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_ConfigWaitingSHdpDataCfm)
{
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    PhdcMgrDeviceListEntry *pDevice;
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
          
    if (pDevice != NULL )
    {
        csrBtPhdcMgrSetTimer(param, &pDevice, PHDC_MGR_TIMEOUT_GET, PHDC_MGR_DEVICE_EV_INT_TO_GET); 
    }
    return returnEvent;
}

/* ConfigChecking */
FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_ConfigCheckingSIntTimedOutGet)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;

    CsrBtPhdc20601AbrtEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId, ABORT_REASON_CONFIGURATION_TIMEOUT);
    pDevice->reason = mapAbrtReason[ABORT_REASON_CONFIGURATION_TIMEOUT];

    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_ConfigCheckingSHdpDataInd)
{
    PhdcMgrDeviceListEntry *pDevice;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    CsrBtHdpDataInd *pInd;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    APDU *pApdu;
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pInd =  (CsrBtHdpDataInd *)pInstance->pRecvMsg;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
    
    CsrBtPhdcMderDecodeApdu(pInd->data, &pApdu);

    if (CsrBtPhdcMgrIsGetRsp(pApdu))
    {
        RegCertDataList *pRegCertDataList = NULL;
        /* i)  do a regulatory check 
        ii) store mds attributes 
        iii) send config response */
        
        csrBtPhdcMgrCancelTimer(pDevice);

        CsrBtPhdcMgrUpdateInvokeIdRcv(pDevice);

        /* Check for Certification List */
        if ( CsrBtPhdcMgrIsGetRegulatoryParam(pApdu,&pRegCertDataList) && CsrBtPhdcMgrIsContinua(pRegCertDataList) )
        {
            /* perform  device specific checks */
            CsrUint8 i = 0;
            TypeVer  *pDevSpec ;
            PhdcMgrDeviceDataTypeEntry *pDataTypeEntry = NULL;
            CsrBool bCompatible = FALSE,bSupported = FALSE;
            ContinuaBodyStruct *pContinuaInfo = NULL;

            /* check to se if Set Time or Sync is requested */
/* TO DO new function to get Set tim bit in attribute list...

            CsrBtPhdcMgrGetSetTimeInMDC_ATTR_MDS_TIME_INFO()

*/
            pContinuaInfo = CsrBtPhdcMgrGetContinuaStruct(pRegCertDataList);
            
            if(CsrBtPhdcMgrGetMDC_ATTR_SYS_TYPE_SPEC_LIST(pApdu,&pDevice->pMds->pSpec))
            {
                pDevSpec = pDevice->pMds->pSpec->value;

                for  ( i=0 ; (i < pDevice->pMds->pSpec->count) ; i++,pDevSpec++  )
                {
                    pDataTypeEntry = PHDC_MGR_TYPE_SPEC_FIND_FROM_TYPE_SPEC(pInstance->typeSpec, &pDevSpec->type);

                    if ( pDataTypeEntry != NULL &&
                        pContinuaInfo  != NULL)
                    {
                        bSupported = CsrBtPhdcMgrIsSupportedDataType(pDataTypeEntry->dataType, pDevice);
                        bCompatible = CsrBtPhdcMgrIsContinuaCertifiedCompatible(pDataTypeEntry,pContinuaInfo);
                        break;
                    }
                }
            }            
            if (CsrBtPhdcMgrCanTrasmit(pDevice))
            {
                if (bSupported == FALSE && bCompatible == TRUE)
                {
                    CsrUint16 configResult,userConfigResult;
                    configResult = CsrBtPhdcMgrIsSupportedStandardConfig(pDevice->pMds->devConfigId);

                    if ((configResult == STANDARD_CONFIG_UNKNOWN) && (configResult == ACCEPTED_CONFIG))
                    {
                        userConfigResult = CSR_BT_PHDC_MGR_RESULT_CODE_CONFIG_STANDARD_CONFIG_UNKNOWN;
                    }
                    else /*configResult = UNSUPPORTED_CONFIG */
                    {
                        userConfigResult = CSR_BT_PHDC_MGR_RESULT_CODE_CONFIG_UNSUPPORTED_CONFIG;
                    }

                    CsrBtPhdc20601ConfigRspSend(CSR_BT_PHDC_MGR_IFACEQUEUE, 
                                                pDevice->dataChannelId, 
                                                pDevice->invokeIdAck,
                                                pDevice->pMds->devConfigId, 
                                                configResult);
                    pDevice->aarqOngoing = AARQ_ONGOING_FALSE;
                    CsrBtPhdcMgrSendAssociateCompleteIndSend(pInstance->qId, 
                                                             pDevice->psmIdentifier, 
                                                             &pDevice->bdAddr, 
                                                             pDevice->mdepId, 
                                                             CSR_BT_PHDC_MGR_INVALID_VAL/*CsrUint16 mdepDataType*/, 
                                                             pDevice->maxPacketLength,
                                                             pDevice->dataChannelId, 
                                                             userConfigResult, 
                                                             CSR_BT_SUPPLIER_PHDC);
                }                        
                else if (pDataTypeEntry != NULL  && bCompatible == TRUE )
                {
                    /* Success */
                    CsrBtPhdc20601ConfigRspSend(CSR_BT_PHDC_MGR_IFACEQUEUE, 
                                                pDevice->dataChannelId, 
                                                pDevice->invokeIdAck,
                                                pDevice->pMds->devConfigId, 
                                                ACCEPTED_CONFIG);
                }
                else
                {
                    CsrBtPhdc20601RlrqEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId, RELEASE_REQUEST_REASON_NO_MORE_CONFIG);
                    returnEvent = PHDC_MGR_DEVICE_EV_INT_RELEASE;
                    pDevice->reason = mapRlReason[RELEASE_REQUEST_REASON_NO_MORE_CONFIG];
                    csrBtPhdcMgrSetTimer(param, &pDevice, PHDC_MGR_TIMEOUT_ASSOCIATION_RELEASE, PHDC_MGR_DEVICE_EV_INT_TO_RLRQ); 
                }
            }
            else
            {
                CSR_BT_PHDC_M_ASSERT(FALSE);
            }
        }
        else
        {    /* Is not a Contina device. */
            if (CsrBtPhdcMgrCanTrasmit(pDevice) )
            {
                CsrBtPhdc20601RlrqEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId, RELEASE_REQUEST_REASON_NO_MORE_CONFIG);
                returnEvent = PHDC_MGR_DEVICE_EV_INT_RELEASE;
                pDevice->reason = mapRlReason[RELEASE_REQUEST_REASON_NO_MORE_CONFIG];
                csrBtPhdcMgrSetTimer(param, &pDevice, PHDC_MGR_TIMEOUT_ASSOCIATION_RELEASE, PHDC_MGR_DEVICE_EV_INT_TO_RLRQ); 
            }
            else
            {
                CSR_BT_PHDC_M_ASSERT(FALSE);
            }

        }
        CsrBtPhdcMgrFreeRegCertDataList(pRegCertDataList);
    }
    else
    {
        returnEvent = csrBtPhdcMgrErrorHandler(pInstance, pDevice, pApdu, CSR_BT_PHDC_MGR_DEVICE_ST_CONFIG_CHECKING);
        pApdu = NULL;
    }
    if (pApdu)
    {
        CsrBtPhdcMgrFreeDecodePrstGetMdsRsp(pApdu);
        pApdu = NULL;
    }
    CsrPmemFree(pInd->data);
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_ConfigCheckingSHdpDataCfm)
{
    CsrBtPhdcFsmParam *param;
    PhdcMgrDeviceListEntry *pDevice;
    CsrBtPhdcMgrInstance *pInstance;    
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;    
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
    
    pDevice->aarqOngoing = AARQ_ONGOING_FALSE;
    CsrBtPhdcMgrSendAssociateCompleteIndSend(pInstance->qId, 
                                             pDevice->psmIdentifier, 
                                             &pDevice->bdAddr, 
                                             pDevice->mdepId, 
                                             CSR_BT_PHDC_MGR_INVALID_VAL, /*CsrUint16 mdepDataType*/
                                             pDevice->maxPacketLength,
                                             pDevice->dataChannelId, 
                                             CSR_BT_PHDC_MGR_RESULT_CODE_SUCCESS, 
                                             CSR_BT_SUPPLIER_PHDC);            

    

    return FSM_EVENT_NULL;
}

/* Operating */
FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_OperatingSHdpDataInd)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    CsrBtHdpDataInd *pInd;
    APDU *pApdu;
    OID_Type eventType;
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pInd =  (CsrBtHdpDataInd *)pInstance->pRecvMsg;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
          
    CsrBtPhdcMderDecodeApdu(pInd->data, &pApdu);

    eventType = CsrBtPhdcMgrIsData(pApdu);
    
    if (eventType)
    {
        CsrBool confirmed = CsrBtPhdcMgrIsConfirmed(pApdu);
        
        CsrBtPhdcMgrUpdateInvokeIdAck(pApdu,pDevice);
                
        if ( eventType == MDC_NOTI_SCAN_REPORT_FIXED || 
             eventType == MDC_NOTI_SCAN_REPORT_MP_FIXED)
        {
            CsrBtPhdcMgrHandleObsScanFixed(pInstance->qId,pDevice,pApdu);
            CsrBtPhdcMgrFreeDecodePrstObsScanFixed(pApdu);
        }
        else /*if ( eventType == MDC_NOTI_SCAN_REPORT_VAR ||
                  eventType == MDC_NOTI_SCAN_REPORT_MP_VAR ) */
        {
            if (CsrBtPhdcMgrHandleObsScanVar(pInstance->qId,pDevice,pApdu) == FALSE)
            {
                /* vendor specific */
                CsrUint16 objHandle = 0;
                CsrUint16 length = 0, offset = 0;
                CsrUint16 count, i;
                CsrUint8 *pAttributesArray = NULL;
                CsrUint16 personId;

                CsrBtPhdcScanReportType scanReportType = ( eventType == MDC_NOTI_SCAN_REPORT_VAR ) ? 
                                                            CSR_BT_PHDC_MGR_SCAN_REPORT_TYPE_VAR:
                                                            CSR_BT_PHDC_MGR_SCAN_REPORT_TYPE_MP_VAR;
                
                if (scanReportType == CSR_BT_PHDC_MGR_SCAN_REPORT_TYPE_VAR)
                {
                    personId = CSR_BT_PHDC_MGR_PERSON_ID_NA;
                    count = pInd->data[PHDC_MGR_DATA_VAR_COUNT_OFFSET];
                    offset = PHDC_MGR_DATA_VAR_FIRST_OFFSET;
                }
                else
                {
                    personId = pInd->data[PHDC_MGR_DATA_VAR_MP_PERSON_ID_OFFSET];
                    count = pInd->data[PHDC_MGR_DATA_VAR_COUNT_OFFSET + PHDC_MGR_DATA_VAR_MP_EXTRA_OFFSET];
                    offset = PHDC_MGR_DATA_VAR_FIRST_OFFSET + PHDC_MGR_DATA_VAR_MP_EXTRA_OFFSET;
                }


                for ( i = 0 ; i < count ; i++)
                {
                    objHandle = pInd->data[offset];
                    
                    length = pInd->data[offset + PHDC_MGR_DATA_REPORT_HEADER_OFFSET] + PHDC_MGR_DATA_REPORT_HEADER_OFFSET;
                    /* Send individual resport seperatly */
                    pAttributesArray = CsrPmemAlloc(length);
                    CsrMemCpy(pAttributesArray, &pInd->data[offset], length);

                    CsrBtPhdcMgrDsIndSend(pInstance->qId, 
                                          pDevice->dataChannelId,
                                          objHandle, 
                                          personId,
                                          scanReportType,
                                          length,
                                          pAttributesArray);
                    offset += length;
                }
                
            }
            CsrBtPhdcMgrFreeDecodePrstObsScanVar(pApdu);
        }
        
        if (CsrBtPhdcMgrCanTrasmit(pDevice))
        {                
            if(confirmed)
            {
                CsrBtPhdc20601DataConfirmedEventSend(CSR_BT_PHDC_MGR_IFACEQUEUE, 
                                                     pDevice->dataChannelId,
                                                     pDevice->invokeIdAck,
                                                     eventType);
            }
        }
        else
        {
            CSR_BT_PHDC_M_ASSERT(FALSE);
        }
    }
    else 
    {
        returnEvent = csrBtPhdcMgrErrorHandler(pInstance, pDevice, pApdu, CSR_BT_PHDC_MGR_DEVICE_ST_OPERATING);
    }        
    /* pApdu has been freed either in CsrBtPhdcMgrFreeDecodePrstObsScanFixed, CsrBtPhdcMgrFreeDecodePrstObsScanVar or csrBtPhdcMgrErrorHandler */
    CsrPmemFree(pInd->data);
    
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_OperatingSHdpDisconnectAssociateChannelInd)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
    
    csrBtPhdcMgrCancelTimer(pDevice);
    CsrBtPhdcMgrBtLinkRelIndSend(pInstance->qId, pDevice->dataChannelId);

    return returnEvent;
}

/* AwaitingCfm */
FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_AwaitingCfmSHdpDataCfm)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
        
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
          
    csrBtPhdcMgrCancelTimer(pDevice);            
    csrBtPhdcMgrSetTimer(param, &pDevice, PHDC_MGR_TIMEOUT_INACTIVTY, PHDC_MGR_DEVICE_EV_INT_TO_INACTIVITY); 

    return returnEvent;
}

/* AbortCfm */
FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_AbortCfmSHdpDataCfm)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
        
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;    
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
            
    csrBtPhdcMgrCancelTimer(pDevice);            
    csrBtPhdcMgrSetTimer(param, &pDevice, PHDC_MGR_TIMEOUT_INACTIVTY, PHDC_MGR_DEVICE_EV_INT_TO_INACTIVITY); 

    if(pDevice->aarqOngoing == AARQ_ONGOING_FALSE)
    {
        if (pDevice->isDisUserIntiated == TRUE)
        {
            pDevice->isDisUserIntiated = FALSE;
        
            CsrBtPhdcMgrDisassociateCfmSend(pInstance->qId,
                                            pDevice->dataChannelId, 
                                            &pDevice->bdAddr, 
                                            pDevice->reason, 
                                            CSR_BT_SUPPLIER_PHDC);               
        }
        else
        {
            CsrBtPhdcMgrDisassociateIndSend(pInstance->qId, 
                                            pDevice->dataChannelId, 
                                            &pDevice->bdAddr, 
                                            pDevice->reason, 
                                            CSR_BT_SUPPLIER_PHDC);
        }
    }
    else if(pDevice->aarqOngoing == AARQ_ONGOING_STARTED)
    {
        pDevice->aarqOngoing = AARQ_ONGOING_FALSE;
        CsrBtPhdcMgrSendAssociateCompleteIndSend(pInstance->qId, 
                                                 CSR_BT_PHDC_MGR_INVALID_VAL, 
                                                 &pDevice->bdAddr, 
                                                 pDevice->mdepId, 
                                                 CSR_BT_PHDC_MGR_INVALID_VAL, 
                                                 pDevice->maxPacketLength,
                                                 pDevice->dataChannelId, 
                                                 pDevice->reason, 
                                                 CSR_BT_SUPPLIER_PHDC);        
    }
    else if(pDevice->aarqOngoing == AARQ_ONGOING_FAILED)
    {
        /* Don't send reponse */
        pDevice->aarqOngoing = AARQ_ONGOING_FALSE;
        
    }


    return returnEvent;
}

/* RleaseCfm */
FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_ReleaseCfmSHdpDataCfm)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
        
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;    
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
          
    csrBtPhdcMgrCancelTimer(pDevice);            
    csrBtPhdcMgrSetTimer(param, &pDevice, PHDC_MGR_TIMEOUT_INACTIVTY, PHDC_MGR_DEVICE_EV_INT_TO_INACTIVITY); 

    if(pDevice->aarqOngoing == AARQ_ONGOING_STARTED)
    {
        pDevice->aarqOngoing = AARQ_ONGOING_FALSE;
        CsrBtPhdcMgrSendAssociateCompleteIndSend(pInstance->qId, 
                                                 CSR_BT_PHDC_MGR_INVALID_VAL, 
                                                 &pDevice->bdAddr, 
                                                 pDevice->mdepId, 
                                                 CSR_BT_PHDC_MGR_INVALID_VAL, 
                                                 pDevice->maxPacketLength,
                                                 pDevice->dataChannelId, 
                                                 pDevice->reason, 
                                                 CSR_BT_SUPPLIER_PHDC);     
    }
    else if(pDevice->aarqOngoing == AARQ_ONGOING_FAILED)
    {
        /* Don't send reponse */
        pDevice->aarqOngoing = AARQ_ONGOING_FALSE;
    }
    else if(pDevice->aarqOngoing == AARQ_ONGOING_FALSE)
    {
        CsrBtPhdcMgrDisassociateIndSend(pInstance->qId, 
                                        pDevice->dataChannelId, 
                                        &pDevice->bdAddr, 
                                        pDevice->reason, 
                                        CSR_BT_SUPPLIER_PHDC);
    }
    return returnEvent;
}

/* Disassociating */
FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_DisassociatingSIntTimedOutInactivity)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;

    CsrBtPhdc20601AbrtEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId, ABORT_REASON_RESPONSE_TIMEOUT);                
    pDevice->reason = mapAbrtReason[ABORT_REASON_RESPONSE_TIMEOUT];

    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_DisassociatingSHdpDataInd)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    CsrBtHdpDataInd *pInd;
    APDU *pApdu;
    CsrUint16 choice;
    CsrBool removeSavedApdu = FALSE;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pInd =  (CsrBtHdpDataInd *)pInstance->pRecvMsg;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
    
    CsrBtPhdcMderDecodeApdu(pInd->data, &pApdu);

    if (pApdu)
    {
        if (pApdu->choice == RLRE_CHOSEN)
        {
            csrBtPhdcMgrCancelTimer(pDevice);
            csrBtPhdcMgrSetTimer(param, &pDevice, PHDC_MGR_TIMEOUT_INACTIVTY, PHDC_MGR_DEVICE_EV_INT_TO_INACTIVITY); 
            returnEvent = PHDC_MGR_DEVICE_EV_INT_RELEASE_CFM;
        }
        else if (pApdu->choice == RLRQ_CHOSEN)
        {
            /* collision scenario - wait for RLE */
            CsrBtPhdc20601RlreEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId,RELEASE_REQUEST_REASON_NORMAL );
            pDevice->reason = mapRlReason[RELEASE_REQUEST_REASON_NORMAL];
            returnEvent = PHDC_MGR_DEVICE_EV_INT_RELEASE_IND;         
        }
        else
        {
            returnEvent = csrBtPhdcMgrErrorHandler(pInstance, pDevice, pApdu, CSR_BT_PHDC_MGR_DEVICE_ST_DISASSOCIATING);
            removeSavedApdu = TRUE;
        }

        if (!removeSavedApdu)
        {
            CsrBtPhdcMgrFreeDecodeAnyApdu(pApdu,&choice);
        }
    }

    if ((pDevice->pMds) && (removeSavedApdu))
    {
        if (pDevice->pMds->pSavedApdu == pApdu)
        {
            pDevice->pMds->pSavedApdu = NULL;
        }
    }
    CsrPmemFree(pInd->data);
    
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_DisassociatingSHdpDisconnectAssociateChannelInd)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    CsrBtHdpDisconnectAssociateChannelInd *pInd;
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pInd =  (CsrBtHdpDisconnectAssociateChannelInd *)pInstance->pRecvMsg;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
    
    csrBtPhdcMgrCancelTimer(pDevice);
    
    if ( pInd->resultSupplier == CSR_BT_SUPPLIER_HDP &&
        pInd->resultCode     == CSR_BT_RESULT_CODE_HDP_SUCCESS )
    {
        pInd->resultSupplier = CSR_BT_SUPPLIER_PHDC;
        pInd->resultCode     = CSR_BT_PHDC_MGR_RESULT_CODE_SUCCESS;
    }

    if (pDevice->isDisUserIntiated == TRUE)
    {
        pDevice->isDisUserIntiated = FALSE;
        
        CsrBtPhdcMgrDisassociateCfmSend(pInstance->qId,
                                    pDevice->dataChannelId, 
                                    &pDevice->bdAddr, 
                                    pInd->resultCode, 
                                    pInd->resultSupplier);               
    }
    
    CsrBtPhdcMgrBtLinkRelIndSend(pInstance->qId, pDevice->dataChannelId);    

    return returnEvent;
}


/* Any other state */
FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_AnyStatePhdcDisassociateReq)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    CsrBtPhdcMgrDisassociateReq *pReq;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pReq =  (CsrBtPhdcMgrDisassociateReq *)pInstance->pRecvMsg;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
          
    pDevice->isDisUserIntiated = TRUE;

    switch (pReq->rlrqReason)
    {
        case CSR_BT_PHDC_MGR_RLRQ_REASON_CONFIGURATION_CHANGED : 
        {
            pReq->rlrqReason = 2;
            break;
        }        
        case CSR_BT_PHDC_MGR_RLRQ_REASON_NO_MORE_CONFIGURATIONS : 
        {
            pReq->rlrqReason = 1;
            break;
        }
        case CSR_BT_PHDC_MGR_RLRQ_REASON_NORMAL : 
        default :
        {
            pReq->rlrqReason = 0;
            break;
        }
    }

    CsrBtPhdc20601RlrqEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId, pReq->rlrqReason);

    /* start the timer for Release response */
    csrBtPhdcMgrSetTimer(param, &pDevice, PHDC_MGR_TIMEOUT_ASSOCIATION_RELEASE, PHDC_MGR_DEVICE_EV_INT_TO_RLRQ); 

    return returnEvent;
}

                                
FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_DisconnectedSHdpDisconnectAssociateChannelInd)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;
    CsrBtPhdcMgrInstance *pInstance;
    CsrBtHdpDisconnectAssociateChannelInd *pInd;
    
    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pInstance = (CsrBtPhdcMgrInstance *)param->pInstance;
    pInd =  (CsrBtHdpDisconnectAssociateChannelInd *)pInstance->pRecvMsg;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;
    
    csrBtPhdcMgrCancelTimer(pDevice);
    
    if ( pInd->resultSupplier == CSR_BT_SUPPLIER_HDP &&
        pInd->resultCode     == CSR_BT_RESULT_CODE_HDP_SUCCESS )
    {
        pInd->resultSupplier = CSR_BT_SUPPLIER_PHDC;
        pInd->resultCode     = CSR_BT_PHDC_MGR_RESULT_CODE_SUCCESS;
    }

    if (pDevice->isDisUserIntiated == TRUE)
    {
        pDevice->isDisUserIntiated = FALSE;
        
        CsrBtPhdcMgrDisassociateCfmSend(pInstance->qId,
                                    pDevice->dataChannelId, 
                                    &pDevice->bdAddr, 
                                    pInd->resultCode, 
                                    pInd->resultSupplier);               
    }
    else
    {
        CsrBtPhdcMgrDisassociateIndSend(pInstance->qId, 
                                        pDevice->dataChannelId,
                                        &pDevice->bdAddr, 
                                        pInd->resultCode, 
                                        pInd->resultSupplier);

        
    }
    CsrBtPhdcMgrBtLinkRelIndSend(pInstance->qId, pDevice->dataChannelId);
    
    return returnEvent;
}


FSM_ACTION_FN(CsrBtPhdcMgrDeviceFsm_AnyStateIntTimedOutReleaseReq)
{
    PhdcMgrDeviceListEntry *pDevice;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcFsmParam *param;

    CSR_BT_PHDC_M_ASSERT(pv_arg1 != NULL);
    CSR_BT_PHDC_M_ASSERT(pv_arg2 == NULL);
    CSR_UNUSED(pv_arg2);

    param = (CsrBtPhdcFsmParam *) pv_arg1;
    pDevice = (PhdcMgrDeviceListEntry *) param->argP;

    CsrBtPhdc20601AbrtEncodeSend(CSR_BT_PHDC_MGR_IFACEQUEUE, pDevice->dataChannelId, ABORT_REASON_RESPONSE_TIMEOUT);
    pDevice->reason = mapAbrtReason[ABORT_REASON_RESPONSE_TIMEOUT];

    return returnEvent;
}

#endif
