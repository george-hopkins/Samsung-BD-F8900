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
#include "csr_bt_hidd_prim.h"

#ifndef EXCLUDE_CSR_BT_HIDD_MODULE

void CsrBtHiddPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_HIDD_UNPLUG_CFM
CsrSize CsrBtHiddUnplugCfmSizeof(void *msg)
{
    CsrBtHiddUnplugCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_UNPLUG_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_UNPLUG_CFM
CsrUint8 *CsrBtHiddUnplugCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddUnplugCfm *primitive;

    primitive = (CsrBtHiddUnplugCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_UNPLUG_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_UNPLUG_CFM
void *CsrBtHiddUnplugCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddUnplugCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddUnplugCfm *) CsrPmemZalloc(sizeof(CsrBtHiddUnplugCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_UNPLUG_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_STATUS_IND
CsrSize CsrBtHiddStatusIndSizeof(void *msg)
{
    CsrBtHiddStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_STATUS_IND
CsrUint8 *CsrBtHiddStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddStatusInd *primitive;

    primitive = (CsrBtHiddStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_STATUS_IND
void *CsrBtHiddStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddStatusInd *) CsrPmemZalloc(sizeof(CsrBtHiddStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_DATA_CFM
CsrSize CsrBtHiddDataCfmSizeof(void *msg)
{
    CsrBtHiddDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_DATA_CFM
CsrUint8 *CsrBtHiddDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddDataCfm *primitive;

    primitive = (CsrBtHiddDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_DATA_CFM
void *CsrBtHiddDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddDataCfm *) CsrPmemZalloc(sizeof(CsrBtHiddDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_IN_CFM
CsrSize CsrBtHiddSecurityInCfmSizeof(void *msg)
{
    CsrBtHiddSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_IN_CFM
CsrUint8 *CsrBtHiddSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddSecurityInCfm *primitive;

    primitive = (CsrBtHiddSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_IN_CFM
void *CsrBtHiddSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtHiddSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_DEACTIVATE_REQ
CsrSize CsrBtHiddDeactivateReqSizeof(void *msg)
{
    CsrBtHiddDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_DEACTIVATE_REQ
CsrUint8 *CsrBtHiddDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddDeactivateReq *primitive;

    primitive = (CsrBtHiddDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_DEACTIVATE_REQ
void *CsrBtHiddDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtHiddDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_IND
CsrSize CsrBtHiddModeChangeIndSizeof(void *msg)
{
    CsrBtHiddModeChangeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_IND
CsrUint8 *CsrBtHiddModeChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddModeChangeInd *primitive;

    primitive = (CsrBtHiddModeChangeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_IND
void *CsrBtHiddModeChangeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddModeChangeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddModeChangeInd *) CsrPmemZalloc(sizeof(CsrBtHiddModeChangeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_UNPLUG_IND
CsrSize CsrBtHiddUnplugIndSizeof(void *msg)
{
    CsrBtHiddUnplugInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_UNPLUG_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_UNPLUG_IND
CsrUint8 *CsrBtHiddUnplugIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddUnplugInd *primitive;

    primitive = (CsrBtHiddUnplugInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_UNPLUG_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_UNPLUG_IND
void *CsrBtHiddUnplugIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddUnplugInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddUnplugInd *) CsrPmemZalloc(sizeof(CsrBtHiddUnplugInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_UNPLUG_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_DATA_REQ
CsrSize CsrBtHiddDataReqSizeof(void *msg)
{
    CsrBtHiddDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->reportLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_DATA_REQ
CsrUint8 *CsrBtHiddDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddDataReq *primitive;

    primitive = (CsrBtHiddDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reportType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reportLen);
    CsrMemCpySer(buffer, length, &primitive->report, sizeof(primitive->report));
    if(primitive->report)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->report), ((CsrSize) (primitive->reportLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_DATA_REQ
void *CsrBtHiddDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddDataReq *) CsrPmemZalloc(sizeof(CsrBtHiddDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reportType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reportLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->report), buffer, &offset, ((CsrSize) sizeof(primitive->report)));
    if (primitive->reportLen)
    {
        primitive->report = CsrPmemZalloc(((CsrUint32) (primitive->reportLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->report), buffer, &offset, ((CsrSize) (primitive->reportLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->report = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_DATA_REQ
void CsrBtHiddDataReqSerFree(void *msg)
{
    CsrBtHiddDataReq *primitive;

    primitive = (CsrBtHiddDataReq *) msg;

    if(primitive->report != NULL)
    {
        CsrPmemFree(primitive->report);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_ACTIVATE_REQ
CsrSize CsrBtHiddActivateReqSizeof(void *msg)
{
    CsrBtHiddActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->qosCtrlCount * sizeof(L2CA_QOS_T)) + (primitive->qosIntrCount * sizeof(L2CA_QOS_T)) + (primitive->deviceIdSdpLen * sizeof(CsrUint8)) + (primitive->hidSdpLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_ACTIVATE_REQ
CsrUint8 *CsrBtHiddActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddActivateReq *primitive;

    primitive = (CsrBtHiddActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->qosCtrlCount);
    CsrMemCpySer(buffer, length, &primitive->qosCtrl, sizeof(primitive->qosCtrl));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->qosIntrCount);
    CsrMemCpySer(buffer, length, &primitive->qosIntr, sizeof(primitive->qosIntr));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flushTimeout);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceIdSdpLen);
    CsrMemCpySer(buffer, length, &primitive->deviceIdSdp, sizeof(primitive->deviceIdSdp));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->hidSdpLen);
    CsrMemCpySer(buffer, length, &primitive->hidSdp, sizeof(primitive->hidSdp));
    if(primitive->qosCtrl)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->qosCtrl), ((CsrSize) (primitive->qosCtrlCount * sizeof(L2CA_QOS_T))));
    }
    if(primitive->qosIntr)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->qosIntr), ((CsrSize) (primitive->qosIntrCount * sizeof(L2CA_QOS_T))));
    }
    if(primitive->deviceIdSdp)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->deviceIdSdp), ((CsrSize) (primitive->deviceIdSdpLen * sizeof(CsrUint8))));
    }
    if(primitive->hidSdp)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->hidSdp), ((CsrSize) (primitive->hidSdpLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_ACTIVATE_REQ
void *CsrBtHiddActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddActivateReq *) CsrPmemZalloc(sizeof(CsrBtHiddActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->qosCtrlCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->qosCtrl), buffer, &offset, ((CsrSize) sizeof(primitive->qosCtrl)));
    CsrUint8Des((CsrUint8 *) &primitive->qosIntrCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->qosIntr), buffer, &offset, ((CsrSize) sizeof(primitive->qosIntr)));
    CsrUint16Des((CsrUint16 *) &primitive->flushTimeout, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceIdSdpLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->deviceIdSdp), buffer, &offset, ((CsrSize) sizeof(primitive->deviceIdSdp)));
    CsrUint16Des((CsrUint16 *) &primitive->hidSdpLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->hidSdp), buffer, &offset, ((CsrSize) sizeof(primitive->hidSdp)));
    if (primitive->qosCtrlCount)
    {
        primitive->qosCtrl = CsrPmemZalloc(((CsrUint32) (primitive->qosCtrlCount * sizeof(L2CA_QOS_T))));
        CsrMemCpyDes(((void *) primitive->qosCtrl), buffer, &offset, ((CsrSize) (primitive->qosCtrlCount * sizeof(L2CA_QOS_T))));
    }
    else
    {
        primitive->qosCtrl = NULL;
    }
    if (primitive->qosIntrCount)
    {
        primitive->qosIntr = CsrPmemZalloc(((CsrUint32) (primitive->qosIntrCount * sizeof(L2CA_QOS_T))));
        CsrMemCpyDes(((void *) primitive->qosIntr), buffer, &offset, ((CsrSize) (primitive->qosIntrCount * sizeof(L2CA_QOS_T))));
    }
    else
    {
        primitive->qosIntr = NULL;
    }
    if (primitive->deviceIdSdpLen)
    {
        primitive->deviceIdSdp = CsrPmemZalloc(((CsrUint32) (primitive->deviceIdSdpLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->deviceIdSdp), buffer, &offset, ((CsrSize) (primitive->deviceIdSdpLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->deviceIdSdp = NULL;
    }
    if (primitive->hidSdpLen)
    {
        primitive->hidSdp = CsrPmemZalloc(((CsrUint32) (primitive->hidSdpLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->hidSdp), buffer, &offset, ((CsrSize) (primitive->hidSdpLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->hidSdp = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_ACTIVATE_REQ
void CsrBtHiddActivateReqSerFree(void *msg)
{
    CsrBtHiddActivateReq *primitive;

    primitive = (CsrBtHiddActivateReq *) msg;

    if(primitive->qosCtrl != NULL)
    {
        CsrPmemFree(primitive->qosCtrl);
    }
    if(primitive->qosIntr != NULL)
    {
        CsrPmemFree(primitive->qosIntr);
    }
    if(primitive->deviceIdSdp != NULL)
    {
        CsrPmemFree(primitive->deviceIdSdp);
    }
    if(primitive->hidSdp != NULL)
    {
        CsrPmemFree(primitive->hidSdp);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HIDD_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_REQ
CsrSize CsrBtHiddSecurityOutReqSizeof(void *msg)
{
    CsrBtHiddSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_REQ
CsrUint8 *CsrBtHiddSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddSecurityOutReq *primitive;

    primitive = (CsrBtHiddSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_REQ
void *CsrBtHiddSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtHiddSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_UNPLUG_REQ
CsrSize CsrBtHiddUnplugReqSizeof(void *msg)
{
    CsrBtHiddUnplugReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_UNPLUG_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_UNPLUG_REQ
CsrUint8 *CsrBtHiddUnplugReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddUnplugReq *primitive;

    primitive = (CsrBtHiddUnplugReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_UNPLUG_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_UNPLUG_REQ
void *CsrBtHiddUnplugReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddUnplugReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddUnplugReq *) CsrPmemZalloc(sizeof(CsrBtHiddUnplugReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_UNPLUG_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_CFM
CsrSize CsrBtHiddSecurityOutCfmSizeof(void *msg)
{
    CsrBtHiddSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_CFM
CsrUint8 *CsrBtHiddSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddSecurityOutCfm *primitive;

    primitive = (CsrBtHiddSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_CFM
void *CsrBtHiddSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtHiddSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_IN_REQ
CsrSize CsrBtHiddSecurityInReqSizeof(void *msg)
{
    CsrBtHiddSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_IN_REQ
CsrUint8 *CsrBtHiddSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddSecurityInReq *primitive;

    primitive = (CsrBtHiddSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_SECURITY_IN_REQ
void *CsrBtHiddSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtHiddSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_RESTORE_IND
CsrSize CsrBtHiddRestoreIndSizeof(void *msg)
{
    CsrBtHiddRestoreInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_RESTORE_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_RESTORE_IND
CsrUint8 *CsrBtHiddRestoreIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddRestoreInd *primitive;

    primitive = (CsrBtHiddRestoreInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_RESTORE_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_RESTORE_IND
void *CsrBtHiddRestoreIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddRestoreInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddRestoreInd *) CsrPmemZalloc(sizeof(CsrBtHiddRestoreInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_RESTORE_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_REQ
CsrSize CsrBtHiddModeChangeReqSizeof(void *msg)
{
    CsrBtHiddModeChangeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_REQ
CsrUint8 *CsrBtHiddModeChangeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddModeChangeReq *primitive;

    primitive = (CsrBtHiddModeChangeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_REQ
void *CsrBtHiddModeChangeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddModeChangeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddModeChangeReq *) CsrPmemZalloc(sizeof(CsrBtHiddModeChangeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_HIDD_DEACTIVATE_CFM
CsrSize CsrBtHiddDeactivateCfmSizeof(void *msg)
{
    CsrBtHiddDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_DEACTIVATE_CFM
CsrUint8 *CsrBtHiddDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddDeactivateCfm *primitive;

    primitive = (CsrBtHiddDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_DEACTIVATE_CFM
void *CsrBtHiddDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtHiddDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_DATA_IND
CsrSize CsrBtHiddDataIndSizeof(void *msg)
{
    CsrBtHiddDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->reportLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_DATA_IND
CsrUint8 *CsrBtHiddDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddDataInd *primitive;

    primitive = (CsrBtHiddDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reportType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reportLen);
    CsrMemCpySer(buffer, length, &primitive->report, sizeof(primitive->report));
    if(primitive->report)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->report), ((CsrSize) (primitive->reportLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_DATA_IND
void *CsrBtHiddDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddDataInd *) CsrPmemZalloc(sizeof(CsrBtHiddDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reportType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reportLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->report), buffer, &offset, ((CsrSize) sizeof(primitive->report)));
    if (primitive->reportLen)
    {
        primitive->report = CsrPmemZalloc(((CsrUint32) (primitive->reportLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->report), buffer, &offset, ((CsrSize) (primitive->reportLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->report = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_DATA_IND
void CsrBtHiddDataIndSerFree(void *msg)
{
    CsrBtHiddDataInd *primitive;

    primitive = (CsrBtHiddDataInd *) msg;

    if(primitive->report != NULL)
    {
        CsrPmemFree(primitive->report);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HIDD_DATA_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_CONTROL_IND
CsrSize CsrBtHiddControlIndSizeof(void *msg)
{
    CsrBtHiddControlInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_CONTROL_IND
CsrUint8 *CsrBtHiddControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddControlInd *primitive;

    primitive = (CsrBtHiddControlInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->transactionType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameter);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLen);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_CONTROL_IND
void *CsrBtHiddControlIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddControlInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddControlInd *) CsrPmemZalloc(sizeof(CsrBtHiddControlInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->transactionType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameter, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    if (primitive->dataLen)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->dataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->dataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_CONTROL_IND
void CsrBtHiddControlIndSerFree(void *msg)
{
    CsrBtHiddControlInd *primitive;

    primitive = (CsrBtHiddControlInd *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HIDD_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_ACTIVATE_CFM
CsrSize CsrBtHiddActivateCfmSizeof(void *msg)
{
    CsrBtHiddActivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_ACTIVATE_CFM
CsrUint8 *CsrBtHiddActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddActivateCfm *primitive;

    primitive = (CsrBtHiddActivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_ACTIVATE_CFM
void *CsrBtHiddActivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddActivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddActivateCfm *) CsrPmemZalloc(sizeof(CsrBtHiddActivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HIDD_REACTIVATE_IND
CsrSize CsrBtHiddReactivateIndSizeof(void *msg)
{
    CsrBtHiddReactivateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_REACTIVATE_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_REACTIVATE_IND
CsrUint8 *CsrBtHiddReactivateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddReactivateInd *primitive;

    primitive = (CsrBtHiddReactivateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_REACTIVATE_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_REACTIVATE_IND
void *CsrBtHiddReactivateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddReactivateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddReactivateInd *) CsrPmemZalloc(sizeof(CsrBtHiddReactivateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_REACTIVATE_IND */

#ifndef EXCLUDE_CSR_BT_HIDD_CONTROL_RES
CsrSize CsrBtHiddControlResSizeof(void *msg)
{
    CsrBtHiddControlRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HIDD_CONTROL_RES */

#ifndef EXCLUDE_CSR_BT_HIDD_CONTROL_RES
CsrUint8 *CsrBtHiddControlResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHiddControlRes *primitive;

    primitive = (CsrBtHiddControlRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->transactionType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parameter);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLen);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HIDD_CONTROL_RES */

#ifndef EXCLUDE_CSR_BT_HIDD_CONTROL_RES
void *CsrBtHiddControlResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHiddControlRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHiddControlRes *) CsrPmemZalloc(sizeof(CsrBtHiddControlRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->transactionType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->parameter, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    if (primitive->dataLen)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->dataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->dataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HIDD_CONTROL_RES */

#ifndef EXCLUDE_CSR_BT_HIDD_CONTROL_RES
void CsrBtHiddControlResSerFree(void *msg)
{
    CsrBtHiddControlRes *primitive;

    primitive = (CsrBtHiddControlRes *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HIDD_CONTROL_RES */

#endif /* EXCLUDE_CSR_BT_HIDD_MODULE */
