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
#include "csr_bt_syncs_prim.h"

#ifndef EXCLUDE_CSR_BT_SYNCS_MODULE

void CsrBtSyncsPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_SYNCS_DISCONNECT_IND
CsrSize CsrBtSyncsDisconnectIndSizeof(void *msg)
{
    CsrBtSyncsDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_DISCONNECT_IND
CsrUint8 *CsrBtSyncsDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsDisconnectInd *primitive;

    primitive = (CsrBtSyncsDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_DISCONNECT_IND
void *CsrBtSyncsDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtSyncsDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_IND
CsrSize CsrBtSyncsGetPbEntryIndSizeof(void *msg)
{
    CsrBtSyncsGetPbEntryInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_IND
CsrUint8 *CsrBtSyncsGetPbEntryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetPbEntryInd *primitive;

    primitive = (CsrBtSyncsGetPbEntryInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_IND
void *CsrBtSyncsGetPbEntryIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetPbEntryInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetPbEntryInd *) CsrPmemZalloc(sizeof(CsrBtSyncsGetPbEntryInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_IND
void CsrBtSyncsGetPbEntryIndSerFree(void *msg)
{
    CsrBtSyncsGetPbEntryInd *primitive;

    primitive = (CsrBtSyncsGetPbEntryInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_IND
CsrSize CsrBtSyncsGetPbAllIndSizeof(void *msg)
{
    CsrBtSyncsGetPbAllInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_IND
CsrUint8 *CsrBtSyncsGetPbAllIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetPbAllInd *primitive;

    primitive = (CsrBtSyncsGetPbAllInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_IND
void *CsrBtSyncsGetPbAllIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetPbAllInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetPbAllInd *) CsrPmemZalloc(sizeof(CsrBtSyncsGetPbAllInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES
CsrSize CsrBtSyncsGetCalChangeLogResSizeof(void *msg)
{
    CsrBtSyncsGetCalChangeLogRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES
CsrUint8 *CsrBtSyncsGetCalChangeLogResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetCalChangeLogRes *primitive;

    primitive = (CsrBtSyncsGetCalChangeLogRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalObjectSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES
void *CsrBtSyncsGetCalChangeLogResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetCalChangeLogRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetCalChangeLogRes *) CsrPmemZalloc(sizeof(CsrBtSyncsGetCalChangeLogRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalObjectSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES
void CsrBtSyncsGetCalChangeLogResSerFree(void *msg)
{
    CsrBtSyncsGetCalChangeLogRes *primitive;

    primitive = (CsrBtSyncsGetCalChangeLogRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_RES
CsrSize CsrBtSyncsPutPbEntryResSizeof(void *msg)
{
    CsrBtSyncsPutPbEntryRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->appParameterLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_RES
CsrUint8 *CsrBtSyncsPutPbEntryResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsPutPbEntryRes *primitive;

    primitive = (CsrBtSyncsPutPbEntryRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appParameterLength);
    CsrMemCpySer(buffer, length, &primitive->appParameter, sizeof(primitive->appParameter));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->appParameter)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->appParameter), ((CsrSize) (primitive->appParameterLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_RES
void *CsrBtSyncsPutPbEntryResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsPutPbEntryRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsPutPbEntryRes *) CsrPmemZalloc(sizeof(CsrBtSyncsPutPbEntryRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appParameterLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->appParameter), buffer, &offset, ((CsrSize) sizeof(primitive->appParameter)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->appParameterLength)
    {
        primitive->appParameter = CsrPmemZalloc(((CsrUint32) (primitive->appParameterLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->appParameter), buffer, &offset, ((CsrSize) (primitive->appParameterLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->appParameter = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_RES
void CsrBtSyncsPutPbEntryResSerFree(void *msg)
{
    CsrBtSyncsPutPbEntryRes *primitive;

    primitive = (CsrBtSyncsPutPbEntryRes *) msg;

    if(primitive->appParameter != NULL)
    {
        CsrPmemFree(primitive->appParameter);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_IND
CsrSize CsrBtSyncsPutIndSizeof(void *msg)
{
    CsrBtSyncsPutInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_IND
CsrUint8 *CsrBtSyncsPutIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsPutInd *primitive;

    primitive = (CsrBtSyncsPutInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lengthOfObject);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_IND
void *CsrBtSyncsPutIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsPutInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsPutInd *) CsrPmemZalloc(sizeof(CsrBtSyncsPutInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->lengthOfObject, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_IND
void CsrBtSyncsPutIndSerFree(void *msg)
{
    CsrBtSyncsPutInd *primitive;

    primitive = (CsrBtSyncsPutInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_CONNECT_IND
CsrSize CsrBtSyncsConnectIndSizeof(void *msg)
{
    CsrBtSyncsConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_CONNECT_IND
CsrUint8 *CsrBtSyncsConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsConnectInd *primitive;

    primitive = (CsrBtSyncsConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexPeerMaxPacketSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->count);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_CONNECT_IND
void *CsrBtSyncsConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsConnectInd *) CsrPmemZalloc(sizeof(CsrBtSyncsConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexPeerMaxPacketSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->count, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_RES
CsrSize CsrBtSyncsAuthenticateResSizeof(void *msg)
{
    CsrBtSyncsAuthenticateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_RES
CsrUint8 *CsrBtSyncsAuthenticateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsAuthenticateRes *primitive;

    primitive = (CsrBtSyncsAuthenticateRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->passwordLength);
    CsrMemCpySer(buffer, length, &primitive->password, sizeof(primitive->password));
    CsrMemCpySer(buffer, length, &primitive->userId, sizeof(primitive->userId));
    if(primitive->password)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->password), ((CsrSize) (primitive->passwordLength * sizeof(CsrUint8))));
    }
    CsrCharStringSer(buffer, length, primitive->userId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_RES
void *CsrBtSyncsAuthenticateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsAuthenticateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsAuthenticateRes *) CsrPmemZalloc(sizeof(CsrBtSyncsAuthenticateRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->passwordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->password), buffer, &offset, ((CsrSize) sizeof(primitive->password)));
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
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_RES
void CsrBtSyncsAuthenticateResSerFree(void *msg)
{
    CsrBtSyncsAuthenticateRes *primitive;

    primitive = (CsrBtSyncsAuthenticateRes *) msg;

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
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_RES
CsrSize CsrBtSyncsGetPbAllResSizeof(void *msg)
{
    CsrBtSyncsGetPbAllRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_RES
CsrUint8 *CsrBtSyncsGetPbAllResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetPbAllRes *primitive;

    primitive = (CsrBtSyncsGetPbAllRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_RES
void *CsrBtSyncsGetPbAllResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetPbAllRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetPbAllRes *) CsrPmemZalloc(sizeof(CsrBtSyncsGetPbAllRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_RES
void CsrBtSyncsGetPbAllResSerFree(void *msg)
{
    CsrBtSyncsGetPbAllRes *primitive;

    primitive = (CsrBtSyncsGetPbAllRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ALL_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_NEXT_IND
CsrSize CsrBtSyncsGetNextIndSizeof(void *msg)
{
    CsrBtSyncsGetNextInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_NEXT_IND
CsrUint8 *CsrBtSyncsGetNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetNextInd *primitive;

    primitive = (CsrBtSyncsGetNextInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_NEXT_IND
void *CsrBtSyncsGetNextIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetNextInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetNextInd *) CsrPmemZalloc(sizeof(CsrBtSyncsGetNextInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_IND
CsrSize CsrBtSyncsGetDeviceInfoIndSizeof(void *msg)
{
    CsrBtSyncsGetDeviceInfoInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_IND
CsrUint8 *CsrBtSyncsGetDeviceInfoIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetDeviceInfoInd *primitive;

    primitive = (CsrBtSyncsGetDeviceInfoInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_IND
void *CsrBtSyncsGetDeviceInfoIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetDeviceInfoInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetDeviceInfoInd *) CsrPmemZalloc(sizeof(CsrBtSyncsGetDeviceInfoInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_IND
CsrSize CsrBtSyncsPutPbEntryIndSizeof(void *msg)
{
    CsrBtSyncsPutPbEntryInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_IND
CsrUint8 *CsrBtSyncsPutPbEntryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsPutPbEntryInd *primitive;

    primitive = (CsrBtSyncsPutPbEntryInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lengthOfObject);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bodyLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_IND
void *CsrBtSyncsPutPbEntryIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsPutPbEntryInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsPutPbEntryInd *) CsrPmemZalloc(sizeof(CsrBtSyncsPutPbEntryInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->lengthOfObject, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_IND
void CsrBtSyncsPutPbEntryIndSerFree(void *msg)
{
    CsrBtSyncsPutPbEntryInd *primitive;

    primitive = (CsrBtSyncsPutPbEntryInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_IND
CsrSize CsrBtSyncsGetPbInfoLogIndSizeof(void *msg)
{
    CsrBtSyncsGetPbInfoLogInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_IND
CsrUint8 *CsrBtSyncsGetPbInfoLogIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetPbInfoLogInd *primitive;

    primitive = (CsrBtSyncsGetPbInfoLogInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_IND
void *CsrBtSyncsGetPbInfoLogIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetPbInfoLogInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetPbInfoLogInd *) CsrPmemZalloc(sizeof(CsrBtSyncsGetPbInfoLogInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_IND
CsrSize CsrBtSyncsGetCalCurChangeLogIndSizeof(void *msg)
{
    CsrBtSyncsGetCalCurChangeLogInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_IND
CsrUint8 *CsrBtSyncsGetCalCurChangeLogIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetCalCurChangeLogInd *primitive;

    primitive = (CsrBtSyncsGetCalCurChangeLogInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_IND
void *CsrBtSyncsGetCalCurChangeLogIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetCalCurChangeLogInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetCalCurChangeLogInd *) CsrPmemZalloc(sizeof(CsrBtSyncsGetCalCurChangeLogInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES
CsrSize CsrBtSyncsGetCalInfoLogResSizeof(void *msg)
{
    CsrBtSyncsGetCalInfoLogRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES
CsrUint8 *CsrBtSyncsGetCalInfoLogResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetCalInfoLogRes *primitive;

    primitive = (CsrBtSyncsGetCalInfoLogRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalObjectSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES
void *CsrBtSyncsGetCalInfoLogResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetCalInfoLogRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetCalInfoLogRes *) CsrPmemZalloc(sizeof(CsrBtSyncsGetCalInfoLogRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalObjectSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES
void CsrBtSyncsGetCalInfoLogResSerFree(void *msg)
{
    CsrBtSyncsGetCalInfoLogRes *primitive;

    primitive = (CsrBtSyncsGetCalInfoLogRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_CFM
CsrSize CsrBtSyncsSecurityInCfmSizeof(void *msg)
{
    CsrBtSyncsSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_CFM
CsrUint8 *CsrBtSyncsSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsSecurityInCfm *primitive;

    primitive = (CsrBtSyncsSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_CFM
void *CsrBtSyncsSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtSyncsSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_REQ
CsrSize CsrBtSyncsDeactivateReqSizeof(void *msg)
{
    CsrBtSyncsDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_REQ
CsrUint8 *CsrBtSyncsDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsDeactivateReq *primitive;

    primitive = (CsrBtSyncsDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_REQ
void *CsrBtSyncsDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtSyncsDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_IND
CsrSize CsrBtSyncsGetIndSizeof(void *msg)
{
    CsrBtSyncsGetInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_IND
CsrUint8 *CsrBtSyncsGetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetInd *primitive;

    primitive = (CsrBtSyncsGetInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_IND
void *CsrBtSyncsGetIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetInd *) CsrPmemZalloc(sizeof(CsrBtSyncsGetInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_IND
void CsrBtSyncsGetIndSerFree(void *msg)
{
    CsrBtSyncsGetInd *primitive;

    primitive = (CsrBtSyncsGetInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_IND
CsrSize CsrBtSyncsPutCalEntryIndSizeof(void *msg)
{
    CsrBtSyncsPutCalEntryInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_IND
CsrUint8 *CsrBtSyncsPutCalEntryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsPutCalEntryInd *primitive;

    primitive = (CsrBtSyncsPutCalEntryInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalObjectSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_IND
void *CsrBtSyncsPutCalEntryIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsPutCalEntryInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsPutCalEntryInd *) CsrPmemZalloc(sizeof(CsrBtSyncsPutCalEntryInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalObjectSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_IND
void CsrBtSyncsPutCalEntryIndSerFree(void *msg)
{
    CsrBtSyncsPutCalEntryInd *primitive;

    primitive = (CsrBtSyncsPutCalEntryInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND
CsrSize CsrBtSyncsPutCalAddEntryIndSizeof(void *msg)
{
    CsrBtSyncsPutCalAddEntryInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND
CsrUint8 *CsrBtSyncsPutCalAddEntryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsPutCalAddEntryInd *primitive;

    primitive = (CsrBtSyncsPutCalAddEntryInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalObjectSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND
void *CsrBtSyncsPutCalAddEntryIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsPutCalAddEntryInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsPutCalAddEntryInd *) CsrPmemZalloc(sizeof(CsrBtSyncsPutCalAddEntryInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalObjectSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND
void CsrBtSyncsPutCalAddEntryIndSerFree(void *msg)
{
    CsrBtSyncsPutCalAddEntryInd *primitive;

    primitive = (CsrBtSyncsPutCalAddEntryInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_RES
CsrSize CsrBtSyncsGetCalAllResSizeof(void *msg)
{
    CsrBtSyncsGetCalAllRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_RES
CsrUint8 *CsrBtSyncsGetCalAllResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetCalAllRes *primitive;

    primitive = (CsrBtSyncsGetCalAllRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_RES
void *CsrBtSyncsGetCalAllResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetCalAllRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetCalAllRes *) CsrPmemZalloc(sizeof(CsrBtSyncsGetCalAllRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_RES
void CsrBtSyncsGetCalAllResSerFree(void *msg)
{
    CsrBtSyncsGetCalAllRes *primitive;

    primitive = (CsrBtSyncsGetCalAllRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_RES
CsrSize CsrBtSyncsGetPbEntryResSizeof(void *msg)
{
    CsrBtSyncsGetPbEntryRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_RES
CsrUint8 *CsrBtSyncsGetPbEntryResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetPbEntryRes *primitive;

    primitive = (CsrBtSyncsGetPbEntryRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalObjectSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_RES
void *CsrBtSyncsGetPbEntryResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetPbEntryRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetPbEntryRes *) CsrPmemZalloc(sizeof(CsrBtSyncsGetPbEntryRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalObjectSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_RES
void CsrBtSyncsGetPbEntryResSerFree(void *msg)
{
    CsrBtSyncsGetPbEntryRes *primitive;

    primitive = (CsrBtSyncsGetPbEntryRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_IND
CsrSize CsrBtSyncsGetCalAllIndSizeof(void *msg)
{
    CsrBtSyncsGetCalAllInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_IND
CsrUint8 *CsrBtSyncsGetCalAllIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetCalAllInd *primitive;

    primitive = (CsrBtSyncsGetCalAllInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_IND
void *CsrBtSyncsGetCalAllIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetCalAllInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetCalAllInd *) CsrPmemZalloc(sizeof(CsrBtSyncsGetCalAllInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ALL_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_RES
CsrSize CsrBtSyncsPutCalEntryResSizeof(void *msg)
{
    CsrBtSyncsPutCalEntryRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->appParameterLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_RES
CsrUint8 *CsrBtSyncsPutCalEntryResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsPutCalEntryRes *primitive;

    primitive = (CsrBtSyncsPutCalEntryRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appParameterLength);
    CsrMemCpySer(buffer, length, &primitive->appParameter, sizeof(primitive->appParameter));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->appParameter)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->appParameter), ((CsrSize) (primitive->appParameterLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_RES
void *CsrBtSyncsPutCalEntryResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsPutCalEntryRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsPutCalEntryRes *) CsrPmemZalloc(sizeof(CsrBtSyncsPutCalEntryRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appParameterLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->appParameter), buffer, &offset, ((CsrSize) sizeof(primitive->appParameter)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->appParameterLength)
    {
        primitive->appParameter = CsrPmemZalloc(((CsrUint32) (primitive->appParameterLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->appParameter), buffer, &offset, ((CsrSize) (primitive->appParameterLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->appParameter = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_RES
void CsrBtSyncsPutCalEntryResSerFree(void *msg)
{
    CsrBtSyncsPutCalEntryRes *primitive;

    primitive = (CsrBtSyncsPutCalEntryRes *) msg;

    if(primitive->appParameter != NULL)
    {
        CsrPmemFree(primitive->appParameter);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND
CsrSize CsrBtSyncsGetPbChangeLogIndSizeof(void *msg)
{
    CsrBtSyncsGetPbChangeLogInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND
CsrUint8 *CsrBtSyncsGetPbChangeLogIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetPbChangeLogInd *primitive;

    primitive = (CsrBtSyncsGetPbChangeLogInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND
void *CsrBtSyncsGetPbChangeLogIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetPbChangeLogInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetPbChangeLogInd *) CsrPmemZalloc(sizeof(CsrBtSyncsGetPbChangeLogInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND
void CsrBtSyncsGetPbChangeLogIndSerFree(void *msg)
{
    CsrBtSyncsGetPbChangeLogInd *primitive;

    primitive = (CsrBtSyncsGetPbChangeLogInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND
CsrSize CsrBtSyncsPutPbAddEntryIndSizeof(void *msg)
{
    CsrBtSyncsPutPbAddEntryInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND
CsrUint8 *CsrBtSyncsPutPbAddEntryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsPutPbAddEntryInd *primitive;

    primitive = (CsrBtSyncsPutPbAddEntryInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->lengthOfObject);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND
void *CsrBtSyncsPutPbAddEntryIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsPutPbAddEntryInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsPutPbAddEntryInd *) CsrPmemZalloc(sizeof(CsrBtSyncsPutPbAddEntryInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->lengthOfObject, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bodyLength, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND
void CsrBtSyncsPutPbAddEntryIndSerFree(void *msg)
{
    CsrBtSyncsPutPbAddEntryInd *primitive;

    primitive = (CsrBtSyncsPutPbAddEntryInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_RES
CsrSize CsrBtSyncsPutNextResSizeof(void *msg)
{
    CsrBtSyncsPutNextRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->appParameterLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_RES
CsrUint8 *CsrBtSyncsPutNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsPutNextRes *primitive;

    primitive = (CsrBtSyncsPutNextRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appParameterLength);
    CsrMemCpySer(buffer, length, &primitive->appParameter, sizeof(primitive->appParameter));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->appParameter)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->appParameter), ((CsrSize) (primitive->appParameterLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_RES
void *CsrBtSyncsPutNextResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsPutNextRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsPutNextRes *) CsrPmemZalloc(sizeof(CsrBtSyncsPutNextRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appParameterLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->appParameter), buffer, &offset, ((CsrSize) sizeof(primitive->appParameter)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->appParameterLength)
    {
        primitive->appParameter = CsrPmemZalloc(((CsrUint32) (primitive->appParameterLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->appParameter), buffer, &offset, ((CsrSize) (primitive->appParameterLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->appParameter = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_RES
void CsrBtSyncsPutNextResSerFree(void *msg)
{
    CsrBtSyncsPutNextRes *primitive;

    primitive = (CsrBtSyncsPutNextRes *) msg;

    if(primitive->appParameter != NULL)
    {
        CsrPmemFree(primitive->appParameter);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_CFM
CsrSize CsrBtSyncsDeactivateCfmSizeof(void *msg)
{
    CsrBtSyncsDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_CFM
CsrUint8 *CsrBtSyncsDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsDeactivateCfm *primitive;

    primitive = (CsrBtSyncsDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_CFM
void *CsrBtSyncsDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtSyncsDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_REQ
CsrSize CsrBtSyncsSecurityInReqSizeof(void *msg)
{
    CsrBtSyncsSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_REQ
CsrUint8 *CsrBtSyncsSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsSecurityInReq *primitive;

    primitive = (CsrBtSyncsSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_REQ
void *CsrBtSyncsSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtSyncsSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_RES
CsrSize CsrBtSyncsPutResSizeof(void *msg)
{
    CsrBtSyncsPutRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->appParameterLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_RES
CsrUint8 *CsrBtSyncsPutResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsPutRes *primitive;

    primitive = (CsrBtSyncsPutRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appParameterLength);
    CsrMemCpySer(buffer, length, &primitive->appParameter, sizeof(primitive->appParameter));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->appParameter)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->appParameter), ((CsrSize) (primitive->appParameterLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_RES
void *CsrBtSyncsPutResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsPutRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsPutRes *) CsrPmemZalloc(sizeof(CsrBtSyncsPutRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appParameterLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->appParameter), buffer, &offset, ((CsrSize) sizeof(primitive->appParameter)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->appParameterLength)
    {
        primitive->appParameter = CsrPmemZalloc(((CsrUint32) (primitive->appParameterLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->appParameter), buffer, &offset, ((CsrSize) (primitive->appParameterLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->appParameter = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_RES
void CsrBtSyncsPutResSerFree(void *msg)
{
    CsrBtSyncsPutRes *primitive;

    primitive = (CsrBtSyncsPutRes *) msg;

    if(primitive->appParameter != NULL)
    {
        CsrPmemFree(primitive->appParameter);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_RES
CsrSize CsrBtSyncsGetCalEntryResSizeof(void *msg)
{
    CsrBtSyncsGetCalEntryRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_RES
CsrUint8 *CsrBtSyncsGetCalEntryResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetCalEntryRes *primitive;

    primitive = (CsrBtSyncsGetCalEntryRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalObjectSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_RES
void *CsrBtSyncsGetCalEntryResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetCalEntryRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetCalEntryRes *) CsrPmemZalloc(sizeof(CsrBtSyncsGetCalEntryRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalObjectSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_RES
void CsrBtSyncsGetCalEntryResSerFree(void *msg)
{
    CsrBtSyncsGetCalEntryRes *primitive;

    primitive = (CsrBtSyncsGetCalEntryRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND
CsrSize CsrBtSyncsGetCalChangeLogIndSizeof(void *msg)
{
    CsrBtSyncsGetCalChangeLogInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND
CsrUint8 *CsrBtSyncsGetCalChangeLogIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetCalChangeLogInd *primitive;

    primitive = (CsrBtSyncsGetCalChangeLogInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND
void *CsrBtSyncsGetCalChangeLogIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetCalChangeLogInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetCalChangeLogInd *) CsrPmemZalloc(sizeof(CsrBtSyncsGetCalChangeLogInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND
void CsrBtSyncsGetCalChangeLogIndSerFree(void *msg)
{
    CsrBtSyncsGetCalChangeLogInd *primitive;

    primitive = (CsrBtSyncsGetCalChangeLogInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_REQ
CsrSize CsrBtSyncsAuthenticateReqSizeof(void *msg)
{
    CsrBtSyncsAuthenticateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)) + (primitive->passwordLength * sizeof(CsrUint8)) + CsrCharStringSerLen((CsrCharString *) primitive->userId));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_REQ
CsrUint8 *CsrBtSyncsAuthenticateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsAuthenticateReq *primitive;

    primitive = (CsrBtSyncsAuthenticateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->realmLength);
    CsrMemCpySer(buffer, length, &primitive->realm, sizeof(primitive->realm));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->passwordLength);
    CsrMemCpySer(buffer, length, &primitive->password, sizeof(primitive->password));
    CsrMemCpySer(buffer, length, &primitive->userId, sizeof(primitive->userId));
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
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_REQ
void *CsrBtSyncsAuthenticateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsAuthenticateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsAuthenticateReq *) CsrPmemZalloc(sizeof(CsrBtSyncsAuthenticateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->realmLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->realm), buffer, &offset, ((CsrSize) sizeof(primitive->realm)));
    CsrUint16Des((CsrUint16 *) &primitive->passwordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->password), buffer, &offset, ((CsrSize) sizeof(primitive->password)));
    CsrMemCpyDes(((void *) &primitive->userId), buffer, &offset, ((CsrSize) sizeof(primitive->userId)));
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
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_REQ
void CsrBtSyncsAuthenticateReqSerFree(void *msg)
{
    CsrBtSyncsAuthenticateReq *primitive;

    primitive = (CsrBtSyncsAuthenticateReq *) msg;

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
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES
CsrSize CsrBtSyncsPutPbAddEntryResSizeof(void *msg)
{
    CsrBtSyncsPutPbAddEntryRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->appParameterLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES
CsrUint8 *CsrBtSyncsPutPbAddEntryResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsPutPbAddEntryRes *primitive;

    primitive = (CsrBtSyncsPutPbAddEntryRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appParameterLength);
    CsrMemCpySer(buffer, length, &primitive->appParameter, sizeof(primitive->appParameter));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->appParameter)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->appParameter), ((CsrSize) (primitive->appParameterLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES
void *CsrBtSyncsPutPbAddEntryResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsPutPbAddEntryRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsPutPbAddEntryRes *) CsrPmemZalloc(sizeof(CsrBtSyncsPutPbAddEntryRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appParameterLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->appParameter), buffer, &offset, ((CsrSize) sizeof(primitive->appParameter)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->appParameterLength)
    {
        primitive->appParameter = CsrPmemZalloc(((CsrUint32) (primitive->appParameterLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->appParameter), buffer, &offset, ((CsrSize) (primitive->appParameterLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->appParameter = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES
void CsrBtSyncsPutPbAddEntryResSerFree(void *msg)
{
    CsrBtSyncsPutPbAddEntryRes *primitive;

    primitive = (CsrBtSyncsPutPbAddEntryRes *) msg;

    if(primitive->appParameter != NULL)
    {
        CsrPmemFree(primitive->appParameter);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_PB_ADD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_CONNECT_RES
CsrSize CsrBtSyncsConnectResSizeof(void *msg)
{
    CsrBtSyncsConnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_CONNECT_RES
CsrUint8 *CsrBtSyncsConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsConnectRes *primitive;

    primitive = (CsrBtSyncsConnectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_CONNECT_RES
void *CsrBtSyncsConnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsConnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsConnectRes *) CsrPmemZalloc(sizeof(CsrBtSyncsConnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_IND
CsrSize CsrBtSyncsAuthenticateIndSizeof(void *msg)
{
    CsrBtSyncsAuthenticateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->realmLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_IND
CsrUint8 *CsrBtSyncsAuthenticateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsAuthenticateInd *primitive;

    primitive = (CsrBtSyncsAuthenticateInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_IND
void *CsrBtSyncsAuthenticateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsAuthenticateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsAuthenticateInd *) CsrPmemZalloc(sizeof(CsrBtSyncsAuthenticateInd));

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
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_IND
void CsrBtSyncsAuthenticateIndSerFree(void *msg)
{
    CsrBtSyncsAuthenticateInd *primitive;

    primitive = (CsrBtSyncsAuthenticateInd *) msg;

    if(primitive->realm != NULL)
    {
        CsrPmemFree(primitive->realm);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_IND
CsrSize CsrBtSyncsGetCalEntryIndSizeof(void *msg)
{
    CsrBtSyncsGetCalEntryInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_IND
CsrUint8 *CsrBtSyncsGetCalEntryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetCalEntryInd *primitive;

    primitive = (CsrBtSyncsGetCalEntryInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ucs2nameOffset);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_IND
void *CsrBtSyncsGetCalEntryIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetCalEntryInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetCalEntryInd *) CsrPmemZalloc(sizeof(CsrBtSyncsGetCalEntryInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ucs2nameOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_IND
void CsrBtSyncsGetCalEntryIndSerFree(void *msg)
{
    CsrBtSyncsGetCalEntryInd *primitive;

    primitive = (CsrBtSyncsGetCalEntryInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_ENTRY_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_RES
CsrSize CsrBtSyncsGetPbInfoLogResSizeof(void *msg)
{
    CsrBtSyncsGetPbInfoLogRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_RES
CsrUint8 *CsrBtSyncsGetPbInfoLogResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetPbInfoLogRes *primitive;

    primitive = (CsrBtSyncsGetPbInfoLogRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalObjectSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_RES
void *CsrBtSyncsGetPbInfoLogResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetPbInfoLogRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetPbInfoLogRes *) CsrPmemZalloc(sizeof(CsrBtSyncsGetPbInfoLogRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalObjectSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_RES
void CsrBtSyncsGetPbInfoLogResSerFree(void *msg)
{
    CsrBtSyncsGetPbInfoLogRes *primitive;

    primitive = (CsrBtSyncsGetPbInfoLogRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_INFO_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_ACTIVATE_REQ
CsrSize CsrBtSyncsActivateReqSizeof(void *msg)
{
    CsrBtSyncsActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_ACTIVATE_REQ
CsrUint8 *CsrBtSyncsActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsActivateReq *primitive;

    primitive = (CsrBtSyncsActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supportedFormats);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->obexMaxPacketSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->windowSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmEnable);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_ACTIVATE_REQ
void *CsrBtSyncsActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsActivateReq *) CsrPmemZalloc(sizeof(CsrBtSyncsActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supportedFormats, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->obexMaxPacketSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->windowSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->srmEnable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_RES
CsrSize CsrBtSyncsGetResSizeof(void *msg)
{
    CsrBtSyncsGetRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_RES
CsrUint8 *CsrBtSyncsGetResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetRes *primitive;

    primitive = (CsrBtSyncsGetRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalObjectSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_RES
void *CsrBtSyncsGetResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetRes *) CsrPmemZalloc(sizeof(CsrBtSyncsGetRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalObjectSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_RES
void CsrBtSyncsGetResSerFree(void *msg)
{
    CsrBtSyncsGetRes *primitive;

    primitive = (CsrBtSyncsGetRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_IND
CsrSize CsrBtSyncsGetCalInfoLogIndSizeof(void *msg)
{
    CsrBtSyncsGetCalInfoLogInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_IND
CsrUint8 *CsrBtSyncsGetCalInfoLogIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetCalInfoLogInd *primitive;

    primitive = (CsrBtSyncsGetCalInfoLogInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_IND
void *CsrBtSyncsGetCalInfoLogIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetCalInfoLogInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetCalInfoLogInd *) CsrPmemZalloc(sizeof(CsrBtSyncsGetCalInfoLogInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_INFO_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_IND
CsrSize CsrBtSyncsGetPbCurChangeLogIndSizeof(void *msg)
{
    CsrBtSyncsGetPbCurChangeLogInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_IND
CsrUint8 *CsrBtSyncsGetPbCurChangeLogIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetPbCurChangeLogInd *primitive;

    primitive = (CsrBtSyncsGetPbCurChangeLogInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_IND
void *CsrBtSyncsGetPbCurChangeLogIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetPbCurChangeLogInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetPbCurChangeLogInd *) CsrPmemZalloc(sizeof(CsrBtSyncsGetPbCurChangeLogInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_CFM
CsrSize CsrBtSyncsAuthenticateCfmSizeof(void *msg)
{
    CsrBtSyncsAuthenticateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_CFM
CsrUint8 *CsrBtSyncsAuthenticateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsAuthenticateCfm *primitive;

    primitive = (CsrBtSyncsAuthenticateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_CFM
void *CsrBtSyncsAuthenticateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsAuthenticateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsAuthenticateCfm *) CsrPmemZalloc(sizeof(CsrBtSyncsAuthenticateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES
CsrSize CsrBtSyncsPutCalAddEntryResSizeof(void *msg)
{
    CsrBtSyncsPutCalAddEntryRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->appParameterLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES
CsrUint8 *CsrBtSyncsPutCalAddEntryResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsPutCalAddEntryRes *primitive;

    primitive = (CsrBtSyncsPutCalAddEntryRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appParameterLength);
    CsrMemCpySer(buffer, length, &primitive->appParameter, sizeof(primitive->appParameter));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->appParameter)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->appParameter), ((CsrSize) (primitive->appParameterLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES
void *CsrBtSyncsPutCalAddEntryResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsPutCalAddEntryRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsPutCalAddEntryRes *) CsrPmemZalloc(sizeof(CsrBtSyncsPutCalAddEntryRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appParameterLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->appParameter), buffer, &offset, ((CsrSize) sizeof(primitive->appParameter)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->appParameterLength)
    {
        primitive->appParameter = CsrPmemZalloc(((CsrUint32) (primitive->appParameterLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->appParameter), buffer, &offset, ((CsrSize) (primitive->appParameterLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->appParameter = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES
void CsrBtSyncsPutCalAddEntryResSerFree(void *msg)
{
    CsrBtSyncsPutCalAddEntryRes *primitive;

    primitive = (CsrBtSyncsPutCalAddEntryRes *) msg;

    if(primitive->appParameter != NULL)
    {
        CsrPmemFree(primitive->appParameter);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_CAL_ADD_ENTRY_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES
CsrSize CsrBtSyncsGetPbCurChangeLogResSizeof(void *msg)
{
    CsrBtSyncsGetPbCurChangeLogRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->changeCounterLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES
CsrUint8 *CsrBtSyncsGetPbCurChangeLogResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetPbCurChangeLogRes *primitive;

    primitive = (CsrBtSyncsGetPbCurChangeLogRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalObjectSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->changeCounterLength);
    CsrMemCpySer(buffer, length, &primitive->changeCounter, sizeof(primitive->changeCounter));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->changeCounter)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->changeCounter), ((CsrSize) (primitive->changeCounterLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES
void *CsrBtSyncsGetPbCurChangeLogResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetPbCurChangeLogRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetPbCurChangeLogRes *) CsrPmemZalloc(sizeof(CsrBtSyncsGetPbCurChangeLogRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalObjectSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->changeCounterLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->changeCounter), buffer, &offset, ((CsrSize) sizeof(primitive->changeCounter)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->changeCounterLength)
    {
        primitive->changeCounter = CsrPmemZalloc(((CsrUint32) (primitive->changeCounterLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->changeCounter), buffer, &offset, ((CsrSize) (primitive->changeCounterLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->changeCounter = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES
void CsrBtSyncsGetPbCurChangeLogResSerFree(void *msg)
{
    CsrBtSyncsGetPbCurChangeLogRes *primitive;

    primitive = (CsrBtSyncsGetPbCurChangeLogRes *) msg;

    if(primitive->changeCounter != NULL)
    {
        CsrPmemFree(primitive->changeCounter);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CUR_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_RES
CsrSize CsrBtSyncsGetDeviceInfoResSizeof(void *msg)
{
    CsrBtSyncsGetDeviceInfoRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_RES
CsrUint8 *CsrBtSyncsGetDeviceInfoResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetDeviceInfoRes *primitive;

    primitive = (CsrBtSyncsGetDeviceInfoRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalObjectSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_RES
void *CsrBtSyncsGetDeviceInfoResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetDeviceInfoRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetDeviceInfoRes *) CsrPmemZalloc(sizeof(CsrBtSyncsGetDeviceInfoRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalObjectSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_RES
void CsrBtSyncsGetDeviceInfoResSerFree(void *msg)
{
    CsrBtSyncsGetDeviceInfoRes *primitive;

    primitive = (CsrBtSyncsGetDeviceInfoRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_DEVICE_INFO_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_NEXT_RES
CsrSize CsrBtSyncsGetNextResSizeof(void *msg)
{
    CsrBtSyncsGetNextRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_NEXT_RES
CsrUint8 *CsrBtSyncsGetNextResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetNextRes *primitive;

    primitive = (CsrBtSyncsGetNextRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_NEXT_RES
void *CsrBtSyncsGetNextResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetNextRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetNextRes *) CsrPmemZalloc(sizeof(CsrBtSyncsGetNextRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_NEXT_RES
void CsrBtSyncsGetNextResSerFree(void *msg)
{
    CsrBtSyncsGetNextRes *primitive;

    primitive = (CsrBtSyncsGetNextRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_NEXT_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES
CsrSize CsrBtSyncsGetCalCurChangeLogResSizeof(void *msg)
{
    CsrBtSyncsGetCalCurChangeLogRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->changeCounterLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES
CsrUint8 *CsrBtSyncsGetCalCurChangeLogResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetCalCurChangeLogRes *primitive;

    primitive = (CsrBtSyncsGetCalCurChangeLogRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalObjectSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->changeCounterLength);
    CsrMemCpySer(buffer, length, &primitive->changeCounter, sizeof(primitive->changeCounter));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->changeCounter)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->changeCounter), ((CsrSize) (primitive->changeCounterLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES
void *CsrBtSyncsGetCalCurChangeLogResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetCalCurChangeLogRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetCalCurChangeLogRes *) CsrPmemZalloc(sizeof(CsrBtSyncsGetCalCurChangeLogRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalObjectSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->changeCounterLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->changeCounter), buffer, &offset, ((CsrSize) sizeof(primitive->changeCounter)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->changeCounterLength)
    {
        primitive->changeCounter = CsrPmemZalloc(((CsrUint32) (primitive->changeCounterLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->changeCounter), buffer, &offset, ((CsrSize) (primitive->changeCounterLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->changeCounter = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES
void CsrBtSyncsGetCalCurChangeLogResSerFree(void *msg)
{
    CsrBtSyncsGetCalCurChangeLogRes *primitive;

    primitive = (CsrBtSyncsGetCalCurChangeLogRes *) msg;

    if(primitive->changeCounter != NULL)
    {
        CsrPmemFree(primitive->changeCounter);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_CAL_CUR_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES
CsrSize CsrBtSyncsGetPbChangeLogResSizeof(void *msg)
{
    CsrBtSyncsGetPbChangeLogRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bodyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES
CsrUint8 *CsrBtSyncsGetPbChangeLogResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsGetPbChangeLogRes *primitive;

    primitive = (CsrBtSyncsGetPbChangeLogRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->responseCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalObjectSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyLength);
    CsrMemCpySer(buffer, length, &primitive->body, sizeof(primitive->body));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->srmpOn);
    if(primitive->body)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->body), ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES
void *CsrBtSyncsGetPbChangeLogResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsGetPbChangeLogRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsGetPbChangeLogRes *) CsrPmemZalloc(sizeof(CsrBtSyncsGetPbChangeLogRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->responseCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalObjectSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->body), buffer, &offset, ((CsrSize) sizeof(primitive->body)));
    CsrUint8Des((CsrUint8 *) &primitive->srmpOn, buffer, &offset);
    if (primitive->bodyLength)
    {
        primitive->body = CsrPmemZalloc(((CsrUint32) (primitive->bodyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->body), buffer, &offset, ((CsrSize) (primitive->bodyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->body = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES
void CsrBtSyncsGetPbChangeLogResSerFree(void *msg)
{
    CsrBtSyncsGetPbChangeLogRes *primitive;

    primitive = (CsrBtSyncsGetPbChangeLogRes *) msg;

    if(primitive->body != NULL)
    {
        CsrPmemFree(primitive->body);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_GET_PB_CHANGE_LOG_RES */

#ifndef EXCLUDE_CSR_BT_SYNCS_ABORT_IND
CsrSize CsrBtSyncsAbortIndSizeof(void *msg)
{
    CsrBtSyncsAbortInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_ABORT_IND
CsrUint8 *CsrBtSyncsAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsAbortInd *primitive;

    primitive = (CsrBtSyncsAbortInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
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
#endif /* EXCLUDE_CSR_BT_SYNCS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_ABORT_IND
void *CsrBtSyncsAbortIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsAbortInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsAbortInd *) CsrPmemZalloc(sizeof(CsrBtSyncsAbortInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_ABORT_IND
void CsrBtSyncsAbortIndSerFree(void *msg)
{
    CsrBtSyncsAbortInd *primitive;

    primitive = (CsrBtSyncsAbortInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_IND
CsrSize CsrBtSyncsPutNextIndSizeof(void *msg)
{
    CsrBtSyncsPutNextInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_IND
CsrUint8 *CsrBtSyncsPutNextIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSyncsPutNextInd *primitive;

    primitive = (CsrBtSyncsPutNextInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->finalFlag);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bodyLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bodyOffset);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_IND
void *CsrBtSyncsPutNextIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSyncsPutNextInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSyncsPutNextInd *) CsrPmemZalloc(sizeof(CsrBtSyncsPutNextInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->finalFlag, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bodyLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bodyOffset, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_IND */

#ifndef EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_IND
void CsrBtSyncsPutNextIndSerFree(void *msg)
{
    CsrBtSyncsPutNextInd *primitive;

    primitive = (CsrBtSyncsPutNextInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SYNCS_PUT_NEXT_IND */

#endif /* EXCLUDE_CSR_BT_SYNCS_MODULE */
