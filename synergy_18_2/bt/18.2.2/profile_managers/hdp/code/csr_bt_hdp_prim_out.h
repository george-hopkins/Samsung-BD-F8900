#ifndef CSR_BT_HDP_PRIM_OUT_H__
#define CSR_BT_HDP_PRIM_OUT_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_types.h"
#include "csr_bt_hdp_prim.h"

#ifdef __cplusplus
extern "C" {
#endif


void CsrBtHdpRegisterCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtHdpRegisterIndSend(CsrSchedQid appHandle, CsrBtMdepId mdepId);
void CsrBtHdpActivateCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtHdpGetCapabCfmSend(CsrSchedQid appHandle, CsrBtDeviceAddr *addr, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtHdpRemoteFeaturesIndSend(CsrSchedQid appHandle, CsrBtDeviceAddr *addr, CsrBtHdpSdpRecord *record, CsrUint32 hdpInstance);

void CsrBtHdpConnectAssociateChannelCfmSend(CsrSchedQid appHandle,
                                       CsrBtMdepId mdepId,
                                       CsrBtMdepDataType mdepdatatype,
                                       l2ca_mtu_t mtu,
                                       CsrBtResultCode resultCode,
                                       CsrBtSupplier resultSupplier,
                                       CsrBtDeviceAddr *addr,
                                       CsrUint32 hdpInstanceId,
                                       CsrUint32 assocId);


void CsrBtHdpConnectAssociateChannelIndSend(CsrSchedQid appHandle,
                                       CsrBtDeviceAddr *bd_addr,
                                       CsrBtResultCode resultCode,
                                       CsrBtSupplier resultSupplier,
                                       CsrBtMdepId mdepId,
                                       CsrBtMdepDataType dataType,
                                       CsrUint32 assocId,
                                       CsrUint16 mtu,
                                       CsrUint32 hdpInstanceId);


void CsrBtHdpAcceptConnectAssociateChannelIndSend(CsrSchedQid               appHandle,
                                                  CsrBtMdepId       mdepId,
                                                  CsrUint32          assocId,
                                                  CsrBtDeviceAddr   *bd_addr);

void CsrBtHdpConnectVirtualChCfmSend(CsrSchedQid appHandle,
                                CsrUint32 assocChId,
                                CsrUint32 virtualChId,
                                CsrUint16 maxPacketLength,
                                CsrBtResultCode resultCode,
                                CsrBtSupplier resultSupplier);
void CsrBtHdpAcceptConnectVirtualChIndSend(CsrSchedQid appHandle,
                                CsrUint32 assocChId,
                                CsrUint32 virtualChId,
                                CsrUint8 virtualChannelConfig);
void CsrBtHdpConnectVirtualChIndSend(CsrSchedQid appHandle,
                                CsrUint32 assocChId,
                                CsrUint32 virtualChId,
                                CsrUint16 maxPacketLength,
                                CsrBtResultCode resultCode,
                                CsrBtSupplier resultSupplier);

void CsrBtHdpDisconnectAssociateChIndSend(CsrSchedQid appHandle,
                                     CsrUint32 assocChId,
                                     CsrBtResultCode resultCode,
                                     CsrBtSupplier resultSupplier);

void CsrBtHdpDisconnectAssociateChCfmSend(CsrSchedQid appHandle,
                                     CsrUint32 assocChId,
                                     CsrBtResultCode resultCode,
                                     CsrBtSupplier resultSupplier);

void CsrBtHdpDisconnectVirtualChIndSend(CsrSchedQid appHandle,
                                   CsrUint32 virtualChId,
                                   CsrBtResultCode resultCode,
                                   CsrBtSupplier resultSupplier);

void CsrBtHdpDisconnectVirtualChCfmSend(CsrSchedQid appHandle,
                                   CsrUint32 virtualChId,
                                   CsrBtResultCode resultCode,
                                   CsrBtSupplier resultSupplier);

void CsrBtHdpDataIndSend(CsrSchedQid    appHandle,
                         CsrUint8       *payload,
                         CsrUint16      length,
                         CsrUint32      assocChId);

void CsrBtHdpDataCfmSend(CsrSchedQid appHandle,CsrUint32 assocChId, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtHdpSuspendCfmSend(CsrSchedQid appHandle,CsrUint32 assocChId, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtHdpResumeCfmSend(CsrSchedQid appHandle,CsrBtDeviceAddr *bd_addr,CsrUint32 assocChId, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtHdpResumeIndSend(CsrSchedQid appHandle,CsrUint32 chId,CsrBtDeviceAddr    deviceAddr);
void CsrBtHdpUnregisterCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);
void CsrBtHdpDeactivateCfmSend(CsrSchedQid appHandle, CsrBtResultCode resultCode, CsrBtSupplier resultSupplier);


#ifdef __cplusplus
   }
#endif

#endif


