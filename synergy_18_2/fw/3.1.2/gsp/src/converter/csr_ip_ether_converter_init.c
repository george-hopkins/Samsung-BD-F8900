/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_hand_coded.h"
#include "csr_ip_ether_converter_init.h"
#ifndef EXCLUDE_CSR_IP_ETHER_MODULE
#include "csr_ip_ether_serialize.h"
#include "csr_ip_ether_prim.h"

static CsrMsgConvMsgEntry csr_ip_ether_conv_lut[] = {
#ifndef EXCLUDE_CSR_IP_ETHER_IF_ADD_CFM
    { CSR_IP_ETHER_IF_ADD_CFM, CsrIpEtherIfAddCfmSizeof, CsrIpEtherIfAddCfmSer, CsrIpEtherIfAddCfmDes, CsrIpEtherIfAddCfmSerFree },
#endif /* EXCLUDE_CSR_IP_ETHER_IF_ADD_CFM */
#ifndef EXCLUDE_CSR_IP_ETHER_IF_ADD_REQ
    { CSR_IP_ETHER_IF_ADD_REQ, CsrIpEtherIfAddReqSizeof, CsrIpEtherIfAddReqSer, CsrIpEtherIfAddReqDes, CsrIpEtherIfAddReqSerFree },
#endif /* EXCLUDE_CSR_IP_ETHER_IF_ADD_REQ */
#ifndef EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_PAUSE_REQ
    { CSR_IP_ETHER_IF_FLOW_CONTROL_PAUSE_REQ, CsrIpEtherIfFlowControlPauseReqSizeof, CsrIpEtherIfFlowControlPauseReqSer, CsrIpEtherIfFlowControlPauseReqDes, CsrIpEtherIfFlowControlPauseReqSerFree },
#endif /* EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_PAUSE_REQ */
#ifndef EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_RESUME_REQ
    { CSR_IP_ETHER_IF_FLOW_CONTROL_RESUME_REQ, CsrIpEtherIfFlowControlResumeReqSizeof, CsrIpEtherIfFlowControlResumeReqSer, CsrIpEtherIfFlowControlResumeReqDes, CsrIpEtherIfFlowControlResumeReqSerFree },
#endif /* EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_RESUME_REQ */
#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_CFM
    { CSR_IP_ETHER_IF_LINK_DOWN_CFM, CsrIpEtherIfLinkDownCfmSizeof, CsrIpEtherIfLinkDownCfmSer, CsrIpEtherIfLinkDownCfmDes, CsrIpEtherIfLinkDownCfmSerFree },
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_CFM */
#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_REQ
    { CSR_IP_ETHER_IF_LINK_DOWN_REQ, CsrIpEtherIfLinkDownReqSizeof, CsrIpEtherIfLinkDownReqSer, CsrIpEtherIfLinkDownReqDes, CsrIpEtherIfLinkDownReqSerFree },
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_REQ */
#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_CFM
    { CSR_IP_ETHER_IF_LINK_UP_CFM, CsrIpEtherIfLinkUpCfmSizeof, CsrIpEtherIfLinkUpCfmSer, CsrIpEtherIfLinkUpCfmDes, CsrIpEtherIfLinkUpCfmSerFree },
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_CFM */
#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_REQ
    { CSR_IP_ETHER_IF_LINK_UP_REQ, CsrIpEtherIfLinkUpReqSizeof, CsrIpEtherIfLinkUpReqSer, CsrIpEtherIfLinkUpReqDes, CsrIpEtherIfLinkUpReqSerFree },
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_REQ */
#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_IND
    { CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_IND, CsrIpEtherIfMulticastAddrAddIndSizeof, CsrIpEtherIfMulticastAddrAddIndSer, CsrIpEtherIfMulticastAddrAddIndDes, CsrIpEtherIfMulticastAddrAddIndSerFree },
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_IND */
#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_RES
    { CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_RES, CsrIpEtherIfMulticastAddrAddResSizeof, CsrIpEtherIfMulticastAddrAddResSer, CsrIpEtherIfMulticastAddrAddResDes, CsrIpEtherIfMulticastAddrAddResSerFree },
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_RES */
#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_IND
    { CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_IND, CsrIpEtherIfMulticastAddrFlushIndSizeof, CsrIpEtherIfMulticastAddrFlushIndSer, CsrIpEtherIfMulticastAddrFlushIndDes, CsrIpEtherIfMulticastAddrFlushIndSerFree },
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_IND */
#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_RES
    { CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_RES, CsrIpEtherIfMulticastAddrFlushResSizeof, CsrIpEtherIfMulticastAddrFlushResSer, CsrIpEtherIfMulticastAddrFlushResDes, CsrIpEtherIfMulticastAddrFlushResSerFree },
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_RES */
#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_IND
    { CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_IND, CsrIpEtherIfMulticastAddrRemoveIndSizeof, CsrIpEtherIfMulticastAddrRemoveIndSer, CsrIpEtherIfMulticastAddrRemoveIndDes, CsrIpEtherIfMulticastAddrRemoveIndSerFree },
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_IND */
#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_RES
    { CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_RES, CsrIpEtherIfMulticastAddrRemoveResSizeof, CsrIpEtherIfMulticastAddrRemoveResSer, CsrIpEtherIfMulticastAddrRemoveResDes, CsrIpEtherIfMulticastAddrRemoveResSerFree },
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_RES */
#ifndef EXCLUDE_CSR_IP_ETHER_IF_REMOVE_CFM
    { CSR_IP_ETHER_IF_REMOVE_CFM, CsrIpEtherIfRemoveCfmSizeof, CsrIpEtherIfRemoveCfmSer, CsrIpEtherIfRemoveCfmDes, CsrIpEtherIfRemoveCfmSerFree },
#endif /* EXCLUDE_CSR_IP_ETHER_IF_REMOVE_CFM */
#ifndef EXCLUDE_CSR_IP_ETHER_IF_REMOVE_REQ
    { CSR_IP_ETHER_IF_REMOVE_REQ, CsrIpEtherIfRemoveReqSizeof, CsrIpEtherIfRemoveReqSer, CsrIpEtherIfRemoveReqDes, CsrIpEtherIfRemoveReqSerFree },
#endif /* EXCLUDE_CSR_IP_ETHER_IF_REMOVE_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrIpEtherConverterInit(void)
{
    CsrMsgConvInsert(CSR_IP_ETHER_PRIM, csr_ip_ether_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_ip_ether_conv_info = {
    CSR_IP_ETHER_PRIM,
    "CSR_IP_ETHER_PRIM",
    csr_ip_ether_conv_lut
};

CsrLogPrimitiveInformation* CsrIpEtherTechInfoGet(void)
{
    return &csr_ip_ether_conv_info;
}
#endif
#endif

