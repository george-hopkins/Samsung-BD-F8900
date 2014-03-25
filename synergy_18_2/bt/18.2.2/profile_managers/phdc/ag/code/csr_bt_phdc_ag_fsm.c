/****************************************************************************

                (c) Cambridge Silicon Radio Limited 2009

                All rights reserved and confidential information of CSR

REVISION:       $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_PHDC_AG_MODULE

#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_tasks.h"
#include "csr_bt_hdp_lib.h"
#include "csr_bt_cm_prim.h"
#include "csr_bt_core_stack_fsm.h"

#include "csr_bt_phdc_ieee_asn.h"
#include "csr_bt_phdc_ieee_endec.h"
#include "csr_bt_phdc_common.h"
#include "csr_bt_phdc_ag_fsm.h"
#include "csr_bt_phdc_ag_genfsm.h"
#include "csr_bt_phdc_ag_device_genfsm.h"
#include "csr_bt_phdc_ag_out.h"
#include "csr_bt_phdc_ag_prim.h"
#include "csr_bt_phdc_ag_lib.h"
#include "csr_bt_phdc_ag_common.h"

#define CSR_BT_PHDC_AG_MAX_SCAN_REPORTS 0xFFFF

void CsrBtPhdcAgentFsmRun(CsrBtPhdcAgentInstance *pAgInstance, fsm_event_t event)
{
    fsm_result_t fsmResult;

    fsmResult = fsm_16bit_run(&csr_bt_phdc_ag_fsm, &pAgInstance->state, event, pAgInstance, NULL);

    CSR_BT_PHDC_AG_ASSERT(FSM_RESULT_OK == fsmResult);
    CSR_UNUSED(fsmResult);
}

void CsrBtPhdcAgentDeviceFsmRun(CsrBtPhdcAgentInstance *pAgInstance, fsm_event_t event)
{
    fsm_result_t fsmResult;

    fsmResult = fsm_16bit_run(&csr_bt_phdc_ag_device_fsm, &pAgInstance->deviceState, event, pAgInstance, NULL);

    CSR_BT_PHDC_AG_ASSERT(FSM_RESULT_OK == fsmResult);
    CSR_UNUSED(fsmResult);
}

void CsrBtPhdcAgentTimedEvent(CsrUint16 event, void *pv_arg1)
{
    CsrBtPhdcAgentInstance *pInstance;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);
    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;

    pInstance->timerId = 0;
    CsrBtPhdcAgentDeviceFsmRun(pInstance,event);
}


/* local functions */
static fsm_event_t CsrBtPhdcAgErrorHandler(CsrBtPhdcAgentInstance *pInstance, APDU *pApdu)
{
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);

    switch (pApdu->choice)
    {
        case PRST_CHOSEN:
        case RLRE_CHOSEN:
        case AARQ_CHOSEN:
        case AARE_CHOSEN:         
        case RLRQ_CHOSEN:
        {
            CsrBtPhdc20601AbrtEncodeSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                         pInstance->assocChannelId, 
                                         ABORT_REASON_UNDEFINED);
            pInstance->associationResult = CSR_BT_PHDC_AG_RESULT_CODE_FAILURE;
            
            returnEvent = PHDC_AG_DEVICE_EV_INT_ABORT_REQ;
            break;
        }
        case ABRT_CHOSEN:
        {
            pInstance->associationResult = CSR_BT_PHDC_AG_RESULT_CODE_ABORT_REASON_UNDEFINED;
            returnEvent = PHDC_AG_DEVICE_EV_INT_DATA_IND_ABRT;
            break;        
        }
            
        default:
        {
            CsrBtPhdc20601RorjEncodeSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                        pInstance->assocChannelId,
                                        pInstance->invokeId,
                                        UNEXPECTED_ERROR);
            break;
        }

    }
    CsrBtPhdcAgFreeObjectInfo(pInstance);

    return returnEvent;
}

static void CsrBtPhdcUpdateAttribInfo(CsrBtPhdcAgentAttribInfo *pAgentAttribInfo,CsrBtPhdcAgDimAttribRes *pDimAttribRes)
{

    pAgentAttribInfo->attribId = pDimAttribRes->attribId;        
    pAgentAttribInfo->attribInfoLength = pDimAttribRes->attribInfoLength;
    pAgentAttribInfo->attribInfo = (CsrUint8 *)CsrPmemAlloc(pAgentAttribInfo->attribInfoLength);
    CsrMemCpy(pAgentAttribInfo->attribInfo, pDimAttribRes->attribInfo, pAgentAttribInfo->attribInfoLength);
}

static CsrBool CsrBtPhdcIsMoreObjectsPresent(CsrUint16 numObjects,CsrUint16 objectIndex)
{
    CsrBool retVal;

    if(objectIndex < numObjects)
    {
        retVal = TRUE;
    }
    else
    {
        retVal = FALSE;
    }

    return retVal; 
}

static CsrBool CsrBtPhdcIsMoreAttribsPresent(CsrBtPhdcAgentObjectInfo *pAgentObjectInfo)
{
    CsrBool retVal;

    if(pAgentObjectInfo->attribIndex < pAgentObjectInfo->numAttrib)
    {
        retVal = TRUE;
    }
    else
    {
        retVal = FALSE;
    }

    return retVal;
}

void CsrBtAgentSendHdpDataReq(CsrBtPhdcAgentInstance *pInstance,CsrUint8 *data,CsrUint16 length)
{ 
    CsrBtHdpDataReqSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                        pInstance->assocChannelId,
                        data,
                        length);
    pInstance->waitingForDataCfm = TRUE;

    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);
    pInstance->timerId = CsrSchedTimerSet(CSR_BT_PHDC_TO_CER_MDS, 
                                          CsrBtPhdcAgentTimedEvent, 
                                          PHDC_AG_DEVICE_EV_TIMER_TO_CER_MDS_EXPIRY, 
                                          (void *) pInstance);    
}


/* Event handlers */
FSM_ACTION_FN(CsrBtPhdcAgFsm_HdpDeactivateCfm)
{
    CsrBtPhdcAgentInstance *pInstance;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtHdpDeactivateCfm *pCfm;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pCfm = (CsrBtHdpDeactivateCfm *)pInstance->pRecvMsg;

    if (pCfm->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS ||
        pCfm->resultSupplier == CSR_BT_SUPPLIER_HDP)
    {
        CsrBtHdpUnregisterReqSend(CSR_BT_PHDC_AG_IFACEQUEUE); 
    }
    else
    {
        CsrBtPhdcAgUnregisterCfmSend(pInstance->appHandle, 
                                     pCfm->resultCode, 
                                     pCfm->resultSupplier);
    }
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgFsm_ActivatedPhdcUnregisterReq)
{ 
    CsrBtPhdcAgentInstance *pInstance;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcAgUnregisterReq *pReq;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pReq =  (CsrBtPhdcAgUnregisterReq *)pInstance->pRecvMsg;

    if (pReq->qId == pInstance->appHandle)  /* new connection from the device */
    {
        CsrBtHdpDeactivateReqSend(CSR_BT_PHDC_AG_IFACEQUEUE);              
    }
    else
    {
        /* The error could be because it is a request from not a registered Qid */
        CsrBtPhdcAgUnregisterCfmSend(pReq->qId,
                                     CSR_BT_PHDC_AG_RESULT_CODE_FAILURE,
                                     CSR_BT_SUPPLIER_PHDC_AG);
    }
    
    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);
    return returnEvent;

}



FSM_ACTION_FN(CsrBtPhdcAgFsm_RegisteringPhdcUnregisterReq)
{ 
    CsrBtPhdcAgentInstance *pInstance;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtPhdcAgUnregisterReq *pReq;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pReq =  (CsrBtPhdcAgUnregisterReq *)pInstance->pRecvMsg;

    if (pReq->qId == pInstance->appHandle)  /* new connection from the device */
    {
        CsrBtHdpUnregisterReqSend(CSR_BT_PHDC_AG_IFACEQUEUE);    
    }
    else
    {
        /* The error could be because it is a request from not a registered Qid */
        CsrBtPhdcAgUnregisterCfmSend(pReq->qId,
                                     CSR_BT_PHDC_AG_RESULT_CODE_FAILURE,
                                     CSR_BT_SUPPLIER_PHDC_AG);
    }
    
    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);
    return returnEvent;

}

FSM_ACTION_FN(CsrBtPhdcAgFsm_HdpUnregisterCfm)
{
    CsrBtPhdcAgentInstance *pInstance;
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    CsrBtHdpUnregisterCfm *pCfm;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pCfm = (CsrBtHdpUnregisterCfm *)pInstance->pRecvMsg;

    if (pCfm->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS ||
        pCfm->resultSupplier == CSR_BT_SUPPLIER_HDP)
    {
        if(pInstance->pApdu != NULL)
        {
            CsrPmemFree(pInstance->pApdu);
            pInstance->pApdu = NULL;
        }
        if(pInstance->pEventReportNotifyConfig != NULL)
        {
            CsrPmemFree(pInstance->pEventReportNotifyConfig);
            pInstance->pEventReportNotifyConfig = NULL;
        }

        CsrBtPhdcAgentFreeMdsConfiguration(pInstance);

        if(pInstance->tempApdu.commandApduLength != 0)
        {
            CsrPmemFree(pInstance->tempApdu.apdu);
        }

        if(pInstance->pObjectList != NULL)
        {  
            CsrUint8 tempObjCount,tempAttribCount;
            for(tempObjCount=0;tempObjCount<pInstance->objCount;tempObjCount++)
            {                
                for(tempAttribCount=0;tempAttribCount<pInstance->pObjectList[tempObjCount].numAttrib;tempAttribCount++)
                {
                    CsrPmemFree(pInstance->pObjectList[tempObjCount].pAttribList[tempAttribCount].attribInfo);
                }
                CsrPmemFree(pInstance->pObjectList[tempObjCount].pAttribList);
                CsrPmemFree(pInstance->pObjectList[tempObjCount].dataValue);

            }
            CsrPmemFree(pInstance->pObjectList);
            pInstance->pObjectList = NULL;
        }     

        if(pInstance->pRoivCmfdEventReport != NULL)
        {
            CsrPmemFree(pInstance->pRoivCmfdEventReport);
            pInstance->pRoivCmfdEventReport = NULL;
        }

        pInstance->sendConfigAgain = FALSE;
        pInstance->scanReportNo = 0;
        pInstance->localUnitCodeReq = FALSE;
        pInstance->waitingForDataCfm = FALSE;
        pInstance->rcAssoc = 0;
        
        pInstance->exhaustedAllConfig = FALSE;
        pInstance->invokeId = 0;
        pInstance->objCount = 0;
        pInstance->objIndex = 0; 
        
        CsrBtPhdcAgUnregisterCfmSend(pInstance->appHandle, 
                                     CSR_BT_PHDC_AG_RESULT_CODE_SUCCESS, 
                                     CSR_BT_SUPPLIER_PHDC_AG);

    }
    else
    {
        CsrBtPhdcAgUnregisterCfmSend(pInstance->appHandle, 
                                     pCfm->resultCode, 
                                     pCfm->resultSupplier);
    }
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgFsm_PhdcConfigureEndpointReq)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtPhdcAgConfigureEndpointReq *pConfigureEndpointReq;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;

    pConfigureEndpointReq = (CsrBtPhdcAgConfigureEndpointReq *)pInstance->pRecvMsg;
    pInstance->appHandle = pConfigureEndpointReq->qId;

    CsrBtHdpRegisterReqSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                            pConfigureEndpointReq->secLevel,
                            pConfigureEndpointReq->serviceName,
                            pConfigureEndpointReq->serviceDescription,
                            pConfigureEndpointReq->providerName,
                            pConfigureEndpointReq->sniffTimeOut,
                            pConfigureEndpointReq->numOfMdep,
                            pConfigureEndpointReq->supportedProcedures);
    pConfigureEndpointReq->serviceName = NULL;
    pConfigureEndpointReq->serviceDescription = NULL;
    pConfigureEndpointReq->providerName = NULL;
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgFsm_HdpRegisterInd)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpRegisterInd *pHdpRegisterInd;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;

    pHdpRegisterInd = (CsrBtHdpRegisterInd *)pInstance->pRecvMsg;
    CsrBtPhdcAgConfigureEndpointIndSend(pInstance->appHandle,pHdpRegisterInd->mdepId);
    return returnEvent;

}

