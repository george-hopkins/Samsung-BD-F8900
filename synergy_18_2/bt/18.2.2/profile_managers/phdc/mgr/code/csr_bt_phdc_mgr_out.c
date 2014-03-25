/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_MODULE

#include "csr_bt_phdc_mgr_out.h"
#include "csr_pmem.h"
#include "csr_bt_hdp_lib.h"
#include "csr_bt_util.h"

static void csrBtPhdcMgrMessagePut(CsrSchedQid phandle, void *msg)
{
    CsrSchedMessagePut(phandle, CSR_BT_PHDC_MGR_PRIM, msg);
}

void CsrBtPhdcMgrConfigureEndpointCfmSend(CsrSchedQid qId,
                                   CsrBtPhdcMgrResult   resultCode,
                                   CsrBtSupplier     resultSupplier)
{
    CsrBtPhdcMgrConfigureEndpointCfm *pCfm = CsrPmemAlloc(sizeof(*pCfm));

    pCfm->type = CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_CFM;

    pCfm->resultCode = resultCode;
    pCfm->resultSupplier = resultSupplier;

    csrBtPhdcMgrMessagePut(qId,pCfm);
}

void CsrBtPhdcMgrConfigureEndpointIndSend(CsrSchedQid qId, CsrBtMdepId mdepId)
{

    CsrBtPhdcMgrConfigureEndpointInd *pInd = CsrPmemAlloc(sizeof(CsrBtPhdcMgrConfigureEndpointInd));

    pInd->type = CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_IND;
    pInd->mdepId = mdepId;

    csrBtPhdcMgrMessagePut(qId, pInd);
}


void CsrBtPhdcMgrSendAcceptBtLinkIndSend(CsrSchedQid qId,
                                     CsrBtDeviceAddr  deviceAddr)
{
    CsrBtPhdcMgrAcceptBtLinkInd    *pInd = CsrPmemAlloc(sizeof(*pInd));

    pInd->type = CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_IND;
    pInd->deviceAddr = deviceAddr;

    csrBtPhdcMgrMessagePut(qId,pInd);
}


void CsrBtPhdcMgrAcceptAssociateIndSend(CsrSchedQid qId,
                                    CsrUint16         devConfigId,
                                    CsrUint8          *systemId,
                                    CsrBtDeviceAddr   *pDeviceAddr)
{
    CsrBtPhdcMgrAcceptAssociateInd   *pInd = CsrPmemAlloc(sizeof(*pInd));

    pInd->type = CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_IND;

    pInd->devConfigId = devConfigId;
    CsrMemCpy(pInd->systemId,systemId,8);
    pInd->deviceAddr  = *pDeviceAddr;

    csrBtPhdcMgrMessagePut(qId,pInd);
}


void CsrBtPhdcMgrExtConfigObjIndSend(CsrSchedQid qId,
                                    CsrUint16         devConfigId,
                                    CsrBtDeviceAddr   *pDeviceAddr,
                                    CsrUint16         objHandle,
                                    CsrUint16         dataLength,
                                    CsrUint8          *data,
                                    CsrUint16         opCode)
{
    CsrBtPhdcMgrExtConfigObjInd   *pInd = CsrPmemAlloc(sizeof(*pInd));

    pInd->type = CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND;

    pInd->devConfigId   = devConfigId;
    pInd->deviceAddr    = *pDeviceAddr;
    pInd->objHandle     = objHandle;
    pInd->opCode        = opCode;
    pInd->dataLength    = dataLength;
    pInd->data          = data;

    csrBtPhdcMgrMessagePut(qId,pInd);
}


void CsrBtPhdcMgrSendAssociateCompleteIndSend(CsrSchedQid qId,
                                          CsrUint32 psmIdentifier  ,
                                          CsrBtDeviceAddr  *pDeviceAddr,
                                          CsrBtMdepId       mdepId,
                                          CsrUint16         mdepDataType,
                                          CsrUint16         maxPacketLength,
                                          CsrUint32         dataChannelId,
                                          CsrBtPhdcMgrResult   resultCode,
                                          CsrBtSupplier     resultSupplier)
{
    CsrBtPhdcMgrAssociateCompleteInd    *pInd = CsrPmemAlloc(sizeof(*pInd));

    pInd->type = CSR_BT_PHDC_MGR_ASSOCIATE_COMPLETE_IND;

    if ( pDeviceAddr != NULL)
    {
        pInd->deviceAddr = *pDeviceAddr;
    }
    else
    {
        CsrBtBdAddrZero(&pInd->deviceAddr);
    }

    pInd->psmIdentifier = psmIdentifier;

    pInd->mdepId = mdepId;
    pInd->mdepDataType = mdepDataType;
    pInd->maxPacketLength = maxPacketLength;
    pInd->dataChannelId = dataChannelId;
    pInd->resultCode = resultCode;
    pInd->resultSupplier = resultSupplier;

    csrBtPhdcMgrMessagePut(qId,pInd);
}

