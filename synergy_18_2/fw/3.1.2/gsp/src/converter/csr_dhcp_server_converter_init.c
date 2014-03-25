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
#include "csr_dhcp_server_converter_init.h"
#ifndef EXCLUDE_CSR_DHCP_SERVER_MODULE
#include "csr_dhcp_server_serialize.h"
#include "csr_dhcp_server_prim.h"

static CsrMsgConvMsgEntry csr_dhcp_server_conv_lut[] = {
#ifndef EXCLUDE_CSR_DHCP_SERVER_CREATE_CFM
    { CSR_DHCP_SERVER_CREATE_CFM, CsrDhcpServerCreateCfmSizeof, CsrDhcpServerCreateCfmSer, CsrDhcpServerCreateCfmDes, CsrDhcpServerCreateCfmSerFree },
#endif /* EXCLUDE_CSR_DHCP_SERVER_CREATE_CFM */
#ifndef EXCLUDE_CSR_DHCP_SERVER_CREATE_REQ
    { CSR_DHCP_SERVER_CREATE_REQ, CsrDhcpServerCreateReqSizeof, CsrDhcpServerCreateReqSer, CsrDhcpServerCreateReqDes, CsrDhcpServerCreateReqSerFree },
#endif /* EXCLUDE_CSR_DHCP_SERVER_CREATE_REQ */
#ifndef EXCLUDE_CSR_DHCP_SERVER_DESTROY_CFM
    { CSR_DHCP_SERVER_DESTROY_CFM, CsrDhcpServerDestroyCfmSizeof, CsrDhcpServerDestroyCfmSer, CsrDhcpServerDestroyCfmDes, CsrDhcpServerDestroyCfmSerFree },
#endif /* EXCLUDE_CSR_DHCP_SERVER_DESTROY_CFM */
#ifndef EXCLUDE_CSR_DHCP_SERVER_DESTROY_REQ
    { CSR_DHCP_SERVER_DESTROY_REQ, CsrDhcpServerDestroyReqSizeof, CsrDhcpServerDestroyReqSer, CsrDhcpServerDestroyReqDes, CsrDhcpServerDestroyReqSerFree },
#endif /* EXCLUDE_CSR_DHCP_SERVER_DESTROY_REQ */
#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_CFM
    { CSR_DHCP_SERVER_LEASES_GET_CFM, CsrDhcpServerLeasesGetCfmSizeof, CsrDhcpServerLeasesGetCfmSer, CsrDhcpServerLeasesGetCfmDes, CsrDhcpServerLeasesGetCfmSerFree },
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_CFM */
#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_REQ
    { CSR_DHCP_SERVER_LEASES_GET_REQ, CsrDhcpServerLeasesGetReqSizeof, CsrDhcpServerLeasesGetReqSer, CsrDhcpServerLeasesGetReqDes, CsrDhcpServerLeasesGetReqSerFree },
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_REQ */
#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_CFM
    { CSR_DHCP_SERVER_LEASES_SET_CFM, CsrDhcpServerLeasesSetCfmSizeof, CsrDhcpServerLeasesSetCfmSer, CsrDhcpServerLeasesSetCfmDes, CsrDhcpServerLeasesSetCfmSerFree },
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_CFM */
#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_REQ
    { CSR_DHCP_SERVER_LEASES_SET_REQ, CsrDhcpServerLeasesSetReqSizeof, CsrDhcpServerLeasesSetReqSer, CsrDhcpServerLeasesSetReqDes, CsrDhcpServerLeasesSetReqSerFree },
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_REQ */
#ifndef EXCLUDE_CSR_DHCP_SERVER_ROGUE_IP_ADDRESS_IND
    { CSR_DHCP_SERVER_ROGUE_IP_ADDRESS_IND, CsrDhcpServerRogueIpAddressIndSizeof, CsrDhcpServerRogueIpAddressIndSer, CsrDhcpServerRogueIpAddressIndDes, CsrDhcpServerRogueIpAddressIndSerFree },
#endif /* EXCLUDE_CSR_DHCP_SERVER_ROGUE_IP_ADDRESS_IND */
#ifndef EXCLUDE_CSR_DHCP_SERVER_START_CFM
    { CSR_DHCP_SERVER_START_CFM, CsrDhcpServerStartCfmSizeof, CsrDhcpServerStartCfmSer, CsrDhcpServerStartCfmDes, CsrDhcpServerStartCfmSerFree },
#endif /* EXCLUDE_CSR_DHCP_SERVER_START_CFM */
#ifndef EXCLUDE_CSR_DHCP_SERVER_START_REQ
    { CSR_DHCP_SERVER_START_REQ, CsrDhcpServerStartReqSizeof, CsrDhcpServerStartReqSer, CsrDhcpServerStartReqDes, CsrDhcpServerStartReqSerFree },
#endif /* EXCLUDE_CSR_DHCP_SERVER_START_REQ */
#ifndef EXCLUDE_CSR_DHCP_SERVER_STOP_CFM
    { CSR_DHCP_SERVER_STOP_CFM, CsrDhcpServerStopCfmSizeof, CsrDhcpServerStopCfmSer, CsrDhcpServerStopCfmDes, CsrDhcpServerStopCfmSerFree },
#endif /* EXCLUDE_CSR_DHCP_SERVER_STOP_CFM */
#ifndef EXCLUDE_CSR_DHCP_SERVER_STOP_REQ
    { CSR_DHCP_SERVER_STOP_REQ, CsrDhcpServerStopReqSizeof, CsrDhcpServerStopReqSer, CsrDhcpServerStopReqDes, CsrDhcpServerStopReqSerFree },
#endif /* EXCLUDE_CSR_DHCP_SERVER_STOP_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrDhcpServerConverterInit(void)
{
    CsrMsgConvInsert(CSR_DHCP_SERVER_PRIM, csr_dhcp_server_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_dhcp_server_conv_info = {
    CSR_DHCP_SERVER_PRIM,
    "CSR_DHCP_SERVER_PRIM",
    csr_dhcp_server_conv_lut
};

CsrLogPrimitiveInformation* CsrDhcpServerTechInfoGet(void)
{
    return &csr_dhcp_server_conv_info;
}
#endif
#endif

