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
#include "csr_ip_ifconfig_converter_init.h"
#ifndef EXCLUDE_CSR_IP_IFCONFIG_MODULE
#include "csr_ip_ifconfig_serialize.h"
#include "csr_ip_ifconfig_prim.h"

static CsrMsgConvMsgEntry csr_ip_ifconfig_conv_lut[] = {
#ifndef EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_CFM
    { CSR_IP_IFCONFIG_ARP_ENTRY_ADD_CFM, CsrIpIfconfigArpEntryAddCfmSizeof, CsrIpIfconfigArpEntryAddCfmSer, CsrIpIfconfigArpEntryAddCfmDes, CsrIpIfconfigArpEntryAddCfmSerFree },
#endif /* EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_CFM */
#ifndef EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_REQ
    { CSR_IP_IFCONFIG_ARP_ENTRY_ADD_REQ, CsrIpIfconfigArpEntryAddReqSizeof, CsrIpIfconfigArpEntryAddReqSer, CsrIpIfconfigArpEntryAddReqDes, CsrIpIfconfigArpEntryAddReqSerFree },
#endif /* EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_REQ */
#ifndef EXCLUDE_CSR_IP_IFCONFIG_DOWN_CFM
    { CSR_IP_IFCONFIG_DOWN_CFM, CsrIpIfconfigDownCfmSizeof, CsrIpIfconfigDownCfmSer, CsrIpIfconfigDownCfmDes, CsrIpIfconfigDownCfmSerFree },
#endif /* EXCLUDE_CSR_IP_IFCONFIG_DOWN_CFM */
#ifndef EXCLUDE_CSR_IP_IFCONFIG_DOWN_REQ
    { CSR_IP_IFCONFIG_DOWN_REQ, CsrIpIfconfigDownReqSizeof, CsrIpIfconfigDownReqSer, CsrIpIfconfigDownReqDes, CsrIpIfconfigDownReqSerFree },
#endif /* EXCLUDE_CSR_IP_IFCONFIG_DOWN_REQ */
#ifndef EXCLUDE_CSR_IP_IFCONFIG_FORWARD_CFM
    { CSR_IP_IFCONFIG_FORWARD_CFM, CsrIpIfconfigForwardCfmSizeof, CsrIpIfconfigForwardCfmSer, CsrIpIfconfigForwardCfmDes, CsrIpIfconfigForwardCfmSerFree },
#endif /* EXCLUDE_CSR_IP_IFCONFIG_FORWARD_CFM */
#ifndef EXCLUDE_CSR_IP_IFCONFIG_FORWARD_REQ
    { CSR_IP_IFCONFIG_FORWARD_REQ, CsrIpIfconfigForwardReqSizeof, CsrIpIfconfigForwardReqSer, CsrIpIfconfigForwardReqDes, CsrIpIfconfigForwardReqSerFree },
#endif /* EXCLUDE_CSR_IP_IFCONFIG_FORWARD_REQ */
#ifndef EXCLUDE_CSR_IP_IFCONFIG_NAT_CFM
    { CSR_IP_IFCONFIG_NAT_CFM, CsrIpIfconfigNatCfmSizeof, CsrIpIfconfigNatCfmSer, CsrIpIfconfigNatCfmDes, CsrIpIfconfigNatCfmSerFree },
#endif /* EXCLUDE_CSR_IP_IFCONFIG_NAT_CFM */
#ifndef EXCLUDE_CSR_IP_IFCONFIG_NAT_REQ
    { CSR_IP_IFCONFIG_NAT_REQ, CsrIpIfconfigNatReqSizeof, CsrIpIfconfigNatReqSer, CsrIpIfconfigNatReqDes, CsrIpIfconfigNatReqSerFree },
#endif /* EXCLUDE_CSR_IP_IFCONFIG_NAT_REQ */
#ifndef EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_CFM
    { CSR_IP_IFCONFIG_SUBSCRIBE_CFM, CsrIpIfconfigSubscribeCfmSizeof, CsrIpIfconfigSubscribeCfmSer, CsrIpIfconfigSubscribeCfmDes, CsrIpIfconfigSubscribeCfmSerFree },
#endif /* EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_CFM */
#ifndef EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_REQ
    { CSR_IP_IFCONFIG_SUBSCRIBE_REQ, CsrIpIfconfigSubscribeReqSizeof, CsrIpIfconfigSubscribeReqSer, CsrIpIfconfigSubscribeReqDes, CsrIpIfconfigSubscribeReqSerFree },
#endif /* EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_REQ */
#ifndef EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_CFM
    { CSR_IP_IFCONFIG_UNSUBSCRIBE_CFM, CsrIpIfconfigUnsubscribeCfmSizeof, CsrIpIfconfigUnsubscribeCfmSer, CsrIpIfconfigUnsubscribeCfmDes, CsrIpIfconfigUnsubscribeCfmSerFree },
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_CFM */
#ifndef EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_REQ
    { CSR_IP_IFCONFIG_UNSUBSCRIBE_REQ, CsrIpIfconfigUnsubscribeReqSizeof, CsrIpIfconfigUnsubscribeReqSer, CsrIpIfconfigUnsubscribeReqDes, CsrIpIfconfigUnsubscribeReqSerFree },
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_REQ */
#ifndef EXCLUDE_CSR_IP_IFCONFIG_UP_CFM
    { CSR_IP_IFCONFIG_UP_CFM, CsrIpIfconfigUpCfmSizeof, CsrIpIfconfigUpCfmSer, CsrIpIfconfigUpCfmDes, CsrIpIfconfigUpCfmSerFree },
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UP_CFM */
#ifndef EXCLUDE_CSR_IP_IFCONFIG_UP_REQ
    { CSR_IP_IFCONFIG_UP_REQ, CsrIpIfconfigUpReqSizeof, CsrIpIfconfigUpReqSer, CsrIpIfconfigUpReqDes, CsrIpIfconfigUpReqSerFree },
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UP_REQ */
#ifndef EXCLUDE_CSR_IP_IFCONFIG_UPDATE_IND
    { CSR_IP_IFCONFIG_UPDATE_IND, CsrIpIfconfigUpdateIndSizeof, CsrIpIfconfigUpdateIndSer, CsrIpIfconfigUpdateIndDes, CsrIpIfconfigUpdateIndSerFree },
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UPDATE_IND */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrIpIfconfigConverterInit(void)
{
    CsrMsgConvInsert(CSR_IP_IFCONFIG_PRIM, csr_ip_ifconfig_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_ip_ifconfig_conv_info = {
    CSR_IP_IFCONFIG_PRIM,
    "CSR_IP_IFCONFIG_PRIM",
    csr_ip_ifconfig_conv_lut
};

CsrLogPrimitiveInformation* CsrIpIfconfigTechInfoGet(void)
{
    return &csr_ip_ifconfig_conv_info;
}
#endif
#endif