void CsrBtPhdcMgrDisassociateIndSend(CsrSchedQid qId,
                                    CsrUint32         dataChannelId,
                                    CsrBtDeviceAddr   *pDeviceAddr,
                                    CsrBtResultCode   resultCode,
                                    CsrBtSupplier     resultSupplier)
{
    CsrBtPhdcMgrDisassociateInd   *pInd = CsrPmemAlloc(sizeof(*pInd));

    pInd->type = CSR_BT_PHDC_MGR_DISASSOCIATE_IND;

    pInd->dataChannelId = dataChannelId;
    pInd->deviceAddr = *pDeviceAddr;
    pInd->resultCode = resultCode;
    pInd->resultSupplier = resultSupplier;

    csrBtPhdcMgrMessagePut(qId,pInd);
}

void CsrBtPhdcMgrDisassociateCfmSend(CsrSchedQid qId,
                                    CsrUint32         dataChannelId,
                                    CsrBtDeviceAddr   *pDeviceAddr,
                                    CsrBtResultCode   resultCode,
                                    CsrBtSupplier     resultSupplier)
{
    CsrBtPhdcMgrDisassociateCfm   *pCfm = CsrPmemAlloc(sizeof(*pCfm));

    pCfm->type = CSR_BT_PHDC_MGR_DISASSOCIATE_CFM;

    pCfm->dataChannelId = dataChannelId;
    if ( pDeviceAddr != NULL )
    {
        pCfm->deviceAddr = *pDeviceAddr;
    }
    else
    {
        CsrBtBdAddrZero(&pCfm->deviceAddr);
    }
    pCfm->resultCode = resultCode;
    pCfm->resultSupplier = resultSupplier;

    csrBtPhdcMgrMessagePut(qId,pCfm);
}

void CsrBtPhdcMgrBpmBpIndSend(CsrSchedQid               qId,
                              CsrUint32                 dataChannelId,
                              CsrUint16                 personId,
                              CsrUint16                 unitCode,
                              CsrBtPhdcMgrAbsoluteTim   *pTime,
                              CsrUint16                 systolicPressure,
                              CsrUint16                 diastolicPressure,
                              CsrUint16                 meanArterialPressure)
{
    CsrBtPhdcMgrBpmBpInd   *pInd = CsrPmemAlloc(sizeof(*pInd));

    pInd->type = CSR_BT_PHDC_MGR_BPM_BP_IND;

    pInd->dataChannelId = dataChannelId;
    pInd->personId = personId;
    pInd->unitCode = unitCode;
    pInd->time = *pTime;
    pInd->systolicPressure = systolicPressure;
    pInd->diastolicPressure = diastolicPressure;
    pInd->meanArterialPressure = meanArterialPressure;

    csrBtPhdcMgrMessagePut(qId,pInd);
}


void CsrBtPhdcMgrBpmPulseIndSend(CsrSchedQid                qId,
                                 CsrUint32                  dataChannelId,
                                 CsrUint16                  personId,
                                 CsrUint16                  unitCode,
                                 CsrBtPhdcMgrAbsoluteTim   *pTime,
                                 CsrUint16                  Pulse)
{
    CsrBtPhdcMgrBpmPulseInd   *pInd = CsrPmemAlloc(sizeof(*pInd));

    pInd->type = CSR_BT_PHDC_MGR_BPM_PULSE_IND;

    pInd->dataChannelId = dataChannelId;
    pInd->personId = personId;
    pInd->unitCode = unitCode;
    pInd->time = *pTime;
    pInd->pulse = Pulse;


    csrBtPhdcMgrMessagePut(qId,pInd);
}

void CsrBtPhdcMgrWsWeightIndSend(CsrSchedQid                qId,
                                 CsrUint32                  dataChannelId,
                                 CsrUint16                  personId,
                                 CsrUint16                  unitCode,
                                 CsrBtPhdcMgrAbsoluteTim   *pTime,
                                 CsrUint32                  weight)
{
    CsrBtPhdcMgrWsWeightInd   *pInd = CsrPmemAlloc(sizeof(*pInd));

    pInd->type = CSR_BT_PHDC_MGR_WS_WEIGHT_IND;

    pInd->dataChannelId = dataChannelId;
    pInd->personId = personId;
    pInd->unitCode= unitCode;
    pInd->time = *pTime;
    pInd->weight = weight;

    csrBtPhdcMgrMessagePut(qId,pInd);
}

