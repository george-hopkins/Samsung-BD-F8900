/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_msgconv.h"
#include "csr_autogen.h"
#include "csr_unicode.h"
#include "csr_dhcp_server_prim.h"

#ifndef EXCLUDE_CSR_DHCP_SERVER_MODULE

void CsrDhcp_serverPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_DHCP_SERVER_STOP_REQ
CsrSize CsrDhcpServerStopReqSizeof(void *msg)
{
    CsrDhcpServerStopReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_STOP_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_STOP_REQ
CsrUint8 *CsrDhcpServerStopReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDhcpServerStopReq *primitive;

    primitive = (CsrDhcpServerStopReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->serverHandle, sizeof(primitive->serverHandle));

    return buffer;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_STOP_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_STOP_REQ
void *CsrDhcpServerStopReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDhcpServerStopReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDhcpServerStopReq *) CsrPmemZalloc(sizeof(CsrDhcpServerStopReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serverHandle), buffer, &offset, ((CsrSize) sizeof(primitive->serverHandle)));

    return primitive;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_STOP_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_STOP_REQ
void CsrDhcpServerStopReqSerFree(void *msg)
{
    CsrDhcpServerStopReq *primitive;

    primitive = (CsrDhcpServerStopReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_STOP_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_CREATE_REQ
CsrSize CsrDhcpServerCreateReqSizeof(void *msg)
{
    CsrDhcpServerCreateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_CREATE_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_CREATE_REQ
CsrUint8 *CsrDhcpServerCreateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDhcpServerCreateReq *primitive;

    primitive = (CsrDhcpServerCreateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qid);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (4)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->network), ((CsrSize) (4)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->networkMask), ((CsrSize) (4)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->gateway), ((CsrSize) (4)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->dns1), ((CsrSize) (4)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->dns2), ((CsrSize) (4)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->leaseTime);

    return buffer;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_CREATE_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_CREATE_REQ
void *CsrDhcpServerCreateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDhcpServerCreateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDhcpServerCreateReq *) CsrPmemZalloc(sizeof(CsrDhcpServerCreateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (4)));
    CsrMemCpyDes(((void *) primitive->network), buffer, &offset, ((CsrSize) (4)));
    CsrMemCpyDes(((void *) primitive->networkMask), buffer, &offset, ((CsrSize) (4)));
    CsrMemCpyDes(((void *) primitive->gateway), buffer, &offset, ((CsrSize) (4)));
    CsrMemCpyDes(((void *) primitive->dns1), buffer, &offset, ((CsrSize) (4)));
    CsrMemCpyDes(((void *) primitive->dns2), buffer, &offset, ((CsrSize) (4)));
    CsrUint8Des((CsrUint8 *) &primitive->leaseTime, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_CREATE_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_DESTROY_REQ
CsrSize CsrDhcpServerDestroyReqSizeof(void *msg)
{
    CsrDhcpServerDestroyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_DESTROY_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_DESTROY_REQ
CsrUint8 *CsrDhcpServerDestroyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDhcpServerDestroyReq *primitive;

    primitive = (CsrDhcpServerDestroyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->serverHandle, sizeof(primitive->serverHandle));

    return buffer;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_DESTROY_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_DESTROY_REQ
void *CsrDhcpServerDestroyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDhcpServerDestroyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDhcpServerDestroyReq *) CsrPmemZalloc(sizeof(CsrDhcpServerDestroyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serverHandle), buffer, &offset, ((CsrSize) sizeof(primitive->serverHandle)));

    return primitive;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_DESTROY_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_DESTROY_REQ
void CsrDhcpServerDestroyReqSerFree(void *msg)
{
    CsrDhcpServerDestroyReq *primitive;

    primitive = (CsrDhcpServerDestroyReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_DESTROY_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_REQ
CsrSize CsrDhcpServerLeasesSetReqSizeof(void *msg)
{
    CsrDhcpServerLeasesSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->leasesCount * sizeof(CsrDhcpServerLease)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_REQ
CsrUint8 *CsrDhcpServerLeasesSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDhcpServerLeasesSetReq *primitive;

    primitive = (CsrDhcpServerLeasesSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->serverHandle, sizeof(primitive->serverHandle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->leasesCount);
    CsrMemCpySer(buffer, length, &primitive->leases, sizeof(primitive->leases));
    if(primitive->leases)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->leases), ((CsrSize) (primitive->leasesCount * sizeof(CsrDhcpServerLease))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_REQ
void *CsrDhcpServerLeasesSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDhcpServerLeasesSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDhcpServerLeasesSetReq *) CsrPmemZalloc(sizeof(CsrDhcpServerLeasesSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serverHandle), buffer, &offset, ((CsrSize) sizeof(primitive->serverHandle)));
    CsrUint16Des((CsrUint16 *) &primitive->leasesCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->leases), buffer, &offset, ((CsrSize) sizeof(primitive->leases)));
    if (primitive->leasesCount)
    {
        primitive->leases = CsrPmemZalloc(((CsrUint32) (primitive->leasesCount * sizeof(CsrDhcpServerLease))));
        CsrMemCpyDes(((void *) primitive->leases), buffer, &offset, ((CsrSize) (primitive->leasesCount * sizeof(CsrDhcpServerLease))));
    }
    else
    {
        primitive->leases = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_REQ
void CsrDhcpServerLeasesSetReqSerFree(void *msg)
{
    CsrDhcpServerLeasesSetReq *primitive;

    primitive = (CsrDhcpServerLeasesSetReq *) msg;

    if(primitive->leases != NULL)
    {
        CsrPmemFree(primitive->leases);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_CREATE_CFM
CsrSize CsrDhcpServerCreateCfmSizeof(void *msg)
{
    CsrDhcpServerCreateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_CREATE_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_CREATE_CFM
CsrUint8 *CsrDhcpServerCreateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDhcpServerCreateCfm *primitive;

    primitive = (CsrDhcpServerCreateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->serverHandle, sizeof(primitive->serverHandle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_CREATE_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_CREATE_CFM
void *CsrDhcpServerCreateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDhcpServerCreateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDhcpServerCreateCfm *) CsrPmemZalloc(sizeof(CsrDhcpServerCreateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serverHandle), buffer, &offset, ((CsrSize) sizeof(primitive->serverHandle)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_CREATE_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_CREATE_CFM
void CsrDhcpServerCreateCfmSerFree(void *msg)
{
    CsrDhcpServerCreateCfm *primitive;

    primitive = (CsrDhcpServerCreateCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_CREATE_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_START_CFM
CsrSize CsrDhcpServerStartCfmSizeof(void *msg)
{
    CsrDhcpServerStartCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_START_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_START_CFM
CsrUint8 *CsrDhcpServerStartCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDhcpServerStartCfm *primitive;

    primitive = (CsrDhcpServerStartCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->serverHandle, sizeof(primitive->serverHandle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_START_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_START_CFM
void *CsrDhcpServerStartCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDhcpServerStartCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDhcpServerStartCfm *) CsrPmemZalloc(sizeof(CsrDhcpServerStartCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serverHandle), buffer, &offset, ((CsrSize) sizeof(primitive->serverHandle)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_START_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_START_CFM
void CsrDhcpServerStartCfmSerFree(void *msg)
{
    CsrDhcpServerStartCfm *primitive;

    primitive = (CsrDhcpServerStartCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_START_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_STOP_CFM
CsrSize CsrDhcpServerStopCfmSizeof(void *msg)
{
    CsrDhcpServerStopCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_STOP_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_STOP_CFM
CsrUint8 *CsrDhcpServerStopCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDhcpServerStopCfm *primitive;

    primitive = (CsrDhcpServerStopCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->serverHandle, sizeof(primitive->serverHandle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_STOP_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_STOP_CFM
void *CsrDhcpServerStopCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDhcpServerStopCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDhcpServerStopCfm *) CsrPmemZalloc(sizeof(CsrDhcpServerStopCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serverHandle), buffer, &offset, ((CsrSize) sizeof(primitive->serverHandle)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_STOP_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_STOP_CFM
void CsrDhcpServerStopCfmSerFree(void *msg)
{
    CsrDhcpServerStopCfm *primitive;

    primitive = (CsrDhcpServerStopCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_STOP_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_ROGUE_IP_ADDRESS_IND
CsrSize CsrDhcpServerRogueIpAddressIndSizeof(void *msg)
{
    CsrDhcpServerRogueIpAddressInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_ROGUE_IP_ADDRESS_IND */

#ifndef EXCLUDE_CSR_DHCP_SERVER_ROGUE_IP_ADDRESS_IND
CsrUint8 *CsrDhcpServerRogueIpAddressIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDhcpServerRogueIpAddressInd *primitive;

    primitive = (CsrDhcpServerRogueIpAddressInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->serverHandle, sizeof(primitive->serverHandle));
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (4)));

    return buffer;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_ROGUE_IP_ADDRESS_IND */

#ifndef EXCLUDE_CSR_DHCP_SERVER_ROGUE_IP_ADDRESS_IND
void *CsrDhcpServerRogueIpAddressIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDhcpServerRogueIpAddressInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDhcpServerRogueIpAddressInd *) CsrPmemZalloc(sizeof(CsrDhcpServerRogueIpAddressInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serverHandle), buffer, &offset, ((CsrSize) sizeof(primitive->serverHandle)));
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (4)));

    return primitive;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_ROGUE_IP_ADDRESS_IND */

#ifndef EXCLUDE_CSR_DHCP_SERVER_ROGUE_IP_ADDRESS_IND
void CsrDhcpServerRogueIpAddressIndSerFree(void *msg)
{
    CsrDhcpServerRogueIpAddressInd *primitive;

    primitive = (CsrDhcpServerRogueIpAddressInd *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_ROGUE_IP_ADDRESS_IND */

#ifndef EXCLUDE_CSR_DHCP_SERVER_START_REQ
CsrSize CsrDhcpServerStartReqSizeof(void *msg)
{
    CsrDhcpServerStartReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_START_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_START_REQ
CsrUint8 *CsrDhcpServerStartReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDhcpServerStartReq *primitive;

    primitive = (CsrDhcpServerStartReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->serverHandle, sizeof(primitive->serverHandle));

    return buffer;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_START_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_START_REQ
void *CsrDhcpServerStartReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDhcpServerStartReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDhcpServerStartReq *) CsrPmemZalloc(sizeof(CsrDhcpServerStartReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serverHandle), buffer, &offset, ((CsrSize) sizeof(primitive->serverHandle)));

    return primitive;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_START_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_START_REQ
void CsrDhcpServerStartReqSerFree(void *msg)
{
    CsrDhcpServerStartReq *primitive;

    primitive = (CsrDhcpServerStartReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_START_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_CFM
CsrSize CsrDhcpServerLeasesGetCfmSizeof(void *msg)
{
    CsrDhcpServerLeasesGetCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->leasesCount * sizeof(CsrDhcpServerLease)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_CFM
CsrUint8 *CsrDhcpServerLeasesGetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDhcpServerLeasesGetCfm *primitive;

    primitive = (CsrDhcpServerLeasesGetCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->serverHandle, sizeof(primitive->serverHandle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->leasesCount);
    CsrMemCpySer(buffer, length, &primitive->leases, sizeof(primitive->leases));
    if(primitive->leases)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->leases), ((CsrSize) (primitive->leasesCount * sizeof(CsrDhcpServerLease))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_CFM
void *CsrDhcpServerLeasesGetCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDhcpServerLeasesGetCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDhcpServerLeasesGetCfm *) CsrPmemZalloc(sizeof(CsrDhcpServerLeasesGetCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serverHandle), buffer, &offset, ((CsrSize) sizeof(primitive->serverHandle)));
    CsrUint16Des((CsrUint16 *) &primitive->leasesCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->leases), buffer, &offset, ((CsrSize) sizeof(primitive->leases)));
    if (primitive->leasesCount)
    {
        primitive->leases = CsrPmemZalloc(((CsrUint32) (primitive->leasesCount * sizeof(CsrDhcpServerLease))));
        CsrMemCpyDes(((void *) primitive->leases), buffer, &offset, ((CsrSize) (primitive->leasesCount * sizeof(CsrDhcpServerLease))));
    }
    else
    {
        primitive->leases = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_CFM
void CsrDhcpServerLeasesGetCfmSerFree(void *msg)
{
    CsrDhcpServerLeasesGetCfm *primitive;

    primitive = (CsrDhcpServerLeasesGetCfm *) msg;

    if(primitive->leases != NULL)
    {
        CsrPmemFree(primitive->leases);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_REQ
CsrSize CsrDhcpServerLeasesGetReqSizeof(void *msg)
{
    CsrDhcpServerLeasesGetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_REQ
CsrUint8 *CsrDhcpServerLeasesGetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDhcpServerLeasesGetReq *primitive;

    primitive = (CsrDhcpServerLeasesGetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->serverHandle, sizeof(primitive->serverHandle));

    return buffer;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_REQ
void *CsrDhcpServerLeasesGetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDhcpServerLeasesGetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDhcpServerLeasesGetReq *) CsrPmemZalloc(sizeof(CsrDhcpServerLeasesGetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serverHandle), buffer, &offset, ((CsrSize) sizeof(primitive->serverHandle)));

    return primitive;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_REQ
void CsrDhcpServerLeasesGetReqSerFree(void *msg)
{
    CsrDhcpServerLeasesGetReq *primitive;

    primitive = (CsrDhcpServerLeasesGetReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_GET_REQ */

#ifndef EXCLUDE_CSR_DHCP_SERVER_DESTROY_CFM
CsrSize CsrDhcpServerDestroyCfmSizeof(void *msg)
{
    CsrDhcpServerDestroyCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_DESTROY_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_DESTROY_CFM
CsrUint8 *CsrDhcpServerDestroyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDhcpServerDestroyCfm *primitive;

    primitive = (CsrDhcpServerDestroyCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->serverHandle, sizeof(primitive->serverHandle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_DESTROY_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_DESTROY_CFM
void *CsrDhcpServerDestroyCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDhcpServerDestroyCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDhcpServerDestroyCfm *) CsrPmemZalloc(sizeof(CsrDhcpServerDestroyCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serverHandle), buffer, &offset, ((CsrSize) sizeof(primitive->serverHandle)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_DESTROY_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_DESTROY_CFM
void CsrDhcpServerDestroyCfmSerFree(void *msg)
{
    CsrDhcpServerDestroyCfm *primitive;

    primitive = (CsrDhcpServerDestroyCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_DESTROY_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_CFM
CsrSize CsrDhcpServerLeasesSetCfmSizeof(void *msg)
{
    CsrDhcpServerLeasesSetCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_CFM
CsrUint8 *CsrDhcpServerLeasesSetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrDhcpServerLeasesSetCfm *primitive;

    primitive = (CsrDhcpServerLeasesSetCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->serverHandle, sizeof(primitive->serverHandle));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_CFM
void *CsrDhcpServerLeasesSetCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrDhcpServerLeasesSetCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrDhcpServerLeasesSetCfm *) CsrPmemZalloc(sizeof(CsrDhcpServerLeasesSetCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serverHandle), buffer, &offset, ((CsrSize) sizeof(primitive->serverHandle)));
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_CFM */

#ifndef EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_CFM
void CsrDhcpServerLeasesSetCfmSerFree(void *msg)
{
    CsrDhcpServerLeasesSetCfm *primitive;

    primitive = (CsrDhcpServerLeasesSetCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_DHCP_SERVER_LEASES_SET_CFM */

#endif /* EXCLUDE_CSR_DHCP_SERVER_MODULE */
