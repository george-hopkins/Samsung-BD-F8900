#ifndef CSR_BT_HDP_SEF_H__
#define CSR_BT_HDP_SEF_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_bt_hdp_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---------- Primitives: ----------*/
/* hdp event handler */
void CsrBtHdpIdleRegisterReqEvHandler(HdpInstanceData *pInstance);
void CsrBtHdpInvalidRegisterReqEvHandler(HdpInstanceData *pInstance);
void CsrBtHdpIdleRegisterRspEvHandler(HdpInstanceData *pInstance);
void CsrBtHdpInvalidRegisterRspEvHandler(HdpInstanceData *pInstance);
void CsrBtHdpGetCapabReqHandler(HdpInstanceData *pInstance);
void CsrBtHdpConnectAssociateChannelReqHandler(HdpInstanceData *pInstance);
void CsrBtHdpXXXConnectAssociateChannelReqHandler(HdpInstanceData *pInstance);
void CsrBtHdpAcceptConnectAssociateChannelResHandler(HdpInstanceData *pInstance);
void CsrBtHdpIdleActivateReqEvHandler(HdpInstanceData *pInstance);
void CsrBtHdpInvalidActivateReqEvHandler(HdpInstanceData *pInstance);
void CsrBtHdpDataConnectVirtualChannelReqEvHandler(HdpInstanceData *pInstance);
void CsrBtHdpConnectedAcceptConnectVirtualChannelResEvHandler(HdpInstanceData *pInstance);
void CsrBtHdpAssociateChDisconnectReqHandler(HdpInstanceData *pInstance);
void CsrBtHdpVirtualChDisconnectReqHandler(HdpInstanceData *pInstance);
void CsrBtHdpDataReqHandler(HdpInstanceData *pInstance);
void CsrBtHdpXXXSuspendReqEvHandler(HdpInstanceData *pInstance);
void CsrBtHdpInvalidSuspendReqEvHandler(HdpInstanceData *pInstance);
void CsrBtHdpXXXResumeReqEvHandler(HdpInstanceData *pInstance);
void CsrBtHdpInvalidResumeReqEvHandler(HdpInstanceData *pInstance);
void CsrBtHdpSuspendHdpResumeRspHandler(HdpInstanceData *pInstance);
void CsrBtHdpXXXHdpUnregisterReqHandler(HdpInstanceData *pInstance);


/* cm sds event handler */
void CsrBtHdpIdleCmSdsRegisterCfmHandler(HdpInstanceData *pInstance);
void CsrBtHdpIdleCmSdsUnregisterCfmHandler(HdpInstanceData *pInstance);

/* l2ca event handler */
void CsrBtHdpIdleL2CaRegisterCfmEvHandler(HdpInstanceData *pInstance);

/* mcap event handler */
void CsrBtHdpIdleMcapActivateCfmEvHandler(HdpInstanceData *pInstance);
void CsrBtHdpMcapConnectMclCfmHandler(HdpInstanceData *pInstance);
void CsrBtHdpMcapConnectMdlCfmHandler(HdpInstanceData *pInstance);
void CsrBtHdpMcapConnectMdlIndHandler(HdpInstanceData *pInstance);
void CsrBtHdpMcapConnectMclIndHandler(HdpInstanceData *pInstance);
void CsrBtHdpMcapCreateMdlIndHandler(HdpInstanceData *pInstance);
void CsrBtHdpConnectedMcapConnectMdlCfmEvHandler(HdpInstanceData *pInstance);
void CsrBtHdpXXXMcapCreateMdlIndEvHandler(HdpInstanceData *pInstance);
void CsrBtHdpXXXMcapConnectMdlIndEvHandler(HdpInstanceData *pInstance);
void CsrBtHdpConnectingMcapMclDiscIndHandler(HdpInstanceData *pInstance);
void CsrBtHdpXXXMcapMclDisconnectIndHandler(HdpInstanceData *pInstance);
void CsrBtHdpXXXMcapMclDisconnectCfmHandler(HdpInstanceData *pInstance);
void CsrBtHdpXXXMcapMdlDisconnectIndHandler(HdpInstanceData *pInstance);
void CsrBtHdpXXXMcapMdlDisconnectCfmHandler(HdpInstanceData *pInstance);
void CsrBtHdpXXXMcapMdlDeleteCfmHandler(HdpInstanceData *pInstance);
void CsrBtHdpXXXMcapMdlDeleteIndHandler(HdpInstanceData *pInstance);
void CsrBtHdpSuspendMcapMdlDisconnectCfmHandler(HdpInstanceData *pInstance);
void CsrBtHdpSuspendMcapMclDisconnectCfmHandler(HdpInstanceData *pInstance);
void CsrBtHdpSuspendMcapMclConnectCfmHandler(HdpInstanceData *pInstance);
void CsrBtHdpXXXMcapReconnectMdlIndHandler(HdpInstanceData *pInstance);
void CsrBtHdpSuspendMcapReconnectMdlCfmHandler(HdpInstanceData *pInstance);
void CsrBtHdpMcapDataIndHandler(HdpInstanceData * pInstance);
void CsrBtHdpMcapDataCfmHandler(HdpInstanceData * pInstance);
void CsrBtHdpDeactivateReqHandler(HdpInstanceData * pInstance);
void CsrBtHdpMcapDeactivateCfmHandler(HdpInstanceData * pInstance);

/* Upstream helper functions */


#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_HDP_SEF_H__ */