void CsrBtPhdcMgrDsIndSend(CsrSchedQid                  qId,
                             CsrUint32                  dataChannelId,
                             CsrUint16                  objHandle,
                             CsrUint16                  personId,
                             CsrBtPhdcScanReportType    scanReportType,
                             CsrUint16                  dataLength,
                             CsrUint8                   *data)
{
    CsrBtPhdcMgrDsInd   *pInd = CsrPmemAlloc(sizeof(*pInd));

    pInd->type = CSR_BT_PHDC_MGR_DS_IND;

    pInd->dataChannelId = dataChannelId;
    pInd->objHandle = objHandle;
    pInd->personId = personId;
    pInd->scanReportType = scanReportType;
    pInd->dataLength = dataLength;
    pInd->data = data;


    csrBtPhdcMgrMessagePut(qId,pInd);
}

void CsrBtPhdcMgrBtLinkRelIndSend(CsrSchedQid qId,
                             CsrUint32         dataChannelId)
{
    CsrBtPhdcMgrBtLinkRelInd   *pInd = CsrPmemAlloc(sizeof(*pInd));

    pInd->type = CSR_BT_PHDC_MGR_BT_LINK_REL_IND;

    pInd->dataChannelId = dataChannelId;

    csrBtPhdcMgrMessagePut(qId,pInd);
}

void CsrBtPhdcMgrGetDeviceCapabCfmSend(CsrSchedQid          qId,
                                       CsrBtDeviceAddr      *pDeviceAddr,
                                       CsrBtPhdcMgrResult   resultCode,
                                       CsrBtSupplier        resultSupplier)
{
    CsrBtPhdcMgrGetDeviceCapabCfm *pCfm = CsrPmemAlloc(sizeof(*pCfm));

    pCfm->type = CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_CFM;

    if ( pDeviceAddr != NULL )
    {
        pCfm->deviceAddr = *pDeviceAddr;
    }
    else
    {
        CsrBtBdAddrZero(&pCfm->deviceAddr);
    }

    pCfm->resultCode = resultCode;
    pCfm->resultSupplier = resultSupplier;


    csrBtPhdcMgrMessagePut(qId,pCfm);
}

void CsrBtPhdcMgrGetDeviceCapabIndSend(CsrSchedQid      appHandle,
                                      CsrBtDeviceAddr   *pDeviceAddr,
                                      CsrUint32         psmIdentifier,
                                      CsrCharString     *serviceName,
                                      CsrCharString     *providerName,
                                      CsrUint8          supportedFeatureListLen,
                                      CsrBtHdpMdep      *supportedFeatureList)
{
    CsrBtPhdcMgrGetDeviceCapabInd *pMsg = CsrPmemAlloc(sizeof(*pMsg));

    pMsg->type = CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND;
    pMsg->deviceAddr=  *pDeviceAddr;
    pMsg->psmIdentifier = psmIdentifier;
    pMsg->serviceName = serviceName;
    pMsg->providerName = providerName;
    pMsg->supportedFeatureListLength = supportedFeatureListLen;

    pMsg->supportedFeatureList = supportedFeatureList;

    csrBtPhdcMgrMessagePut(appHandle, pMsg);

}

void CsrBtPhdcMgrSetupBtLinkCfmSend(CsrSchedQid         qId,
                                    CsrUint32           psmIdentifier,
                                    CsrBtDeviceAddr     *pDeviceAddr,
                                    CsrBtMdepId         mdepId,
                                    CsrUint16           maxPacketLength,
                                    CsrUint32           dataChannelId,
                                    CsrBtPhdcMgrResult  resultCode,
                                    CsrBtSupplier       resultSupplier)
{
    CsrBtPhdcMgrSetupBtLinkCfm    *pCfm = CsrPmemAlloc(sizeof(*pCfm));

    pCfm->type = CSR_BT_PHDC_MGR_SETUP_BT_LINK_CFM;

    if ( pDeviceAddr != NULL)
    {
        pCfm->deviceAddr = *pDeviceAddr;
    }
    else
    {
        CsrBtBdAddrZero(&pCfm->deviceAddr);
    }

    pCfm->psmIdentifier = psmIdentifier;

    pCfm->mdepId = mdepId;
    pCfm->maxPacketLength = maxPacketLength;
    pCfm->dataChannelId = dataChannelId;
    pCfm->resultCode = resultCode;
    pCfm->resultSupplier = resultSupplier;

    csrBtPhdcMgrMessagePut(qId,pCfm);
}

void CsrBtPhdcMgrUnregisterCfmSend(CsrSchedQid      qId,
                                   CsrBtResultCode  resultCode,
                                   CsrBtSupplier    resultSupplier)
{
    CsrBtPhdcMgrUnregisterCfm   *pCfm = CsrPmemAlloc(sizeof(*pCfm));

    pCfm->type = CSR_BT_PHDC_MGR_UNREGISTER_CFM;

    pCfm->qId = qId;
    pCfm->resultCode = resultCode;
    pCfm->resultSupplier = resultSupplier;

    csrBtPhdcMgrMessagePut(qId,pCfm);
}



#endif


