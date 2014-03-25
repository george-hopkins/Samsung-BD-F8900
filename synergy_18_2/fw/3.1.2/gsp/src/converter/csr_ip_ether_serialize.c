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
#include "csr_ip_ether_prim.h"

#ifndef EXCLUDE_CSR_IP_ETHER_MODULE

void CsrIp_etherPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_IP_ETHER_IF_ADD_CFM
CsrSize CsrIpEtherIfAddCfmSizeof(void *msg)
{
    CsrIpEtherIfAddCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_ADD_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_ADD_CFM
CsrUint8 *CsrIpEtherIfAddCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpEtherIfAddCfm *primitive;

    primitive = (CsrIpEtherIfAddCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->encapsulation);
    CsrMemCpySer(buffer, length, &primitive->frameRxFunction, sizeof(primitive->frameRxFunction));
    CsrMemCpySer(buffer, length, &primitive->ipContext, sizeof(primitive->ipContext));

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_ADD_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_ADD_CFM
void *CsrIpEtherIfAddCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpEtherIfAddCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpEtherIfAddCfm *) CsrPmemZalloc(sizeof(CsrIpEtherIfAddCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->encapsulation, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->frameRxFunction), buffer, &offset, ((CsrSize) sizeof(primitive->frameRxFunction)));
    CsrMemCpyDes(((void *) &primitive->ipContext), buffer, &offset, ((CsrSize) sizeof(primitive->ipContext)));

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_ADD_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_ADD_CFM
void CsrIpEtherIfAddCfmSerFree(void *msg)
{
    CsrIpEtherIfAddCfm *primitive;

    primitive = (CsrIpEtherIfAddCfm *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_ADD_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_RESUME_REQ
CsrSize CsrIpEtherIfFlowControlResumeReqSizeof(void *msg)
{
    CsrIpEtherIfFlowControlResumeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_RESUME_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_RESUME_REQ
CsrUint8 *CsrIpEtherIfFlowControlResumeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpEtherIfFlowControlResumeReq *primitive;

    primitive = (CsrIpEtherIfFlowControlResumeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->priority);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_RESUME_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_RESUME_REQ
void *CsrIpEtherIfFlowControlResumeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpEtherIfFlowControlResumeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpEtherIfFlowControlResumeReq *) CsrPmemZalloc(sizeof(CsrIpEtherIfFlowControlResumeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->priority, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_RESUME_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_CFM
CsrSize CsrIpEtherIfLinkUpCfmSizeof(void *msg)
{
    CsrIpEtherIfLinkUpCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_CFM
CsrUint8 *CsrIpEtherIfLinkUpCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpEtherIfLinkUpCfm *primitive;

    primitive = (CsrIpEtherIfLinkUpCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_CFM
void *CsrIpEtherIfLinkUpCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpEtherIfLinkUpCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpEtherIfLinkUpCfm *) CsrPmemZalloc(sizeof(CsrIpEtherIfLinkUpCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_RES
CsrSize CsrIpEtherIfMulticastAddrFlushResSizeof(void *msg)
{
    CsrIpEtherIfMulticastAddrFlushRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_RES */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_RES
CsrUint8 *CsrIpEtherIfMulticastAddrFlushResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpEtherIfMulticastAddrFlushRes *primitive;

    primitive = (CsrIpEtherIfMulticastAddrFlushRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_RES */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_RES
void *CsrIpEtherIfMulticastAddrFlushResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpEtherIfMulticastAddrFlushRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpEtherIfMulticastAddrFlushRes *) CsrPmemZalloc(sizeof(CsrIpEtherIfMulticastAddrFlushRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_RES */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_REQ
CsrSize CsrIpEtherIfLinkUpReqSizeof(void *msg)
{
    CsrIpEtherIfLinkUpReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_REQ
CsrUint8 *CsrIpEtherIfLinkUpReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpEtherIfLinkUpReq *primitive;

    primitive = (CsrIpEtherIfLinkUpReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_REQ
void *CsrIpEtherIfLinkUpReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpEtherIfLinkUpReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpEtherIfLinkUpReq *) CsrPmemZalloc(sizeof(CsrIpEtherIfLinkUpReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_UP_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_REQ
CsrSize CsrIpEtherIfLinkDownReqSizeof(void *msg)
{
    CsrIpEtherIfLinkDownReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_REQ
CsrUint8 *CsrIpEtherIfLinkDownReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpEtherIfLinkDownReq *primitive;

    primitive = (CsrIpEtherIfLinkDownReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_REQ
void *CsrIpEtherIfLinkDownReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpEtherIfLinkDownReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpEtherIfLinkDownReq *) CsrPmemZalloc(sizeof(CsrIpEtherIfLinkDownReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_IND
CsrSize CsrIpEtherIfMulticastAddrFlushIndSizeof(void *msg)
{
    CsrIpEtherIfMulticastAddrFlushInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_IND */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_IND
CsrUint8 *CsrIpEtherIfMulticastAddrFlushIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpEtherIfMulticastAddrFlushInd *primitive;

    primitive = (CsrIpEtherIfMulticastAddrFlushInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_IND */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_IND
void *CsrIpEtherIfMulticastAddrFlushIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpEtherIfMulticastAddrFlushInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpEtherIfMulticastAddrFlushInd *) CsrPmemZalloc(sizeof(CsrIpEtherIfMulticastAddrFlushInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_FLUSH_IND */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_REMOVE_REQ
CsrSize CsrIpEtherIfRemoveReqSizeof(void *msg)
{
    CsrIpEtherIfRemoveReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_REMOVE_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_REMOVE_REQ
CsrUint8 *CsrIpEtherIfRemoveReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpEtherIfRemoveReq *primitive;

    primitive = (CsrIpEtherIfRemoveReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_REMOVE_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_REMOVE_REQ
void *CsrIpEtherIfRemoveReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpEtherIfRemoveReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpEtherIfRemoveReq *) CsrPmemZalloc(sizeof(CsrIpEtherIfRemoveReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_REMOVE_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_CFM
CsrSize CsrIpEtherIfLinkDownCfmSizeof(void *msg)
{
    CsrIpEtherIfLinkDownCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_CFM
CsrUint8 *CsrIpEtherIfLinkDownCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpEtherIfLinkDownCfm *primitive;

    primitive = (CsrIpEtherIfLinkDownCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_CFM
void *CsrIpEtherIfLinkDownCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpEtherIfLinkDownCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpEtherIfLinkDownCfm *) CsrPmemZalloc(sizeof(CsrIpEtherIfLinkDownCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_LINK_DOWN_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_IND
CsrSize CsrIpEtherIfMulticastAddrRemoveIndSizeof(void *msg)
{
    CsrIpEtherIfMulticastAddrRemoveInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_IND */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_IND
CsrUint8 *CsrIpEtherIfMulticastAddrRemoveIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpEtherIfMulticastAddrRemoveInd *primitive;

    primitive = (CsrIpEtherIfMulticastAddrRemoveInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);
    CsrMemCpySer(buffer, length, ((const void *) primitive->multicastMacAddr), ((CsrSize) (6)));

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_IND */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_IND
void *CsrIpEtherIfMulticastAddrRemoveIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpEtherIfMulticastAddrRemoveInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpEtherIfMulticastAddrRemoveInd *) CsrPmemZalloc(sizeof(CsrIpEtherIfMulticastAddrRemoveInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->multicastMacAddr), buffer, &offset, ((CsrSize) (6)));

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_IND */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_REMOVE_CFM
CsrSize CsrIpEtherIfRemoveCfmSizeof(void *msg)
{
    CsrIpEtherIfRemoveCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_REMOVE_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_REMOVE_CFM
CsrUint8 *CsrIpEtherIfRemoveCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpEtherIfRemoveCfm *primitive;

    primitive = (CsrIpEtherIfRemoveCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_REMOVE_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_REMOVE_CFM
void *CsrIpEtherIfRemoveCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpEtherIfRemoveCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpEtherIfRemoveCfm *) CsrPmemZalloc(sizeof(CsrIpEtherIfRemoveCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_REMOVE_CFM */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_RES
CsrSize CsrIpEtherIfMulticastAddrAddResSizeof(void *msg)
{
    CsrIpEtherIfMulticastAddrAddRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_RES */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_RES
CsrUint8 *CsrIpEtherIfMulticastAddrAddResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpEtherIfMulticastAddrAddRes *primitive;

    primitive = (CsrIpEtherIfMulticastAddrAddRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_RES */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_RES
void *CsrIpEtherIfMulticastAddrAddResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpEtherIfMulticastAddrAddRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpEtherIfMulticastAddrAddRes *) CsrPmemZalloc(sizeof(CsrIpEtherIfMulticastAddrAddRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_RES */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_RES
CsrSize CsrIpEtherIfMulticastAddrRemoveResSizeof(void *msg)
{
    CsrIpEtherIfMulticastAddrRemoveRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_RES */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_RES
CsrUint8 *CsrIpEtherIfMulticastAddrRemoveResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpEtherIfMulticastAddrRemoveRes *primitive;

    primitive = (CsrIpEtherIfMulticastAddrRemoveRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_RES */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_RES
void *CsrIpEtherIfMulticastAddrRemoveResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpEtherIfMulticastAddrRemoveRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpEtherIfMulticastAddrRemoveRes *) CsrPmemZalloc(sizeof(CsrIpEtherIfMulticastAddrRemoveRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_REMOVE_RES */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_IND
CsrSize CsrIpEtherIfMulticastAddrAddIndSizeof(void *msg)
{
    CsrIpEtherIfMulticastAddrAddInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_IND */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_IND
CsrUint8 *CsrIpEtherIfMulticastAddrAddIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpEtherIfMulticastAddrAddInd *primitive;

    primitive = (CsrIpEtherIfMulticastAddrAddInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);
    CsrMemCpySer(buffer, length, ((const void *) primitive->multicastMacAddr), ((CsrSize) (6)));

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_IND */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_IND
void *CsrIpEtherIfMulticastAddrAddIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpEtherIfMulticastAddrAddInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpEtherIfMulticastAddrAddInd *) CsrPmemZalloc(sizeof(CsrIpEtherIfMulticastAddrAddInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->multicastMacAddr), buffer, &offset, ((CsrSize) (6)));

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_MULTICAST_ADDR_ADD_IND */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_ADD_REQ
CsrSize CsrIpEtherIfAddReqSizeof(void *msg)
{
    CsrIpEtherIfAddReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_ADD_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_ADD_REQ
CsrUint8 *CsrIpEtherIfAddReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpEtherIfAddReq *primitive;

    primitive = (CsrIpEtherIfAddReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ifType);
    CsrMemCpySer(buffer, length, ((const void *) primitive->mac), ((CsrSize) (6)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->encapsulation);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxTxUnit);
    CsrMemCpySer(buffer, length, &primitive->frameTxFunction, sizeof(primitive->frameTxFunction));
    CsrMemCpySer(buffer, length, &primitive->ifContext, sizeof(primitive->ifContext));

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_ADD_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_ADD_REQ
void *CsrIpEtherIfAddReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpEtherIfAddReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpEtherIfAddReq *) CsrPmemZalloc(sizeof(CsrIpEtherIfAddReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ifType, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->mac), buffer, &offset, ((CsrSize) (6)));
    CsrUint8Des((CsrUint8 *) &primitive->encapsulation, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxTxUnit, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->frameTxFunction), buffer, &offset, ((CsrSize) sizeof(primitive->frameTxFunction)));
    CsrMemCpyDes(((void *) &primitive->ifContext), buffer, &offset, ((CsrSize) sizeof(primitive->ifContext)));

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_ADD_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_ADD_REQ
void CsrIpEtherIfAddReqSerFree(void *msg)
{
    CsrIpEtherIfAddReq *primitive;

    primitive = (CsrIpEtherIfAddReq *) msg;

    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_ADD_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_PAUSE_REQ
CsrSize CsrIpEtherIfFlowControlPauseReqSizeof(void *msg)
{
    CsrIpEtherIfFlowControlPauseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_PAUSE_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_PAUSE_REQ
CsrUint8 *CsrIpEtherIfFlowControlPauseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrIpEtherIfFlowControlPauseReq *primitive;

    primitive = (CsrIpEtherIfFlowControlPauseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ifHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->priority);

    return buffer;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_PAUSE_REQ */

#ifndef EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_PAUSE_REQ
void *CsrIpEtherIfFlowControlPauseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrIpEtherIfFlowControlPauseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrIpEtherIfFlowControlPauseReq *) CsrPmemZalloc(sizeof(CsrIpEtherIfFlowControlPauseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ifHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->priority, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_IP_ETHER_IF_FLOW_CONTROL_PAUSE_REQ */

#endif /* EXCLUDE_CSR_IP_ETHER_MODULE */
