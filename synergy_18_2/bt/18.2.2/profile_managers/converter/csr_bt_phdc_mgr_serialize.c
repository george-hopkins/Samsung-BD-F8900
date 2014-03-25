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
#include "csr_bt_phdc_mgr_prim.h"

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_MODULE

void CsrBtPhdc_mgrPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_RES
CsrSize CsrBtPhdcMgrAcceptBtLinkResSizeof(void *msg)
{
    CsrBtPhdcMgrAcceptBtLinkRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_RES
CsrUint8 *CsrBtPhdcMgrAcceptBtLinkResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrAcceptBtLinkRes *primitive;

    primitive = (CsrBtPhdcMgrAcceptBtLinkRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_RES
void *CsrBtPhdcMgrAcceptBtLinkResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrAcceptBtLinkRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrAcceptBtLinkRes *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrAcceptBtLinkRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_BT_LINK_REL_IND
CsrSize CsrBtPhdcMgrBtLinkRelIndSizeof(void *msg)
{
    CsrBtPhdcMgrBtLinkRelInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_BT_LINK_REL_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_BT_LINK_REL_IND
CsrUint8 *CsrBtPhdcMgrBtLinkRelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrBtLinkRelInd *primitive;

    primitive = (CsrBtPhdcMgrBtLinkRelInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_BT_LINK_REL_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_BT_LINK_REL_IND
void *CsrBtPhdcMgrBtLinkRelIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrBtLinkRelInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrBtLinkRelInd *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrBtLinkRelInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_BT_LINK_REL_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_WS_HEIGHT_IND
CsrSize CsrBtPhdcMgrWsHeightIndSizeof(void *msg)
{
    CsrBtPhdcMgrWsHeightInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_WS_HEIGHT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_WS_HEIGHT_IND
CsrUint8 *CsrBtPhdcMgrWsHeightIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrWsHeightInd *primitive;

    primitive = (CsrBtPhdcMgrWsHeightInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->personId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->unitCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.century);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.year);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.month);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.day);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.hour);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.minute);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.second);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.secondFractions);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->height);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_WS_HEIGHT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_WS_HEIGHT_IND
void *CsrBtPhdcMgrWsHeightIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrWsHeightInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrWsHeightInd *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrWsHeightInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->personId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->unitCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.century, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.year, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.month, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.day, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.hour, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.minute, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.second, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.secondFractions, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->height, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_WS_HEIGHT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DS_IND
CsrSize CsrBtPhdcMgrDsIndSizeof(void *msg)
{
    CsrBtPhdcMgrDsInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DS_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DS_IND
CsrUint8 *CsrBtPhdcMgrDsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrDsInd *primitive;

    primitive = (CsrBtPhdcMgrDsInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->personId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scanReportType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLength);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DS_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DS_IND
void *CsrBtPhdcMgrDsIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrDsInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrDsInd *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrDsInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->personId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scanReportType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    if (primitive->dataLength)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->dataLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DS_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DS_IND
void CsrBtPhdcMgrDsIndSerFree(void *msg)
{
    CsrBtPhdcMgrDsInd *primitive;

    primitive = (CsrBtPhdcMgrDsInd *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DS_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_REQ
CsrSize CsrBtPhdcMgrConfigureEndpointReqSizeof(void *msg)
{
    CsrBtPhdcMgrConfigureEndpointReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->serviceName) + CsrCharStringSerLen((CsrCharString *) primitive->serviceDescription) + CsrCharStringSerLen((CsrCharString *) primitive->providerName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_REQ
CsrUint8 *CsrBtPhdcMgrConfigureEndpointReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrConfigureEndpointReq *primitive;

    primitive = (CsrBtPhdcMgrConfigureEndpointReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);
    CsrMemCpySer(buffer, length, &primitive->serviceName, sizeof(primitive->serviceName));
    CsrMemCpySer(buffer, length, &primitive->serviceDescription, sizeof(primitive->serviceDescription));
    CsrMemCpySer(buffer, length, &primitive->providerName, sizeof(primitive->providerName));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sniffTimeOut);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->numOfMdep);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->supportedProcedures);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->numOfActiveDeviceConnections);
    CsrCharStringSer(buffer, length, primitive->serviceName);
    CsrCharStringSer(buffer, length, primitive->serviceDescription);
    CsrCharStringSer(buffer, length, primitive->providerName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_REQ
void *CsrBtPhdcMgrConfigureEndpointReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrConfigureEndpointReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrConfigureEndpointReq *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrConfigureEndpointReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->serviceName)));
    CsrMemCpyDes(((void *) &primitive->serviceDescription), buffer, &offset, ((CsrSize) sizeof(primitive->serviceDescription)));
    CsrMemCpyDes(((void *) &primitive->providerName), buffer, &offset, ((CsrSize) sizeof(primitive->providerName)));
    CsrUint32Des((CsrUint32 *) &primitive->sniffTimeOut, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->numOfMdep, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->supportedProcedures, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->numOfActiveDeviceConnections, buffer, &offset);
    CsrCharStringDes(&primitive->serviceName, buffer, &offset);
    CsrCharStringDes(&primitive->serviceDescription, buffer, &offset);
    CsrCharStringDes(&primitive->providerName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_REQ
void CsrBtPhdcMgrConfigureEndpointReqSerFree(void *msg)
{
    CsrBtPhdcMgrConfigureEndpointReq *primitive;

    primitive = (CsrBtPhdcMgrConfigureEndpointReq *) msg;

    if(primitive->serviceName != NULL)
    {
        CsrPmemFree(primitive->serviceName);
    }
    if(primitive->serviceDescription != NULL)
    {
        CsrPmemFree(primitive->serviceDescription);
    }
    if(primitive->providerName != NULL)
    {
        CsrPmemFree(primitive->providerName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_CFM
CsrSize CsrBtPhdcMgrDisassociateCfmSizeof(void *msg)
{
    CsrBtPhdcMgrDisassociateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_CFM
CsrUint8 *CsrBtPhdcMgrDisassociateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrDisassociateCfm *primitive;

    primitive = (CsrBtPhdcMgrDisassociateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_CFM
void *CsrBtPhdcMgrDisassociateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrDisassociateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrDisassociateCfm *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrDisassociateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_RES
CsrSize CsrBtPhdcMgrConfigureEndpointResSizeof(void *msg)
{
    CsrBtPhdcMgrConfigureEndpointRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->description));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_RES
CsrUint8 *CsrBtPhdcMgrConfigureEndpointResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrConfigureEndpointRes *primitive;

    primitive = (CsrBtPhdcMgrConfigureEndpointRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->datatype);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);
    CsrMemCpySer(buffer, length, &primitive->description, sizeof(primitive->description));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reuseMdepId);
    CsrUtf8StringSer(buffer, length, primitive->description);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_RES
void *CsrBtPhdcMgrConfigureEndpointResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrConfigureEndpointRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrConfigureEndpointRes *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrConfigureEndpointRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->datatype, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->description), buffer, &offset, ((CsrSize) sizeof(primitive->description)));
    CsrUint8Des((CsrUint8 *) &primitive->reuseMdepId, buffer, &offset);
    CsrUtf8StringDes(&primitive->description, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_RES
void CsrBtPhdcMgrConfigureEndpointResSerFree(void *msg)
{
    CsrBtPhdcMgrConfigureEndpointRes *primitive;

    primitive = (CsrBtPhdcMgrConfigureEndpointRes *) msg;

    if(primitive->description != NULL)
    {
        CsrPmemFree(primitive->description);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_IND
CsrSize CsrBtPhdcMgrConfigureEndpointIndSizeof(void *msg)
{
    CsrBtPhdcMgrConfigureEndpointInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_IND
CsrUint8 *CsrBtPhdcMgrConfigureEndpointIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrConfigureEndpointInd *primitive;

    primitive = (CsrBtPhdcMgrConfigureEndpointInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_IND
void *CsrBtPhdcMgrConfigureEndpointIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrConfigureEndpointInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrConfigureEndpointInd *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrConfigureEndpointInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_REQ
CsrSize CsrBtPhdcMgrDisassociateReqSizeof(void *msg)
{
    CsrBtPhdcMgrDisassociateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_REQ
CsrUint8 *CsrBtPhdcMgrDisassociateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrDisassociateReq *primitive;

    primitive = (CsrBtPhdcMgrDisassociateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rlrqReason);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_REQ
void *CsrBtPhdcMgrDisassociateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrDisassociateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrDisassociateReq *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrDisassociateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->rlrqReason, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_CFM
CsrSize CsrBtPhdcMgrConfigureEndpointCfmSizeof(void *msg)
{
    CsrBtPhdcMgrConfigureEndpointCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_CFM
CsrUint8 *CsrBtPhdcMgrConfigureEndpointCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrConfigureEndpointCfm *primitive;

    primitive = (CsrBtPhdcMgrConfigureEndpointCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_CFM
void *CsrBtPhdcMgrConfigureEndpointCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrConfigureEndpointCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrConfigureEndpointCfm *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrConfigureEndpointCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_CONFIGURE_ENDPOINT_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_IND
CsrSize CsrBtPhdcMgrAcceptBtLinkIndSizeof(void *msg)
{
    CsrBtPhdcMgrAcceptBtLinkInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_IND
CsrUint8 *CsrBtPhdcMgrAcceptBtLinkIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrAcceptBtLinkInd *primitive;

    primitive = (CsrBtPhdcMgrAcceptBtLinkInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_IND
void *CsrBtPhdcMgrAcceptBtLinkIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrAcceptBtLinkInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrAcceptBtLinkInd *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrAcceptBtLinkInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_BT_LINK_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_IND
CsrSize CsrBtPhdcMgrDisassociateIndSizeof(void *msg)
{
    CsrBtPhdcMgrDisassociateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_IND
CsrUint8 *CsrBtPhdcMgrDisassociateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrDisassociateInd *primitive;

    primitive = (CsrBtPhdcMgrDisassociateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_IND
void *CsrBtPhdcMgrDisassociateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrDisassociateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrDisassociateInd *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrDisassociateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_DISASSOCIATE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_CFM
CsrSize CsrBtPhdcMgrUnregisterCfmSizeof(void *msg)
{
    CsrBtPhdcMgrUnregisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_CFM
CsrUint8 *CsrBtPhdcMgrUnregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrUnregisterCfm *primitive;

    primitive = (CsrBtPhdcMgrUnregisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_CFM
void *CsrBtPhdcMgrUnregisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrUnregisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrUnregisterCfm *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrUnregisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ASSOCIATE_COMPLETE_IND
CsrSize CsrBtPhdcMgrAssociateCompleteIndSizeof(void *msg)
{
    CsrBtPhdcMgrAssociateCompleteInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ASSOCIATE_COMPLETE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ASSOCIATE_COMPLETE_IND
CsrUint8 *CsrBtPhdcMgrAssociateCompleteIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrAssociateCompleteInd *primitive;

    primitive = (CsrBtPhdcMgrAssociateCompleteInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->psmIdentifier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdepDataType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketLength);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ASSOCIATE_COMPLETE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ASSOCIATE_COMPLETE_IND
void *CsrBtPhdcMgrAssociateCompleteIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrAssociateCompleteInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrAssociateCompleteInd *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrAssociateCompleteInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->psmIdentifier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdepDataType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketLength, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ASSOCIATE_COMPLETE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_IND
CsrSize CsrBtPhdcMgrAcceptAssociateIndSizeof(void *msg)
{
    CsrBtPhdcMgrAcceptAssociateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_IND
CsrUint8 *CsrBtPhdcMgrAcceptAssociateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrAcceptAssociateInd *primitive;

    primitive = (CsrBtPhdcMgrAcceptAssociateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->devConfigId);
    CsrMemCpySer(buffer, length, ((const void *) primitive->systemId), ((CsrSize) (8)));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_IND
void *CsrBtPhdcMgrAcceptAssociateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrAcceptAssociateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrAcceptAssociateInd *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrAcceptAssociateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->devConfigId, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->systemId), buffer, &offset, ((CsrSize) (8)));
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND
CsrSize CsrBtPhdcMgrGetDeviceCapabIndSizeof(void *msg)
{
    CsrBtPhdcMgrGetDeviceCapabInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->serviceName) + CsrCharStringSerLen((CsrCharString *) primitive->providerName) + (primitive->supportedFeatureListLength * sizeof(CsrBtHdpMdep)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND
CsrUint8 *CsrBtPhdcMgrGetDeviceCapabIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrGetDeviceCapabInd *primitive;

    primitive = (CsrBtPhdcMgrGetDeviceCapabInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->psmIdentifier);
    CsrMemCpySer(buffer, length, &primitive->serviceName, sizeof(primitive->serviceName));
    CsrMemCpySer(buffer, length, &primitive->providerName, sizeof(primitive->providerName));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->supportedFeatureListLength);
    CsrMemCpySer(buffer, length, &primitive->supportedFeatureList, sizeof(primitive->supportedFeatureList));
    CsrCharStringSer(buffer, length, primitive->serviceName);
    CsrCharStringSer(buffer, length, primitive->providerName);
    if(primitive->supportedFeatureList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->supportedFeatureList), ((CsrSize) (primitive->supportedFeatureListLength * sizeof(CsrBtHdpMdep))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND
void *CsrBtPhdcMgrGetDeviceCapabIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrGetDeviceCapabInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrGetDeviceCapabInd *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrGetDeviceCapabInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->psmIdentifier, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->serviceName)));
    CsrMemCpyDes(((void *) &primitive->providerName), buffer, &offset, ((CsrSize) sizeof(primitive->providerName)));
    CsrUint8Des((CsrUint8 *) &primitive->supportedFeatureListLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->supportedFeatureList), buffer, &offset, ((CsrSize) sizeof(primitive->supportedFeatureList)));
    CsrCharStringDes(&primitive->serviceName, buffer, &offset);
    CsrCharStringDes(&primitive->providerName, buffer, &offset);
    if (primitive->supportedFeatureListLength)
    {
        primitive->supportedFeatureList = CsrPmemZalloc(((CsrUint32) (primitive->supportedFeatureListLength * sizeof(CsrBtHdpMdep))));
        CsrMemCpyDes(((void *) primitive->supportedFeatureList), buffer, &offset, ((CsrSize) (primitive->supportedFeatureListLength * sizeof(CsrBtHdpMdep))));
    }
    else
    {
        primitive->supportedFeatureList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND
void CsrBtPhdcMgrGetDeviceCapabIndSerFree(void *msg)
{
    CsrBtPhdcMgrGetDeviceCapabInd *primitive;

    primitive = (CsrBtPhdcMgrGetDeviceCapabInd *) msg;

    if(primitive->serviceName != NULL)
    {
        CsrPmemFree(primitive->serviceName);
    }
    if(primitive->providerName != NULL)
    {
        CsrPmemFree(primitive->providerName);
    }
    if(primitive->supportedFeatureList != NULL)
    {
        CsrPmemFree(primitive->supportedFeatureList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_REQ
CsrSize CsrBtPhdcMgrUnregisterReqSizeof(void *msg)
{
    CsrBtPhdcMgrUnregisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_REQ
CsrUint8 *CsrBtPhdcMgrUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrUnregisterReq *primitive;

    primitive = (CsrBtPhdcMgrUnregisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_REQ
void *CsrBtPhdcMgrUnregisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrUnregisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrUnregisterReq *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrUnregisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_BPM_PULSE_IND
CsrSize CsrBtPhdcMgrBpmPulseIndSizeof(void *msg)
{
    CsrBtPhdcMgrBpmPulseInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_BPM_PULSE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_BPM_PULSE_IND
CsrUint8 *CsrBtPhdcMgrBpmPulseIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrBpmPulseInd *primitive;

    primitive = (CsrBtPhdcMgrBpmPulseInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->personId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->unitCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.century);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.year);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.month);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.day);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.hour);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.minute);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.second);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.secondFractions);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pulse);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_BPM_PULSE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_BPM_PULSE_IND
void *CsrBtPhdcMgrBpmPulseIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrBpmPulseInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrBpmPulseInd *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrBpmPulseInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->personId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->unitCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.century, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.year, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.month, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.day, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.hour, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.minute, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.second, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.secondFractions, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pulse, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_BPM_PULSE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_REQ
CsrSize CsrBtPhdcMgrSetupBtLinkReqSizeof(void *msg)
{
    CsrBtPhdcMgrSetupBtLinkReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_REQ
CsrUint8 *CsrBtPhdcMgrSetupBtLinkReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrSetupBtLinkReq *primitive;

    primitive = (CsrBtPhdcMgrSetupBtLinkReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->psmIdentifier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdepDataType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_REQ
void *CsrBtPhdcMgrSetupBtLinkReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrSetupBtLinkReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrSetupBtLinkReq *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrSetupBtLinkReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->psmIdentifier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdepDataType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_BPM_BP_IND
CsrSize CsrBtPhdcMgrBpmBpIndSizeof(void *msg)
{
    CsrBtPhdcMgrBpmBpInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_BPM_BP_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_BPM_BP_IND
CsrUint8 *CsrBtPhdcMgrBpmBpIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrBpmBpInd *primitive;

    primitive = (CsrBtPhdcMgrBpmBpInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->personId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->unitCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.century);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.year);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.month);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.day);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.hour);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.minute);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.second);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.secondFractions);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->systolicPressure);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->diastolicPressure);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->meanArterialPressure);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_BPM_BP_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_BPM_BP_IND
void *CsrBtPhdcMgrBpmBpIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrBpmBpInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrBpmBpInd *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrBpmBpInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->personId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->unitCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.century, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.year, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.month, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.day, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.hour, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.minute, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.second, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.secondFractions, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->systolicPressure, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->diastolicPressure, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->meanArterialPressure, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_BPM_BP_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_RES
CsrSize CsrBtPhdcMgrAcceptAssociateResSizeof(void *msg)
{
    CsrBtPhdcMgrAcceptAssociateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_RES
CsrUint8 *CsrBtPhdcMgrAcceptAssociateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrAcceptAssociateRes *primitive;

    primitive = (CsrBtPhdcMgrAcceptAssociateRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, ((const void *) primitive->systemId), ((CsrSize) (8)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_RES
void *CsrBtPhdcMgrAcceptAssociateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrAcceptAssociateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrAcceptAssociateRes *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrAcceptAssociateRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->systemId), buffer, &offset, ((CsrSize) (8)));
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_ACCEPT_ASSOCIATE_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_CFM
CsrSize CsrBtPhdcMgrGetDeviceCapabCfmSizeof(void *msg)
{
    CsrBtPhdcMgrGetDeviceCapabCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_CFM
CsrUint8 *CsrBtPhdcMgrGetDeviceCapabCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrGetDeviceCapabCfm *primitive;

    primitive = (CsrBtPhdcMgrGetDeviceCapabCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_CFM
void *CsrBtPhdcMgrGetDeviceCapabCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrGetDeviceCapabCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrGetDeviceCapabCfm *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrGetDeviceCapabCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_REQ
CsrSize CsrBtPhdcMgrGetDeviceCapabReqSizeof(void *msg)
{
    CsrBtPhdcMgrGetDeviceCapabReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_REQ
CsrUint8 *CsrBtPhdcMgrGetDeviceCapabReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrGetDeviceCapabReq *primitive;

    primitive = (CsrBtPhdcMgrGetDeviceCapabReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepDataTypeMask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_REQ
void *CsrBtPhdcMgrGetDeviceCapabReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrGetDeviceCapabReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrGetDeviceCapabReq *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrGetDeviceCapabReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepDataTypeMask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_GET_DEVICE_CAPAB_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_WS_BMI_IND
CsrSize CsrBtPhdcMgrWsBmiIndSizeof(void *msg)
{
    CsrBtPhdcMgrWsBmiInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_WS_BMI_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_WS_BMI_IND
CsrUint8 *CsrBtPhdcMgrWsBmiIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrWsBmiInd *primitive;

    primitive = (CsrBtPhdcMgrWsBmiInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->personId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->unitCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.century);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.year);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.month);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.day);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.hour);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.minute);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.second);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.secondFractions);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bmi);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_WS_BMI_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_WS_BMI_IND
void *CsrBtPhdcMgrWsBmiIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrWsBmiInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrWsBmiInd *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrWsBmiInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->personId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->unitCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.century, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.year, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.month, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.day, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.hour, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.minute, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.second, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.secondFractions, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bmi, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_WS_BMI_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_WS_WEIGHT_IND
CsrSize CsrBtPhdcMgrWsWeightIndSizeof(void *msg)
{
    CsrBtPhdcMgrWsWeightInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_WS_WEIGHT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_WS_WEIGHT_IND
CsrUint8 *CsrBtPhdcMgrWsWeightIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrWsWeightInd *primitive;

    primitive = (CsrBtPhdcMgrWsWeightInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->personId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->unitCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.century);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.year);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.month);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.day);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.hour);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.minute);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.second);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->time.secondFractions);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->weight);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_WS_WEIGHT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_WS_WEIGHT_IND
void *CsrBtPhdcMgrWsWeightIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrWsWeightInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrWsWeightInd *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrWsWeightInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->personId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->unitCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.century, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.year, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.month, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.day, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.hour, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.minute, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.second, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->time.secondFractions, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->weight, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_WS_WEIGHT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND
CsrSize CsrBtPhdcMgrExtConfigObjIndSizeof(void *msg)
{
    CsrBtPhdcMgrExtConfigObjInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND
CsrUint8 *CsrBtPhdcMgrExtConfigObjIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrExtConfigObjInd *primitive;

    primitive = (CsrBtPhdcMgrExtConfigObjInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->devConfigId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->opCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLength);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND
void *CsrBtPhdcMgrExtConfigObjIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrExtConfigObjInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrExtConfigObjInd *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrExtConfigObjInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->devConfigId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->opCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    if (primitive->dataLength)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->dataLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->dataLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND
void CsrBtPhdcMgrExtConfigObjIndSerFree(void *msg)
{
    CsrBtPhdcMgrExtConfigObjInd *primitive;

    primitive = (CsrBtPhdcMgrExtConfigObjInd *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_EXT_CONFIG_OBJ_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_CFM
CsrSize CsrBtPhdcMgrSetupBtLinkCfmSizeof(void *msg)
{
    CsrBtPhdcMgrSetupBtLinkCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_CFM
CsrUint8 *CsrBtPhdcMgrSetupBtLinkCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcMgrSetupBtLinkCfm *primitive;

    primitive = (CsrBtPhdcMgrSetupBtLinkCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->psmIdentifier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketLength);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_CFM
void *CsrBtPhdcMgrSetupBtLinkCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcMgrSetupBtLinkCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcMgrSetupBtLinkCfm *) CsrPmemZalloc(sizeof(CsrBtPhdcMgrSetupBtLinkCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->psmIdentifier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketLength, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_MGR_SETUP_BT_LINK_CFM */

#endif /* EXCLUDE_CSR_BT_PHDC_MGR_MODULE */