FSM_ACTION_FN(CsrBtPhdcAgFsm_PhdcConfigureEndpointRes)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtPhdcAgConfigureEndpointRes *pConfigureEndpointRes;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;

    pConfigureEndpointRes = (CsrBtPhdcAgConfigureEndpointRes *)pInstance->pRecvMsg;

    CsrBtHdpRegisterRspSend((CsrSchedQid)CSR_BT_PHDC_AG_IFACEQUEUE,
                            pConfigureEndpointRes->datatype,
                            pConfigureEndpointRes->role,
                            pConfigureEndpointRes->description,
                            pConfigureEndpointRes->reuseMdepId);
    pConfigureEndpointRes->description = NULL;
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgFsm_HdpRegisterCfm)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpRegisterCfm *pHdpRegisterCfm;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pHdpRegisterCfm = (CsrBtHdpRegisterCfm *)pInstance->pRecvMsg;
    if((pHdpRegisterCfm->resultSupplier == CSR_BT_SUPPLIER_HDP) &&
       (pHdpRegisterCfm->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS))
    {
        CsrBtHdpActivateReqSend(CSR_BT_PHDC_AG_IFACEQUEUE, 1);        
    }
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgFsm_HdpActivateCfm)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpActivateCfm *pHdpActivateCfm;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pHdpActivateCfm = (CsrBtHdpActivateCfm *)pInstance->pRecvMsg;
    if((pHdpActivateCfm->resultSupplier == CSR_BT_SUPPLIER_HDP) &&
       (pHdpActivateCfm->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS))
    {
        CsrBtPhdcAgConfigureEndpointCfmSend(pInstance->appHandle,
                                            pHdpActivateCfm->resultCode,
                                            pHdpActivateCfm->resultSupplier);
    }
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgFsm_PhdcConfigureMdsReq)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtPhdcAgConfigureMdsReq *pConfigureMdsReq;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pConfigureMdsReq = (CsrBtPhdcAgConfigureMdsReq *)pInstance->pRecvMsg;

    CsrBtPhdcAgentStoreMdsConfiguration(pConfigureMdsReq, pInstance);

    return returnEvent;

}

FSM_ACTION_FN(CsrBtPhdcAgFsm_PhdcDimConfigureReq)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtPhdcAgDimConfigureReq *pDimConfigureReq;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pDimConfigureReq = (CsrBtPhdcAgDimConfigureReq *)pInstance->pRecvMsg;

    pInstance->objCount = pDimConfigureReq->objCount;

    if((pDimConfigureReq->devConfigId == CSR_BT_PHDC_AG_STD_DEVICE_CONFIG_WS) ||
        (pDimConfigureReq->devConfigId == CSR_BT_PHDC_AG_STD_DEVICE_CONFIG_BP))
    {
        CsrBtPhdcAgDimConfigCfmSend(pInstance->appHandle,CSR_BT_PHDC_AG_RESULT_CODE_SUCCESS);
        pInstance->eventReportingType = CSR_BT_PHDC_EVENT_REPORTING_TYPE_FIXED;
    }
    else
    {
        pInstance->pObjectList = 
            (CsrBtPhdcAgentObjectInfo *)CsrPmemAlloc(sizeof(CsrBtPhdcAgentObjectInfo)*(pInstance->objCount));
        CsrBtPhdcAgDimObjectIndSend(pInstance->appHandle, pInstance->objIndex);
    }
    return returnEvent;

}

FSM_ACTION_FN(CsrBtPhdcAgFsm_PhdcDimObjectRes)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtPhdcAgDimObjectRes *pDimObjectRes;
    CsrBtPhdcAgentObjectInfo *pAgentObjectInfo;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pDimObjectRes = (CsrBtPhdcAgDimObjectRes *)pInstance->pRecvMsg;

    CSR_BT_PHDC_AG_ASSERT((pInstance->objIndex == pDimObjectRes->objIndex));

    pInstance->objHandles[pDimObjectRes->objIndex] = pDimObjectRes->objHandle;

    pAgentObjectInfo = &pInstance->pObjectList[pInstance->objIndex];

    pAgentObjectInfo->objClass = pDimObjectRes->objClass;
    pAgentObjectInfo->objHandle = pDimObjectRes->objHandle;
    pAgentObjectInfo->dataValueLength = pDimObjectRes->dataValueLength;

    pAgentObjectInfo->dataValue = (CsrUint16 *)CsrPmemAlloc(sizeof(CsrUint16) * pDimObjectRes->dataValueLength);
    CsrMemCpy(pAgentObjectInfo->dataValue,pDimObjectRes->dataValue,sizeof(pAgentObjectInfo->dataValueLength));
    pAgentObjectInfo->numAttrib = pDimObjectRes->numAttrib;
    pAgentObjectInfo->attribIndex = 0;
    
    pAgentObjectInfo->pAttribList = (CsrBtPhdcAgentAttribInfo *)CsrPmemAlloc(sizeof(CsrBtPhdcAgentAttribInfo) * pDimObjectRes->numAttrib);

    CsrBtPhdcAgDimAttribIndSend(pInstance->appHandle,
                                pInstance->objIndex,
                                pAgentObjectInfo->attribIndex);
    return returnEvent;

}

FSM_ACTION_FN(CsrBtPhdcAgFsm_PhdcDimAttribRes)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtPhdcAgDimAttribRes *pDimAttribRes;
    CsrBtPhdcAgentAttribInfo *pAgentAttribInfo;
    CsrUint16 tempAttribIndex;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pDimAttribRes = (CsrBtPhdcAgDimAttribRes *)pInstance->pRecvMsg;

    CSR_BT_PHDC_AG_ASSERT((pInstance->objIndex == pDimAttribRes->objIndex));
    CSR_BT_PHDC_AG_ASSERT((pInstance->pObjectList[pInstance->objIndex].attribIndex == pDimAttribRes->attribIndex));

    tempAttribIndex = pInstance->pObjectList[pInstance->objIndex].attribIndex;

    pAgentAttribInfo = &pInstance->pObjectList[pInstance->objIndex].pAttribList[tempAttribIndex];

    CsrBtPhdcUpdateAttribInfo(pAgentAttribInfo,pDimAttribRes);
    if(pDimAttribRes->attribId == MDC_ATTR_ATTRIBUTE_VAL_MAP)
    {
        pInstance->eventReportingType = CSR_BT_PHDC_EVENT_REPORTING_TYPE_FIXED;
    }
    else
    {
        pInstance->eventReportingType = CSR_BT_PHDC_EVENT_REPORTING_TYPE_VAR;
    }

    pInstance->pObjectList[pInstance->objIndex].attribIndex++;

    if(CsrBtPhdcIsMoreAttribsPresent(&pInstance->pObjectList[pInstance->objIndex]) == TRUE)
    {
        CsrBtPhdcAgDimAttribIndSend(pInstance->appHandle,
                                    pInstance->objIndex,
                                    pInstance->pObjectList[pInstance->objIndex].attribIndex);
    }
    else
    {
        pInstance->objIndex++;
        if(CsrBtPhdcIsMoreObjectsPresent(pInstance->objCount,pInstance->objIndex) == TRUE)
        {
            CsrBtPhdcAgDimObjectIndSend(pInstance->appHandle,pInstance->objIndex);
        }
        else
        {
            CsrBtPhdcAgDimConfigCfmSend(pInstance->appHandle,CSR_BT_PHDC_AG_RESULT_CODE_SUCCESS);

        }
    }
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgFsm_PhdcGetDeviceCapabReq)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtPhdcAgGetDeviceCapabReq *pGetDeviceCapabReq;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pGetDeviceCapabReq = (CsrBtPhdcAgGetDeviceCapabReq *)pInstance->pRecvMsg;

    CsrBtBdAddrCopy(&pInstance->local_bd_addr, &pGetDeviceCapabReq->deviceAddr);

    CsrBtHdpGetCapabReqSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                            pGetDeviceCapabReq->deviceAddr,
                            pGetDeviceCapabReq->mdepDataTypeMask,
                            0x00,
                            CSR_BT_HDP_SINK_ROLE);
    

    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgFsm_HdpGetCapabInd)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpCtrlGetCapabInd *pGetCapabInd;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pGetCapabInd = (CsrBtHdpCtrlGetCapabInd *)pInstance->pRecvMsg;

    CsrBtPhdcAgGetDeviceCapabIndSend(pInstance->appHandle,
                                    pGetCapabInd->deviceAddr,
                                    pGetCapabInd->hdpInstanceId,
                                    pGetCapabInd->serviceName,
                                    pGetCapabInd->providerName,
                                    pGetCapabInd->supportedFeatureListLen,
                                    pGetCapabInd->supportedFeatureList);
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgFsm_HdpGetCapabCfm)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpCtrlGetCapabCfm *pGetCapabCfm;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pGetCapabCfm = (CsrBtHdpCtrlGetCapabCfm *)pInstance->pRecvMsg;

    if((pGetCapabCfm->resultSupplier == CSR_BT_SUPPLIER_CM) &&
        (pGetCapabCfm->resultCode == CSR_BT_RESULT_CODE_CM_SUCCESS))
    {  
        CsrBtHdpCtrlGetCapabCfmSend(pInstance->appHandle,
                                    pGetCapabCfm->deviceAddr,
                                    pGetCapabCfm->resultCode,
                                    pGetCapabCfm->resultSupplier);
    }

    return returnEvent;

}

