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
#include "csr_bt_hcrp_prim.h"

#ifndef EXCLUDE_CSR_BT_HCRP_MODULE

void CsrBtHcrpPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_REQ
CsrSize CsrBtHcrpRegisterDataPathReqSizeof(void *msg)
{
    CsrBtHcrpRegisterDataPathReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_REQ
CsrUint8 *CsrBtHcrpRegisterDataPathReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHcrpRegisterDataPathReq *primitive;

    primitive = (CsrBtHcrpRegisterDataPathReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_REQ
void *CsrBtHcrpRegisterDataPathReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHcrpRegisterDataPathReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHcrpRegisterDataPathReq *) CsrPmemZalloc(sizeof(CsrBtHcrpRegisterDataPathReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_DISCONNECT_IND
CsrSize CsrBtHcrpL2caDisconnectIndSizeof(void *msg)
{
    CsrBtHcrpL2caDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_DISCONNECT_IND
CsrUint8 *CsrBtHcrpL2caDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHcrpL2caDisconnectInd *primitive;

    primitive = (CsrBtHcrpL2caDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_DISCONNECT_IND
void *CsrBtHcrpL2caDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHcrpL2caDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHcrpL2caDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtHcrpL2caDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_CONNECT_IND
CsrSize CsrBtHcrpL2caConnectIndSizeof(void *msg)
{
    CsrBtHcrpL2caConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_CONNECT_IND
CsrUint8 *CsrBtHcrpL2caConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHcrpL2caConnectInd *primitive;

    primitive = (CsrBtHcrpL2caConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_CONNECT_IND
void *CsrBtHcrpL2caConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHcrpL2caConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHcrpL2caConnectInd *) CsrPmemZalloc(sizeof(CsrBtHcrpL2caConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_STATUS_IND
CsrSize CsrBtHcrpStatusIndSizeof(void *msg)
{
    CsrBtHcrpStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HCRP_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_STATUS_IND
CsrUint8 *CsrBtHcrpStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHcrpStatusInd *primitive;

    primitive = (CsrBtHcrpStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connect);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HCRP_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_STATUS_IND
void *CsrBtHcrpStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHcrpStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHcrpStatusInd *) CsrPmemZalloc(sizeof(CsrBtHcrpStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connect, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HCRP_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_SECURITY_IN_CFM
CsrSize CsrBtHcrpSecurityInCfmSizeof(void *msg)
{
    CsrBtHcrpSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HCRP_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HCRP_SECURITY_IN_CFM
CsrUint8 *CsrBtHcrpSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHcrpSecurityInCfm *primitive;

    primitive = (CsrBtHcrpSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HCRP_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HCRP_SECURITY_IN_CFM
void *CsrBtHcrpSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHcrpSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHcrpSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtHcrpSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HCRP_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HCRP_DEACTIVATE_REQ
CsrSize CsrBtHcrpDeactivateReqSizeof(void *msg)
{
    CsrBtHcrpDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HCRP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_DEACTIVATE_REQ
CsrUint8 *CsrBtHcrpDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHcrpDeactivateReq *primitive;

    primitive = (CsrBtHcrpDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HCRP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_DEACTIVATE_REQ
void *CsrBtHcrpDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHcrpDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHcrpDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtHcrpDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HCRP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_RES
CsrSize CsrBtHcrpGetIeee1284ResSizeof(void *msg)
{
    CsrBtHcrpGetIeee1284Res *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->ieee1284));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_RES */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_RES
CsrUint8 *CsrBtHcrpGetIeee1284ResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHcrpGetIeee1284Res *primitive;

    primitive = (CsrBtHcrpGetIeee1284Res *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pduId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->transId);
    CsrMemCpySer(buffer, length, &primitive->ieee1284, sizeof(primitive->ieee1284));
    CsrUtf8StringSer(buffer, length, primitive->ieee1284);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_RES */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_RES
void *CsrBtHcrpGetIeee1284ResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHcrpGetIeee1284Res *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHcrpGetIeee1284Res *) CsrPmemZalloc(sizeof(CsrBtHcrpGetIeee1284Res));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pduId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->transId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ieee1284), buffer, &offset, ((CsrSize) sizeof(primitive->ieee1284)));
    CsrUtf8StringDes(&primitive->ieee1284, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_RES */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_RES
void CsrBtHcrpGetIeee1284ResSerFree(void *msg)
{
    CsrBtHcrpGetIeee1284Res *primitive;

    primitive = (CsrBtHcrpGetIeee1284Res *) msg;

    if(primitive->ieee1284 != NULL)
    {
        CsrPmemFree(primitive->ieee1284);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_RES */

#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_DATA_IND
CsrSize CsrBtHcrpL2caDataIndSizeof(void *msg)
{
    CsrBtHcrpL2caDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_DATA_IND
CsrUint8 *CsrBtHcrpL2caDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHcrpL2caDataInd *primitive;

    primitive = (CsrBtHcrpL2caDataInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_DATA_IND
void *CsrBtHcrpL2caDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHcrpL2caDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHcrpL2caDataInd *) CsrPmemZalloc(sizeof(CsrBtHcrpL2caDataInd));

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
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_L2CA_DATA_IND
void CsrBtHcrpL2caDataIndSerFree(void *msg)
{
    CsrBtHcrpL2caDataInd *primitive;

    primitive = (CsrBtHcrpL2caDataInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HCRP_L2CA_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_IND
CsrSize CsrBtHcrpGetIeee1284IndSizeof(void *msg)
{
    CsrBtHcrpGetIeee1284Ind *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_IND
CsrUint8 *CsrBtHcrpGetIeee1284IndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHcrpGetIeee1284Ind *primitive;

    primitive = (CsrBtHcrpGetIeee1284Ind *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pduId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->transId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_IND
void *CsrBtHcrpGetIeee1284IndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHcrpGetIeee1284Ind *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHcrpGetIeee1284Ind *) CsrPmemZalloc(sizeof(CsrBtHcrpGetIeee1284Ind));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pduId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->transId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HCRP_GET_IEEE1284_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_RES
CsrSize CsrBtHcrpGetLptStatusResSizeof(void *msg)
{
    CsrBtHcrpGetLptStatusRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_RES */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_RES
CsrUint8 *CsrBtHcrpGetLptStatusResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHcrpGetLptStatusRes *primitive;

    primitive = (CsrBtHcrpGetLptStatusRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pduId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->transId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->lptStatus);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_RES */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_RES
void *CsrBtHcrpGetLptStatusResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHcrpGetLptStatusRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHcrpGetLptStatusRes *) CsrPmemZalloc(sizeof(CsrBtHcrpGetLptStatusRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pduId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->transId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->lptStatus, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_RES */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_IND
CsrSize CsrBtHcrpGetLptStatusIndSizeof(void *msg)
{
    CsrBtHcrpGetLptStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_IND
CsrUint8 *CsrBtHcrpGetLptStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHcrpGetLptStatusInd *primitive;

    primitive = (CsrBtHcrpGetLptStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pduId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->transId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_IND
void *CsrBtHcrpGetLptStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHcrpGetLptStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHcrpGetLptStatusInd *) CsrPmemZalloc(sizeof(CsrBtHcrpGetLptStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pduId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->transId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HCRP_GET_LPT_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HCRP_SECURITY_IN_REQ
CsrSize CsrBtHcrpSecurityInReqSizeof(void *msg)
{
    CsrBtHcrpSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HCRP_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_SECURITY_IN_REQ
CsrUint8 *CsrBtHcrpSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHcrpSecurityInReq *primitive;

    primitive = (CsrBtHcrpSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HCRP_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_SECURITY_IN_REQ
void *CsrBtHcrpSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHcrpSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHcrpSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtHcrpSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HCRP_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_HOUSE_CLEANING
CsrSize CsrBtHcrpHouseCleaningSizeof(void *msg)
{
    CsrBtHcrpHouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HCRP_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_HCRP_HOUSE_CLEANING
CsrUint8 *CsrBtHcrpHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHcrpHouseCleaning *primitive;

    primitive = (CsrBtHcrpHouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HCRP_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_HCRP_HOUSE_CLEANING
void *CsrBtHcrpHouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHcrpHouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHcrpHouseCleaning *) CsrPmemZalloc(sizeof(CsrBtHcrpHouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HCRP_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_HCRP_ACTIVATE_REQ
CsrSize CsrBtHcrpActivateReqSizeof(void *msg)
{
    CsrBtHcrpActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->ieee1284Id));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HCRP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_ACTIVATE_REQ
CsrUint8 *CsrBtHcrpActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHcrpActivateReq *primitive;

    primitive = (CsrBtHcrpActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrMemCpySer(buffer, length, &primitive->ieee1284Id, sizeof(primitive->ieee1284Id));
    CsrUtf8StringSer(buffer, length, primitive->ieee1284Id);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HCRP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_ACTIVATE_REQ
void *CsrBtHcrpActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHcrpActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHcrpActivateReq *) CsrPmemZalloc(sizeof(CsrBtHcrpActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ieee1284Id), buffer, &offset, ((CsrSize) sizeof(primitive->ieee1284Id)));
    CsrUtf8StringDes(&primitive->ieee1284Id, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HCRP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_ACTIVATE_REQ
void CsrBtHcrpActivateReqSerFree(void *msg)
{
    CsrBtHcrpActivateReq *primitive;

    primitive = (CsrBtHcrpActivateReq *) msg;

    if(primitive->ieee1284Id != NULL)
    {
        CsrPmemFree(primitive->ieee1284Id);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HCRP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_CFM
CsrSize CsrBtHcrpRegisterDataPathCfmSizeof(void *msg)
{
    CsrBtHcrpRegisterDataPathCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_CFM */

#ifndef EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_CFM
CsrUint8 *CsrBtHcrpRegisterDataPathCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHcrpRegisterDataPathCfm *primitive;

    primitive = (CsrBtHcrpRegisterDataPathCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_CFM */

#ifndef EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_CFM
void *CsrBtHcrpRegisterDataPathCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHcrpRegisterDataPathCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHcrpRegisterDataPathCfm *) CsrPmemZalloc(sizeof(CsrBtHcrpRegisterDataPathCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HCRP_REGISTER_DATA_PATH_CFM */

#endif /* EXCLUDE_CSR_BT_HCRP_MODULE */
