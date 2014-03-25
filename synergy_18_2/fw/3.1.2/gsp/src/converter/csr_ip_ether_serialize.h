#ifndef CSR_IP_ETHER_SERIALIZE_H__
#define CSR_IP_ETHER_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrIp_etherPfree(void *ptr);

#ifndef EXCLUDE_CSR_IP_ETHER_IF_ADD_CFM
CsrSize CsrIpEtherIfAddCfmSizeof(void *msg);
CsrUint8 *CsrIpEtherIfAddCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpEtherIfAddCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrIpEtherIfAddCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_IP_ETHER_IF_ADD_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_RESUME_REQ
CsrSize CsrIpEtherIfFlowControlResumeReqSizeof(void *msg);
CsrUint8 *CsrIpEtherIfFlowControlResumeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpEtherIfFlowControlResumeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpEtherIfFlowControlResumeReqSerFree CsrIp_etherPfree
#endif /* EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_RESUME_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_CFM
CsrSize CsrIpEtherIfLinkUpCfmSizeof(void *msg);
CsrUint8 *CsrIpEtherIfLinkUpCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpEtherIfLinkUpCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpEtherIfLinkUpCfmSerFree CsrIp_etherPfree
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_RES
CsrSize CsrIpEtherIfMulticastAddrFlushResSizeof(void *msg);
CsrUint8 *CsrIpEtherIfMulticastAddrFlushResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpEtherIfMulticastAddrFlushResDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpEtherIfMulticastAddrFlushResSerFree CsrIp_etherPfree
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_RES */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_REQ
CsrSize CsrIpEtherIfLinkUpReqSizeof(void *msg);
CsrUint8 *CsrIpEtherIfLinkUpReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpEtherIfLinkUpReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpEtherIfLinkUpReqSerFree CsrIp_etherPfree
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_REQ
CsrSize CsrIpEtherIfLinkDownReqSizeof(void *msg);
CsrUint8 *CsrIpEtherIfLinkDownReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpEtherIfLinkDownReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpEtherIfLinkDownReqSerFree CsrIp_etherPfree
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_IND
CsrSize CsrIpEtherIfMulticastAddrFlushIndSizeof(void *msg);
CsrUint8 *CsrIpEtherIfMulticastAddrFlushIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpEtherIfMulticastAddrFlushIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpEtherIfMulticastAddrFlushIndSerFree CsrIp_etherPfree
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_IND */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_REMOVE_REQ
CsrSize CsrIpEtherIfRemoveReqSizeof(void *msg);
CsrUint8 *CsrIpEtherIfRemoveReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpEtherIfRemoveReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpEtherIfRemoveReqSerFree CsrIp_etherPfree
#endif /* EXCLUDE_CSR_IP_ETHER_IF_REMOVE_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_CFM
CsrSize CsrIpEtherIfLinkDownCfmSizeof(void *msg);
CsrUint8 *CsrIpEtherIfLinkDownCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpEtherIfLinkDownCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpEtherIfLinkDownCfmSerFree CsrIp_etherPfree
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_IND
CsrSize CsrIpEtherIfMulticastAddrRemoveIndSizeof(void *msg);
CsrUint8 *CsrIpEtherIfMulticastAddrRemoveIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpEtherIfMulticastAddrRemoveIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpEtherIfMulticastAddrRemoveIndSerFree CsrIp_etherPfree
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_IND */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_REMOVE_CFM
CsrSize CsrIpEtherIfRemoveCfmSizeof(void *msg);
CsrUint8 *CsrIpEtherIfRemoveCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpEtherIfRemoveCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpEtherIfRemoveCfmSerFree CsrIp_etherPfree
#endif /* EXCLUDE_CSR_IP_ETHER_IF_REMOVE_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_RES
CsrSize CsrIpEtherIfMulticastAddrAddResSizeof(void *msg);
CsrUint8 *CsrIpEtherIfMulticastAddrAddResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpEtherIfMulticastAddrAddResDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpEtherIfMulticastAddrAddResSerFree CsrIp_etherPfree
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_RES */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_RES
CsrSize CsrIpEtherIfMulticastAddrRemoveResSizeof(void *msg);
CsrUint8 *CsrIpEtherIfMulticastAddrRemoveResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpEtherIfMulticastAddrRemoveResDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpEtherIfMulticastAddrRemoveResSerFree CsrIp_etherPfree
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_RES */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_IND
CsrSize CsrIpEtherIfMulticastAddrAddIndSizeof(void *msg);
CsrUint8 *CsrIpEtherIfMulticastAddrAddIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpEtherIfMulticastAddrAddIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpEtherIfMulticastAddrAddIndSerFree CsrIp_etherPfree
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_IND */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_ADD_REQ
CsrSize CsrIpEtherIfAddReqSizeof(void *msg);
CsrUint8 *CsrIpEtherIfAddReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpEtherIfAddReqDes(CsrUint8 *buffer, CsrSize length);
void CsrIpEtherIfAddReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_IP_ETHER_IF_ADD_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_PAUSE_REQ
CsrSize CsrIpEtherIfFlowControlPauseReqSizeof(void *msg);
CsrUint8 *CsrIpEtherIfFlowControlPauseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpEtherIfFlowControlPauseReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpEtherIfFlowControlPauseReqSerFree CsrIp_etherPfree
#endif /* EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_PAUSE_REQ */

#ifdef __cplusplus
}
#endif

#endif
