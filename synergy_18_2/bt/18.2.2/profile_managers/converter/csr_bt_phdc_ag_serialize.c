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
#include "csr_bt_phdc_ag_prim.h"

#ifndef EXCLUDE_CSR_BT_PHDC_AG_MODULE

void CsrBtPhdc_agPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_REQ
CsrSize CsrBtPhdcAgAssociateReqSizeof(void *msg)
{
    CsrBtPhdcAgAssociateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_REQ
CsrUint8 *CsrBtPhdcAgAssociateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgAssociateReq *primitive;

    primitive = (CsrBtPhdcAgAssociateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->psmIdentifier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdepDataType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->devConfigId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->release);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_REQ
void *CsrBtPhdcAgAssociateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgAssociateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgAssociateReq *) CsrPmemZalloc(sizeof(CsrBtPhdcAgAssociateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->psmIdentifier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdepDataType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->devConfigId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->release, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_IND
CsrSize CsrBtPhdcAgDimAttribIndSizeof(void *msg)
{
    CsrBtPhdcAgDimAttribInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_IND
CsrUint8 *CsrBtPhdcAgDimAttribIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgDimAttribInd *primitive;

    primitive = (CsrBtPhdcAgDimAttribInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objIndex);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attribIndex);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_IND
void *CsrBtPhdcAgDimAttribIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgDimAttribInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgDimAttribInd *) CsrPmemZalloc(sizeof(CsrBtPhdcAgDimAttribInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objIndex, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attribIndex, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_IND
CsrSize CsrBtPhdcAgDisassociateIndSizeof(void *msg)
{
    CsrBtPhdcAgDisassociateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_IND
CsrUint8 *CsrBtPhdcAgDisassociateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgDisassociateInd *primitive;

    primitive = (CsrBtPhdcAgDisassociateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_IND
void *CsrBtPhdcAgDisassociateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgDisassociateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgDisassociateInd *) CsrPmemZalloc(sizeof(CsrBtPhdcAgDisassociateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_RESUME_CFM
CsrSize CsrBtPhdcAgResumeCfmSizeof(void *msg)
{
    CsrBtPhdcAgResumeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_RESUME_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_RESUME_CFM
CsrUint8 *CsrBtPhdcAgResumeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgResumeCfm *primitive;

    primitive = (CsrBtPhdcAgResumeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_RESUME_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_RESUME_CFM
void *CsrBtPhdcAgResumeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgResumeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgResumeCfm *) CsrPmemZalloc(sizeof(CsrBtPhdcAgResumeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_RESUME_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_REQ
CsrSize CsrBtPhdcAgGetDeviceCapabReqSizeof(void *msg)
{
    CsrBtPhdcAgGetDeviceCapabReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_REQ
CsrUint8 *CsrBtPhdcAgGetDeviceCapabReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgGetDeviceCapabReq *primitive;

    primitive = (CsrBtPhdcAgGetDeviceCapabReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepDataTypeMask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_REQ
void *CsrBtPhdcAgGetDeviceCapabReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgGetDeviceCapabReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgGetDeviceCapabReq *) CsrPmemZalloc(sizeof(CsrBtPhdcAgGetDeviceCapabReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepDataTypeMask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND
CsrSize CsrBtPhdcAgGetDeviceCapabIndSizeof(void *msg)
{
    CsrBtPhdcAgGetDeviceCapabInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->serviceName) + CsrCharStringSerLen((CsrCharString *) primitive->providerName) + (primitive->supportedFeatureListLength * sizeof(CsrBtHdpMdep)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND
CsrUint8 *CsrBtPhdcAgGetDeviceCapabIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgGetDeviceCapabInd *primitive;

    primitive = (CsrBtPhdcAgGetDeviceCapabInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND
void *CsrBtPhdcAgGetDeviceCapabIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgGetDeviceCapabInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgGetDeviceCapabInd *) CsrPmemZalloc(sizeof(CsrBtPhdcAgGetDeviceCapabInd));

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
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND
void CsrBtPhdcAgGetDeviceCapabIndSerFree(void *msg)
{
    CsrBtPhdcAgGetDeviceCapabInd *primitive;

    primitive = (CsrBtPhdcAgGetDeviceCapabInd *) msg;

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
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_RES
CsrSize CsrBtPhdcAgDimAttribResSizeof(void *msg)
{
    CsrBtPhdcAgDimAttribRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attribInfoLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_RES
CsrUint8 *CsrBtPhdcAgDimAttribResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgDimAttribRes *primitive;

    primitive = (CsrBtPhdcAgDimAttribRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objIndex);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attribIndex);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attribId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attribInfoLength);
    CsrMemCpySer(buffer, length, &primitive->attribInfo, sizeof(primitive->attribInfo));
    if(primitive->attribInfo)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attribInfo), ((CsrSize) (primitive->attribInfoLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_RES
void *CsrBtPhdcAgDimAttribResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgDimAttribRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgDimAttribRes *) CsrPmemZalloc(sizeof(CsrBtPhdcAgDimAttribRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objIndex, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attribIndex, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attribId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attribInfoLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attribInfo), buffer, &offset, ((CsrSize) sizeof(primitive->attribInfo)));
    if (primitive->attribInfoLength)
    {
        primitive->attribInfo = CsrPmemZalloc(((CsrUint32) (primitive->attribInfoLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->attribInfo), buffer, &offset, ((CsrSize) (primitive->attribInfoLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->attribInfo = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_RES
void CsrBtPhdcAgDimAttribResSerFree(void *msg)
{
    CsrBtPhdcAgDimAttribRes *primitive;

    primitive = (CsrBtPhdcAgDimAttribRes *) msg;

    if(primitive->attribInfo != NULL)
    {
        CsrPmemFree(primitive->attribInfo);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_ATTRIB_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_CFM
CsrSize CsrBtPhdcAgConfigureEndpointCfmSizeof(void *msg)
{
    CsrBtPhdcAgConfigureEndpointCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_CFM
CsrUint8 *CsrBtPhdcAgConfigureEndpointCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgConfigureEndpointCfm *primitive;

    primitive = (CsrBtPhdcAgConfigureEndpointCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_CFM
void *CsrBtPhdcAgConfigureEndpointCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgConfigureEndpointCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgConfigureEndpointCfm *) CsrPmemZalloc(sizeof(CsrBtPhdcAgConfigureEndpointCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_BT_LINK_REL_IND
CsrSize CsrBtPhdcAgBtLinkRelIndSizeof(void *msg)
{
    CsrBtPhdcAgBtLinkRelInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_BT_LINK_REL_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_BT_LINK_REL_IND
CsrUint8 *CsrBtPhdcAgBtLinkRelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgBtLinkRelInd *primitive;

    primitive = (CsrBtPhdcAgBtLinkRelInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_BT_LINK_REL_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_BT_LINK_REL_IND
void *CsrBtPhdcAgBtLinkRelIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgBtLinkRelInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgBtLinkRelInd *) CsrPmemZalloc(sizeof(CsrBtPhdcAgBtLinkRelInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_BT_LINK_REL_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_IND
CsrSize CsrBtPhdcAgSuspendIndSizeof(void *msg)
{
    CsrBtPhdcAgSuspendInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_IND
CsrUint8 *CsrBtPhdcAgSuspendIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgSuspendInd *primitive;

    primitive = (CsrBtPhdcAgSuspendInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_IND
void *CsrBtPhdcAgSuspendIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgSuspendInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgSuspendInd *) CsrPmemZalloc(sizeof(CsrBtPhdcAgSuspendInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_SETUP_BT_LINK_IND
CsrSize CsrBtPhdcAgSetupBtLinkIndSizeof(void *msg)
{
    CsrBtPhdcAgSetupBtLinkInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SETUP_BT_LINK_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_SETUP_BT_LINK_IND
CsrUint8 *CsrBtPhdcAgSetupBtLinkIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgSetupBtLinkInd *primitive;

    primitive = (CsrBtPhdcAgSetupBtLinkInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SETUP_BT_LINK_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_SETUP_BT_LINK_IND
void *CsrBtPhdcAgSetupBtLinkIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgSetupBtLinkInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgSetupBtLinkInd *) CsrPmemZalloc(sizeof(CsrBtPhdcAgSetupBtLinkInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SETUP_BT_LINK_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_CFM
CsrSize CsrBtPhdcAgSuspendCfmSizeof(void *msg)
{
    CsrBtPhdcAgSuspendCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_CFM
CsrUint8 *CsrBtPhdcAgSuspendCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgSuspendCfm *primitive;

    primitive = (CsrBtPhdcAgSuspendCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_CFM
void *CsrBtPhdcAgSuspendCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgSuspendCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgSuspendCfm *) CsrPmemZalloc(sizeof(CsrBtPhdcAgSuspendCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DATA_CFM
CsrSize CsrBtPhdcAgDataCfmSizeof(void *msg)
{
    CsrBtPhdcAgDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DATA_CFM
CsrUint8 *CsrBtPhdcAgDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgDataCfm *primitive;

    primitive = (CsrBtPhdcAgDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DATA_CFM
void *CsrBtPhdcAgDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgDataCfm *) CsrPmemZalloc(sizeof(CsrBtPhdcAgDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_RESUME_REQ
CsrSize CsrBtPhdcAgResumeReqSizeof(void *msg)
{
    CsrBtPhdcAgResumeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_RESUME_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_RESUME_REQ
CsrUint8 *CsrBtPhdcAgResumeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgResumeReq *primitive;

    primitive = (CsrBtPhdcAgResumeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_RESUME_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_RESUME_REQ
void *CsrBtPhdcAgResumeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgResumeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgResumeReq *) CsrPmemZalloc(sizeof(CsrBtPhdcAgResumeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_RESUME_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_IND
CsrSize CsrBtPhdcAgAcceptBtLinkIndSizeof(void *msg)
{
    CsrBtPhdcAgAcceptBtLinkInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_IND
CsrUint8 *CsrBtPhdcAgAcceptBtLinkIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgAcceptBtLinkInd *primitive;

    primitive = (CsrBtPhdcAgAcceptBtLinkInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_IND
void *CsrBtPhdcAgAcceptBtLinkIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgAcceptBtLinkInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgAcceptBtLinkInd *) CsrPmemZalloc(sizeof(CsrBtPhdcAgAcceptBtLinkInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DATA_REQ
CsrSize CsrBtPhdcAgDataReqSizeof(void *msg)
{
    CsrBtPhdcAgDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->data.dataLength * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DATA_REQ
CsrUint8 *CsrBtPhdcAgDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgDataReq *primitive;

    primitive = (CsrBtPhdcAgDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->transmitFlag);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->data.objHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->data.personId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->data.time.century);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->data.time.year);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->data.time.month);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->data.time.day);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->data.time.hour);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->data.time.minute);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->data.time.second);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->data.time.secondFractions);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->data.dataLength);
    CsrMemCpySer(buffer, length, &primitive->data.data, sizeof(primitive->data.data));
    if(primitive->data.data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data.data), ((CsrSize) (primitive->data.dataLength * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DATA_REQ
void *CsrBtPhdcAgDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgDataReq *) CsrPmemZalloc(sizeof(CsrBtPhdcAgDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->transmitFlag, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->data.objHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->data.personId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->data.time.century, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->data.time.year, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->data.time.month, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->data.time.day, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->data.time.hour, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->data.time.minute, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->data.time.second, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->data.time.secondFractions, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->data.dataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data.data), buffer, &offset, ((CsrSize) sizeof(primitive->data.data)));
    if (primitive->data.dataLength)
    {
        primitive->data.data = CsrPmemZalloc(((CsrUint32) (primitive->data.dataLength * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->data.data), buffer, &offset, ((CsrSize) (primitive->data.dataLength * sizeof(CsrUint16))));
    }
    else
    {
        primitive->data.data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DATA_REQ
void CsrBtPhdcAgDataReqSerFree(void *msg)
{
    CsrBtPhdcAgDataReq *primitive;

    primitive = (CsrBtPhdcAgDataReq *) msg;

    if(primitive->data.data != NULL)
    {
        CsrPmemFree(primitive->data.data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_REQ
CsrSize CsrBtPhdcAgDisassociateReqSizeof(void *msg)
{
    CsrBtPhdcAgDisassociateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_REQ
CsrUint8 *CsrBtPhdcAgDisassociateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgDisassociateReq *primitive;

    primitive = (CsrBtPhdcAgDisassociateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rlrqReason);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_REQ
void *CsrBtPhdcAgDisassociateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgDisassociateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgDisassociateReq *) CsrPmemZalloc(sizeof(CsrBtPhdcAgDisassociateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->rlrqReason, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_RESUME_IND
CsrSize CsrBtPhdcAgResumeIndSizeof(void *msg)
{
    CsrBtPhdcAgResumeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_RESUME_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_RESUME_IND
CsrUint8 *CsrBtPhdcAgResumeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgResumeInd *primitive;

    primitive = (CsrBtPhdcAgResumeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_RESUME_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_RESUME_IND
void *CsrBtPhdcAgResumeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgResumeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgResumeInd *) CsrPmemZalloc(sizeof(CsrBtPhdcAgResumeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_RESUME_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_REQ
CsrSize CsrBtPhdcAgChangeDimUnitCodeReqSizeof(void *msg)
{
    CsrBtPhdcAgChangeDimUnitCodeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_REQ
CsrUint8 *CsrBtPhdcAgChangeDimUnitCodeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgChangeDimUnitCodeReq *primitive;

    primitive = (CsrBtPhdcAgChangeDimUnitCodeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->unitCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_REQ
void *CsrBtPhdcAgChangeDimUnitCodeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgChangeDimUnitCodeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgChangeDimUnitCodeReq *) CsrPmemZalloc(sizeof(CsrBtPhdcAgChangeDimUnitCodeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->unitCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES
CsrSize CsrBtPhdcAgConfigureEndpointResSizeof(void *msg)
{
    CsrBtPhdcAgConfigureEndpointRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->description));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES
CsrUint8 *CsrBtPhdcAgConfigureEndpointResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgConfigureEndpointRes *primitive;

    primitive = (CsrBtPhdcAgConfigureEndpointRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES
void *CsrBtPhdcAgConfigureEndpointResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgConfigureEndpointRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgConfigureEndpointRes *) CsrPmemZalloc(sizeof(CsrBtPhdcAgConfigureEndpointRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->datatype, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->description), buffer, &offset, ((CsrSize) sizeof(primitive->description)));
    CsrUint8Des((CsrUint8 *) &primitive->reuseMdepId, buffer, &offset);
    CsrUtf8StringDes(&primitive->description, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES
void CsrBtPhdcAgConfigureEndpointResSerFree(void *msg)
{
    CsrBtPhdcAgConfigureEndpointRes *primitive;

    primitive = (CsrBtPhdcAgConfigureEndpointRes *) msg;

    if(primitive->description != NULL)
    {
        CsrPmemFree(primitive->description);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_IND
CsrSize CsrBtPhdcAgDimObjectIndSizeof(void *msg)
{
    CsrBtPhdcAgDimObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_IND
CsrUint8 *CsrBtPhdcAgDimObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgDimObjectInd *primitive;

    primitive = (CsrBtPhdcAgDimObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objIndex);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_IND
void *CsrBtPhdcAgDimObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgDimObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgDimObjectInd *) CsrPmemZalloc(sizeof(CsrBtPhdcAgDimObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objIndex, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_CFM
CsrSize CsrBtPhdcAgDimConfigureCfmSizeof(void *msg)
{
    CsrBtPhdcAgDimConfigureCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_CFM
CsrUint8 *CsrBtPhdcAgDimConfigureCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgDimConfigureCfm *primitive;

    primitive = (CsrBtPhdcAgDimConfigureCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_CFM
void *CsrBtPhdcAgDimConfigureCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgDimConfigureCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgDimConfigureCfm *) CsrPmemZalloc(sizeof(CsrBtPhdcAgDimConfigureCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_REQ
CsrSize CsrBtPhdcAgSuspendReqSizeof(void *msg)
{
    CsrBtPhdcAgSuspendReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_REQ
CsrUint8 *CsrBtPhdcAgSuspendReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgSuspendReq *primitive;

    primitive = (CsrBtPhdcAgSuspendReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataChannelId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_REQ
void *CsrBtPhdcAgSuspendReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgSuspendReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgSuspendReq *) CsrPmemZalloc(sizeof(CsrBtPhdcAgSuspendReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_SUSPEND_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_RES
CsrSize CsrBtPhdcAgAcceptBtLinkResSizeof(void *msg)
{
    CsrBtPhdcAgAcceptBtLinkRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_RES
CsrUint8 *CsrBtPhdcAgAcceptBtLinkResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgAcceptBtLinkRes *primitive;

    primitive = (CsrBtPhdcAgAcceptBtLinkRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_RES
void *CsrBtPhdcAgAcceptBtLinkResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgAcceptBtLinkRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgAcceptBtLinkRes *) CsrPmemZalloc(sizeof(CsrBtPhdcAgAcceptBtLinkRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ACCEPT_BT_LINK_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_CFM
CsrSize CsrBtPhdcAgChangeDimUnitCodeCfmSizeof(void *msg)
{
    CsrBtPhdcAgChangeDimUnitCodeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_CFM
CsrUint8 *CsrBtPhdcAgChangeDimUnitCodeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgChangeDimUnitCodeCfm *primitive;

    primitive = (CsrBtPhdcAgChangeDimUnitCodeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_CFM
void *CsrBtPhdcAgChangeDimUnitCodeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgChangeDimUnitCodeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgChangeDimUnitCodeCfm *) CsrPmemZalloc(sizeof(CsrBtPhdcAgChangeDimUnitCodeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CHANGE_DIM_UNIT_CODE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_CFM
CsrSize CsrBtPhdcAgUnregisterCfmSizeof(void *msg)
{
    CsrBtPhdcAgUnregisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_CFM
CsrUint8 *CsrBtPhdcAgUnregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgUnregisterCfm *primitive;

    primitive = (CsrBtPhdcAgUnregisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_CFM
void *CsrBtPhdcAgUnregisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgUnregisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgUnregisterCfm *) CsrPmemZalloc(sizeof(CsrBtPhdcAgUnregisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_REQ
CsrSize CsrBtPhdcAgUnregisterReqSizeof(void *msg)
{
    CsrBtPhdcAgUnregisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_REQ
CsrUint8 *CsrBtPhdcAgUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgUnregisterReq *primitive;

    primitive = (CsrBtPhdcAgUnregisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_REQ
void *CsrBtPhdcAgUnregisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgUnregisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgUnregisterReq *) CsrPmemZalloc(sizeof(CsrBtPhdcAgUnregisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_CFM
CsrSize CsrBtPhdcAgGetDeviceCapabCfmSizeof(void *msg)
{
    CsrBtPhdcAgGetDeviceCapabCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_CFM
CsrUint8 *CsrBtPhdcAgGetDeviceCapabCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgGetDeviceCapabCfm *primitive;

    primitive = (CsrBtPhdcAgGetDeviceCapabCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_CFM
void *CsrBtPhdcAgGetDeviceCapabCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgGetDeviceCapabCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgGetDeviceCapabCfm *) CsrPmemZalloc(sizeof(CsrBtPhdcAgGetDeviceCapabCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_GET_DEVICE_CAPAB_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_RES
CsrSize CsrBtPhdcAgDimObjectResSizeof(void *msg)
{
    CsrBtPhdcAgDimObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataValueLength * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_RES
CsrUint8 *CsrBtPhdcAgDimObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgDimObjectRes *primitive;

    primitive = (CsrBtPhdcAgDimObjectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objIndex);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objClass);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataValueLength);
    CsrMemCpySer(buffer, length, &primitive->dataValue, sizeof(primitive->dataValue));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->numAttrib);
    if(primitive->dataValue)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->dataValue), ((CsrSize) (primitive->dataValueLength * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_RES
void *CsrBtPhdcAgDimObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgDimObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgDimObjectRes *) CsrPmemZalloc(sizeof(CsrBtPhdcAgDimObjectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objIndex, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objClass, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataValueLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->dataValue), buffer, &offset, ((CsrSize) sizeof(primitive->dataValue)));
    CsrUint16Des((CsrUint16 *) &primitive->numAttrib, buffer, &offset);
    if (primitive->dataValueLength)
    {
        primitive->dataValue = CsrPmemZalloc(((CsrUint32) (primitive->dataValueLength * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->dataValue), buffer, &offset, ((CsrSize) (primitive->dataValueLength * sizeof(CsrUint16))));
    }
    else
    {
        primitive->dataValue = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_RES
void CsrBtPhdcAgDimObjectResSerFree(void *msg)
{
    CsrBtPhdcAgDimObjectRes *primitive;

    primitive = (CsrBtPhdcAgDimObjectRes *) msg;

    if(primitive->dataValue != NULL)
    {
        CsrPmemFree(primitive->dataValue);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_CFM
CsrSize CsrBtPhdcAgAssociateCfmSizeof(void *msg)
{
    CsrBtPhdcAgAssociateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_CFM
CsrUint8 *CsrBtPhdcAgAssociateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgAssociateCfm *primitive;

    primitive = (CsrBtPhdcAgAssociateCfm *) msg;
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

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_CFM
void *CsrBtPhdcAgAssociateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgAssociateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgAssociateCfm *) CsrPmemZalloc(sizeof(CsrBtPhdcAgAssociateCfm));

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

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_ASSOCIATE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_REQ
CsrSize CsrBtPhdcAgDimConfigureReqSizeof(void *msg)
{
    CsrBtPhdcAgDimConfigureReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_REQ
CsrUint8 *CsrBtPhdcAgDimConfigureReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgDimConfigureReq *primitive;

    primitive = (CsrBtPhdcAgDimConfigureReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->devConfigId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->objCount);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_REQ
void *CsrBtPhdcAgDimConfigureReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgDimConfigureReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgDimConfigureReq *) CsrPmemZalloc(sizeof(CsrBtPhdcAgDimConfigureReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->devConfigId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->objCount, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DIM_CONFIGURE_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ
CsrSize CsrBtPhdcAgConfigureMdsReqSizeof(void *msg)
{
    CsrBtPhdcAgConfigureMdsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->manufacturer) + CsrCharStringSerLen((CsrCharString *) primitive->modelNumber) + CsrCharStringSerLen((CsrCharString *) primitive->serialNumber) + CsrCharStringSerLen((CsrCharString *) primitive->fwVersion));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ
CsrUint8 *CsrBtPhdcAgConfigureMdsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgConfigureMdsReq *primitive;

    primitive = (CsrBtPhdcAgConfigureMdsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->supportedDeviceSpecializations);
    CsrMemCpySer(buffer, length, &primitive->manufacturer, sizeof(primitive->manufacturer));
    CsrMemCpySer(buffer, length, &primitive->modelNumber, sizeof(primitive->modelNumber));
    CsrMemCpySer(buffer, length, ((const void *) primitive->systemId), ((CsrSize) (8)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->devConfigId);
    CsrMemCpySer(buffer, length, &primitive->serialNumber, sizeof(primitive->serialNumber));
    CsrMemCpySer(buffer, length, &primitive->fwVersion, sizeof(primitive->fwVersion));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->absolutetime.century);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->absolutetime.year);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->absolutetime.month);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->absolutetime.day);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->absolutetime.hour);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->absolutetime.minute);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->absolutetime.second);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->absolutetime.secondFractions);
    CsrCharStringSer(buffer, length, primitive->manufacturer);
    CsrCharStringSer(buffer, length, primitive->modelNumber);
    CsrCharStringSer(buffer, length, primitive->serialNumber);
    CsrCharStringSer(buffer, length, primitive->fwVersion);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ
void *CsrBtPhdcAgConfigureMdsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgConfigureMdsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgConfigureMdsReq *) CsrPmemZalloc(sizeof(CsrBtPhdcAgConfigureMdsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->supportedDeviceSpecializations, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->manufacturer), buffer, &offset, ((CsrSize) sizeof(primitive->manufacturer)));
    CsrMemCpyDes(((void *) &primitive->modelNumber), buffer, &offset, ((CsrSize) sizeof(primitive->modelNumber)));
    CsrMemCpyDes(((void *) primitive->systemId), buffer, &offset, ((CsrSize) (8)));
    CsrUint16Des((CsrUint16 *) &primitive->devConfigId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serialNumber), buffer, &offset, ((CsrSize) sizeof(primitive->serialNumber)));
    CsrMemCpyDes(((void *) &primitive->fwVersion), buffer, &offset, ((CsrSize) sizeof(primitive->fwVersion)));
    CsrUint8Des((CsrUint8 *) &primitive->absolutetime.century, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->absolutetime.year, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->absolutetime.month, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->absolutetime.day, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->absolutetime.hour, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->absolutetime.minute, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->absolutetime.second, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->absolutetime.secondFractions, buffer, &offset);
    CsrCharStringDes(&primitive->manufacturer, buffer, &offset);
    CsrCharStringDes(&primitive->modelNumber, buffer, &offset);
    CsrCharStringDes(&primitive->serialNumber, buffer, &offset);
    CsrCharStringDes(&primitive->fwVersion, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ
void CsrBtPhdcAgConfigureMdsReqSerFree(void *msg)
{
    CsrBtPhdcAgConfigureMdsReq *primitive;

    primitive = (CsrBtPhdcAgConfigureMdsReq *) msg;

    if(primitive->manufacturer != NULL)
    {
        CsrPmemFree(primitive->manufacturer);
    }
    if(primitive->modelNumber != NULL)
    {
        CsrPmemFree(primitive->modelNumber);
    }
    if(primitive->serialNumber != NULL)
    {
        CsrPmemFree(primitive->serialNumber);
    }
    if(primitive->fwVersion != NULL)
    {
        CsrPmemFree(primitive->fwVersion);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_MDS_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_IND
CsrSize CsrBtPhdcAgConfigureEndpointIndSizeof(void *msg)
{
    CsrBtPhdcAgConfigureEndpointInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_IND
CsrUint8 *CsrBtPhdcAgConfigureEndpointIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgConfigureEndpointInd *primitive;

    primitive = (CsrBtPhdcAgConfigureEndpointInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_IND
void *CsrBtPhdcAgConfigureEndpointIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgConfigureEndpointInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgConfigureEndpointInd *) CsrPmemZalloc(sizeof(CsrBtPhdcAgConfigureEndpointInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_IND */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ
CsrSize CsrBtPhdcAgConfigureEndpointReqSizeof(void *msg)
{
    CsrBtPhdcAgConfigureEndpointReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->serviceName) + CsrCharStringSerLen((CsrCharString *) primitive->serviceDescription) + CsrCharStringSerLen((CsrCharString *) primitive->providerName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ
CsrUint8 *CsrBtPhdcAgConfigureEndpointReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgConfigureEndpointReq *primitive;

    primitive = (CsrBtPhdcAgConfigureEndpointReq *) msg;
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
    CsrCharStringSer(buffer, length, primitive->serviceName);
    CsrCharStringSer(buffer, length, primitive->serviceDescription);
    CsrCharStringSer(buffer, length, primitive->providerName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ
void *CsrBtPhdcAgConfigureEndpointReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgConfigureEndpointReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgConfigureEndpointReq *) CsrPmemZalloc(sizeof(CsrBtPhdcAgConfigureEndpointReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->serviceName)));
    CsrMemCpyDes(((void *) &primitive->serviceDescription), buffer, &offset, ((CsrSize) sizeof(primitive->serviceDescription)));
    CsrMemCpyDes(((void *) &primitive->providerName), buffer, &offset, ((CsrSize) sizeof(primitive->providerName)));
    CsrUint32Des((CsrUint32 *) &primitive->sniffTimeOut, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->numOfMdep, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->supportedProcedures, buffer, &offset);
    CsrCharStringDes(&primitive->serviceName, buffer, &offset);
    CsrCharStringDes(&primitive->serviceDescription, buffer, &offset);
    CsrCharStringDes(&primitive->providerName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ
void CsrBtPhdcAgConfigureEndpointReqSerFree(void *msg)
{
    CsrBtPhdcAgConfigureEndpointReq *primitive;

    primitive = (CsrBtPhdcAgConfigureEndpointReq *) msg;

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
#endif /* EXCLUDE_CSR_BT_PHDC_AG_CONFIGURE_ENDPOINT_REQ */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_CFM
CsrSize CsrBtPhdcAgDisassociateCfmSizeof(void *msg)
{
    CsrBtPhdcAgDisassociateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_CFM
CsrUint8 *CsrBtPhdcAgDisassociateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtPhdcAgDisassociateCfm *primitive;

    primitive = (CsrBtPhdcAgDisassociateCfm *) msg;
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
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_CFM */

#ifndef EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_CFM
void *CsrBtPhdcAgDisassociateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtPhdcAgDisassociateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtPhdcAgDisassociateCfm *) CsrPmemZalloc(sizeof(CsrBtPhdcAgDisassociateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataChannelId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PHDC_AG_DISASSOCIATE_CFM */

#endif /* EXCLUDE_CSR_BT_PHDC_AG_MODULE */
