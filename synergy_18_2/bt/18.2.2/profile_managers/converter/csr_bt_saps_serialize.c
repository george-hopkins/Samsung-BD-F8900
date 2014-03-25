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
#include "csr_bt_saps_prim.h"

#ifndef EXCLUDE_CSR_BT_SAPS_MODULE

void CsrBtSapsPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_RES
CsrSize CsrBtSapsPowerSimOnResSizeof(void *msg)
{
    CsrBtSapsPowerSimOnRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_RES
CsrUint8 *CsrBtSapsPowerSimOnResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsPowerSimOnRes *primitive;

    primitive = (CsrBtSapsPowerSimOnRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_RES
void *CsrBtSapsPowerSimOnResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsPowerSimOnRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsPowerSimOnRes *) CsrPmemZalloc(sizeof(CsrBtSapsPowerSimOnRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_IND
CsrSize CsrBtSapsPowerSimOffIndSizeof(void *msg)
{
    CsrBtSapsPowerSimOffInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_IND
CsrUint8 *CsrBtSapsPowerSimOffIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsPowerSimOffInd *primitive;

    primitive = (CsrBtSapsPowerSimOffInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_IND
void *CsrBtSapsPowerSimOffIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsPowerSimOffInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsPowerSimOffInd *) CsrPmemZalloc(sizeof(CsrBtSapsPowerSimOffInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_SECURITY_IN_CFM
CsrSize CsrBtSapsSecurityInCfmSizeof(void *msg)
{
    CsrBtSapsSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SAPS_SECURITY_IN_CFM
CsrUint8 *CsrBtSapsSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsSecurityInCfm *primitive;

    primitive = (CsrBtSapsSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SAPS_SECURITY_IN_CFM
void *CsrBtSapsSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtSapsSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_IND
CsrSize CsrBtSapsTransferCardReaderStatusIndSizeof(void *msg)
{
    CsrBtSapsTransferCardReaderStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_IND
CsrUint8 *CsrBtSapsTransferCardReaderStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsTransferCardReaderStatusInd *primitive;

    primitive = (CsrBtSapsTransferCardReaderStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_IND
void *CsrBtSapsTransferCardReaderStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsTransferCardReaderStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsTransferCardReaderStatusInd *) CsrPmemZalloc(sizeof(CsrBtSapsTransferCardReaderStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_SEND_STATUS_REQ
CsrSize CsrBtSapsSendStatusReqSizeof(void *msg)
{
    CsrBtSapsSendStatusReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_SEND_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_SEND_STATUS_REQ
CsrUint8 *CsrBtSapsSendStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsSendStatusReq *primitive;

    primitive = (CsrBtSapsSendStatusReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->statusChange);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_SEND_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_SEND_STATUS_REQ
void *CsrBtSapsSendStatusReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsSendStatusReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsSendStatusReq *) CsrPmemZalloc(sizeof(CsrBtSapsSendStatusReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->statusChange, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_SEND_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_ACTIVATE_CFM
CsrSize CsrBtSapsActivateCfmSizeof(void *msg)
{
    CsrBtSapsActivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SAPS_ACTIVATE_CFM
CsrUint8 *CsrBtSapsActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsActivateCfm *primitive;

    primitive = (CsrBtSapsActivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SAPS_ACTIVATE_CFM
void *CsrBtSapsActivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsActivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsActivateCfm *) CsrPmemZalloc(sizeof(CsrBtSapsActivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SAPS_RESET_SIM_RES
CsrSize CsrBtSapsResetSimResSizeof(void *msg)
{
    CsrBtSapsResetSimRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_RESET_SIM_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_RESET_SIM_RES
CsrUint8 *CsrBtSapsResetSimResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsResetSimRes *primitive;

    primitive = (CsrBtSapsResetSimRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_RESET_SIM_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_RESET_SIM_RES
void *CsrBtSapsResetSimResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsResetSimRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsResetSimRes *) CsrPmemZalloc(sizeof(CsrBtSapsResetSimRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_RESET_SIM_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_CONNECT_RES
CsrSize CsrBtSapsConnectResSizeof(void *msg)
{
    CsrBtSapsConnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_CONNECT_RES
CsrUint8 *CsrBtSapsConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsConnectRes *primitive;

    primitive = (CsrBtSapsConnectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxMsgSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->cardStatus);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_CONNECT_RES
void *CsrBtSapsConnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsConnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsConnectRes *) CsrPmemZalloc(sizeof(CsrBtSapsConnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxMsgSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cardStatus, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES
CsrSize CsrBtSapsTransferCardReaderStatusResSizeof(void *msg)
{
    CsrBtSapsTransferCardReaderStatusRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES
CsrUint8 *CsrBtSapsTransferCardReaderStatusResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsTransferCardReaderStatusRes *primitive;

    primitive = (CsrBtSapsTransferCardReaderStatusRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->cardReaderStatus);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES
void *CsrBtSapsTransferCardReaderStatusResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsTransferCardReaderStatusRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsTransferCardReaderStatusRes *) CsrPmemZalloc(sizeof(CsrBtSapsTransferCardReaderStatusRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cardReaderStatus, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_CARD_READER_STATUS_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_IND
CsrSize CsrBtSapsPowerSimOnIndSizeof(void *msg)
{
    CsrBtSapsPowerSimOnInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_IND
CsrUint8 *CsrBtSapsPowerSimOnIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsPowerSimOnInd *primitive;

    primitive = (CsrBtSapsPowerSimOnInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_IND
void *CsrBtSapsPowerSimOnIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsPowerSimOnInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsPowerSimOnInd *) CsrPmemZalloc(sizeof(CsrBtSapsPowerSimOnInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_ON_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_RES
CsrSize CsrBtSapsTransferAtrResSizeof(void *msg)
{
    CsrBtSapsTransferAtrRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->atrResponseLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_RES
CsrUint8 *CsrBtSapsTransferAtrResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsTransferAtrRes *primitive;

    primitive = (CsrBtSapsTransferAtrRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrMemCpySer(buffer, length, &primitive->atrResponse, sizeof(primitive->atrResponse));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->atrResponseLength);
    if(primitive->atrResponse)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->atrResponse), ((CsrSize) (primitive->atrResponseLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_RES
void *CsrBtSapsTransferAtrResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsTransferAtrRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsTransferAtrRes *) CsrPmemZalloc(sizeof(CsrBtSapsTransferAtrRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->atrResponse), buffer, &offset, ((CsrSize) sizeof(primitive->atrResponse)));
    CsrUint16Des((CsrUint16 *) &primitive->atrResponseLength, buffer, &offset);
    if (primitive->atrResponseLength)
    {
        primitive->atrResponse = CsrPmemZalloc(((CsrUint32) (primitive->atrResponseLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->atrResponse), buffer, &offset, ((CsrSize) (primitive->atrResponseLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->atrResponse = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_RES
void CsrBtSapsTransferAtrResSerFree(void *msg)
{
    CsrBtSapsTransferAtrRes *primitive;

    primitive = (CsrBtSapsTransferAtrRes *) msg;

    if(primitive->atrResponse != NULL)
    {
        CsrPmemFree(primitive->atrResponse);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_IND
CsrSize CsrBtSapsTransferAtrIndSizeof(void *msg)
{
    CsrBtSapsTransferAtrInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_IND
CsrUint8 *CsrBtSapsTransferAtrIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsTransferAtrInd *primitive;

    primitive = (CsrBtSapsTransferAtrInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_IND
void *CsrBtSapsTransferAtrIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsTransferAtrInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsTransferAtrInd *) CsrPmemZalloc(sizeof(CsrBtSapsTransferAtrInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_ATR_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_RESET_SIM_IND
CsrSize CsrBtSapsResetSimIndSizeof(void *msg)
{
    CsrBtSapsResetSimInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_RESET_SIM_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_RESET_SIM_IND
CsrUint8 *CsrBtSapsResetSimIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsResetSimInd *primitive;

    primitive = (CsrBtSapsResetSimInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_RESET_SIM_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_RESET_SIM_IND
void *CsrBtSapsResetSimIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsResetSimInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsResetSimInd *) CsrPmemZalloc(sizeof(CsrBtSapsResetSimInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_RESET_SIM_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES
CsrSize CsrBtSapsSetTransferProtocolResSizeof(void *msg)
{
    CsrBtSapsSetTransferProtocolRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES
CsrUint8 *CsrBtSapsSetTransferProtocolResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsSetTransferProtocolRes *primitive;

    primitive = (CsrBtSapsSetTransferProtocolRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES
void *CsrBtSapsSetTransferProtocolResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsSetTransferProtocolRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsSetTransferProtocolRes *) CsrPmemZalloc(sizeof(CsrBtSapsSetTransferProtocolRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_CONNECT_IND
CsrSize CsrBtSapsConnectIndSizeof(void *msg)
{
    CsrBtSapsConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_CONNECT_IND
CsrUint8 *CsrBtSapsConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsConnectInd *primitive;

    primitive = (CsrBtSapsConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxMsgSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_CONNECT_IND
void *CsrBtSapsConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsConnectInd *) CsrPmemZalloc(sizeof(CsrBtSapsConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxMsgSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_ACTIVATE_REQ
CsrSize CsrBtSapsActivateReqSizeof(void *msg)
{
    CsrBtSapsActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_ACTIVATE_REQ
CsrUint8 *CsrBtSapsActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsActivateReq *primitive;

    primitive = (CsrBtSapsActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_ACTIVATE_REQ
void *CsrBtSapsActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsActivateReq *) CsrPmemZalloc(sizeof(CsrBtSapsActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_SECURITY_IN_REQ
CsrSize CsrBtSapsSecurityInReqSizeof(void *msg)
{
    CsrBtSapsSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_SECURITY_IN_REQ
CsrUint8 *CsrBtSapsSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsSecurityInReq *primitive;

    primitive = (CsrBtSapsSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_SECURITY_IN_REQ
void *CsrBtSapsSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtSapsSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_DISCONNECT_IND
CsrSize CsrBtSapsDisconnectIndSizeof(void *msg)
{
    CsrBtSapsDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_DISCONNECT_IND
CsrUint8 *CsrBtSapsDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsDisconnectInd *primitive;

    primitive = (CsrBtSapsDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_DISCONNECT_IND
void *CsrBtSapsDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtSapsDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_IND
CsrSize CsrBtSapsTransferApduIndSizeof(void *msg)
{
    CsrBtSapsTransferApduInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->apduCommandLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_IND
CsrUint8 *CsrBtSapsTransferApduIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsTransferApduInd *primitive;

    primitive = (CsrBtSapsTransferApduInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->apduCommand, sizeof(primitive->apduCommand));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->apduCommandLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->isApdu7816Type);
    if(primitive->apduCommand)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->apduCommand), ((CsrSize) (primitive->apduCommandLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_IND
void *CsrBtSapsTransferApduIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsTransferApduInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsTransferApduInd *) CsrPmemZalloc(sizeof(CsrBtSapsTransferApduInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->apduCommand), buffer, &offset, ((CsrSize) sizeof(primitive->apduCommand)));
    CsrUint16Des((CsrUint16 *) &primitive->apduCommandLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->isApdu7816Type, buffer, &offset);
    if (primitive->apduCommandLength)
    {
        primitive->apduCommand = CsrPmemZalloc(((CsrUint32) (primitive->apduCommandLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->apduCommand), buffer, &offset, ((CsrSize) (primitive->apduCommandLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->apduCommand = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_IND
void CsrBtSapsTransferApduIndSerFree(void *msg)
{
    CsrBtSapsTransferApduInd *primitive;

    primitive = (CsrBtSapsTransferApduInd *) msg;

    if(primitive->apduCommand != NULL)
    {
        CsrPmemFree(primitive->apduCommand);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_DEACTIVATE_CFM
CsrSize CsrBtSapsDeactivateCfmSizeof(void *msg)
{
    CsrBtSapsDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SAPS_DEACTIVATE_CFM
CsrUint8 *CsrBtSapsDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsDeactivateCfm *primitive;

    primitive = (CsrBtSapsDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SAPS_DEACTIVATE_CFM
void *CsrBtSapsDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtSapsDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_SAPS_DISCONNECT_REQ
CsrSize CsrBtSapsDisconnectReqSizeof(void *msg)
{
    CsrBtSapsDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_DISCONNECT_REQ
CsrUint8 *CsrBtSapsDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsDisconnectReq *primitive;

    primitive = (CsrBtSapsDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->disconnectType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_DISCONNECT_REQ
void *CsrBtSapsDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtSapsDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->disconnectType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_RES
CsrSize CsrBtSapsPowerSimOffResSizeof(void *msg)
{
    CsrBtSapsPowerSimOffRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_RES
CsrUint8 *CsrBtSapsPowerSimOffResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsPowerSimOffRes *primitive;

    primitive = (CsrBtSapsPowerSimOffRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_RES
void *CsrBtSapsPowerSimOffResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsPowerSimOffRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsPowerSimOffRes *) CsrPmemZalloc(sizeof(CsrBtSapsPowerSimOffRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_POWER_SIM_OFF_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_DEACTIVATE_REQ
CsrSize CsrBtSapsDeactivateReqSizeof(void *msg)
{
    CsrBtSapsDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_DEACTIVATE_REQ
CsrUint8 *CsrBtSapsDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsDeactivateReq *primitive;

    primitive = (CsrBtSapsDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_DEACTIVATE_REQ
void *CsrBtSapsDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtSapsDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_IND
CsrSize CsrBtSapsSetTransferProtocolIndSizeof(void *msg)
{
    CsrBtSapsSetTransferProtocolInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_IND
CsrUint8 *CsrBtSapsSetTransferProtocolIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsSetTransferProtocolInd *primitive;

    primitive = (CsrBtSapsSetTransferProtocolInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->protocol);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_IND
void *CsrBtSapsSetTransferProtocolIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsSetTransferProtocolInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsSetTransferProtocolInd *) CsrPmemZalloc(sizeof(CsrBtSapsSetTransferProtocolInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->protocol, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_SET_TRANSFER_PROTOCOL_IND */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_RES
CsrSize CsrBtSapsTransferApduResSizeof(void *msg)
{
    CsrBtSapsTransferApduRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->apduResponseLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_RES
CsrUint8 *CsrBtSapsTransferApduResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsTransferApduRes *primitive;

    primitive = (CsrBtSapsTransferApduRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrMemCpySer(buffer, length, &primitive->apduResponse, sizeof(primitive->apduResponse));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->apduResponseLength);
    if(primitive->apduResponse)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->apduResponse), ((CsrSize) (primitive->apduResponseLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_RES
void *CsrBtSapsTransferApduResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsTransferApduRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsTransferApduRes *) CsrPmemZalloc(sizeof(CsrBtSapsTransferApduRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->apduResponse), buffer, &offset, ((CsrSize) sizeof(primitive->apduResponse)));
    CsrUint16Des((CsrUint16 *) &primitive->apduResponseLength, buffer, &offset);
    if (primitive->apduResponseLength)
    {
        primitive->apduResponse = CsrPmemZalloc(((CsrUint32) (primitive->apduResponseLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->apduResponse), buffer, &offset, ((CsrSize) (primitive->apduResponseLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->apduResponse = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_RES
void CsrBtSapsTransferApduResSerFree(void *msg)
{
    CsrBtSapsTransferApduRes *primitive;

    primitive = (CsrBtSapsTransferApduRes *) msg;

    if(primitive->apduResponse != NULL)
    {
        CsrPmemFree(primitive->apduResponse);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SAPS_TRANSFER_APDU_RES */

#ifndef EXCLUDE_CSR_BT_SAPS_HOUSE_CLEANING
CsrSize CsrBtSapsHouseCleaningSizeof(void *msg)
{
    CsrBtSapsHouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SAPS_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SAPS_HOUSE_CLEANING
CsrUint8 *CsrBtSapsHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSapsHouseCleaning *primitive;

    primitive = (CsrBtSapsHouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SAPS_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SAPS_HOUSE_CLEANING
void *CsrBtSapsHouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSapsHouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSapsHouseCleaning *) CsrPmemZalloc(sizeof(CsrBtSapsHouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SAPS_HOUSE_CLEANING */

#endif /* EXCLUDE_CSR_BT_SAPS_MODULE */
