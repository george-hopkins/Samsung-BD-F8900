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

CsrDhcpServerCreateReq *CsrDhcpServerCreateReq_struct(CsrSchedQid qid,
    CsrUint8 ipAddress[4],
    CsrUint8 network[4],
    CsrUint8 networkMask[4],
    CsrUint8 gateway[4],
    CsrUint8 dns1[4],
    CsrUint8 dns2[4],
    CsrUint8 leaseTime)
{
    CsrDhcpServerCreateReq *prim;

    prim = (CsrDhcpServerCreateReq *) CsrPmemZalloc(sizeof(CsrDhcpServerCreateReq));
    prim->type = CSR_DHCP_SERVER_CREATE_REQ;
    prim->qid = qid;

    CsrMemCpy(prim->ipAddress, ipAddress, 4);
    CsrMemCpy(prim->network, network, 4);
    CsrMemCpy(prim->networkMask, networkMask, 4);

    CsrMemCpy(prim->gateway, gateway, 4);
    CsrMemCpy(prim->dns1, dns1, 4);
    CsrMemCpy(prim->dns2, dns2, 4);

    prim->leaseTime = leaseTime;

    return prim;
}

CsrDhcpServerDestroyReq *CsrDhcpServerDestroyReq_struct(CsrDhcpServerHandle serverHandle)
{
    CsrDhcpServerDestroyReq *prim;

    prim = (CsrDhcpServerDestroyReq *) CsrPmemZalloc(sizeof(CsrDhcpServerDestroyReq));
    prim->type = CSR_DHCP_SERVER_DESTROY_REQ;
    prim->serverHandle = serverHandle;

    return prim;
}

CsrDhcpServerStartReq *CsrDhcpServerStartReq_struct(CsrDhcpServerHandle serverHandle)
{
    CsrDhcpServerStartReq *prim;

    prim = (CsrDhcpServerStartReq *) CsrPmemZalloc(sizeof(CsrDhcpServerStartReq));
    prim->type = CSR_DHCP_SERVER_START_REQ;
    prim->serverHandle = serverHandle;

    return prim;
}

CsrDhcpServerStopReq *CsrDhcpServerStopReq_struct(CsrDhcpServerHandle serverHandle)
{
    CsrDhcpServerStopReq *prim;

    prim = (CsrDhcpServerStopReq *) CsrPmemZalloc(sizeof(CsrDhcpServerStopReq));
    prim->type = CSR_DHCP_SERVER_STOP_REQ;
    prim->serverHandle = serverHandle;

    return prim;
}

CsrDhcpServerLeasesGetReq *CsrDhcpServerLeasesGetReq_struct(CsrDhcpServerHandle serverHandle)
{
    CsrDhcpServerLeasesGetReq *prim;

    prim = (CsrDhcpServerLeasesGetReq *) CsrPmemZalloc(sizeof(CsrDhcpServerLeasesGetReq));
    prim->type = CSR_DHCP_SERVER_LEASES_GET_REQ;
    prim->serverHandle = serverHandle;

    return prim;
}

CsrDhcpServerLeasesSetReq *CsrDhcpServerLeasesSetReq_struct(CsrDhcpServerHandle serverHandle,
    CsrUint16 leasesCount,
    CsrDhcpServerLease *leases)
{
    CsrDhcpServerLeasesSetReq *prim;

    prim = (CsrDhcpServerLeasesSetReq *) CsrPmemZalloc(sizeof(CsrDhcpServerLeasesSetReq));
    prim->type = CSR_DHCP_SERVER_LEASES_SET_REQ;
    prim->serverHandle = serverHandle;
    prim->leasesCount = leasesCount;
    prim->leases = leases;

    return prim;
}
