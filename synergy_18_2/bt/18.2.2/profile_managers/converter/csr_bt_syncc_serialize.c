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
#include "csr_bt_syncc_prim.h"

#ifndef EXCLUDE_CSR_BT_SYNCC_MODULE

void CsrBtSynccPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_SYNCC_DISCONNECT_IND
CsrSize CsrBtSynccDisconnectIndSizeof(void *msg)
{
    CsrBtSynccDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_DISCONNECT_IND
CsrUint8 *CsrBtSynccDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccDisconnectInd *primitive;

    primitive = (CsrBtSynccDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_DISCONNECT_IND
void *CsrBtSynccDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtSynccDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_IND
CsrSize CsrBtSynccSyncCommandIndSizeof(void *msg)
{
    CsrBtSynccSyncCommandInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_IND
CsrUint8 *CsrBtSynccSyncCommandIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccSyncCommandInd *primitive;

    primitive = (CsrBtSynccSyncCommandInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lengthOfObject);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_IND
void *CsrBtSynccSyncCommandIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccSyncCommandInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccSyncCommandInd *) CsrPmemZalloc(sizeof(CsrBtSynccSyncCommandInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->lengthOfObject, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_IND
void CsrBtSynccSyncCommandIndSerFree(void *msg)
{
    CsrBtSynccSyncCommandInd *primitive;

    primitive = (CsrBtSynccSyncCommandInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_RES
CsrSize CsrBtSynccModifyObjectResSizeof(void *msg)
{
    CsrBtSynccModifyObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_RES
CsrUint8 *CsrBtSynccModifyObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccModifyObjectRes *primitive;

    primitive = (CsrBtSynccModifyObjectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_RES
void *CsrBtSynccModifyObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccModifyObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccModifyObjectRes *) CsrPmemZalloc(sizeof(CsrBtSynccModifyObjectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_RES
void CsrBtSynccModifyObjectResSerFree(void *msg)
{
    CsrBtSynccModifyObjectRes *primitive;

    primitive = (CsrBtSynccModifyObjectRes *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_DISCONNECT_REQ
CsrSize CsrBtSynccDisconnectReqSizeof(void *msg)
{
    CsrBtSynccDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_DISCONNECT_REQ
CsrUint8 *CsrBtSynccDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccDisconnectReq *primitive;

    primitive = (CsrBtSynccDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->normalObexDisconnect);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_DISCONNECT_REQ
void *CsrBtSynccDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtSynccDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->normalObexDisconnect, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_CFM
CsrSize CsrBtSynccDeleteObjectCfmSizeof(void *msg)
{
    CsrBtSynccDeleteObjectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->luid) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->changeCounter) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->timeStamp));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_CFM
CsrUint8 *CsrBtSynccDeleteObjectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccDeleteObjectCfm *primitive;

    primitive = (CsrBtSynccDeleteObjectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrMemCpySer(buffer, length, &primitive->luid, sizeof(primitive->luid));
    CsrMemCpySer(buffer, length, &primitive->changeCounter, sizeof(primitive->changeCounter));
    CsrMemCpySer(buffer, length, &primitive->timeStamp, sizeof(primitive->timeStamp));
    CsrUtf8StringSer(buffer, length, primitive->luid);
    CsrUtf8StringSer(buffer, length, primitive->changeCounter);
    CsrUtf8StringSer(buffer, length, primitive->timeStamp);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_CFM
void *CsrBtSynccDeleteObjectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccDeleteObjectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccDeleteObjectCfm *) CsrPmemZalloc(sizeof(CsrBtSynccDeleteObjectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->luid), buffer, &offset, ((CsrSize) sizeof(primitive->luid)));
    CsrMemCpyDes(((void *) &primitive->changeCounter), buffer, &offset, ((CsrSize) sizeof(primitive->changeCounter)));
    CsrMemCpyDes(((void *) &primitive->timeStamp), buffer, &offset, ((CsrSize) sizeof(primitive->timeStamp)));
    CsrUtf8StringDes(&primitive->luid, buffer, &offset);
    CsrUtf8StringDes(&primitive->changeCounter, buffer, &offset);
    CsrUtf8StringDes(&primitive->timeStamp, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_CFM
void CsrBtSynccDeleteObjectCfmSerFree(void *msg)
{
    CsrBtSynccDeleteObjectCfm *primitive;

    primitive = (CsrBtSynccDeleteObjectCfm *) msg;

    if(primitive->luid != NULL)
    {
        CsrPmemFree(primitive->luid);
    }
    if(primitive->changeCounter != NULL)
    {
        CsrPmemFree(primitive->changeCounter);
    }
    if(primitive->timeStamp != NULL)
    {
        CsrPmemFree(primitive->timeStamp);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_CONNECT_REQ
CsrSize CsrBtSynccConnectReqSizeof(void *msg)
{
    CsrBtSynccConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_CONNECT_REQ
CsrUint8 *CsrBtSynccConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccConnectReq *primitive;

    primitive = (CsrBtSynccConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authorize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->realmLength);
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
#endif /* EXCLUDE_CSR_BT_SYNCC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_CONNECT_REQ
void *CsrBtSynccConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccConnectReq *) CsrPmemZalloc(sizeof(CsrBtSynccConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authorize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->realmLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_CONNECT_REQ
void CsrBtSynccConnectReqSerFree(void *msg)
{
    CsrBtSynccConnectReq *primitive;

    primitive = (CsrBtSynccConnectReq *) msg;

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
#endif /* EXCLUDE_CSR_BT_SYNCC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_REQ
CsrSize CsrBtSynccModifyObjectReqSizeof(void *msg)
{
    CsrBtSynccModifyObjectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->maxExpectedChangeCounter) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->objectName) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_REQ
CsrUint8 *CsrBtSynccModifyObjectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccModifyObjectReq *primitive;

    primitive = (CsrBtSynccModifyObjectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->maxExpectedChangeCounter, sizeof(primitive->maxExpectedChangeCounter));
    CsrMemCpySer(buffer, length, &primitive->objectName, sizeof(primitive->objectName));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lengthOfObject);
    CsrUtf8StringSer(buffer, length, primitive->maxExpectedChangeCounter);
    if (primitive->objectName)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->objectName), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->objectName) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->objectName) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_REQ
void *CsrBtSynccModifyObjectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccModifyObjectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccModifyObjectReq *) CsrPmemZalloc(sizeof(CsrBtSynccModifyObjectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->maxExpectedChangeCounter), buffer, &offset, ((CsrSize) sizeof(primitive->maxExpectedChangeCounter)));
    CsrMemCpyDes(((void *) &primitive->objectName), buffer, &offset, ((CsrSize) sizeof(primitive->objectName)));
    CsrUint32Des((CsrUint32 *) &primitive->lengthOfObject, buffer, &offset);
    CsrUtf8StringDes(&primitive->maxExpectedChangeCounter, buffer, &offset);
    primitive->objectName = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->objectName), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->objectName) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_REQ
void CsrBtSynccModifyObjectReqSerFree(void *msg)
{
    CsrBtSynccModifyObjectReq *primitive;

    primitive = (CsrBtSynccModifyObjectReq *) msg;

    if(primitive->maxExpectedChangeCounter != NULL)
    {
        CsrPmemFree(primitive->maxExpectedChangeCounter);
    }
    if(primitive->objectName != NULL)
    {
        CsrPmemFree(primitive->objectName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_REQ
CsrSize CsrBtSynccAddObjectReqSizeof(void *msg)
{
    CsrBtSynccAddObjectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->objectName) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_REQ
CsrUint8 *CsrBtSynccAddObjectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccAddObjectReq *primitive;

    primitive = (CsrBtSynccAddObjectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->objectName, sizeof(primitive->objectName));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lengthOfObject);
    if (primitive->objectName)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->objectName), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->objectName) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->objectName) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_REQ
void *CsrBtSynccAddObjectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccAddObjectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccAddObjectReq *) CsrPmemZalloc(sizeof(CsrBtSynccAddObjectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->objectName), buffer, &offset, ((CsrSize) sizeof(primitive->objectName)));
    CsrUint32Des((CsrUint32 *) &primitive->lengthOfObject, buffer, &offset);
    primitive->objectName = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->objectName), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->objectName) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_REQ
void CsrBtSynccAddObjectReqSerFree(void *msg)
{
    CsrBtSynccAddObjectReq *primitive;

    primitive = (CsrBtSynccAddObjectReq *) msg;

    if(primitive->objectName != NULL)
    {
        CsrPmemFree(primitive->objectName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_IND
CsrSize CsrBtSynccAuthenticateIndSizeof(void *msg)
{
    CsrBtSynccAuthenticateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_IND
CsrUint8 *CsrBtSynccAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccAuthenticateInd *primitive;

    primitive = (CsrBtSynccAuthenticateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
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
#endif /* EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_IND
void *CsrBtSynccAuthenticateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccAuthenticateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccAuthenticateInd *) CsrPmemZalloc(sizeof(CsrBtSynccAuthenticateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_IND
void CsrBtSynccAuthenticateIndSerFree(void *msg)
{
    CsrBtSynccAuthenticateInd *primitive;

    primitive = (CsrBtSynccAuthenticateInd *) msg;

    if(primitive->realm != NULL)
    {
        CsrPmemFree(primitive->realm);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_CANCEL_CONNECT_REQ
CsrSize CsrBtSynccCancelConnectReqSizeof(void *msg)
{
    CsrBtSynccCancelConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_CANCEL_CONNECT_REQ
CsrUint8 *CsrBtSynccCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccCancelConnectReq *primitive;

    primitive = (CsrBtSynccCancelConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_CANCEL_CONNECT_REQ
void *CsrBtSynccCancelConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccCancelConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccCancelConnectReq *) CsrPmemZalloc(sizeof(CsrBtSynccCancelConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_ABORT_CFM
CsrSize CsrBtSynccAbortCfmSizeof(void *msg)
{
    CsrBtSynccAbortCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_ABORT_CFM
CsrUint8 *CsrBtSynccAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccAbortCfm *primitive;

    primitive = (CsrBtSynccAbortCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_ABORT_CFM
void *CsrBtSynccAbortCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccAbortCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccAbortCfm *) CsrPmemZalloc(sizeof(CsrBtSynccAbortCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_RES
CsrSize CsrBtSynccAddObjectResSizeof(void *msg)
{
    CsrBtSynccAddObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_RES
CsrUint8 *CsrBtSynccAddObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccAddObjectRes *primitive;

    primitive = (CsrBtSynccAddObjectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_RES
void *CsrBtSynccAddObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccAddObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccAddObjectRes *) CsrPmemZalloc(sizeof(CsrBtSynccAddObjectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_RES
void CsrBtSynccAddObjectResSerFree(void *msg)
{
    CsrBtSynccAddObjectRes *primitive;

    primitive = (CsrBtSynccAddObjectRes *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_CFM
CsrSize CsrBtSynccDeactivateSyncCommandCfmSizeof(void *msg)
{
    CsrBtSynccDeactivateSyncCommandCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_CFM
CsrUint8 *CsrBtSynccDeactivateSyncCommandCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccDeactivateSyncCommandCfm *primitive;

    primitive = (CsrBtSynccDeactivateSyncCommandCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_CFM
void *CsrBtSynccDeactivateSyncCommandCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccDeactivateSyncCommandCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccDeactivateSyncCommandCfm *) CsrPmemZalloc(sizeof(CsrBtSynccDeactivateSyncCommandCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_ABORT_REQ
CsrSize CsrBtSynccAbortReqSizeof(void *msg)
{
    CsrBtSynccAbortReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_ABORT_REQ
CsrUint8 *CsrBtSynccAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccAbortReq *primitive;

    primitive = (CsrBtSynccAbortReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_ABORT_REQ
void *CsrBtSynccAbortReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccAbortReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccAbortReq *) CsrPmemZalloc(sizeof(CsrBtSynccAbortReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_REQ
CsrSize CsrBtSynccSecurityInReqSizeof(void *msg)
{
    CsrBtSynccSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_REQ
CsrUint8 *CsrBtSynccSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccSecurityInReq *primitive;

    primitive = (CsrBtSynccSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_REQ
void *CsrBtSynccSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtSynccSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_RES
CsrSize CsrBtSynccAuthenticateResSizeof(void *msg)
{
    CsrBtSynccAuthenticateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_RES
CsrUint8 *CsrBtSynccAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccAuthenticateRes *primitive;

    primitive = (CsrBtSynccAuthenticateRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_RES
void *CsrBtSynccAuthenticateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccAuthenticateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccAuthenticateRes *) CsrPmemZalloc(sizeof(CsrBtSynccAuthenticateRes));

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
#endif /* EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_RES
void CsrBtSynccAuthenticateResSerFree(void *msg)
{
    CsrBtSynccAuthenticateRes *primitive;

    primitive = (CsrBtSynccAuthenticateRes *) msg;

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
#endif /* EXCLUDE_CSR_BT_SYNCC_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_REQ
CsrSize CsrBtSynccActivateSyncCommandReqSizeof(void *msg)
{
    CsrBtSynccActivateSyncCommandReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_REQ
CsrUint8 *CsrBtSynccActivateSyncCommandReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccActivateSyncCommandReq *primitive;

    primitive = (CsrBtSynccActivateSyncCommandReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxPacketSize);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_REQ
void *CsrBtSynccActivateSyncCommandReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccActivateSyncCommandReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccActivateSyncCommandReq *) CsrPmemZalloc(sizeof(CsrBtSynccActivateSyncCommandReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxPacketSize, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_RES
CsrSize CsrBtSynccSyncCommandResSizeof(void *msg)
{
    CsrBtSynccSyncCommandRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_RES
CsrUint8 *CsrBtSynccSyncCommandResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccSyncCommandRes *primitive;

    primitive = (CsrBtSynccSyncCommandRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_RES
void *CsrBtSynccSyncCommandResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccSyncCommandRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccSyncCommandRes *) CsrPmemZalloc(sizeof(CsrBtSynccSyncCommandRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_IND
CsrSize CsrBtSynccAddObjectIndSizeof(void *msg)
{
    CsrBtSynccAddObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_IND
CsrUint8 *CsrBtSynccAddObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccAddObjectInd *primitive;

    primitive = (CsrBtSynccAddObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxAllowedPayloadSize);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_IND
void *CsrBtSynccAddObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccAddObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccAddObjectInd *) CsrPmemZalloc(sizeof(CsrBtSynccAddObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxAllowedPayloadSize, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_IND
CsrSize CsrBtSynccModifyObjectIndSizeof(void *msg)
{
    CsrBtSynccModifyObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_IND
CsrUint8 *CsrBtSynccModifyObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccModifyObjectInd *primitive;

    primitive = (CsrBtSynccModifyObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxAllowedPayloadSize);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_IND
void *CsrBtSynccModifyObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccModifyObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccModifyObjectInd *) CsrPmemZalloc(sizeof(CsrBtSynccModifyObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxAllowedPayloadSize, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_RES
CsrSize CsrBtSynccGetObjectResSizeof(void *msg)
{
    CsrBtSynccGetObjectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_RES
CsrUint8 *CsrBtSynccGetObjectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccGetObjectRes *primitive;

    primitive = (CsrBtSynccGetObjectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_RES
void *CsrBtSynccGetObjectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccGetObjectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccGetObjectRes *) CsrPmemZalloc(sizeof(CsrBtSynccGetObjectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_CFM
CsrSize CsrBtSynccGetObjectCfmSizeof(void *msg)
{
    CsrBtSynccGetObjectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_CFM
CsrUint8 *CsrBtSynccGetObjectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccGetObjectCfm *primitive;

    primitive = (CsrBtSynccGetObjectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_CFM
void *CsrBtSynccGetObjectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccGetObjectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccGetObjectCfm *) CsrPmemZalloc(sizeof(CsrBtSynccGetObjectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_CFM
void CsrBtSynccGetObjectCfmSerFree(void *msg)
{
    CsrBtSynccGetObjectCfm *primitive;

    primitive = (CsrBtSynccGetObjectCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_CFM
CsrSize CsrBtSynccSecurityOutCfmSizeof(void *msg)
{
    CsrBtSynccSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_CFM
CsrUint8 *CsrBtSynccSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccSecurityOutCfm *primitive;

    primitive = (CsrBtSynccSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_CFM
void *CsrBtSynccSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtSynccSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND
CsrSize CsrBtSynccSyncCommandAbortIndSizeof(void *msg)
{
    CsrBtSynccSyncCommandAbortInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND
CsrUint8 *CsrBtSynccSyncCommandAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccSyncCommandAbortInd *primitive;

    primitive = (CsrBtSynccSyncCommandAbortInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->descriptionOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->descriptionLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND
void *CsrBtSynccSyncCommandAbortIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccSyncCommandAbortInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccSyncCommandAbortInd *) CsrPmemZalloc(sizeof(CsrBtSynccSyncCommandAbortInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->descriptionOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->descriptionLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND
void CsrBtSynccSyncCommandAbortIndSerFree(void *msg)
{
    CsrBtSynccSyncCommandAbortInd *primitive;

    primitive = (CsrBtSynccSyncCommandAbortInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SYNC_COMMAND_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_IND
CsrSize CsrBtSynccGetObjectIndSizeof(void *msg)
{
    CsrBtSynccGetObjectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_IND
CsrUint8 *CsrBtSynccGetObjectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccGetObjectInd *primitive;

    primitive = (CsrBtSynccGetObjectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_IND
void *CsrBtSynccGetObjectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccGetObjectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccGetObjectInd *) CsrPmemZalloc(sizeof(CsrBtSynccGetObjectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_IND
void CsrBtSynccGetObjectIndSerFree(void *msg)
{
    CsrBtSynccGetObjectInd *primitive;

    primitive = (CsrBtSynccGetObjectInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_REQ
CsrSize CsrBtSynccDeactivateSyncCommandReqSizeof(void *msg)
{
    CsrBtSynccDeactivateSyncCommandReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_REQ
CsrUint8 *CsrBtSynccDeactivateSyncCommandReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccDeactivateSyncCommandReq *primitive;

    primitive = (CsrBtSynccDeactivateSyncCommandReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_REQ
void *CsrBtSynccDeactivateSyncCommandReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccDeactivateSyncCommandReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccDeactivateSyncCommandReq *) CsrPmemZalloc(sizeof(CsrBtSynccDeactivateSyncCommandReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DEACTIVATE_SYNC_COMMAND_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_CFM
CsrSize CsrBtSynccModifyObjectCfmSizeof(void *msg)
{
    CsrBtSynccModifyObjectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->luid) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->changeCounter) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->timeStamp));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_CFM
CsrUint8 *CsrBtSynccModifyObjectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccModifyObjectCfm *primitive;

    primitive = (CsrBtSynccModifyObjectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrMemCpySer(buffer, length, &primitive->luid, sizeof(primitive->luid));
    CsrMemCpySer(buffer, length, &primitive->changeCounter, sizeof(primitive->changeCounter));
    CsrMemCpySer(buffer, length, &primitive->timeStamp, sizeof(primitive->timeStamp));
    CsrUtf8StringSer(buffer, length, primitive->luid);
    CsrUtf8StringSer(buffer, length, primitive->changeCounter);
    CsrUtf8StringSer(buffer, length, primitive->timeStamp);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_CFM
void *CsrBtSynccModifyObjectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccModifyObjectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccModifyObjectCfm *) CsrPmemZalloc(sizeof(CsrBtSynccModifyObjectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->luid), buffer, &offset, ((CsrSize) sizeof(primitive->luid)));
    CsrMemCpyDes(((void *) &primitive->changeCounter), buffer, &offset, ((CsrSize) sizeof(primitive->changeCounter)));
    CsrMemCpyDes(((void *) &primitive->timeStamp), buffer, &offset, ((CsrSize) sizeof(primitive->timeStamp)));
    CsrUtf8StringDes(&primitive->luid, buffer, &offset);
    CsrUtf8StringDes(&primitive->changeCounter, buffer, &offset);
    CsrUtf8StringDes(&primitive->timeStamp, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_CFM
void CsrBtSynccModifyObjectCfmSerFree(void *msg)
{
    CsrBtSynccModifyObjectCfm *primitive;

    primitive = (CsrBtSynccModifyObjectCfm *) msg;

    if(primitive->luid != NULL)
    {
        CsrPmemFree(primitive->luid);
    }
    if(primitive->changeCounter != NULL)
    {
        CsrPmemFree(primitive->changeCounter);
    }
    if(primitive->timeStamp != NULL)
    {
        CsrPmemFree(primitive->timeStamp);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCC_MODIFY_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_REQ
CsrSize CsrBtSynccGetObjectReqSizeof(void *msg)
{
    CsrBtSynccGetObjectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->ucs2name) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_REQ
CsrUint8 *CsrBtSynccGetObjectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccGetObjectReq *primitive;

    primitive = (CsrBtSynccGetObjectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->ucs2name, sizeof(primitive->ucs2name));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if (primitive->ucs2name)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->ucs2name), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_REQ
void *CsrBtSynccGetObjectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccGetObjectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccGetObjectReq *) CsrPmemZalloc(sizeof(CsrBtSynccGetObjectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ucs2name), buffer, &offset, ((CsrSize) sizeof(primitive->ucs2name)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    primitive->ucs2name = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->ucs2name), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->ucs2name) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_REQ
void CsrBtSynccGetObjectReqSerFree(void *msg)
{
    CsrBtSynccGetObjectReq *primitive;

    primitive = (CsrBtSynccGetObjectReq *) msg;

    if(primitive->ucs2name != NULL)
    {
        CsrPmemFree(primitive->ucs2name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCC_GET_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_CFM
CsrSize CsrBtSynccSecurityInCfmSizeof(void *msg)
{
    CsrBtSynccSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_CFM
CsrUint8 *CsrBtSynccSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccSecurityInCfm *primitive;

    primitive = (CsrBtSynccSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_CFM
void *CsrBtSynccSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtSynccSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_REQ
CsrSize CsrBtSynccSecurityOutReqSizeof(void *msg)
{
    CsrBtSynccSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_REQ
CsrUint8 *CsrBtSynccSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccSecurityOutReq *primitive;

    primitive = (CsrBtSynccSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_REQ
void *CsrBtSynccSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtSynccSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_CONNECT_CFM
CsrSize CsrBtSynccConnectCfmSizeof(void *msg)
{
    CsrBtSynccConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_CONNECT_CFM
CsrUint8 *CsrBtSynccConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccConnectCfm *primitive;

    primitive = (CsrBtSynccConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supportedStores);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexPeerMaxPacketSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_CONNECT_CFM
void *CsrBtSynccConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccConnectCfm *) CsrPmemZalloc(sizeof(CsrBtSynccConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supportedStores, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexPeerMaxPacketSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_CFM
CsrSize CsrBtSynccAddObjectCfmSizeof(void *msg)
{
    CsrBtSynccAddObjectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->luid) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->changeCounter) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->timeStamp));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_CFM
CsrUint8 *CsrBtSynccAddObjectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccAddObjectCfm *primitive;

    primitive = (CsrBtSynccAddObjectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrMemCpySer(buffer, length, &primitive->luid, sizeof(primitive->luid));
    CsrMemCpySer(buffer, length, &primitive->changeCounter, sizeof(primitive->changeCounter));
    CsrMemCpySer(buffer, length, &primitive->timeStamp, sizeof(primitive->timeStamp));
    CsrUtf8StringSer(buffer, length, primitive->luid);
    CsrUtf8StringSer(buffer, length, primitive->changeCounter);
    CsrUtf8StringSer(buffer, length, primitive->timeStamp);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_CFM
void *CsrBtSynccAddObjectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccAddObjectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccAddObjectCfm *) CsrPmemZalloc(sizeof(CsrBtSynccAddObjectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->luid), buffer, &offset, ((CsrSize) sizeof(primitive->luid)));
    CsrMemCpyDes(((void *) &primitive->changeCounter), buffer, &offset, ((CsrSize) sizeof(primitive->changeCounter)));
    CsrMemCpyDes(((void *) &primitive->timeStamp), buffer, &offset, ((CsrSize) sizeof(primitive->timeStamp)));
    CsrUtf8StringDes(&primitive->luid, buffer, &offset);
    CsrUtf8StringDes(&primitive->changeCounter, buffer, &offset);
    CsrUtf8StringDes(&primitive->timeStamp, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_CFM
void CsrBtSynccAddObjectCfmSerFree(void *msg)
{
    CsrBtSynccAddObjectCfm *primitive;

    primitive = (CsrBtSynccAddObjectCfm *) msg;

    if(primitive->luid != NULL)
    {
        CsrPmemFree(primitive->luid);
    }
    if(primitive->changeCounter != NULL)
    {
        CsrPmemFree(primitive->changeCounter);
    }
    if(primitive->timeStamp != NULL)
    {
        CsrPmemFree(primitive->timeStamp);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ADD_OBJECT_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_REQ
CsrSize CsrBtSynccDeleteObjectReqSizeof(void *msg)
{
    CsrBtSynccDeleteObjectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->maxExpectedChangeCounter) + CsrConverterUcs2ByteStrLen((CsrUtf8String*) primitive->objectName) + 2);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_REQ
CsrUint8 *CsrBtSynccDeleteObjectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccDeleteObjectReq *primitive;

    primitive = (CsrBtSynccDeleteObjectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->hardDelete);
    CsrMemCpySer(buffer, length, &primitive->maxExpectedChangeCounter, sizeof(primitive->maxExpectedChangeCounter));
    CsrMemCpySer(buffer, length, &primitive->objectName, sizeof(primitive->objectName));
    CsrUtf8StringSer(buffer, length, primitive->maxExpectedChangeCounter);
    if (primitive->objectName)
    {
        CsrMemCpy(((CsrUint8*) &buffer[*length]), ((CsrUint8*) primitive->objectName), CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->objectName) + 2);
        *length += ((CsrUint16) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->objectName) + 2));
    }
    else
    {
        buffer[*length] = '\0';
        buffer[*length + 1] = '\0';
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_REQ
void *CsrBtSynccDeleteObjectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccDeleteObjectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccDeleteObjectReq *) CsrPmemZalloc(sizeof(CsrBtSynccDeleteObjectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->hardDelete, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->maxExpectedChangeCounter), buffer, &offset, ((CsrSize) sizeof(primitive->maxExpectedChangeCounter)));
    CsrMemCpyDes(((void *) &primitive->objectName), buffer, &offset, ((CsrSize) sizeof(primitive->objectName)));
    CsrUtf8StringDes(&primitive->maxExpectedChangeCounter, buffer, &offset);
    primitive->objectName = CsrPmemZalloc(CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    CsrMemCpy(((CsrUint8*) primitive->objectName), ((CsrUint8*) &buffer[offset]),CsrConverterUcs2ByteStrLen((CsrUint8*) &buffer[offset]) + 2);
    offset += ((CsrSize) (CsrConverterUcs2ByteStrLen((CsrUint8*) primitive->objectName) + 2));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_REQ
void CsrBtSynccDeleteObjectReqSerFree(void *msg)
{
    CsrBtSynccDeleteObjectReq *primitive;

    primitive = (CsrBtSynccDeleteObjectReq *) msg;

    if(primitive->maxExpectedChangeCounter != NULL)
    {
        CsrPmemFree(primitive->maxExpectedChangeCounter);
    }
    if(primitive->objectName != NULL)
    {
        CsrPmemFree(primitive->objectName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCC_DELETE_OBJECT_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_CFM
CsrSize CsrBtSynccActivateSyncCommandCfmSizeof(void *msg)
{
    CsrBtSynccActivateSyncCommandCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_CFM
CsrUint8 *CsrBtSynccActivateSyncCommandCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSynccActivateSyncCommandCfm *primitive;

    primitive = (CsrBtSynccActivateSyncCommandCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_CFM
void *CsrBtSynccActivateSyncCommandCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSynccActivateSyncCommandCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSynccActivateSyncCommandCfm *) CsrPmemZalloc(sizeof(CsrBtSynccActivateSyncCommandCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCC_ACTIVATE_SYNC_COMMAND_CFM */

#endif /* EXCLUDE_CSR_BT_SYNCC_MODULE */
