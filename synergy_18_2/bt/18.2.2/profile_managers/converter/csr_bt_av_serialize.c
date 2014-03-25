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
#include "csr_bt_av_prim.h"

#ifndef EXCLUDE_CSR_BT_AV_MODULE

void CsrBtAvPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_AV_DEACTIVATE_REQ
CsrSize CsrBtAvDeactivateReqSizeof(void *msg)
{
    CsrBtAvDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_DEACTIVATE_REQ
CsrUint8 *CsrBtAvDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvDeactivateReq *primitive;

    primitive = (CsrBtAvDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localRole);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_DEACTIVATE_REQ
void *CsrBtAvDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtAvDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localRole, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_IND
CsrSize CsrBtAvSecurityControlIndSizeof(void *msg)
{
    CsrBtAvSecurityControlInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->contProtMethodLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_IND
CsrUint8 *CsrBtAvSecurityControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSecurityControlInd *primitive;

    primitive = (CsrBtAvSecurityControlInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->contProtMethodLen);
    CsrMemCpySer(buffer, length, &primitive->contProtMethodData, sizeof(primitive->contProtMethodData));
    if(primitive->contProtMethodData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->contProtMethodData), ((CsrSize) (primitive->contProtMethodLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_IND
void *CsrBtAvSecurityControlIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSecurityControlInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSecurityControlInd *) CsrPmemZalloc(sizeof(CsrBtAvSecurityControlInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->contProtMethodLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->contProtMethodData), buffer, &offset, ((CsrSize) sizeof(primitive->contProtMethodData)));
    if (primitive->contProtMethodLen)
    {
        primitive->contProtMethodData = CsrPmemZalloc(((CsrUint32) (primitive->contProtMethodLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->contProtMethodData), buffer, &offset, ((CsrSize) (primitive->contProtMethodLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->contProtMethodData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_IND
void CsrBtAvSecurityControlIndSerFree(void *msg)
{
    CsrBtAvSecurityControlInd *primitive;

    primitive = (CsrBtAvSecurityControlInd *) msg;

    if(primitive->contProtMethodData != NULL)
    {
        CsrPmemFree(primitive->contProtMethodData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_IN_CFM
CsrSize CsrBtAvSecurityInCfmSizeof(void *msg)
{
    CsrBtAvSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_IN_CFM
CsrUint8 *CsrBtAvSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSecurityInCfm *primitive;

    primitive = (CsrBtAvSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_IN_CFM
void *CsrBtAvSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtAvSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_RES
CsrSize CsrBtAvSecurityControlResSizeof(void *msg)
{
    CsrBtAvSecurityControlRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->contProtMethodLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_RES */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_RES
CsrUint8 *CsrBtAvSecurityControlResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSecurityControlRes *primitive;

    primitive = (CsrBtAvSecurityControlRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->contProtMethodLen);
    CsrMemCpySer(buffer, length, &primitive->contProtMethodData, sizeof(primitive->contProtMethodData));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->avResponse);
    if(primitive->contProtMethodData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->contProtMethodData), ((CsrSize) (primitive->contProtMethodLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_RES */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_RES
void *CsrBtAvSecurityControlResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSecurityControlRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSecurityControlRes *) CsrPmemZalloc(sizeof(CsrBtAvSecurityControlRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->contProtMethodLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->contProtMethodData), buffer, &offset, ((CsrSize) sizeof(primitive->contProtMethodData)));
    CsrUint8Des((CsrUint8 *) &primitive->avResponse, buffer, &offset);
    if (primitive->contProtMethodLen)
    {
        primitive->contProtMethodData = CsrPmemZalloc(((CsrUint32) (primitive->contProtMethodLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->contProtMethodData), buffer, &offset, ((CsrSize) (primitive->contProtMethodLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->contProtMethodData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_RES */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_RES
void CsrBtAvSecurityControlResSerFree(void *msg)
{
    CsrBtAvSecurityControlRes *primitive;

    primitive = (CsrBtAvSecurityControlRes *) msg;

    if(primitive->contProtMethodData != NULL)
    {
        CsrPmemFree(primitive->contProtMethodData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_RES */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_REQ
CsrSize CsrBtAvSecurityControlReqSizeof(void *msg)
{
    CsrBtAvSecurityControlReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->contProtMethodLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_REQ
CsrUint8 *CsrBtAvSecurityControlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSecurityControlReq *primitive;

    primitive = (CsrBtAvSecurityControlReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->contProtMethodLen);
    CsrMemCpySer(buffer, length, &primitive->contProtMethodData, sizeof(primitive->contProtMethodData));
    if(primitive->contProtMethodData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->contProtMethodData), ((CsrSize) (primitive->contProtMethodLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_REQ
void *CsrBtAvSecurityControlReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSecurityControlReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSecurityControlReq *) CsrPmemZalloc(sizeof(CsrBtAvSecurityControlReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->contProtMethodLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->contProtMethodData), buffer, &offset, ((CsrSize) sizeof(primitive->contProtMethodData)));
    if (primitive->contProtMethodLen)
    {
        primitive->contProtMethodData = CsrPmemZalloc(((CsrUint32) (primitive->contProtMethodLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->contProtMethodData), buffer, &offset, ((CsrSize) (primitive->contProtMethodLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->contProtMethodData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_REQ
void CsrBtAvSecurityControlReqSerFree(void *msg)
{
    CsrBtAvSecurityControlReq *primitive;

    primitive = (CsrBtAvSecurityControlReq *) msg;

    if(primitive->contProtMethodData != NULL)
    {
        CsrPmemFree(primitive->contProtMethodData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_CFM
CsrSize CsrBtAvSecurityControlCfmSizeof(void *msg)
{
    CsrBtAvSecurityControlCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->contProtMethodLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_CFM
CsrUint8 *CsrBtAvSecurityControlCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSecurityControlCfm *primitive;

    primitive = (CsrBtAvSecurityControlCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->contProtMethodLen);
    CsrMemCpySer(buffer, length, &primitive->contProtMethodData, sizeof(primitive->contProtMethodData));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultSupplier);
    if(primitive->contProtMethodData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->contProtMethodData), ((CsrSize) (primitive->contProtMethodLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_CFM
void *CsrBtAvSecurityControlCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSecurityControlCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSecurityControlCfm *) CsrPmemZalloc(sizeof(CsrBtAvSecurityControlCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->contProtMethodLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->contProtMethodData), buffer, &offset, ((CsrSize) sizeof(primitive->contProtMethodData)));
    CsrUint16Des((CsrUint16 *) &primitive->avResultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultSupplier, buffer, &offset);
    if (primitive->contProtMethodLen)
    {
        primitive->contProtMethodData = CsrPmemZalloc(((CsrUint32) (primitive->contProtMethodLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->contProtMethodData), buffer, &offset, ((CsrSize) (primitive->contProtMethodLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->contProtMethodData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_CFM
void CsrBtAvSecurityControlCfmSerFree(void *msg)
{
    CsrBtAvSecurityControlCfm *primitive;

    primitive = (CsrBtAvSecurityControlCfm *) msg;

    if(primitive->contProtMethodData != NULL)
    {
        CsrPmemFree(primitive->contProtMethodData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_CONTROL_CFM */

#ifndef EXCLUDE_CSR_BT_AV_STATUS_IND
CsrSize CsrBtAvStatusIndSizeof(void *msg)
{
    CsrBtAvStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AV_STATUS_IND
CsrUint8 *CsrBtAvStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvStatusInd *primitive;

    primitive = (CsrBtAvStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->statusType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->roleType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AV_STATUS_IND
void *CsrBtAvStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvStatusInd *) CsrPmemZalloc(sizeof(CsrBtAvStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->statusType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->roleType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AV_ABORT_REQ
CsrSize CsrBtAvAbortReqSizeof(void *msg)
{
    CsrBtAvAbortReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_ABORT_REQ
CsrUint8 *CsrBtAvAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvAbortReq *primitive;

    primitive = (CsrBtAvAbortReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_ABORT_REQ
void *CsrBtAvAbortReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvAbortReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvAbortReq *) CsrPmemZalloc(sizeof(CsrBtAvAbortReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_IN_REQ
CsrSize CsrBtAvSecurityInReqSizeof(void *msg)
{
    CsrBtAvSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_IN_REQ
CsrUint8 *CsrBtAvSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSecurityInReq *primitive;

    primitive = (CsrBtAvSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_IN_REQ
void *CsrBtAvSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtAvSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_AV_ABORT_IND
CsrSize CsrBtAvAbortIndSizeof(void *msg)
{
    CsrBtAvAbortInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_AV_ABORT_IND
CsrUint8 *CsrBtAvAbortIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvAbortInd *primitive;

    primitive = (CsrBtAvAbortInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_AV_ABORT_IND
void *CsrBtAvAbortIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvAbortInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvAbortInd *) CsrPmemZalloc(sizeof(CsrBtAvAbortInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_ABORT_IND */

#ifndef EXCLUDE_CSR_BT_AV_CLOSE_CFM
CsrSize CsrBtAvCloseCfmSizeof(void *msg)
{
    CsrBtAvCloseCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_CLOSE_CFM
CsrUint8 *CsrBtAvCloseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvCloseCfm *primitive;

    primitive = (CsrBtAvCloseCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_CLOSE_CFM
void *CsrBtAvCloseCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvCloseCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvCloseCfm *) CsrPmemZalloc(sizeof(CsrBtAvCloseCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_STREAM_DATA_IND
CsrSize CsrBtAvStreamDataIndSizeof(void *msg)
{
    CsrBtAvStreamDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_STREAM_DATA_IND */

#ifndef EXCLUDE_CSR_BT_AV_STREAM_DATA_IND
CsrUint8 *CsrBtAvStreamDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvStreamDataInd *primitive;

    primitive = (CsrBtAvStreamDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pad);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pad2);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_STREAM_DATA_IND */

#ifndef EXCLUDE_CSR_BT_AV_STREAM_DATA_IND
void *CsrBtAvStreamDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvStreamDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvStreamDataInd *) CsrPmemZalloc(sizeof(CsrBtAvStreamDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pad, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pad2, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    if (primitive->length)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_STREAM_DATA_IND */

#ifndef EXCLUDE_CSR_BT_AV_STREAM_DATA_IND
void CsrBtAvStreamDataIndSerFree(void *msg)
{
    CsrBtAvStreamDataInd *primitive;

    primitive = (CsrBtAvStreamDataInd *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_STREAM_DATA_IND */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_CFM
CsrSize CsrBtAvGetConfigurationCfmSizeof(void *msg)
{
    CsrBtAvGetConfigurationCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->servCapLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_CFM
CsrUint8 *CsrBtAvGetConfigurationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvGetConfigurationCfm *primitive;

    primitive = (CsrBtAvGetConfigurationCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->servCapLen);
    CsrMemCpySer(buffer, length, &primitive->servCapData, sizeof(primitive->servCapData));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultSupplier);
    if(primitive->servCapData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->servCapData), ((CsrSize) (primitive->servCapLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_CFM
void *CsrBtAvGetConfigurationCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvGetConfigurationCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvGetConfigurationCfm *) CsrPmemZalloc(sizeof(CsrBtAvGetConfigurationCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->servCapLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->servCapData), buffer, &offset, ((CsrSize) sizeof(primitive->servCapData)));
    CsrUint16Des((CsrUint16 *) &primitive->avResultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultSupplier, buffer, &offset);
    if (primitive->servCapLen)
    {
        primitive->servCapData = CsrPmemZalloc(((CsrUint32) (primitive->servCapLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->servCapData), buffer, &offset, ((CsrSize) (primitive->servCapLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->servCapData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_CFM
void CsrBtAvGetConfigurationCfmSerFree(void *msg)
{
    CsrBtAvGetConfigurationCfm *primitive;

    primitive = (CsrBtAvGetConfigurationCfm *) msg;

    if(primitive->servCapData != NULL)
    {
        CsrPmemFree(primitive->servCapData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_AV_ACTIVATE_CFM
CsrSize CsrBtAvActivateCfmSizeof(void *msg)
{
    CsrBtAvActivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_ACTIVATE_CFM
CsrUint8 *CsrBtAvActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvActivateCfm *primitive;

    primitive = (CsrBtAvActivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_ACTIVATE_CFM
void *CsrBtAvActivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvActivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvActivateCfm *) CsrPmemZalloc(sizeof(CsrBtAvActivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_HOUSE_CLEANING
CsrSize CsrBtAvHouseCleaningSizeof(void *msg)
{
    CsrBtAvHouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_AV_HOUSE_CLEANING
CsrUint8 *CsrBtAvHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvHouseCleaning *primitive;

    primitive = (CsrBtAvHouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_AV_HOUSE_CLEANING
void *CsrBtAvHouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvHouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvHouseCleaning *) CsrPmemZalloc(sizeof(CsrBtAvHouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_CFM
CsrSize CsrBtAvSetConfigurationCfmSizeof(void *msg)
{
    CsrBtAvSetConfigurationCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_CFM
CsrUint8 *CsrBtAvSetConfigurationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSetConfigurationCfm *primitive;

    primitive = (CsrBtAvSetConfigurationCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->servCategory);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_CFM
void *CsrBtAvSetConfigurationCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSetConfigurationCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSetConfigurationCfm *) CsrPmemZalloc(sizeof(CsrBtAvSetConfigurationCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->servCategory, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_AV_LP_SUPERVISION_TIMER
CsrSize CsrBtAvLpSupervisionTimerSizeof(void *msg)
{
    CsrBtAvLpSupervisionTimer *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_LP_SUPERVISION_TIMER */

#ifndef EXCLUDE_CSR_BT_AV_LP_SUPERVISION_TIMER
CsrUint8 *CsrBtAvLpSupervisionTimerSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvLpSupervisionTimer *primitive;

    primitive = (CsrBtAvLpSupervisionTimer *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_LP_SUPERVISION_TIMER */

#ifndef EXCLUDE_CSR_BT_AV_LP_SUPERVISION_TIMER
void *CsrBtAvLpSupervisionTimerDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvLpSupervisionTimer *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvLpSupervisionTimer *) CsrPmemZalloc(sizeof(CsrBtAvLpSupervisionTimer));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_LP_SUPERVISION_TIMER */

#ifndef EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_CFM
CsrSize CsrBtAvGetChannelInfoCfmSizeof(void *msg)
{
    CsrBtAvGetChannelInfoCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_CFM
CsrUint8 *CsrBtAvGetChannelInfoCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvGetChannelInfoCfm *primitive;

    primitive = (CsrBtAvGetChannelInfoCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->aclHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remoteCid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_CFM
void *CsrBtAvGetChannelInfoCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvGetChannelInfoCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvGetChannelInfoCfm *) CsrPmemZalloc(sizeof(CsrBtAvGetChannelInfoCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->aclHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remoteCid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_AV_START_REQ
CsrSize CsrBtAvStartReqSizeof(void *msg)
{
    CsrBtAvStartReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->listLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_START_REQ */

#ifndef EXCLUDE_CSR_BT_AV_START_REQ
CsrUint8 *CsrBtAvStartReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvStartReq *primitive;

    primitive = (CsrBtAvStartReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->listLength);
    CsrMemCpySer(buffer, length, &primitive->list, sizeof(primitive->list));
    if(primitive->list)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->list), ((CsrSize) (primitive->listLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_START_REQ */

#ifndef EXCLUDE_CSR_BT_AV_START_REQ
void *CsrBtAvStartReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvStartReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvStartReq *) CsrPmemZalloc(sizeof(CsrBtAvStartReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->listLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->list), buffer, &offset, ((CsrSize) sizeof(primitive->list)));
    if (primitive->listLength)
    {
        primitive->list = CsrPmemZalloc(((CsrUint32) (primitive->listLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->list), buffer, &offset, ((CsrSize) (primitive->listLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->list = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_START_REQ */

#ifndef EXCLUDE_CSR_BT_AV_START_REQ
void CsrBtAvStartReqSerFree(void *msg)
{
    CsrBtAvStartReq *primitive;

    primitive = (CsrBtAvStartReq *) msg;

    if(primitive->list != NULL)
    {
        CsrPmemFree(primitive->list);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_START_REQ */

#ifndef EXCLUDE_CSR_BT_AV_DEACTIVATE_CFM
CsrSize CsrBtAvDeactivateCfmSizeof(void *msg)
{
    CsrBtAvDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_DEACTIVATE_CFM
CsrUint8 *CsrBtAvDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvDeactivateCfm *primitive;

    primitive = (CsrBtAvDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_DEACTIVATE_CFM
void *CsrBtAvDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtAvDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_IND
CsrSize CsrBtAvReconfigureIndSizeof(void *msg)
{
    CsrBtAvReconfigureInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->servCapLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_IND */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_IND
CsrUint8 *CsrBtAvReconfigureIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvReconfigureInd *primitive;

    primitive = (CsrBtAvReconfigureInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->servCapLen);
    CsrMemCpySer(buffer, length, &primitive->servCapData, sizeof(primitive->servCapData));
    if(primitive->servCapData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->servCapData), ((CsrSize) (primitive->servCapLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_IND */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_IND
void *CsrBtAvReconfigureIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvReconfigureInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvReconfigureInd *) CsrPmemZalloc(sizeof(CsrBtAvReconfigureInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->servCapLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->servCapData), buffer, &offset, ((CsrSize) sizeof(primitive->servCapData)));
    if (primitive->servCapLen)
    {
        primitive->servCapData = CsrPmemZalloc(((CsrUint32) (primitive->servCapLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->servCapData), buffer, &offset, ((CsrSize) (primitive->servCapLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->servCapData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_IND */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_IND
void CsrBtAvReconfigureIndSerFree(void *msg)
{
    CsrBtAvReconfigureInd *primitive;

    primitive = (CsrBtAvReconfigureInd *) msg;

    if(primitive->servCapData != NULL)
    {
        CsrPmemFree(primitive->servCapData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_IND */

#ifndef EXCLUDE_CSR_BT_AV_DELAY_REPORT_REQ
CsrSize CsrBtAvDelayReportReqSizeof(void *msg)
{
    CsrBtAvDelayReportReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_DELAY_REPORT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_DELAY_REPORT_REQ
CsrUint8 *CsrBtAvDelayReportReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvDelayReportReq *primitive;

    primitive = (CsrBtAvDelayReportReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->delay);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_DELAY_REPORT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_DELAY_REPORT_REQ
void *CsrBtAvDelayReportReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvDelayReportReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvDelayReportReq *) CsrPmemZalloc(sizeof(CsrBtAvDelayReportReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->delay, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_DELAY_REPORT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_LP_NEG_CONFIG_REQ
CsrSize CsrBtAvLpNegConfigReqSizeof(void *msg)
{
    CsrBtAvLpNegConfigReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_LP_NEG_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_AV_LP_NEG_CONFIG_REQ
CsrUint8 *CsrBtAvLpNegConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvLpNegConfigReq *primitive;

    primitive = (CsrBtAvLpNegConfigReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enable);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_LP_NEG_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_AV_LP_NEG_CONFIG_REQ
void *CsrBtAvLpNegConfigReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvLpNegConfigReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvLpNegConfigReq *) CsrPmemZalloc(sizeof(CsrBtAvLpNegConfigReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_LP_NEG_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_AV_START_IND
CsrSize CsrBtAvStartIndSizeof(void *msg)
{
    CsrBtAvStartInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->listLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_START_IND */

#ifndef EXCLUDE_CSR_BT_AV_START_IND
CsrUint8 *CsrBtAvStartIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvStartInd *primitive;

    primitive = (CsrBtAvStartInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->listLength);
    CsrMemCpySer(buffer, length, &primitive->list, sizeof(primitive->list));
    if(primitive->list)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->list), ((CsrSize) (primitive->listLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_START_IND */

#ifndef EXCLUDE_CSR_BT_AV_START_IND
void *CsrBtAvStartIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvStartInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvStartInd *) CsrPmemZalloc(sizeof(CsrBtAvStartInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->listLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->list), buffer, &offset, ((CsrSize) sizeof(primitive->list)));
    if (primitive->listLength)
    {
        primitive->list = CsrPmemZalloc(((CsrUint32) (primitive->listLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->list), buffer, &offset, ((CsrSize) (primitive->listLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->list = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_START_IND */

#ifndef EXCLUDE_CSR_BT_AV_START_IND
void CsrBtAvStartIndSerFree(void *msg)
{
    CsrBtAvStartInd *primitive;

    primitive = (CsrBtAvStartInd *) msg;

    if(primitive->list != NULL)
    {
        CsrPmemFree(primitive->list);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_START_IND */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_REQ
CsrSize CsrBtAvGetCapabilitiesReqSizeof(void *msg)
{
    CsrBtAvGetCapabilitiesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_REQ */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_REQ
CsrUint8 *CsrBtAvGetCapabilitiesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvGetCapabilitiesReq *primitive;

    primitive = (CsrBtAvGetCapabilitiesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->acpSeid);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_REQ */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_REQ
void *CsrBtAvGetCapabilitiesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvGetCapabilitiesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvGetCapabilitiesReq *) CsrPmemZalloc(sizeof(CsrBtAvGetCapabilitiesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->acpSeid, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_OUT_CFM
CsrSize CsrBtAvSecurityOutCfmSizeof(void *msg)
{
    CsrBtAvSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_OUT_CFM
CsrUint8 *CsrBtAvSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSecurityOutCfm *primitive;

    primitive = (CsrBtAvSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_OUT_CFM
void *CsrBtAvSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtAvSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_OUT_REQ
CsrSize CsrBtAvSecurityOutReqSizeof(void *msg)
{
    CsrBtAvSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_OUT_REQ
CsrUint8 *CsrBtAvSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSecurityOutReq *primitive;

    primitive = (CsrBtAvSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SECURITY_OUT_REQ
void *CsrBtAvSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtAvSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_CONNECT_REQ
CsrSize CsrBtAvConnectReqSizeof(void *msg)
{
    CsrBtAvConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_CONNECT_REQ
CsrUint8 *CsrBtAvConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvConnectReq *primitive;

    primitive = (CsrBtAvConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remoteRole);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localRole);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_CONNECT_REQ
void *CsrBtAvConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvConnectReq *) CsrPmemZalloc(sizeof(CsrBtAvConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remoteRole, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localRole, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_OPEN_IND
CsrSize CsrBtAvOpenIndSizeof(void *msg)
{
    CsrBtAvOpenInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_OPEN_IND */

#ifndef EXCLUDE_CSR_BT_AV_OPEN_IND
CsrUint8 *CsrBtAvOpenIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvOpenInd *primitive;

    primitive = (CsrBtAvOpenInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_OPEN_IND */

#ifndef EXCLUDE_CSR_BT_AV_OPEN_IND
void *CsrBtAvOpenIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvOpenInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvOpenInd *) CsrPmemZalloc(sizeof(CsrBtAvOpenInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_OPEN_IND */

#ifndef EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_REQ
CsrSize CsrBtAvGetChannelInfoReqSizeof(void *msg)
{
    CsrBtAvGetChannelInfoReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_REQ
CsrUint8 *CsrBtAvGetChannelInfoReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvGetChannelInfoReq *primitive;

    primitive = (CsrBtAvGetChannelInfoReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_REQ
void *CsrBtAvGetChannelInfoReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvGetChannelInfoReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvGetChannelInfoReq *) CsrPmemZalloc(sizeof(CsrBtAvGetChannelInfoReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CHANNEL_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_RES
CsrSize CsrBtAvSuspendResSizeof(void *msg)
{
    CsrBtAvSuspendRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->listLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_RES */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_RES
CsrUint8 *CsrBtAvSuspendResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSuspendRes *primitive;

    primitive = (CsrBtAvSuspendRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reject_shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->listLength);
    CsrMemCpySer(buffer, length, &primitive->list, sizeof(primitive->list));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->avResponse);
    if(primitive->list)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->list), ((CsrSize) (primitive->listLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_RES */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_RES
void *CsrBtAvSuspendResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSuspendRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSuspendRes *) CsrPmemZalloc(sizeof(CsrBtAvSuspendRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reject_shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->listLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->list), buffer, &offset, ((CsrSize) sizeof(primitive->list)));
    CsrUint8Des((CsrUint8 *) &primitive->avResponse, buffer, &offset);
    if (primitive->listLength)
    {
        primitive->list = CsrPmemZalloc(((CsrUint32) (primitive->listLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->list), buffer, &offset, ((CsrSize) (primitive->listLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->list = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_RES */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_RES
void CsrBtAvSuspendResSerFree(void *msg)
{
    CsrBtAvSuspendRes *primitive;

    primitive = (CsrBtAvSuspendRes *) msg;

    if(primitive->list != NULL)
    {
        CsrPmemFree(primitive->list);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_RES */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_RES
CsrSize CsrBtAvDiscoverResSizeof(void *msg)
{
    CsrBtAvDiscoverRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->seidInfoCount * sizeof(CsrBtAvSeidInfo)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_RES */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_RES
CsrUint8 *CsrBtAvDiscoverResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvDiscoverRes *primitive;

    primitive = (CsrBtAvDiscoverRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->seidInfoCount);
    CsrMemCpySer(buffer, length, &primitive->seidInfo, sizeof(primitive->seidInfo));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->avResponse);
    if(primitive->seidInfo)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->seidInfo), ((CsrSize) (primitive->seidInfoCount * sizeof(CsrBtAvSeidInfo))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_RES */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_RES
void *CsrBtAvDiscoverResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvDiscoverRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvDiscoverRes *) CsrPmemZalloc(sizeof(CsrBtAvDiscoverRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->seidInfoCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->seidInfo), buffer, &offset, ((CsrSize) sizeof(primitive->seidInfo)));
    CsrUint8Des((CsrUint8 *) &primitive->avResponse, buffer, &offset);
    if (primitive->seidInfoCount)
    {
        primitive->seidInfo = CsrPmemZalloc(((CsrUint32) (primitive->seidInfoCount * sizeof(CsrBtAvSeidInfo))));
        CsrMemCpyDes(((void *) primitive->seidInfo), buffer, &offset, ((CsrSize) (primitive->seidInfoCount * sizeof(CsrBtAvSeidInfo))));
    }
    else
    {
        primitive->seidInfo = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_RES */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_RES
void CsrBtAvDiscoverResSerFree(void *msg)
{
    CsrBtAvDiscoverRes *primitive;

    primitive = (CsrBtAvDiscoverRes *) msg;

    if(primitive->seidInfo != NULL)
    {
        CsrPmemFree(primitive->seidInfo);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_RES */

#ifndef EXCLUDE_CSR_BT_AV_DISCONNECT_IND
CsrSize CsrBtAvDisconnectIndSizeof(void *msg)
{
    CsrBtAvDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AV_DISCONNECT_IND
CsrUint8 *CsrBtAvDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvDisconnectInd *primitive;

    primitive = (CsrBtAvDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localTerminated);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AV_DISCONNECT_IND
void *CsrBtAvDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtAvDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localTerminated, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AV_CONNECT_CFM
CsrSize CsrBtAvConnectCfmSizeof(void *msg)
{
    CsrBtAvConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_AV_CONNECT_CFM
CsrUint8 *CsrBtAvConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvConnectCfm *primitive;

    primitive = (CsrBtAvConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_AV_CONNECT_CFM
void *CsrBtAvConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvConnectCfm *) CsrPmemZalloc(sizeof(CsrBtAvConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_RES
CsrSize CsrBtAvGetCapabilitiesResSizeof(void *msg)
{
    CsrBtAvGetCapabilitiesRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->servCapLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_RES
CsrUint8 *CsrBtAvGetCapabilitiesResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvGetCapabilitiesRes *primitive;

    primitive = (CsrBtAvGetCapabilitiesRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->servCapLen);
    CsrMemCpySer(buffer, length, &primitive->servCapData, sizeof(primitive->servCapData));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->avResponse);
    if(primitive->servCapData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->servCapData), ((CsrSize) (primitive->servCapLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_RES
void *CsrBtAvGetCapabilitiesResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvGetCapabilitiesRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvGetCapabilitiesRes *) CsrPmemZalloc(sizeof(CsrBtAvGetCapabilitiesRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->servCapLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->servCapData), buffer, &offset, ((CsrSize) sizeof(primitive->servCapData)));
    CsrUint8Des((CsrUint8 *) &primitive->avResponse, buffer, &offset);
    if (primitive->servCapLen)
    {
        primitive->servCapData = CsrPmemZalloc(((CsrUint32) (primitive->servCapLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->servCapData), buffer, &offset, ((CsrSize) (primitive->servCapLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->servCapData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_RES
void CsrBtAvGetCapabilitiesResSerFree(void *msg)
{
    CsrBtAvGetCapabilitiesRes *primitive;

    primitive = (CsrBtAvGetCapabilitiesRes *) msg;

    if(primitive->servCapData != NULL)
    {
        CsrPmemFree(primitive->servCapData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_AV_CONNECT_IND
CsrSize CsrBtAvConnectIndSizeof(void *msg)
{
    CsrBtAvConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AV_CONNECT_IND
CsrUint8 *CsrBtAvConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvConnectInd *primitive;

    primitive = (CsrBtAvConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AV_CONNECT_IND
void *CsrBtAvConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvConnectInd *) CsrPmemZalloc(sizeof(CsrBtAvConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AV_CLOSE_RES
CsrSize CsrBtAvCloseResSizeof(void *msg)
{
    CsrBtAvCloseRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_RES */

#ifndef EXCLUDE_CSR_BT_AV_CLOSE_RES
CsrUint8 *CsrBtAvCloseResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvCloseRes *primitive;

    primitive = (CsrBtAvCloseRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->avResponse);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_RES */

#ifndef EXCLUDE_CSR_BT_AV_CLOSE_RES
void *CsrBtAvCloseResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvCloseRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvCloseRes *) CsrPmemZalloc(sizeof(CsrBtAvCloseRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->avResponse, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_RES */

#ifndef EXCLUDE_CSR_BT_AV_ACTIVATE_REQ
CsrSize CsrBtAvActivateReqSizeof(void *msg)
{
    CsrBtAvActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_ACTIVATE_REQ
CsrUint8 *CsrBtAvActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvActivateReq *primitive;

    primitive = (CsrBtAvActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localRole);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_ACTIVATE_REQ
void *CsrBtAvActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvActivateReq *) CsrPmemZalloc(sizeof(CsrBtAvActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localRole, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_CFM
CsrSize CsrBtAvDiscoverCfmSizeof(void *msg)
{
    CsrBtAvDiscoverCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->seidInfoCount * sizeof(CsrBtAvSeidInfo)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_CFM */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_CFM
CsrUint8 *CsrBtAvDiscoverCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvDiscoverCfm *primitive;

    primitive = (CsrBtAvDiscoverCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->seidInfoCount);
    CsrMemCpySer(buffer, length, &primitive->seidInfo, sizeof(primitive->seidInfo));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultSupplier);
    if(primitive->seidInfo)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->seidInfo), ((CsrSize) (primitive->seidInfoCount * sizeof(CsrBtAvSeidInfo))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_CFM */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_CFM
void *CsrBtAvDiscoverCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvDiscoverCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvDiscoverCfm *) CsrPmemZalloc(sizeof(CsrBtAvDiscoverCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->seidInfoCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->seidInfo), buffer, &offset, ((CsrSize) sizeof(primitive->seidInfo)));
    CsrUint16Des((CsrUint16 *) &primitive->avResultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultSupplier, buffer, &offset);
    if (primitive->seidInfoCount)
    {
        primitive->seidInfo = CsrPmemZalloc(((CsrUint32) (primitive->seidInfoCount * sizeof(CsrBtAvSeidInfo))));
        CsrMemCpyDes(((void *) primitive->seidInfo), buffer, &offset, ((CsrSize) (primitive->seidInfoCount * sizeof(CsrBtAvSeidInfo))));
    }
    else
    {
        primitive->seidInfo = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_CFM */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_CFM
void CsrBtAvDiscoverCfmSerFree(void *msg)
{
    CsrBtAvDiscoverCfm *primitive;

    primitive = (CsrBtAvDiscoverCfm *) msg;

    if(primitive->seidInfo != NULL)
    {
        CsrPmemFree(primitive->seidInfo);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_CFM */

#ifndef EXCLUDE_CSR_BT_AV_DELAY_REPORT_IND
CsrSize CsrBtAvDelayReportIndSizeof(void *msg)
{
    CsrBtAvDelayReportInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_DELAY_REPORT_IND */

#ifndef EXCLUDE_CSR_BT_AV_DELAY_REPORT_IND
CsrUint8 *CsrBtAvDelayReportIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvDelayReportInd *primitive;

    primitive = (CsrBtAvDelayReportInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->delay);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_DELAY_REPORT_IND */

#ifndef EXCLUDE_CSR_BT_AV_DELAY_REPORT_IND
void *CsrBtAvDelayReportIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvDelayReportInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvDelayReportInd *) CsrPmemZalloc(sizeof(CsrBtAvDelayReportInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->delay, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_DELAY_REPORT_IND */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_REQ
CsrSize CsrBtAvGetConfigurationReqSizeof(void *msg)
{
    CsrBtAvGetConfigurationReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_REQ */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_REQ
CsrUint8 *CsrBtAvGetConfigurationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvGetConfigurationReq *primitive;

    primitive = (CsrBtAvGetConfigurationReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_REQ */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_REQ
void *CsrBtAvGetConfigurationReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvGetConfigurationReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvGetConfigurationReq *) CsrPmemZalloc(sizeof(CsrBtAvGetConfigurationReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_REQ */

#ifndef EXCLUDE_CSR_BT_AV_OPEN_CFM
CsrSize CsrBtAvOpenCfmSizeof(void *msg)
{
    CsrBtAvOpenCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_OPEN_CFM */

#ifndef EXCLUDE_CSR_BT_AV_OPEN_CFM
CsrUint8 *CsrBtAvOpenCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvOpenCfm *primitive;

    primitive = (CsrBtAvOpenCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_OPEN_CFM */

#ifndef EXCLUDE_CSR_BT_AV_OPEN_CFM
void *CsrBtAvOpenCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvOpenCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvOpenCfm *) CsrPmemZalloc(sizeof(CsrBtAvOpenCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_OPEN_CFM */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_CFM
CsrSize CsrBtAvReconfigureCfmSizeof(void *msg)
{
    CsrBtAvReconfigureCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_CFM
CsrUint8 *CsrBtAvReconfigureCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvReconfigureCfm *primitive;

    primitive = (CsrBtAvReconfigureCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->servCategory);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_CFM
void *CsrBtAvReconfigureCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvReconfigureCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvReconfigureCfm *) CsrPmemZalloc(sizeof(CsrBtAvReconfigureCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->servCategory, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_ABORT_CFM
CsrSize CsrBtAvAbortCfmSizeof(void *msg)
{
    CsrBtAvAbortCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_AV_ABORT_CFM
CsrUint8 *CsrBtAvAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvAbortCfm *primitive;

    primitive = (CsrBtAvAbortCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_AV_ABORT_CFM
void *CsrBtAvAbortCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvAbortCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvAbortCfm *) CsrPmemZalloc(sizeof(CsrBtAvAbortCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_REQ
CsrSize CsrBtAvSetConfigurationReqSizeof(void *msg)
{
    CsrBtAvSetConfigurationReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->appServCapLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_REQ
CsrUint8 *CsrBtAvSetConfigurationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSetConfigurationReq *primitive;

    primitive = (CsrBtAvSetConfigurationReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->acpSeid);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->intSeid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appServCapLen);
    CsrMemCpySer(buffer, length, &primitive->appServCapData, sizeof(primitive->appServCapData));
    if(primitive->appServCapData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->appServCapData), ((CsrSize) (primitive->appServCapLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_REQ
void *CsrBtAvSetConfigurationReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSetConfigurationReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSetConfigurationReq *) CsrPmemZalloc(sizeof(CsrBtAvSetConfigurationReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->acpSeid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->intSeid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appServCapLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->appServCapData), buffer, &offset, ((CsrSize) sizeof(primitive->appServCapData)));
    if (primitive->appServCapLen)
    {
        primitive->appServCapData = CsrPmemZalloc(((CsrUint32) (primitive->appServCapLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->appServCapData), buffer, &offset, ((CsrSize) (primitive->appServCapLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->appServCapData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_REQ
void CsrBtAvSetConfigurationReqSerFree(void *msg)
{
    CsrBtAvSetConfigurationReq *primitive;

    primitive = (CsrBtAvSetConfigurationReq *) msg;

    if(primitive->appServCapData != NULL)
    {
        CsrPmemFree(primitive->appServCapData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_REQ */

#ifndef EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_CFM
CsrSize CsrBtAvRegisterStreamHandleCfmSizeof(void *msg)
{
    CsrBtAvRegisterStreamHandleCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_CFM
CsrUint8 *CsrBtAvRegisterStreamHandleCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvRegisterStreamHandleCfm *primitive;

    primitive = (CsrBtAvRegisterStreamHandleCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_CFM
void *CsrBtAvRegisterStreamHandleCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvRegisterStreamHandleCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvRegisterStreamHandleCfm *) CsrPmemZalloc(sizeof(CsrBtAvRegisterStreamHandleCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_CFM */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_IND
CsrSize CsrBtAvGetConfigurationIndSizeof(void *msg)
{
    CsrBtAvGetConfigurationInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_IND */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_IND
CsrUint8 *CsrBtAvGetConfigurationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvGetConfigurationInd *primitive;

    primitive = (CsrBtAvGetConfigurationInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_IND */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_IND
void *CsrBtAvGetConfigurationIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvGetConfigurationInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvGetConfigurationInd *) CsrPmemZalloc(sizeof(CsrBtAvGetConfigurationInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_IND */

#ifndef EXCLUDE_CSR_BT_AV_START_RES
CsrSize CsrBtAvStartResSizeof(void *msg)
{
    CsrBtAvStartRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->listLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_START_RES */

#ifndef EXCLUDE_CSR_BT_AV_START_RES
CsrUint8 *CsrBtAvStartResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvStartRes *primitive;

    primitive = (CsrBtAvStartRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reject_shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->listLength);
    CsrMemCpySer(buffer, length, &primitive->list, sizeof(primitive->list));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->avResponse);
    if(primitive->list)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->list), ((CsrSize) (primitive->listLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_START_RES */

#ifndef EXCLUDE_CSR_BT_AV_START_RES
void *CsrBtAvStartResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvStartRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvStartRes *) CsrPmemZalloc(sizeof(CsrBtAvStartRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reject_shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->listLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->list), buffer, &offset, ((CsrSize) sizeof(primitive->list)));
    CsrUint8Des((CsrUint8 *) &primitive->avResponse, buffer, &offset);
    if (primitive->listLength)
    {
        primitive->list = CsrPmemZalloc(((CsrUint32) (primitive->listLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->list), buffer, &offset, ((CsrSize) (primitive->listLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->list = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_START_RES */

#ifndef EXCLUDE_CSR_BT_AV_START_RES
void CsrBtAvStartResSerFree(void *msg)
{
    CsrBtAvStartRes *primitive;

    primitive = (CsrBtAvStartRes *) msg;

    if(primitive->list != NULL)
    {
        CsrPmemFree(primitive->list);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_START_RES */

#ifndef EXCLUDE_CSR_BT_AV_CLOSE_REQ
CsrSize CsrBtAvCloseReqSizeof(void *msg)
{
    CsrBtAvCloseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_CLOSE_REQ
CsrUint8 *CsrBtAvCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvCloseReq *primitive;

    primitive = (CsrBtAvCloseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_CLOSE_REQ
void *CsrBtAvCloseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvCloseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvCloseReq *) CsrPmemZalloc(sizeof(CsrBtAvCloseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_DISCONNECT_REQ
CsrSize CsrBtAvDisconnectReqSizeof(void *msg)
{
    CsrBtAvDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_DISCONNECT_REQ
CsrUint8 *CsrBtAvDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvDisconnectReq *primitive;

    primitive = (CsrBtAvDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_DISCONNECT_REQ
void *CsrBtAvDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtAvDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_REQ
CsrSize CsrBtAvRegisterStreamHandleReqSizeof(void *msg)
{
    CsrBtAvRegisterStreamHandleReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_REQ
CsrUint8 *CsrBtAvRegisterStreamHandleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvRegisterStreamHandleReq *primitive;

    primitive = (CsrBtAvRegisterStreamHandleReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->streamHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_REQ
void *CsrBtAvRegisterStreamHandleReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvRegisterStreamHandleReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvRegisterStreamHandleReq *) CsrPmemZalloc(sizeof(CsrBtAvRegisterStreamHandleReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->streamHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_REGISTER_STREAM_HANDLE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_CLOSE_IND
CsrSize CsrBtAvCloseIndSizeof(void *msg)
{
    CsrBtAvCloseInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_IND */

#ifndef EXCLUDE_CSR_BT_AV_CLOSE_IND
CsrUint8 *CsrBtAvCloseIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvCloseInd *primitive;

    primitive = (CsrBtAvCloseInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_IND */

#ifndef EXCLUDE_CSR_BT_AV_CLOSE_IND
void *CsrBtAvCloseIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvCloseInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvCloseInd *) CsrPmemZalloc(sizeof(CsrBtAvCloseInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_CLOSE_IND */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_REQ
CsrSize CsrBtAvDiscoverReqSizeof(void *msg)
{
    CsrBtAvDiscoverReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_REQ */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_REQ
CsrUint8 *CsrBtAvDiscoverReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvDiscoverReq *primitive;

    primitive = (CsrBtAvDiscoverReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_REQ */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_REQ
void *CsrBtAvDiscoverReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvDiscoverReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvDiscoverReq *) CsrPmemZalloc(sizeof(CsrBtAvDiscoverReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_REQ */

#ifndef EXCLUDE_CSR_BT_AV_STREAM_MTU_SIZE_IND
CsrSize CsrBtAvStreamMtuSizeIndSizeof(void *msg)
{
    CsrBtAvStreamMtuSizeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_STREAM_MTU_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_AV_STREAM_MTU_SIZE_IND
CsrUint8 *CsrBtAvStreamMtuSizeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvStreamMtuSizeInd *primitive;

    primitive = (CsrBtAvStreamMtuSizeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remoteMtuSize);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_STREAM_MTU_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_AV_STREAM_MTU_SIZE_IND
void *CsrBtAvStreamMtuSizeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvStreamMtuSizeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvStreamMtuSizeInd *) CsrPmemZalloc(sizeof(CsrBtAvStreamMtuSizeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remoteMtuSize, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_STREAM_MTU_SIZE_IND */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_REQ
CsrSize CsrBtAvReconfigureReqSizeof(void *msg)
{
    CsrBtAvReconfigureReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->servCapLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_REQ
CsrUint8 *CsrBtAvReconfigureReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvReconfigureReq *primitive;

    primitive = (CsrBtAvReconfigureReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->servCapLen);
    CsrMemCpySer(buffer, length, &primitive->servCapData, sizeof(primitive->servCapData));
    if(primitive->servCapData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->servCapData), ((CsrSize) (primitive->servCapLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_REQ
void *CsrBtAvReconfigureReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvReconfigureReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvReconfigureReq *) CsrPmemZalloc(sizeof(CsrBtAvReconfigureReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->servCapLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->servCapData), buffer, &offset, ((CsrSize) sizeof(primitive->servCapData)));
    if (primitive->servCapLen)
    {
        primitive->servCapData = CsrPmemZalloc(((CsrUint32) (primitive->servCapLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->servCapData), buffer, &offset, ((CsrSize) (primitive->servCapLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->servCapData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_REQ
void CsrBtAvReconfigureReqSerFree(void *msg)
{
    CsrBtAvReconfigureReq *primitive;

    primitive = (CsrBtAvReconfigureReq *) msg;

    if(primitive->servCapData != NULL)
    {
        CsrPmemFree(primitive->servCapData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_REQ */

#ifndef EXCLUDE_CSR_BT_AV_QOS_IND
CsrSize CsrBtAvQosIndSizeof(void *msg)
{
    CsrBtAvQosInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_QOS_IND */

#ifndef EXCLUDE_CSR_BT_AV_QOS_IND
CsrUint8 *CsrBtAvQosIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvQosInd *primitive;

    primitive = (CsrBtAvQosInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bufferStatus);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_QOS_IND */

#ifndef EXCLUDE_CSR_BT_AV_QOS_IND
void *CsrBtAvQosIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvQosInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvQosInd *) CsrPmemZalloc(sizeof(CsrBtAvQosInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bufferStatus, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_QOS_IND */

#ifndef EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_RES
CsrSize CsrBtAvGetAllCapabilitiesResSizeof(void *msg)
{
    CsrBtAvGetAllCapabilitiesRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->servCapLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_RES
CsrUint8 *CsrBtAvGetAllCapabilitiesResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvGetAllCapabilitiesRes *primitive;

    primitive = (CsrBtAvGetAllCapabilitiesRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->servCapLen);
    CsrMemCpySer(buffer, length, &primitive->servCapData, sizeof(primitive->servCapData));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->avResponse);
    if(primitive->servCapData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->servCapData), ((CsrSize) (primitive->servCapLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_RES
void *CsrBtAvGetAllCapabilitiesResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvGetAllCapabilitiesRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvGetAllCapabilitiesRes *) CsrPmemZalloc(sizeof(CsrBtAvGetAllCapabilitiesRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->servCapLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->servCapData), buffer, &offset, ((CsrSize) sizeof(primitive->servCapData)));
    CsrUint8Des((CsrUint8 *) &primitive->avResponse, buffer, &offset);
    if (primitive->servCapLen)
    {
        primitive->servCapData = CsrPmemZalloc(((CsrUint32) (primitive->servCapLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->servCapData), buffer, &offset, ((CsrSize) (primitive->servCapLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->servCapData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_RES
void CsrBtAvGetAllCapabilitiesResSerFree(void *msg)
{
    CsrBtAvGetAllCapabilitiesRes *primitive;

    primitive = (CsrBtAvGetAllCapabilitiesRes *) msg;

    if(primitive->servCapData != NULL)
    {
        CsrPmemFree(primitive->servCapData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_CFM
CsrSize CsrBtAvGetCapabilitiesCfmSizeof(void *msg)
{
    CsrBtAvGetCapabilitiesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->servCapLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_CFM */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_CFM
CsrUint8 *CsrBtAvGetCapabilitiesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvGetCapabilitiesCfm *primitive;

    primitive = (CsrBtAvGetCapabilitiesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->servCapLen);
    CsrMemCpySer(buffer, length, &primitive->servCapData, sizeof(primitive->servCapData));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultSupplier);
    if(primitive->servCapData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->servCapData), ((CsrSize) (primitive->servCapLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_CFM */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_CFM
void *CsrBtAvGetCapabilitiesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvGetCapabilitiesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvGetCapabilitiesCfm *) CsrPmemZalloc(sizeof(CsrBtAvGetCapabilitiesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->servCapLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->servCapData), buffer, &offset, ((CsrSize) sizeof(primitive->servCapData)));
    CsrUint16Des((CsrUint16 *) &primitive->avResultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultSupplier, buffer, &offset);
    if (primitive->servCapLen)
    {
        primitive->servCapData = CsrPmemZalloc(((CsrUint32) (primitive->servCapLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->servCapData), buffer, &offset, ((CsrSize) (primitive->servCapLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->servCapData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_CFM */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_CFM
void CsrBtAvGetCapabilitiesCfmSerFree(void *msg)
{
    CsrBtAvGetCapabilitiesCfm *primitive;

    primitive = (CsrBtAvGetCapabilitiesCfm *) msg;

    if(primitive->servCapData != NULL)
    {
        CsrPmemFree(primitive->servCapData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_CFM */

#ifndef EXCLUDE_CSR_BT_AV_STREAM_DATA_REQ
CsrSize CsrBtAvStreamDataReqSizeof(void *msg)
{
    CsrBtAvStreamDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_STREAM_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_AV_STREAM_DATA_REQ
CsrUint8 *CsrBtAvStreamDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvStreamDataReq *primitive;

    primitive = (CsrBtAvStreamDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->hdr_type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_STREAM_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_AV_STREAM_DATA_REQ
void *CsrBtAvStreamDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvStreamDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvStreamDataReq *) CsrPmemZalloc(sizeof(CsrBtAvStreamDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->hdr_type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    if (primitive->length)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_STREAM_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_AV_STREAM_DATA_REQ
void CsrBtAvStreamDataReqSerFree(void *msg)
{
    CsrBtAvStreamDataReq *primitive;

    primitive = (CsrBtAvStreamDataReq *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_STREAM_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_IND
CsrSize CsrBtAvSuspendIndSizeof(void *msg)
{
    CsrBtAvSuspendInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->listLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_IND */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_IND
CsrUint8 *CsrBtAvSuspendIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSuspendInd *primitive;

    primitive = (CsrBtAvSuspendInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->listLength);
    CsrMemCpySer(buffer, length, &primitive->list, sizeof(primitive->list));
    if(primitive->list)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->list), ((CsrSize) (primitive->listLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_IND */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_IND
void *CsrBtAvSuspendIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSuspendInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSuspendInd *) CsrPmemZalloc(sizeof(CsrBtAvSuspendInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->listLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->list), buffer, &offset, ((CsrSize) sizeof(primitive->list)));
    if (primitive->listLength)
    {
        primitive->list = CsrPmemZalloc(((CsrUint32) (primitive->listLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->list), buffer, &offset, ((CsrSize) (primitive->listLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->list = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_IND */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_IND
void CsrBtAvSuspendIndSerFree(void *msg)
{
    CsrBtAvSuspendInd *primitive;

    primitive = (CsrBtAvSuspendInd *) msg;

    if(primitive->list != NULL)
    {
        CsrPmemFree(primitive->list);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_IND */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_RES
CsrSize CsrBtAvSetConfigurationResSizeof(void *msg)
{
    CsrBtAvSetConfigurationRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_RES */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_RES
CsrUint8 *CsrBtAvSetConfigurationResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSetConfigurationRes *primitive;

    primitive = (CsrBtAvSetConfigurationRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->servCategory);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->avResponse);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_RES */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_RES
void *CsrBtAvSetConfigurationResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSetConfigurationRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSetConfigurationRes *) CsrPmemZalloc(sizeof(CsrBtAvSetConfigurationRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->servCategory, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->avResponse, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_IND
CsrSize CsrBtAvGetCapabilitiesIndSizeof(void *msg)
{
    CsrBtAvGetCapabilitiesInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_IND
CsrUint8 *CsrBtAvGetCapabilitiesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvGetCapabilitiesInd *primitive;

    primitive = (CsrBtAvGetCapabilitiesInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->acpSeid);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_IND
void *CsrBtAvGetCapabilitiesIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvGetCapabilitiesInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvGetCapabilitiesInd *) CsrPmemZalloc(sizeof(CsrBtAvGetCapabilitiesInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->acpSeid, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_AV_START_CFM
CsrSize CsrBtAvStartCfmSizeof(void *msg)
{
    CsrBtAvStartCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_START_CFM */

#ifndef EXCLUDE_CSR_BT_AV_START_CFM
CsrUint8 *CsrBtAvStartCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvStartCfm *primitive;

    primitive = (CsrBtAvStartCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reject_shandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_START_CFM */

#ifndef EXCLUDE_CSR_BT_AV_START_CFM
void *CsrBtAvStartCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvStartCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvStartCfm *) CsrPmemZalloc(sizeof(CsrBtAvStartCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reject_shandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_START_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_IND
CsrSize CsrBtAvSetConfigurationIndSizeof(void *msg)
{
    CsrBtAvSetConfigurationInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->servCapLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_IND */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_IND
CsrUint8 *CsrBtAvSetConfigurationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSetConfigurationInd *primitive;

    primitive = (CsrBtAvSetConfigurationInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->acpSeid);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->intSeid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->servCapLen);
    CsrMemCpySer(buffer, length, &primitive->servCapData, sizeof(primitive->servCapData));
    if(primitive->servCapData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->servCapData), ((CsrSize) (primitive->servCapLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_IND */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_IND
void *CsrBtAvSetConfigurationIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSetConfigurationInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSetConfigurationInd *) CsrPmemZalloc(sizeof(CsrBtAvSetConfigurationInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->acpSeid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->intSeid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->servCapLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->servCapData), buffer, &offset, ((CsrSize) sizeof(primitive->servCapData)));
    if (primitive->servCapLen)
    {
        primitive->servCapData = CsrPmemZalloc(((CsrUint32) (primitive->servCapLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->servCapData), buffer, &offset, ((CsrSize) (primitive->servCapLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->servCapData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_IND */

#ifndef EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_IND
void CsrBtAvSetConfigurationIndSerFree(void *msg)
{
    CsrBtAvSetConfigurationInd *primitive;

    primitive = (CsrBtAvSetConfigurationInd *) msg;

    if(primitive->servCapData != NULL)
    {
        CsrPmemFree(primitive->servCapData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_SET_CONFIGURATION_IND */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_REQ
CsrSize CsrBtAvSuspendReqSizeof(void *msg)
{
    CsrBtAvSuspendReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->listLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_REQ
CsrUint8 *CsrBtAvSuspendReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSuspendReq *primitive;

    primitive = (CsrBtAvSuspendReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->listLength);
    CsrMemCpySer(buffer, length, &primitive->list, sizeof(primitive->list));
    if(primitive->list)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->list), ((CsrSize) (primitive->listLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_REQ
void *CsrBtAvSuspendReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSuspendReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSuspendReq *) CsrPmemZalloc(sizeof(CsrBtAvSuspendReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->listLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->list), buffer, &offset, ((CsrSize) sizeof(primitive->list)));
    if (primitive->listLength)
    {
        primitive->list = CsrPmemZalloc(((CsrUint32) (primitive->listLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->list), buffer, &offset, ((CsrSize) (primitive->listLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->list = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_REQ
void CsrBtAvSuspendReqSerFree(void *msg)
{
    CsrBtAvSuspendReq *primitive;

    primitive = (CsrBtAvSuspendReq *) msg;

    if(primitive->list != NULL)
    {
        CsrPmemFree(primitive->list);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_REQ */

#ifndef EXCLUDE_CSR_BT_AV_OPEN_RES
CsrSize CsrBtAvOpenResSizeof(void *msg)
{
    CsrBtAvOpenRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_OPEN_RES */

#ifndef EXCLUDE_CSR_BT_AV_OPEN_RES
CsrUint8 *CsrBtAvOpenResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvOpenRes *primitive;

    primitive = (CsrBtAvOpenRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->avResponse);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_OPEN_RES */

#ifndef EXCLUDE_CSR_BT_AV_OPEN_RES
void *CsrBtAvOpenResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvOpenRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvOpenRes *) CsrPmemZalloc(sizeof(CsrBtAvOpenRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->avResponse, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_OPEN_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_IND
CsrSize CsrBtAvGetAllCapabilitiesIndSizeof(void *msg)
{
    CsrBtAvGetAllCapabilitiesInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_IND
CsrUint8 *CsrBtAvGetAllCapabilitiesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvGetAllCapabilitiesInd *primitive;

    primitive = (CsrBtAvGetAllCapabilitiesInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->acpSeid);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_IND
void *CsrBtAvGetAllCapabilitiesIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvGetAllCapabilitiesInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvGetAllCapabilitiesInd *) CsrPmemZalloc(sizeof(CsrBtAvGetAllCapabilitiesInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->acpSeid, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_ALL_CAPABILITIES_IND */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_IND
CsrSize CsrBtAvDiscoverIndSizeof(void *msg)
{
    CsrBtAvDiscoverInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_IND */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_IND
CsrUint8 *CsrBtAvDiscoverIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvDiscoverInd *primitive;

    primitive = (CsrBtAvDiscoverInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_IND */

#ifndef EXCLUDE_CSR_BT_AV_DISCOVER_IND
void *CsrBtAvDiscoverIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvDiscoverInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvDiscoverInd *) CsrPmemZalloc(sizeof(CsrBtAvDiscoverInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_DISCOVER_IND */

#ifndef EXCLUDE_CSR_BT_AV_OPEN_REQ
CsrSize CsrBtAvOpenReqSizeof(void *msg)
{
    CsrBtAvOpenReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_AV_OPEN_REQ
CsrUint8 *CsrBtAvOpenReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvOpenReq *primitive;

    primitive = (CsrBtAvOpenReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_AV_OPEN_REQ
void *CsrBtAvOpenReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvOpenReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvOpenReq *) CsrPmemZalloc(sizeof(CsrBtAvOpenReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SET_QOS_INTERVAL_REQ
CsrSize CsrBtAvSetQosIntervalReqSizeof(void *msg)
{
    CsrBtAvSetQosIntervalReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SET_QOS_INTERVAL_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SET_QOS_INTERVAL_REQ
CsrUint8 *CsrBtAvSetQosIntervalReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSetQosIntervalReq *primitive;

    primitive = (CsrBtAvSetQosIntervalReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qosInterval);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SET_QOS_INTERVAL_REQ */

#ifndef EXCLUDE_CSR_BT_AV_SET_QOS_INTERVAL_REQ
void *CsrBtAvSetQosIntervalReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSetQosIntervalReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSetQosIntervalReq *) CsrPmemZalloc(sizeof(CsrBtAvSetQosIntervalReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qosInterval, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SET_QOS_INTERVAL_REQ */

#ifndef EXCLUDE_CSR_BT_AV_ABORT_RES
CsrSize CsrBtAvAbortResSizeof(void *msg)
{
    CsrBtAvAbortRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_ABORT_RES */

#ifndef EXCLUDE_CSR_BT_AV_ABORT_RES
CsrUint8 *CsrBtAvAbortResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvAbortRes *primitive;

    primitive = (CsrBtAvAbortRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_ABORT_RES */

#ifndef EXCLUDE_CSR_BT_AV_ABORT_RES
void *CsrBtAvAbortResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvAbortRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvAbortRes *) CsrPmemZalloc(sizeof(CsrBtAvAbortRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_ABORT_RES */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_CFM
CsrSize CsrBtAvSuspendCfmSizeof(void *msg)
{
    CsrBtAvSuspendCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_CFM
CsrUint8 *CsrBtAvSuspendCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvSuspendCfm *primitive;

    primitive = (CsrBtAvSuspendCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reject_shandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->avResultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_CFM */

#ifndef EXCLUDE_CSR_BT_AV_SUSPEND_CFM
void *CsrBtAvSuspendCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvSuspendCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvSuspendCfm *) CsrPmemZalloc(sizeof(CsrBtAvSuspendCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reject_shandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->avResultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_SUSPEND_CFM */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_RES
CsrSize CsrBtAvGetConfigurationResSizeof(void *msg)
{
    CsrBtAvGetConfigurationRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->servCapLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_RES
CsrUint8 *CsrBtAvGetConfigurationResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvGetConfigurationRes *primitive;

    primitive = (CsrBtAvGetConfigurationRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->servCapLen);
    CsrMemCpySer(buffer, length, &primitive->servCapData, sizeof(primitive->servCapData));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->avResponse);
    if(primitive->servCapData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->servCapData), ((CsrSize) (primitive->servCapLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_RES
void *CsrBtAvGetConfigurationResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvGetConfigurationRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvGetConfigurationRes *) CsrPmemZalloc(sizeof(CsrBtAvGetConfigurationRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->servCapLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->servCapData), buffer, &offset, ((CsrSize) sizeof(primitive->servCapData)));
    CsrUint8Des((CsrUint8 *) &primitive->avResponse, buffer, &offset);
    if (primitive->servCapLen)
    {
        primitive->servCapData = CsrPmemZalloc(((CsrUint32) (primitive->servCapLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->servCapData), buffer, &offset, ((CsrSize) (primitive->servCapLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->servCapData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_RES
void CsrBtAvGetConfigurationResSerFree(void *msg)
{
    CsrBtAvGetConfigurationRes *primitive;

    primitive = (CsrBtAvGetConfigurationRes *) msg;

    if(primitive->servCapData != NULL)
    {
        CsrPmemFree(primitive->servCapData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AV_GET_CONFIGURATION_RES */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_RES
CsrSize CsrBtAvReconfigureResSizeof(void *msg)
{
    CsrBtAvReconfigureRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_RES */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_RES
CsrUint8 *CsrBtAvReconfigureResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvReconfigureRes *primitive;

    primitive = (CsrBtAvReconfigureRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tLabel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->servCategory);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->avResponse);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_RES */

#ifndef EXCLUDE_CSR_BT_AV_RECONFIGURE_RES
void *CsrBtAvReconfigureResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvReconfigureRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvReconfigureRes *) CsrPmemZalloc(sizeof(CsrBtAvReconfigureRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tLabel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->servCategory, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->avResponse, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_RECONFIGURE_RES */

#ifndef EXCLUDE_CSR_BT_AV_GET_STREAM_CHANNEL_INFO_REQ
CsrSize CsrBtAvGetStreamChannelInfoReqSizeof(void *msg)
{
    CsrBtAvGetStreamChannelInfoReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_GET_STREAM_CHANNEL_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_AV_GET_STREAM_CHANNEL_INFO_REQ
CsrUint8 *CsrBtAvGetStreamChannelInfoReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvGetStreamChannelInfoReq *primitive;

    primitive = (CsrBtAvGetStreamChannelInfoReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->shandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_STREAM_CHANNEL_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_AV_GET_STREAM_CHANNEL_INFO_REQ
void *CsrBtAvGetStreamChannelInfoReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvGetStreamChannelInfoReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvGetStreamChannelInfoReq *) CsrPmemZalloc(sizeof(CsrBtAvGetStreamChannelInfoReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->shandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_GET_STREAM_CHANNEL_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_AV_CANCEL_CONNECT_REQ
CsrSize CsrBtAvCancelConnectReqSizeof(void *msg)
{
    CsrBtAvCancelConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AV_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_CANCEL_CONNECT_REQ
CsrUint8 *CsrBtAvCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvCancelConnectReq *primitive;

    primitive = (CsrBtAvCancelConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AV_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AV_CANCEL_CONNECT_REQ
void *CsrBtAvCancelConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvCancelConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvCancelConnectReq *) CsrPmemZalloc(sizeof(CsrBtAvCancelConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AV_CANCEL_CONNECT_REQ */

#endif /* EXCLUDE_CSR_BT_AV_MODULE */