/* local functions for Device handlers */
static void CsrBtPhdcSendMdcNotifyConfig(CsrBtPhdcAgentInstance *pInstance)
{
    CsrUint16 encodedMsgLen;
    CsrUint8 *msgPtr;

    if(pInstance->devConfigId == CSR_BT_PHDC_AG_STD_DEVICE_CONFIG_WS)
    {
        encodedMsgLen = CSR_BT_PHDC_AG_STDCONFIG_PRSTAPDU_WS_LEN;
        pInstance->stdConfigPrstApduWs[6] = (pInstance->invokeId & 0xff00)>>8;
        pInstance->stdConfigPrstApduWs[7] = (pInstance->invokeId & 0x00ff);
        msgPtr = CsrPmemAlloc(encodedMsgLen * sizeof(CsrUint8));
        CsrMemCpy(msgPtr,pInstance->stdConfigPrstApduWs,encodedMsgLen);
        pInstance->objCount = 1;
        pInstance->objHandles[0] = 1;
    }
    else if(pInstance->devConfigId == CSR_BT_PHDC_AG_STD_DEVICE_CONFIG_BP)
    {
        encodedMsgLen = CSR_BT_PHDC_AG_STDCONFIG_PRSTAPDU_BP_LEN;
        pInstance->stdConfigPrstApduWs[6] = (pInstance->invokeId & 0xff00)>>8;
        pInstance->stdConfigPrstApduWs[7] = (pInstance->invokeId & 0x00ff);
        msgPtr = CsrPmemAlloc(encodedMsgLen * sizeof(CsrUint8));
        CsrMemCpy(msgPtr,pInstance->stdConfigPrstApduBp,encodedMsgLen);
        pInstance->objCount = 2;
        pInstance->objHandles[0] = 1;
        pInstance->objHandles[1] = 2;
    }
    else
    {
        CsrUint8 tempObjCount,tempAttribCount;
        CsrBtPhdcFormEventReportArgumentSimpleNotiConfig(pInstance);

        encodedMsgLen = CsrBtPhdcMderCalculateApduEncodedLength(pInstance->pApdu);
        msgPtr = CsrPmemAlloc(encodedMsgLen * sizeof(CsrUint8));
        
        CsrBtPhdcMderEncodeApdu(pInstance->pApdu, msgPtr);            

        for(tempObjCount=0;tempObjCount<pInstance->objCount;tempObjCount++)
        {
            for(tempAttribCount=0;tempAttribCount<pInstance->configReport.config_obj_list.value[tempObjCount].attributes.count;tempAttribCount++)
            {
                CsrPmemFree(pInstance->configReport.config_obj_list.value[tempObjCount].attributes.value[tempAttribCount].attribute_value.value);
            }
            CsrPmemFree(pInstance->configReport.config_obj_list.value[tempObjCount].attributes.value);
        }
        CsrPmemFree(pInstance->configReport.config_obj_list.value);
        CsrPmemFree(pInstance->pEventReportNotifyConfig);
        pInstance->pEventReportNotifyConfig = NULL;

    }
    CsrBtHdpDataReqSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                        pInstance->assocChannelId,
                        msgPtr,
                        encodedMsgLen);

    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);
    pInstance->timerId = CsrSchedTimerSet(CSR_BT_PHDC_TO_CONFIG, 
                                          CsrBtPhdcAgentTimedEvent, 
                                          PHDC_AG_DEVICE_EV_TIMER_TO_CONFIG_EXPIRY, 
                                          (void *) pInstance);

}

static void CsrBtPhdcSendGetResponse(CsrBtPhdcAgentInstance *pInstance, APDU* pApdu)
{
    CsrUint16 getMdsInvokeId;
    GetArgumentSimple *pRoivCmipGet;
    DATA_apdu *pDataApdu = (DATA_apdu  *)pApdu->u.prst.value;
    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);

    getMdsInvokeId = pDataApdu->invoke_id; 
    pRoivCmipGet = (GetArgumentSimple *)&pDataApdu->choice.u.roiv_cmipGet;
    if(pRoivCmipGet->obj_handle != CSR_BT_PHDC_AG_MDS_OBJECT_HANDLE)
    {
        CsrBtPhdc20601RoerEncodeSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                     pInstance->assocChannelId,
                                     getMdsInvokeId,
                                     NO_SUCH_OBJECT_INSTANCE);
    }
    else
    {
        APDU *pGetResultSimpleApdu;
        CsrUint16 encodedMsgLen;
        CsrUint8 *msgPtr;            

        pInstance->pGetResultSimple->invoke_id = getMdsInvokeId;

        /**/
        pGetResultSimpleApdu = (APDU *)CsrPmemAlloc(sizeof(APDU));
        pGetResultSimpleApdu->choice = PRST_CHOSEN;
        pGetResultSimpleApdu->u.prst.value = (CsrUint8 *)pInstance->pGetResultSimple;

        /**/
        encodedMsgLen = CsrBtPhdcMderCalculateApduEncodedLength(pGetResultSimpleApdu);
        msgPtr = CsrPmemAlloc(encodedMsgLen*sizeof(CsrUint8));
        CsrBtPhdcMderEncodeApdu(pGetResultSimpleApdu, msgPtr);
        CsrBtHdpDataReqSend(CSR_BT_PHDC_AG_IFACEQUEUE, pInstance->assocChannelId, msgPtr, encodedMsgLen);    
        CsrPmemFree(pGetResultSimpleApdu);
    }
}

static fsm_event_t CsrBtPhdcHandleConfirmedEventReport(CsrBtPhdcAgentInstance *pInstance, APDU* pApdu)
{
    PRST_apdu *prst;
    EventReportResultSimple *pRorsCmipConfirmedEventReport;
    DATA_apdu *dataApdu;

    fsm_event_t returnEvent = FSM_EVENT_NULL;
    prst = &pApdu->u.prst;
    dataApdu = (DATA_apdu *)(prst->value);

    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);

    pRorsCmipConfirmedEventReport = (EventReportResultSimple *)&dataApdu->choice.u.rors_cmipConfirmedEventReport;

    if((pRorsCmipConfirmedEventReport->event_type == MDC_NOTI_CONFIG) &&
       (pRorsCmipConfirmedEventReport->event_reply_info.length != 0))
    {
        ConfigReportRsp *pConfigReportRsp = (ConfigReportRsp *)pRorsCmipConfirmedEventReport->event_reply_info.value;
        if(pConfigReportRsp->config_report_id == pInstance->devConfigId)
        {
            if(pConfigReportRsp->config_result == ACCEPTED_CONFIG)
            {
                CsrBtPhdcAgAssociateCfmSend(pInstance->appHandle,
                                            pInstance->psmIdentifier,
                                            pInstance->local_bd_addr,
                                            pInstance->mdepId,
                                            pInstance->mdepDataType,
                                            pInstance->maxPacketLength,
                                            pInstance->assocChannelId,
                                            CSR_BT_PHDC_AG_RESULT_CODE_CONFIG_ACCEPTED_CONFIG);

                pInstance->associateReq = FALSE;
                
                returnEvent = PHDC_AG_DEVICE_EV_INT_CONFIG_ACCEPTED;
            }
            else if(pConfigReportRsp->config_result == UNSUPPORTED_CONFIG)
            {
                if(pInstance->exhaustedAllConfig != TRUE)
                {
                    CsrBtPhdcAgAssociateCfmSend(pInstance->appHandle,
                                                pInstance->psmIdentifier,
                                                pInstance->local_bd_addr,
                                                pInstance->mdepId,
                                                pInstance->mdepDataType,
                                                pInstance->maxPacketLength,
                                                pInstance->assocChannelId,
                                                CSR_BT_PHDC_AG_RESULT_CODE_CONFIG_UNSUPPORTED_CONFIG);
                    pInstance->associateReq = FALSE;
                    CsrBtPhdcAgentFreeMdsConfiguration(pInstance);
                    returnEvent = PHDC_AG_DEVICE_EV_INT_CONFIG_UNKNOWN;
                }
                else
                {
                    pInstance->localDisassociateReq = TRUE; 

                    CsrBtPhdc20601RlrqEncodeSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                                 pInstance->assocChannelId,
                                                 RELEASE_REQUEST_REASON_NO_MORE_CONFIG);
                    
                    pInstance->associationResult = CSR_BT_PHDC_AG_RESULT_CODE_CONFIG_UNSUPPORTED_CONFIG;                                              
                    
                    pInstance->timerId = CsrSchedTimerSet(CSR_BT_PHDC_TO_RELEASE, 
                                                          CsrBtPhdcAgentTimedEvent, 
                                                          PHDC_AG_DEVICE_EV_TIMER_TO_RELEASE_EXPIRY, 
                                                          (void *) pInstance);

                    returnEvent = PHDC_AG_DEVICE_EV_INT_RELEASE_REQ;
                }
                CsrBtPhdcAgFreeObjectInfo(pInstance);
            }            
        }
    }

    return returnEvent;
}

static void CsrBtPhdcAgHandlePhdcDataReqFree(CsrUint16 scanReportType, CsrUint8 *pScanReport, APDU *pRoivCmfdEventReportApdu, EventReportArgumentSimple *pRoivCmipConfirmedEventReport)
{
    /* FREE according to type */
    if (pScanReport)
    {
        if(scanReportType == MDC_NOTI_SCAN_REPORT_FIXED)
        {
            CsrPmemFree(((ScanReportInfoFixed*)pScanReport)->obs_scan_fixed.value->obs_val_data.value);
            CsrPmemFree(((ScanReportInfoFixed*)pScanReport)->obs_scan_fixed.value);
        }
        else if(scanReportType == MDC_NOTI_SCAN_REPORT_VAR)
        {
            CsrPmemFree(((ScanReportInfoVar*)pScanReport)->obs_scan_var.value->attributes.value);
            CsrPmemFree(((ScanReportInfoVar*)pScanReport)->obs_scan_var.value);
        }
        else if(scanReportType == MDC_NOTI_SCAN_REPORT_MP_FIXED)
        {
            /* how about count */
            CsrPmemFree(((ScanReportInfoMPFixed*)pScanReport)->scan_per_fixed.value->obs_scan_fix.value->obs_val_data.value);
            CsrPmemFree(((ScanReportInfoMPFixed*)pScanReport)->scan_per_fixed.value->obs_scan_fix.value);
            CsrPmemFree(((ScanReportInfoMPFixed*)pScanReport)->scan_per_fixed.value);
        }
        else  /*if (scanReportType == MDC_NOTI_SCAN_REPORT_MP_VAR)*/
        {
            /* how about count */
            CsrPmemFree(((ScanReportInfoMPVar*)pScanReport)->scan_per_var.value->obs_scan_var.value->attributes.value);
            CsrPmemFree(((ScanReportInfoMPVar*)pScanReport)->scan_per_var.value->obs_scan_var.value);
            CsrPmemFree(((ScanReportInfoMPVar*)pScanReport)->scan_per_var.value);
        }
    }
    
    /* Free ROIV */
    if (pRoivCmipConfirmedEventReport)
    {
        CsrPmemFree(pRoivCmipConfirmedEventReport->event_info.value);
    }
    CsrPmemFree(pRoivCmfdEventReportApdu);
}
            

