#ifndef CSR_IP_IFCONFIG_SERIALIZE_H__
#define CSR_IP_IFCONFIG_SERIALIZE_H__
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

void CsrIp_ifconfigPfree(void *ptr);

#ifndef EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_CFM
CsrSize CsrIpIfconfigSubscribeCfmSizeof(void *msg);
CsrUint8 *CsrIpIfconfigSubscribeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpIfconfigSubscribeCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrIpIfconfigSubscribeCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_REQ
CsrSize CsrIpIfconfigUnsubscribeReqSizeof(void *msg);
CsrUint8 *CsrIpIfconfigUnsubscribeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpIfconfigUnsubscribeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpIfconfigUnsubscribeReqSerFree CsrIp_ifconfigPfree
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_DOWN_CFM
CsrSize CsrIpIfconfigDownCfmSizeof(void *msg);
CsrUint8 *CsrIpIfconfigDownCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpIfconfigDownCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpIfconfigDownCfmSerFree CsrIp_ifconfigPfree
#endif /* EXCLUDE_CSR_IP_IFCONFIG_DOWN_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UPDATE_IND
CsrSize CsrIpIfconfigUpdateIndSizeof(void *msg);
CsrUint8 *CsrIpIfconfigUpdateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpIfconfigUpdateIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpIfconfigUpdateIndSerFree CsrIp_ifconfigPfree
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UPDATE_IND */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_FORWARD_CFM
CsrSize CsrIpIfconfigForwardCfmSizeof(void *msg);
CsrUint8 *CsrIpIfconfigForwardCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpIfconfigForwardCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpIfconfigForwardCfmSerFree CsrIp_ifconfigPfree
#endif /* EXCLUDE_CSR_IP_IFCONFIG_FORWARD_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_FORWARD_REQ
CsrSize CsrIpIfconfigForwardReqSizeof(void *msg);
CsrUint8 *CsrIpIfconfigForwardReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpIfconfigForwardReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpIfconfigForwardReqSerFree CsrIp_ifconfigPfree
#endif /* EXCLUDE_CSR_IP_IFCONFIG_FORWARD_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UP_REQ
CsrSize CsrIpIfconfigUpReqSizeof(void *msg);
CsrUint8 *CsrIpIfconfigUpReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpIfconfigUpReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpIfconfigUpReqSerFree CsrIp_ifconfigPfree
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UP_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_DOWN_REQ
CsrSize CsrIpIfconfigDownReqSizeof(void *msg);
CsrUint8 *CsrIpIfconfigDownReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpIfconfigDownReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpIfconfigDownReqSerFree CsrIp_ifconfigPfree
#endif /* EXCLUDE_CSR_IP_IFCONFIG_DOWN_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_REQ
CsrSize CsrIpIfconfigSubscribeReqSizeof(void *msg);
CsrUint8 *CsrIpIfconfigSubscribeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpIfconfigSubscribeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpIfconfigSubscribeReqSerFree CsrIp_ifconfigPfree
#endif /* EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_CFM
CsrSize CsrIpIfconfigArpEntryAddCfmSizeof(void *msg);
CsrUint8 *CsrIpIfconfigArpEntryAddCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpIfconfigArpEntryAddCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpIfconfigArpEntryAddCfmSerFree CsrIp_ifconfigPfree
#endif /* EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UP_CFM
CsrSize CsrIpIfconfigUpCfmSizeof(void *msg);
CsrUint8 *CsrIpIfconfigUpCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpIfconfigUpCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpIfconfigUpCfmSerFree CsrIp_ifconfigPfree
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UP_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_CFM
CsrSize CsrIpIfconfigUnsubscribeCfmSizeof(void *msg);
CsrUint8 *CsrIpIfconfigUnsubscribeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpIfconfigUnsubscribeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpIfconfigUnsubscribeCfmSerFree CsrIp_ifconfigPfree
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_NAT_REQ
CsrSize CsrIpIfconfigNatReqSizeof(void *msg);
CsrUint8 *CsrIpIfconfigNatReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpIfconfigNatReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpIfconfigNatReqSerFree CsrIp_ifconfigPfree
#endif /* EXCLUDE_CSR_IP_IFCONFIG_NAT_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_REQ
CsrSize CsrIpIfconfigArpEntryAddReqSizeof(void *msg);
CsrUint8 *CsrIpIfconfigArpEntryAddReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpIfconfigArpEntryAddReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpIfconfigArpEntryAddReqSerFree CsrIp_ifconfigPfree
#endif /* EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_NAT_CFM
CsrSize CsrIpIfconfigNatCfmSizeof(void *msg);
CsrUint8 *CsrIpIfconfigNatCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpIfconfigNatCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpIfconfigNatCfmSerFree CsrIp_ifconfigPfree
#endif /* EXCLUDE_CSR_IP_IFCONFIG_NAT_CFM */

#ifdef __cplusplus
}
#endif

#endif
