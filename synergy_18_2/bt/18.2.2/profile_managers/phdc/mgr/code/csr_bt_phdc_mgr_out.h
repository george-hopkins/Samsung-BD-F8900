#ifndef CSR_BT_PHDC_MGR_OUT_H__
#define CSR_BT_PHDC_MGR_OUT_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_types.h"
#include "csr_bt_hdp_prim.h"
#include "csr_bt_phdc_mgr_prim.h"
#include "csr_bt_result.h"
#include "csr_bt_profiles.h"


#include "csr_bt_phdc_ieee_endec.h"  /*could be renamed to MDER_endec.h */


#ifdef __cplusplus
extern "C" {
#endif

void CsrBtPhdcMgrConfigureEndpointCfmSend(CsrSchedQid           qId,
                                          CsrBtPhdcMgrResult    resultCode,
                                          CsrBtSupplier         resultSupplier);

void CsrBtPhdcMgrConfigureEndpointIndSend(CsrSchedQid           qId, 
                                          CsrBtMdepId           mdepId);

void CsrBtPhdcMgrSendAcceptBtLinkIndSend(CsrSchedQid            qId,
                                         CsrBtDeviceAddr        deviceAddr);


void CsrBtPhdcMgrSendAssociateCompleteIndSend(CsrSchedQid       qId,
                                              CsrUint32         psmIdentifier,
                                              CsrBtDeviceAddr  *pDeviceAddr,
                                              CsrBtMdepId       mdepId,
                                              CsrUint16         mdepDataType,
                                              CsrUint16         maxPacketLength,
                                              CsrUint32         dataChannelId,
                                              CsrBtPhdcMgrResult resultCode,
                                              CsrBtSupplier     resultSupplier);

void CsrBtPhdcMgrAcceptAssociateIndSend(CsrSchedQid         qId,
                                        CsrUint16           devConfigId,
                                        CsrUint8            systemId[8],
                                        CsrBtDeviceAddr     *pDeviceAddr);

void CsrBtPhdcMgrExtConfigObjIndSend(CsrSchedQid       qId,
                                     CsrUint16         devConfigId,
                                     CsrBtDeviceAddr   *pDeviceAddr,
                                     CsrUint16         objHandle,
                                     CsrUint16         dataLength,
                                     CsrUint8          *data,
                                     CsrUint16         opCode);

void CsrBtPhdcMgrDisassociateIndSend(CsrSchedQid       qId,
                                     CsrUint32         dataChannelId,
                                     CsrBtDeviceAddr   *pDeviceAddr,
                                     CsrBtResultCode   resultCode,
                                     CsrBtSupplier     resultSupplier);

void CsrBtPhdcMgrDisassociateCfmSend(CsrSchedQid       qId,
                                     CsrUint32         dataChannelId,
                                     CsrBtDeviceAddr   *pDeviceAddr,
                                     CsrBtResultCode   resultCode,
                                     CsrBtSupplier     resultSupplier);

void CsrBtPhdcMgrBpmBpIndSend(CsrSchedQid               qId,
                              CsrUint32                 dataChannelId,
                              CsrUint16                 personId,
                              CsrUint16                 unitCode,
                              CsrBtPhdcMgrAbsoluteTim   *pTime,
                              CsrUint16                 systolicPressure,
                              CsrUint16                 diastolicPressure,
                              CsrUint16                 meanArterialPressure);


void CsrBtPhdcMgrBpmPulseIndSend(CsrSchedQid                qId,
                                 CsrUint32                  dataChannelId,
                                 CsrUint16                  personId,
                                 CsrUint16                  unitCode,
                                 CsrBtPhdcMgrAbsoluteTim   *pTime,
                                 CsrUint16                  Pulse);

void CsrBtPhdcMgrDsIndSend(CsrSchedQid              qId,
                           CsrUint32                dataChannelId,
                           CsrUint16                objHandle,
                           CsrUint16                personId,
                           CsrBtPhdcScanReportType  scanReportType,
                           CsrUint16                dataLength,
                           CsrUint8                *data);

void CsrBtPhdcMgrBtLinkRelIndSend(CsrSchedQid   qId,
                                  CsrUint32     dataChannelId);

void CsrBtPhdcMgrGetDeviceCapabCfmSend(CsrSchedQid          qId,
                                       CsrBtDeviceAddr      *pDeviceAddr,
                                       CsrBtPhdcMgrResult   resultCode,
                                       CsrBtSupplier        resultSupplier);

void CsrBtPhdcMgrGetDeviceCapabIndSend(CsrSchedQid          appHandle,
                                       CsrBtDeviceAddr      *pDeviceAddr,
                                       CsrUint32            psmIdentifier,
                                       CsrCharString        *serviceName,
                                       CsrCharString        *providerName,
                                       CsrUint8             supportedFeatureListLen,
                                       CsrBtHdpMdep         *supportedFeatureList);

void CsrBtPhdcMgrSetupBtLinkCfmSend(CsrSchedQid         qId,
                                    CsrUint32           psmIdentifier,
                                    CsrBtDeviceAddr     *pDeviceAddr,
                                    CsrBtMdepId         mdepId,
                                    CsrUint16           maxPacketLength,
                                    CsrUint32           dataChannelId,
                                    CsrBtPhdcMgrResult  resultCode,
                                    CsrBtSupplier       resultSupplier);

void CsrBtPhdcMgrWsWeightIndSend(CsrSchedQid                qId,
                                 CsrUint32                  dataChannelId,
                                 CsrUint16                  personId,
                                 CsrUint16                  unitCode,
                                 CsrBtPhdcMgrAbsoluteTim   *pTime,
                                 CsrUint32                  weight);

void CsrBtPhdcMgrUnregisterCfmSend(CsrSchedQid      qId,
                                   CsrBtResultCode  resultCode,
                                   CsrBtSupplier    resultSupplier);


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif /*  CSR_BT_PHDC_MGR_OUT_H*/