static void CsrBtPhdcAgHandlePhdcDataReq(CsrBtPhdcAgentInstance *pInstance, APDU *pRoivCmfdEventReportApdu, EventReportArgumentSimple *pRoivCmipConfirmedEventReport)
{
    ScanReportInfoFixed *pScanReportInfoFixed = NULL;
    ScanReportInfoVar *pScanReportInfoVar = NULL;
    ScanReportInfoMPFixed *pScanReportInfoMPFixed = NULL;
    ScanReportInfoMPVar *pScanReportInfoMPVar = NULL;

    CsrBtPhdcAgDataReq *pPhdcAgDataReq = (CsrBtPhdcAgDataReq *)pInstance->pRecvMsg;
    CsrUint16 encodedMsgLen = 0;
    CsrUint8 *msgPtr = NULL;
    CsrUint16 count = 0, tempCount;
    CsrUint16 scanReportType = 0;
    CsrUint8 *pScanReport = NULL;

    scanReportType = pRoivCmipConfirmedEventReport->event_type;

    if(scanReportType == MDC_NOTI_SCAN_REPORT_FIXED)
    {
        /* Fixed event reporting */
        pInstance->pScanReportInfoFixed->obs_scan_fixed.value = (ObservationScanFixed *)CsrPmemAlloc(sizeof(ObservationScanFixed));
        CsrBtPhdcAgFormObservationScanFixed(pInstance->pScanReportInfoFixed->obs_scan_fixed.value,&pPhdcAgDataReq->data);
        pScanReport = (CsrUint8 *)pInstance->pScanReportInfoFixed;
        pScanReportInfoFixed = pInstance->pScanReportInfoFixed;
        count = pScanReportInfoFixed->obs_scan_fixed.count;
    }
    else if(scanReportType == MDC_NOTI_SCAN_REPORT_VAR)
    {
        /* Varible event reporting */
        pInstance->pScanReportInfoVar->obs_scan_var.value = (ObservationScan *)CsrPmemAlloc(sizeof(ObservationScan));
        CsrBtPhdcAgFormObservationScan(pInstance,&pPhdcAgDataReq->data);
        pScanReport = (CsrUint8 *)pInstance->pScanReportInfoVar;
        pScanReportInfoVar = pInstance->pScanReportInfoVar;
        count = pScanReportInfoVar->obs_scan_var.count;
    }
    else if(scanReportType == MDC_NOTI_SCAN_REPORT_MP_FIXED)
    {
        /* MultiPerson Fixed event reporting */
        pInstance->pScanReportInfoMPFixed->scan_per_fixed.value = (ScanReportPerFixed *)CsrPmemAlloc(sizeof(ScanReportPerFixed));
        CsrBtPhdcAgFormScanReportPerFixed(pInstance->pScanReportInfoMPFixed->scan_per_fixed.value,&pPhdcAgDataReq->data);
        pScanReport = (CsrUint8 *)pInstance->pScanReportInfoMPFixed;
        pScanReportInfoMPFixed = pInstance->pScanReportInfoMPFixed;
        count = pScanReportInfoMPFixed->scan_per_fixed.count;
    }
    else /* if (scanReportType == MDC_NOTI_SCAN_REPORT_MP_VAR) */
    {
        /* Multiperson Varible event reporting */
        pInstance->pScanReportInfoMPVar->scan_per_var.value = (ScanReportPerVar *)CsrPmemAlloc(sizeof(ScanReportPerVar));
        CsrBtPhdcAgFormScanReportPerVar(pInstance->pScanReportInfoMPVar->scan_per_var.value,&pPhdcAgDataReq->data,pInstance);
        pScanReport = (CsrUint8 *)pInstance->pScanReportInfoMPVar;
        pScanReportInfoMPVar = pInstance->pScanReportInfoMPVar;
        count = pScanReportInfoMPVar->scan_per_var.count;
    }
    
    switch(pInstance->transmitFlag)
    {
        case CSR_BT_PHDC_TRANSMIT_FLAG_FINAL:
        {          
            pRoivCmipConfirmedEventReport->event_info.value = pScanReport;
                            
            if(count > 1)
            {
                for(tempCount = 0; tempCount < count; tempCount++)
                {
                    if(scanReportType == MDC_NOTI_SCAN_REPORT_FIXED)
                    {
                        --pScanReportInfoFixed->obs_scan_fixed.value;
                    }
                    else if(scanReportType == MDC_NOTI_SCAN_REPORT_VAR)
                    {
                        --pScanReportInfoVar->obs_scan_var.value;
                    }
                    else if(scanReportType == MDC_NOTI_SCAN_REPORT_MP_FIXED)
                    {
                        --pScanReportInfoMPFixed->scan_per_fixed.value;
                    }
                    else /*if (scanReportType == MDC_NOTI_SCAN_REPORT_MP_VAR)*/
                    {
                        --pScanReportInfoMPVar->scan_per_var.value;
                    }
                }
            }

            if (pScanReport)
            {
                ((ScanReportInfoVar *)pScanReport)->scan_report_no = pInstance->scanReportNo;
            }

            if (pRoivCmfdEventReportApdu)
            {
                pRoivCmfdEventReportApdu->u.prst.value = (CsrUint8 *)pInstance->pRoivCmfdEventReport;
            
                encodedMsgLen = CsrBtPhdcMderCalculateApduEncodedLength(pRoivCmfdEventReportApdu);
                msgPtr = CsrPmemAlloc(encodedMsgLen*sizeof(CsrUint8));
                CsrBtPhdcMderEncodeApdu(pRoivCmfdEventReportApdu, msgPtr);
            }
            
            CsrBtAgentSendHdpDataReq(pInstance, msgPtr, encodedMsgLen);

            CsrBtPhdcAgHandlePhdcDataReqFree(scanReportType, pScanReport, pRoivCmfdEventReportApdu, pRoivCmipConfirmedEventReport);

            CsrPmemFree(pInstance->pRoivCmfdEventReport);
            pInstance->pRoivCmfdEventReport = NULL;
            break;
        }

        case CSR_BT_PHDC_TRANSMIT_FLAG_START:
        {
            CsrBtPhdcAgDataCfmSend(pInstance->appHandle,
                                   pInstance->assocChannelId,
                                   CSR_BT_PHDC_AG_RESULT_CODE_SEND_NEXT_MEASUREMENT);

            pInstance->waitingForDataCfm = FALSE;
         
            if(scanReportType == MDC_NOTI_SCAN_REPORT_FIXED)
            {
                pScanReportInfoFixed->obs_scan_fixed.value++;
            }
            else if(scanReportType == MDC_NOTI_SCAN_REPORT_VAR)
            {
                pScanReportInfoVar->obs_scan_var.value++;
            }
            else if(scanReportType == MDC_NOTI_SCAN_REPORT_MP_FIXED)
            {
                pScanReportInfoMPFixed->scan_per_fixed.value++;
            }
            else /* if (scanReportType == MDC_NOTI_SCAN_REPORT_MP_VAR) */
            {
                pScanReportInfoMPVar->scan_per_var.value++;
            }
            break;
        }

        case CSR_BT_PHDC_TRANSMIT_FLAG_CONTINUE:
        {
            count++;

            if(count == CSR_BT_PHDC_AG_MAX_MEAS_REPORTS)
            {
                for(tempCount = 0; tempCount < count; tempCount++)
                {
                    if(scanReportType == MDC_NOTI_SCAN_REPORT_FIXED)
                    {
                        --pScanReportInfoFixed->obs_scan_fixed.value;
                    }
                    else if(scanReportType == MDC_NOTI_SCAN_REPORT_VAR)
                    {
                        --pScanReportInfoVar->obs_scan_var.value;
                    }
                    else if(scanReportType == MDC_NOTI_SCAN_REPORT_MP_FIXED)
                    {
                        --pScanReportInfoMPFixed->scan_per_fixed.value;
                    }
                    else /*if (scanReportType == MDC_NOTI_SCAN_REPORT_MP_VAR)*/
                    {
                        --pScanReportInfoMPVar->scan_per_var.value;
                    }
                }
                if (pRoivCmfdEventReportApdu)
                {
                    (pRoivCmfdEventReportApdu)->u.prst.value = (CsrUint8 *)pInstance->pRoivCmfdEventReport;

                    encodedMsgLen = CsrBtPhdcMderCalculateApduEncodedLength(pRoivCmfdEventReportApdu);
                    msgPtr = CsrPmemAlloc(encodedMsgLen*sizeof(CsrUint8));
                    CsrBtPhdcMderEncodeApdu(pRoivCmfdEventReportApdu, msgPtr);
                }

                CsrBtAgentSendHdpDataReq(pInstance,msgPtr,encodedMsgLen);

                CsrBtPhdcAgHandlePhdcDataReqFree(scanReportType, pScanReport, pRoivCmfdEventReportApdu, pRoivCmipConfirmedEventReport);

                CsrPmemFree(pInstance->pRoivCmfdEventReport);
                pInstance->pRoivCmfdEventReport = NULL;
            }
            else
            {
                CsrBtPhdcAgDataCfmSend(pInstance->appHandle, 
                                       pInstance->assocChannelId,
                                       CSR_BT_PHDC_AG_RESULT_CODE_SEND_NEXT_MEASUREMENT);
                                  
                pInstance->waitingForDataCfm = FALSE;
             
                if(scanReportType == MDC_NOTI_SCAN_REPORT_FIXED)
                {
                    pScanReportInfoFixed->obs_scan_fixed.value++;
                }
                else if(scanReportType == MDC_NOTI_SCAN_REPORT_VAR)
                {
                    pScanReportInfoVar->obs_scan_var.value++;
                }
                else if(scanReportType == MDC_NOTI_SCAN_REPORT_MP_FIXED)
                {
                    pScanReportInfoMPFixed->scan_per_fixed.value++;
                }
                else /* if (scanReportType == MDC_NOTI_SCAN_REPORT_MP_VAR) */
                {
                    pScanReportInfoMPVar->scan_per_var.value++;
                }
            }
            break;
        }

        default:
        {
            break;
        }                
    }
}

