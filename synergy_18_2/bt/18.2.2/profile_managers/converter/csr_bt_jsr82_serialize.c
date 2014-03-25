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
#include "csr_bt_jsr82_prim.h"

#ifndef EXCLUDE_CSR_BT_JSR82_MODULE

void CsrBtJsr82Pfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_JSR82_DL_DEVICE_DISCOVERED_IND
CsrSize CsrBtJsr82DlDeviceDiscoveredIndSizeof(void *msg)
{
    CsrBtJsr82DlDeviceDiscoveredInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_DL_DEVICE_DISCOVERED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_DL_DEVICE_DISCOVERED_IND
CsrUint8 *CsrBtJsr82DlDeviceDiscoveredIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82DlDeviceDiscoveredInd *primitive;

    primitive = (CsrBtJsr82DlDeviceDiscoveredInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->classOfDevice);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rssi);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DL_DEVICE_DISCOVERED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_DL_DEVICE_DISCOVERED_IND
void *CsrBtJsr82DlDeviceDiscoveredIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82DlDeviceDiscoveredInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82DlDeviceDiscoveredInd *) CsrPmemZalloc(sizeof(CsrBtJsr82DlDeviceDiscoveredInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->classOfDevice, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->rssi, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DL_DEVICE_DISCOVERED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_REQ
CsrSize CsrBtJsr82RfcDisconnectReqSizeof(void *msg)
{
    CsrBtJsr82RfcDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_REQ
CsrUint8 *CsrBtJsr82RfcDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcDisconnectReq *primitive;

    primitive = (CsrBtJsr82RfcDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_REQ
void *CsrBtJsr82RfcDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_CFM
CsrSize CsrBtJsr82L2caRxReadyCfmSizeof(void *msg)
{
    CsrBtJsr82L2caRxReadyCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_CFM
CsrUint8 *CsrBtJsr82L2caRxReadyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caRxReadyCfm *primitive;

    primitive = (CsrBtJsr82L2caRxReadyCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->dataReady);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_CFM
void *CsrBtJsr82L2caRxReadyCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caRxReadyCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caRxReadyCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caRxReadyCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->dataReady, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_REQ
CsrSize CsrBtJsr82RfcCloseReqSizeof(void *msg)
{
    CsrBtJsr82RfcCloseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_REQ
CsrUint8 *CsrBtJsr82RfcCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcCloseReq *primitive;

    primitive = (CsrBtJsr82RfcCloseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_REQ
void *CsrBtJsr82RfcCloseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcCloseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcCloseReq *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcCloseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ
CsrSize CsrBtJsr82RdIsEncryptedReqSizeof(void *msg)
{
    CsrBtJsr82RdIsEncryptedReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ
CsrUint8 *CsrBtJsr82RdIsEncryptedReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RdIsEncryptedReq *primitive;

    primitive = (CsrBtJsr82RdIsEncryptedReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ
void *CsrBtJsr82RdIsEncryptedReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RdIsEncryptedReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RdIsEncryptedReq *) CsrPmemZalloc(sizeof(CsrBtJsr82RdIsEncryptedReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_REQ
CsrSize CsrBtJsr82DaSelectServiceReqSizeof(void *msg)
{
    CsrBtJsr82DaSelectServiceReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->requestedUuidLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_REQ
CsrUint8 *CsrBtJsr82DaSelectServiceReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82DaSelectServiceReq *primitive;

    primitive = (CsrBtJsr82DaSelectServiceReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->requestedUuidLength);
    CsrMemCpySer(buffer, length, &primitive->requestedUuid, sizeof(primitive->requestedUuid));
    if(primitive->requestedUuid)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->requestedUuid), ((CsrSize) (primitive->requestedUuidLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_REQ
void *CsrBtJsr82DaSelectServiceReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82DaSelectServiceReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82DaSelectServiceReq *) CsrPmemZalloc(sizeof(CsrBtJsr82DaSelectServiceReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->requestedUuidLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->requestedUuid), buffer, &offset, ((CsrSize) sizeof(primitive->requestedUuid)));
    if (primitive->requestedUuidLength)
    {
        primitive->requestedUuid = CsrPmemZalloc(((CsrUint32) (primitive->requestedUuidLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->requestedUuid), buffer, &offset, ((CsrSize) (primitive->requestedUuidLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->requestedUuid = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_REQ
void CsrBtJsr82DaSelectServiceReqSerFree(void *msg)
{
    CsrBtJsr82DaSelectServiceReq *primitive;

    primitive = (CsrBtJsr82DaSelectServiceReq *) msg;

    if(primitive->requestedUuid != NULL)
    {
        CsrPmemFree(primitive->requestedUuid);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_REQ
CsrSize CsrBtJsr82SrRemoveRecordReqSizeof(void *msg)
{
    CsrBtJsr82SrRemoveRecordReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_REQ
CsrUint8 *CsrBtJsr82SrRemoveRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82SrRemoveRecordReq *primitive;

    primitive = (CsrBtJsr82SrRemoveRecordReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceRecHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_REQ
void *CsrBtJsr82SrRemoveRecordReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82SrRemoveRecordReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82SrRemoveRecordReq *) CsrPmemZalloc(sizeof(CsrBtJsr82SrRemoveRecordReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceRecHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_CFM
CsrSize CsrBtJsr82L2caRxDataCfmSizeof(void *msg)
{
    CsrBtJsr82L2caRxDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_CFM
CsrUint8 *CsrBtJsr82L2caRxDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caRxDataCfm *primitive;

    primitive = (CsrBtJsr82L2caRxDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_CFM
void *CsrBtJsr82L2caRxDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caRxDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caRxDataCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caRxDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_CFM
void CsrBtJsr82L2caRxDataCfmSerFree(void *msg)
{
    CsrBtJsr82L2caRxDataCfm *primitive;

    primitive = (CsrBtJsr82L2caRxDataCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_CFM
CsrSize CsrBtJsr82RdAuthenticateCfmSizeof(void *msg)
{
    CsrBtJsr82RdAuthenticateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_CFM
CsrUint8 *CsrBtJsr82RdAuthenticateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RdAuthenticateCfm *primitive;

    primitive = (CsrBtJsr82RdAuthenticateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_CFM
void *CsrBtJsr82RdAuthenticateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RdAuthenticateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RdAuthenticateCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82RdAuthenticateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_REQ
CsrSize CsrBtJsr82L2caAcceptReqSizeof(void *msg)
{
    CsrBtJsr82L2caAcceptReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_REQ
CsrUint8 *CsrBtJsr82L2caAcceptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caAcceptReq *primitive;

    primitive = (CsrBtJsr82L2caAcceptReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->receiveMtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->transmitMtu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->attrs);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_REQ
void *CsrBtJsr82L2caAcceptReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caAcceptReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caAcceptReq *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caAcceptReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->receiveMtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->transmitMtu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->attrs, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_REQ
CsrSize CsrBtJsr82L2caOpenReqSizeof(void *msg)
{
    CsrBtJsr82L2caOpenReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_REQ
CsrUint8 *CsrBtJsr82L2caOpenReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caOpenReq *primitive;

    primitive = (CsrBtJsr82L2caOpenReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remotePsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->receiveMtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->transmitMtu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->attrs);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_REQ
void *CsrBtJsr82L2caOpenReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caOpenReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caOpenReq *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caOpenReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remotePsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->receiveMtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->transmitMtu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->attrs, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_CFM
CsrSize CsrBtJsr82LdUpdateRecordCfmSizeof(void *msg)
{
    CsrBtJsr82LdUpdateRecordCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_CFM
CsrUint8 *CsrBtJsr82LdUpdateRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdUpdateRecordCfm *primitive;

    primitive = (CsrBtJsr82LdUpdateRecordCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceRecHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_CFM
void *CsrBtJsr82LdUpdateRecordCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdUpdateRecordCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdUpdateRecordCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82LdUpdateRecordCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceRecHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM
CsrSize CsrBtJsr82RfcReceiveDataCfmSizeof(void *msg)
{
    CsrBtJsr82RfcReceiveDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM
CsrUint8 *CsrBtJsr82RfcReceiveDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcReceiveDataCfm *primitive;

    primitive = (CsrBtJsr82RfcReceiveDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM
void *CsrBtJsr82RfcReceiveDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcReceiveDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcReceiveDataCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcReceiveDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM
void CsrBtJsr82RfcReceiveDataCfmSerFree(void *msg)
{
    CsrBtJsr82RfcReceiveDataCfm *primitive;

    primitive = (CsrBtJsr82RfcReceiveDataCfm *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_REQ
CsrSize CsrBtJsr82RdEncryptReqSizeof(void *msg)
{
    CsrBtJsr82RdEncryptReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_REQ
CsrUint8 *CsrBtJsr82RdEncryptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RdEncryptReq *primitive;

    primitive = (CsrBtJsr82RdEncryptReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->encrypt);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_REQ
void *CsrBtJsr82RdEncryptReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RdEncryptReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RdEncryptReq *) CsrPmemZalloc(sizeof(CsrBtJsr82RdEncryptReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->encrypt, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_REQ
CsrSize CsrBtJsr82RfcConnectReqSizeof(void *msg)
{
    CsrBtJsr82RfcConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_REQ
CsrUint8 *CsrBtJsr82RfcConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcConnectReq *primitive;

    primitive = (CsrBtJsr82RfcConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->remoteServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->attrs);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_REQ
void *CsrBtJsr82RfcConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcConnectReq *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->remoteServerChannel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->attrs, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_CFM
CsrSize CsrBtJsr82SrPopulateRecordCfmSizeof(void *msg)
{
    CsrBtJsr82SrPopulateRecordCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attributesLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_CFM
CsrUint8 *CsrBtJsr82SrPopulateRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82SrPopulateRecordCfm *primitive;

    primitive = (CsrBtJsr82SrPopulateRecordCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attributesLength);
    CsrMemCpySer(buffer, length, &primitive->attributes, sizeof(primitive->attributes));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    if(primitive->attributes)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attributes), ((CsrSize) (primitive->attributesLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_CFM
void *CsrBtJsr82SrPopulateRecordCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82SrPopulateRecordCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82SrPopulateRecordCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82SrPopulateRecordCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attributesLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attributes), buffer, &offset, ((CsrSize) sizeof(primitive->attributes)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    if (primitive->attributesLength)
    {
        primitive->attributes = CsrPmemZalloc(((CsrUint32) (primitive->attributesLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->attributes), buffer, &offset, ((CsrSize) (primitive->attributesLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->attributes = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_CFM
void CsrBtJsr82SrPopulateRecordCfmSerFree(void *msg)
{
    CsrBtJsr82SrPopulateRecordCfm *primitive;

    primitive = (CsrBtJsr82SrPopulateRecordCfm *) msg;

    if(primitive->attributes != NULL)
    {
        CsrPmemFree(primitive->attributes);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_REQ
CsrSize CsrBtJsr82L2caTxDataReqSizeof(void *msg)
{
    CsrBtJsr82L2caTxDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_REQ
CsrUint8 *CsrBtJsr82L2caTxDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caTxDataReq *primitive;

    primitive = (CsrBtJsr82L2caTxDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_REQ
void *CsrBtJsr82L2caTxDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caTxDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caTxDataReq *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caTxDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_REQ
void CsrBtJsr82L2caTxDataReqSerFree(void *msg)
{
    CsrBtJsr82L2caTxDataReq *primitive;

    primitive = (CsrBtJsr82L2caTxDataReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM
CsrSize CsrBtJsr82RdGetFriendlyNameCfmSizeof(void *msg)
{
    CsrBtJsr82RdGetFriendlyNameCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->remoteName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM
CsrUint8 *CsrBtJsr82RdGetFriendlyNameCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RdGetFriendlyNameCfm *primitive;

    primitive = (CsrBtJsr82RdGetFriendlyNameCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrMemCpySer(buffer, length, &primitive->remoteName, sizeof(primitive->remoteName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUtf8StringSer(buffer, length, primitive->remoteName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM
void *CsrBtJsr82RdGetFriendlyNameCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RdGetFriendlyNameCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RdGetFriendlyNameCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82RdGetFriendlyNameCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->remoteName), buffer, &offset, ((CsrSize) sizeof(primitive->remoteName)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUtf8StringDes(&primitive->remoteName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM
void CsrBtJsr82RdGetFriendlyNameCfmSerFree(void *msg)
{
    CsrBtJsr82RdGetFriendlyNameCfm *primitive;

    primitive = (CsrBtJsr82RdGetFriendlyNameCfm *) msg;

    if(primitive->remoteName != NULL)
    {
        CsrPmemFree(primitive->remoteName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_CFM
CsrSize CsrBtJsr82SrRegisterRecordCfmSizeof(void *msg)
{
    CsrBtJsr82SrRegisterRecordCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_CFM
CsrUint8 *CsrBtJsr82SrRegisterRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82SrRegisterRecordCfm *primitive;

    primitive = (CsrBtJsr82SrRegisterRecordCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceRecHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_CFM
void *CsrBtJsr82SrRegisterRecordCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82SrRegisterRecordCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82SrRegisterRecordCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82SrRegisterRecordCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceRecHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_IND
CsrSize CsrBtJsr82RfcCloseIndSizeof(void *msg)
{
    CsrBtJsr82RfcCloseInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_IND
CsrUint8 *CsrBtJsr82RfcCloseIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcCloseInd *primitive;

    primitive = (CsrBtJsr82RfcCloseInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localTerminated);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_IND
void *CsrBtJsr82RfcCloseIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcCloseInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcCloseInd *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcCloseInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localTerminated, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_HOUSE_CLEANING
CsrSize CsrBtJsr82HouseCleaningSizeof(void *msg)
{
    CsrBtJsr82HouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_JSR82_HOUSE_CLEANING
CsrUint8 *CsrBtJsr82HouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82HouseCleaning *primitive;

    primitive = (CsrBtJsr82HouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_JSR82_HOUSE_CLEANING
void *CsrBtJsr82HouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82HouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82HouseCleaning *) CsrPmemZalloc(sizeof(CsrBtJsr82HouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_REQ
CsrSize CsrBtJsr82L2caRxDataReqSizeof(void *msg)
{
    CsrBtJsr82L2caRxDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_REQ
CsrUint8 *CsrBtJsr82L2caRxDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caRxDataReq *primitive;

    primitive = (CsrBtJsr82L2caRxDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_REQ
void *CsrBtJsr82L2caRxDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caRxDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caRxDataReq *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caRxDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_CFM
CsrSize CsrBtJsr82LdGetSecurityLevelCfmSizeof(void *msg)
{
    CsrBtJsr82LdGetSecurityLevelCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_CFM
CsrUint8 *CsrBtJsr82LdGetSecurityLevelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdGetSecurityLevelCfm *primitive;

    primitive = (CsrBtJsr82LdGetSecurityLevelCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_CFM
void *CsrBtJsr82LdGetSecurityLevelCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdGetSecurityLevelCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdGetSecurityLevelCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82LdGetSecurityLevelCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ
CsrSize CsrBtJsr82RfcGetServerChannelReqSizeof(void *msg)
{
    CsrBtJsr82RfcGetServerChannelReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ
CsrUint8 *CsrBtJsr82RfcGetServerChannelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcGetServerChannelReq *primitive;

    primitive = (CsrBtJsr82RfcGetServerChannelReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ
void *CsrBtJsr82RfcGetServerChannelReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcGetServerChannelReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcGetServerChannelReq *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcGetServerChannelReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_IND
CsrSize CsrBtJsr82L2caCloseIndSizeof(void *msg)
{
    CsrBtJsr82L2caCloseInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_IND
CsrUint8 *CsrBtJsr82L2caCloseIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caCloseInd *primitive;

    primitive = (CsrBtJsr82L2caCloseInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localTerminated);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_IND
void *CsrBtJsr82L2caCloseIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caCloseInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caCloseInd *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caCloseInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localTerminated, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ
CsrSize CsrBtJsr82RdIsAuthenticatedReqSizeof(void *msg)
{
    CsrBtJsr82RdIsAuthenticatedReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ
CsrUint8 *CsrBtJsr82RdIsAuthenticatedReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RdIsAuthenticatedReq *primitive;

    primitive = (CsrBtJsr82RdIsAuthenticatedReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ
void *CsrBtJsr82RdIsAuthenticatedReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RdIsAuthenticatedReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RdIsAuthenticatedReq *) CsrPmemZalloc(sizeof(CsrBtJsr82RdIsAuthenticatedReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_REQ
CsrSize CsrBtJsr82L2caGetConfigReqSizeof(void *msg)
{
    CsrBtJsr82L2caGetConfigReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_REQ
CsrUint8 *CsrBtJsr82L2caGetConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caGetConfigReq *primitive;

    primitive = (CsrBtJsr82L2caGetConfigReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_REQ
void *CsrBtJsr82L2caGetConfigReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caGetConfigReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caGetConfigReq *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caGetConfigReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_REQ
CsrSize CsrBtJsr82LdUpdateRecordReqSizeof(void *msg)
{
    CsrBtJsr82LdUpdateRecordReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->serviceRecordSize * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_REQ
CsrUint8 *CsrBtJsr82LdUpdateRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdUpdateRecordReq *primitive;

    primitive = (CsrBtJsr82LdUpdateRecordReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceRecHandle);
    CsrMemCpySer(buffer, length, &primitive->serviceRecord, sizeof(primitive->serviceRecord));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->serviceRecordSize);
    if(primitive->serviceRecord)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->serviceRecord), ((CsrSize) (primitive->serviceRecordSize * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_REQ
void *CsrBtJsr82LdUpdateRecordReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdUpdateRecordReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdUpdateRecordReq *) CsrPmemZalloc(sizeof(CsrBtJsr82LdUpdateRecordReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceRecHandle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceRecord), buffer, &offset, ((CsrSize) sizeof(primitive->serviceRecord)));
    CsrUint16Des((CsrUint16 *) &primitive->serviceRecordSize, buffer, &offset);
    if (primitive->serviceRecordSize)
    {
        primitive->serviceRecord = CsrPmemZalloc(((CsrUint32) (primitive->serviceRecordSize * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->serviceRecord), buffer, &offset, ((CsrSize) (primitive->serviceRecordSize * sizeof(CsrUint8))));
    }
    else
    {
        primitive->serviceRecord = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_REQ
void CsrBtJsr82LdUpdateRecordReqSerFree(void *msg)
{
    CsrBtJsr82LdUpdateRecordReq *primitive;

    primitive = (CsrBtJsr82LdUpdateRecordReq *) msg;

    if(primitive->serviceRecord != NULL)
    {
        CsrPmemFree(primitive->serviceRecord);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_UPDATE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_CFM
CsrSize CsrBtJsr82L2caAcceptCfmSizeof(void *msg)
{
    CsrBtJsr82L2caAcceptCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_CFM
CsrUint8 *CsrBtJsr82L2caAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caAcceptCfm *primitive;

    primitive = (CsrBtJsr82L2caAcceptCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->transmitMtu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_CFM
void *CsrBtJsr82L2caAcceptCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caAcceptCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caAcceptCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caAcceptCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->transmitMtu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ
CsrSize CsrBtJsr82LdSetDiscoverableReqSizeof(void *msg)
{
    CsrBtJsr82LdSetDiscoverableReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ
CsrUint8 *CsrBtJsr82LdSetDiscoverableReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdSetDiscoverableReq *primitive;

    primitive = (CsrBtJsr82LdSetDiscoverableReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ
void *CsrBtJsr82LdSetDiscoverableReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdSetDiscoverableReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdSetDiscoverableReq *) CsrPmemZalloc(sizeof(CsrBtJsr82LdSetDiscoverableReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ
CsrSize CsrBtJsr82RfcReceiveDataReqSizeof(void *msg)
{
    CsrBtJsr82RfcReceiveDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ
CsrUint8 *CsrBtJsr82RfcReceiveDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcReceiveDataReq *primitive;

    primitive = (CsrBtJsr82RfcReceiveDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bytesToRead);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ
void *CsrBtJsr82RfcReceiveDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcReceiveDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcReceiveDataReq *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcReceiveDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bytesToRead, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_RECEIVE_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_CFM
CsrSize CsrBtJsr82RdIsEncryptedCfmSizeof(void *msg)
{
    CsrBtJsr82RdIsEncryptedCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_CFM
CsrUint8 *CsrBtJsr82RdIsEncryptedCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RdIsEncryptedCfm *primitive;

    primitive = (CsrBtJsr82RdIsEncryptedCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->encrypted);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_CFM
void *CsrBtJsr82RdIsEncryptedCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RdIsEncryptedCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RdIsEncryptedCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82RdIsEncryptedCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->encrypted, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_ENCRYPTED_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_REQ
CsrSize CsrBtJsr82L2caRxReadyReqSizeof(void *msg)
{
    CsrBtJsr82L2caRxReadyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_REQ
CsrUint8 *CsrBtJsr82L2caRxReadyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caRxReadyReq *primitive;

    primitive = (CsrBtJsr82L2caRxReadyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_REQ
void *CsrBtJsr82L2caRxReadyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caRxReadyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caRxReadyReq *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caRxReadyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_RX_READY_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_REQ
CsrSize CsrBtJsr82SetEventMaskReqSizeof(void *msg)
{
    CsrBtJsr82SetEventMaskReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_REQ
CsrUint8 *CsrBtJsr82SetEventMaskReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82SetEventMaskReq *primitive;

    primitive = (CsrBtJsr82SetEventMaskReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->eventMask);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->conditionMask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_REQ
void *CsrBtJsr82SetEventMaskReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82SetEventMaskReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82SetEventMaskReq *) CsrPmemZalloc(sizeof(CsrBtJsr82SetEventMaskReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->eventMask, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->conditionMask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ
CsrSize CsrBtJsr82LdGetSecurityLevelReqSizeof(void *msg)
{
    CsrBtJsr82LdGetSecurityLevelReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ
CsrUint8 *CsrBtJsr82LdGetSecurityLevelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdGetSecurityLevelReq *primitive;

    primitive = (CsrBtJsr82LdGetSecurityLevelReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ
void *CsrBtJsr82LdGetSecurityLevelReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdGetSecurityLevelReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdGetSecurityLevelReq *) CsrPmemZalloc(sizeof(CsrBtJsr82LdGetSecurityLevelReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_SECURITY_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_CFM
CsrSize CsrBtJsr82LdGetBluetoothAddressCfmSizeof(void *msg)
{
    CsrBtJsr82LdGetBluetoothAddressCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_CFM
CsrUint8 *CsrBtJsr82LdGetBluetoothAddressCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdGetBluetoothAddressCfm *primitive;

    primitive = (CsrBtJsr82LdGetBluetoothAddressCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_CFM
void *CsrBtJsr82LdGetBluetoothAddressCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdGetBluetoothAddressCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdGetBluetoothAddressCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82LdGetBluetoothAddressCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_CFM
CsrSize CsrBtJsr82RdIsAuthenticatedCfmSizeof(void *msg)
{
    CsrBtJsr82RdIsAuthenticatedCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_CFM
CsrUint8 *CsrBtJsr82RdIsAuthenticatedCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RdIsAuthenticatedCfm *primitive;

    primitive = (CsrBtJsr82RdIsAuthenticatedCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authenticated);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_CFM
void *CsrBtJsr82RdIsAuthenticatedCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RdIsAuthenticatedCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RdIsAuthenticatedCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82RdIsAuthenticatedCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authenticated, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_AUTHENTICATED_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_CFM
CsrSize CsrBtJsr82DaSelectServiceCfmSizeof(void *msg)
{
    CsrBtJsr82DaSelectServiceCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_CFM
CsrUint8 *CsrBtJsr82DaSelectServiceCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82DaSelectServiceCfm *primitive;

    primitive = (CsrBtJsr82DaSelectServiceCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->protocol);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->chanPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_CFM
void *CsrBtJsr82DaSelectServiceCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82DaSelectServiceCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82DaSelectServiceCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82DaSelectServiceCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->protocol, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->chanPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SELECT_SERVICE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_CFM
CsrSize CsrBtJsr82L2caGetConfigCfmSizeof(void *msg)
{
    CsrBtJsr82L2caGetConfigCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_CFM
CsrUint8 *CsrBtJsr82L2caGetConfigCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caGetConfigCfm *primitive;

    primitive = (CsrBtJsr82L2caGetConfigCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->receiveMTU);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->transmitMTU);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_CFM
void *CsrBtJsr82L2caGetConfigCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caGetConfigCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caGetConfigCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caGetConfigCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->receiveMTU, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->transmitMTU, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_CLEANUP_REQ
CsrSize CsrBtJsr82CleanupReqSizeof(void *msg)
{
    CsrBtJsr82CleanupReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_CLEANUP_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_CLEANUP_REQ
CsrUint8 *CsrBtJsr82CleanupReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82CleanupReq *primitive;

    primitive = (CsrBtJsr82CleanupReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_CLEANUP_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_CLEANUP_REQ
void *CsrBtJsr82CleanupReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82CleanupReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82CleanupReq *) CsrPmemZalloc(sizeof(CsrBtJsr82CleanupReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_CLEANUP_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_REQ
CsrSize CsrBtJsr82LdGetPropertyReqSizeof(void *msg)
{
    CsrBtJsr82LdGetPropertyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_REQ
CsrUint8 *CsrBtJsr82LdGetPropertyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdGetPropertyReq *primitive;

    primitive = (CsrBtJsr82LdGetPropertyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->propertyName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_REQ
void *CsrBtJsr82LdGetPropertyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdGetPropertyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdGetPropertyReq *) CsrPmemZalloc(sizeof(CsrBtJsr82LdGetPropertyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->propertyName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ
CsrSize CsrBtJsr82DaCancelServiceSearchReqSizeof(void *msg)
{
    CsrBtJsr82DaCancelServiceSearchReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ
CsrUint8 *CsrBtJsr82DaCancelServiceSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82DaCancelServiceSearchReq *primitive;

    primitive = (CsrBtJsr82DaCancelServiceSearchReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ
void *CsrBtJsr82DaCancelServiceSearchReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82DaCancelServiceSearchReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82DaCancelServiceSearchReq *) CsrPmemZalloc(sizeof(CsrBtJsr82DaCancelServiceSearchReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_CANCEL_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_REQ
CsrSize CsrBtJsr82SrPopulateRecordReqSizeof(void *msg)
{
    CsrBtJsr82SrPopulateRecordReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attrSetLength * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_REQ
CsrUint8 *CsrBtJsr82SrPopulateRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82SrPopulateRecordReq *primitive;

    primitive = (CsrBtJsr82SrPopulateRecordReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceDataBaseState);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceRecHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attrSetLength);
    CsrMemCpySer(buffer, length, &primitive->attrSet, sizeof(primitive->attrSet));
    if(primitive->attrSet)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attrSet), ((CsrSize) (primitive->attrSetLength * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_REQ
void *CsrBtJsr82SrPopulateRecordReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82SrPopulateRecordReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82SrPopulateRecordReq *) CsrPmemZalloc(sizeof(CsrBtJsr82SrPopulateRecordReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceDataBaseState, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceRecHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attrSetLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attrSet), buffer, &offset, ((CsrSize) sizeof(primitive->attrSet)));
    if (primitive->attrSetLength)
    {
        primitive->attrSet = CsrPmemZalloc(((CsrUint32) (primitive->attrSetLength * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->attrSet), buffer, &offset, ((CsrSize) (primitive->attrSetLength * sizeof(CsrUint16))));
    }
    else
    {
        primitive->attrSet = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_REQ
void CsrBtJsr82SrPopulateRecordReqSerFree(void *msg)
{
    CsrBtJsr82SrPopulateRecordReq *primitive;

    primitive = (CsrBtJsr82SrPopulateRecordReq *) msg;

    if(primitive->attrSet != NULL)
    {
        CsrPmemFree(primitive->attrSet);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_POPULATE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_REQ
CsrSize CsrBtJsr82L2caCloseReqSizeof(void *msg)
{
    CsrBtJsr82L2caCloseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_REQ
CsrUint8 *CsrBtJsr82L2caCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caCloseReq *primitive;

    primitive = (CsrBtJsr82L2caCloseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_REQ
void *CsrBtJsr82L2caCloseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caCloseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caCloseReq *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caCloseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ
CsrSize CsrBtJsr82DaCancelInquiryReqSizeof(void *msg)
{
    CsrBtJsr82DaCancelInquiryReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ
CsrUint8 *CsrBtJsr82DaCancelInquiryReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82DaCancelInquiryReq *primitive;

    primitive = (CsrBtJsr82DaCancelInquiryReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ
void *CsrBtJsr82DaCancelInquiryReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82DaCancelInquiryReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82DaCancelInquiryReq *) CsrPmemZalloc(sizeof(CsrBtJsr82DaCancelInquiryReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_CANCEL_INQUIRY_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_CFM
CsrSize CsrBtJsr82RfcAcceptAndOpenCfmSizeof(void *msg)
{
    CsrBtJsr82RfcAcceptAndOpenCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_CFM
CsrUint8 *CsrBtJsr82RfcAcceptAndOpenCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcAcceptAndOpenCfm *primitive;

    primitive = (CsrBtJsr82RfcAcceptAndOpenCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_CFM
void *CsrBtJsr82RfcAcceptAndOpenCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcAcceptAndOpenCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcAcceptAndOpenCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcAcceptAndOpenCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_DL_INQUIRY_COMPLETED_IND
CsrSize CsrBtJsr82DlInquiryCompletedIndSizeof(void *msg)
{
    CsrBtJsr82DlInquiryCompletedInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_DL_INQUIRY_COMPLETED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_DL_INQUIRY_COMPLETED_IND
CsrUint8 *CsrBtJsr82DlInquiryCompletedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82DlInquiryCompletedInd *primitive;

    primitive = (CsrBtJsr82DlInquiryCompletedInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->respCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DL_INQUIRY_COMPLETED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_DL_INQUIRY_COMPLETED_IND
void *CsrBtJsr82DlInquiryCompletedIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82DlInquiryCompletedInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82DlInquiryCompletedInd *) CsrPmemZalloc(sizeof(CsrBtJsr82DlInquiryCompletedInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->respCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DL_INQUIRY_COMPLETED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ
CsrSize CsrBtJsr82LdGetDeviceClassReqSizeof(void *msg)
{
    CsrBtJsr82LdGetDeviceClassReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ
CsrUint8 *CsrBtJsr82LdGetDeviceClassReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdGetDeviceClassReq *primitive;

    primitive = (CsrBtJsr82LdGetDeviceClassReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->fieldsMask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ
void *CsrBtJsr82LdGetDeviceClassReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdGetDeviceClassReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdGetDeviceClassReq *) CsrPmemZalloc(sizeof(CsrBtJsr82LdGetDeviceClassReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->fieldsMask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_CFM
CsrSize CsrBtJsr82SrCreateRecordCfmSizeof(void *msg)
{
    CsrBtJsr82SrCreateRecordCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_CFM
CsrUint8 *CsrBtJsr82SrCreateRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82SrCreateRecordCfm *primitive;

    primitive = (CsrBtJsr82SrCreateRecordCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceRecHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_CFM
void *CsrBtJsr82SrCreateRecordCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82SrCreateRecordCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82SrCreateRecordCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82SrCreateRecordCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceRecHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_REQ
CsrSize CsrBtJsr82RdAuthenticateReqSizeof(void *msg)
{
    CsrBtJsr82RdAuthenticateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_REQ
CsrUint8 *CsrBtJsr82RdAuthenticateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RdAuthenticateReq *primitive;

    primitive = (CsrBtJsr82RdAuthenticateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_REQ
void *CsrBtJsr82RdAuthenticateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RdAuthenticateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RdAuthenticateReq *) CsrPmemZalloc(sizeof(CsrBtJsr82RdAuthenticateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM
CsrSize CsrBtJsr82DaRetrieveDevicesCfmSizeof(void *msg)
{
    CsrBtJsr82DaRetrieveDevicesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->devicesCount * sizeof(CsrBtDeviceAddr)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM
CsrUint8 *CsrBtJsr82DaRetrieveDevicesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82DaRetrieveDevicesCfm *primitive;

    primitive = (CsrBtJsr82DaRetrieveDevicesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->devicesCount);
    CsrMemCpySer(buffer, length, &primitive->devices, sizeof(primitive->devices));
    if(primitive->devices)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->devices), ((CsrSize) (primitive->devicesCount * sizeof(CsrBtDeviceAddr))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM
void *CsrBtJsr82DaRetrieveDevicesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82DaRetrieveDevicesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82DaRetrieveDevicesCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82DaRetrieveDevicesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->devicesCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->devices), buffer, &offset, ((CsrSize) sizeof(primitive->devices)));
    if (primitive->devicesCount)
    {
        primitive->devices = CsrPmemZalloc(((CsrUint32) (primitive->devicesCount * sizeof(CsrBtDeviceAddr))));
        CsrMemCpyDes(((void *) primitive->devices), buffer, &offset, ((CsrSize) (primitive->devicesCount * sizeof(CsrBtDeviceAddr))));
    }
    else
    {
        primitive->devices = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM
void CsrBtJsr82DaRetrieveDevicesCfmSerFree(void *msg)
{
    CsrBtJsr82DaRetrieveDevicesCfm *primitive;

    primitive = (CsrBtJsr82DaRetrieveDevicesCfm *) msg;

    if(primitive->devices != NULL)
    {
        CsrPmemFree(primitive->devices);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_CFM
CsrSize CsrBtJsr82RfcConnectCfmSizeof(void *msg)
{
    CsrBtJsr82RfcConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_CFM
CsrUint8 *CsrBtJsr82RfcConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcConnectCfm *primitive;

    primitive = (CsrBtJsr82RfcConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_CFM
void *CsrBtJsr82RfcConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcConnectCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ
CsrSize CsrBtJsr82RdGetFriendlyNameReqSizeof(void *msg)
{
    CsrBtJsr82RdGetFriendlyNameReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ
CsrUint8 *CsrBtJsr82RdGetFriendlyNameReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RdGetFriendlyNameReq *primitive;

    primitive = (CsrBtJsr82RdGetFriendlyNameReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->alwaysAsk);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ
void *CsrBtJsr82RdGetFriendlyNameReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RdGetFriendlyNameReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RdGetFriendlyNameReq *) CsrPmemZalloc(sizeof(CsrBtJsr82RdGetFriendlyNameReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->alwaysAsk, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_GET_FRIENDLY_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_REQ
CsrSize CsrBtJsr82RfcSendDataReqSizeof(void *msg)
{
    CsrBtJsr82RfcSendDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_REQ
CsrUint8 *CsrBtJsr82RfcSendDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcSendDataReq *primitive;

    primitive = (CsrBtJsr82RfcSendDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_REQ
void *CsrBtJsr82RfcSendDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcSendDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcSendDataReq *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcSendDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_REQ
void CsrBtJsr82RfcSendDataReqSerFree(void *msg)
{
    CsrBtJsr82RfcSendDataReq *primitive;

    primitive = (CsrBtJsr82RfcSendDataReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ
CsrSize CsrBtJsr82RfcAcceptAndOpenReqSizeof(void *msg)
{
    CsrBtJsr82RfcAcceptAndOpenReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ
CsrUint8 *CsrBtJsr82RfcAcceptAndOpenReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcAcceptAndOpenReq *primitive;

    primitive = (CsrBtJsr82RfcAcceptAndOpenReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->attrs);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ
void *CsrBtJsr82RfcAcceptAndOpenReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcAcceptAndOpenReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcAcceptAndOpenReq *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcAcceptAndOpenReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->attrs, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_ACCEPT_AND_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_CONTINUE_RFC_SEND_DATA
CsrSize CsrBtJsr82ContinueRfcSendDataSizeof(void *msg)
{
    CsrBtJsr82ContinueRfcSendData *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_CONTINUE_RFC_SEND_DATA */

#ifndef EXCLUDE_CSR_BT_JSR82_CONTINUE_RFC_SEND_DATA
CsrUint8 *CsrBtJsr82ContinueRfcSendDataSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82ContinueRfcSendData *primitive;

    primitive = (CsrBtJsr82ContinueRfcSendData *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_CONTINUE_RFC_SEND_DATA */

#ifndef EXCLUDE_CSR_BT_JSR82_CONTINUE_RFC_SEND_DATA
void *CsrBtJsr82ContinueRfcSendDataDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82ContinueRfcSendData *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82ContinueRfcSendData *) CsrPmemZalloc(sizeof(CsrBtJsr82ContinueRfcSendData));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_CONTINUE_RFC_SEND_DATA */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ
CsrSize CsrBtJsr82RfcGetAvailableReqSizeof(void *msg)
{
    CsrBtJsr82RfcGetAvailableReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ
CsrUint8 *CsrBtJsr82RfcGetAvailableReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcGetAvailableReq *primitive;

    primitive = (CsrBtJsr82RfcGetAvailableReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ
void *CsrBtJsr82RfcGetAvailableReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcGetAvailableReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcGetAvailableReq *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcGetAvailableReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_CFM
CsrSize CsrBtJsr82RfcGetAvailableCfmSizeof(void *msg)
{
    CsrBtJsr82RfcGetAvailableCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_CFM
CsrUint8 *CsrBtJsr82RfcGetAvailableCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcGetAvailableCfm *primitive;

    primitive = (CsrBtJsr82RfcGetAvailableCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bytesAvailable);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_CFM
void *CsrBtJsr82RfcGetAvailableCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcGetAvailableCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcGetAvailableCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcGetAvailableCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bytesAvailable, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_AVAILABLE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND
CsrSize CsrBtJsr82DlServicesDiscoveredIndSizeof(void *msg)
{
    CsrBtJsr82DlServicesDiscoveredInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attributesLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND
CsrUint8 *CsrBtJsr82DlServicesDiscoveredIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82DlServicesDiscoveredInd *primitive;

    primitive = (CsrBtJsr82DlServicesDiscoveredInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attributesLength);
    CsrMemCpySer(buffer, length, &primitive->attributes, sizeof(primitive->attributes));
    if(primitive->attributes)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attributes), ((CsrSize) (primitive->attributesLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND
void *CsrBtJsr82DlServicesDiscoveredIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82DlServicesDiscoveredInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82DlServicesDiscoveredInd *) CsrPmemZalloc(sizeof(CsrBtJsr82DlServicesDiscoveredInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attributesLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attributes), buffer, &offset, ((CsrSize) sizeof(primitive->attributes)));
    if (primitive->attributesLength)
    {
        primitive->attributes = CsrPmemZalloc(((CsrUint32) (primitive->attributesLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->attributes), buffer, &offset, ((CsrSize) (primitive->attributesLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->attributes = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND
void CsrBtJsr82DlServicesDiscoveredIndSerFree(void *msg)
{
    CsrBtJsr82DlServicesDiscoveredInd *primitive;

    primitive = (CsrBtJsr82DlServicesDiscoveredInd *) msg;

    if(primitive->attributes != NULL)
    {
        CsrPmemFree(primitive->attributes);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_JSR82_DL_SERVICES_DISCOVERED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_REQ
CsrSize CsrBtJsr82L2caGetPsmReqSizeof(void *msg)
{
    CsrBtJsr82L2caGetPsmReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_REQ
CsrUint8 *CsrBtJsr82L2caGetPsmReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caGetPsmReq *primitive;

    primitive = (CsrBtJsr82L2caGetPsmReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_REQ
void *CsrBtJsr82L2caGetPsmReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caGetPsmReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caGetPsmReq *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caGetPsmReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_CFM
CsrSize CsrBtJsr82RdEncryptCfmSizeof(void *msg)
{
    CsrBtJsr82RdEncryptCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_CFM
CsrUint8 *CsrBtJsr82RdEncryptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RdEncryptCfm *primitive;

    primitive = (CsrBtJsr82RdEncryptCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_CFM
void *CsrBtJsr82RdEncryptCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RdEncryptCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RdEncryptCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82RdEncryptCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_ENCRYPT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_CFM
CsrSize CsrBtJsr82RfcDisconnectCfmSizeof(void *msg)
{
    CsrBtJsr82RfcDisconnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_CFM
CsrUint8 *CsrBtJsr82RfcDisconnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcDisconnectCfm *primitive;

    primitive = (CsrBtJsr82RfcDisconnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localTerminated);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_CFM
void *CsrBtJsr82RfcDisconnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcDisconnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcDisconnectCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcDisconnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localTerminated, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ
CsrSize CsrBtJsr82LdGetBluetoothAddressReqSizeof(void *msg)
{
    CsrBtJsr82LdGetBluetoothAddressReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ
CsrUint8 *CsrBtJsr82LdGetBluetoothAddressReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdGetBluetoothAddressReq *primitive;

    primitive = (CsrBtJsr82LdGetBluetoothAddressReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ
void *CsrBtJsr82LdGetBluetoothAddressReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdGetBluetoothAddressReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdGetBluetoothAddressReq *) CsrPmemZalloc(sizeof(CsrBtJsr82LdGetBluetoothAddressReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_BLUETOOTH_ADDRESS_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ
CsrSize CsrBtJsr82LdGetDiscoverableReqSizeof(void *msg)
{
    CsrBtJsr82LdGetDiscoverableReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ
CsrUint8 *CsrBtJsr82LdGetDiscoverableReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdGetDiscoverableReq *primitive;

    primitive = (CsrBtJsr82LdGetDiscoverableReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ
void *CsrBtJsr82LdGetDiscoverableReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdGetDiscoverableReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdGetDiscoverableReq *) CsrPmemZalloc(sizeof(CsrBtJsr82LdGetDiscoverableReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_CFM
CsrSize CsrBtJsr82SetEventMaskCfmSizeof(void *msg)
{
    CsrBtJsr82SetEventMaskCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_CFM
CsrUint8 *CsrBtJsr82SetEventMaskCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82SetEventMaskCfm *primitive;

    primitive = (CsrBtJsr82SetEventMaskCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->eventMask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_CFM
void *CsrBtJsr82SetEventMaskCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82SetEventMaskCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82SetEventMaskCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82SetEventMaskCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->eventMask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_CFM
CsrSize CsrBtJsr82SrRemoveRecordCfmSizeof(void *msg)
{
    CsrBtJsr82SrRemoveRecordCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_CFM
CsrUint8 *CsrBtJsr82SrRemoveRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82SrRemoveRecordCfm *primitive;

    primitive = (CsrBtJsr82SrRemoveRecordCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceRecHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_CFM
void *CsrBtJsr82SrRemoveRecordCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82SrRemoveRecordCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82SrRemoveRecordCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82SrRemoveRecordCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceRecHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REMOVE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_REQ
CsrSize CsrBtJsr82SrRegisterRecordReqSizeof(void *msg)
{
    CsrBtJsr82SrRegisterRecordReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->serviceRecordLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_REQ
CsrUint8 *CsrBtJsr82SrRegisterRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82SrRegisterRecordReq *primitive;

    primitive = (CsrBtJsr82SrRegisterRecordReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->serviceRecordLength);
    CsrMemCpySer(buffer, length, &primitive->serviceRecord, sizeof(primitive->serviceRecord));
    if(primitive->serviceRecord)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->serviceRecord), ((CsrSize) (primitive->serviceRecordLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_REQ
void *CsrBtJsr82SrRegisterRecordReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82SrRegisterRecordReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82SrRegisterRecordReq *) CsrPmemZalloc(sizeof(CsrBtJsr82SrRegisterRecordReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->serviceRecordLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceRecord), buffer, &offset, ((CsrSize) sizeof(primitive->serviceRecord)));
    if (primitive->serviceRecordLength)
    {
        primitive->serviceRecord = CsrPmemZalloc(((CsrUint32) (primitive->serviceRecordLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->serviceRecord), buffer, &offset, ((CsrSize) (primitive->serviceRecordLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->serviceRecord = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_REQ
void CsrBtJsr82SrRegisterRecordReqSerFree(void *msg)
{
    CsrBtJsr82SrRegisterRecordReq *primitive;

    primitive = (CsrBtJsr82SrRegisterRecordReq *) msg;

    if(primitive->serviceRecord != NULL)
    {
        CsrPmemFree(primitive->serviceRecord);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_REGISTER_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_CFM
CsrSize CsrBtJsr82LdGetDiscoverableCfmSizeof(void *msg)
{
    CsrBtJsr82LdGetDiscoverableCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_CFM
CsrUint8 *CsrBtJsr82LdGetDiscoverableCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdGetDiscoverableCfm *primitive;

    primitive = (CsrBtJsr82LdGetDiscoverableCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_CFM
void *CsrBtJsr82LdGetDiscoverableCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdGetDiscoverableCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdGetDiscoverableCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82LdGetDiscoverableCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DISCOVERABLE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_REQ
CsrSize CsrBtJsr82LdIsMasterReqSizeof(void *msg)
{
    CsrBtJsr82LdIsMasterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_REQ
CsrUint8 *CsrBtJsr82LdIsMasterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdIsMasterReq *primitive;

    primitive = (CsrBtJsr82LdIsMasterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_REQ
void *CsrBtJsr82LdIsMasterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdIsMasterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdIsMasterReq *) CsrPmemZalloc(sizeof(CsrBtJsr82LdIsMasterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_CFM
CsrSize CsrBtJsr82L2caGetPsmCfmSizeof(void *msg)
{
    CsrBtJsr82L2caGetPsmCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_CFM
CsrUint8 *CsrBtJsr82L2caGetPsmCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caGetPsmCfm *primitive;

    primitive = (CsrBtJsr82L2caGetPsmCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_CFM
void *CsrBtJsr82L2caGetPsmCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caGetPsmCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caGetPsmCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caGetPsmCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_GET_PSM_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_REQ
CsrSize CsrBtJsr82SrCreateRecordReqSizeof(void *msg)
{
    CsrBtJsr82SrCreateRecordReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_REQ
CsrUint8 *CsrBtJsr82SrCreateRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82SrCreateRecordReq *primitive;

    primitive = (CsrBtJsr82SrCreateRecordReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_REQ
void *CsrBtJsr82SrCreateRecordReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82SrCreateRecordReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82SrCreateRecordReq *) CsrPmemZalloc(sizeof(CsrBtJsr82SrCreateRecordReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_SR_CREATE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_CFM
CsrSize CsrBtJsr82LdGetDeviceClassCfmSizeof(void *msg)
{
    CsrBtJsr82LdGetDeviceClassCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_CFM
CsrUint8 *CsrBtJsr82LdGetDeviceClassCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdGetDeviceClassCfm *primitive;

    primitive = (CsrBtJsr82LdGetDeviceClassCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceClass);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_CFM
void *CsrBtJsr82LdGetDeviceClassCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdGetDeviceClassCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdGetDeviceClassCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82LdGetDeviceClassCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceClass, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_DEVICE_CLASS_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ
CsrSize CsrBtJsr82LdGetFriendlyNameReqSizeof(void *msg)
{
    CsrBtJsr82LdGetFriendlyNameReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ
CsrUint8 *CsrBtJsr82LdGetFriendlyNameReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdGetFriendlyNameReq *primitive;

    primitive = (CsrBtJsr82LdGetFriendlyNameReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ
void *CsrBtJsr82LdGetFriendlyNameReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdGetFriendlyNameReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdGetFriendlyNameReq *) CsrPmemZalloc(sizeof(CsrBtJsr82LdGetFriendlyNameReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ
CsrSize CsrBtJsr82DaRetrieveDevicesReqSizeof(void *msg)
{
    CsrBtJsr82DaRetrieveDevicesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ
CsrUint8 *CsrBtJsr82DaRetrieveDevicesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82DaRetrieveDevicesReq *primitive;

    primitive = (CsrBtJsr82DaRetrieveDevicesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->option);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ
void *CsrBtJsr82DaRetrieveDevicesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82DaRetrieveDevicesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82DaRetrieveDevicesReq *) CsrPmemZalloc(sizeof(CsrBtJsr82DaRetrieveDevicesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->option, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_RETRIEVE_DEVICES_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_CFM
CsrSize CsrBtJsr82L2caCloseCfmSizeof(void *msg)
{
    CsrBtJsr82L2caCloseCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_CFM
CsrUint8 *CsrBtJsr82L2caCloseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caCloseCfm *primitive;

    primitive = (CsrBtJsr82L2caCloseCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localTerminated);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_CFM
void *CsrBtJsr82L2caCloseCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caCloseCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caCloseCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caCloseCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localTerminated, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_CLOSE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_CFM
CsrSize CsrBtJsr82L2caTxDataCfmSizeof(void *msg)
{
    CsrBtJsr82L2caTxDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_CFM
CsrUint8 *CsrBtJsr82L2caTxDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caTxDataCfm *primitive;

    primitive = (CsrBtJsr82L2caTxDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_CFM
void *CsrBtJsr82L2caTxDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caTxDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caTxDataCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caTxDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_TX_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_CFM
CsrSize CsrBtJsr82L2caOpenCfmSizeof(void *msg)
{
    CsrBtJsr82L2caOpenCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_CFM
CsrUint8 *CsrBtJsr82L2caOpenCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caOpenCfm *primitive;

    primitive = (CsrBtJsr82L2caOpenCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->transmitMtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_CFM
void *CsrBtJsr82L2caOpenCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caOpenCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caOpenCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caOpenCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->transmitMtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_OPEN_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_START_INQUIRY_REQ
CsrSize CsrBtJsr82DaStartInquiryReqSizeof(void *msg)
{
    CsrBtJsr82DaStartInquiryReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_START_INQUIRY_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_START_INQUIRY_REQ
CsrUint8 *CsrBtJsr82DaStartInquiryReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82DaStartInquiryReq *primitive;

    primitive = (CsrBtJsr82DaStartInquiryReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->iac);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_START_INQUIRY_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_START_INQUIRY_REQ
void *CsrBtJsr82DaStartInquiryReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82DaStartInquiryReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82DaStartInquiryReq *) CsrPmemZalloc(sizeof(CsrBtJsr82DaStartInquiryReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->iac, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_START_INQUIRY_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_CFM
CsrSize CsrBtJsr82RfcGetServerChannelCfmSizeof(void *msg)
{
    CsrBtJsr82RfcGetServerChannelCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_CFM
CsrUint8 *CsrBtJsr82RfcGetServerChannelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcGetServerChannelCfm *primitive;

    primitive = (CsrBtJsr82RfcGetServerChannelCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_CFM
void *CsrBtJsr82RfcGetServerChannelCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcGetServerChannelCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcGetServerChannelCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcGetServerChannelCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_GET_SERVER_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ
CsrSize CsrBtJsr82LdSetDeviceClassReqSizeof(void *msg)
{
    CsrBtJsr82LdSetDeviceClassReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ
CsrUint8 *CsrBtJsr82LdSetDeviceClassReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdSetDeviceClassReq *primitive;

    primitive = (CsrBtJsr82LdSetDeviceClassReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceClass);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ
void *CsrBtJsr82LdSetDeviceClassReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdSetDeviceClassReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdSetDeviceClassReq *) CsrPmemZalloc(sizeof(CsrBtJsr82LdSetDeviceClassReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceClass, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_REQ
CsrSize CsrBtJsr82L2caDisconnectReqSizeof(void *msg)
{
    CsrBtJsr82L2caDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_REQ
CsrUint8 *CsrBtJsr82L2caDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caDisconnectReq *primitive;

    primitive = (CsrBtJsr82L2caDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_REQ
void *CsrBtJsr82L2caDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM
CsrSize CsrBtJsr82LdGetFriendlyNameCfmSizeof(void *msg)
{
    CsrBtJsr82LdGetFriendlyNameCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->localName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM
CsrUint8 *CsrBtJsr82LdGetFriendlyNameCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdGetFriendlyNameCfm *primitive;

    primitive = (CsrBtJsr82LdGetFriendlyNameCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrMemCpySer(buffer, length, &primitive->localName, sizeof(primitive->localName));
    CsrUtf8StringSer(buffer, length, primitive->localName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM
void *CsrBtJsr82LdGetFriendlyNameCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdGetFriendlyNameCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdGetFriendlyNameCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82LdGetFriendlyNameCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->localName), buffer, &offset, ((CsrSize) sizeof(primitive->localName)));
    CsrUtf8StringDes(&primitive->localName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM
void CsrBtJsr82LdGetFriendlyNameCfmSerFree(void *msg)
{
    CsrBtJsr82LdGetFriendlyNameCfm *primitive;

    primitive = (CsrBtJsr82LdGetFriendlyNameCfm *) msg;

    if(primitive->localName != NULL)
    {
        CsrPmemFree(primitive->localName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_FRIENDLY_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_CFM
CsrSize CsrBtJsr82LdSetDiscoverableCfmSizeof(void *msg)
{
    CsrBtJsr82LdSetDiscoverableCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_CFM
CsrUint8 *CsrBtJsr82LdSetDiscoverableCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdSetDiscoverableCfm *primitive;

    primitive = (CsrBtJsr82LdSetDiscoverableCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_CFM
void *CsrBtJsr82LdSetDiscoverableCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdSetDiscoverableCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdSetDiscoverableCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82LdSetDiscoverableCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DISCOVERABLE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_DL_SERVICE_SEARCH_COMPLETED_IND
CsrSize CsrBtJsr82DlServiceSearchCompletedIndSizeof(void *msg)
{
    CsrBtJsr82DlServiceSearchCompletedInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_DL_SERVICE_SEARCH_COMPLETED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_DL_SERVICE_SEARCH_COMPLETED_IND
CsrUint8 *CsrBtJsr82DlServiceSearchCompletedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82DlServiceSearchCompletedInd *primitive;

    primitive = (CsrBtJsr82DlServiceSearchCompletedInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceDataBaseState);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serviceDBStateValid);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->respCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DL_SERVICE_SEARCH_COMPLETED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_DL_SERVICE_SEARCH_COMPLETED_IND
void *CsrBtJsr82DlServiceSearchCompletedIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82DlServiceSearchCompletedInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82DlServiceSearchCompletedInd *) CsrPmemZalloc(sizeof(CsrBtJsr82DlServiceSearchCompletedInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceDataBaseState, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serviceDBStateValid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->respCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DL_SERVICE_SEARCH_COMPLETED_IND */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_CFM
CsrSize CsrBtJsr82LdIsMasterCfmSizeof(void *msg)
{
    CsrBtJsr82LdIsMasterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_CFM
CsrUint8 *CsrBtJsr82LdIsMasterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdIsMasterCfm *primitive;

    primitive = (CsrBtJsr82LdIsMasterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_CFM
void *CsrBtJsr82LdIsMasterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdIsMasterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdIsMasterCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82LdIsMasterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_IS_MASTER_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_REQ
CsrSize CsrBtJsr82RdIsConnectedReqSizeof(void *msg)
{
    CsrBtJsr82RdIsConnectedReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_REQ
CsrUint8 *CsrBtJsr82RdIsConnectedReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RdIsConnectedReq *primitive;

    primitive = (CsrBtJsr82RdIsConnectedReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_REQ
void *CsrBtJsr82RdIsConnectedReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RdIsConnectedReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RdIsConnectedReq *) CsrPmemZalloc(sizeof(CsrBtJsr82RdIsConnectedReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_REQ
CsrSize CsrBtJsr82RdIsTrustedReqSizeof(void *msg)
{
    CsrBtJsr82RdIsTrustedReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_REQ
CsrUint8 *CsrBtJsr82RdIsTrustedReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RdIsTrustedReq *primitive;

    primitive = (CsrBtJsr82RdIsTrustedReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_REQ
void *CsrBtJsr82RdIsTrustedReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RdIsTrustedReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RdIsTrustedReq *) CsrPmemZalloc(sizeof(CsrBtJsr82RdIsTrustedReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ
CsrSize CsrBtJsr82DaSearchServicesReqSizeof(void *msg)
{
    CsrBtJsr82DaSearchServicesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->uuidSetLength * sizeof(CsrUint8)) + (primitive->attrSetLength * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ
CsrUint8 *CsrBtJsr82DaSearchServicesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82DaSearchServicesReq *primitive;

    primitive = (CsrBtJsr82DaSearchServicesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uuidSetLength);
    CsrMemCpySer(buffer, length, &primitive->uuidSet, sizeof(primitive->uuidSet));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attrSetLength);
    CsrMemCpySer(buffer, length, &primitive->attrSet, sizeof(primitive->attrSet));
    if(primitive->uuidSet)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->uuidSet), ((CsrSize) (primitive->uuidSetLength * sizeof(CsrUint8))));
    }
    if(primitive->attrSet)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attrSet), ((CsrSize) (primitive->attrSetLength * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ
void *CsrBtJsr82DaSearchServicesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82DaSearchServicesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82DaSearchServicesReq *) CsrPmemZalloc(sizeof(CsrBtJsr82DaSearchServicesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uuidSetLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->uuidSet), buffer, &offset, ((CsrSize) sizeof(primitive->uuidSet)));
    CsrUint16Des((CsrUint16 *) &primitive->attrSetLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attrSet), buffer, &offset, ((CsrSize) sizeof(primitive->attrSet)));
    if (primitive->uuidSetLength)
    {
        primitive->uuidSet = CsrPmemZalloc(((CsrUint32) (primitive->uuidSetLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->uuidSet), buffer, &offset, ((CsrSize) (primitive->uuidSetLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->uuidSet = NULL;
    }
    if (primitive->attrSetLength)
    {
        primitive->attrSet = CsrPmemZalloc(((CsrUint32) (primitive->attrSetLength * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->attrSet), buffer, &offset, ((CsrSize) (primitive->attrSetLength * sizeof(CsrUint16))));
    }
    else
    {
        primitive->attrSet = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ
void CsrBtJsr82DaSearchServicesReqSerFree(void *msg)
{
    CsrBtJsr82DaSearchServicesReq *primitive;

    primitive = (CsrBtJsr82DaSearchServicesReq *) msg;

    if(primitive->uuidSet != NULL)
    {
        CsrPmemFree(primitive->uuidSet);
    }
    if(primitive->attrSet != NULL)
    {
        CsrPmemFree(primitive->attrSet);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_JSR82_DA_SEARCH_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_CFM
CsrSize CsrBtJsr82RfcCloseCfmSizeof(void *msg)
{
    CsrBtJsr82RfcCloseCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_CFM
CsrUint8 *CsrBtJsr82RfcCloseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcCloseCfm *primitive;

    primitive = (CsrBtJsr82RfcCloseCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localTerminated);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_CFM
void *CsrBtJsr82RfcCloseCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcCloseCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcCloseCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcCloseCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localTerminated, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_CLOSE_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_CFM
CsrSize CsrBtJsr82RfcSendDataCfmSizeof(void *msg)
{
    CsrBtJsr82RfcSendDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_CFM
CsrUint8 *CsrBtJsr82RfcSendDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RfcSendDataCfm *primitive;

    primitive = (CsrBtJsr82RfcSendDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bytesWritten);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_CFM
void *CsrBtJsr82RfcSendDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RfcSendDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RfcSendDataCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82RfcSendDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bytesWritten, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RFC_SEND_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_CFM
CsrSize CsrBtJsr82RdIsTrustedCfmSizeof(void *msg)
{
    CsrBtJsr82RdIsTrustedCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_CFM
CsrUint8 *CsrBtJsr82RdIsTrustedCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RdIsTrustedCfm *primitive;

    primitive = (CsrBtJsr82RdIsTrustedCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->trusted);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_CFM
void *CsrBtJsr82RdIsTrustedCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RdIsTrustedCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RdIsTrustedCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82RdIsTrustedCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->trusted, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_TRUSTED_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_CFM
CsrSize CsrBtJsr82LdSetDeviceClassCfmSizeof(void *msg)
{
    CsrBtJsr82LdSetDeviceClassCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_CFM
CsrUint8 *CsrBtJsr82LdSetDeviceClassCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdSetDeviceClassCfm *primitive;

    primitive = (CsrBtJsr82LdSetDeviceClassCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_CFM
void *CsrBtJsr82LdSetDeviceClassCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdSetDeviceClassCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdSetDeviceClassCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82LdSetDeviceClassCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_SET_DEVICE_CLASS_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_CFM
CsrSize CsrBtJsr82L2caDisconnectCfmSizeof(void *msg)
{
    CsrBtJsr82L2caDisconnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_CFM
CsrUint8 *CsrBtJsr82L2caDisconnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82L2caDisconnectCfm *primitive;

    primitive = (CsrBtJsr82L2caDisconnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localTerminated);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_CFM
void *CsrBtJsr82L2caDisconnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82L2caDisconnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82L2caDisconnectCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82L2caDisconnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localTerminated, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_L2CA_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_CFM
CsrSize CsrBtJsr82RdIsConnectedCfmSizeof(void *msg)
{
    CsrBtJsr82RdIsConnectedCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_CFM
CsrUint8 *CsrBtJsr82RdIsConnectedCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82RdIsConnectedCfm *primitive;

    primitive = (CsrBtJsr82RdIsConnectedCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->handle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_CFM
void *CsrBtJsr82RdIsConnectedCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82RdIsConnectedCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82RdIsConnectedCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82RdIsConnectedCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->handle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_RD_IS_CONNECTED_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_CFM
CsrSize CsrBtJsr82LdGetPropertyCfmSizeof(void *msg)
{
    CsrBtJsr82LdGetPropertyCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_CFM
CsrUint8 *CsrBtJsr82LdGetPropertyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtJsr82LdGetPropertyCfm *primitive;

    primitive = (CsrBtJsr82LdGetPropertyCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->reqID);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->propertyName);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->propertyValue);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_CFM */

#ifndef EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_CFM
void *CsrBtJsr82LdGetPropertyCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtJsr82LdGetPropertyCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtJsr82LdGetPropertyCfm *) CsrPmemZalloc(sizeof(CsrBtJsr82LdGetPropertyCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->reqID, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->propertyName, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->propertyValue, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_JSR82_LD_GET_PROPERTY_CFM */

#endif /* EXCLUDE_CSR_BT_JSR82_MODULE */
