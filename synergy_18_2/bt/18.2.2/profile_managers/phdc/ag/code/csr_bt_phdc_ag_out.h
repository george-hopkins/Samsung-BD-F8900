#ifndef CSR_BT_PHDC_AG_OUT_H__
#define CSR_BT_PHDC_AG_OUT_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/
#include "csr_types.h"
#include "csr_bt_hdp_prim.h"
#include "csr_bt_phdc_ag_prim.h"
#include "csr_sched.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBtPhdcAgConfigureEndpointIndSend(CsrSchedQid appHandle, CsrBtMdepId mdepId);
void CsrBtPhdcAgConfigureEndpointCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtPhdcAgDimObjectIndSend(CsrSchedQid appHandle,CsrUint16  objIdx);
void CsrBtPhdcAgDimAttribIndSend(CsrSchedQid appHandle,CsrUint16  objIdx,CsrUint16  attribIdx);
void CsrBtPhdcAgDimConfigCfmSend(CsrSchedQid appHandle,CsrBtPhdcAgResult resultCode);
void CsrBtPhdcAgGetDeviceCapabIndSend(CsrSchedQid appHandle,CsrBtDeviceAddr deviceAddr,CsrUint32 psmIdentifier,
                                    CsrCharString *serviceName,CsrCharString *providerName,
                                    CsrUint8 supportedFeatureListLen,CsrBtHdpMdep *supportedFeatureList);

void CsrBtHdpCtrlGetCapabCfmSend(CsrSchedQid appHandle,CsrBtDeviceAddr deviceAddr,CsrBtResultCode resultCode,CsrBtSupplier resultSupplier);
void CsrBtPhdcAgAssociateCfmSend(CsrSchedQid appHandle,
                               CsrUint32         psmIdentifier,
                               CsrBtDeviceAddr   deviceAddr,
                               CsrBtMdepId       mdepId,
                               CsrUint16         mdepDataType,
                               CsrUint16         maxPacketLength,
                               CsrUint32         dataChannelId,  
                               CsrBtPhdcAgResult   resultCode);
void CsrBtPhdcAgDataCfmSend(CsrSchedQid appHandle,
                            CsrUint32 dataChannelId,
                            CsrBtPhdcAgResult   resultCode);
void CsrBtPhdcAgSuspendCfmSend(CsrSchedQid appHandle,
                            CsrUint32 dataChannelId,
                            CsrBtResultCode   resultCode,
                            CsrBtSupplier     resultSupplier);
void CsrBtPhdcAgSuspendIndSend(CsrSchedQid appHandle,
                            CsrUint32 dataChannelId);
void CsrBtPhdcResumeCfmSend(CsrSchedQid appHandle,
                            CsrUint32 dataChannelId,
                            CsrBtResultCode   resultCode,
                            CsrBtSupplier     resultSupplier);
void CsrBtPhdcResumeIndSend(CsrSchedQid appHandle,
                            CsrUint32 dataChannelId,
                            CsrBtResultCode   resultCode,
                            CsrBtSupplier     resultSupplier);
void CsrBtPhdcAgDisassociateCfmSend(CsrSchedQid appHandle,
                            CsrUint32 dataChannelId,
                            CsrBtDeviceAddr   deviceAddr,
                            CsrBtResultCode   resultCode,
                            CsrBtSupplier     resultSupplier);
void CsrBtPhdcAgChangeDimUnitCodeCfmSend(CsrSchedQid appHandle,
                                         CsrUint16         objHandle,
                                         CsrBtPhdcAgResult   resultCode); 
void CsrBtPhdcAgDisassociateIndSend(CsrSchedQid appHandle,
                            CsrUint32 dataChannelId,
                            CsrBtDeviceAddr   deviceAddr);                            

void CsrBtPhdcAgAcceptBtLinkIndSend(CsrSchedQid appHandle,
                            CsrBtDeviceAddr   deviceAddr); 

void CsrBtPhdcAgSetupBtLinkIndSend(CsrSchedQid appHandle,CsrBtDeviceAddr deviceAddr,
    CsrBtResultCode   resultCode,CsrBtSupplier     resultSupplier);                           
void CsrBtPhdcAgBtLinkRelIndSend(CsrSchedQid appHandle,CsrBtDeviceAddr deviceAddr,
    CsrBtResultCode   resultCode,CsrBtSupplier     resultSupplier);

void CsrBtPhdcAgUnregisterCfmSend(CsrSchedQid appHandle,
    CsrBtResultCode   resultCode,CsrBtSupplier     resultSupplier);                            

#ifdef __cplusplus
   }
#endif
   
#endif


