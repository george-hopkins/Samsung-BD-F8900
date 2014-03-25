#ifndef CSR_IP_SOCKET_SEF_H__
#define CSR_IP_SOCKET_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_ip_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrIpSocketThreadShutdown(CsrIpInstanceData *instanceData);
void CsrIpSocketThread(CsrIpInstanceData *instanceData);

void CsrIpSocketDnsResolveNameReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketUdpNewReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketUdpBindReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketUdpDataReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketUdpCloseReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketUdpMulticastSubscribeReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketUdpMulticastUnsubscribeReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketUdpMulticastInterfaceReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketTcpNewReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketTcpBindReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketTcpListenReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketTcpConnectReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketTcpDataReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketTcpDataResHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketTcpCloseReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketTcpAbortReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketRawNewReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketRawBindReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketRawDataReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketRawCloseReqHandler(CsrIpInstanceData *instanceData);
void CsrIpSocketOptionsReqHandler(CsrIpInstanceData *instanceData);

#ifdef __cplusplus
}
#endif

#endif
