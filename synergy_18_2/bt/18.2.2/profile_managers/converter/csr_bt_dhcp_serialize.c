/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_synergy.h"
#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_msgconv.h"
#include "csr_bt_autogen.h"
#include "csr_unicode.h"
#include "csr_bt_dhcp_prim.h"

#ifndef EXCLUDE_CSR_BT_DHCP_MODULE

void CsrBtDhcpPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_REQ
CsrSize CsrBtDhcpDeactivateClientReqSizeof(void *msg)
{
    CsrBtDhcpDeactivateClientReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_REQ */

#ifndef EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_REQ
CsrUint8 *CsrBtDhcpDeactivateClientReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDhcpDeactivateClientReq *primitive;

    primitive = (CsrBtDhcpDeactivateClientReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_REQ */

#ifndef EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_REQ
void *CsrBtDhcpDeactivateClientReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDhcpDeactivateClientReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDhcpDeactivateClientReq *) CsrPmemZalloc(sizeof(CsrBtDhcpDeactivateClientReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_REQ */

#ifndef EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_CFM
CsrSize CsrBtDhcpDeactivateClientCfmSizeof(void *msg)
{
    CsrBtDhcpDeactivateClientCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_CFM */

#ifndef EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_CFM
CsrUint8 *CsrBtDhcpDeactivateClientCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDhcpDeactivateClientCfm *primitive;

    primitive = (CsrBtDhcpDeactivateClientCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_CFM */

#ifndef EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_CFM
void *CsrBtDhcpDeactivateClientCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDhcpDeactivateClientCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDhcpDeactivateClientCfm *) CsrPmemZalloc(sizeof(CsrBtDhcpDeactivateClientCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_CFM */

#ifndef EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_REQ
CsrSize CsrBtDhcpActivateClientReqSizeof(void *msg)
{
    CsrBtDhcpActivateClientReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_REQ */

#ifndef EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_REQ
CsrUint8 *CsrBtDhcpActivateClientReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDhcpActivateClientReq *primitive;

    primitive = (CsrBtDhcpActivateClientReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->applicationQueue);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ethernetAddress), ((CsrSize) 2 * (3)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_REQ */

#ifndef EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_REQ
void *CsrBtDhcpActivateClientReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDhcpActivateClientReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDhcpActivateClientReq *) CsrPmemZalloc(sizeof(CsrBtDhcpActivateClientReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->applicationQueue, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ethernetAddress), buffer, &offset, ((CsrSize) 2 * (3)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_REQ */

#ifndef EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_IND
CsrSize CsrBtDhcpDeactivateClientIndSizeof(void *msg)
{
    CsrBtDhcpDeactivateClientInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_IND */

#ifndef EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_IND
CsrUint8 *CsrBtDhcpDeactivateClientIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDhcpDeactivateClientInd *primitive;

    primitive = (CsrBtDhcpDeactivateClientInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_IND */

#ifndef EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_IND
void *CsrBtDhcpDeactivateClientIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDhcpDeactivateClientInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDhcpDeactivateClientInd *) CsrPmemZalloc(sizeof(CsrBtDhcpDeactivateClientInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DHCP_DEACTIVATE_CLIENT_IND */

#ifndef EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_CFM
CsrSize CsrBtDhcpActivateClientCfmSizeof(void *msg)
{
    CsrBtDhcpActivateClientCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_CFM */

#ifndef EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_CFM
CsrUint8 *CsrBtDhcpActivateClientCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtDhcpActivateClientCfm *primitive;

    primitive = (CsrBtDhcpActivateClientCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) 2 * (2)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_CFM */

#ifndef EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_CFM
void *CsrBtDhcpActivateClientCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtDhcpActivateClientCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtDhcpActivateClientCfm *) CsrPmemZalloc(sizeof(CsrBtDhcpActivateClientCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) 2 * (2)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_DHCP_ACTIVATE_CLIENT_CFM */

#endif /* EXCLUDE_CSR_BT_DHCP_MODULE */
