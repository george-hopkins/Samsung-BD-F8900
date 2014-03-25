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
#include "csr_ip_ifconfig_prim.h"

#ifndef EXCLUDE_CSR_IP_IFCONFIG_MODULE

void CsrIp_ifconfigPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_CFM
CsrSize CsrIpIfconfigSubscribeCfmSizeof(void *msg)
{
    CsrIpIfconfigSubscribeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->ifDetailsCount * sizeof(CsrIpIfconfigIfDetails)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_CFM
CsrUint8 *CsrIpIfconfigSubscribeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpIfconfigSubscribeCfm *primitive;

    primitive = (CsrIpIfconfigSubscribeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ifDetailsCount);
    CsrMemCpySer(buffer, length, &primitive->ifDetails, sizeof(primitive->ifDetails));
    if(primitive->ifDetails)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->ifDetails), ((CsrSize) (primitive->ifDetailsCount * sizeof(CsrIpIfconfigIfDetails))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_CFM
void *CsrIpIfconfigSubscribeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpIfconfigSubscribeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpIfconfigSubscribeCfm *) CsrPmemZalloc(sizeof(CsrIpIfconfigSubscribeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ifDetailsCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ifDetails), buffer, &offset, ((CsrSize) sizeof(primitive->ifDetails)));
    if (primitive->ifDetailsCount)
    {
        primitive->ifDetails = CsrPmemZalloc(((CsrUint32) (primitive->ifDetailsCount * sizeof(CsrIpIfconfigIfDetails))));
        CsrMemCpyDes(((void *) primitive->ifDetails), buffer, &offset, ((CsrSize) (primitive->ifDetailsCount * sizeof(CsrIpIfconfigIfDetails))));
    }
    else
    {
        primitive->ifDetails = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_CFM
void CsrIpIfconfigSubscribeCfmSerFree(void *msg)
{
    CsrIpIfconfigSubscribeCfm *primitive;

    primitive = (CsrIpIfconfigSubscribeCfm *) msg;

    if(primitive->ifDetails != NULL)
    {
        CsrPmemFree(primitive->ifDetails);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_REQ
CsrSize CsrIpIfconfigUnsubscribeReqSizeof(void *msg)
{
    CsrIpIfconfigUnsubscribeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_REQ
CsrUint8 *CsrIpIfconfigUnsubscribeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpIfconfigUnsubscribeReq *primitive;

    primitive = (CsrIpIfconfigUnsubscribeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_REQ
void *CsrIpIfconfigUnsubscribeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpIfconfigUnsubscribeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpIfconfigUnsubscribeReq *) CsrPmemZalloc(sizeof(CsrIpIfconfigUnsubscribeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_DOWN_CFM
CsrSize CsrIpIfconfigDownCfmSizeof(void *msg)
{
    CsrIpIfconfigDownCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_DOWN_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_DOWN_CFM
CsrUint8 *CsrIpIfconfigDownCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpIfconfigDownCfm *primitive;

    primitive = (CsrIpIfconfigDownCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_DOWN_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_DOWN_CFM
void *CsrIpIfconfigDownCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpIfconfigDownCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpIfconfigDownCfm *) CsrPmemZalloc(sizeof(CsrIpIfconfigDownCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_DOWN_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UPDATE_IND
CsrSize CsrIpIfconfigUpdateIndSizeof(void *msg)
{
    CsrIpIfconfigUpdateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UPDATE_IND */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UPDATE_IND
CsrUint8 *CsrIpIfconfigUpdateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpIfconfigUpdateInd *primitive;

    primitive = (CsrIpIfconfigUpdateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifDetails.ifHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ifDetails.ifType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ifDetails.ifState);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ifDetails.mac), ((CsrSize) (6)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->ifDetails.ipAddress), ((CsrSize) (4)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->ifDetails.gatewayIpAddress), ((CsrSize) (4)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->ifDetails.networkMask), ((CsrSize) (4)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->ifDetails.dnsPrimaryIpAddress), ((CsrSize) (4)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->ifDetails.dnsSecondaryIpAddress), ((CsrSize) (4)));

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UPDATE_IND */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UPDATE_IND
void *CsrIpIfconfigUpdateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpIfconfigUpdateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpIfconfigUpdateInd *) CsrPmemZalloc(sizeof(CsrIpIfconfigUpdateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifDetails.ifHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ifDetails.ifType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ifDetails.ifState, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ifDetails.mac), buffer, &offset, ((CsrSize) (6)));
    CsrMemCpyDes(((void *) primitive->ifDetails.ipAddress), buffer, &offset, ((CsrSize) (4)));
    CsrMemCpyDes(((void *) primitive->ifDetails.gatewayIpAddress), buffer, &offset, ((CsrSize) (4)));
    CsrMemCpyDes(((void *) primitive->ifDetails.networkMask), buffer, &offset, ((CsrSize) (4)));
    CsrMemCpyDes(((void *) primitive->ifDetails.dnsPrimaryIpAddress), buffer, &offset, ((CsrSize) (4)));
    CsrMemCpyDes(((void *) primitive->ifDetails.dnsSecondaryIpAddress), buffer, &offset, ((CsrSize) (4)));

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UPDATE_IND */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_FORWARD_CFM
CsrSize CsrIpIfconfigForwardCfmSizeof(void *msg)
{
    CsrIpIfconfigForwardCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_FORWARD_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_FORWARD_CFM
CsrUint8 *CsrIpIfconfigForwardCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpIfconfigForwardCfm *primitive;

    primitive = (CsrIpIfconfigForwardCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_FORWARD_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_FORWARD_CFM
void *CsrIpIfconfigForwardCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpIfconfigForwardCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpIfconfigForwardCfm *) CsrPmemZalloc(sizeof(CsrIpIfconfigForwardCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_FORWARD_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_FORWARD_REQ
CsrSize CsrIpIfconfigForwardReqSizeof(void *msg)
{
    CsrIpIfconfigForwardReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_FORWARD_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_FORWARD_REQ
CsrUint8 *CsrIpIfconfigForwardReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpIfconfigForwardReq *primitive;

    primitive = (CsrIpIfconfigForwardReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enable);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_FORWARD_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_FORWARD_REQ
void *CsrIpIfconfigForwardReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpIfconfigForwardReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpIfconfigForwardReq *) CsrPmemZalloc(sizeof(CsrIpIfconfigForwardReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_FORWARD_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UP_REQ
CsrSize CsrIpIfconfigUpReqSizeof(void *msg)
{
    CsrIpIfconfigUpReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UP_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UP_REQ
CsrUint8 *CsrIpIfconfigUpReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpIfconfigUpReq *primitive;

    primitive = (CsrIpIfconfigUpReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->configMethod);
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (4)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->gatewayIpAddress), ((CsrSize) (4)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->networkMask), ((CsrSize) (4)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->dnsPrimaryIpAddress), ((CsrSize) (4)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->dnsSecondaryIpAddress), ((CsrSize) (4)));

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UP_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UP_REQ
void *CsrIpIfconfigUpReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpIfconfigUpReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpIfconfigUpReq *) CsrPmemZalloc(sizeof(CsrIpIfconfigUpReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->configMethod, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (4)));
    CsrMemCpyDes(((void *) primitive->gatewayIpAddress), buffer, &offset, ((CsrSize) (4)));
    CsrMemCpyDes(((void *) primitive->networkMask), buffer, &offset, ((CsrSize) (4)));
    CsrMemCpyDes(((void *) primitive->dnsPrimaryIpAddress), buffer, &offset, ((CsrSize) (4)));
    CsrMemCpyDes(((void *) primitive->dnsSecondaryIpAddress), buffer, &offset, ((CsrSize) (4)));

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UP_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_DOWN_REQ
CsrSize CsrIpIfconfigDownReqSizeof(void *msg)
{
    CsrIpIfconfigDownReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_DOWN_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_DOWN_REQ
CsrUint8 *CsrIpIfconfigDownReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpIfconfigDownReq *primitive;

    primitive = (CsrIpIfconfigDownReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_DOWN_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_DOWN_REQ
void *CsrIpIfconfigDownReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpIfconfigDownReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpIfconfigDownReq *) CsrPmemZalloc(sizeof(CsrIpIfconfigDownReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_DOWN_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_REQ
CsrSize CsrIpIfconfigSubscribeReqSizeof(void *msg)
{
    CsrIpIfconfigSubscribeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_REQ
CsrUint8 *CsrIpIfconfigSubscribeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpIfconfigSubscribeReq *primitive;

    primitive = (CsrIpIfconfigSubscribeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_REQ
void *CsrIpIfconfigSubscribeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpIfconfigSubscribeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpIfconfigSubscribeReq *) CsrPmemZalloc(sizeof(CsrIpIfconfigSubscribeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_SUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_CFM
CsrSize CsrIpIfconfigArpEntryAddCfmSizeof(void *msg)
{
    CsrIpIfconfigArpEntryAddCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_CFM
CsrUint8 *CsrIpIfconfigArpEntryAddCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpIfconfigArpEntryAddCfm *primitive;

    primitive = (CsrIpIfconfigArpEntryAddCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_CFM
void *CsrIpIfconfigArpEntryAddCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpIfconfigArpEntryAddCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpIfconfigArpEntryAddCfm *) CsrPmemZalloc(sizeof(CsrIpIfconfigArpEntryAddCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UP_CFM
CsrSize CsrIpIfconfigUpCfmSizeof(void *msg)
{
    CsrIpIfconfigUpCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UP_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UP_CFM
CsrUint8 *CsrIpIfconfigUpCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpIfconfigUpCfm *primitive;

    primitive = (CsrIpIfconfigUpCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UP_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UP_CFM
void *CsrIpIfconfigUpCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpIfconfigUpCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpIfconfigUpCfm *) CsrPmemZalloc(sizeof(CsrIpIfconfigUpCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UP_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_CFM
CsrSize CsrIpIfconfigUnsubscribeCfmSizeof(void *msg)
{
    CsrIpIfconfigUnsubscribeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_CFM
CsrUint8 *CsrIpIfconfigUnsubscribeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpIfconfigUnsubscribeCfm *primitive;

    primitive = (CsrIpIfconfigUnsubscribeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_CFM
void *CsrIpIfconfigUnsubscribeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpIfconfigUnsubscribeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpIfconfigUnsubscribeCfm *) CsrPmemZalloc(sizeof(CsrIpIfconfigUnsubscribeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_UNSUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_NAT_REQ
CsrSize CsrIpIfconfigNatReqSizeof(void *msg)
{
    CsrIpIfconfigNatReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_NAT_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_NAT_REQ
CsrUint8 *CsrIpIfconfigNatReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpIfconfigNatReq *primitive;

    primitive = (CsrIpIfconfigNatReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enable);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_NAT_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_NAT_REQ
void *CsrIpIfconfigNatReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpIfconfigNatReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpIfconfigNatReq *) CsrPmemZalloc(sizeof(CsrIpIfconfigNatReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_NAT_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_REQ
CsrSize CsrIpIfconfigArpEntryAddReqSizeof(void *msg)
{
    CsrIpIfconfigArpEntryAddReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_REQ
CsrUint8 *CsrIpIfconfigArpEntryAddReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpIfconfigArpEntryAddReq *primitive;

    primitive = (CsrIpIfconfigArpEntryAddReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrMemCpySer(buffer, length, ((const void *) primitive->mac), ((CsrSize) (6)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->ipAddress), ((CsrSize) (4)));

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_REQ
void *CsrIpIfconfigArpEntryAddReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpIfconfigArpEntryAddReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpIfconfigArpEntryAddReq *) CsrPmemZalloc(sizeof(CsrIpIfconfigArpEntryAddReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->mac), buffer, &offset, ((CsrSize) (6)));
    CsrMemCpyDes(((void *) primitive->ipAddress), buffer, &offset, ((CsrSize) (4)));

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_ARP_ENTRY_ADD_REQ */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_NAT_CFM
CsrSize CsrIpIfconfigNatCfmSizeof(void *msg)
{
    CsrIpIfconfigNatCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_NAT_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_NAT_CFM
CsrUint8 *CsrIpIfconfigNatCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpIfconfigNatCfm *primitive;

    primitive = (CsrIpIfconfigNatCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_NAT_CFM */

#ifndef EXCLUDE_CSR_IP_IFCONFIG_NAT_CFM
void *CsrIpIfconfigNatCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpIfconfigNatCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpIfconfigNatCfm *) CsrPmemZalloc(sizeof(CsrIpIfconfigNatCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_IFCONFIG_NAT_CFM */

#endif /* EXCLUDE_CSR_IP_IFCONFIG_MODULE */
