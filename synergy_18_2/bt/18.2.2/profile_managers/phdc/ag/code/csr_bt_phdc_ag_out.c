/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_PHDC_AG_MODULE

#include "csr_bt_phdc_ag_out.h"
#include "csr_pmem.h"
#include "csr_bt_util.h"
#include "csr_bt_profiles.h"

static void csrBtPhdcAgMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_PHDC_AG_PRIM, msg);
}

void CsrBtPhdcAgConfigureEndpointIndSend(CsrSchedQid appHandle, CsrBtMdepId mdepId)
{

    CsrBtPhdcAgConfigureEndpointInd *pMsg = CsrPmemAlloc(sizeof(CsrBtPhdcAgConfigureEndpointInd));

    pMsg->type = CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_IND;
    pMsg->mdepId = mdepId;

    csrBtPhdcAgMessagePut(appHandle, pMsg);
}

void CsrBtPhdcAgConfigureEndpointCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier)
{

    CsrBtPhdcAgConfigureEndpointCfm *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_CFM;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;

    csrBtPhdcAgMessagePut(appHandle, pMsg);
}

void CsrBtPhdcAgDimObjectIndSend(CsrSchedQid appHandle,CsrUint16  objIdx)
{

    CsrBtPhdcAgDimObjectInd *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_DIM_OBJECT_IND;
    pMsg->objIndex = objIdx;

    csrBtPhdcAgMessagePut(appHandle, pMsg);
}

void CsrBtPhdcAgDimAttribIndSend(CsrSchedQid appHandle,CsrUint16  objIdx,CsrUint16  attribIdx)
{

    CsrBtPhdcAgDimAttribInd *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_DIM_ATTRIB_IND;
    pMsg->objIndex = objIdx;
    pMsg->attribIndex = attribIdx;

    csrBtPhdcAgMessagePut(appHandle, pMsg);
}

void CsrBtPhdcAgDimConfigCfmSend(CsrSchedQid appHandle,CsrBtPhdcAgResult resultCode)
{

    CsrBtPhdcAgDimConfigureCfm *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_DIM_CONFIGURE_CFM;
    pMsg->resultCode = resultCode;

    csrBtPhdcAgMessagePut(appHandle, pMsg);
}

void CsrBtPhdcAgGetDeviceCapabIndSend(CsrSchedQid appHandle,CsrBtDeviceAddr deviceAddr,CsrUint32 psmIdentifier,
                                    CsrCharString *serviceName,CsrCharString *providerName,
                                    CsrUint8 supportedFeatureListLen,CsrBtHdpMdep *supportedFeatureList)
{
    CsrBtPhdcAgGetDeviceCapabInd *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND;
    pMsg->deviceAddr = deviceAddr;
    pMsg->psmIdentifier = psmIdentifier;

    pMsg->serviceName = serviceName;
    pMsg->providerName = providerName;
    pMsg->supportedFeatureListLength = supportedFeatureListLen;
    pMsg->supportedFeatureList = supportedFeatureList;

    csrBtPhdcAgMessagePut(appHandle, pMsg);
}

void CsrBtHdpCtrlGetCapabCfmSend(CsrSchedQid appHandle,CsrBtDeviceAddr deviceAddr,CsrBtResultCode resultCode,CsrBtSupplier resultSupplier)
{
    CsrBtPhdcAgGetDeviceCapabCfm *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_CFM;
    CsrMemCpy(&pMsg->deviceAddr,&deviceAddr,sizeof(CsrBtDeviceAddr));
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;

    csrBtPhdcAgMessagePut(appHandle, pMsg);
}

void CsrBtPhdcAgAssociateCfmSend(CsrSchedQid appHandle,
                               CsrUint32         psmIdentifier,
                               CsrBtDeviceAddr   deviceAddr,
                               CsrBtMdepId       mdepId,
                               CsrUint16         mdepDataType,
                               CsrUint16         maxPacketLength,
                               CsrUint32         dataChannelId,
                               CsrBtPhdcAgResult   resultCode)
{
    CsrBtPhdcAgAssociateCfm *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_ASSOCIATE_CFM;
    pMsg->psmIdentifier = psmIdentifier;
    CsrMemCpy(&pMsg->deviceAddr,&deviceAddr,sizeof(CsrBtDeviceAddr));
    pMsg->mdepId = mdepId;
    pMsg->mdepDataType = mdepDataType;
    pMsg->maxPacketLength = maxPacketLength;
    pMsg->dataChannelId = dataChannelId;
    pMsg->resultCode = resultCode;

    csrBtPhdcAgMessagePut(appHandle, pMsg);
}

void CsrBtPhdcAgDataCfmSend(CsrSchedQid appHandle,
                            CsrUint32 dataChannelId,
                            CsrBtPhdcAgResult   resultCode)
{
    CsrBtPhdcAgDataCfm *pMsg =  CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_DATA_CFM;
    pMsg->dataChannelId = dataChannelId;
    pMsg->resultCode = resultCode;

    csrBtPhdcAgMessagePut(appHandle, pMsg);
}


void CsrBtPhdcAgSuspendCfmSend(CsrSchedQid appHandle,
                            CsrUint32 dataChannelId,
                            CsrBtResultCode   resultCode,
                            CsrBtSupplier     resultSupplier)
{
    CsrBtPhdcAgSuspendCfm *pMsg =  CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_SUSPEND_CFM;
    pMsg->dataChannelId = dataChannelId;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;

    csrBtPhdcAgMessagePut(appHandle, pMsg);
}

