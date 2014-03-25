#ifndef CSR_IP_SOCKET_SERIALIZE_H__
#define CSR_IP_SOCKET_SERIALIZE_H__
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

void CsrIp_socketPfree(void *ptr);

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_REQ
CsrSize CsrIpSocketUdpMulticastUnsubscribeReqSizeof(void *msg);
CsrUint8 *CsrIpSocketUdpMulticastUnsubscribeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketUdpMulticastUnsubscribeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketUdpMulticastUnsubscribeReqSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_OPTIONS_REQ
CsrSize CsrIpSocketOptionsReqSizeof(void *msg);
CsrUint8 *CsrIpSocketOptionsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketOptionsReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketOptionsReqSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_OPTIONS_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_OPTIONS_CFM
CsrSize CsrIpSocketOptionsCfmSizeof(void *msg);
CsrUint8 *CsrIpSocketOptionsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketOptionsCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketOptionsCfmSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_OPTIONS_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_BIND_CFM
CsrSize CsrIpSocketRawBindCfmSizeof(void *msg);
CsrUint8 *CsrIpSocketRawBindCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketRawBindCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketRawBindCfmSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_BIND_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_IND
CsrSize CsrIpSocketRawDataIndSizeof(void *msg);
CsrUint8 *CsrIpSocketRawDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketRawDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrIpSocketRawDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_REQ
CsrSize CsrIpSocketUdpMulticastSubscribeReqSizeof(void *msg);
CsrUint8 *CsrIpSocketUdpMulticastSubscribeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketUdpMulticastSubscribeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketUdpMulticastSubscribeReqSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_CFM
CsrSize CsrIpSocketRawDataCfmSizeof(void *msg);
CsrUint8 *CsrIpSocketRawDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketRawDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketRawDataCfmSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_NEW_CFM
CsrSize CsrIpSocketTcpNewCfmSizeof(void *msg);
CsrUint8 *CsrIpSocketTcpNewCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketTcpNewCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketTcpNewCfmSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_NEW_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_REQ
CsrSize CsrIpSocketUdpDataReqSizeof(void *msg);
CsrUint8 *CsrIpSocketUdpDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketUdpDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrIpSocketUdpDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_ABORT_REQ
CsrSize CsrIpSocketTcpAbortReqSizeof(void *msg);
CsrUint8 *CsrIpSocketTcpAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketTcpAbortReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketTcpAbortReqSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_ABORT_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_NEW_REQ
CsrSize CsrIpSocketTcpNewReqSizeof(void *msg);
CsrUint8 *CsrIpSocketTcpNewReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketTcpNewReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketTcpNewReqSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_NEW_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_CFM
CsrSize CsrIpSocketUdpMulticastUnsubscribeCfmSizeof(void *msg);
CsrUint8 *CsrIpSocketUdpMulticastUnsubscribeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketUdpMulticastUnsubscribeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketUdpMulticastUnsubscribeCfmSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_REQ
CsrSize CsrIpSocketTcpCloseReqSizeof(void *msg);
CsrUint8 *CsrIpSocketTcpCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketTcpCloseReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketTcpCloseReqSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_REQ
CsrSize CsrIpSocketTcpConnectReqSizeof(void *msg);
CsrUint8 *CsrIpSocketTcpConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketTcpConnectReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketTcpConnectReqSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_REQ
CsrSize CsrIpSocketUdpMulticastInterfaceReqSizeof(void *msg);
CsrUint8 *CsrIpSocketUdpMulticastInterfaceReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketUdpMulticastInterfaceReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketUdpMulticastInterfaceReqSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_REQ
CsrSize CsrIpSocketDnsResolveNameReqSizeof(void *msg);
CsrUint8 *CsrIpSocketDnsResolveNameReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketDnsResolveNameReqDes(CsrUint8 *buffer, CsrSize length);
void CsrIpSocketDnsResolveNameReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_CFM
CsrSize CsrIpSocketUdpMulticastInterfaceCfmSizeof(void *msg);
CsrUint8 *CsrIpSocketUdpMulticastInterfaceCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketUdpMulticastInterfaceCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketUdpMulticastInterfaceCfmSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_NEW_REQ
CsrSize CsrIpSocketRawNewReqSizeof(void *msg);
CsrUint8 *CsrIpSocketRawNewReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketRawNewReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketRawNewReqSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_NEW_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_CFM
CsrSize CsrIpSocketUdpMulticastSubscribeCfmSizeof(void *msg);
CsrUint8 *CsrIpSocketUdpMulticastSubscribeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketUdpMulticastSubscribeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketUdpMulticastSubscribeCfmSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_IND
CsrSize CsrIpSocketUdpDataIndSizeof(void *msg);
CsrUint8 *CsrIpSocketUdpDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketUdpDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrIpSocketUdpDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_BIND_CFM
CsrSize CsrIpSocketTcpBindCfmSizeof(void *msg);
CsrUint8 *CsrIpSocketTcpBindCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketTcpBindCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketTcpBindCfmSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_BIND_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_BIND_REQ
CsrSize CsrIpSocketUdpBindReqSizeof(void *msg);
CsrUint8 *CsrIpSocketUdpBindReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketUdpBindReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketUdpBindReqSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_BIND_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_BIND_CFM
CsrSize CsrIpSocketUdpBindCfmSizeof(void *msg);
CsrUint8 *CsrIpSocketUdpBindCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketUdpBindCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketUdpBindCfmSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_BIND_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_IND
CsrSize CsrIpSocketTcpCloseIndSizeof(void *msg);
CsrUint8 *CsrIpSocketTcpCloseIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketTcpCloseIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketTcpCloseIndSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_ACCEPT_IND
CsrSize CsrIpSocketTcpAcceptIndSizeof(void *msg);
CsrUint8 *CsrIpSocketTcpAcceptIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketTcpAcceptIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketTcpAcceptIndSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_ACCEPT_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_CFM
CsrSize CsrIpSocketTcpDataCfmSizeof(void *msg);
CsrUint8 *CsrIpSocketTcpDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketTcpDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketTcpDataCfmSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_BIND_REQ
CsrSize CsrIpSocketTcpBindReqSizeof(void *msg);
CsrUint8 *CsrIpSocketTcpBindReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketTcpBindReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketTcpBindReqSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_BIND_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_NEW_CFM
CsrSize CsrIpSocketRawNewCfmSizeof(void *msg);
CsrUint8 *CsrIpSocketRawNewCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketRawNewCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketRawNewCfmSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_NEW_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_BIND_REQ
CsrSize CsrIpSocketRawBindReqSizeof(void *msg);
CsrUint8 *CsrIpSocketRawBindReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketRawBindReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketRawBindReqSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_BIND_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_IND
CsrSize CsrIpSocketTcpDataIndSizeof(void *msg);
CsrUint8 *CsrIpSocketTcpDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketTcpDataIndDes(CsrUint8 *buffer, CsrSize length);
void CsrIpSocketTcpDataIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_IND */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_NEW_CFM
CsrSize CsrIpSocketUdpNewCfmSizeof(void *msg);
CsrUint8 *CsrIpSocketUdpNewCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketUdpNewCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketUdpNewCfmSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_NEW_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_CLOSE_REQ
CsrSize CsrIpSocketRawCloseReqSizeof(void *msg);
CsrUint8 *CsrIpSocketRawCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketRawCloseReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketRawCloseReqSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_CLOSE_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_CFM
CsrSize CsrIpSocketDnsResolveNameCfmSizeof(void *msg);
CsrUint8 *CsrIpSocketDnsResolveNameCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketDnsResolveNameCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrIpSocketDnsResolveNameCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_REQ
CsrSize CsrIpSocketTcpDataReqSizeof(void *msg);
CsrUint8 *CsrIpSocketTcpDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketTcpDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrIpSocketTcpDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_CFM
CsrSize CsrIpSocketTcpListenCfmSizeof(void *msg);
CsrUint8 *CsrIpSocketTcpListenCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketTcpListenCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketTcpListenCfmSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_NEW_REQ
CsrSize CsrIpSocketUdpNewReqSizeof(void *msg);
CsrUint8 *CsrIpSocketUdpNewReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketUdpNewReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketUdpNewReqSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_NEW_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_REQ
CsrSize CsrIpSocketRawDataReqSizeof(void *msg);
CsrUint8 *CsrIpSocketRawDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketRawDataReqDes(CsrUint8 *buffer, CsrSize length);
void CsrIpSocketRawDataReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_REQ
CsrSize CsrIpSocketTcpListenReqSizeof(void *msg);
CsrUint8 *CsrIpSocketTcpListenReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketTcpListenReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketTcpListenReqSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_REQ */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_CFM
CsrSize CsrIpSocketTcpConnectCfmSizeof(void *msg);
CsrUint8 *CsrIpSocketTcpConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketTcpConnectCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketTcpConnectCfmSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_RES
CsrSize CsrIpSocketTcpDataResSizeof(void *msg);
CsrUint8 *CsrIpSocketTcpDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketTcpDataResDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketTcpDataResSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_RES */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_CFM
CsrSize CsrIpSocketUdpDataCfmSizeof(void *msg);
CsrUint8 *CsrIpSocketUdpDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketUdpDataCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketUdpDataCfmSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_CFM */

#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_CLOSE_REQ
CsrSize CsrIpSocketUdpCloseReqSizeof(void *msg);
CsrUint8 *CsrIpSocketUdpCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrIpSocketUdpCloseReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrIpSocketUdpCloseReqSerFree CsrIp_socketPfree
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_CLOSE_REQ */

#ifdef __cplusplus
}
#endif

#endif
