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
#include "csr_bt_bipc_prim.h"

#ifndef EXCLUDE_CSR_BT_BIPC_MODULE

void CsrBtBipcPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND
CsrSize CsrBtBipcRcGetImagePropertiesIndSizeof(void *msg)
{
    CsrBtBipcRcGetImagePropertiesInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND
CsrUint8 *CsrBtBipcRcGetImagePropertiesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetImagePropertiesInd *primitive;

    primitive = (CsrBtBipcRcGetImagePropertiesInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND
void *CsrBtBipcRcGetImagePropertiesIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetImagePropertiesInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetImagePropertiesInd *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetImagePropertiesInd));

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
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND
void CsrBtBipcRcGetImagePropertiesIndSerFree(void *msg)
{
    CsrBtBipcRcGetImagePropertiesInd *primitive;

    primitive = (CsrBtBipcRcGetImagePropertiesInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_CFM
CsrSize CsrBtBipcSecurityOutCfmSizeof(void *msg)
{
    CsrBtBipcSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_CFM
CsrUint8 *CsrBtBipcSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcSecurityOutCfm *primitive;

    primitive = (CsrBtBipcSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_CFM
void *CsrBtBipcSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtBipcSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_RES
CsrSize CsrBtBipcRcGetMonitoringImageHeaderResSizeof(void *msg)
{
    CsrBtBipcRcGetMonitoringImageHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_RES
CsrUint8 *CsrBtBipcRcGetMonitoringImageHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetMonitoringImageHeaderRes *primitive;

    primitive = (CsrBtBipcRcGetMonitoringImageHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_RES
void *CsrBtBipcRcGetMonitoringImageHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetMonitoringImageHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetMonitoringImageHeaderRes *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetMonitoringImageHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES
CsrSize CsrBtBipcAaGetImageObjectResSizeof(void *msg)
{
    CsrBtBipcAaGetImageObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->imageObjectLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES
CsrUint8 *CsrBtBipcAaGetImageObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetImageObjectRes *primitive;

    primitive = (CsrBtBipcAaGetImageObjectRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES
void *CsrBtBipcAaGetImageObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetImageObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetImageObjectRes *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetImageObjectRes));

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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES
void CsrBtBipcAaGetImageObjectResSerFree(void *msg)
{
    CsrBtBipcAaGetImageObjectRes *primitive;

    primitive = (CsrBtBipcAaGetImageObjectRes *) msg;

    if(primitive->imageObject != NULL)
    {
        CsrPmemFree(primitive->imageObject);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ
CsrSize CsrBtBipcPushPutImageReqSizeof(void *msg)
{
    CsrBtBipcPushPutImageReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2imageName) + 2 + (primitive->imageDescriptorLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ
CsrUint8 *CsrBtBipcPushPutImageReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcPushPutImageReq *primitive;

    primitive = (CsrBtBipcPushPutImageReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->ucs2imageName, sizeof(primitive->ucs2imageName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageDescriptorLength);
    CsrMemCpySer(buffer, length, &primitive->imageDescriptor, sizeof(primitive->imageDescriptor));
    if (primitive->ucs2imageName)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->ucs2imageName), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2imageName) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2imageName) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }
    if(primitive->imageDescriptor)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->imageDescriptor), ((CsrSize) (primitive->imageDescriptorLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ
void *CsrBtBipcPushPutImageReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcPushPutImageReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcPushPutImageReq *) CsrPmemZalloc(sizeof(CsrBtBipcPushPutImageReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2imageName), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2imageName)));
    CsrUint16Des((CsrUint16 *) &primitive->imageDescriptorLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->imageDescriptor), buffer, &offset, ((CsrSize) sizeof(primitive->imageDescriptor)));
    primitive->ucs2imageName = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2imageName), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2imageName) + 2));
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
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ
void CsrBtBipcPushPutImageReqSerFree(void *msg)
{
    CsrBtBipcPushPutImageReq *primitive;

    primitive = (CsrBtBipcPushPutImageReq *) msg;

    if(primitive->ucs2imageName != NULL)
    {
        CsrPmemFree(primitive->ucs2imageName);
    }
    if(primitive->imageDescriptor != NULL)
    {
        CsrPmemFree(primitive->imageDescriptor);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_RES
CsrSize CsrBtBipcAaGetLinkedThumbnailHeaderResSizeof(void *msg)
{
    CsrBtBipcAaGetLinkedThumbnailHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_RES
CsrUint8 *CsrBtBipcAaGetLinkedThumbnailHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetLinkedThumbnailHeaderRes *primitive;

    primitive = (CsrBtBipcAaGetLinkedThumbnailHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_RES
void *CsrBtBipcAaGetLinkedThumbnailHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetLinkedThumbnailHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetLinkedThumbnailHeaderRes *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetLinkedThumbnailHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND
CsrSize CsrBtBipcAaGetImageHeaderIndSizeof(void *msg)
{
    CsrBtBipcAaGetImageHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND
CsrUint8 *CsrBtBipcAaGetImageHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetImageHeaderInd *primitive;

    primitive = (CsrBtBipcAaGetImageHeaderInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND
void *CsrBtBipcAaGetImageHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetImageHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetImageHeaderInd *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetImageHeaderInd));

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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND
void CsrBtBipcAaGetImageHeaderIndSerFree(void *msg)
{
    CsrBtBipcAaGetImageHeaderInd *primitive;

    primitive = (CsrBtBipcAaGetImageHeaderInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES
CsrSize CsrBtBipcAaGetLinkedAttachmentObjectResSizeof(void *msg)
{
    CsrBtBipcAaGetLinkedAttachmentObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attachmentObjectLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES
CsrUint8 *CsrBtBipcAaGetLinkedAttachmentObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetLinkedAttachmentObjectRes *primitive;

    primitive = (CsrBtBipcAaGetLinkedAttachmentObjectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attachmentObjectLength);
    CsrMemCpySer(buffer, length, &primitive->attachmentObject, sizeof(primitive->attachmentObject));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->attachmentObject)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attachmentObject), ((CsrSize) (primitive->attachmentObjectLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES
void *CsrBtBipcAaGetLinkedAttachmentObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetLinkedAttachmentObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetLinkedAttachmentObjectRes *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetLinkedAttachmentObjectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attachmentObjectLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attachmentObject), buffer, &offset, ((CsrSize) sizeof(primitive->attachmentObject)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->attachmentObjectLength)
    {
        primitive->attachmentObject = CsrPmemZalloc(((CsrUint32) (primitive->attachmentObjectLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->attachmentObject), buffer, &offset, ((CsrSize) (primitive->attachmentObjectLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->attachmentObject = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES
void CsrBtBipcAaGetLinkedAttachmentObjectResSerFree(void *msg)
{
    CsrBtBipcAaGetLinkedAttachmentObjectRes *primitive;

    primitive = (CsrBtBipcAaGetLinkedAttachmentObjectRes *) msg;

    if(primitive->attachmentObject != NULL)
    {
        CsrPmemFree(primitive->attachmentObject);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_REQ
CsrSize CsrBtBipcRcGetMonitoringImageReqSizeof(void *msg)
{
    CsrBtBipcRcGetMonitoringImageReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_REQ
CsrUint8 *CsrBtBipcRcGetMonitoringImageReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetMonitoringImageReq *primitive;

    primitive = (CsrBtBipcRcGetMonitoringImageReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->storeFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_REQ
void *CsrBtBipcRcGetMonitoringImageReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetMonitoringImageReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetMonitoringImageReq *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetMonitoringImageReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->storeFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_CONNECT_REQ
CsrSize CsrBtBipcConnectReqSizeof(void *msg)
{
    CsrBtBipcConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_CONNECT_REQ
CsrUint8 *CsrBtBipcConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcConnectReq *primitive;

    primitive = (CsrBtBipcConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authorize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->feature);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->realmLength);
    CsrMemCpySer(buffer, length, &primitive->realm, sizeof(primitive->realm));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->passwordLength);
    CsrMemCpySer(buffer, length, &primitive->password, sizeof(primitive->password));
    CsrMemCpySer(buffer, length, &primitive->userId, sizeof(primitive->userId));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->count);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->windowSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmEnable);
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
#endif /* EXCLUDE_CSR_BT_BIPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_CONNECT_REQ
void *CsrBtBipcConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcConnectReq *) CsrPmemZalloc(sizeof(CsrBtBipcConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authorize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->feature, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->realmLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->realm), buffer, &offset, ((CsrSize) sizeof(primitive->realm)));
    CsrUint16Des((CsrUint16 *) &primitive->passwordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->password), buffer, &offset, ((CsrSize) sizeof(primitive->password)));
    CsrMemCpyDes(((void *) &primitive->userId), buffer, &offset, ((CsrSize) sizeof(primitive->userId)));
    CsrUint32Des((CsrUint32 *) &primitive->count, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->windowSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmEnable, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_BIPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_CONNECT_REQ
void CsrBtBipcConnectReqSerFree(void *msg)
{
    CsrBtBipcConnectReq *primitive;

    primitive = (CsrBtBipcConnectReq *) msg;

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
#endif /* EXCLUDE_CSR_BT_BIPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_REQ
CsrSize CsrBtBipcPushGetCapabilitiesReqSizeof(void *msg)
{
    CsrBtBipcPushGetCapabilitiesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_REQ
CsrUint8 *CsrBtBipcPushGetCapabilitiesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcPushGetCapabilitiesReq *primitive;

    primitive = (CsrBtBipcPushGetCapabilitiesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_REQ
void *CsrBtBipcPushGetCapabilitiesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcPushGetCapabilitiesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcPushGetCapabilitiesReq *) CsrPmemZalloc(sizeof(CsrBtBipcPushGetCapabilitiesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_IND
CsrSize CsrBtBipcPushPutThumbnailFileIndSizeof(void *msg)
{
    CsrBtBipcPushPutThumbnailFileInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_IND
CsrUint8 *CsrBtBipcPushPutThumbnailFileIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcPushPutThumbnailFileInd *primitive;

    primitive = (CsrBtBipcPushPutThumbnailFileInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->thumbnailFileLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_IND
void *CsrBtBipcPushPutThumbnailFileIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcPushPutThumbnailFileInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcPushPutThumbnailFileInd *) CsrPmemZalloc(sizeof(CsrBtBipcPushPutThumbnailFileInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->thumbnailFileLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_RES
CsrSize CsrBtBipcRcGetLinkedThumbnailResSizeof(void *msg)
{
    CsrBtBipcRcGetLinkedThumbnailRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_RES
CsrUint8 *CsrBtBipcRcGetLinkedThumbnailResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetLinkedThumbnailRes *primitive;

    primitive = (CsrBtBipcRcGetLinkedThumbnailRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_RES
void *CsrBtBipcRcGetLinkedThumbnailResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetLinkedThumbnailRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetLinkedThumbnailRes *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetLinkedThumbnailRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_RES
CsrSize CsrBtBipcAaGetCapabilitiesHeaderResSizeof(void *msg)
{
    CsrBtBipcAaGetCapabilitiesHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_RES
CsrUint8 *CsrBtBipcAaGetCapabilitiesHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetCapabilitiesHeaderRes *primitive;

    primitive = (CsrBtBipcAaGetCapabilitiesHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_RES
void *CsrBtBipcAaGetCapabilitiesHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetCapabilitiesHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetCapabilitiesHeaderRes *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetCapabilitiesHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_ABORT_CFM
CsrSize CsrBtBipcAbortCfmSizeof(void *msg)
{
    CsrBtBipcAbortCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_ABORT_CFM
CsrUint8 *CsrBtBipcAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAbortCfm *primitive;

    primitive = (CsrBtBipcAbortCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_ABORT_CFM
void *CsrBtBipcAbortCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAbortCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAbortCfm *) CsrPmemZalloc(sizeof(CsrBtBipcAbortCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_IND
CsrSize CsrBtBipcRcGetMonitoringImageHeaderIndSizeof(void *msg)
{
    CsrBtBipcRcGetMonitoringImageHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_IND
CsrUint8 *CsrBtBipcRcGetMonitoringImageHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetMonitoringImageHeaderInd *primitive;

    primitive = (CsrBtBipcRcGetMonitoringImageHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrMemCpySer(buffer, length, ((const void *) primitive->imageHandle), ((CsrSize) (8)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_IND
void *CsrBtBipcRcGetMonitoringImageHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetMonitoringImageHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetMonitoringImageHeaderInd *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetMonitoringImageHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->imageHandle), buffer, &offset, ((CsrSize) (8)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_RES
CsrSize CsrBtBipcRcGetImageResSizeof(void *msg)
{
    CsrBtBipcRcGetImageRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_RES
CsrUint8 *CsrBtBipcRcGetImageResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetImageRes *primitive;

    primitive = (CsrBtBipcRcGetImageRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_RES
void *CsrBtBipcRcGetImageResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetImageRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetImageRes *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetImageRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ
CsrSize CsrBtBipcPushPutAttachmentReqSizeof(void *msg)
{
    CsrBtBipcPushPutAttachmentReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attachmentDescriptorLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ
CsrUint8 *CsrBtBipcPushPutAttachmentReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcPushPutAttachmentReq *primitive;

    primitive = (CsrBtBipcPushPutAttachmentReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->attachmentDescriptor, sizeof(primitive->attachmentDescriptor));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attachmentDescriptorLength);
    CsrMemCpySer(buffer, length, ((const void *) primitive->imageHandle), ((CsrSize) (8)));
    if(primitive->attachmentDescriptor)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attachmentDescriptor), ((CsrSize) (primitive->attachmentDescriptorLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ
void *CsrBtBipcPushPutAttachmentReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcPushPutAttachmentReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcPushPutAttachmentReq *) CsrPmemZalloc(sizeof(CsrBtBipcPushPutAttachmentReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attachmentDescriptor), buffer, &offset, ((CsrSize) sizeof(primitive->attachmentDescriptor)));
    CsrUint16Des((CsrUint16 *) &primitive->attachmentDescriptorLength, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->imageHandle), buffer, &offset, ((CsrSize) (8)));
    if (primitive->attachmentDescriptorLength)
    {
        primitive->attachmentDescriptor = CsrPmemZalloc(((CsrUint32) (primitive->attachmentDescriptorLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->attachmentDescriptor), buffer, &offset, ((CsrSize) (primitive->attachmentDescriptorLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->attachmentDescriptor = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ
void CsrBtBipcPushPutAttachmentReqSerFree(void *msg)
{
    CsrBtBipcPushPutAttachmentReq *primitive;

    primitive = (CsrBtBipcPushPutAttachmentReq *) msg;

    if(primitive->attachmentDescriptor != NULL)
    {
        CsrPmemFree(primitive->attachmentDescriptor);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM
CsrSize CsrBtBipcRcGetImagePropertiesCfmSizeof(void *msg)
{
    CsrBtBipcRcGetImagePropertiesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM
CsrUint8 *CsrBtBipcRcGetImagePropertiesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetImagePropertiesCfm *primitive;

    primitive = (CsrBtBipcRcGetImagePropertiesCfm *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM
void *CsrBtBipcRcGetImagePropertiesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetImagePropertiesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetImagePropertiesCfm *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetImagePropertiesCfm));

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
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM
void CsrBtBipcRcGetImagePropertiesCfmSerFree(void *msg)
{
    CsrBtBipcRcGetImagePropertiesCfm *primitive;

    primitive = (CsrBtBipcRcGetImagePropertiesCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_RES
CsrSize CsrBtBipcAaGetLinkedAttachmentHeaderResSizeof(void *msg)
{
    CsrBtBipcAaGetLinkedAttachmentHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_RES
CsrUint8 *CsrBtBipcAaGetLinkedAttachmentHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetLinkedAttachmentHeaderRes *primitive;

    primitive = (CsrBtBipcAaGetLinkedAttachmentHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_RES
void *CsrBtBipcAaGetLinkedAttachmentHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetLinkedAttachmentHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetLinkedAttachmentHeaderRes *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetLinkedAttachmentHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_IND
CsrSize CsrBtBipcPushPutAttachmentFileIndSizeof(void *msg)
{
    CsrBtBipcPushPutAttachmentFileInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_IND
CsrUint8 *CsrBtBipcPushPutAttachmentFileIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcPushPutAttachmentFileInd *primitive;

    primitive = (CsrBtBipcPushPutAttachmentFileInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attachmentFileLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_IND
void *CsrBtBipcPushPutAttachmentFileIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcPushPutAttachmentFileInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcPushPutAttachmentFileInd *) CsrPmemZalloc(sizeof(CsrBtBipcPushPutAttachmentFileInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attachmentFileLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_RES
CsrSize CsrBtBipcPushGetCapabilitiesResSizeof(void *msg)
{
    CsrBtBipcPushGetCapabilitiesRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_RES
CsrUint8 *CsrBtBipcPushGetCapabilitiesResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcPushGetCapabilitiesRes *primitive;

    primitive = (CsrBtBipcPushGetCapabilitiesRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_RES
void *CsrBtBipcPushGetCapabilitiesResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcPushGetCapabilitiesRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcPushGetCapabilitiesRes *) CsrPmemZalloc(sizeof(CsrBtBipcPushGetCapabilitiesRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_CFM
CsrSize CsrBtBipcRcGetMonitoringImageCfmSizeof(void *msg)
{
    CsrBtBipcRcGetMonitoringImageCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_CFM
CsrUint8 *CsrBtBipcRcGetMonitoringImageCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetMonitoringImageCfm *primitive;

    primitive = (CsrBtBipcRcGetMonitoringImageCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_CFM
void *CsrBtBipcRcGetMonitoringImageCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetMonitoringImageCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetMonitoringImageCfm *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetMonitoringImageCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_RES
CsrSize CsrBtBipcAuthenticateResSizeof(void *msg)
{
    CsrBtBipcAuthenticateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->authPasswordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->authUserId) + (primitive->chalRealmLength * sizeof(CsrUint8)) + (primitive->chalPasswordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->chalUserId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_RES
CsrUint8 *CsrBtBipcAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAuthenticateRes *primitive;

    primitive = (CsrBtBipcAuthenticateRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->authPassword, sizeof(primitive->authPassword));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->authPasswordLength);
    CsrMemCpySer(buffer, length, &primitive->authUserId, sizeof(primitive->authUserId));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->chalRealmLength);
    CsrMemCpySer(buffer, length, &primitive->chalRealm, sizeof(primitive->chalRealm));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->chalPasswordLength);
    CsrMemCpySer(buffer, length, &primitive->chalPassword, sizeof(primitive->chalPassword));
    CsrMemCpySer(buffer, length, &primitive->chalUserId, sizeof(primitive->chalUserId));
    if(primitive->authPassword)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->authPassword), ((CsrSize) (primitive->authPasswordLength * sizeof(CsrUint8))));
    }
    CsrCharStringSer(buffer, length, primitive->authUserId);
    if(primitive->chalRealm)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->chalRealm), ((CsrSize) (primitive->chalRealmLength * sizeof(CsrUint8))));
    }
    if(primitive->chalPassword)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->chalPassword), ((CsrSize) (primitive->chalPasswordLength * sizeof(CsrUint8))));
    }
    CsrCharStringSer(buffer, length, primitive->chalUserId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_RES
void *CsrBtBipcAuthenticateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAuthenticateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAuthenticateRes *) CsrPmemZalloc(sizeof(CsrBtBipcAuthenticateRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->authPassword), buffer, &offset, ((CsrSize) sizeof(primitive->authPassword)));
    CsrUint16Des((CsrUint16 *) &primitive->authPasswordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->authUserId), buffer, &offset, ((CsrSize) sizeof(primitive->authUserId)));
    CsrUint16Des((CsrUint16 *) &primitive->chalRealmLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->chalRealm), buffer, &offset, ((CsrSize) sizeof(primitive->chalRealm)));
    CsrUint16Des((CsrUint16 *) &primitive->chalPasswordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->chalPassword), buffer, &offset, ((CsrSize) sizeof(primitive->chalPassword)));
    CsrMemCpyDes(((void *) &primitive->chalUserId), buffer, &offset, ((CsrSize) sizeof(primitive->chalUserId)));
    if (primitive->authPasswordLength)
    {
        primitive->authPassword = CsrPmemZalloc(((CsrUint32) (primitive->authPasswordLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->authPassword), buffer, &offset, ((CsrSize) (primitive->authPasswordLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->authPassword = NULL;
    }
    CsrCharStringDes(&primitive->authUserId, buffer, &offset);
    if (primitive->chalRealmLength)
    {
        primitive->chalRealm = CsrPmemZalloc(((CsrUint32) (primitive->chalRealmLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->chalRealm), buffer, &offset, ((CsrSize) (primitive->chalRealmLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->chalRealm = NULL;
    }
    if (primitive->chalPasswordLength)
    {
        primitive->chalPassword = CsrPmemZalloc(((CsrUint32) (primitive->chalPasswordLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->chalPassword), buffer, &offset, ((CsrSize) (primitive->chalPasswordLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->chalPassword = NULL;
    }
    CsrCharStringDes(&primitive->chalUserId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_RES
void CsrBtBipcAuthenticateResSerFree(void *msg)
{
    CsrBtBipcAuthenticateRes *primitive;

    primitive = (CsrBtBipcAuthenticateRes *) msg;

    if(primitive->authPassword != NULL)
    {
        CsrPmemFree(primitive->authPassword);
    }
    if(primitive->authUserId != NULL)
    {
        CsrPmemFree(primitive->authUserId);
    }
    if(primitive->chalRealm != NULL)
    {
        CsrPmemFree(primitive->chalRealm);
    }
    if(primitive->chalPassword != NULL)
    {
        CsrPmemFree(primitive->chalPassword);
    }
    if(primitive->chalUserId != NULL)
    {
        CsrPmemFree(primitive->chalUserId);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES
CsrSize CsrBtBipcPushPutAttachmentFileResSizeof(void *msg)
{
    CsrBtBipcPushPutAttachmentFileRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attachmentFileLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES
CsrUint8 *CsrBtBipcPushPutAttachmentFileResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcPushPutAttachmentFileRes *primitive;

    primitive = (CsrBtBipcPushPutAttachmentFileRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attachmentFileLength);
    CsrMemCpySer(buffer, length, &primitive->attachmentFile, sizeof(primitive->attachmentFile));
    if(primitive->attachmentFile)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attachmentFile), ((CsrSize) (primitive->attachmentFileLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES
void *CsrBtBipcPushPutAttachmentFileResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcPushPutAttachmentFileRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcPushPutAttachmentFileRes *) CsrPmemZalloc(sizeof(CsrBtBipcPushPutAttachmentFileRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attachmentFileLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attachmentFile), buffer, &offset, ((CsrSize) sizeof(primitive->attachmentFile)));
    if (primitive->attachmentFileLength)
    {
        primitive->attachmentFile = CsrPmemZalloc(((CsrUint32) (primitive->attachmentFileLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->attachmentFile), buffer, &offset, ((CsrSize) (primitive->attachmentFileLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->attachmentFile = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES
void CsrBtBipcPushPutAttachmentFileResSerFree(void *msg)
{
    CsrBtBipcPushPutAttachmentFileRes *primitive;

    primitive = (CsrBtBipcPushPutAttachmentFileRes *) msg;

    if(primitive->attachmentFile != NULL)
    {
        CsrPmemFree(primitive->attachmentFile);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_REQ
CsrSize CsrBtBipcRcGetImageReqSizeof(void *msg)
{
    CsrBtBipcRcGetImageReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->imageDescriptorLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_REQ
CsrUint8 *CsrBtBipcRcGetImageReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetImageReq *primitive;

    primitive = (CsrBtBipcRcGetImageReq *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_REQ
void *CsrBtBipcRcGetImageReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetImageReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetImageReq *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetImageReq));

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
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_REQ
void CsrBtBipcRcGetImageReqSerFree(void *msg)
{
    CsrBtBipcRcGetImageReq *primitive;

    primitive = (CsrBtBipcRcGetImageReq *) msg;

    if(primitive->imageDescriptor != NULL)
    {
        CsrPmemFree(primitive->imageDescriptor);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_CFM
CsrSize CsrBtBipcRcGetImageCfmSizeof(void *msg)
{
    CsrBtBipcRcGetImageCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_CFM
CsrUint8 *CsrBtBipcRcGetImageCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetImageCfm *primitive;

    primitive = (CsrBtBipcRcGetImageCfm *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_CFM
void *CsrBtBipcRcGetImageCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetImageCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetImageCfm *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetImageCfm));

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
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_CFM
void CsrBtBipcRcGetImageCfmSerFree(void *msg)
{
    CsrBtBipcRcGetImageCfm *primitive;

    primitive = (CsrBtBipcRcGetImageCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_RES
CsrSize CsrBtBipcAaGetLinkedThumbnailObjectResSizeof(void *msg)
{
    CsrBtBipcAaGetLinkedThumbnailObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->thumbnailObjectLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_RES
CsrUint8 *CsrBtBipcAaGetLinkedThumbnailObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetLinkedThumbnailObjectRes *primitive;

    primitive = (CsrBtBipcAaGetLinkedThumbnailObjectRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_RES
void *CsrBtBipcAaGetLinkedThumbnailObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetLinkedThumbnailObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetLinkedThumbnailObjectRes *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetLinkedThumbnailObjectRes));

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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_RES
void CsrBtBipcAaGetLinkedThumbnailObjectResSerFree(void *msg)
{
    CsrBtBipcAaGetLinkedThumbnailObjectRes *primitive;

    primitive = (CsrBtBipcAaGetLinkedThumbnailObjectRes *) msg;

    if(primitive->thumbnailObject != NULL)
    {
        CsrPmemFree(primitive->thumbnailObject);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_IND
CsrSize CsrBtBipcAaGetLinkedAttachmentObjectIndSizeof(void *msg)
{
    CsrBtBipcAaGetLinkedAttachmentObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_IND
CsrUint8 *CsrBtBipcAaGetLinkedAttachmentObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetLinkedAttachmentObjectInd *primitive;

    primitive = (CsrBtBipcAaGetLinkedAttachmentObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->allowedObjectLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_IND
void *CsrBtBipcAaGetLinkedAttachmentObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetLinkedAttachmentObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetLinkedAttachmentObjectInd *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetLinkedAttachmentObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->allowedObjectLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_REQ
CsrSize CsrBtBipcRcGetLinkedThumbnailReqSizeof(void *msg)
{
    CsrBtBipcRcGetLinkedThumbnailReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_REQ
CsrUint8 *CsrBtBipcRcGetLinkedThumbnailReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetLinkedThumbnailReq *primitive;

    primitive = (CsrBtBipcRcGetLinkedThumbnailReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->imageHandle), ((CsrSize) (8)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_REQ
void *CsrBtBipcRcGetLinkedThumbnailReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetLinkedThumbnailReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetLinkedThumbnailReq *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetLinkedThumbnailReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->imageHandle), buffer, &offset, ((CsrSize) (8)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_IND
CsrSize CsrBtBipcAuthenticateIndSizeof(void *msg)
{
    CsrBtBipcAuthenticateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_IND
CsrUint8 *CsrBtBipcAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAuthenticateInd *primitive;

    primitive = (CsrBtBipcAuthenticateInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_IND
void *CsrBtBipcAuthenticateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAuthenticateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAuthenticateInd *) CsrPmemZalloc(sizeof(CsrBtBipcAuthenticateInd));

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
#endif /* EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_IND
void CsrBtBipcAuthenticateIndSerFree(void *msg)
{
    CsrBtBipcAuthenticateInd *primitive;

    primitive = (CsrBtBipcAuthenticateInd *) msg;

    if(primitive->realm != NULL)
    {
        CsrPmemFree(primitive->realm);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_IND
CsrSize CsrBtBipcPushPutImageFileIndSizeof(void *msg)
{
    CsrBtBipcPushPutImageFileInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_IND
CsrUint8 *CsrBtBipcPushPutImageFileIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcPushPutImageFileInd *primitive;

    primitive = (CsrBtBipcPushPutImageFileInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageFileLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_IND
void *CsrBtBipcPushPutImageFileIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcPushPutImageFileInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcPushPutImageFileInd *) CsrPmemZalloc(sizeof(CsrBtBipcPushPutImageFileInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageFileLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_CFM
CsrSize CsrBtBipcGetRemoteFeaturesCfmSizeof(void *msg)
{
    CsrBtBipcGetRemoteFeaturesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_CFM
CsrUint8 *CsrBtBipcGetRemoteFeaturesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcGetRemoteFeaturesCfm *primitive;

    primitive = (CsrBtBipcGetRemoteFeaturesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supportedFeatures);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_CFM
void *CsrBtBipcGetRemoteFeaturesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcGetRemoteFeaturesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcGetRemoteFeaturesCfm *) CsrPmemZalloc(sizeof(CsrBtBipcGetRemoteFeaturesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supportedFeatures, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_IND
CsrSize CsrBtBipcAaGetImageListObjectIndSizeof(void *msg)
{
    CsrBtBipcAaGetImageListObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_IND
CsrUint8 *CsrBtBipcAaGetImageListObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetImageListObjectInd *primitive;

    primitive = (CsrBtBipcAaGetImageListObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->allowedObjectLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_IND
void *CsrBtBipcAaGetImageListObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetImageListObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetImageListObjectInd *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetImageListObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->allowedObjectLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND
CsrSize CsrBtBipcAaGetLinkedAttachmentHeaderIndSizeof(void *msg)
{
    CsrBtBipcAaGetLinkedAttachmentHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND
CsrUint8 *CsrBtBipcAaGetLinkedAttachmentHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetLinkedAttachmentHeaderInd *primitive;

    primitive = (CsrBtBipcAaGetLinkedAttachmentHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageHandleOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attachmentNameOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attachmentNameLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND
void *CsrBtBipcAaGetLinkedAttachmentHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetLinkedAttachmentHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetLinkedAttachmentHeaderInd *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetLinkedAttachmentHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageHandleOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attachmentNameOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attachmentNameLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND
void CsrBtBipcAaGetLinkedAttachmentHeaderIndSerFree(void *msg)
{
    CsrBtBipcAaGetLinkedAttachmentHeaderInd *primitive;

    primitive = (CsrBtBipcAaGetLinkedAttachmentHeaderInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_ATTACHMENT_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES
CsrSize CsrBtBipcAaGetImagePropertiesObjectResSizeof(void *msg)
{
    CsrBtBipcAaGetImagePropertiesObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->propertiesObjectLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES
CsrUint8 *CsrBtBipcAaGetImagePropertiesObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetImagePropertiesObjectRes *primitive;

    primitive = (CsrBtBipcAaGetImagePropertiesObjectRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES
void *CsrBtBipcAaGetImagePropertiesObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetImagePropertiesObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetImagePropertiesObjectRes *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetImagePropertiesObjectRes));

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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES
void CsrBtBipcAaGetImagePropertiesObjectResSerFree(void *msg)
{
    CsrBtBipcAaGetImagePropertiesObjectRes *primitive;

    primitive = (CsrBtBipcAaGetImagePropertiesObjectRes *) msg;

    if(primitive->propertiesObject != NULL)
    {
        CsrPmemFree(primitive->propertiesObject);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_IND
CsrSize CsrBtBipcAaGetImageObjectIndSizeof(void *msg)
{
    CsrBtBipcAaGetImageObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_IND
CsrUint8 *CsrBtBipcAaGetImageObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetImageObjectInd *primitive;

    primitive = (CsrBtBipcAaGetImageObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->allowedObjectLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_IND
void *CsrBtBipcAaGetImageObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetImageObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetImageObjectInd *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetImageObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->allowedObjectLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND
CsrSize CsrBtBipcRcGetMonitoringImageFileIndSizeof(void *msg)
{
    CsrBtBipcRcGetMonitoringImageFileInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND
CsrUint8 *CsrBtBipcRcGetMonitoringImageFileIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetMonitoringImageFileInd *primitive;

    primitive = (CsrBtBipcRcGetMonitoringImageFileInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->imageFileOffset);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->imageFileLength);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND
void *CsrBtBipcRcGetMonitoringImageFileIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetMonitoringImageFileInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetMonitoringImageFileInd *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetMonitoringImageFileInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->imageFileOffset, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->imageFileLength, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->payloadLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND
void CsrBtBipcRcGetMonitoringImageFileIndSerFree(void *msg)
{
    CsrBtBipcRcGetMonitoringImageFileInd *primitive;

    primitive = (CsrBtBipcRcGetMonitoringImageFileInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_IND
CsrSize CsrBtBipcAaGetCapabilitiesObjectIndSizeof(void *msg)
{
    CsrBtBipcAaGetCapabilitiesObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_IND
CsrUint8 *CsrBtBipcAaGetCapabilitiesObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetCapabilitiesObjectInd *primitive;

    primitive = (CsrBtBipcAaGetCapabilitiesObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_IND
void *CsrBtBipcAaGetCapabilitiesObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetCapabilitiesObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetCapabilitiesObjectInd *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetCapabilitiesObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_DISCONNECT_IND
CsrSize CsrBtBipcDisconnectIndSizeof(void *msg)
{
    CsrBtBipcDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_DISCONNECT_IND
CsrUint8 *CsrBtBipcDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcDisconnectInd *primitive;

    primitive = (CsrBtBipcDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_DISCONNECT_IND
void *CsrBtBipcDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtBipcDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_CFM
CsrSize CsrBtBipcPushPutAttachmentCfmSizeof(void *msg)
{
    CsrBtBipcPushPutAttachmentCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_CFM
CsrUint8 *CsrBtBipcPushPutAttachmentCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcPushPutAttachmentCfm *primitive;

    primitive = (CsrBtBipcPushPutAttachmentCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_CFM
void *CsrBtBipcPushPutAttachmentCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcPushPutAttachmentCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcPushPutAttachmentCfm *) CsrPmemZalloc(sizeof(CsrBtBipcPushPutAttachmentCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_ATTACHMENT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND
CsrSize CsrBtBipcRcGetLinkedThumbnailIndSizeof(void *msg)
{
    CsrBtBipcRcGetLinkedThumbnailInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND
CsrUint8 *CsrBtBipcRcGetLinkedThumbnailIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetLinkedThumbnailInd *primitive;

    primitive = (CsrBtBipcRcGetLinkedThumbnailInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND
void *CsrBtBipcRcGetLinkedThumbnailIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetLinkedThumbnailInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetLinkedThumbnailInd *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetLinkedThumbnailInd));

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
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND
void CsrBtBipcRcGetLinkedThumbnailIndSerFree(void *msg)
{
    CsrBtBipcRcGetLinkedThumbnailInd *primitive;

    primitive = (CsrBtBipcRcGetLinkedThumbnailInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_REQ
CsrSize CsrBtBipcSecurityOutReqSizeof(void *msg)
{
    CsrBtBipcSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_REQ
CsrUint8 *CsrBtBipcSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcSecurityOutReq *primitive;

    primitive = (CsrBtBipcSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_REQ
void *CsrBtBipcSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtBipcSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_IND
CsrSize CsrBtBipcAaGetImagePropertiesObjectIndSizeof(void *msg)
{
    CsrBtBipcAaGetImagePropertiesObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_IND
CsrUint8 *CsrBtBipcAaGetImagePropertiesObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetImagePropertiesObjectInd *primitive;

    primitive = (CsrBtBipcAaGetImagePropertiesObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->allowedImageLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_IND
void *CsrBtBipcAaGetImagePropertiesObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetImagePropertiesObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetImagePropertiesObjectInd *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetImagePropertiesObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->allowedImageLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND
CsrSize CsrBtBipcAaGetLinkedThumbnailHeaderIndSizeof(void *msg)
{
    CsrBtBipcAaGetLinkedThumbnailHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND
CsrUint8 *CsrBtBipcAaGetLinkedThumbnailHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetLinkedThumbnailHeaderInd *primitive;

    primitive = (CsrBtBipcAaGetLinkedThumbnailHeaderInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND
void *CsrBtBipcAaGetLinkedThumbnailHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetLinkedThumbnailHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetLinkedThumbnailHeaderInd *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetLinkedThumbnailHeaderInd));

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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND
void CsrBtBipcAaGetLinkedThumbnailHeaderIndSerFree(void *msg)
{
    CsrBtBipcAaGetLinkedThumbnailHeaderInd *primitive;

    primitive = (CsrBtBipcAaGetLinkedThumbnailHeaderInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_DISCONNECT_REQ
CsrSize CsrBtBipcDisconnectReqSizeof(void *msg)
{
    CsrBtBipcDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_DISCONNECT_REQ
CsrUint8 *CsrBtBipcDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcDisconnectReq *primitive;

    primitive = (CsrBtBipcDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->normalDisconnect);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_DISCONNECT_REQ
void *CsrBtBipcDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtBipcDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->normalDisconnect, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_RES
CsrSize CsrBtBipcAaGetImagePropertiesHeaderResSizeof(void *msg)
{
    CsrBtBipcAaGetImagePropertiesHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_RES
CsrUint8 *CsrBtBipcAaGetImagePropertiesHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetImagePropertiesHeaderRes *primitive;

    primitive = (CsrBtBipcAaGetImagePropertiesHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_RES
void *CsrBtBipcAaGetImagePropertiesHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetImagePropertiesHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetImagePropertiesHeaderRes *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetImagePropertiesHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES
CsrSize CsrBtBipcAaGetCapabilitiesObjectResSizeof(void *msg)
{
    CsrBtBipcAaGetCapabilitiesObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->capabilitiesObjectLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES
CsrUint8 *CsrBtBipcAaGetCapabilitiesObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetCapabilitiesObjectRes *primitive;

    primitive = (CsrBtBipcAaGetCapabilitiesObjectRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES
void *CsrBtBipcAaGetCapabilitiesObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetCapabilitiesObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetCapabilitiesObjectRes *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetCapabilitiesObjectRes));

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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES
void CsrBtBipcAaGetCapabilitiesObjectResSerFree(void *msg)
{
    CsrBtBipcAaGetCapabilitiesObjectRes *primitive;

    primitive = (CsrBtBipcAaGetCapabilitiesObjectRes *) msg;

    if(primitive->capabilitiesObject != NULL)
    {
        CsrPmemFree(primitive->capabilitiesObject);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_REQ
CsrSize CsrBtBipcRcGetImagePropertiesReqSizeof(void *msg)
{
    CsrBtBipcRcGetImagePropertiesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_REQ
CsrUint8 *CsrBtBipcRcGetImagePropertiesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetImagePropertiesReq *primitive;

    primitive = (CsrBtBipcRcGetImagePropertiesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->imageHandle), ((CsrSize) (8)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_REQ
void *CsrBtBipcRcGetImagePropertiesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetImagePropertiesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetImagePropertiesReq *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetImagePropertiesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->imageHandle), buffer, &offset, ((CsrSize) (8)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_REQ
CsrSize CsrBtBipcGetRemoteFeaturesReqSizeof(void *msg)
{
    CsrBtBipcGetRemoteFeaturesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_REQ
CsrUint8 *CsrBtBipcGetRemoteFeaturesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcGetRemoteFeaturesReq *primitive;

    primitive = (CsrBtBipcGetRemoteFeaturesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_REQ
void *CsrBtBipcGetRemoteFeaturesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcGetRemoteFeaturesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcGetRemoteFeaturesReq *) CsrPmemZalloc(sizeof(CsrBtBipcGetRemoteFeaturesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_GET_REMOTE_FEATURES_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND
CsrSize CsrBtBipcAaGetImagePropertiesHeaderIndSizeof(void *msg)
{
    CsrBtBipcAaGetImagePropertiesHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND
CsrUint8 *CsrBtBipcAaGetImagePropertiesHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetImagePropertiesHeaderInd *primitive;

    primitive = (CsrBtBipcAaGetImagePropertiesHeaderInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND
void *CsrBtBipcAaGetImagePropertiesHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetImagePropertiesHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetImagePropertiesHeaderInd *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetImagePropertiesHeaderInd));

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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND
void CsrBtBipcAaGetImagePropertiesHeaderIndSerFree(void *msg)
{
    CsrBtBipcAaGetImagePropertiesHeaderInd *primitive;

    primitive = (CsrBtBipcAaGetImagePropertiesHeaderInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_PROPERTIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_RES
CsrSize CsrBtBipcRcGetImagePropertiesResSizeof(void *msg)
{
    CsrBtBipcRcGetImagePropertiesRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_RES
CsrUint8 *CsrBtBipcRcGetImagePropertiesResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetImagePropertiesRes *primitive;

    primitive = (CsrBtBipcRcGetImagePropertiesRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_RES
void *CsrBtBipcRcGetImagePropertiesResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetImagePropertiesRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetImagePropertiesRes *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetImagePropertiesRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_PROPERTIES_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_RES
CsrSize CsrBtBipcAaGetImageHeaderResSizeof(void *msg)
{
    CsrBtBipcAaGetImageHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_RES
CsrUint8 *CsrBtBipcAaGetImageHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetImageHeaderRes *primitive;

    primitive = (CsrBtBipcAaGetImageHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->imageTotalLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_RES
void *CsrBtBipcAaGetImageHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetImageHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetImageHeaderRes *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetImageHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->imageTotalLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND
CsrSize CsrBtBipcPushGetCapabilitiesIndSizeof(void *msg)
{
    CsrBtBipcPushGetCapabilitiesInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND
CsrUint8 *CsrBtBipcPushGetCapabilitiesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcPushGetCapabilitiesInd *primitive;

    primitive = (CsrBtBipcPushGetCapabilitiesInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND
void *CsrBtBipcPushGetCapabilitiesIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcPushGetCapabilitiesInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcPushGetCapabilitiesInd *) CsrPmemZalloc(sizeof(CsrBtBipcPushGetCapabilitiesInd));

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
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND
void CsrBtBipcPushGetCapabilitiesIndSerFree(void *msg)
{
    CsrBtBipcPushGetCapabilitiesInd *primitive;

    primitive = (CsrBtBipcPushGetCapabilitiesInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_RES
CsrSize CsrBtBipcRcGetMonitoringImageFileResSizeof(void *msg)
{
    CsrBtBipcRcGetMonitoringImageFileRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_RES
CsrUint8 *CsrBtBipcRcGetMonitoringImageFileResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetMonitoringImageFileRes *primitive;

    primitive = (CsrBtBipcRcGetMonitoringImageFileRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_RES
void *CsrBtBipcRcGetMonitoringImageFileResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetMonitoringImageFileRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetMonitoringImageFileRes *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetMonitoringImageFileRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_MONITORING_IMAGE_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES
CsrSize CsrBtBipcAaGetImageListHeaderResSizeof(void *msg)
{
    CsrBtBipcAaGetImageListHeaderRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->imageDescriptorLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES
CsrUint8 *CsrBtBipcAaGetImageListHeaderResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetImageListHeaderRes *primitive;

    primitive = (CsrBtBipcAaGetImageListHeaderRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->nbReturnedHandles);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageDescriptorLength);
    CsrMemCpySer(buffer, length, &primitive->imageDescriptor, sizeof(primitive->imageDescriptor));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->imageDescriptor)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->imageDescriptor), ((CsrSize) (primitive->imageDescriptorLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES
void *CsrBtBipcAaGetImageListHeaderResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetImageListHeaderRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetImageListHeaderRes *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetImageListHeaderRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->nbReturnedHandles, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES
void CsrBtBipcAaGetImageListHeaderResSerFree(void *msg)
{
    CsrBtBipcAaGetImageListHeaderRes *primitive;

    primitive = (CsrBtBipcAaGetImageListHeaderRes *) msg;

    if(primitive->imageDescriptor != NULL)
    {
        CsrPmemFree(primitive->imageDescriptor);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_IND
CsrSize CsrBtBipcAaDeleteImageIndSizeof(void *msg)
{
    CsrBtBipcAaDeleteImageInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_IND
CsrUint8 *CsrBtBipcAaDeleteImageIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaDeleteImageInd *primitive;

    primitive = (CsrBtBipcAaDeleteImageInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_IND
void *CsrBtBipcAaDeleteImageIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaDeleteImageInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaDeleteImageInd *) CsrPmemZalloc(sizeof(CsrBtBipcAaDeleteImageInd));

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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_IND
void CsrBtBipcAaDeleteImageIndSerFree(void *msg)
{
    CsrBtBipcAaDeleteImageInd *primitive;

    primitive = (CsrBtBipcAaDeleteImageInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_IND
CsrSize CsrBtBipcRcGetImageIndSizeof(void *msg)
{
    CsrBtBipcRcGetImageInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_IND
CsrUint8 *CsrBtBipcRcGetImageIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetImageInd *primitive;

    primitive = (CsrBtBipcRcGetImageInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_IND
void *CsrBtBipcRcGetImageIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetImageInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetImageInd *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetImageInd));

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
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_IND
void CsrBtBipcRcGetImageIndSerFree(void *msg)
{
    CsrBtBipcRcGetImageInd *primitive;

    primitive = (CsrBtBipcRcGetImageInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_IMAGE_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES
CsrSize CsrBtBipcAaGetImageListObjectResSizeof(void *msg)
{
    CsrBtBipcAaGetImageListObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->imagesListObjectLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES
CsrUint8 *CsrBtBipcAaGetImageListObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetImageListObjectRes *primitive;

    primitive = (CsrBtBipcAaGetImageListObjectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imagesListObjectLength);
    CsrMemCpySer(buffer, length, &primitive->imagesListObject, sizeof(primitive->imagesListObject));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->imagesListObject)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->imagesListObject), ((CsrSize) (primitive->imagesListObjectLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES
void *CsrBtBipcAaGetImageListObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetImageListObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetImageListObjectRes *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetImageListObjectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imagesListObjectLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->imagesListObject), buffer, &offset, ((CsrSize) sizeof(primitive->imagesListObject)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->imagesListObjectLength)
    {
        primitive->imagesListObject = CsrPmemZalloc(((CsrUint32) (primitive->imagesListObjectLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->imagesListObject), buffer, &offset, ((CsrSize) (primitive->imagesListObjectLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->imagesListObject = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES
void CsrBtBipcAaGetImageListObjectResSerFree(void *msg)
{
    CsrBtBipcAaGetImageListObjectRes *primitive;

    primitive = (CsrBtBipcAaGetImageListObjectRes *) msg;

    if(primitive->imagesListObject != NULL)
    {
        CsrPmemFree(primitive->imagesListObject);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES
CsrSize CsrBtBipcPushPutImageFileResSizeof(void *msg)
{
    CsrBtBipcPushPutImageFileRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->imageFileLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES
CsrUint8 *CsrBtBipcPushPutImageFileResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcPushPutImageFileRes *primitive;

    primitive = (CsrBtBipcPushPutImageFileRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageFileLength);
    CsrMemCpySer(buffer, length, &primitive->imageFile, sizeof(primitive->imageFile));
    if(primitive->imageFile)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->imageFile), ((CsrSize) (primitive->imageFileLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES
void *CsrBtBipcPushPutImageFileResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcPushPutImageFileRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcPushPutImageFileRes *) CsrPmemZalloc(sizeof(CsrBtBipcPushPutImageFileRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageFileLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->imageFile), buffer, &offset, ((CsrSize) sizeof(primitive->imageFile)));
    if (primitive->imageFileLength)
    {
        primitive->imageFile = CsrPmemZalloc(((CsrUint32) (primitive->imageFileLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->imageFile), buffer, &offset, ((CsrSize) (primitive->imageFileLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->imageFile = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES
void CsrBtBipcPushPutImageFileResSerFree(void *msg)
{
    CsrBtBipcPushPutImageFileRes *primitive;

    primitive = (CsrBtBipcPushPutImageFileRes *) msg;

    if(primitive->imageFile != NULL)
    {
        CsrPmemFree(primitive->imageFile);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_CONNECT_CFM
CsrSize CsrBtBipcConnectCfmSizeof(void *msg)
{
    CsrBtBipcConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_CONNECT_CFM
CsrUint8 *CsrBtBipcConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcConnectCfm *primitive;

    primitive = (CsrBtBipcConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->supportedFunctions);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexPeerMaxPacketSize);
    CsrMemCpySer(buffer, length, ((const void *) primitive->totalImagingDataCapacity), ((CsrSize) (8)));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_CONNECT_CFM
void *CsrBtBipcConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcConnectCfm *) CsrPmemZalloc(sizeof(CsrBtBipcConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->supportedFunctions, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexPeerMaxPacketSize, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->totalImagingDataCapacity), buffer, &offset, ((CsrSize) (8)));
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_CFM
CsrSize CsrBtBipcPushPutImageCfmSizeof(void *msg)
{
    CsrBtBipcPushPutImageCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_CFM
CsrUint8 *CsrBtBipcPushPutImageCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcPushPutImageCfm *primitive;

    primitive = (CsrBtBipcPushPutImageCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrMemCpySer(buffer, length, ((const void *) primitive->imageHandle), ((CsrSize) (8)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_CFM
void *CsrBtBipcPushPutImageCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcPushPutImageCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcPushPutImageCfm *) CsrPmemZalloc(sizeof(CsrBtBipcPushPutImageCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->imageHandle), buffer, &offset, ((CsrSize) (8)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_IMAGE_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_IND
CsrSize CsrBtBipcAaGetCapabilitiesHeaderIndSizeof(void *msg)
{
    CsrBtBipcAaGetCapabilitiesHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_IND
CsrUint8 *CsrBtBipcAaGetCapabilitiesHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetCapabilitiesHeaderInd *primitive;

    primitive = (CsrBtBipcAaGetCapabilitiesHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_IND
void *CsrBtBipcAaGetCapabilitiesHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetCapabilitiesHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetCapabilitiesHeaderInd *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetCapabilitiesHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_CAPABILITIES_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND
CsrSize CsrBtBipcAaGetImageListHeaderIndSizeof(void *msg)
{
    CsrBtBipcAaGetImageListHeaderInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND
CsrUint8 *CsrBtBipcAaGetImageListHeaderIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetImageListHeaderInd *primitive;

    primitive = (CsrBtBipcAaGetImageListHeaderInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->nbReturnedHandles);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->listStartOffset);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->latestCapturedImages);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->allowedDescriptorLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageDescriptorOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->imageDescriptorLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND
void *CsrBtBipcAaGetImageListHeaderIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetImageListHeaderInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetImageListHeaderInd *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetImageListHeaderInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->nbReturnedHandles, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->listStartOffset, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->latestCapturedImages, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->allowedDescriptorLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageDescriptorOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->imageDescriptorLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND
void CsrBtBipcAaGetImageListHeaderIndSerFree(void *msg)
{
    CsrBtBipcAaGetImageListHeaderInd *primitive;

    primitive = (CsrBtBipcAaGetImageListHeaderInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_IMAGE_LIST_HEADER_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES
CsrSize CsrBtBipcPushPutThumbnailFileResSizeof(void *msg)
{
    CsrBtBipcPushPutThumbnailFileRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->thumbnailFileLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES
CsrUint8 *CsrBtBipcPushPutThumbnailFileResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcPushPutThumbnailFileRes *primitive;

    primitive = (CsrBtBipcPushPutThumbnailFileRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->thumbnailFileLength);
    CsrMemCpySer(buffer, length, &primitive->thumbnailFile, sizeof(primitive->thumbnailFile));
    if(primitive->thumbnailFile)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->thumbnailFile), ((CsrSize) (primitive->thumbnailFileLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES
void *CsrBtBipcPushPutThumbnailFileResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcPushPutThumbnailFileRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcPushPutThumbnailFileRes *) CsrPmemZalloc(sizeof(CsrBtBipcPushPutThumbnailFileRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->thumbnailFileLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->thumbnailFile), buffer, &offset, ((CsrSize) sizeof(primitive->thumbnailFile)));
    if (primitive->thumbnailFileLength)
    {
        primitive->thumbnailFile = CsrPmemZalloc(((CsrUint32) (primitive->thumbnailFileLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->thumbnailFile), buffer, &offset, ((CsrSize) (primitive->thumbnailFileLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->thumbnailFile = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES
void CsrBtBipcPushPutThumbnailFileResSerFree(void *msg)
{
    CsrBtBipcPushPutThumbnailFileRes *primitive;

    primitive = (CsrBtBipcPushPutThumbnailFileRes *) msg;

    if(primitive->thumbnailFile != NULL)
    {
        CsrPmemFree(primitive->thumbnailFile);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_PUT_THUMBNAIL_FILE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM
CsrSize CsrBtBipcPushGetCapabilitiesCfmSizeof(void *msg)
{
    CsrBtBipcPushGetCapabilitiesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM
CsrUint8 *CsrBtBipcPushGetCapabilitiesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcPushGetCapabilitiesCfm *primitive;

    primitive = (CsrBtBipcPushGetCapabilitiesCfm *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM
void *CsrBtBipcPushGetCapabilitiesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcPushGetCapabilitiesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcPushGetCapabilitiesCfm *) CsrPmemZalloc(sizeof(CsrBtBipcPushGetCapabilitiesCfm));

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
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM
void CsrBtBipcPushGetCapabilitiesCfmSerFree(void *msg)
{
    CsrBtBipcPushGetCapabilitiesCfm *primitive;

    primitive = (CsrBtBipcPushGetCapabilitiesCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_PUSH_GET_CAPABILITIES_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_RES
CsrSize CsrBtBipcAaDeleteImageResSizeof(void *msg)
{
    CsrBtBipcAaDeleteImageRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_RES
CsrUint8 *CsrBtBipcAaDeleteImageResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaDeleteImageRes *primitive;

    primitive = (CsrBtBipcAaDeleteImageRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_RES
void *CsrBtBipcAaDeleteImageResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaDeleteImageRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaDeleteImageRes *) CsrPmemZalloc(sizeof(CsrBtBipcAaDeleteImageRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_DELETE_IMAGE_RES */

#ifndef EXCLUDE_CSR_BT_BIPC_ABORT_REQ
CsrSize CsrBtBipcAbortReqSizeof(void *msg)
{
    CsrBtBipcAbortReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_ABORT_REQ
CsrUint8 *CsrBtBipcAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAbortReq *primitive;

    primitive = (CsrBtBipcAbortReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_ABORT_REQ
void *CsrBtBipcAbortReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAbortReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAbortReq *) CsrPmemZalloc(sizeof(CsrBtBipcAbortReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM
CsrSize CsrBtBipcRcGetLinkedThumbnailCfmSizeof(void *msg)
{
    CsrBtBipcRcGetLinkedThumbnailCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM
CsrUint8 *CsrBtBipcRcGetLinkedThumbnailCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcRcGetLinkedThumbnailCfm *primitive;

    primitive = (CsrBtBipcRcGetLinkedThumbnailCfm *) msg;
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
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM
void *CsrBtBipcRcGetLinkedThumbnailCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcRcGetLinkedThumbnailCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcRcGetLinkedThumbnailCfm *) CsrPmemZalloc(sizeof(CsrBtBipcRcGetLinkedThumbnailCfm));

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
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM
void CsrBtBipcRcGetLinkedThumbnailCfmSerFree(void *msg)
{
    CsrBtBipcRcGetLinkedThumbnailCfm *primitive;

    primitive = (CsrBtBipcRcGetLinkedThumbnailCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BIPC_RC_GET_LINKED_THUMBNAIL_CFM */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_ABORT_IND
CsrSize CsrBtBipcAaAbortIndSizeof(void *msg)
{
    CsrBtBipcAaAbortInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_ABORT_IND
CsrUint8 *CsrBtBipcAaAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaAbortInd *primitive;

    primitive = (CsrBtBipcAaAbortInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_ABORT_IND
void *CsrBtBipcAaAbortIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaAbortInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaAbortInd *) CsrPmemZalloc(sizeof(CsrBtBipcAaAbortInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_IND
CsrSize CsrBtBipcAaGetLinkedThumbnailObjectIndSizeof(void *msg)
{
    CsrBtBipcAaGetLinkedThumbnailObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_IND
CsrUint8 *CsrBtBipcAaGetLinkedThumbnailObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBipcAaGetLinkedThumbnailObjectInd *primitive;

    primitive = (CsrBtBipcAaGetLinkedThumbnailObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pHandleInst);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->allowedObjectLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_IND
void *CsrBtBipcAaGetLinkedThumbnailObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBipcAaGetLinkedThumbnailObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBipcAaGetLinkedThumbnailObjectInd *) CsrPmemZalloc(sizeof(CsrBtBipcAaGetLinkedThumbnailObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pHandleInst, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->allowedObjectLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BIPC_AA_GET_LINKED_THUMBNAIL_OBJECT_IND */

#endif /* EXCLUDE_CSR_BT_BIPC_MODULE */