/* Device event handlers */
FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_DisconnectedHdpAcceptConnectAssociateChannelInd)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpAcceptConnectAssociateChannelInd *pAcceptConnectAssociateChannelInd;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pAcceptConnectAssociateChannelInd = (CsrBtHdpAcceptConnectAssociateChannelInd *)pInstance->pRecvMsg;

    pInstance->mdepId = pAcceptConnectAssociateChannelInd->mdepId;
    pInstance->assocChannelId = pAcceptConnectAssociateChannelInd->assocChannelId;

    CsrBtPhdcAgAcceptBtLinkIndSend(pInstance->appHandle,
                                   pAcceptConnectAssociateChannelInd->deviceAddr);

    return returnEvent;

}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_DisconnectedPhdcAcceptBtLinkRes)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtPhdcAgAcceptBtLinkRes *pcceptBtLinkRes;
    CsrUint8 response;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pcceptBtLinkRes = (CsrBtPhdcAgAcceptBtLinkRes *)pInstance->pRecvMsg;

    response = ( pcceptBtLinkRes->accept == TRUE  ) ?  CSR_BT_RESULT_CODE_HDP_SUCCESS : 
    CSR_BT_RESULT_CODE_HDP_UNSPECIFIED_ERROR;


    CsrBtHdpAcceptConnectAssociateChannelResSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                                 response,
                                                 pcceptBtLinkRes->maxPacketLength,
                                                 pInstance->assocChannelId);

    return returnEvent;

}


FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_DisconnectedHdpConnectAssociateChannelInd)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpConnectAssociateChannelInd *pConnectAssociateChannelInd;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pConnectAssociateChannelInd = (CsrBtHdpConnectAssociateChannelInd *)pInstance->pRecvMsg;

    if((pConnectAssociateChannelInd->resultSupplier == CSR_BT_SUPPLIER_HDP) &&
        (pConnectAssociateChannelInd->resultCode == CSR_BT_RESULT_CODE_HDP_SUCCESS))
    {  
        pInstance->dataChannelId= pConnectAssociateChannelInd->hdpInstanceId;
        CsrBtBdAddrCopy(&pInstance->local_bd_addr, &pConnectAssociateChannelInd->deviceAddr);
        pInstance->mdepId = pConnectAssociateChannelInd->mdepId;
        pInstance->mdepDataType = pConnectAssociateChannelInd->dataType;
        pInstance->assocChannelId = pConnectAssociateChannelInd->assocChannelId;
        pInstance->maxPacketLength = pConnectAssociateChannelInd->maxPacketLength;       
    }
    CsrBtPhdcAgSetupBtLinkIndSend(pInstance->appHandle,
                                  pConnectAssociateChannelInd->deviceAddr,
                                  pConnectAssociateChannelInd->resultCode,
                                  pConnectAssociateChannelInd->resultSupplier);        

    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_DisconnectedPhdcAssociateReq)
{   
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtPhdcAgAssociateReq *pAssociateReq;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pAssociateReq = (CsrBtPhdcAgAssociateReq *)pInstance->pRecvMsg;

    pInstance->exhaustedAllConfig = pAssociateReq->release;
    pInstance->devConfigId = pAssociateReq->devConfigId;
    pInstance->associateReq = TRUE;
    
    CsrBtHdpConnectAssociateChannelReqSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                           pAssociateReq->psmIdentifier,
                                           pAssociateReq->deviceAddr,
                                           pAssociateReq->mdepId,
                                           pAssociateReq->mdepDataType,
                                           CSR_BT_HDP_MDEP_SOURCE,
                                           pAssociateReq->maxPacketLength);

    return returnEvent;    

}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_UnassociatedTimerBtLinkRelExpiry)
{
    CsrBtPhdcAgentInstance *pInstance;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    CsrBtHdpDisconnectAssociateChannelReqSend(CSR_BT_PHDC_AG_IFACEQUEUE,pInstance->assocChannelId);    
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_UnassociatedPhdcAssociateReq)
{   
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtPhdcAgAssociateReq *pAssociateReq;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pAssociateReq = (CsrBtPhdcAgAssociateReq *)pInstance->pRecvMsg;

    pInstance->exhaustedAllConfig = pAssociateReq->release;
    pInstance->devConfigId = pAssociateReq->devConfigId;
    pInstance->associateReq = TRUE;

    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);
    CsrBtPhdc20601AarqEncodeSend(pInstance);

    return returnEvent;    

}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_UnassociatedHdpDataInd)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpDataInd *pHdpDataInd;
    APDU *apdu;

    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pHdpDataInd = (CsrBtHdpDataInd *)pInstance->pRecvMsg;

    CsrBtPhdcMderDecodeApdu(pHdpDataInd->data,&apdu);
    CsrPmemFree(pHdpDataInd->data);

    if(apdu->choice == RLRQ_CHOSEN)
    {
        CsrBtPhdc20601RlreEncodeSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                    pInstance->assocChannelId, 
                                    RELEASE_RESPONSE_REASON_NORMAL);
    }
    else
    {
        returnEvent = CsrBtPhdcAgErrorHandler(pInstance, apdu);
    }
    CsrBtPhdcFreeDecodeAnyApdu(apdu);

    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_UnassociatedHdpDisconnectAssociateChannelInd)
{
    CsrBtPhdcAgentInstance *pInstance;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;

    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);

    if(pInstance->associateReq)
    {
        CsrBtPhdcAgAssociateCfmSend(pInstance->appHandle,
                                    pInstance->psmIdentifier, 
                                    pInstance->local_bd_addr, 
                                    pInstance->mdepId, 
                                    pInstance->mdepDataType, 
                                    pInstance->maxPacketLength, 
                                    pInstance->assocChannelId, 
                                    pInstance->associationResult);   
        
        pInstance->associateReq = FALSE;

    }
    

    CsrBtPhdcAgBtLinkRelIndSend(pInstance->appHandle, 
                                pInstance->local_bd_addr, 
                                CSR_BT_PHDC_AG_RESULT_CODE_SUCCESS, 
                                CSR_BT_SUPPLIER_PHDC_AG );

    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_ConnectingHdpConnectAssociateChannelCfm)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpConnectAssociateChannelCfm *pConnectAssociateChannelCfm;

    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pConnectAssociateChannelCfm = (CsrBtHdpConnectAssociateChannelCfm *)pInstance->pRecvMsg;

    pInstance->assocChannelId = pConnectAssociateChannelCfm->assocChannelId;
    pInstance->psmIdentifier = pConnectAssociateChannelCfm->hdpInstanceId;
    pInstance->mdepId = pConnectAssociateChannelCfm->mdepId;
    pInstance->mdepDataType = pConnectAssociateChannelCfm->dataType;
    pInstance->maxPacketLength = pConnectAssociateChannelCfm->maxPacketLength;
    pInstance->dataChannelId = pConnectAssociateChannelCfm->hdpInstanceId;

    CsrBtPhdc20601AarqEncodeSend(pInstance);

    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_AssociatingTimerToAssocExpiry)
{
    CsrBtPhdcAgentInstance *pInstance;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;

    if(pInstance->rcAssoc < CSR_BT_PHDC_RC_ASSOC)
    {
        CsrUint8 *tempPtr =  CsrPmemAlloc(pInstance->tempApdu.commandApduLength * sizeof(CsrUint8));
        CsrMemCpy(tempPtr,pInstance->tempApdu.apdu,pInstance->tempApdu.commandApduLength);
        CsrBtHdpDataReqSend(CSR_BT_PHDC_AG_IFACEQUEUE,pInstance->assocChannelId,
                            tempPtr,pInstance->tempApdu.commandApduLength);
        pInstance->rcAssoc++;
        pInstance->timerId = CsrSchedTimerSet(CSR_BT_PHDC_TO_ASSOC, 
                                              CsrBtPhdcAgentTimedEvent, 
                                              PHDC_AG_DEVICE_EV_TIMER_TO_ASSOC_EXPIRY, 
                                              (void *) pInstance);        
    }
    else
    {
        CsrBtPhdc20601AbrtEncodeSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                     pInstance->assocChannelId, 
                                     ABORT_REASON_CONFIGURATION_TIMEOUT);
        CsrPmemFree(pInstance->tempApdu.apdu);
        pInstance->tempApdu.commandApduLength = 0;
        pInstance->rcAssoc = 0;
        pInstance->associationResult = CSR_BT_PHDC_AG_RESULT_CODE_TO_ASSOC_EXPIRY;
        returnEvent = PHDC_AG_DEVICE_EV_INT_ABORT_REQ;
    }

    return returnEvent;

}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_AssociatingHdpDataInd)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpDataInd *pHdpDataInd;
    APDU *apdu;
    AARE_apdu *pAare;
    CsrBool validatonResult=FALSE;

    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pHdpDataInd = (CsrBtHdpDataInd *)pInstance->pRecvMsg;

    CsrBtPhdcMderDecodeApdu(pHdpDataInd->data,&apdu);
    CsrPmemFree(pHdpDataInd->data);

    if(apdu->choice == AARE_CHOSEN)
    {
        pInstance->associationResult = apdu->u.aare.result;
        pAare = &apdu->u.aare;
        validatonResult = CsrBtPhdcAgentValidateAare(pAare);

        CsrSchedTimerCancel(pInstance->timerId,NULL,NULL);

        if( (validatonResult == TRUE) && (pInstance->associationResult == ACCEPTED_UNKNOWN_CONFIG))
        {
            CsrBtPhdcSendMdcNotifyConfig(pInstance);
            returnEvent = PHDC_AG_DEVICE_EV_INT_CONFIG_UNKNOWN;
        }
        else if( (validatonResult == TRUE) && (pInstance->associationResult == ACCEPTED))
        {
            if(pInstance->devConfigId == CSR_BT_PHDC_AG_STD_DEVICE_CONFIG_WS)
            {
                pInstance->objCount = 1;
                pInstance->objHandles[0] = 1;            
            }
            else if(pInstance->devConfigId == CSR_BT_PHDC_AG_STD_DEVICE_CONFIG_BP)
            {
                pInstance->objCount = 2;
                pInstance->objHandles[0] = 1;
                pInstance->objHandles[1] = 2;            
            }
            CsrBtPhdcAgAssociateCfmSend(pInstance->appHandle,
                                        pInstance->psmIdentifier, 
                                        pInstance->local_bd_addr, 
                                        pInstance->mdepId, 
                                        pInstance->mdepDataType, 
                                        pInstance->maxPacketLength, 
                                        pInstance->assocChannelId, 
                                        CSR_BT_PHDC_AG_RESULT_CODE_SUCCESS);
            pInstance->associateReq = FALSE;

            returnEvent = PHDC_AG_DEVICE_EV_INT_CONFIG_ACCEPTED;
        }
        else
        {
            CsrBtPhdcAgAssociateCfmSend(pInstance->appHandle,
                                        pInstance->psmIdentifier, 
                                        pInstance->local_bd_addr, 
                                        pInstance->mdepId, 
                                        pInstance->mdepDataType, 
                                        pInstance->maxPacketLength, 
                                        pInstance->assocChannelId, 
                                        pInstance->associationResult);   
            pInstance->associateReq = FALSE;

            pInstance->timerId = CsrSchedTimerSet(CSR_BT_PHDC_BT_LINK_RELEASE, 
                                                  CsrBtPhdcAgentTimedEvent, 
                                                  PHDC_AG_DEVICE_EV_TIMER_BT_LINK_REL_EXPIRY, 
                                                  (void *) pInstance);

            returnEvent = PHDC_AG_DEVICE_EV_INT_CONFIG_REJECTED;
        }
    }
    else if(apdu->choice == AARQ_CHOSEN)
    {
        DataProto tempProto;

        tempProto.data_proto_id = DATA_PROTO_ID_EMPTY;
        tempProto.data_proto_info.length = 0;
        tempProto.data_proto_info.value = NULL;
        
        CsrBtPhdcAgAssociateCfmSend(pInstance->appHandle,
                                    pInstance->psmIdentifier, 
                                    pInstance->local_bd_addr, 
                                    pInstance->mdepId, 
                                    pInstance->mdepDataType, 
                                    pInstance->maxPacketLength, 
                                    pInstance->assocChannelId, 
                                    CSR_BT_PHDC_AG_RESULT_CODE_AARE_REJECTED_PERMANENT);   
        pInstance->associateReq = FALSE;

        CsrBtPhdc20601AareEncodeSend(CSR_BT_PHDC_AG_IFACEQUEUE, 
                                     pInstance->assocChannelId, 
                                     &tempProto, 
                                     CSR_BT_PHDC_AG_RESULT_CODE_AARE_REJECTED_PERMANENT);
        
        pInstance->timerId = CsrSchedTimerSet(CSR_BT_PHDC_BT_LINK_RELEASE, 
                                              CsrBtPhdcAgentTimedEvent, 
                                              PHDC_AG_DEVICE_EV_TIMER_BT_LINK_REL_EXPIRY, 
                                              (void *) pInstance);

        returnEvent = PHDC_AG_DEVICE_EV_INT_CONFIG_REJECTED;    
    }
    else 
    {
        returnEvent = CsrBtPhdcAgErrorHandler(pInstance, apdu);
    }
    CsrBtPhdcFreeDecodeAnyApdu(apdu);

    return returnEvent;

}


FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_AssociatingHdpDisconnectAssociateChannelInd)
{
    CsrBtPhdcAgentInstance *pInstance;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;

    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);
    CsrBtPhdcAgAssociateCfmSend(pInstance->appHandle,
                                pInstance->psmIdentifier,
                                pInstance->local_bd_addr,
                                pInstance->mdepId,
                                pInstance->mdepDataType,
                                pInstance->maxPacketLength,
                                pInstance->assocChannelId,
                                CSR_BT_PHDC_AG_RESULT_CODE_FAILURE);
    pInstance->associateReq = FALSE;

    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_OtherHdpDataCfm)
{
    fsm_event_t returnEvent = FSM_EVENT_NULL;
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_OtherHdpDataInd)
{   
    CsrBtPhdcAgentInstance *pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    CsrBtHdpDataInd *pHdpDataInd = (CsrBtHdpDataInd *)pInstance->pRecvMsg;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CsrPmemFree(pHdpDataInd->data);
    
    return returnEvent;    
}


FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_TimerToConfigExpiry)
{
    CsrBtPhdcAgentInstance *pInstance;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;

    CsrBtPhdc20601AbrtEncodeSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                 pInstance->assocChannelId, 
                                 ABORT_REASON_CONFIGURATION_TIMEOUT);
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_SendingConfigHdpDataCfm)
{
    CsrBtPhdcAgentInstance *pInstance;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);
    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    
    if(pInstance->sendConfigAgain)
    {
        CsrBtPhdcSendMdcNotifyConfig(pInstance);
        pInstance->sendConfigAgain = FALSE;
    }
    else
    {
        returnEvent = PHDC_AG_DEVICE_EV_INT_DATA_IND_CONFIG_SENT;
    }

    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_SendingConfigHdpDataInd)
{   
    CsrBtPhdcAgentInstance *pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    CsrBtHdpDataInd *pHdpDataInd = (CsrBtHdpDataInd *)pInstance->pRecvMsg;
    APDU *pApdu;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CsrBtPhdcMderDecodeApdu(pHdpDataInd->data, &pApdu);
    CsrPmemFree(pHdpDataInd->data);

    if(pApdu->choice == PRST_CHOSEN)
    {
        DATA_apdu *dataApdu;
        CsrUint16 prstChoiceType;
        PRST_apdu *prst;
 
        prst = &pApdu->u.prst;
        dataApdu = (DATA_apdu *)(prst->value);
        prstChoiceType = dataApdu->choice.choice;
            
        if(prstChoiceType == ROIV_CMIP_GET_CHOSEN)
        {
            CsrBtPhdcSendGetResponse(pInstance, pApdu);
            /* Config must be sent again */
            pInstance->sendConfigAgain = TRUE;      
        }
        else
        {
            returnEvent = CsrBtPhdcHandleConfirmedEventReport(pInstance, pApdu);
        }     
    }
    else
    {
        /* Ignore */
    }

    CsrBtPhdcFreeDecodeAnyApdu(pApdu);

    return returnEvent;    
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_SendingConfigPhdcAssociateReq)
{   
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtPhdcAgAssociateReq *pAssociateReq;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pAssociateReq = (CsrBtPhdcAgAssociateReq *)pInstance->pRecvMsg;

    pInstance->exhaustedAllConfig = pAssociateReq->release;
    pInstance->devConfigId = pAssociateReq->devConfigId;
    pInstance->associateReq = TRUE;

    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);
    CsrBtPhdcSendMdcNotifyConfig(pInstance);

    return returnEvent;    

}
    
FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_WaitingApprovalHdpDataInd)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpDataInd *pHdpDataInd;
    APDU *pApdu;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pHdpDataInd = (CsrBtHdpDataInd *)pInstance->pRecvMsg;

    CsrBtPhdcMderDecodeApdu(pHdpDataInd->data, &pApdu);
    CsrPmemFree(pHdpDataInd->data);

    if(pApdu->choice == PRST_CHOSEN)
    {
        DATA_apdu *dataApdu;
        CsrUint16 prstChoiceType;
        PRST_apdu *prst;
 
        prst = &pApdu->u.prst;
        dataApdu = (DATA_apdu *)(prst->value);
        prstChoiceType = dataApdu->choice.choice;
            
        if(prstChoiceType == ROIV_CMIP_GET_CHOSEN)
        {
            CsrBtPhdcSendGetResponse(pInstance, pApdu);
            /* Config must be sent again */
            pInstance->sendConfigAgain = TRUE;      
        }
        else if(prstChoiceType == RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN)
        {
            returnEvent = CsrBtPhdcHandleConfirmedEventReport(pInstance, pApdu);
        }     
    }
    else if(pApdu->choice == RLRQ_CHOSEN)
    {
        CsrBtPhdc20601RlreEncodeSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                    pInstance->assocChannelId, 
                                    RELEASE_RESPONSE_REASON_NORMAL);
        if(pApdu->u.rlrq.reason == RELEASE_REQUEST_REASON_NO_MORE_CONFIG)
        {
            pInstance->associationResult = CSR_BT_PHDC_AG_RESULT_CODE_FAILURE_NO_MORE_CONFIG;
        }
        else
        {
            pInstance->associationResult = CSR_BT_PHDC_AG_RESULT_CODE_FAILURE;
        }   
        returnEvent = PHDC_AG_DEVICE_EV_INT_DATA_IND_RLRQ;      
    }
    else 
    {
        returnEvent = CsrBtPhdcAgErrorHandler(pInstance, pApdu);
    }
    
    CsrBtPhdcFreeDecodeAnyApdu(pApdu);

    return returnEvent;
    
}


FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_OperatingPhdcDataReq)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtPhdcAgDataReq *pPhdcAgDataReq;
    APDU *pRoivCmfdEventReportApdu = NULL;
    EventReportArgumentSimple *pRoivCmipConfirmedEventReport = NULL; 
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pPhdcAgDataReq = (CsrBtPhdcAgDataReq *)pInstance->pRecvMsg;
    
    if(pInstance->waitingForDataCfm == TRUE)
    {        
        CsrBtPhdcAgDataCfmSend(pInstance->appHandle,pInstance->assocChannelId,
            CSR_BT_PHDC_AG_RESULT_CODE_BUSY);

        pInstance->waitingForDataCfm = FALSE;
        CsrPmemFree(pPhdcAgDataReq->data.data);
        pPhdcAgDataReq->data.data = NULL;
        return returnEvent;        
    }

    if(!CsrBtPhdcAgIsValidObjectHandle(pPhdcAgDataReq->data.objHandle,pInstance))
    {
        CsrBtPhdcAgDataCfmSend(pInstance->appHandle,pInstance->assocChannelId,
            CSR_BT_PHDC_AG_RESULT_CODE_INVALID_OBJECT_HANDLE);
        CsrPmemFree(pPhdcAgDataReq->data.data);
        pPhdcAgDataReq->data.data = NULL;
        return returnEvent;        
    }

    if(pInstance->pRoivCmfdEventReport == NULL)
    {
        /* Allocate memory in DATA_apdu for invoke_id(2 Bytes),choice(2 Bytes), length (2 Bytes)and
        EventReportArgumentSimple */
        pInstance->pRoivCmfdEventReport = (DATA_apdu *)CsrPmemAlloc(sizeof(DATA_apdu));
        pInstance->pRoivCmfdEventReport->invoke_id = pInstance->invokeId++;
        pInstance->pRoivCmfdEventReport->choice.choice = ROIV_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN;
        pRoivCmipConfirmedEventReport = &pInstance->pRoivCmfdEventReport->choice.u.roiv_cmipConfirmedEventReport;
        CsrBtPhdcAgFormRoivCmipConfirmedEventReport(pRoivCmipConfirmedEventReport,pInstance,&pPhdcAgDataReq->data);

        pRoivCmfdEventReportApdu = (APDU *)CsrPmemAlloc(sizeof(APDU));
        pRoivCmfdEventReportApdu->choice = PRST_CHOSEN;        
    }

    pInstance->transmitFlag = pPhdcAgDataReq->transmitFlag;


    if (pRoivCmipConfirmedEventReport)
    {/* call function */
        CsrBtPhdcAgHandlePhdcDataReq(pInstance, pRoivCmfdEventReportApdu, pRoivCmipConfirmedEventReport);
    }
            
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_OperatingHdpDataInd)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpDataInd *pHdpDataInd;
    APDU *pApdu;
    DATA_apdu *pDATA_apdu;
    EventReportResultSimple *pRorsCmipConfirmedEventReport;
    
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pHdpDataInd = (CsrBtHdpDataInd *)pInstance->pRecvMsg;

    CsrBtPhdcMderDecodeApdu(pHdpDataInd->data,&pApdu);
    CsrPmemFree(pHdpDataInd->data);    

    if(pApdu->choice == PRST_CHOSEN)
    {
        pDATA_apdu = (DATA_apdu *)pApdu->u.prst.value;
        if(pDATA_apdu->choice.choice == RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN)
        {
            if(pDATA_apdu->invoke_id == (pInstance->invokeId - 1))  /* Match the outstanding invoke id */
            {
                pRorsCmipConfirmedEventReport = (EventReportResultSimple *)&pDATA_apdu->choice.u.rors_cmipConfirmedEventReport;

                if((pRorsCmipConfirmedEventReport->event_type == MDC_NOTI_SCAN_REPORT_VAR) && 
                    (pInstance->localUnitCodeReq == TRUE))
                {
                    CsrBtPhdcAgChangeDimUnitCodeCfmSend(pInstance->appHandle,pInstance->localUnitCodeReqObjHandle,
                        CSR_BT_PHDC_AG_RESULT_CODE_SUCCESS);
                    pInstance->localUnitCodeReq = FALSE;
                    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);
                }                
                else if((pRorsCmipConfirmedEventReport->event_type == MDC_NOTI_SCAN_REPORT_FIXED)   ||
                        (pRorsCmipConfirmedEventReport->event_type == MDC_NOTI_SCAN_REPORT_VAR)     ||
                        (pRorsCmipConfirmedEventReport->event_type == MDC_NOTI_SCAN_REPORT_MP_FIXED)||
                        (pRorsCmipConfirmedEventReport->event_type == MDC_NOTI_SCAN_REPORT_MP_VAR))
                {
                    CsrBtPhdcAgDataCfmSend(pInstance->appHandle,
                                           pInstance->assocChannelId, 
                                           CSR_BT_PHDC_AG_RESULT_CODE_SUCCESS);
                    pInstance->waitingForDataCfm = FALSE;

                    if(pInstance->scanReportNo != CSR_BT_PHDC_AG_MAX_SCAN_REPORTS)
                    {
                        pInstance->scanReportNo++;
                    }
                    else
                    {
                        pInstance->scanReportNo = 0;
                    }
                    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);
                }
            }
        }
        else if(pDATA_apdu->choice.choice == ROIV_CMIP_GET_CHOSEN)
        {
            if (pDATA_apdu->choice.u.roiv_cmipGet.attribute_id_list.count == 0 ||
                pDATA_apdu->choice.u.roiv_cmipGet.attribute_id_list.length == 0)
            {
                CsrBtPhdcSendGetResponse(pInstance, pApdu);
            }
            else
            {
                CsrBtPhdc20601RoerEncodeSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                             pInstance->assocChannelId,
                                             pDATA_apdu->invoke_id,
                                             NOT_ALLOWED_BY_OBJECT);
            }
        }
        else if(pDATA_apdu->choice.choice == ROIV_CMIP_CONFIRMED_ACTION_CHOSEN ||
                pDATA_apdu->choice.choice == ROIV_CMIP_SET_CHOSEN)
        {
            CsrBtPhdc20601RoerEncodeSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                         pInstance->assocChannelId,
                                         pDATA_apdu->invoke_id,
                                         NO_SUCH_ACTION);
        }
    }
    else if(pApdu->choice == RLRQ_CHOSEN)
    {
        CsrBtPhdc20601RlreEncodeSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                    pInstance->assocChannelId, 
                                    RELEASE_RESPONSE_REASON_NORMAL);
        returnEvent = PHDC_AG_DEVICE_EV_INT_DATA_IND_RLRQ;
    }
    else
    {
        returnEvent = CsrBtPhdcAgErrorHandler(pInstance, pApdu);
    }
    CsrBtPhdcFreeDecodeAnyApdu(pApdu);


    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_OtherHdpDisconnectAssociateChannelInd)
{
    CsrBtPhdcAgentInstance *pInstance;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;

    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);

    CsrBtPhdcAgDisassociateIndSend(pInstance->appHandle,
                                   pInstance->dataChannelId, 
                                   pInstance->local_bd_addr);

    CsrBtPhdcAgBtLinkRelIndSend(pInstance->appHandle, 
                                pInstance->local_bd_addr, 
                                CSR_BT_PHDC_AG_RESULT_CODE_SUCCESS, 
                                CSR_BT_SUPPLIER_PHDC_AG );

    return returnEvent;
}


FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_OperatingTimerToMdsExpiry)
{
    CsrBtPhdcAgentInstance *pInstance;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;

    CsrBtPhdc20601AbrtEncodeSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                 pInstance->assocChannelId, 
                                 ABORT_REASON_RESPONSE_TIMEOUT);
    
    return returnEvent;
}


FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_OperatingHdpDisconnectAssociateChannelInd)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpDisconnectAssociateChannelInd *pDisconnectInd;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);
    pDisconnectInd = (CsrBtHdpDisconnectAssociateChannelInd *)pInstance->pRecvMsg;    

    CsrBtPhdcAgSuspendIndSend(pInstance->appHandle,pDisconnectInd->assocChId);
    return returnEvent;
}



FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_OperatingPhdcChangeUnitCodeReq)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtPhdcAgChangeDimUnitCodeReq *pChangeDimUnitCodeReq;
    EventReportArgumentSimple *pRoivCmipConfirmedEventReport;
    APDU *pApdu;
    DATA_apdu *pDataApdu;
    ScanReportInfoVar scanReportInfoVar;
    CsrUint16 encodedMsgLen;
    CsrUint8 *msgPtr;

    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pChangeDimUnitCodeReq = (CsrBtPhdcAgChangeDimUnitCodeReq *)pInstance->pRecvMsg;

    pInstance->localUnitCodeReq = TRUE;
    pInstance->localUnitCodeReqObjHandle = pChangeDimUnitCodeReq->objHandle;

    /* Allocate memory in DATA_apdu for invoke_id(2 Bytes),choice(2 Bytes), length (2 Bytes)and
    EventReportArgumentSimple */
    pDataApdu = (DATA_apdu *)CsrPmemAlloc(sizeof(DATA_apdu));

    pDataApdu->invoke_id = pInstance->invokeId++;

    pDataApdu->choice.choice = ROIV_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN;
    pRoivCmipConfirmedEventReport = &pDataApdu->choice.u.roiv_cmipConfirmedEventReport;

    pRoivCmipConfirmedEventReport->obj_handle = CSR_BT_PHDC_AG_MDS_OBJECT_HANDLE;
    pRoivCmipConfirmedEventReport->event_time = 0;
    pRoivCmipConfirmedEventReport->event_type = MDC_NOTI_SCAN_REPORT_VAR;

    /* Fill ScanReportInfoVar */
    scanReportInfoVar.data_req_id = DATA_REQ_ID_AGENT_INITIATED;
    scanReportInfoVar.scan_report_no = pInstance->scanReportNo++;
    scanReportInfoVar.obs_scan_var.count = 1;

    scanReportInfoVar.obs_scan_var.value = (ObservationScan *)CsrPmemAlloc(sizeof(ObservationScan)*scanReportInfoVar.obs_scan_var.count);
    scanReportInfoVar.obs_scan_var.value->obj_handle = pChangeDimUnitCodeReq->objHandle;
    scanReportInfoVar.obs_scan_var.value->attributes.count = 1;

    scanReportInfoVar.obs_scan_var.value->attributes.value = (AVA_Type *)CsrPmemAlloc(sizeof(AVA_Type)*scanReportInfoVar.obs_scan_var.value->attributes.count);
    scanReportInfoVar.obs_scan_var.value->attributes.value->attribute_id = MDC_ATTR_UNIT_CODE;
    scanReportInfoVar.obs_scan_var.value->attributes.value->attribute_value.length = sizeof(pChangeDimUnitCodeReq->unitCode); /* Length of unit code */

    scanReportInfoVar.obs_scan_var.value->attributes.value->attribute_value.value = CsrPmemAlloc(scanReportInfoVar.obs_scan_var.value->attributes.value->attribute_value.length);
    scanReportInfoVar.obs_scan_var.value->attributes.value->attribute_value.value[0] = (pChangeDimUnitCodeReq->unitCode & 0xff00)>>8;
    scanReportInfoVar.obs_scan_var.value->attributes.value->attribute_value.value[1] = (pChangeDimUnitCodeReq->unitCode & 0x00ff);

    pRoivCmipConfirmedEventReport->event_info.value = (CsrUint8 *)&scanReportInfoVar;

    pApdu = (APDU *)CsrPmemAlloc(sizeof(APDU));
    pApdu->choice = PRST_CHOSEN;
    pApdu->u.prst.value = (CsrUint8 *)pDataApdu;

    encodedMsgLen = CsrBtPhdcMderCalculateApduEncodedLength(pApdu);
    msgPtr = CsrPmemAlloc(encodedMsgLen*sizeof(CsrUint8));
    CsrBtPhdcMderEncodeApdu(pApdu, msgPtr);

    CsrBtHdpDataReqSend(CSR_BT_PHDC_AG_IFACEQUEUE,pInstance->assocChannelId,msgPtr,encodedMsgLen);

    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);
    pInstance->timerId = CsrSchedTimerSet(CSR_BT_PHDC_TO_CER_MDS, 
                                          CsrBtPhdcAgentTimedEvent, 
                                          PHDC_AG_DEVICE_EV_TIMER_TO_CER_MDS_EXPIRY, 
                                          (void *) pInstance);
    
    CsrPmemFree(scanReportInfoVar.obs_scan_var.value->attributes.value->attribute_value.value);
    CsrPmemFree(scanReportInfoVar.obs_scan_var.value->attributes.value);
    CsrPmemFree(scanReportInfoVar.obs_scan_var.value);
    CsrPmemFree(pDataApdu);
    CsrPmemFree(pApdu);

    return returnEvent; 
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_OperationPhdcDisassociateReq)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtPhdcAgDisassociateReq *pAgDisassociateReq;

    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pAgDisassociateReq = (CsrBtPhdcAgDisassociateReq *)pInstance->pRecvMsg;

    pInstance->localDisassociateReq = TRUE;

    CsrBtPhdc20601RlrqEncodeSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                 pInstance->assocChannelId,
                                 pAgDisassociateReq->rlrqReason);

    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);
    pInstance->timerId = CsrSchedTimerSet(CSR_BT_PHDC_TO_RELEASE, 
                                          CsrBtPhdcAgentTimedEvent, 
                                          PHDC_AG_DEVICE_EV_TIMER_TO_RELEASE_EXPIRY, 
                                          (void *) pv_arg1);

    return returnEvent;  
}


FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_OperatingPhdcSuspendReq)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtPhdcAgSuspendReq *pAgSuspendReq;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pAgSuspendReq = (CsrBtPhdcAgSuspendReq *)pInstance->pRecvMsg;

    pInstance->assocChannelId = pAgSuspendReq->dataChannelId;

    CsrBtHdpSuspendReqSend(CSR_BT_PHDC_AG_IFACEQUEUE,pAgSuspendReq->dataChannelId);

    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_SuspendHdpSuspendCfm)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpSuspendCfm *pHdpSuspendCfm;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pHdpSuspendCfm = (CsrBtHdpSuspendCfm *)pInstance->pRecvMsg;

    CsrBtPhdcAgSuspendCfmSend(pInstance->appHandle,
                              pHdpSuspendCfm->assocChId,
                              pHdpSuspendCfm->resultCode,
                              pHdpSuspendCfm->resultSupplier);

    return returnEvent;

}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_SuspendPhdcResumeReq)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtPhdcAgResumeReq *pAgResumeReq;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pAgResumeReq = (CsrBtPhdcAgResumeReq *)pInstance->pRecvMsg;

    CsrBtHdpResumeReqSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                          pAgResumeReq->dataChannelId);

    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_SuspendHdpResumeCfm)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpResumeCfm *pHdpResumeCfm;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pHdpResumeCfm = (CsrBtHdpResumeCfm *)pInstance->pRecvMsg;

    pInstance->assocChannelId = pHdpResumeCfm->assocChId;

    CsrBtPhdcResumeCfmSend(pInstance->appHandle,
                           pHdpResumeCfm->assocChId,
                           pHdpResumeCfm->resultCode,
                           pHdpResumeCfm->resultSupplier);

    return returnEvent;    
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_SuspendHdpResumeInd)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpResumeInd *pHdpResumeInd;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pHdpResumeInd = (CsrBtHdpResumeInd *)pInstance->pRecvMsg;

    pInstance->assocChannelId = pHdpResumeInd->chId;

    CsrBtPhdcResumeIndSend(pInstance->appHandle,
                           pHdpResumeInd->chId,
                           pHdpResumeInd->resultCode,
                           pHdpResumeInd->resultSupplier);

    return returnEvent;    
}


FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_DisassociatingToReleaseExpiry)
{
    CsrBtPhdcAgentInstance *pInstance;

    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    CsrBtPhdc20601AbrtEncodeSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                 pInstance->assocChannelId,
                                 ABORT_REASON_RESPONSE_TIMEOUT);

    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_DisassociatingHdpDataInd)
{
    CsrBtPhdcAgentInstance *pInstance;
    CsrBtHdpDataInd *pHdpDataInd;
    APDU *apdu;

    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    pHdpDataInd = (CsrBtHdpDataInd *)pInstance->pRecvMsg;

    CsrBtPhdcMderDecodeApdu(pHdpDataInd->data,&apdu);
    CsrPmemFree(pHdpDataInd->data);

    if(apdu->choice == RLRE_CHOSEN)
    {
        CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);
        if(pInstance->associateReq)
        {
            CsrBtPhdcAgAssociateCfmSend(pInstance->appHandle,
                                    pInstance->psmIdentifier, 
                                    pInstance->local_bd_addr, 
                                    pInstance->mdepId, 
                                    pInstance->mdepDataType, 
                                    pInstance->maxPacketLength, 
                                    pInstance->assocChannelId, 
                                    pInstance->associationResult);   
            pInstance->associateReq = FALSE;
        }
        else if(pInstance->localDisassociateReq)
        {
            CsrBtPhdcAgDisassociateCfmSend(pInstance->appHandle,
                                           pInstance->dataChannelId, 
                                           pInstance->local_bd_addr,
                                           CSR_BT_PHDC_AG_RESULT_CODE_SUCCESS,
                                           CSR_BT_SUPPLIER_PHDC_AG );
            pInstance->localDisassociateReq = FALSE;
        }
        returnEvent = PHDC_AG_DEVICE_EV_INT_DATA_IND_RLRE;
        pInstance->timerId = CsrSchedTimerSet(CSR_BT_PHDC_BT_LINK_RELEASE, 
                                                CsrBtPhdcAgentTimedEvent, 
                                                PHDC_AG_DEVICE_EV_TIMER_BT_LINK_REL_EXPIRY, 
                                                (void *) pInstance);
        
    }
    else if(apdu->choice == RLRQ_CHOSEN)
    {
        CsrBtPhdc20601RlreEncodeSend(CSR_BT_PHDC_AG_IFACEQUEUE,
                                     pInstance->assocChannelId, 
                                     RELEASE_RESPONSE_REASON_NORMAL);
        /* already disassociating - wait for cfm */
    }
    CsrBtPhdcFreeDecodeAnyApdu(apdu);

    return returnEvent;
}


FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_DisassociatingHdpDataCfm)
{
    CsrBtPhdcAgentInstance *pInstance;

    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;
    

    if(!pInstance->localDisassociateReq && !pInstance->associateReq)
    {
        CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);        
        CsrBtPhdcAgDisassociateIndSend(pInstance->appHandle,
                                       pInstance->dataChannelId, 
                                       pInstance->local_bd_addr);
        
        pInstance->timerId = CsrSchedTimerSet(CSR_BT_PHDC_BT_LINK_RELEASE, 
                                              CsrBtPhdcAgentTimedEvent, 
                                              PHDC_AG_DEVICE_EV_TIMER_BT_LINK_REL_EXPIRY, 
                                              (void *) pInstance);
        returnEvent = PHDC_AG_DEVICE_EV_INT_DATA_IND_RLRE;
    }
    else if(pInstance->associateReq)
    {
        CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);       
        if(!pInstance->localDisassociateReq)
        {
            returnEvent = PHDC_AG_DEVICE_EV_INT_DATA_IND_RLRE;
            CsrBtPhdcAgAssociateCfmSend(pInstance->appHandle,
                                        pInstance->psmIdentifier, 
                                        pInstance->local_bd_addr, 
                                        pInstance->mdepId, 
                                        pInstance->mdepDataType, 
                                        pInstance->maxPacketLength, 
                                        pInstance->assocChannelId, 
                                        pInstance->associationResult);   
            pInstance->associateReq = FALSE;
        }
        else
        {
            pInstance->localDisassociateReq = FALSE;
        }
        pInstance->timerId = CsrSchedTimerSet(CSR_BT_PHDC_BT_LINK_RELEASE, 
                                              CsrBtPhdcAgentTimedEvent, 
                                              PHDC_AG_DEVICE_EV_TIMER_BT_LINK_REL_EXPIRY, 
                                              (void *) pInstance);
    }
        
    return returnEvent;
}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_DisconnectingHdpDisconnectAssociateChannelCfm)
{
    CsrBtPhdcAgentInstance *pInstance;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;

    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);

    CsrBtPhdcAgBtLinkRelIndSend(pInstance->appHandle, 
                                pInstance->local_bd_addr, 
                                CSR_BT_PHDC_AG_RESULT_CODE_SUCCESS,
                                CSR_BT_SUPPLIER_PHDC_AG );

    return returnEvent;    

}

FSM_ACTION_FN(CsrBtPhdcAgDeviceFsm_AbortCfmHdpDataCfm)
{
    CsrBtPhdcAgentInstance *pInstance;
    fsm_event_t returnEvent = FSM_EVENT_NULL;

    CSR_BT_PHDC_AG_ASSERT(pv_arg1 != NULL);

    pInstance = (CsrBtPhdcAgentInstance *) pv_arg1;

    if(pInstance->associateReq)
    {
        CsrBtPhdcAgAssociateCfmSend(pInstance->appHandle,
                                    pInstance->psmIdentifier, 
                                    pInstance->local_bd_addr, 
                                    pInstance->mdepId, 
                                    pInstance->mdepDataType, 
                                    pInstance->maxPacketLength, 
                                    pInstance->assocChannelId, 
                                    pInstance->associationResult);   
        pInstance->associateReq = FALSE;
    }
    else
    {
        if(pInstance->waitingForDataCfm)
        {
            CsrBtPhdcAgDataCfmSend(pInstance->appHandle,
                                   pInstance->assocChannelId, 
                                   CSR_BT_PHDC_AG_RESULT_CODE_FAILURE);
            pInstance->waitingForDataCfm = FALSE;
        }
            
        if(pInstance->localDisassociateReq)
        {
            pInstance->localDisassociateReq = FALSE;
            CsrBtPhdcAgDisassociateCfmSend(pInstance->appHandle,
                                           pInstance->dataChannelId, 
                                           pInstance->local_bd_addr,
                                           CSR_BT_PHDC_AG_RESULT_CODE_ABORT_REASON_RESPONSE_TMEOUT,
                                           CSR_BT_SUPPLIER_PHDC_AG );
        }
        else
        {
            CsrBtPhdcAgDisassociateIndSend(pInstance->appHandle,
                                           pInstance->dataChannelId,
                                           pInstance->local_bd_addr);
        }
    }
    CsrSchedTimerCancel(pInstance->timerId, NULL, NULL);
    pInstance->timerId = CsrSchedTimerSet(CSR_BT_PHDC_BT_LINK_RELEASE, 
                                          CsrBtPhdcAgentTimedEvent, 
                                          PHDC_AG_DEVICE_EV_TIMER_BT_LINK_REL_EXPIRY, 
                                          (void *) pInstance);

    return returnEvent;
}



#endif
