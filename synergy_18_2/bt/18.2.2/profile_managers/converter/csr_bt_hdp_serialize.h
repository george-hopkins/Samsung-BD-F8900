#ifndef CSR_BT_HDP_SERIALIZE_H__
#define CSR_BT_HDP_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#include "csr_synergy.h"
#ifdef __cplusplus
extern "C" {
#endif

void CsrBtHdpPfree(void *ptr);

#ifndef EXCLUDE_CSR_BT_HDP_DATA_IND
CsrSize CsrBtHdpDataIndSizeof(void *msg);
CsrUint8 *CsrBtHdpDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHdpDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HDP_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HDP_DEACTIVATE_CFM
CsrSize CsrBtHdpDeactivateCfmSizeof(void *msg);
CsrUint8 *CsrBtHdpDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpDeactivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpDeactivateCfmSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_SUSPEND_CFM
CsrSize CsrBtHdpSuspendCfmSizeof(void *msg);
CsrUint8 *CsrBtHdpSuspendCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpSuspendCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpSuspendCfmSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_SUSPEND_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_RES
CsrSize CsrBtHdpAcceptConnectVirtualChannelResSizeof(void *msg);
CsrUint8 *CsrBtHdpAcceptConnectVirtualChannelResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpAcceptConnectVirtualChannelResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpAcceptConnectVirtualChannelResSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_RES */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_CFM
CsrSize CsrBtHdpConnectVirtualChannelCfmSizeof(void *msg);
CsrUint8 *CsrBtHdpConnectVirtualChannelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpConnectVirtualChannelCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpConnectVirtualChannelCfmSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_RESUME_REQ
CsrSize CsrBtHdpResumeReqSizeof(void *msg);
CsrUint8 *CsrBtHdpResumeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpResumeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpResumeReqSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_RESUME_CFM
CsrSize CsrBtHdpResumeCfmSizeof(void *msg);
CsrUint8 *CsrBtHdpResumeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpResumeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpResumeCfmSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_DEACTIVATE_REQ
CsrSize CsrBtHdpDeactivateReqSizeof(void *msg);
CsrUint8 *CsrBtHdpDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpDeactivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpDeactivateReqSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_REQ
CsrSize CsrBtHdpDisconnectVirtualChannelReqSizeof(void *msg);
CsrUint8 *CsrBtHdpDisconnectVirtualChannelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpDisconnectVirtualChannelReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpDisconnectVirtualChannelReqSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND
CsrSize CsrBtHdpDisconnectAssociateChannelIndSizeof(void *msg);
CsrUint8 *CsrBtHdpDisconnectAssociateChannelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpDisconnectAssociateChannelIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpDisconnectAssociateChannelIndSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_IND
CsrSize CsrBtHdpRegisterIndSizeof(void *msg);
CsrUint8 *CsrBtHdpRegisterIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpRegisterIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpRegisterIndSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_IND */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_REQ
CsrSize CsrBtHdpConnectAssociateChannelReqSizeof(void *msg);
CsrUint8 *CsrBtHdpConnectAssociateChannelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpConnectAssociateChannelReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpConnectAssociateChannelReqSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_CFM
CsrSize CsrBtHdpCtrlGetCapabCfmSizeof(void *msg);
CsrUint8 *CsrBtHdpCtrlGetCapabCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpCtrlGetCapabCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpCtrlGetCapabCfmSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_SECURITY_IN_REQ
CsrSize CsrBtHdpSecurityInReqSizeof(void *msg);
CsrUint8 *CsrBtHdpSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpSecurityInReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpSecurityInReqSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_SECURITY_OUT_REQ
CsrSize CsrBtHdpSecurityOutReqSizeof(void *msg);
CsrUint8 *CsrBtHdpSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpSecurityOutReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpSecurityOutReqSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_DATA_REQ
CsrSize CsrBtHdpDataReqSizeof(void *msg);
CsrUint8 *CsrBtHdpDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHdpDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HDP_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_RSP
CsrSize CsrBtHdpRegisterRspSizeof(void *msg);
CsrUint8 *CsrBtHdpRegisterRspSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpRegisterRspDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHdpRegisterRspSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_RSP */

#ifndef EXCLUDE_CSR_BT_HDP_RESUME_RSP
CsrSize CsrBtHdpResumeRspSizeof(void *msg);
CsrUint8 *CsrBtHdpResumeRspSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpResumeRspDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpResumeRspSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_RSP */

#ifndef EXCLUDE_CSR_BT_HDP_RESUME_IND
CsrSize CsrBtHdpResumeIndSizeof(void *msg);
CsrUint8 *CsrBtHdpResumeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpResumeIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpResumeIndSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_IND */

#ifndef EXCLUDE_CSR_BT_HDP_DATA_CFM
CsrSize CsrBtHdpDataCfmSizeof(void *msg);
CsrUint8 *CsrBtHdpDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpDataCfmSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_UNREGISTER_REQ
CsrSize CsrBtHdpUnregisterReqSizeof(void *msg);
CsrUint8 *CsrBtHdpUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpUnregisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpUnregisterReqSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_IND
CsrSize CsrBtHdpAcceptConnectVirtualChannelIndSizeof(void *msg);
CsrUint8 *CsrBtHdpAcceptConnectVirtualChannelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpAcceptConnectVirtualChannelIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpAcceptConnectVirtualChannelIndSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_SUSPEND_REQ
CsrSize CsrBtHdpSuspendReqSizeof(void *msg);
CsrUint8 *CsrBtHdpSuspendReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpSuspendReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpSuspendReqSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_SUSPEND_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND
CsrSize CsrBtHdpAcceptConnectAssociateChannelIndSizeof(void *msg);
CsrUint8 *CsrBtHdpAcceptConnectAssociateChannelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpAcceptConnectAssociateChannelIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpAcceptConnectAssociateChannelIndSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_CFM
CsrSize CsrBtHdpRegisterCfmSizeof(void *msg);
CsrUint8 *CsrBtHdpRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpRegisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpRegisterCfmSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_CFM
CsrSize CsrBtHdpDisconnectVirtualChannelCfmSizeof(void *msg);
CsrUint8 *CsrBtHdpDisconnectVirtualChannelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpDisconnectVirtualChannelCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpDisconnectVirtualChannelCfmSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND
CsrSize CsrBtHdpConnectAssociateChannelIndSizeof(void *msg);
CsrUint8 *CsrBtHdpConnectAssociateChannelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpConnectAssociateChannelIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpConnectAssociateChannelIndSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_IND
CsrSize CsrBtHdpCtrlGetCapabIndSizeof(void *msg);
CsrUint8 *CsrBtHdpCtrlGetCapabIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpCtrlGetCapabIndDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHdpCtrlGetCapabIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_IND */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM
CsrSize CsrBtHdpConnectAssociateChannelCfmSizeof(void *msg);
CsrUint8 *CsrBtHdpConnectAssociateChannelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpConnectAssociateChannelCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpConnectAssociateChannelCfmSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_REQ
CsrSize CsrBtHdpDisconnectAssociateChannelReqSizeof(void *msg);
CsrUint8 *CsrBtHdpDisconnectAssociateChannelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpDisconnectAssociateChannelReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpDisconnectAssociateChannelReqSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_IND
CsrSize CsrBtHdpConnectVirtualChannelIndSizeof(void *msg);
CsrUint8 *CsrBtHdpConnectVirtualChannelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpConnectVirtualChannelIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpConnectVirtualChannelIndSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_ACTIVATE_CFM
CsrSize CsrBtHdpActivateCfmSizeof(void *msg);
CsrUint8 *CsrBtHdpActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpActivateCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpActivateCfmSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_ACTIVATE_REQ
CsrSize CsrBtHdpActivateReqSizeof(void *msg);
CsrUint8 *CsrBtHdpActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpActivateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpActivateReqSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_REQ
CsrSize CsrBtHdpRegisterReqSizeof(void *msg);
CsrUint8 *CsrBtHdpRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpRegisterReqDes(CsrUint8 *buffer, CsrSize length);
void CsrBtHdpRegisterReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_REQ
CsrSize CsrBtHdpCtrlGetCapabReqSizeof(void *msg);
CsrUint8 *CsrBtHdpCtrlGetCapabReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpCtrlGetCapabReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpCtrlGetCapabReqSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_REQ
CsrSize CsrBtHdpConnectVirtualChannelReqSizeof(void *msg);
CsrUint8 *CsrBtHdpConnectVirtualChannelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpConnectVirtualChannelReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpConnectVirtualChannelReqSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_HDP_UNREGISTER_CFM
CsrSize CsrBtHdpUnregisterCfmSizeof(void *msg);
CsrUint8 *CsrBtHdpUnregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpUnregisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpUnregisterCfmSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_RES
CsrSize CsrBtHdpAcceptConnectAssociateChannelResSizeof(void *msg);
CsrUint8 *CsrBtHdpAcceptConnectAssociateChannelResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpAcceptConnectAssociateChannelResDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpAcceptConnectAssociateChannelResSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_RES */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_IND
CsrSize CsrBtHdpDisconnectVirtualChannelIndSizeof(void *msg);
CsrUint8 *CsrBtHdpDisconnectVirtualChannelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpDisconnectVirtualChannelIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpDisconnectVirtualChannelIndSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM
CsrSize CsrBtHdpDisconnectAssociateChannelCfmSizeof(void *msg);
CsrUint8 *CsrBtHdpDisconnectAssociateChannelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrBtHdpDisconnectAssociateChannelCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrBtHdpDisconnectAssociateChannelCfmSerFree CsrBtHdpPfree
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM */

#ifdef __cplusplus
}
#endif

#endif