void CsrBtPhdcAgSuspendIndSend(CsrSchedQid appHandle,
                            CsrUint32 dataChannelId)

{
    CsrBtPhdcAgSuspendInd *pMsg =  CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_SUSPEND_IND;
    pMsg->dataChannelId = dataChannelId;

    csrBtPhdcAgMessagePut(appHandle, pMsg);
}


void CsrBtPhdcResumeCfmSend(CsrSchedQid appHandle,
                            CsrUint32 dataChannelId,
                            CsrBtResultCode   resultCode,
                            CsrBtSupplier     resultSupplier)
{
    CsrBtPhdcAgResumeCfm *pMsg =  CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_RESUME_CFM;
    pMsg->dataChannelId = dataChannelId;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;

    csrBtPhdcAgMessagePut(appHandle, pMsg);
}


void CsrBtPhdcResumeIndSend(CsrSchedQid appHandle,
                            CsrUint32 dataChannelId,
                            CsrBtResultCode   resultCode,
                            CsrBtSupplier     resultSupplier)
{
    CsrBtPhdcAgResumeInd *pMsg =  CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_RESUME_IND;
    pMsg->dataChannelId = dataChannelId;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;

    csrBtPhdcAgMessagePut(appHandle, pMsg);
}


void CsrBtPhdcAgDisassociateCfmSend(CsrSchedQid appHandle,
                            CsrUint32 dataChannelId,
                            CsrBtDeviceAddr   deviceAddr,
                            CsrBtResultCode   resultCode,
                            CsrBtSupplier     resultSupplier)
{
    CsrBtPhdcAgDisassociateCfm *pMsg =  CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_DISASSOCIATE_CFM;
    pMsg->dataChannelId = dataChannelId;
    CsrMemCpy(&pMsg->deviceAddr,&deviceAddr,sizeof(CsrBtDeviceAddr));
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;

    csrBtPhdcAgMessagePut(appHandle, pMsg);
}

void CsrBtPhdcAgDisassociateIndSend(CsrSchedQid appHandle,
                            CsrUint32 dataChannelId,
                            CsrBtDeviceAddr   deviceAddr)
{
    CsrBtPhdcAgDisassociateInd *pMsg =  CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_DISASSOCIATE_IND;
    pMsg->dataChannelId = dataChannelId;
    CsrMemCpy(&pMsg->deviceAddr,&deviceAddr,sizeof(CsrBtDeviceAddr));

    csrBtPhdcAgMessagePut(appHandle, pMsg);
}


void CsrBtPhdcAgChangeDimUnitCodeCfmSend(CsrSchedQid appHandle,
                                         CsrUint16         objHandle,
                                         CsrBtPhdcAgResult   resultCode)
{
    CsrBtPhdcAgChangeDimUnitCodeCfm *pMsg =  CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_CFM;
    pMsg->objHandle = objHandle;
    pMsg->resultCode = resultCode;


    csrBtPhdcAgMessagePut(appHandle, pMsg);
}

void CsrBtPhdcAgAcceptBtLinkIndSend(CsrSchedQid appHandle,CsrBtDeviceAddr deviceAddr)
{
    CsrBtPhdcAgAcceptBtLinkInd *pMsg =  CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_ACCEPT_BT_LINK_IND;
    CsrMemCpy(&pMsg->deviceAddr,&deviceAddr,sizeof(CsrBtDeviceAddr));

    csrBtPhdcAgMessagePut(appHandle, pMsg);
}

void CsrBtPhdcAgSetupBtLinkIndSend(CsrSchedQid appHandle,CsrBtDeviceAddr deviceAddr,
    CsrBtResultCode   resultCode,CsrBtSupplier     resultSupplier)
{
    CsrBtPhdcAgSetupBtLinkInd *pMsg =  CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_SETUP_BT_LINK_IND;
    CsrMemCpy(&pMsg->deviceAddr,&deviceAddr,sizeof(CsrBtDeviceAddr));
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    csrBtPhdcAgMessagePut(appHandle, pMsg);
}

void CsrBtPhdcAgBtLinkRelIndSend(CsrSchedQid appHandle,CsrBtDeviceAddr deviceAddr,
    CsrBtResultCode   resultCode,CsrBtSupplier     resultSupplier)                            
{
    CsrBtPhdcAgBtLinkRelInd *pMsg =  CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_BT_LINK_REL_IND;
    CsrMemCpy(&pMsg->deviceAddr,&deviceAddr,sizeof(CsrBtDeviceAddr));
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    csrBtPhdcAgMessagePut(appHandle, pMsg);    
}


void CsrBtPhdcAgUnregisterCfmSend(CsrSchedQid appHandle,
    CsrBtResultCode   resultCode,CsrBtSupplier     resultSupplier)
{
    CsrBtPhdcAgUnregisterCfm *pMsg =  CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_AG_UNREGISTER_CFM;
    pMsg->qId = appHandle;
    pMsg->resultCode = resultCode;
    pMsg->resultSupplier = resultSupplier;
    csrBtPhdcAgMessagePut(appHandle, pMsg);
}


#endif
