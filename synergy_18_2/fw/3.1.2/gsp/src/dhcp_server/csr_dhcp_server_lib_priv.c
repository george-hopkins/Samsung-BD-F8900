/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_pmem.h"
#include "csr_dhcp_server_prim.h"
#include "csr_dhcp_server_lib.h"

void CsrDhcpServerCreateCfmSend(CsrSchedQid queue, CsrDhcpServerHandle serverHandle, CsrResult result)
{
    CsrDhcpServerCreateCfm *prim;

    prim = (CsrDhcpServerCreateCfm *) CsrPmemZalloc(sizeof(CsrDhcpServerCreateCfm));
    prim->type = CSR_DHCP_SERVER_CREATE_CFM;
    prim->serverHandle = serverHandle;
    prim->result = result;

    CsrSchedMessagePut(queue, CSR_DHCP_SERVER_PRIM, prim);
}

void CsrDhcpServerDestroyCfmSend(CsrSchedQid queue, CsrDhcpServerHandle serverHandle, CsrResult result)
{
    CsrDhcpServerDestroyCfm *prim;

    prim = (CsrDhcpServerDestroyCfm *) CsrPmemZalloc(sizeof(CsrDhcpServerDestroyCfm));
    prim->type = CSR_DHCP_SERVER_DESTROY_CFM;
    prim->serverHandle = serverHandle;
    prim->result = result;

    CsrSchedMessagePut(queue, CSR_DHCP_SERVER_PRIM, prim);
}

void CsrDhcpServerStartCfmSend(CsrSchedQid queue, CsrDhcpServerHandle serverHandle, CsrResult result)
{
    CsrDhcpServerStartCfm *prim;

    prim = (CsrDhcpServerStartCfm *) CsrPmemZalloc(sizeof(CsrDhcpServerStartCfm));
    prim->type = CSR_DHCP_SERVER_START_CFM;
    prim->serverHandle = serverHandle;
    prim->result = result;

    CsrSchedMessagePut(queue, CSR_DHCP_SERVER_PRIM, prim);
}

void CsrDhcpServerStopCfmSend(CsrSchedQid queue, CsrDhcpServerHandle serverHandle, CsrResult result)
{
    CsrDhcpServerStopCfm *prim;

    prim = (CsrDhcpServerStopCfm *) CsrPmemZalloc(sizeof(CsrDhcpServerStopCfm));
    prim->type = CSR_DHCP_SERVER_STOP_CFM;
    prim->serverHandle = serverHandle;
    prim->result = result;

    CsrSchedMessagePut(queue, CSR_DHCP_SERVER_PRIM, prim);
}

void CsrDhcpServerLeasesGetCfmSend(CsrSchedQid queue, CsrDhcpServerHandle serverHandle, CsrUint16 leasesCount, CsrDhcpServerLease *leases)
{
    CsrDhcpServerLeasesGetCfm *prim;

    prim = (CsrDhcpServerLeasesGetCfm *) CsrPmemZalloc(sizeof(CsrDhcpServerLeasesGetCfm));
    prim->type = CSR_DHCP_SERVER_LEASES_GET_CFM;
    prim->serverHandle = serverHandle;
    prim->leasesCount = leasesCount;
    prim->leases = leases;

    CsrSchedMessagePut(queue, CSR_DHCP_SERVER_PRIM, prim);
}

void CsrDhcpServerLeasesSetCfmSend(CsrSchedQid queue, CsrDhcpServerHandle serverHandle, CsrResult result)
{
    CsrDhcpServerLeasesSetCfm *prim;

    prim = (CsrDhcpServerLeasesSetCfm *) CsrPmemZalloc(sizeof(CsrDhcpServerLeasesSetCfm));
    prim->type = CSR_DHCP_SERVER_LEASES_SET_CFM;
    prim->serverHandle = serverHandle;
    prim->result = result;

    CsrSchedMessagePut(queue, CSR_DHCP_SERVER_PRIM, prim);
}

void CsrDhcpServerRogueIpAddressIndSend(CsrSchedQid queue, CsrDhcpServerHandle serverHandle, CsrUint8 *ip)
{
    CsrDhcpServerRogueIpAddressInd *prim;

    prim = (CsrDhcpServerRogueIpAddressInd *) CsrPmemZalloc(sizeof(CsrDhcpServerRogueIpAddressInd));
    prim->type = CSR_DHCP_SERVER_ROGUE_IP_ADDRESS_IND;
    prim->serverHandle = serverHandle;

    CsrMemCpy(prim->ipAddress, ip, 4);

    CsrSchedMessagePut(queue, CSR_DHCP_SERVER_PRIM, prim);
}
