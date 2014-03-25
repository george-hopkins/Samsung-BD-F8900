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
#include "csr_bt_bips_prim.h"

#ifndef EXCLUDE_CSR_BT_BIPS_MODULE

void CsrBtBipsPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_RES
CsrSize CsrBtBipsPushGetCapabilitiesHeaderResSizeof(void *msg)
{
    CsrBtBipsPushGetCapabilitiesHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_RES
CsrUint8 *CsrBtBipsPushGetCapabilitiesHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsPushGetCapabilitiesHeaderRes *primitive;

    primitive = (CsrBtBipsPushGetCapabilitiesHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_RES
void *CsrBtBipsPushGetCapabilitiesHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsPushGetCapabilitiesHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsPushGetCapabilitiesHeaderRes *) CsrPmemZalloc(sizeof(CsrBtBipsPushGetCapabilitiesHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES
CsrSize CsrBtBipsRcGetMonitoringImageObjectResSizeof(void *msg)
{
    CsrBtBipsRcGetMonitoringImageObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->monitoringObjectLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES
CsrUint8 *CsrBtBipsRcGetMonitoringImageObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRcGetMonitoringImageObjectRes *primitive;

    primitive = (CsrBtBipsRcGetMonitoringImageObjectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->monitoringObjectLength);
    CsrMemCpySer(buffer, length, &primitive->monitoringObject, sizeof(primitive->monitoringObject));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->monitoringObject)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->monitoringObject), ((CsrSize) (primitive->monitoringObjectLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES
void *CsrBtBipsRcGetMonitoringImageObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRcGetMonitoringImageObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRcGetMonitoringImageObjectRes *) CsrPmemZalloc(sizeof(CsrBtBipsRcGetMonitoringImageObjectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->monitoringObjectLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->monitoringObject), buffer, &offset, ((CsrSize) sizeof(primitive->monitoringObject)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->monitoringObjectLength)
    {
        primitive->monitoringObject = CsrPmemZalloc(((CsrUint32) (primitive->monitoringObjectLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->monitoringObject), buffer, &offset, ((CsrSize) (primitive->monitoringObjectLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->monitoringObject = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES
void CsrBtBipsRcGetMonitoringImageObjectResSerFree(void *msg)
{
    CsrBtBipsRcGetMonitoringImageObjectRes *primitive;

    primitive = (CsrBtBipsRcGetMonitoringImageObjectRes *) msg;

    if(primitive->monitoringObject != NULL)
    {
        CsrPmemFree(primitive->monitoringObject);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_REQ
CsrSize CsrBtBipsAaGetCapabilitiesReqSizeof(void *msg)
{
    CsrBtBipsAaGetCapabilitiesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_REQ
CsrUint8 *CsrBtBipsAaGetCapabilitiesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetCapabilitiesReq *primitive;

    primitive = (CsrBtBipsAaGetCapabilitiesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_REQ
void *CsrBtBipsAaGetCapabilitiesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetCapabilitiesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetCapabilitiesReq *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetCapabilitiesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ
CsrSize CsrBtBipsAaGetLinkedAttachmentReqSizeof(void *msg)
{
    CsrBtBipsAaGetLinkedAttachmentReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->fileName) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ
CsrUint8 *CsrBtBipsAaGetLinkedAttachmentReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetLinkedAttachmentReq *primitive;

    primitive = (CsrBtBipsAaGetLinkedAttachmentReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->imageHandle), ((CsrSize) (8)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->fileNameLength);
    CsrMemCpySer(buffer, length, &primitive->fileName, sizeof(primitive->fileName));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if (primitive->fileName)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->fileName), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->fileName) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->fileName) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ
void *CsrBtBipsAaGetLinkedAttachmentReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetLinkedAttachmentReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetLinkedAttachmentReq *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetLinkedAttachmentReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->imageHandle), buffer, &offset, ((CsrSize) (8)));
    CsrUint16Des((CsrUint16 *) &primitive->fileNameLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->fileName), buffer, &offset, ((CsrSize) sizeof(primitive->fileName)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    primitive->fileName = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->fileName), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->fileName) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ
void CsrBtBipsAaGetLinkedAttachmentReqSerFree(void *msg)
{
    CsrBtBipsAaGetLinkedAttachmentReq *primitive;

    primitive = (CsrBtBipsAaGetLinkedAttachmentReq *) msg;

    if(primitive->fileName != NULL)
    {
        CsrPmemFree(primitive->fileName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM
CsrSize CsrBtBipsAaGetLinkedThumbnailCfmSizeof(void *msg)
{
    CsrBtBipsAaGetLinkedThumbnailCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM
CsrUint8 *CsrBtBipsAaGetLinkedThumbnailCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetLinkedThumbnailCfm *primitive;

    primitive = (CsrBtBipsAaGetLinkedThumbnailCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->thumbnailObjectLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->thumbnailObjectOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM
void *CsrBtBipsAaGetLinkedThumbnailCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetLinkedThumbnailCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetLinkedThumbnailCfm *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetLinkedThumbnailCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->thumbnailObjectLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->thumbnailObjectOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM
void CsrBtBipsAaGetLinkedThumbnailCfmSerFree(void *msg)
{
    CsrBtBipsAaGetLinkedThumbnailCfm *primitive;

    primitive = (CsrBtBipsAaGetLinkedThumbnailCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_ABORT_CFM
CsrSize CsrBtBipsAaAbortCfmSizeof(void *msg)
{
    CsrBtBipsAaAbortCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_ABORT_CFM
CsrUint8 *CsrBtBipsAaAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaAbortCfm *primitive;

    primitive = (CsrBtBipsAaAbortCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_ABORT_CFM
void *CsrBtBipsAaAbortCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaAbortCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaAbortCfm *) CsrPmemZalloc(sizeof(CsrBtBipsAaAbortCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_RES
CsrSize CsrBtBipsAaGetCapabilitiesResSizeof(void *msg)
{
    CsrBtBipsAaGetCapabilitiesRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_RES
CsrUint8 *CsrBtBipsAaGetCapabilitiesResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetCapabilitiesRes *primitive;

    primitive = (CsrBtBipsAaGetCapabilitiesRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_RES
void *CsrBtBipsAaGetCapabilitiesResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetCapabilitiesRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetCapabilitiesRes *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetCapabilitiesRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_RES
CsrSize CsrBtBipsRcGetLinkedThumbnailHeaderResSizeof(void *msg)
{
    CsrBtBipsRcGetLinkedThumbnailHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_RES
CsrUint8 *CsrBtBipsRcGetLinkedThumbnailHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRcGetLinkedThumbnailHeaderRes *primitive;

    primitive = (CsrBtBipsRcGetLinkedThumbnailHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_RES
void *CsrBtBipsRcGetLinkedThumbnailHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRcGetLinkedThumbnailHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRcGetLinkedThumbnailHeaderRes *) CsrPmemZalloc(sizeof(CsrBtBipsRcGetLinkedThumbnailHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM
CsrSize CsrBtBipsAaGetImagePropertiesCfmSizeof(void *msg)
{
    CsrBtBipsAaGetImagePropertiesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM
CsrUint8 *CsrBtBipsAaGetImagePropertiesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetImagePropertiesCfm *primitive;

    primitive = (CsrBtBipsAaGetImagePropertiesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->propertiesObjectLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->propertiesObjectOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM
void *CsrBtBipsAaGetImagePropertiesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetImagePropertiesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetImagePropertiesCfm *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetImagePropertiesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->propertiesObjectLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->propertiesObjectOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM
void CsrBtBipsAaGetImagePropertiesCfmSerFree(void *msg)
{
    CsrBtBipsAaGetImagePropertiesCfm *primitive;

    primitive = (CsrBtBipsAaGetImagePropertiesCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND
CsrSize CsrBtBipsPushPutImageHeaderIndSizeof(void *msg)
{
    CsrBtBipsPushPutImageHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND
CsrUint8 *CsrBtBipsPushPutImageHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsPushPutImageHeaderInd *primitive;

    primitive = (CsrBtBipsPushPutImageHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageDescriptorOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageDescriptorLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND
void *CsrBtBipsPushPutImageHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsPushPutImageHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsPushPutImageHeaderInd *) CsrPmemZalloc(sizeof(CsrBtBipsPushPutImageHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageDescriptorOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageDescriptorLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND
void CsrBtBipsPushPutImageHeaderIndSerFree(void *msg)
{
    CsrBtBipsPushPutImageHeaderInd *primitive;

    primitive = (CsrBtBipsPushPutImageHeaderInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND
CsrSize CsrBtBipsAaGetLinkedAttachmentIndSizeof(void *msg)
{
    CsrBtBipsAaGetLinkedAttachmentInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND
CsrUint8 *CsrBtBipsAaGetLinkedAttachmentIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetLinkedAttachmentInd *primitive;

    primitive = (CsrBtBipsAaGetLinkedAttachmentInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attachmentFileOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attachmentFileLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND
void *CsrBtBipsAaGetLinkedAttachmentIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetLinkedAttachmentInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetLinkedAttachmentInd *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetLinkedAttachmentInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attachmentFileOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attachmentFileLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND
void CsrBtBipsAaGetLinkedAttachmentIndSerFree(void *msg)
{
    CsrBtBipsAaGetLinkedAttachmentInd *primitive;

    primitive = (CsrBtBipsAaGetLinkedAttachmentInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_CONNECT_IND
CsrSize CsrBtBipsConnectIndSizeof(void *msg)
{
    CsrBtBipsConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_CONNECT_IND
CsrUint8 *CsrBtBipsConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsConnectInd *primitive;

    primitive = (CsrBtBipsConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexPeerMaxPacketSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->count);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->supportedFunctions);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_CONNECT_IND
void *CsrBtBipsConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsConnectInd *) CsrPmemZalloc(sizeof(CsrBtBipsConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexPeerMaxPacketSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->count, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->supportedFunctions, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_CFM
CsrSize CsrBtBipsGetInstancesQidCfmSizeof(void *msg)
{
    CsrBtBipsGetInstancesQidCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->phandlesListSize * sizeof(CsrSchedQid)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_CFM
CsrUint8 *CsrBtBipsGetInstancesQidCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsGetInstancesQidCfm *primitive;

    primitive = (CsrBtBipsGetInstancesQidCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->phandlesListSize);
    CsrMemCpySer(buffer, length, &primitive->phandlesList, sizeof(primitive->phandlesList));
    if(primitive->phandlesList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->phandlesList), ((CsrSize) (primitive->phandlesListSize * sizeof(CsrSchedQid))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_CFM
void *CsrBtBipsGetInstancesQidCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsGetInstancesQidCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsGetInstancesQidCfm *) CsrPmemZalloc(sizeof(CsrBtBipsGetInstancesQidCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->phandlesListSize, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->phandlesList), buffer, &offset, ((CsrSize) sizeof(primitive->phandlesList)));
    if (primitive->phandlesListSize)
    {
        primitive->phandlesList = CsrPmemZalloc(((CsrUint32) (primitive->phandlesListSize * sizeof(CsrSchedQid))));
        CsrMemCpyDes(((void *) primitive->phandlesList), buffer, &offset, ((CsrSize) (primitive->phandlesListSize * sizeof(CsrSchedQid))));
    }
    else
    {
        primitive->phandlesList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_CFM
void CsrBtBipsGetInstancesQidCfmSerFree(void *msg)
{
    CsrBtBipsGetInstancesQidCfm *primitive;

    primitive = (CsrBtBipsGetInstancesQidCfm *) msg;

    if(primitive->phandlesList != NULL)
    {
        CsrPmemFree(primitive->phandlesList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_REQ
CsrSize CsrBtBipsGetInstancesQidReqSizeof(void *msg)
{
    CsrBtBipsGetInstancesQidReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_REQ
CsrUint8 *CsrBtBipsGetInstancesQidReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsGetInstancesQidReq *primitive;

    primitive = (CsrBtBipsGetInstancesQidReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_REQ
void *CsrBtBipsGetInstancesQidReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsGetInstancesQidReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsGetInstancesQidReq *) CsrPmemZalloc(sizeof(CsrBtBipsGetInstancesQidReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_GET_INSTANCES_QID_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_ABORT_REQ
CsrSize CsrBtBipsAaAbortReqSizeof(void *msg)
{
    CsrBtBipsAaAbortReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_ABORT_REQ
CsrUint8 *CsrBtBipsAaAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaAbortReq *primitive;

    primitive = (CsrBtBipsAaAbortReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_ABORT_REQ
void *CsrBtBipsAaAbortReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaAbortReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaAbortReq *) CsrPmemZalloc(sizeof(CsrBtBipsAaAbortReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND
CsrSize CsrBtBipsRcGetImagePropertiesHeaderIndSizeof(void *msg)
{
    CsrBtBipsRcGetImagePropertiesHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND
CsrUint8 *CsrBtBipsRcGetImagePropertiesHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRcGetImagePropertiesHeaderInd *primitive;

    primitive = (CsrBtBipsRcGetImagePropertiesHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageHandleOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND
void *CsrBtBipsRcGetImagePropertiesHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRcGetImagePropertiesHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRcGetImagePropertiesHeaderInd *) CsrPmemZalloc(sizeof(CsrBtBipsRcGetImagePropertiesHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageHandleOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND
void CsrBtBipsRcGetImagePropertiesHeaderIndSerFree(void *msg)
{
    CsrBtBipsRcGetImagePropertiesHeaderInd *primitive;

    primitive = (CsrBtBipsRcGetImagePropertiesHeaderInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES
CsrSize CsrBtBipsRcGetImagePropertiesObjectResSizeof(void *msg)
{
    CsrBtBipsRcGetImagePropertiesObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->propertiesObjectLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES
CsrUint8 *CsrBtBipsRcGetImagePropertiesObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRcGetImagePropertiesObjectRes *primitive;

    primitive = (CsrBtBipsRcGetImagePropertiesObjectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->propertiesObjectLength);
    CsrMemCpySer(buffer, length, &primitive->propertiesObject, sizeof(primitive->propertiesObject));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->propertiesObject)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->propertiesObject), ((CsrSize) (primitive->propertiesObjectLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES
void *CsrBtBipsRcGetImagePropertiesObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRcGetImagePropertiesObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRcGetImagePropertiesObjectRes *) CsrPmemZalloc(sizeof(CsrBtBipsRcGetImagePropertiesObjectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->propertiesObjectLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->propertiesObject), buffer, &offset, ((CsrSize) sizeof(primitive->propertiesObject)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->propertiesObjectLength)
    {
        primitive->propertiesObject = CsrPmemZalloc(((CsrUint32) (primitive->propertiesObjectLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->propertiesObject), buffer, &offset, ((CsrSize) (primitive->propertiesObjectLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->propertiesObject = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES
void CsrBtBipsRcGetImagePropertiesObjectResSerFree(void *msg)
{
    CsrBtBipsRcGetImagePropertiesObjectRes *primitive;

    primitive = (CsrBtBipsRcGetImagePropertiesObjectRes *) msg;

    if(primitive->propertiesObject != NULL)
    {
        CsrPmemFree(primitive->propertiesObject);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_IND
CsrSize CsrBtBipsRcGetImageObjectIndSizeof(void *msg)
{
    CsrBtBipsRcGetImageObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_IND
CsrUint8 *CsrBtBipsRcGetImageObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRcGetImageObjectInd *primitive;

    primitive = (CsrBtBipsRcGetImageObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->allowedObjectLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_IND
void *CsrBtBipsRcGetImageObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRcGetImageObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRcGetImageObjectInd *) CsrPmemZalloc(sizeof(CsrBtBipsRcGetImageObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->allowedObjectLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_IND
CsrSize CsrBtBipsRcGetMonitoringImageHeaderIndSizeof(void *msg)
{
    CsrBtBipsRcGetMonitoringImageHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_IND
CsrUint8 *CsrBtBipsRcGetMonitoringImageHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRcGetMonitoringImageHeaderInd *primitive;

    primitive = (CsrBtBipsRcGetMonitoringImageHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->storeFlag);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_IND
void *CsrBtBipsRcGetMonitoringImageHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRcGetMonitoringImageHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRcGetMonitoringImageHeaderInd *) CsrPmemZalloc(sizeof(CsrBtBipsRcGetMonitoringImageHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->storeFlag, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_CHALLENGE_IND
CsrSize CsrBtBipsChallengeIndSizeof(void *msg)
{
    CsrBtBipsChallengeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_CHALLENGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_CHALLENGE_IND
CsrUint8 *CsrBtBipsChallengeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsChallengeInd *primitive;

    primitive = (CsrBtBipsChallengeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->challenged);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_CHALLENGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_CHALLENGE_IND
void *CsrBtBipsChallengeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsChallengeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsChallengeInd *) CsrPmemZalloc(sizeof(CsrBtBipsChallengeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->challenged, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_CHALLENGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND
CsrSize CsrBtBipsPushPutLinkedThumbnailHeaderIndSizeof(void *msg)
{
    CsrBtBipsPushPutLinkedThumbnailHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND
CsrUint8 *CsrBtBipsPushPutLinkedThumbnailHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsPushPutLinkedThumbnailHeaderInd *primitive;

    primitive = (CsrBtBipsPushPutLinkedThumbnailHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageHandleOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND
void *CsrBtBipsPushPutLinkedThumbnailHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsPushPutLinkedThumbnailHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsPushPutLinkedThumbnailHeaderInd *) CsrPmemZalloc(sizeof(CsrBtBipsPushPutLinkedThumbnailHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageHandleOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND
void CsrBtBipsPushPutLinkedThumbnailHeaderIndSerFree(void *msg)
{
    CsrBtBipsPushPutLinkedThumbnailHeaderInd *primitive;

    primitive = (CsrBtBipsPushPutLinkedThumbnailHeaderInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_RES
CsrSize CsrBtBipsRcGetMonitoringImageHeaderResSizeof(void *msg)
{
    CsrBtBipsRcGetMonitoringImageHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_RES
CsrUint8 *CsrBtBipsRcGetMonitoringImageHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRcGetMonitoringImageHeaderRes *primitive;

    primitive = (CsrBtBipsRcGetMonitoringImageHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrMemCpySer(buffer, length, ((const void *) primitive->imageHandle), ((CsrSize) (8)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_RES
void *CsrBtBipsRcGetMonitoringImageHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRcGetMonitoringImageHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRcGetMonitoringImageHeaderRes *) CsrPmemZalloc(sizeof(CsrBtBipsRcGetMonitoringImageHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->imageHandle), buffer, &offset, ((CsrSize) (8)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_RES
CsrSize CsrBtBipsAaGetImageListHeaderResSizeof(void *msg)
{
    CsrBtBipsAaGetImageListHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_RES
CsrUint8 *CsrBtBipsAaGetImageListHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetImageListHeaderRes *primitive;

    primitive = (CsrBtBipsAaGetImageListHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_RES
void *CsrBtBipsAaGetImageListHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetImageListHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetImageListHeaderRes *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetImageListHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_DISCONNECT_REQ
CsrSize CsrBtBipsDisconnectReqSizeof(void *msg)
{
    CsrBtBipsDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_DISCONNECT_REQ
CsrUint8 *CsrBtBipsDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsDisconnectReq *primitive;

    primitive = (CsrBtBipsDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->normalDisconnect);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_DISCONNECT_REQ
void *CsrBtBipsDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtBipsDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->normalDisconnect, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_CONNECT_RES
CsrSize CsrBtBipsConnectResSizeof(void *msg)
{
    CsrBtBipsConnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_CONNECT_RES
CsrUint8 *CsrBtBipsConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsConnectRes *primitive;

    primitive = (CsrBtBipsConnectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_CONNECT_RES
void *CsrBtBipsConnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsConnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsConnectRes *) CsrPmemZalloc(sizeof(CsrBtBipsConnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND
CsrSize CsrBtBipsAaGetImageListHeaderIndSizeof(void *msg)
{
    CsrBtBipsAaGetImageListHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->imageHandlesDescriptorLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND
CsrUint8 *CsrBtBipsAaGetImageListHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetImageListHeaderInd *primitive;

    primitive = (CsrBtBipsAaGetImageListHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->nbReturnedHandles);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageHandlesDescriptorLength);
    CsrMemCpySer(buffer, length, &primitive->imageHandlesDescriptor, sizeof(primitive->imageHandlesDescriptor));
    if(primitive->imageHandlesDescriptor)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->imageHandlesDescriptor), ((CsrSize) (primitive->imageHandlesDescriptorLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND
void *CsrBtBipsAaGetImageListHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetImageListHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetImageListHeaderInd *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetImageListHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->nbReturnedHandles, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageHandlesDescriptorLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->imageHandlesDescriptor), buffer, &offset, ((CsrSize) sizeof(primitive->imageHandlesDescriptor)));
    if (primitive->imageHandlesDescriptorLength)
    {
        primitive->imageHandlesDescriptor = CsrPmemZalloc(((CsrUint32) (primitive->imageHandlesDescriptorLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->imageHandlesDescriptor), buffer, &offset, ((CsrSize) (primitive->imageHandlesDescriptorLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->imageHandlesDescriptor = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND
void CsrBtBipsAaGetImageListHeaderIndSerFree(void *msg)
{
    CsrBtBipsAaGetImageListHeaderInd *primitive;

    primitive = (CsrBtBipsAaGetImageListHeaderInd *) msg;

    if(primitive->imageHandlesDescriptor != NULL)
    {
        CsrPmemFree(primitive->imageHandlesDescriptor);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_IND
CsrSize CsrBtBipsRcGetMonitoringImageObjectIndSizeof(void *msg)
{
    CsrBtBipsRcGetMonitoringImageObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_IND
CsrUint8 *CsrBtBipsRcGetMonitoringImageObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRcGetMonitoringImageObjectInd *primitive;

    primitive = (CsrBtBipsRcGetMonitoringImageObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->allowedImageLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_IND
void *CsrBtBipsRcGetMonitoringImageObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRcGetMonitoringImageObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRcGetMonitoringImageObjectInd *) CsrPmemZalloc(sizeof(CsrBtBipsRcGetMonitoringImageObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->allowedImageLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_MONITORING_IMAGE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_ACTIVATE_REQ
CsrSize CsrBtBipsActivateReqSizeof(void *msg)
{
    CsrBtBipsActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_ACTIVATE_REQ
CsrUint8 *CsrBtBipsActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsActivateReq *primitive;

    primitive = (CsrBtBipsActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->upperDataCapacity);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lowerDataCapacity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->featureSelection);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexMaxPacketSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->digestChallenge);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->windowSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmEnable);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_ACTIVATE_REQ
void *CsrBtBipsActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsActivateReq *) CsrPmemZalloc(sizeof(CsrBtBipsActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->upperDataCapacity, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->lowerDataCapacity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->featureSelection, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexMaxPacketSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->digestChallenge, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->windowSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmEnable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_RES
CsrSize CsrBtBipsAaGetImagePropertiesResSizeof(void *msg)
{
    CsrBtBipsAaGetImagePropertiesRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_RES
CsrUint8 *CsrBtBipsAaGetImagePropertiesResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetImagePropertiesRes *primitive;

    primitive = (CsrBtBipsAaGetImagePropertiesRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_RES
void *CsrBtBipsAaGetImagePropertiesResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetImagePropertiesRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetImagePropertiesRes *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetImagePropertiesRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_IND
CsrSize CsrBtBipsRcGetLinkedThumbnailObjectIndSizeof(void *msg)
{
    CsrBtBipsRcGetLinkedThumbnailObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_IND
CsrUint8 *CsrBtBipsRcGetLinkedThumbnailObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRcGetLinkedThumbnailObjectInd *primitive;

    primitive = (CsrBtBipsRcGetLinkedThumbnailObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->allowedObjectLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_IND
void *CsrBtBipsRcGetLinkedThumbnailObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRcGetLinkedThumbnailObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRcGetLinkedThumbnailObjectInd *) CsrPmemZalloc(sizeof(CsrBtBipsRcGetLinkedThumbnailObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->allowedObjectLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND
CsrSize CsrBtBipsRcGetImageHeaderIndSizeof(void *msg)
{
    CsrBtBipsRcGetImageHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND
CsrUint8 *CsrBtBipsRcGetImageHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRcGetImageHeaderInd *primitive;

    primitive = (CsrBtBipsRcGetImageHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageHandleOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->descriptorLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->descriptorOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND
void *CsrBtBipsRcGetImageHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRcGetImageHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRcGetImageHeaderInd *) CsrPmemZalloc(sizeof(CsrBtBipsRcGetImageHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageHandleOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->descriptorLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->descriptorOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND
void CsrBtBipsRcGetImageHeaderIndSerFree(void *msg)
{
    CsrBtBipsRcGetImageHeaderInd *primitive;

    primitive = (CsrBtBipsRcGetImageHeaderInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_IND
CsrSize CsrBtBipsPushGetCapabilitiesHeaderIndSizeof(void *msg)
{
    CsrBtBipsPushGetCapabilitiesHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_IND
CsrUint8 *CsrBtBipsPushGetCapabilitiesHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsPushGetCapabilitiesHeaderInd *primitive;

    primitive = (CsrBtBipsPushGetCapabilitiesHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_IND
void *CsrBtBipsPushGetCapabilitiesHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsPushGetCapabilitiesHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsPushGetCapabilitiesHeaderInd *) CsrPmemZalloc(sizeof(CsrBtBipsPushGetCapabilitiesHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_IND
CsrSize CsrBtBipsRcGetImagePropertiesObjectIndSizeof(void *msg)
{
    CsrBtBipsRcGetImagePropertiesObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_IND
CsrUint8 *CsrBtBipsRcGetImagePropertiesObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRcGetImagePropertiesObjectInd *primitive;

    primitive = (CsrBtBipsRcGetImagePropertiesObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->allowedImageLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_IND
void *CsrBtBipsRcGetImagePropertiesObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRcGetImagePropertiesObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRcGetImagePropertiesObjectInd *) CsrPmemZalloc(sizeof(CsrBtBipsRcGetImagePropertiesObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->allowedImageLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_CHALLENGE_RES
CsrSize CsrBtBipsChallengeResSizeof(void *msg)
{
    CsrBtBipsChallengeRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_CHALLENGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_CHALLENGE_RES
CsrUint8 *CsrBtBipsChallengeResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsChallengeRes *primitive;

    primitive = (CsrBtBipsChallengeRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->realmLength);
    CsrMemCpySer(buffer, length, &primitive->realm, sizeof(primitive->realm));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->passwordLength);
    CsrMemCpySer(buffer, length, &primitive->password, sizeof(primitive->password));
    CsrMemCpySer(buffer, length, &primitive->userId, sizeof(primitive->userId));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authenticateResponse);
    if(primitive->realm)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->realm), ((CsrSize) (primitive->realmLength * sizeof(CsrUint8))));
    }
    if(primitive->password)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->password), ((CsrSize) (primitive->passwordLength * sizeof(CsrUint8))));
    }
    CsrCharStringSer(buffer, length, primitive->userId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_CHALLENGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_CHALLENGE_RES
void *CsrBtBipsChallengeResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsChallengeRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsChallengeRes *) CsrPmemZalloc(sizeof(CsrBtBipsChallengeRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->realmLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->realm), buffer, &offset, ((CsrSize) sizeof(primitive->realm)));
    CsrUint16Des((CsrUint16 *) &primitive->passwordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->password), buffer, &offset, ((CsrSize) sizeof(primitive->password)));
    CsrMemCpyDes(((void *) &primitive->userId), buffer, &offset, ((CsrSize) sizeof(primitive->userId)));
    CsrUint8Des((CsrUint8 *) &primitive->authenticateResponse, buffer, &offset);
    if (primitive->realmLength)
    {
        primitive->realm = CsrPmemZalloc(((CsrUint32) (primitive->realmLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->realm), buffer, &offset, ((CsrSize) (primitive->realmLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->realm = NULL;
    }
    if (primitive->passwordLength)
    {
        primitive->password = CsrPmemZalloc(((CsrUint32) (primitive->passwordLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->password), buffer, &offset, ((CsrSize) (primitive->passwordLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->password = NULL;
    }
    CsrCharStringDes(&primitive->userId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_CHALLENGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_CHALLENGE_RES
void CsrBtBipsChallengeResSerFree(void *msg)
{
    CsrBtBipsChallengeRes *primitive;

    primitive = (CsrBtBipsChallengeRes *) msg;

    if(primitive->realm != NULL)
    {
        CsrPmemFree(primitive->realm);
    }
    if(primitive->password != NULL)
    {
        CsrPmemFree(primitive->password);
    }
    if(primitive->userId != NULL)
    {
        CsrPmemFree(primitive->userId);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_CHALLENGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM
CsrSize CsrBtBipsAaGetLinkedAttachmentCfmSizeof(void *msg)
{
    CsrBtBipsAaGetLinkedAttachmentCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM
CsrUint8 *CsrBtBipsAaGetLinkedAttachmentCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetLinkedAttachmentCfm *primitive;

    primitive = (CsrBtBipsAaGetLinkedAttachmentCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attachmentFileOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attachmentFileLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM
void *CsrBtBipsAaGetLinkedAttachmentCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetLinkedAttachmentCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetLinkedAttachmentCfm *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetLinkedAttachmentCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attachmentFileOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attachmentFileLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM
void CsrBtBipsAaGetLinkedAttachmentCfmSerFree(void *msg)
{
    CsrBtBipsAaGetLinkedAttachmentCfm *primitive;

    primitive = (CsrBtBipsAaGetLinkedAttachmentCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM
CsrSize CsrBtBipsAaGetCapabilitiesCfmSizeof(void *msg)
{
    CsrBtBipsAaGetCapabilitiesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM
CsrUint8 *CsrBtBipsAaGetCapabilitiesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetCapabilitiesCfm *primitive;

    primitive = (CsrBtBipsAaGetCapabilitiesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->capabilitiesObjectOffset);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->capabilitiesObjectLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM
void *CsrBtBipsAaGetCapabilitiesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetCapabilitiesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetCapabilitiesCfm *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetCapabilitiesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->capabilitiesObjectOffset, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->capabilitiesObjectLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint32Des((CsrUint32 *) &primitive->payloadLength, buffer, &offset);
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM
void CsrBtBipsAaGetCapabilitiesCfmSerFree(void *msg)
{
    CsrBtBipsAaGetCapabilitiesCfm *primitive;

    primitive = (CsrBtBipsAaGetCapabilitiesCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND
CsrSize CsrBtBipsAaGetImageListIndSizeof(void *msg)
{
    CsrBtBipsAaGetImageListInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND
CsrUint8 *CsrBtBipsAaGetImageListIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetImageListInd *primitive;

    primitive = (CsrBtBipsAaGetImageListInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageListingObjectOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageListingObjectLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND
void *CsrBtBipsAaGetImageListIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetImageListInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetImageListInd *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetImageListInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageListingObjectOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageListingObjectLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND
void CsrBtBipsAaGetImageListIndSerFree(void *msg)
{
    CsrBtBipsAaGetImageListInd *primitive;

    primitive = (CsrBtBipsAaGetImageListInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_RES
CsrSize CsrBtBipsPushPutImageHeaderResSizeof(void *msg)
{
    CsrBtBipsPushPutImageHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_RES
CsrUint8 *CsrBtBipsPushPutImageHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsPushPutImageHeaderRes *primitive;

    primitive = (CsrBtBipsPushPutImageHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrMemCpySer(buffer, length, ((const void *) primitive->imageHandle), ((CsrSize) (8)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_RES
void *CsrBtBipsPushPutImageHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsPushPutImageHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsPushPutImageHeaderRes *) CsrPmemZalloc(sizeof(CsrBtBipsPushPutImageHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->imageHandle), buffer, &offset, ((CsrSize) (8)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_RES
CsrSize CsrBtBipsPushPutLinkedThumbnailHeaderResSizeof(void *msg)
{
    CsrBtBipsPushPutLinkedThumbnailHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_RES
CsrUint8 *CsrBtBipsPushPutLinkedThumbnailHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsPushPutLinkedThumbnailHeaderRes *primitive;

    primitive = (CsrBtBipsPushPutLinkedThumbnailHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_RES
void *CsrBtBipsPushPutLinkedThumbnailHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsPushPutLinkedThumbnailHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsPushPutLinkedThumbnailHeaderRes *) CsrPmemZalloc(sizeof(CsrBtBipsPushPutLinkedThumbnailHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_RES
CsrSize CsrBtBipsPushPutLinkedAttachmentFileResSizeof(void *msg)
{
    CsrBtBipsPushPutLinkedAttachmentFileRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_RES
CsrUint8 *CsrBtBipsPushPutLinkedAttachmentFileResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsPushPutLinkedAttachmentFileRes *primitive;

    primitive = (CsrBtBipsPushPutLinkedAttachmentFileRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_RES
void *CsrBtBipsPushPutLinkedAttachmentFileResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsPushPutLinkedAttachmentFileRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsPushPutLinkedAttachmentFileRes *) CsrPmemZalloc(sizeof(CsrBtBipsPushPutLinkedAttachmentFileRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_REQ
CsrSize CsrBtBipsAaDeleteImageReqSizeof(void *msg)
{
    CsrBtBipsAaDeleteImageReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_REQ
CsrUint8 *CsrBtBipsAaDeleteImageReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaDeleteImageReq *primitive;

    primitive = (CsrBtBipsAaDeleteImageReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->imageHandle), ((CsrSize) (8)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_REQ
void *CsrBtBipsAaDeleteImageReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaDeleteImageReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaDeleteImageReq *) CsrPmemZalloc(sizeof(CsrBtBipsAaDeleteImageReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->imageHandle), buffer, &offset, ((CsrSize) (8)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_RES
CsrSize CsrBtBipsPushPutImageFileResSizeof(void *msg)
{
    CsrBtBipsPushPutImageFileRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_RES
CsrUint8 *CsrBtBipsPushPutImageFileResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsPushPutImageFileRes *primitive;

    primitive = (CsrBtBipsPushPutImageFileRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_RES
void *CsrBtBipsPushPutImageFileResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsPushPutImageFileRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsPushPutImageFileRes *) CsrPmemZalloc(sizeof(CsrBtBipsPushPutImageFileRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND
CsrSize CsrBtBipsPushPutLinkedThumbnailFileIndSizeof(void *msg)
{
    CsrBtBipsPushPutLinkedThumbnailFileInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND
CsrUint8 *CsrBtBipsPushPutLinkedThumbnailFileIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsPushPutLinkedThumbnailFileInd *primitive;

    primitive = (CsrBtBipsPushPutLinkedThumbnailFileInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->thumbnailFileOffset);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->thumbnailFileLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND
void *CsrBtBipsPushPutLinkedThumbnailFileIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsPushPutLinkedThumbnailFileInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsPushPutLinkedThumbnailFileInd *) CsrPmemZalloc(sizeof(CsrBtBipsPushPutLinkedThumbnailFileInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->thumbnailFileOffset, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->thumbnailFileLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND
void CsrBtBipsPushPutLinkedThumbnailFileIndSerFree(void *msg)
{
    CsrBtBipsPushPutLinkedThumbnailFileInd *primitive;

    primitive = (CsrBtBipsPushPutLinkedThumbnailFileInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_CFM
CsrSize CsrBtBipsAaGetImageCfmSizeof(void *msg)
{
    CsrBtBipsAaGetImageCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_CFM
CsrUint8 *CsrBtBipsAaGetImageCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetImageCfm *primitive;

    primitive = (CsrBtBipsAaGetImageCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageObjectLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageObjectOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_CFM
void *CsrBtBipsAaGetImageCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetImageCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetImageCfm *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetImageCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageObjectLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageObjectOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_CFM
void CsrBtBipsAaGetImageCfmSerFree(void *msg)
{
    CsrBtBipsAaGetImageCfm *primitive;

    primitive = (CsrBtBipsAaGetImageCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND
CsrSize CsrBtBipsPushPutLinkedAttachmentHeaderIndSizeof(void *msg)
{
    CsrBtBipsPushPutLinkedAttachmentHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND
CsrUint8 *CsrBtBipsPushPutLinkedAttachmentHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsPushPutLinkedAttachmentHeaderInd *primitive;

    primitive = (CsrBtBipsPushPutLinkedAttachmentHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageHandleOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attachmentDescriptorOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attachmentDescriptorLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND
void *CsrBtBipsPushPutLinkedAttachmentHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsPushPutLinkedAttachmentHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsPushPutLinkedAttachmentHeaderInd *) CsrPmemZalloc(sizeof(CsrBtBipsPushPutLinkedAttachmentHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageHandleOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attachmentDescriptorOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attachmentDescriptorLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND
void CsrBtBipsPushPutLinkedAttachmentHeaderIndSerFree(void *msg)
{
    CsrBtBipsPushPutLinkedAttachmentHeaderInd *primitive;

    primitive = (CsrBtBipsPushPutLinkedAttachmentHeaderInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_RES
CsrSize CsrBtBipsAaGetLinkedThumbnailResSizeof(void *msg)
{
    CsrBtBipsAaGetLinkedThumbnailRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_RES
CsrUint8 *CsrBtBipsAaGetLinkedThumbnailResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetLinkedThumbnailRes *primitive;

    primitive = (CsrBtBipsAaGetLinkedThumbnailRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_RES
void *CsrBtBipsAaGetLinkedThumbnailResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetLinkedThumbnailRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetLinkedThumbnailRes *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetLinkedThumbnailRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_DEACTIVATE_REQ
CsrSize CsrBtBipsDeactivateReqSizeof(void *msg)
{
    CsrBtBipsDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_DEACTIVATE_REQ
CsrUint8 *CsrBtBipsDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsDeactivateReq *primitive;

    primitive = (CsrBtBipsDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_DEACTIVATE_REQ
void *CsrBtBipsDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtBipsDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM
CsrSize CsrBtBipsAaGetImageListCfmSizeof(void *msg)
{
    CsrBtBipsAaGetImageListCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM
CsrUint8 *CsrBtBipsAaGetImageListCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetImageListCfm *primitive;

    primitive = (CsrBtBipsAaGetImageListCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageListingObjectOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageListingObjectLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM
void *CsrBtBipsAaGetImageListCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetImageListCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetImageListCfm *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetImageListCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageListingObjectOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageListingObjectLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM
void CsrBtBipsAaGetImageListCfmSerFree(void *msg)
{
    CsrBtBipsAaGetImageListCfm *primitive;

    primitive = (CsrBtBipsAaGetImageListCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_RES
CsrSize CsrBtBipsAuthenticateResSizeof(void *msg)
{
    CsrBtBipsAuthenticateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_RES
CsrUint8 *CsrBtBipsAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAuthenticateRes *primitive;

    primitive = (CsrBtBipsAuthenticateRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->password, sizeof(primitive->password));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->passwordLength);
    CsrMemCpySer(buffer, length, &primitive->userId, sizeof(primitive->userId));
    if(primitive->password)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->password), ((CsrSize) (primitive->passwordLength * sizeof(CsrUint8))));
    }
    CsrCharStringSer(buffer, length, primitive->userId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_RES
void *CsrBtBipsAuthenticateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAuthenticateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAuthenticateRes *) CsrPmemZalloc(sizeof(CsrBtBipsAuthenticateRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->password), buffer, &offset, ((CsrSize) sizeof(primitive->password)));
    CsrUint16Des((CsrUint16 *) &primitive->passwordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->userId), buffer, &offset, ((CsrSize) sizeof(primitive->userId)));
    if (primitive->passwordLength)
    {
        primitive->password = CsrPmemZalloc(((CsrUint32) (primitive->passwordLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->password), buffer, &offset, ((CsrSize) (primitive->passwordLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->password = NULL;
    }
    CsrCharStringDes(&primitive->userId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_RES
void CsrBtBipsAuthenticateResSerFree(void *msg)
{
    CsrBtBipsAuthenticateRes *primitive;

    primitive = (CsrBtBipsAuthenticateRes *) msg;

    if(primitive->password != NULL)
    {
        CsrPmemFree(primitive->password);
    }
    if(primitive->userId != NULL)
    {
        CsrPmemFree(primitive->userId);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_RES
CsrSize CsrBtBipsAaGetLinkedAttachmentResSizeof(void *msg)
{
    CsrBtBipsAaGetLinkedAttachmentRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_RES
CsrUint8 *CsrBtBipsAaGetLinkedAttachmentResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetLinkedAttachmentRes *primitive;

    primitive = (CsrBtBipsAaGetLinkedAttachmentRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_RES
void *CsrBtBipsAaGetLinkedAttachmentResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetLinkedAttachmentRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetLinkedAttachmentRes *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetLinkedAttachmentRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_ATTACHMENT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_RES
CsrSize CsrBtBipsRcGetImageHeaderResSizeof(void *msg)
{
    CsrBtBipsRcGetImageHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_RES
CsrUint8 *CsrBtBipsRcGetImageHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRcGetImageHeaderRes *primitive;

    primitive = (CsrBtBipsRcGetImageHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->imageTotalLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_RES
void *CsrBtBipsRcGetImageHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRcGetImageHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRcGetImageHeaderRes *) CsrPmemZalloc(sizeof(CsrBtBipsRcGetImageHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->imageTotalLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_IND
CsrSize CsrBtBipsAaGetImageIndSizeof(void *msg)
{
    CsrBtBipsAaGetImageInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_IND
CsrUint8 *CsrBtBipsAaGetImageIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetImageInd *primitive;

    primitive = (CsrBtBipsAaGetImageInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageObjectLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageObjectOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_IND
void *CsrBtBipsAaGetImageIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetImageInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetImageInd *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetImageInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageObjectLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageObjectOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_IND
void CsrBtBipsAaGetImageIndSerFree(void *msg)
{
    CsrBtBipsAaGetImageInd *primitive;

    primitive = (CsrBtBipsAaGetImageInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_RES
CsrSize CsrBtBipsRcGetImagePropertiesHeaderResSizeof(void *msg)
{
    CsrBtBipsRcGetImagePropertiesHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_RES
CsrUint8 *CsrBtBipsRcGetImagePropertiesHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRcGetImagePropertiesHeaderRes *primitive;

    primitive = (CsrBtBipsRcGetImagePropertiesHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_RES
void *CsrBtBipsRcGetImagePropertiesHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRcGetImagePropertiesHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRcGetImagePropertiesHeaderRes *) CsrPmemZalloc(sizeof(CsrBtBipsRcGetImagePropertiesHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_PROPERTIES_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_REQ
CsrSize CsrBtBipsAaGetImageReqSizeof(void *msg)
{
    CsrBtBipsAaGetImageReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->imageDescriptorLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_REQ
CsrUint8 *CsrBtBipsAaGetImageReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetImageReq *primitive;

    primitive = (CsrBtBipsAaGetImageReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->imageHandle), ((CsrSize) (8)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageDescriptorLength);
    CsrMemCpySer(buffer, length, &primitive->imageDescriptor, sizeof(primitive->imageDescriptor));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->imageDescriptor)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->imageDescriptor), ((CsrSize) (primitive->imageDescriptorLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_REQ
void *CsrBtBipsAaGetImageReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetImageReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetImageReq *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetImageReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->imageHandle), buffer, &offset, ((CsrSize) (8)));
    CsrUint16Des((CsrUint16 *) &primitive->imageDescriptorLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->imageDescriptor), buffer, &offset, ((CsrSize) sizeof(primitive->imageDescriptor)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->imageDescriptorLength)
    {
        primitive->imageDescriptor = CsrPmemZalloc(((CsrUint32) (primitive->imageDescriptorLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->imageDescriptor), buffer, &offset, ((CsrSize) (primitive->imageDescriptorLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->imageDescriptor = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_REQ
void CsrBtBipsAaGetImageReqSerFree(void *msg)
{
    CsrBtBipsAaGetImageReq *primitive;

    primitive = (CsrBtBipsAaGetImageReq *) msg;

    if(primitive->imageDescriptor != NULL)
    {
        CsrPmemFree(primitive->imageDescriptor);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_RES
CsrSize CsrBtBipsAaGetImageResSizeof(void *msg)
{
    CsrBtBipsAaGetImageRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_RES
CsrUint8 *CsrBtBipsAaGetImageResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetImageRes *primitive;

    primitive = (CsrBtBipsAaGetImageRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_RES
void *CsrBtBipsAaGetImageResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetImageRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetImageRes *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetImageRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND
CsrSize CsrBtBipsRcGetLinkedThumbnailHeaderIndSizeof(void *msg)
{
    CsrBtBipsRcGetLinkedThumbnailHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND
CsrUint8 *CsrBtBipsRcGetLinkedThumbnailHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRcGetLinkedThumbnailHeaderInd *primitive;

    primitive = (CsrBtBipsRcGetLinkedThumbnailHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageHandleOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND
void *CsrBtBipsRcGetLinkedThumbnailHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRcGetLinkedThumbnailHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRcGetLinkedThumbnailHeaderInd *) CsrPmemZalloc(sizeof(CsrBtBipsRcGetLinkedThumbnailHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageHandleOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND
void CsrBtBipsRcGetLinkedThumbnailHeaderIndSerFree(void *msg)
{
    CsrBtBipsRcGetLinkedThumbnailHeaderInd *primitive;

    primitive = (CsrBtBipsRcGetLinkedThumbnailHeaderInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES
CsrSize CsrBtBipsRcGetImageObjectResSizeof(void *msg)
{
    CsrBtBipsRcGetImageObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->imageObjectLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES
CsrUint8 *CsrBtBipsRcGetImageObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRcGetImageObjectRes *primitive;

    primitive = (CsrBtBipsRcGetImageObjectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageObjectLength);
    CsrMemCpySer(buffer, length, &primitive->imageObject, sizeof(primitive->imageObject));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->imageObject)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->imageObject), ((CsrSize) (primitive->imageObjectLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES
void *CsrBtBipsRcGetImageObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRcGetImageObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRcGetImageObjectRes *) CsrPmemZalloc(sizeof(CsrBtBipsRcGetImageObjectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageObjectLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->imageObject), buffer, &offset, ((CsrSize) sizeof(primitive->imageObject)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->imageObjectLength)
    {
        primitive->imageObject = CsrPmemZalloc(((CsrUint32) (primitive->imageObjectLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->imageObject), buffer, &offset, ((CsrSize) (primitive->imageObjectLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->imageObject = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES
void CsrBtBipsRcGetImageObjectResSerFree(void *msg)
{
    CsrBtBipsRcGetImageObjectRes *primitive;

    primitive = (CsrBtBipsRcGetImageObjectRes *) msg;

    if(primitive->imageObject != NULL)
    {
        CsrPmemFree(primitive->imageObject);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_IMAGE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_RES
CsrSize CsrBtBipsPushPutLinkedAttachmentHeaderResSizeof(void *msg)
{
    CsrBtBipsPushPutLinkedAttachmentHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_RES
CsrUint8 *CsrBtBipsPushPutLinkedAttachmentHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsPushPutLinkedAttachmentHeaderRes *primitive;

    primitive = (CsrBtBipsPushPutLinkedAttachmentHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_RES
void *CsrBtBipsPushPutLinkedAttachmentHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsPushPutLinkedAttachmentHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsPushPutLinkedAttachmentHeaderRes *) CsrPmemZalloc(sizeof(CsrBtBipsPushPutLinkedAttachmentHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_IND
CsrSize CsrBtBipsAaGetCapabilitiesIndSizeof(void *msg)
{
    CsrBtBipsAaGetCapabilitiesInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_IND
CsrUint8 *CsrBtBipsAaGetCapabilitiesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetCapabilitiesInd *primitive;

    primitive = (CsrBtBipsAaGetCapabilitiesInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->capabilitiesObjectOffset);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->capabilitiesObjectLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_IND
void *CsrBtBipsAaGetCapabilitiesIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetCapabilitiesInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetCapabilitiesInd *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetCapabilitiesInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->capabilitiesObjectOffset, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->capabilitiesObjectLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint32Des((CsrUint32 *) &primitive->payloadLength, buffer, &offset);
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_IND
void CsrBtBipsAaGetCapabilitiesIndSerFree(void *msg)
{
    CsrBtBipsAaGetCapabilitiesInd *primitive;

    primitive = (CsrBtBipsAaGetCapabilitiesInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_ABORT_IND
CsrSize CsrBtBipsAbortIndSizeof(void *msg)
{
    CsrBtBipsAbortInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_ABORT_IND
CsrUint8 *CsrBtBipsAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAbortInd *primitive;

    primitive = (CsrBtBipsAbortInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_ABORT_IND
void *CsrBtBipsAbortIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAbortInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAbortInd *) CsrPmemZalloc(sizeof(CsrBtBipsAbortInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND
CsrSize CsrBtBipsPushPutLinkedAttachmentFileIndSizeof(void *msg)
{
    CsrBtBipsPushPutLinkedAttachmentFileInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND
CsrUint8 *CsrBtBipsPushPutLinkedAttachmentFileIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsPushPutLinkedAttachmentFileInd *primitive;

    primitive = (CsrBtBipsPushPutLinkedAttachmentFileInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attachmentFileOffset);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->attachmentFileLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND
void *CsrBtBipsPushPutLinkedAttachmentFileIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsPushPutLinkedAttachmentFileInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsPushPutLinkedAttachmentFileInd *) CsrPmemZalloc(sizeof(CsrBtBipsPushPutLinkedAttachmentFileInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attachmentFileOffset, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->attachmentFileLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND
void CsrBtBipsPushPutLinkedAttachmentFileIndSerFree(void *msg)
{
    CsrBtBipsPushPutLinkedAttachmentFileInd *primitive;

    primitive = (CsrBtBipsPushPutLinkedAttachmentFileInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_ATTACHMENT_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_IND
CsrSize CsrBtBipsPushGetCapabilitiesObjectIndSizeof(void *msg)
{
    CsrBtBipsPushGetCapabilitiesObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_IND
CsrUint8 *CsrBtBipsPushGetCapabilitiesObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsPushGetCapabilitiesObjectInd *primitive;

    primitive = (CsrBtBipsPushGetCapabilitiesObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_IND
void *CsrBtBipsPushGetCapabilitiesObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsPushGetCapabilitiesObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsPushGetCapabilitiesObjectInd *) CsrPmemZalloc(sizeof(CsrBtBipsPushGetCapabilitiesObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_IND
CsrSize CsrBtBipsAuthenticateIndSizeof(void *msg)
{
    CsrBtBipsAuthenticateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_IND
CsrUint8 *CsrBtBipsAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAuthenticateInd *primitive;

    primitive = (CsrBtBipsAuthenticateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->options);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->realmLength);
    CsrMemCpySer(buffer, length, &primitive->realm, sizeof(primitive->realm));
    if(primitive->realm)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->realm), ((CsrSize) (primitive->realmLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_IND
void *CsrBtBipsAuthenticateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAuthenticateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAuthenticateInd *) CsrPmemZalloc(sizeof(CsrBtBipsAuthenticateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->options, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->realmLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->realm), buffer, &offset, ((CsrSize) sizeof(primitive->realm)));
    if (primitive->realmLength)
    {
        primitive->realm = CsrPmemZalloc(((CsrUint32) (primitive->realmLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->realm), buffer, &offset, ((CsrSize) (primitive->realmLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->realm = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_IND
void CsrBtBipsAuthenticateIndSerFree(void *msg)
{
    CsrBtBipsAuthenticateInd *primitive;

    primitive = (CsrBtBipsAuthenticateInd *) msg;

    if(primitive->realm != NULL)
    {
        CsrPmemFree(primitive->realm);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_SECURITY_IN_CFM
CsrSize CsrBtBipsSecurityInCfmSizeof(void *msg)
{
    CsrBtBipsSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_SECURITY_IN_CFM
CsrUint8 *CsrBtBipsSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsSecurityInCfm *primitive;

    primitive = (CsrBtBipsSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_SECURITY_IN_CFM
void *CsrBtBipsSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtBipsSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND
CsrSize CsrBtBipsPushPutImageFileIndSizeof(void *msg)
{
    CsrBtBipsPushPutImageFileInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND
CsrUint8 *CsrBtBipsPushPutImageFileIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsPushPutImageFileInd *primitive;

    primitive = (CsrBtBipsPushPutImageFileInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageFileOffset);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->imageFileLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND
void *CsrBtBipsPushPutImageFileIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsPushPutImageFileInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsPushPutImageFileInd *) CsrPmemZalloc(sizeof(CsrBtBipsPushPutImageFileInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageFileOffset, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->imageFileLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND
void CsrBtBipsPushPutImageFileIndSerFree(void *msg)
{
    CsrBtBipsPushPutImageFileInd *primitive;

    primitive = (CsrBtBipsPushPutImageFileInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_IMAGE_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND
CsrSize CsrBtBipsAaGetLinkedThumbnailIndSizeof(void *msg)
{
    CsrBtBipsAaGetLinkedThumbnailInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND
CsrUint8 *CsrBtBipsAaGetLinkedThumbnailIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetLinkedThumbnailInd *primitive;

    primitive = (CsrBtBipsAaGetLinkedThumbnailInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->thumbnailObjectLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->thumbnailObjectOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND
void *CsrBtBipsAaGetLinkedThumbnailIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetLinkedThumbnailInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetLinkedThumbnailInd *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetLinkedThumbnailInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->thumbnailObjectLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->thumbnailObjectOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND
void CsrBtBipsAaGetLinkedThumbnailIndSerFree(void *msg)
{
    CsrBtBipsAaGetLinkedThumbnailInd *primitive;

    primitive = (CsrBtBipsAaGetLinkedThumbnailInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_SECURITY_IN_REQ
CsrSize CsrBtBipsSecurityInReqSizeof(void *msg)
{
    CsrBtBipsSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_SECURITY_IN_REQ
CsrUint8 *CsrBtBipsSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsSecurityInReq *primitive;

    primitive = (CsrBtBipsSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_SECURITY_IN_REQ
void *CsrBtBipsSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtBipsSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_REQ
CsrSize CsrBtBipsAaGetImagePropertiesReqSizeof(void *msg)
{
    CsrBtBipsAaGetImagePropertiesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_REQ
CsrUint8 *CsrBtBipsAaGetImagePropertiesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetImagePropertiesReq *primitive;

    primitive = (CsrBtBipsAaGetImagePropertiesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->imageHandle), ((CsrSize) (8)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_REQ
void *CsrBtBipsAaGetImagePropertiesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetImagePropertiesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetImagePropertiesReq *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetImagePropertiesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->imageHandle), buffer, &offset, ((CsrSize) (8)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_CFM
CsrSize CsrBtBipsAaDeleteImageCfmSizeof(void *msg)
{
    CsrBtBipsAaDeleteImageCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_CFM
CsrUint8 *CsrBtBipsAaDeleteImageCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaDeleteImageCfm *primitive;

    primitive = (CsrBtBipsAaDeleteImageCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_CFM
void *CsrBtBipsAaDeleteImageCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaDeleteImageCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaDeleteImageCfm *) CsrPmemZalloc(sizeof(CsrBtBipsAaDeleteImageCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_DELETE_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND
CsrSize CsrBtBipsAaGetImagePropertiesIndSizeof(void *msg)
{
    CsrBtBipsAaGetImagePropertiesInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND
CsrUint8 *CsrBtBipsAaGetImagePropertiesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetImagePropertiesInd *primitive;

    primitive = (CsrBtBipsAaGetImagePropertiesInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->propertiesObjectLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->propertiesObjectOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND
void *CsrBtBipsAaGetImagePropertiesIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetImagePropertiesInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetImagePropertiesInd *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetImagePropertiesInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->propertiesObjectLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->propertiesObjectOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->payloadLength)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->payloadLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND
void CsrBtBipsAaGetImagePropertiesIndSerFree(void *msg)
{
    CsrBtBipsAaGetImagePropertiesInd *primitive;

    primitive = (CsrBtBipsAaGetImagePropertiesInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_PROPERTIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ
CsrSize CsrBtBipsAaGetImageListReqSizeof(void *msg)
{
    CsrBtBipsAaGetImageListReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->imageHandlesDescriptorLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ
CsrUint8 *CsrBtBipsAaGetImageListReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetImageListReq *primitive;

    primitive = (CsrBtBipsAaGetImageListReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->nbReturnedHandles);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->listStartOffset);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->latestCapturedImages);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageHandlesDescriptorLength);
    CsrMemCpySer(buffer, length, &primitive->imageHandlesDescriptor, sizeof(primitive->imageHandlesDescriptor));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->imageHandlesDescriptor)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->imageHandlesDescriptor), ((CsrSize) (primitive->imageHandlesDescriptorLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ
void *CsrBtBipsAaGetImageListReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetImageListReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetImageListReq *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetImageListReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->nbReturnedHandles, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->listStartOffset, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->latestCapturedImages, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageHandlesDescriptorLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->imageHandlesDescriptor), buffer, &offset, ((CsrSize) sizeof(primitive->imageHandlesDescriptor)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->imageHandlesDescriptorLength)
    {
        primitive->imageHandlesDescriptor = CsrPmemZalloc(((CsrUint32) (primitive->imageHandlesDescriptorLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->imageHandlesDescriptor), buffer, &offset, ((CsrSize) (primitive->imageHandlesDescriptorLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->imageHandlesDescriptor = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ
void CsrBtBipsAaGetImageListReqSerFree(void *msg)
{
    CsrBtBipsAaGetImageListReq *primitive;

    primitive = (CsrBtBipsAaGetImageListReq *) msg;

    if(primitive->imageHandlesDescriptor != NULL)
    {
        CsrPmemFree(primitive->imageHandlesDescriptor);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_DEACTIVATE_IND
CsrSize CsrBtBipsDeactivateIndSizeof(void *msg)
{
    CsrBtBipsDeactivateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_DEACTIVATE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_DEACTIVATE_IND
CsrUint8 *CsrBtBipsDeactivateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsDeactivateInd *primitive;

    primitive = (CsrBtBipsDeactivateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_DEACTIVATE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_DEACTIVATE_IND
void *CsrBtBipsDeactivateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsDeactivateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsDeactivateInd *) CsrPmemZalloc(sizeof(CsrBtBipsDeactivateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_DEACTIVATE_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_RES
CsrSize CsrBtBipsAaGetImageListResSizeof(void *msg)
{
    CsrBtBipsAaGetImageListRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_RES
CsrUint8 *CsrBtBipsAaGetImageListResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetImageListRes *primitive;

    primitive = (CsrBtBipsAaGetImageListRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_RES
void *CsrBtBipsAaGetImageListResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetImageListRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetImageListRes *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetImageListRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_IMAGE_LIST_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_REGISTER_QID_REQ
CsrSize CsrBtBipsRegisterQidReqSizeof(void *msg)
{
    CsrBtBipsRegisterQidReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_REGISTER_QID_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_REGISTER_QID_REQ
CsrUint8 *CsrBtBipsRegisterQidReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRegisterQidReq *primitive;

    primitive = (CsrBtBipsRegisterQidReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_REGISTER_QID_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_REGISTER_QID_REQ
void *CsrBtBipsRegisterQidReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRegisterQidReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRegisterQidReq *) CsrPmemZalloc(sizeof(CsrBtBipsRegisterQidReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_REGISTER_QID_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES
CsrSize CsrBtBipsRcGetLinkedThumbnailObjectResSizeof(void *msg)
{
    CsrBtBipsRcGetLinkedThumbnailObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->thumbnailObjectLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES
CsrUint8 *CsrBtBipsRcGetLinkedThumbnailObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsRcGetLinkedThumbnailObjectRes *primitive;

    primitive = (CsrBtBipsRcGetLinkedThumbnailObjectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->thumbnailObjectLength);
    CsrMemCpySer(buffer, length, &primitive->thumbnailObject, sizeof(primitive->thumbnailObject));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->thumbnailObject)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->thumbnailObject), ((CsrSize) (primitive->thumbnailObjectLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES
void *CsrBtBipsRcGetLinkedThumbnailObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsRcGetLinkedThumbnailObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsRcGetLinkedThumbnailObjectRes *) CsrPmemZalloc(sizeof(CsrBtBipsRcGetLinkedThumbnailObjectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->thumbnailObjectLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->thumbnailObject), buffer, &offset, ((CsrSize) sizeof(primitive->thumbnailObject)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->thumbnailObjectLength)
    {
        primitive->thumbnailObject = CsrPmemZalloc(((CsrUint32) (primitive->thumbnailObjectLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->thumbnailObject), buffer, &offset, ((CsrSize) (primitive->thumbnailObjectLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->thumbnailObject = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES
void CsrBtBipsRcGetLinkedThumbnailObjectResSerFree(void *msg)
{
    CsrBtBipsRcGetLinkedThumbnailObjectRes *primitive;

    primitive = (CsrBtBipsRcGetLinkedThumbnailObjectRes *) msg;

    if(primitive->thumbnailObject != NULL)
    {
        CsrPmemFree(primitive->thumbnailObject);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_RC_GET_LINKED_THUMBNAIL_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_DISCONNECT_IND
CsrSize CsrBtBipsDisconnectIndSizeof(void *msg)
{
    CsrBtBipsDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_DISCONNECT_IND
CsrUint8 *CsrBtBipsDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsDisconnectInd *primitive;

    primitive = (CsrBtBipsDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_DISCONNECT_IND
void *CsrBtBipsDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtBipsDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES
CsrSize CsrBtBipsPushGetCapabilitiesObjectResSizeof(void *msg)
{
    CsrBtBipsPushGetCapabilitiesObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->capabilitiesObjectLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES
CsrUint8 *CsrBtBipsPushGetCapabilitiesObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsPushGetCapabilitiesObjectRes *primitive;

    primitive = (CsrBtBipsPushGetCapabilitiesObjectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->capabilitiesObjectLength);
    CsrMemCpySer(buffer, length, &primitive->capabilitiesObject, sizeof(primitive->capabilitiesObject));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->capabilitiesObject)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->capabilitiesObject), ((CsrSize) (primitive->capabilitiesObjectLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES
void *CsrBtBipsPushGetCapabilitiesObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsPushGetCapabilitiesObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsPushGetCapabilitiesObjectRes *) CsrPmemZalloc(sizeof(CsrBtBipsPushGetCapabilitiesObjectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->capabilitiesObjectLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->capabilitiesObject), buffer, &offset, ((CsrSize) sizeof(primitive->capabilitiesObject)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->capabilitiesObjectLength)
    {
        primitive->capabilitiesObject = CsrPmemZalloc(((CsrUint32) (primitive->capabilitiesObjectLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->capabilitiesObject), buffer, &offset, ((CsrSize) (primitive->capabilitiesObjectLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->capabilitiesObject = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES
void CsrBtBipsPushGetCapabilitiesObjectResSerFree(void *msg)
{
    CsrBtBipsPushGetCapabilitiesObjectRes *primitive;

    primitive = (CsrBtBipsPushGetCapabilitiesObjectRes *) msg;

    if(primitive->capabilitiesObject != NULL)
    {
        CsrPmemFree(primitive->capabilitiesObject);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_GET_CAPABILITIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_REQ
CsrSize CsrBtBipsAaGetLinkedThumbnailReqSizeof(void *msg)
{
    CsrBtBipsAaGetLinkedThumbnailReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_REQ
CsrUint8 *CsrBtBipsAaGetLinkedThumbnailReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsAaGetLinkedThumbnailReq *primitive;

    primitive = (CsrBtBipsAaGetLinkedThumbnailReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->imageHandle), ((CsrSize) (8)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_REQ
void *CsrBtBipsAaGetLinkedThumbnailReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsAaGetLinkedThumbnailReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsAaGetLinkedThumbnailReq *) CsrPmemZalloc(sizeof(CsrBtBipsAaGetLinkedThumbnailReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->imageHandle), buffer, &offset, ((CsrSize) (8)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_AA_GET_LINKED_THUMBNAIL_REQ */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_RES
CsrSize CsrBtBipsPushPutLinkedThumbnailFileResSizeof(void *msg)
{
    CsrBtBipsPushPutLinkedThumbnailFileRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_RES
CsrUint8 *CsrBtBipsPushPutLinkedThumbnailFileResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipsPushPutLinkedThumbnailFileRes *primitive;

    primitive = (CsrBtBipsPushPutLinkedThumbnailFileRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_RES
void *CsrBtBipsPushPutLinkedThumbnailFileResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipsPushPutLinkedThumbnailFileRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipsPushPutLinkedThumbnailFileRes *) CsrPmemZalloc(sizeof(CsrBtBipsPushPutLinkedThumbnailFileRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPS_PUSH_PUT_LINKED_THUMBNAIL_FILE_RES */

#endif /* EXCLUDE_CSR_BT_BIPS_MODULE */
