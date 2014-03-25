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
#include "csr_bt_sapc_prim.h"

#ifndef EXCLUDE_CSR_BT_SAPC_MODULE

void CsrBtSapcPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_REQ
CsrSize CsrBtSapcPowerSimOffReqSizeof(void *msg)
{
    CsrBtSapcPowerSimOffReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_REQ
CsrUint8 *CsrBtSapcPowerSimOffReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcPowerSimOffReq *primitive;

    primitive = (CsrBtSapcPowerSimOffReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_REQ
void *CsrBtSapcPowerSimOffReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcPowerSimOffReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcPowerSimOffReq *) CsrPmemZalloc(sizeof(CsrBtSapcPowerSimOffReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_HOUSE_CLEANING
CsrSize CsrBtSapcHouseCleaningSizeof(void *msg)
{
    CsrBtSapcHouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SAPC_HOUSE_CLEANING
CsrUint8 *CsrBtSapcHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcHouseCleaning *primitive;

    primitive = (CsrBtSapcHouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SAPC_HOUSE_CLEANING
void *CsrBtSapcHouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcHouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcHouseCleaning *) CsrPmemZalloc(sizeof(CsrBtSapcHouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_CFM
CsrSize CsrBtSapcTransferCardReaderStatusCfmSizeof(void *msg)
{
    CsrBtSapcTransferCardReaderStatusCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_CFM
CsrUint8 *CsrBtSapcTransferCardReaderStatusCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcTransferCardReaderStatusCfm *primitive;

    primitive = (CsrBtSapcTransferCardReaderStatusCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->cardReaderStatus);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_CFM
void *CsrBtSapcTransferCardReaderStatusCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcTransferCardReaderStatusCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcTransferCardReaderStatusCfm *) CsrPmemZalloc(sizeof(CsrBtSapcTransferCardReaderStatusCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cardReaderStatus, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_REQ
CsrSize CsrBtSapcTransferApduReqSizeof(void *msg)
{
    CsrBtSapcTransferApduReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->commandApduLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_REQ
CsrUint8 *CsrBtSapcTransferApduReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcTransferApduReq *primitive;

    primitive = (CsrBtSapcTransferApduReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->commandApdu, sizeof(primitive->commandApdu));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->commandApduLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->apdu7816Type);
    if(primitive->commandApdu)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->commandApdu), ((CsrSize) (primitive->commandApduLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_REQ
void *CsrBtSapcTransferApduReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcTransferApduReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcTransferApduReq *) CsrPmemZalloc(sizeof(CsrBtSapcTransferApduReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->commandApdu), buffer, &offset, ((CsrSize) sizeof(primitive->commandApdu)));
    CsrUint16Des((CsrUint16 *) &primitive->commandApduLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->apdu7816Type, buffer, &offset);
    if (primitive->commandApduLength)
    {
        primitive->commandApdu = CsrPmemZalloc(((CsrUint32) (primitive->commandApduLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->commandApdu), buffer, &offset, ((CsrSize) (primitive->commandApduLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->commandApdu = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_REQ
void CsrBtSapcTransferApduReqSerFree(void *msg)
{
    CsrBtSapcTransferApduReq *primitive;

    primitive = (CsrBtSapcTransferApduReq *) msg;

    if(primitive->commandApdu != NULL)
    {
        CsrPmemFree(primitive->commandApdu);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_DISCONNECT_IND
CsrSize CsrBtSapcDisconnectIndSizeof(void *msg)
{
    CsrBtSapcDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SAPC_DISCONNECT_IND
CsrUint8 *CsrBtSapcDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcDisconnectInd *primitive;

    primitive = (CsrBtSapcDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SAPC_DISCONNECT_IND
void *CsrBtSapcDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtSapcDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SAPC_CONNECT_REQ
CsrSize CsrBtSapcConnectReqSizeof(void *msg)
{
    CsrBtSapcConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_CONNECT_REQ
CsrUint8 *CsrBtSapcConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcConnectReq *primitive;

    primitive = (CsrBtSapcConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bdAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bdAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bdAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxMsgSize);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_CONNECT_REQ
void *CsrBtSapcConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcConnectReq *) CsrPmemZalloc(sizeof(CsrBtSapcConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bdAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bdAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bdAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxMsgSize, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_CFM
CsrSize CsrBtSapcTransferApduCfmSizeof(void *msg)
{
    CsrBtSapcTransferApduCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->responseApduLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_CFM
CsrUint8 *CsrBtSapcTransferApduCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcTransferApduCfm *primitive;

    primitive = (CsrBtSapcTransferApduCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrMemCpySer(buffer, length, &primitive->responseApdu, sizeof(primitive->responseApdu));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->responseApduLength);
    if(primitive->responseApdu)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->responseApdu), ((CsrSize) (primitive->responseApduLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_CFM
void *CsrBtSapcTransferApduCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcTransferApduCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcTransferApduCfm *) CsrPmemZalloc(sizeof(CsrBtSapcTransferApduCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->responseApdu), buffer, &offset, ((CsrSize) sizeof(primitive->responseApdu)));
    CsrUint16Des((CsrUint16 *) &primitive->responseApduLength, buffer, &offset);
    if (primitive->responseApduLength)
    {
        primitive->responseApdu = CsrPmemZalloc(((CsrUint32) (primitive->responseApduLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->responseApdu), buffer, &offset, ((CsrSize) (primitive->responseApduLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->responseApdu = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_CFM
void CsrBtSapcTransferApduCfmSerFree(void *msg)
{
    CsrBtSapcTransferApduCfm *primitive;

    primitive = (CsrBtSapcTransferApduCfm *) msg;

    if(primitive->responseApdu != NULL)
    {
        CsrPmemFree(primitive->responseApdu);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_APDU_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_CFM
CsrSize CsrBtSapcSetTransferProtocolCfmSizeof(void *msg)
{
    CsrBtSapcSetTransferProtocolCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_CFM
CsrUint8 *CsrBtSapcSetTransferProtocolCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcSetTransferProtocolCfm *primitive;

    primitive = (CsrBtSapcSetTransferProtocolCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_CFM
void *CsrBtSapcSetTransferProtocolCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcSetTransferProtocolCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcSetTransferProtocolCfm *) CsrPmemZalloc(sizeof(CsrBtSapcSetTransferProtocolCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_STATUS_IND
CsrSize CsrBtSapcStatusIndSizeof(void *msg)
{
    CsrBtSapcStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_SAPC_STATUS_IND
CsrUint8 *CsrBtSapcStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcStatusInd *primitive;

    primitive = (CsrBtSapcStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->statusChange);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_SAPC_STATUS_IND
void *CsrBtSapcStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcStatusInd *) CsrPmemZalloc(sizeof(CsrBtSapcStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->statusChange, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_REQ
CsrSize CsrBtSapcPowerSimOnReqSizeof(void *msg)
{
    CsrBtSapcPowerSimOnReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_REQ
CsrUint8 *CsrBtSapcPowerSimOnReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcPowerSimOnReq *primitive;

    primitive = (CsrBtSapcPowerSimOnReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_REQ
void *CsrBtSapcPowerSimOnReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcPowerSimOnReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcPowerSimOnReq *) CsrPmemZalloc(sizeof(CsrBtSapcPowerSimOnReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_CFM
CsrSize CsrBtSapcTransferAtrCfmSizeof(void *msg)
{
    CsrBtSapcTransferAtrCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->atrLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_CFM
CsrUint8 *CsrBtSapcTransferAtrCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcTransferAtrCfm *primitive;

    primitive = (CsrBtSapcTransferAtrCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrMemCpySer(buffer, length, &primitive->atr, sizeof(primitive->atr));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->atrLength);
    if(primitive->atr)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->atr), ((CsrSize) (primitive->atrLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_CFM
void *CsrBtSapcTransferAtrCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcTransferAtrCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcTransferAtrCfm *) CsrPmemZalloc(sizeof(CsrBtSapcTransferAtrCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->atr), buffer, &offset, ((CsrSize) sizeof(primitive->atr)));
    CsrUint16Des((CsrUint16 *) &primitive->atrLength, buffer, &offset);
    if (primitive->atrLength)
    {
        primitive->atr = CsrPmemZalloc(((CsrUint32) (primitive->atrLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->atr), buffer, &offset, ((CsrSize) (primitive->atrLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->atr = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_CFM
void CsrBtSapcTransferAtrCfmSerFree(void *msg)
{
    CsrBtSapcTransferAtrCfm *primitive;

    primitive = (CsrBtSapcTransferAtrCfm *) msg;

    if(primitive->atr != NULL)
    {
        CsrPmemFree(primitive->atr);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_RESET_SIM_REQ
CsrSize CsrBtSapcResetSimReqSizeof(void *msg)
{
    CsrBtSapcResetSimReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_RESET_SIM_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_RESET_SIM_REQ
CsrUint8 *CsrBtSapcResetSimReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcResetSimReq *primitive;

    primitive = (CsrBtSapcResetSimReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_RESET_SIM_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_RESET_SIM_REQ
void *CsrBtSapcResetSimReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcResetSimReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcResetSimReq *) CsrPmemZalloc(sizeof(CsrBtSapcResetSimReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_RESET_SIM_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_REQ
CsrSize CsrBtSapcSecurityOutReqSizeof(void *msg)
{
    CsrBtSapcSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_REQ
CsrUint8 *CsrBtSapcSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcSecurityOutReq *primitive;

    primitive = (CsrBtSapcSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_REQ
void *CsrBtSapcSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtSapcSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ
CsrSize CsrBtSapcSetTransferProtocolReqSizeof(void *msg)
{
    CsrBtSapcSetTransferProtocolReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ
CsrUint8 *CsrBtSapcSetTransferProtocolReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcSetTransferProtocolReq *primitive;

    primitive = (CsrBtSapcSetTransferProtocolReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->transportProtocol);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ
void *CsrBtSapcSetTransferProtocolReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcSetTransferProtocolReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcSetTransferProtocolReq *) CsrPmemZalloc(sizeof(CsrBtSapcSetTransferProtocolReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->transportProtocol, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_SET_TRANSFER_PROTOCOL_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ
CsrSize CsrBtSapcTransferCardReaderStatusReqSizeof(void *msg)
{
    CsrBtSapcTransferCardReaderStatusReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ
CsrUint8 *CsrBtSapcTransferCardReaderStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcTransferCardReaderStatusReq *primitive;

    primitive = (CsrBtSapcTransferCardReaderStatusReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ
void *CsrBtSapcTransferCardReaderStatusReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcTransferCardReaderStatusReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcTransferCardReaderStatusReq *) CsrPmemZalloc(sizeof(CsrBtSapcTransferCardReaderStatusReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_CARD_READER_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_CONNECT_CFM
CsrSize CsrBtSapcConnectCfmSizeof(void *msg)
{
    CsrBtSapcConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_CONNECT_CFM
CsrUint8 *CsrBtSapcConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcConnectCfm *primitive;

    primitive = (CsrBtSapcConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxMsgSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_CONNECT_CFM
void *CsrBtSapcConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcConnectCfm *) CsrPmemZalloc(sizeof(CsrBtSapcConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxMsgSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_REQ
CsrSize CsrBtSapcTransferAtrReqSizeof(void *msg)
{
    CsrBtSapcTransferAtrReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_REQ
CsrUint8 *CsrBtSapcTransferAtrReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcTransferAtrReq *primitive;

    primitive = (CsrBtSapcTransferAtrReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_REQ
void *CsrBtSapcTransferAtrReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcTransferAtrReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcTransferAtrReq *) CsrPmemZalloc(sizeof(CsrBtSapcTransferAtrReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_TRANSFER_ATR_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_CFM
CsrSize CsrBtSapcPowerSimOnCfmSizeof(void *msg)
{
    CsrBtSapcPowerSimOnCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_CFM
CsrUint8 *CsrBtSapcPowerSimOnCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcPowerSimOnCfm *primitive;

    primitive = (CsrBtSapcPowerSimOnCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_CFM
void *CsrBtSapcPowerSimOnCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcPowerSimOnCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcPowerSimOnCfm *) CsrPmemZalloc(sizeof(CsrBtSapcPowerSimOnCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_ON_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_CFM
CsrSize CsrBtSapcPowerSimOffCfmSizeof(void *msg)
{
    CsrBtSapcPowerSimOffCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_CFM
CsrUint8 *CsrBtSapcPowerSimOffCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcPowerSimOffCfm *primitive;

    primitive = (CsrBtSapcPowerSimOffCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_CFM
void *CsrBtSapcPowerSimOffCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcPowerSimOffCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcPowerSimOffCfm *) CsrPmemZalloc(sizeof(CsrBtSapcPowerSimOffCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_POWER_SIM_OFF_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_RESET_SIM_CFM
CsrSize CsrBtSapcResetSimCfmSizeof(void *msg)
{
    CsrBtSapcResetSimCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_RESET_SIM_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_RESET_SIM_CFM
CsrUint8 *CsrBtSapcResetSimCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcResetSimCfm *primitive;

    primitive = (CsrBtSapcResetSimCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_RESET_SIM_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_RESET_SIM_CFM
void *CsrBtSapcResetSimCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcResetSimCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcResetSimCfm *) CsrPmemZalloc(sizeof(CsrBtSapcResetSimCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_RESET_SIM_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_CFM
CsrSize CsrBtSapcSecurityOutCfmSizeof(void *msg)
{
    CsrBtSapcSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_CFM
CsrUint8 *CsrBtSapcSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcSecurityOutCfm *primitive;

    primitive = (CsrBtSapcSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_CFM
void *CsrBtSapcSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtSapcSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_SAPC_DISCONNECT_REQ
CsrSize CsrBtSapcDisconnectReqSizeof(void *msg)
{
    CsrBtSapcDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_DISCONNECT_REQ
CsrUint8 *CsrBtSapcDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapcDisconnectReq *primitive;

    primitive = (CsrBtSapcDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPC_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SAPC_DISCONNECT_REQ
void *CsrBtSapcDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapcDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapcDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtSapcDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPC_DISCONNECT_REQ */

#endif /* EXCLUDE_CSR_BT_SAPC_MODULE */
