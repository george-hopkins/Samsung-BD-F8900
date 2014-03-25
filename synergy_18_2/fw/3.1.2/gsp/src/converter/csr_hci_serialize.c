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
#include "csr_hci_private_prim.h"

#ifndef EXCLUDE_CSR_HCI_PRIVATE_MODULE

void CsrHciPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_CFM
CsrSize CsrHciRegisterEventHandlerCfmSizeof(void *msg)
{
    CsrHciRegisterEventHandlerCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_CFM
CsrUint8 *CsrHciRegisterEventHandlerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciRegisterEventHandlerCfm *primitive;

    primitive = (CsrHciRegisterEventHandlerCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_CFM
void *CsrHciRegisterEventHandlerCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciRegisterEventHandlerCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciRegisterEventHandlerCfm *) CsrPmemZalloc(sizeof(CsrHciRegisterEventHandlerCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_CFM
CsrSize CsrHciRegisterAclHandlerCfmSizeof(void *msg)
{
    CsrHciRegisterAclHandlerCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_CFM
CsrUint8 *CsrHciRegisterAclHandlerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciRegisterAclHandlerCfm *primitive;

    primitive = (CsrHciRegisterAclHandlerCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->aclHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_CFM
void *CsrHciRegisterAclHandlerCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciRegisterAclHandlerCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciRegisterAclHandlerCfm *) CsrPmemZalloc(sizeof(CsrHciRegisterAclHandlerCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->aclHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_SCO_DATA_IND
CsrSize CsrHciScoDataIndSizeof(void *msg)
{
    CsrHciScoDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_SCO_DATA_IND */

#ifndef EXCLUDE_CSR_HCI_SCO_DATA_IND
CsrUint8 *CsrHciScoDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciScoDataInd *primitive;

    primitive = (CsrHciScoDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handlePlusFlags);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if (primitive->data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->data, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_SCO_DATA_IND */

#ifndef EXCLUDE_CSR_HCI_SCO_DATA_IND
void *CsrHciScoDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciScoDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciScoDataInd *) CsrPmemZalloc(sizeof(CsrHciScoDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handlePlusFlags, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_SCO_DATA_IND */

#ifndef EXCLUDE_CSR_HCI_SCO_DATA_IND
void CsrHciScoDataIndSerFree(void *msg)
{
    CsrHciScoDataInd *primitive;

    primitive = (CsrHciScoDataInd *) msg;

    if(primitive->data != NULL)
    {
        CsrMblkDestroy(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_HCI_SCO_DATA_IND */

#ifndef EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_EVENT_IND
CsrSize CsrHciVendorSpecificEventIndSizeof(void *msg)
{
    CsrHciVendorSpecificEventInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_EVENT_IND */

#ifndef EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_EVENT_IND
CsrUint8 *CsrHciVendorSpecificEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciVendorSpecificEventInd *primitive;

    primitive = (CsrHciVendorSpecificEventInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->channel);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if (primitive->data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->data, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_EVENT_IND */

#ifndef EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_EVENT_IND
void *CsrHciVendorSpecificEventIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciVendorSpecificEventInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciVendorSpecificEventInd *) CsrPmemZalloc(sizeof(CsrHciVendorSpecificEventInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->channel, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_EVENT_IND */

#ifndef EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_EVENT_IND
void CsrHciVendorSpecificEventIndSerFree(void *msg)
{
    CsrHciVendorSpecificEventInd *primitive;

    primitive = (CsrHciVendorSpecificEventInd *) msg;

    if(primitive->data != NULL)
    {
        CsrMblkDestroy(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_EVENT_IND */

#ifndef EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ
CsrSize CsrHciRegisterVendorSpecificEventHandlerReqSizeof(void *msg)
{
    CsrHciRegisterVendorSpecificEventHandlerReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ
CsrUint8 *CsrHciRegisterVendorSpecificEventHandlerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciRegisterVendorSpecificEventHandlerReq *primitive;

    primitive = (CsrHciRegisterVendorSpecificEventHandlerReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->channel);

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ
void *CsrHciRegisterVendorSpecificEventHandlerReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciRegisterVendorSpecificEventHandlerReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciRegisterVendorSpecificEventHandlerReq *) CsrPmemZalloc(sizeof(CsrHciRegisterVendorSpecificEventHandlerReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->channel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_SCO_DATA_REQ
CsrSize CsrHciScoDataReqSizeof(void *msg)
{
    CsrHciScoDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_SCO_DATA_REQ */

#ifndef EXCLUDE_CSR_HCI_SCO_DATA_REQ
CsrUint8 *CsrHciScoDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciScoDataReq *primitive;

    primitive = (CsrHciScoDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handlePlusFlags);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if (primitive->data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->data, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_SCO_DATA_REQ */

#ifndef EXCLUDE_CSR_HCI_SCO_DATA_REQ
void *CsrHciScoDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciScoDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciScoDataReq *) CsrPmemZalloc(sizeof(CsrHciScoDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handlePlusFlags, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_SCO_DATA_REQ */

#ifndef EXCLUDE_CSR_HCI_SCO_DATA_REQ
void CsrHciScoDataReqSerFree(void *msg)
{
    CsrHciScoDataReq *primitive;

    primitive = (CsrHciScoDataReq *) msg;

    if(primitive->data != NULL)
    {
        CsrMblkDestroy(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_HCI_SCO_DATA_REQ */

#ifndef EXCLUDE_CSR_HCI_ACL_DATA_REQ
CsrSize CsrHciAclDataReqSizeof(void *msg)
{
    CsrHciAclDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_ACL_DATA_REQ */

#ifndef EXCLUDE_CSR_HCI_ACL_DATA_REQ
CsrUint8 *CsrHciAclDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciAclDataReq *primitive;

    primitive = (CsrHciAclDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handlePlusFlags);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if (primitive->data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->data, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_ACL_DATA_REQ */

#ifndef EXCLUDE_CSR_HCI_ACL_DATA_REQ
void *CsrHciAclDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciAclDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciAclDataReq *) CsrPmemZalloc(sizeof(CsrHciAclDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handlePlusFlags, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_ACL_DATA_REQ */

#ifndef EXCLUDE_CSR_HCI_ACL_DATA_REQ
void CsrHciAclDataReqSerFree(void *msg)
{
    CsrHciAclDataReq *primitive;

    primitive = (CsrHciAclDataReq *) msg;

    if(primitive->data != NULL)
    {
        CsrMblkDestroy(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_HCI_ACL_DATA_REQ */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ
CsrSize CsrHciUnregisterVendorSpecificEventHandlerReqSizeof(void *msg)
{
    CsrHciUnregisterVendorSpecificEventHandlerReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ
CsrUint8 *CsrHciUnregisterVendorSpecificEventHandlerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciUnregisterVendorSpecificEventHandlerReq *primitive;

    primitive = (CsrHciUnregisterVendorSpecificEventHandlerReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->channel);

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ
void *CsrHciUnregisterVendorSpecificEventHandlerReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciUnregisterVendorSpecificEventHandlerReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciUnregisterVendorSpecificEventHandlerReq *) CsrPmemZalloc(sizeof(CsrHciUnregisterVendorSpecificEventHandlerReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->channel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_CFM
CsrSize CsrHciUnregisterAclHandlerCfmSizeof(void *msg)
{
    CsrHciUnregisterAclHandlerCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_CFM
CsrUint8 *CsrHciUnregisterAclHandlerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciUnregisterAclHandlerCfm *primitive;

    primitive = (CsrHciUnregisterAclHandlerCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->aclHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_CFM
void *CsrHciUnregisterAclHandlerCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciUnregisterAclHandlerCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciUnregisterAclHandlerCfm *) CsrPmemZalloc(sizeof(CsrHciUnregisterAclHandlerCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->aclHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_COMMAND_REQ
CsrSize CsrHciCommandReqSizeof(void *msg)
{
    CsrHciCommandReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_COMMAND_REQ */

#ifndef EXCLUDE_CSR_HCI_COMMAND_REQ
CsrUint8 *CsrHciCommandReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciCommandReq *primitive;

    primitive = (CsrHciCommandReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_COMMAND_REQ */

#ifndef EXCLUDE_CSR_HCI_COMMAND_REQ
void *CsrHciCommandReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciCommandReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciCommandReq *) CsrPmemZalloc(sizeof(CsrHciCommandReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
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
#endif /* EXCLUDE_CSR_HCI_COMMAND_REQ */

#ifndef EXCLUDE_CSR_HCI_COMMAND_REQ
void CsrHciCommandReqSerFree(void *msg)
{
    CsrHciCommandReq *primitive;

    primitive = (CsrHciCommandReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_HCI_COMMAND_REQ */

#ifndef EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_REQ
CsrSize CsrHciRegisterEventHandlerReqSizeof(void *msg)
{
    CsrHciRegisterEventHandlerReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_REQ
CsrUint8 *CsrHciRegisterEventHandlerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciRegisterEventHandlerReq *primitive;

    primitive = (CsrHciRegisterEventHandlerReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_REQ
void *CsrHciRegisterEventHandlerReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciRegisterEventHandlerReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciRegisterEventHandlerReq *) CsrPmemZalloc(sizeof(CsrHciRegisterEventHandlerReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_EVENT_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_ACL_DATA_IND
CsrSize CsrHciAclDataIndSizeof(void *msg)
{
    CsrHciAclDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_ACL_DATA_IND */

#ifndef EXCLUDE_CSR_HCI_ACL_DATA_IND
CsrUint8 *CsrHciAclDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciAclDataInd *primitive;

    primitive = (CsrHciAclDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handlePlusFlags);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if (primitive->data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->data, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_ACL_DATA_IND */

#ifndef EXCLUDE_CSR_HCI_ACL_DATA_IND
void *CsrHciAclDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciAclDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciAclDataInd *) CsrPmemZalloc(sizeof(CsrHciAclDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handlePlusFlags, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_ACL_DATA_IND */

#ifndef EXCLUDE_CSR_HCI_ACL_DATA_IND
void CsrHciAclDataIndSerFree(void *msg)
{
    CsrHciAclDataInd *primitive;

    primitive = (CsrHciAclDataInd *) msg;

    if(primitive->data != NULL)
    {
        CsrMblkDestroy(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_HCI_ACL_DATA_IND */

#ifndef EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_CFM
CsrSize CsrHciRegisterScoHandlerCfmSizeof(void *msg)
{
    CsrHciRegisterScoHandlerCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_CFM
CsrUint8 *CsrHciRegisterScoHandlerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciRegisterScoHandlerCfm *primitive;

    primitive = (CsrHciRegisterScoHandlerCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_CFM
void *CsrHciRegisterScoHandlerCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciRegisterScoHandlerCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciRegisterScoHandlerCfm *) CsrPmemZalloc(sizeof(CsrHciRegisterScoHandlerCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_EVENT_IND
CsrSize CsrHciEventIndSizeof(void *msg)
{
    CsrHciEventInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_EVENT_IND */

#ifndef EXCLUDE_CSR_HCI_EVENT_IND
CsrUint8 *CsrHciEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciEventInd *primitive;

    primitive = (CsrHciEventInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_EVENT_IND */

#ifndef EXCLUDE_CSR_HCI_EVENT_IND
void *CsrHciEventIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciEventInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciEventInd *) CsrPmemZalloc(sizeof(CsrHciEventInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
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
#endif /* EXCLUDE_CSR_HCI_EVENT_IND */

#ifndef EXCLUDE_CSR_HCI_EVENT_IND
void CsrHciEventIndSerFree(void *msg)
{
    CsrHciEventInd *primitive;

    primitive = (CsrHciEventInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_HCI_EVENT_IND */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_REQ
CsrSize CsrHciUnregisterScoHandlerReqSizeof(void *msg)
{
    CsrHciUnregisterScoHandlerReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_REQ
CsrUint8 *CsrHciUnregisterScoHandlerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciUnregisterScoHandlerReq *primitive;

    primitive = (CsrHciUnregisterScoHandlerReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_REQ
void *CsrHciUnregisterScoHandlerReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciUnregisterScoHandlerReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciUnregisterScoHandlerReq *) CsrPmemZalloc(sizeof(CsrHciUnregisterScoHandlerReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM
CsrSize CsrHciUnregisterVendorSpecificEventHandlerCfmSizeof(void *msg)
{
    CsrHciUnregisterVendorSpecificEventHandlerCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM
CsrUint8 *CsrHciUnregisterVendorSpecificEventHandlerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciUnregisterVendorSpecificEventHandlerCfm *primitive;

    primitive = (CsrHciUnregisterVendorSpecificEventHandlerCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->channel);

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM
void *CsrHciUnregisterVendorSpecificEventHandlerCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciUnregisterVendorSpecificEventHandlerCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciUnregisterVendorSpecificEventHandlerCfm *) CsrPmemZalloc(sizeof(CsrHciUnregisterVendorSpecificEventHandlerCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->channel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_REQ
CsrSize CsrHciRegisterScoHandlerReqSizeof(void *msg)
{
    CsrHciRegisterScoHandlerReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_REQ
CsrUint8 *CsrHciRegisterScoHandlerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciRegisterScoHandlerReq *primitive;

    primitive = (CsrHciRegisterScoHandlerReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_REQ
void *CsrHciRegisterScoHandlerReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciRegisterScoHandlerReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciRegisterScoHandlerReq *) CsrPmemZalloc(sizeof(CsrHciRegisterScoHandlerReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_SCO_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ
CsrSize CsrHciVendorSpecificCommandReqSizeof(void *msg)
{
    CsrHciVendorSpecificCommandReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ */

#ifndef EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ
CsrUint8 *CsrHciVendorSpecificCommandReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciVendorSpecificCommandReq *primitive;

    primitive = (CsrHciVendorSpecificCommandReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->channel);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if (primitive->data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->data, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ */

#ifndef EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ
void *CsrHciVendorSpecificCommandReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciVendorSpecificCommandReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciVendorSpecificCommandReq *) CsrPmemZalloc(sizeof(CsrHciVendorSpecificCommandReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->channel, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ */

#ifndef EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ
void CsrHciVendorSpecificCommandReqSerFree(void *msg)
{
    CsrHciVendorSpecificCommandReq *primitive;

    primitive = (CsrHciVendorSpecificCommandReq *) msg;

    if(primitive->data != NULL)
    {
        CsrMblkDestroy(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_HCI_VENDOR_SPECIFIC_COMMAND_REQ */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_CFM
CsrSize CsrHciUnregisterScoHandlerCfmSizeof(void *msg)
{
    CsrHciUnregisterScoHandlerCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_CFM
CsrUint8 *CsrHciUnregisterScoHandlerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciUnregisterScoHandlerCfm *primitive;

    primitive = (CsrHciUnregisterScoHandlerCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_CFM
void *CsrHciUnregisterScoHandlerCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciUnregisterScoHandlerCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciUnregisterScoHandlerCfm *) CsrPmemZalloc(sizeof(CsrHciUnregisterScoHandlerCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_SCO_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_REQ
CsrSize CsrHciUnregisterAclHandlerReqSizeof(void *msg)
{
    CsrHciUnregisterAclHandlerReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_REQ
CsrUint8 *CsrHciUnregisterAclHandlerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciUnregisterAclHandlerReq *primitive;

    primitive = (CsrHciUnregisterAclHandlerReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->aclHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_REQ
void *CsrHciUnregisterAclHandlerReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciUnregisterAclHandlerReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciUnregisterAclHandlerReq *) CsrPmemZalloc(sizeof(CsrHciUnregisterAclHandlerReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->aclHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_UNREGISTER_ACL_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_INCOMING_DATA_REQ
CsrSize CsrHciIncomingDataReqSizeof(void *msg)
{
    CsrHciIncomingDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->data) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_INCOMING_DATA_REQ */

#ifndef EXCLUDE_CSR_HCI_INCOMING_DATA_REQ
CsrUint8 *CsrHciIncomingDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciIncomingDataReq *primitive;

    primitive = (CsrHciIncomingDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->channel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLen);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if (primitive->data)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->data);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->data, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_INCOMING_DATA_REQ */

#ifndef EXCLUDE_CSR_HCI_INCOMING_DATA_REQ
void *CsrHciIncomingDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciIncomingDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciIncomingDataReq *) CsrPmemZalloc(sizeof(CsrHciIncomingDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->channel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->data = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_INCOMING_DATA_REQ */

#ifndef EXCLUDE_CSR_HCI_INCOMING_DATA_REQ
void CsrHciIncomingDataReqSerFree(void *msg)
{
    CsrHciIncomingDataReq *primitive;

    primitive = (CsrHciIncomingDataReq *) msg;

    if(primitive->data != NULL)
    {
        CsrMblkDestroy(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_HCI_INCOMING_DATA_REQ */

#ifndef EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_REQ
CsrSize CsrHciRegisterAclHandlerReqSizeof(void *msg)
{
    CsrHciRegisterAclHandlerReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_REQ
CsrUint8 *CsrHciRegisterAclHandlerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciRegisterAclHandlerReq *primitive;

    primitive = (CsrHciRegisterAclHandlerReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->queueId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->aclHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_REQ
void *CsrHciRegisterAclHandlerReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciRegisterAclHandlerReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciRegisterAclHandlerReq *) CsrPmemZalloc(sizeof(CsrHciRegisterAclHandlerReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->queueId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->aclHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_ACL_HANDLER_REQ */

#ifndef EXCLUDE_CSR_HCI_NOP_IND
CsrSize CsrHciNopIndSizeof(void *msg)
{
    CsrHciNopInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_NOP_IND */

#ifndef EXCLUDE_CSR_HCI_NOP_IND
CsrUint8 *CsrHciNopIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciNopInd *primitive;

    primitive = (CsrHciNopInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_NOP_IND */

#ifndef EXCLUDE_CSR_HCI_NOP_IND
void *CsrHciNopIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciNopInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciNopInd *) CsrPmemZalloc(sizeof(CsrHciNopInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_HCI_NOP_IND */

#ifndef EXCLUDE_CSR_HCI_NOP_IND
void CsrHciNopIndSerFree(void *msg)
{
    CsrHciNopInd *primitive;

    primitive = (CsrHciNopInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_HCI_NOP_IND */

#ifndef EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM
CsrSize CsrHciRegisterVendorSpecificEventHandlerCfmSizeof(void *msg)
{
    CsrHciRegisterVendorSpecificEventHandlerCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM
CsrUint8 *CsrHciRegisterVendorSpecificEventHandlerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrHciRegisterVendorSpecificEventHandlerCfm *primitive;

    primitive = (CsrHciRegisterVendorSpecificEventHandlerCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->channel);

    return buffer;
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM */

#ifndef EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM
void *CsrHciRegisterVendorSpecificEventHandlerCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrHciRegisterVendorSpecificEventHandlerCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrHciRegisterVendorSpecificEventHandlerCfm *) CsrPmemZalloc(sizeof(CsrHciRegisterVendorSpecificEventHandlerCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->channel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_HCI_REGISTER_VENDOR_SPECIFIC_EVENT_HANDLER_CFM */

#endif /* EXCLUDE_CSR_HCI_PRIVATE_MODULE */
