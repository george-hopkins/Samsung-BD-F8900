/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"
#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_bt_hand_coded.h"
#include "csr_bt_hdp_converter_init.h"
#ifndef EXCLUDE_CSR_BT_HDP_MODULE
#include "csr_bt_hdp_serialize.h"
#include "csr_bt_hdp_prim.h"

static CsrMsgConvMsgEntry csr_bt_hdp_conv_lut[] = {
#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND
    { CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND, CsrBtHdpAcceptConnectAssociateChannelIndSizeof, CsrBtHdpAcceptConnectAssociateChannelIndSer, CsrBtHdpAcceptConnectAssociateChannelIndDes, CsrBtHdpAcceptConnectAssociateChannelIndSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_IND */
#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_RES
    { CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_RES, CsrBtHdpAcceptConnectAssociateChannelResSizeof, CsrBtHdpAcceptConnectAssociateChannelResSer, CsrBtHdpAcceptConnectAssociateChannelResDes, CsrBtHdpAcceptConnectAssociateChannelResSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_ASSOCIATE_CHANNEL_RES */
#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_IND
    { CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_IND, CsrBtHdpAcceptConnectVirtualChannelIndSizeof, CsrBtHdpAcceptConnectVirtualChannelIndSer, CsrBtHdpAcceptConnectVirtualChannelIndDes, CsrBtHdpAcceptConnectVirtualChannelIndSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_IND */
#ifndef EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_RES
    { CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_RES, CsrBtHdpAcceptConnectVirtualChannelResSizeof, CsrBtHdpAcceptConnectVirtualChannelResSer, CsrBtHdpAcceptConnectVirtualChannelResDes, CsrBtHdpAcceptConnectVirtualChannelResSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_ACCEPT_CONNECT_VIRTUAL_CHANNEL_RES */
#ifndef EXCLUDE_CSR_BT_HDP_ACTIVATE_CFM
    { CSR_BT_HDP_ACTIVATE_CFM, CsrBtHdpActivateCfmSizeof, CsrBtHdpActivateCfmSer, CsrBtHdpActivateCfmDes, CsrBtHdpActivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_ACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_HDP_ACTIVATE_REQ
    { CSR_BT_HDP_ACTIVATE_REQ, CsrBtHdpActivateReqSizeof, CsrBtHdpActivateReqSer, CsrBtHdpActivateReqDes, CsrBtHdpActivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_ACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM
    { CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM, CsrBtHdpConnectAssociateChannelCfmSizeof, CsrBtHdpConnectAssociateChannelCfmSer, CsrBtHdpConnectAssociateChannelCfmDes, CsrBtHdpConnectAssociateChannelCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_CFM */
#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND
    { CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND, CsrBtHdpConnectAssociateChannelIndSizeof, CsrBtHdpConnectAssociateChannelIndSer, CsrBtHdpConnectAssociateChannelIndDes, CsrBtHdpConnectAssociateChannelIndSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_IND */
#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_REQ
    { CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_REQ, CsrBtHdpConnectAssociateChannelReqSizeof, CsrBtHdpConnectAssociateChannelReqSer, CsrBtHdpConnectAssociateChannelReqDes, CsrBtHdpConnectAssociateChannelReqSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_ASSOCIATE_CHANNEL_REQ */
#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_CFM
    { CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_CFM, CsrBtHdpConnectVirtualChannelCfmSizeof, CsrBtHdpConnectVirtualChannelCfmSer, CsrBtHdpConnectVirtualChannelCfmDes, CsrBtHdpConnectVirtualChannelCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_CFM */
#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_IND
    { CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_IND, CsrBtHdpConnectVirtualChannelIndSizeof, CsrBtHdpConnectVirtualChannelIndSer, CsrBtHdpConnectVirtualChannelIndDes, CsrBtHdpConnectVirtualChannelIndSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_IND */
#ifndef EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_REQ
    { CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_REQ, CsrBtHdpConnectVirtualChannelReqSizeof, CsrBtHdpConnectVirtualChannelReqSer, CsrBtHdpConnectVirtualChannelReqDes, CsrBtHdpConnectVirtualChannelReqSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_CONNECT_VIRTUAL_CHANNEL_REQ */
#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_CFM
    { CSR_BT_HDP_CTRL_GET_CAPAB_CFM, CsrBtHdpCtrlGetCapabCfmSizeof, CsrBtHdpCtrlGetCapabCfmSer, CsrBtHdpCtrlGetCapabCfmDes, CsrBtHdpCtrlGetCapabCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_CFM */
#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_IND
    { CSR_BT_HDP_CTRL_GET_CAPAB_IND, CsrBtHdpCtrlGetCapabIndSizeof, CsrBtHdpCtrlGetCapabIndSer, CsrBtHdpCtrlGetCapabIndDes, CsrBtHdpCtrlGetCapabIndSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_IND */
#ifndef EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_REQ
    { CSR_BT_HDP_CTRL_GET_CAPAB_REQ, CsrBtHdpCtrlGetCapabReqSizeof, CsrBtHdpCtrlGetCapabReqSer, CsrBtHdpCtrlGetCapabReqDes, CsrBtHdpCtrlGetCapabReqSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_CTRL_GET_CAPAB_REQ */
#ifndef EXCLUDE_CSR_BT_HDP_DATA_CFM
    { CSR_BT_HDP_DATA_CFM, CsrBtHdpDataCfmSizeof, CsrBtHdpDataCfmSer, CsrBtHdpDataCfmDes, CsrBtHdpDataCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_DATA_CFM */
#ifndef EXCLUDE_CSR_BT_HDP_DATA_IND
    { CSR_BT_HDP_DATA_IND, CsrBtHdpDataIndSizeof, CsrBtHdpDataIndSer, CsrBtHdpDataIndDes, CsrBtHdpDataIndSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_DATA_IND */
#ifndef EXCLUDE_CSR_BT_HDP_DATA_REQ
    { CSR_BT_HDP_DATA_REQ, CsrBtHdpDataReqSizeof, CsrBtHdpDataReqSer, CsrBtHdpDataReqDes, CsrBtHdpDataReqSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_DATA_REQ */
#ifndef EXCLUDE_CSR_BT_HDP_DEACTIVATE_CFM
    { CSR_BT_HDP_DEACTIVATE_CFM, CsrBtHdpDeactivateCfmSizeof, CsrBtHdpDeactivateCfmSer, CsrBtHdpDeactivateCfmDes, CsrBtHdpDeactivateCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_DEACTIVATE_CFM */
#ifndef EXCLUDE_CSR_BT_HDP_DEACTIVATE_REQ
    { CSR_BT_HDP_DEACTIVATE_REQ, CsrBtHdpDeactivateReqSizeof, CsrBtHdpDeactivateReqSer, CsrBtHdpDeactivateReqDes, CsrBtHdpDeactivateReqSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_DEACTIVATE_REQ */
#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM
    { CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM, CsrBtHdpDisconnectAssociateChannelCfmSizeof, CsrBtHdpDisconnectAssociateChannelCfmSer, CsrBtHdpDisconnectAssociateChannelCfmDes, CsrBtHdpDisconnectAssociateChannelCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_CFM */
#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND
    { CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND, CsrBtHdpDisconnectAssociateChannelIndSizeof, CsrBtHdpDisconnectAssociateChannelIndSer, CsrBtHdpDisconnectAssociateChannelIndDes, CsrBtHdpDisconnectAssociateChannelIndSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_IND */
#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_REQ
    { CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_REQ, CsrBtHdpDisconnectAssociateChannelReqSizeof, CsrBtHdpDisconnectAssociateChannelReqSer, CsrBtHdpDisconnectAssociateChannelReqDes, CsrBtHdpDisconnectAssociateChannelReqSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_ASSOCIATE_CHANNEL_REQ */
#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_CFM
    { CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_CFM, CsrBtHdpDisconnectVirtualChannelCfmSizeof, CsrBtHdpDisconnectVirtualChannelCfmSer, CsrBtHdpDisconnectVirtualChannelCfmDes, CsrBtHdpDisconnectVirtualChannelCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_CFM */
#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_IND
    { CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_IND, CsrBtHdpDisconnectVirtualChannelIndSizeof, CsrBtHdpDisconnectVirtualChannelIndSer, CsrBtHdpDisconnectVirtualChannelIndDes, CsrBtHdpDisconnectVirtualChannelIndSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_IND */
#ifndef EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_REQ
    { CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_REQ, CsrBtHdpDisconnectVirtualChannelReqSizeof, CsrBtHdpDisconnectVirtualChannelReqSer, CsrBtHdpDisconnectVirtualChannelReqDes, CsrBtHdpDisconnectVirtualChannelReqSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_DISCONNECT_VIRTUAL_CHANNEL_REQ */
#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_CFM
    { CSR_BT_HDP_REGISTER_CFM, CsrBtHdpRegisterCfmSizeof, CsrBtHdpRegisterCfmSer, CsrBtHdpRegisterCfmDes, CsrBtHdpRegisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_IND
    { CSR_BT_HDP_REGISTER_IND, CsrBtHdpRegisterIndSizeof, CsrBtHdpRegisterIndSer, CsrBtHdpRegisterIndDes, CsrBtHdpRegisterIndSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_IND */
#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_REQ
    { CSR_BT_HDP_REGISTER_REQ, CsrBtHdpRegisterReqSizeof, CsrBtHdpRegisterReqSer, CsrBtHdpRegisterReqDes, CsrBtHdpRegisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_REQ */
#ifndef EXCLUDE_CSR_BT_HDP_REGISTER_RSP
    { CSR_BT_HDP_REGISTER_RSP, CsrBtHdpRegisterRspSizeof, CsrBtHdpRegisterRspSer, CsrBtHdpRegisterRspDes, CsrBtHdpRegisterRspSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_REGISTER_RSP */
#ifndef EXCLUDE_CSR_BT_HDP_RESUME_CFM
    { CSR_BT_HDP_RESUME_CFM, CsrBtHdpResumeCfmSizeof, CsrBtHdpResumeCfmSer, CsrBtHdpResumeCfmDes, CsrBtHdpResumeCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_CFM */
#ifndef EXCLUDE_CSR_BT_HDP_RESUME_IND
    { CSR_BT_HDP_RESUME_IND, CsrBtHdpResumeIndSizeof, CsrBtHdpResumeIndSer, CsrBtHdpResumeIndDes, CsrBtHdpResumeIndSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_IND */
#ifndef EXCLUDE_CSR_BT_HDP_RESUME_REQ
    { CSR_BT_HDP_RESUME_REQ, CsrBtHdpResumeReqSizeof, CsrBtHdpResumeReqSer, CsrBtHdpResumeReqDes, CsrBtHdpResumeReqSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_REQ */
#ifndef EXCLUDE_CSR_BT_HDP_RESUME_RSP
    { CSR_BT_HDP_RESUME_RSP, CsrBtHdpResumeRspSizeof, CsrBtHdpResumeRspSer, CsrBtHdpResumeRspDes, CsrBtHdpResumeRspSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_RESUME_RSP */
#ifndef EXCLUDE_CSR_BT_HDP_SECURITY_IN_REQ
    { CSR_BT_HDP_SECURITY_IN_REQ, CsrBtHdpSecurityInReqSizeof, CsrBtHdpSecurityInReqSer, CsrBtHdpSecurityInReqDes, CsrBtHdpSecurityInReqSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_SECURITY_IN_REQ */
#ifndef EXCLUDE_CSR_BT_HDP_SECURITY_OUT_REQ
    { CSR_BT_HDP_SECURITY_OUT_REQ, CsrBtHdpSecurityOutReqSizeof, CsrBtHdpSecurityOutReqSer, CsrBtHdpSecurityOutReqDes, CsrBtHdpSecurityOutReqSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_SECURITY_OUT_REQ */
#ifndef EXCLUDE_CSR_BT_HDP_SUSPEND_CFM
    { CSR_BT_HDP_SUSPEND_CFM, CsrBtHdpSuspendCfmSizeof, CsrBtHdpSuspendCfmSer, CsrBtHdpSuspendCfmDes, CsrBtHdpSuspendCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_SUSPEND_CFM */
#ifndef EXCLUDE_CSR_BT_HDP_SUSPEND_REQ
    { CSR_BT_HDP_SUSPEND_REQ, CsrBtHdpSuspendReqSizeof, CsrBtHdpSuspendReqSer, CsrBtHdpSuspendReqDes, CsrBtHdpSuspendReqSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_SUSPEND_REQ */
#ifndef EXCLUDE_CSR_BT_HDP_UNREGISTER_CFM
    { CSR_BT_HDP_UNREGISTER_CFM, CsrBtHdpUnregisterCfmSizeof, CsrBtHdpUnregisterCfmSer, CsrBtHdpUnregisterCfmDes, CsrBtHdpUnregisterCfmSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_UNREGISTER_CFM */
#ifndef EXCLUDE_CSR_BT_HDP_UNREGISTER_REQ
    { CSR_BT_HDP_UNREGISTER_REQ, CsrBtHdpUnregisterReqSizeof, CsrBtHdpUnregisterReqSer, CsrBtHdpUnregisterReqDes, CsrBtHdpUnregisterReqSerFree },
#endif /* EXCLUDE_CSR_BT_HDP_UNREGISTER_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrBtHdpConverterInit(void)
{
    CsrMsgConvInsert(CSR_BT_HDP_PRIM, csr_bt_hdp_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_bt_hdp_conv_info = {
    CSR_BT_HDP_PRIM,
    "CSR_BT_HDP_PRIM",
    csr_bt_hdp_conv_lut
};

CsrLogPrimitiveInformation* CsrBtHdpTechInfoGet(void)
{
    return &csr_bt_hdp_conv_info;
}
#endif
#endif

