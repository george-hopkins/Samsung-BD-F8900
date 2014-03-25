#ifndef CSR_DHCP_SERVER_SERIALIZE_H__
#define CSR_DHCP_SERVER_SERIALIZE_H__
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

void CsrDhcp_serverPfree(void *ptr);

#ifndef EXCLUDE_CSR_DHCP_SERVER_STOP_REQ
CsrSize CsrDhcpServerStopReqSizeof(void *msg);
CsrUint8 *CsrDhcpServerStopReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDhcpServerStopReqDes(CsrUint8 *buffer, CsrSize length);
void CsrDhcpServerStopReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_DHCP_SERVER_STOP_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_CREATE_REQ
CsrSize CsrDhcpServerCreateReqSizeof(void *msg);
CsrUint8 *CsrDhcpServerCreateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDhcpServerCreateReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrDhcpServerCreateReqSerFree CsrDhcp_serverPfree
#endif /* EXCLUDE_CSR_DHCP_SERVER_CREATE_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_DESTROY_REQ
CsrSize CsrDhcpServerDestroyReqSizeof(void *msg);
CsrUint8 *CsrDhcpServerDestroyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDhcpServerDestroyReqDes(CsrUint8 *buffer, CsrSize length);
void CsrDhcpServerDestroyReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_DHCP_SERVER_DESTROY_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_REQ
CsrSize CsrDhcpServerLeasesSetReqSizeof(void *msg);
CsrUint8 *CsrDhcpServerLeasesSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDhcpServerLeasesSetReqDes(CsrUint8 *buffer, CsrSize length);
void CsrDhcpServerLeasesSetReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_CREATE_CFM
CsrSize CsrDhcpServerCreateCfmSizeof(void *msg);
CsrUint8 *CsrDhcpServerCreateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDhcpServerCreateCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrDhcpServerCreateCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_DHCP_SERVER_CREATE_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_START_CFM
CsrSize CsrDhcpServerStartCfmSizeof(void *msg);
CsrUint8 *CsrDhcpServerStartCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDhcpServerStartCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrDhcpServerStartCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_DHCP_SERVER_START_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_STOP_CFM
CsrSize CsrDhcpServerStopCfmSizeof(void *msg);
CsrUint8 *CsrDhcpServerStopCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDhcpServerStopCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrDhcpServerStopCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_DHCP_SERVER_STOP_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_ROGUE_IP_ADDRESS_IND
CsrSize CsrDhcpServerRogueIpAddressIndSizeof(void *msg);
CsrUint8 *CsrDhcpServerRogueIpAddressIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDhcpServerRogueIpAddressIndDes(CsrUint8 *buffer, CsrSize length);
void CsrDhcpServerRogueIpAddressIndSerFree(void *msg);
#endif /* EXCLUDE_CSR_DHCP_SERVER_ROGUE_IP_ADDRESS_IND */

#ifndef EXCLUDE_CSR_DHCP_SERVER_START_REQ
CsrSize CsrDhcpServerStartReqSizeof(void *msg);
CsrUint8 *CsrDhcpServerStartReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDhcpServerStartReqDes(CsrUint8 *buffer, CsrSize length);
void CsrDhcpServerStartReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_DHCP_SERVER_START_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_CFM
CsrSize CsrDhcpServerLeasesGetCfmSizeof(void *msg);
CsrUint8 *CsrDhcpServerLeasesGetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDhcpServerLeasesGetCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrDhcpServerLeasesGetCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_REQ
CsrSize CsrDhcpServerLeasesGetReqSizeof(void *msg);
CsrUint8 *CsrDhcpServerLeasesGetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDhcpServerLeasesGetReqDes(CsrUint8 *buffer, CsrSize length);
void CsrDhcpServerLeasesGetReqSerFree(void *msg);
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_DESTROY_CFM
CsrSize CsrDhcpServerDestroyCfmSizeof(void *msg);
CsrUint8 *CsrDhcpServerDestroyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDhcpServerDestroyCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrDhcpServerDestroyCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_DHCP_SERVER_DESTROY_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_CFM
CsrSize CsrDhcpServerLeasesSetCfmSizeof(void *msg);
CsrUint8 *CsrDhcpServerLeasesSetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrDhcpServerLeasesSetCfmDes(CsrUint8 *buffer, CsrSize length);
void CsrDhcpServerLeasesSetCfmSerFree(void *msg);
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_CFM */

#ifdef __cplusplus
}
#endif

#endif
