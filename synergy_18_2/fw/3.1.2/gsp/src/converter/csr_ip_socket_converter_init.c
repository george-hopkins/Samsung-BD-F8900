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
#include "csr_ip_socket_converter_init.h"
#ifndef EXCLUDE_CSR_IP_SOCKET_MODULE
#include "csr_ip_socket_serialize.h"
#include "csr_ip_socket_prim.h"

static CsrMsgConvMsgEntry csr_ip_socket_conv_lut[] = {
#ifndef EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_CFM
    { CSR_IP_SOCKET_DNS_RESOLVE_NAME_CFM, CsrIpSocketDnsResolveNameCfmSizeof, CsrIpSocketDnsResolveNameCfmSer, CsrIpSocketDnsResolveNameCfmDes, CsrIpSocketDnsResolveNameCfmSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_CFM */
#ifndef EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_REQ
    { CSR_IP_SOCKET_DNS_RESOLVE_NAME_REQ, CsrIpSocketDnsResolveNameReqSizeof, CsrIpSocketDnsResolveNameReqSer, CsrIpSocketDnsResolveNameReqDes, CsrIpSocketDnsResolveNameReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_DNS_RESOLVE_NAME_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_OPTIONS_CFM
    { CSR_IP_SOCKET_OPTIONS_CFM, CsrIpSocketOptionsCfmSizeof, CsrIpSocketOptionsCfmSer, CsrIpSocketOptionsCfmDes, CsrIpSocketOptionsCfmSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_OPTIONS_CFM */
#ifndef EXCLUDE_CSR_IP_SOCKET_OPTIONS_REQ
    { CSR_IP_SOCKET_OPTIONS_REQ, CsrIpSocketOptionsReqSizeof, CsrIpSocketOptionsReqSer, CsrIpSocketOptionsReqDes, CsrIpSocketOptionsReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_OPTIONS_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_BIND_CFM
    { CSR_IP_SOCKET_RAW_BIND_CFM, CsrIpSocketRawBindCfmSizeof, CsrIpSocketRawBindCfmSer, CsrIpSocketRawBindCfmDes, CsrIpSocketRawBindCfmSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_BIND_CFM */
#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_BIND_REQ
    { CSR_IP_SOCKET_RAW_BIND_REQ, CsrIpSocketRawBindReqSizeof, CsrIpSocketRawBindReqSer, CsrIpSocketRawBindReqDes, CsrIpSocketRawBindReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_BIND_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_CLOSE_REQ
    { CSR_IP_SOCKET_RAW_CLOSE_REQ, CsrIpSocketRawCloseReqSizeof, CsrIpSocketRawCloseReqSer, CsrIpSocketRawCloseReqDes, CsrIpSocketRawCloseReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_CLOSE_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_CFM
    { CSR_IP_SOCKET_RAW_DATA_CFM, CsrIpSocketRawDataCfmSizeof, CsrIpSocketRawDataCfmSer, CsrIpSocketRawDataCfmDes, CsrIpSocketRawDataCfmSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_CFM */
#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_IND
    { CSR_IP_SOCKET_RAW_DATA_IND, CsrIpSocketRawDataIndSizeof, CsrIpSocketRawDataIndSer, CsrIpSocketRawDataIndDes, CsrIpSocketRawDataIndSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_IND */
#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_DATA_REQ
    { CSR_IP_SOCKET_RAW_DATA_REQ, CsrIpSocketRawDataReqSizeof, CsrIpSocketRawDataReqSer, CsrIpSocketRawDataReqDes, CsrIpSocketRawDataReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_DATA_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_NEW_CFM
    { CSR_IP_SOCKET_RAW_NEW_CFM, CsrIpSocketRawNewCfmSizeof, CsrIpSocketRawNewCfmSer, CsrIpSocketRawNewCfmDes, CsrIpSocketRawNewCfmSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_NEW_CFM */
#ifndef EXCLUDE_CSR_IP_SOCKET_RAW_NEW_REQ
    { CSR_IP_SOCKET_RAW_NEW_REQ, CsrIpSocketRawNewReqSizeof, CsrIpSocketRawNewReqSer, CsrIpSocketRawNewReqDes, CsrIpSocketRawNewReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_RAW_NEW_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_ABORT_REQ
    { CSR_IP_SOCKET_TCP_ABORT_REQ, CsrIpSocketTcpAbortReqSizeof, CsrIpSocketTcpAbortReqSer, CsrIpSocketTcpAbortReqDes, CsrIpSocketTcpAbortReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_ABORT_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_ACCEPT_IND
    { CSR_IP_SOCKET_TCP_ACCEPT_IND, CsrIpSocketTcpAcceptIndSizeof, CsrIpSocketTcpAcceptIndSer, CsrIpSocketTcpAcceptIndDes, CsrIpSocketTcpAcceptIndSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_ACCEPT_IND */
#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_BIND_CFM
    { CSR_IP_SOCKET_TCP_BIND_CFM, CsrIpSocketTcpBindCfmSizeof, CsrIpSocketTcpBindCfmSer, CsrIpSocketTcpBindCfmDes, CsrIpSocketTcpBindCfmSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_BIND_CFM */
#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_BIND_REQ
    { CSR_IP_SOCKET_TCP_BIND_REQ, CsrIpSocketTcpBindReqSizeof, CsrIpSocketTcpBindReqSer, CsrIpSocketTcpBindReqDes, CsrIpSocketTcpBindReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_BIND_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_IND
    { CSR_IP_SOCKET_TCP_CLOSE_IND, CsrIpSocketTcpCloseIndSizeof, CsrIpSocketTcpCloseIndSer, CsrIpSocketTcpCloseIndDes, CsrIpSocketTcpCloseIndSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_IND */
#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_REQ
    { CSR_IP_SOCKET_TCP_CLOSE_REQ, CsrIpSocketTcpCloseReqSizeof, CsrIpSocketTcpCloseReqSer, CsrIpSocketTcpCloseReqDes, CsrIpSocketTcpCloseReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CLOSE_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_CFM
    { CSR_IP_SOCKET_TCP_CONNECT_CFM, CsrIpSocketTcpConnectCfmSizeof, CsrIpSocketTcpConnectCfmSer, CsrIpSocketTcpConnectCfmDes, CsrIpSocketTcpConnectCfmSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_CFM */
#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_REQ
    { CSR_IP_SOCKET_TCP_CONNECT_REQ, CsrIpSocketTcpConnectReqSizeof, CsrIpSocketTcpConnectReqSer, CsrIpSocketTcpConnectReqDes, CsrIpSocketTcpConnectReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_CONNECT_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_CFM
    { CSR_IP_SOCKET_TCP_DATA_CFM, CsrIpSocketTcpDataCfmSizeof, CsrIpSocketTcpDataCfmSer, CsrIpSocketTcpDataCfmDes, CsrIpSocketTcpDataCfmSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_CFM */
#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_IND
    { CSR_IP_SOCKET_TCP_DATA_IND, CsrIpSocketTcpDataIndSizeof, CsrIpSocketTcpDataIndSer, CsrIpSocketTcpDataIndDes, CsrIpSocketTcpDataIndSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_IND */
#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_REQ
    { CSR_IP_SOCKET_TCP_DATA_REQ, CsrIpSocketTcpDataReqSizeof, CsrIpSocketTcpDataReqSer, CsrIpSocketTcpDataReqDes, CsrIpSocketTcpDataReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_DATA_RES
    { CSR_IP_SOCKET_TCP_DATA_RES, CsrIpSocketTcpDataResSizeof, CsrIpSocketTcpDataResSer, CsrIpSocketTcpDataResDes, CsrIpSocketTcpDataResSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_DATA_RES */
#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_CFM
    { CSR_IP_SOCKET_TCP_LISTEN_CFM, CsrIpSocketTcpListenCfmSizeof, CsrIpSocketTcpListenCfmSer, CsrIpSocketTcpListenCfmDes, CsrIpSocketTcpListenCfmSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_CFM */
#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_REQ
    { CSR_IP_SOCKET_TCP_LISTEN_REQ, CsrIpSocketTcpListenReqSizeof, CsrIpSocketTcpListenReqSer, CsrIpSocketTcpListenReqDes, CsrIpSocketTcpListenReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_LISTEN_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_NEW_CFM
    { CSR_IP_SOCKET_TCP_NEW_CFM, CsrIpSocketTcpNewCfmSizeof, CsrIpSocketTcpNewCfmSer, CsrIpSocketTcpNewCfmDes, CsrIpSocketTcpNewCfmSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_NEW_CFM */
#ifndef EXCLUDE_CSR_IP_SOCKET_TCP_NEW_REQ
    { CSR_IP_SOCKET_TCP_NEW_REQ, CsrIpSocketTcpNewReqSizeof, CsrIpSocketTcpNewReqSer, CsrIpSocketTcpNewReqDes, CsrIpSocketTcpNewReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_TCP_NEW_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_BIND_CFM
    { CSR_IP_SOCKET_UDP_BIND_CFM, CsrIpSocketUdpBindCfmSizeof, CsrIpSocketUdpBindCfmSer, CsrIpSocketUdpBindCfmDes, CsrIpSocketUdpBindCfmSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_BIND_CFM */
#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_BIND_REQ
    { CSR_IP_SOCKET_UDP_BIND_REQ, CsrIpSocketUdpBindReqSizeof, CsrIpSocketUdpBindReqSer, CsrIpSocketUdpBindReqDes, CsrIpSocketUdpBindReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_BIND_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_CLOSE_REQ
    { CSR_IP_SOCKET_UDP_CLOSE_REQ, CsrIpSocketUdpCloseReqSizeof, CsrIpSocketUdpCloseReqSer, CsrIpSocketUdpCloseReqDes, CsrIpSocketUdpCloseReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_CLOSE_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_CFM
    { CSR_IP_SOCKET_UDP_DATA_CFM, CsrIpSocketUdpDataCfmSizeof, CsrIpSocketUdpDataCfmSer, CsrIpSocketUdpDataCfmDes, CsrIpSocketUdpDataCfmSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_CFM */
#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_IND
    { CSR_IP_SOCKET_UDP_DATA_IND, CsrIpSocketUdpDataIndSizeof, CsrIpSocketUdpDataIndSer, CsrIpSocketUdpDataIndDes, CsrIpSocketUdpDataIndSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_IND */
#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_DATA_REQ
    { CSR_IP_SOCKET_UDP_DATA_REQ, CsrIpSocketUdpDataReqSizeof, CsrIpSocketUdpDataReqSer, CsrIpSocketUdpDataReqDes, CsrIpSocketUdpDataReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_DATA_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_CFM
    { CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_CFM, CsrIpSocketUdpMulticastInterfaceCfmSizeof, CsrIpSocketUdpMulticastInterfaceCfmSer, CsrIpSocketUdpMulticastInterfaceCfmDes, CsrIpSocketUdpMulticastInterfaceCfmSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_CFM */
#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_REQ
    { CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_REQ, CsrIpSocketUdpMulticastInterfaceReqSizeof, CsrIpSocketUdpMulticastInterfaceReqSer, CsrIpSocketUdpMulticastInterfaceReqDes, CsrIpSocketUdpMulticastInterfaceReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_INTERFACE_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_CFM
    { CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_CFM, CsrIpSocketUdpMulticastSubscribeCfmSizeof, CsrIpSocketUdpMulticastSubscribeCfmSer, CsrIpSocketUdpMulticastSubscribeCfmDes, CsrIpSocketUdpMulticastSubscribeCfmSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_CFM */
#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_REQ
    { CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_REQ, CsrIpSocketUdpMulticastSubscribeReqSizeof, CsrIpSocketUdpMulticastSubscribeReqSer, CsrIpSocketUdpMulticastSubscribeReqDes, CsrIpSocketUdpMulticastSubscribeReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_SUBSCRIBE_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_CFM
    { CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_CFM, CsrIpSocketUdpMulticastUnsubscribeCfmSizeof, CsrIpSocketUdpMulticastUnsubscribeCfmSer, CsrIpSocketUdpMulticastUnsubscribeCfmDes, CsrIpSocketUdpMulticastUnsubscribeCfmSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_CFM */
#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_REQ
    { CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_REQ, CsrIpSocketUdpMulticastUnsubscribeReqSizeof, CsrIpSocketUdpMulticastUnsubscribeReqSer, CsrIpSocketUdpMulticastUnsubscribeReqDes, CsrIpSocketUdpMulticastUnsubscribeReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_MULTICAST_UNSUBSCRIBE_REQ */
#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_NEW_CFM
    { CSR_IP_SOCKET_UDP_NEW_CFM, CsrIpSocketUdpNewCfmSizeof, CsrIpSocketUdpNewCfmSer, CsrIpSocketUdpNewCfmDes, CsrIpSocketUdpNewCfmSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_NEW_CFM */
#ifndef EXCLUDE_CSR_IP_SOCKET_UDP_NEW_REQ
    { CSR_IP_SOCKET_UDP_NEW_REQ, CsrIpSocketUdpNewReqSizeof, CsrIpSocketUdpNewReqSer, CsrIpSocketUdpNewReqDes, CsrIpSocketUdpNewReqSerFree },
#endif /* EXCLUDE_CSR_IP_SOCKET_UDP_NEW_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrIpSocketConverterInit(void)
{
    CsrMsgConvInsert(CSR_IP_SOCKET_PRIM, csr_ip_socket_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_ip_socket_conv_info = {
    CSR_IP_SOCKET_PRIM,
    "CSR_IP_SOCKET_PRIM",
    csr_ip_socket_conv_lut
};

CsrLogPrimitiveInformation* CsrIpSocketTechInfoGet(void)
{
    return &csr_ip_socket_conv_info;
}
#endif
#endif

