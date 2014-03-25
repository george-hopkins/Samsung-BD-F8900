#ifndef CSR_DHCP_SERVER_SEF_H__
#define CSR_DHCP_SERVER_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_dhcp_server_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void CsrDhcpServerCreateHandler(CsrDhcpServerInstanceData *instanceData);
extern void CsrDhcpServerDestroyHandler(CsrDhcpServerInstanceData *instanceData);
extern void CsrDhcpServerStartHandler(CsrDhcpServerInstanceData *instanceData);
extern void CsrDhcpServerStopHandler(CsrDhcpServerInstanceData *instanceData);
extern void CsrDhcpServerLeasesGetHandler(CsrDhcpServerInstanceData *instanceData);
extern void CsrDhcpServerLeasesSetHandler(CsrDhcpServerInstanceData *instanceData);

extern void CsrDhcpServerIpSocketUdpNewHandler(CsrDhcpServerInstanceData *instanceData);
extern void CsrDhcpServerIpSocketUdpBindHandler(CsrDhcpServerInstanceData *instanceData);
extern void CsrDhcpServerIpSocketUdpDataIndHandler(CsrDhcpServerInstanceData *instanceData);
extern void CsrDhcpServerIpSocketUdpDataHandler(CsrDhcpServerInstanceData *instanceData);
extern void CsrDhcpServerIpSocketOptionsHandler(CsrDhcpServerInstanceData *instanceData);

#ifdef __cplusplus
}
#endif

#endif
