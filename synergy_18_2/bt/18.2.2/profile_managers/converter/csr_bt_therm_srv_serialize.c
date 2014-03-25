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
#include "csr_bt_therm_srv_prim.h"

#ifndef EXCLUDE_CSR_BT_THERM_SRV_MODULE

void CsrBtTherm_srvPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_THERM_SRV_CONNECT_IND
CsrSize CsrBtThermSrvConnectIndSizeof(void *msg)
{
    CsrBtThermSrvConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_CONNECT_IND
CsrUint8 *CsrBtThermSrvConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtThermSrvConnectInd *primitive;

    primitive = (CsrBtThermSrvConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_CONNECT_IND
void *CsrBtThermSrvConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtThermSrvConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtThermSrvConnectInd *) CsrPmemZalloc(sizeof(CsrBtThermSrvConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_CFM
CsrSize CsrBtThermSrvDeactivateCfmSizeof(void *msg)
{
    CsrBtThermSrvDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->clientConfigSize * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_CFM
CsrUint8 *CsrBtThermSrvDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtThermSrvDeactivateCfm *primitive;

    primitive = (CsrBtThermSrvDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->clientConfigSize);
    CsrMemCpySer(buffer, length, &primitive->clientConfig, sizeof(primitive->clientConfig));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    if(primitive->clientConfig)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->clientConfig), ((CsrSize) (primitive->clientConfigSize * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_CFM
void *CsrBtThermSrvDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtThermSrvDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtThermSrvDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtThermSrvDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->clientConfigSize, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->clientConfig), buffer, &offset, ((CsrSize) sizeof(primitive->clientConfig)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    if (primitive->clientConfigSize)
    {
        primitive->clientConfig = CsrPmemZalloc(((CsrUint32) (primitive->clientConfigSize * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->clientConfig), buffer, &offset, ((CsrSize) (primitive->clientConfigSize * sizeof(CsrUint8))));
    }
    else
    {
        primitive->clientConfig = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_CFM
void CsrBtThermSrvDeactivateCfmSerFree(void *msg)
{
    CsrBtThermSrvDeactivateCfm *primitive;

    primitive = (CsrBtThermSrvDeactivateCfm *) msg;

    if(primitive->clientConfig != NULL)
    {
        CsrPmemFree(primitive->clientConfig);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_CFM
CsrSize CsrBtThermSrvUpdateTemperatureCfmSizeof(void *msg)
{
    CsrBtThermSrvUpdateTemperatureCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_CFM
CsrUint8 *CsrBtThermSrvUpdateTemperatureCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtThermSrvUpdateTemperatureCfm *primitive;

    primitive = (CsrBtThermSrvUpdateTemperatureCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_CFM
void *CsrBtThermSrvUpdateTemperatureCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtThermSrvUpdateTemperatureCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtThermSrvUpdateTemperatureCfm *) CsrPmemZalloc(sizeof(CsrBtThermSrvUpdateTemperatureCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ
CsrSize CsrBtThermSrvUpdateTemperatureReqSizeof(void *msg)
{
    CsrBtThermSrvUpdateTemperatureReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->tempDataSize * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ
CsrUint8 *CsrBtThermSrvUpdateTemperatureReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtThermSrvUpdateTemperatureReq *primitive;

    primitive = (CsrBtThermSrvUpdateTemperatureReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->tempDataSize);
    CsrMemCpySer(buffer, length, &primitive->tempData, sizeof(primitive->tempData));
    if(primitive->tempData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->tempData), ((CsrSize) (primitive->tempDataSize * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ
void *CsrBtThermSrvUpdateTemperatureReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtThermSrvUpdateTemperatureReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtThermSrvUpdateTemperatureReq *) CsrPmemZalloc(sizeof(CsrBtThermSrvUpdateTemperatureReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->tempDataSize, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->tempData), buffer, &offset, ((CsrSize) sizeof(primitive->tempData)));
    if (primitive->tempDataSize)
    {
        primitive->tempData = CsrPmemZalloc(((CsrUint32) (primitive->tempDataSize * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->tempData), buffer, &offset, ((CsrSize) (primitive->tempDataSize * sizeof(CsrUint8))));
    }
    else
    {
        primitive->tempData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ
void CsrBtThermSrvUpdateTemperatureReqSerFree(void *msg)
{
    CsrBtThermSrvUpdateTemperatureReq *primitive;

    primitive = (CsrBtThermSrvUpdateTemperatureReq *) msg;

    if(primitive->tempData != NULL)
    {
        CsrPmemFree(primitive->tempData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_TEMPERATURE_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_CFM
CsrSize CsrBtThermSrvUpdateHealthInfoCfmSizeof(void *msg)
{
    CsrBtThermSrvUpdateHealthInfoCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_CFM
CsrUint8 *CsrBtThermSrvUpdateHealthInfoCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtThermSrvUpdateHealthInfoCfm *primitive;

    primitive = (CsrBtThermSrvUpdateHealthInfoCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_CFM
void *CsrBtThermSrvUpdateHealthInfoCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtThermSrvUpdateHealthInfoCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtThermSrvUpdateHealthInfoCfm *) CsrPmemZalloc(sizeof(CsrBtThermSrvUpdateHealthInfoCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_REQ
CsrSize CsrBtThermSrvDeactivateReqSizeof(void *msg)
{
    CsrBtThermSrvDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_REQ
CsrUint8 *CsrBtThermSrvDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtThermSrvDeactivateReq *primitive;

    primitive = (CsrBtThermSrvDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_REQ
void *CsrBtThermSrvDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtThermSrvDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtThermSrvDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtThermSrvDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ
CsrSize CsrBtThermSrvUpdateHealthInfoReqSizeof(void *msg)
{
    CsrBtThermSrvUpdateHealthInfoReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->info));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ
CsrUint8 *CsrBtThermSrvUpdateHealthInfoReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtThermSrvUpdateHealthInfoReq *primitive;

    primitive = (CsrBtThermSrvUpdateHealthInfoReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->infoType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->infoLength);
    CsrMemCpySer(buffer, length, &primitive->info, sizeof(primitive->info));
    CsrUtf8StringSer(buffer, length, primitive->info);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ
void *CsrBtThermSrvUpdateHealthInfoReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtThermSrvUpdateHealthInfoReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtThermSrvUpdateHealthInfoReq *) CsrPmemZalloc(sizeof(CsrBtThermSrvUpdateHealthInfoReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->infoType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->infoLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->info), buffer, &offset, ((CsrSize) sizeof(primitive->info)));
    CsrUtf8StringDes(&primitive->info, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ
void CsrBtThermSrvUpdateHealthInfoReqSerFree(void *msg)
{
    CsrBtThermSrvUpdateHealthInfoReq *primitive;

    primitive = (CsrBtThermSrvUpdateHealthInfoReq *) msg;

    if(primitive->info != NULL)
    {
        CsrPmemFree(primitive->info);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_HEALTH_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_REQ
CsrSize CsrBtThermSrvActivateReqSizeof(void *msg)
{
    CsrBtThermSrvActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->clientConfigSize * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_REQ
CsrUint8 *CsrBtThermSrvActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtThermSrvActivateReq *primitive;

    primitive = (CsrBtThermSrvActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->clientConfigSize);
    CsrMemCpySer(buffer, length, &primitive->clientConfig, sizeof(primitive->clientConfig));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->eventMask);
    if(primitive->clientConfig)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->clientConfig), ((CsrSize) (primitive->clientConfigSize * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_REQ
void *CsrBtThermSrvActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtThermSrvActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtThermSrvActivateReq *) CsrPmemZalloc(sizeof(CsrBtThermSrvActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->clientConfigSize, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->clientConfig), buffer, &offset, ((CsrSize) sizeof(primitive->clientConfig)));
    CsrUint32Des((CsrUint32 *) &primitive->eventMask, buffer, &offset);
    if (primitive->clientConfigSize)
    {
        primitive->clientConfig = CsrPmemZalloc(((CsrUint32) (primitive->clientConfigSize * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->clientConfig), buffer, &offset, ((CsrSize) (primitive->clientConfigSize * sizeof(CsrUint8))));
    }
    else
    {
        primitive->clientConfig = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_REQ
void CsrBtThermSrvActivateReqSerFree(void *msg)
{
    CsrBtThermSrvActivateReq *primitive;

    primitive = (CsrBtThermSrvActivateReq *) msg;

    if(primitive->clientConfig != NULL)
    {
        CsrPmemFree(primitive->clientConfig);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_DISCONNECT_IND
CsrSize CsrBtThermSrvDisconnectIndSizeof(void *msg)
{
    CsrBtThermSrvDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_DISCONNECT_IND
CsrUint8 *CsrBtThermSrvDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtThermSrvDisconnectInd *primitive;

    primitive = (CsrBtThermSrvDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_DISCONNECT_IND
void *CsrBtThermSrvDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtThermSrvDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtThermSrvDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtThermSrvDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_CFM
CsrSize CsrBtThermSrvUpdateBattLevelCfmSizeof(void *msg)
{
    CsrBtThermSrvUpdateBattLevelCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_CFM
CsrUint8 *CsrBtThermSrvUpdateBattLevelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtThermSrvUpdateBattLevelCfm *primitive;

    primitive = (CsrBtThermSrvUpdateBattLevelCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_CFM
void *CsrBtThermSrvUpdateBattLevelCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtThermSrvUpdateBattLevelCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtThermSrvUpdateBattLevelCfm *) CsrPmemZalloc(sizeof(CsrBtThermSrvUpdateBattLevelCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_WRITE_EVENT_IND
CsrSize CsrBtThermSrvWriteEventIndSizeof(void *msg)
{
    CsrBtThermSrvWriteEventInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->valueSize * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_WRITE_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_WRITE_EVENT_IND
CsrUint8 *CsrBtThermSrvWriteEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtThermSrvWriteEventInd *primitive;

    primitive = (CsrBtThermSrvWriteEventInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->valueHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->valueSize);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->valueSize * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_WRITE_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_WRITE_EVENT_IND
void *CsrBtThermSrvWriteEventIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtThermSrvWriteEventInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtThermSrvWriteEventInd *) CsrPmemZalloc(sizeof(CsrBtThermSrvWriteEventInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->valueHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->valueSize, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->valueSize)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->valueSize * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->valueSize * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_WRITE_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_WRITE_EVENT_IND
void CsrBtThermSrvWriteEventIndSerFree(void *msg)
{
    CsrBtThermSrvWriteEventInd *primitive;

    primitive = (CsrBtThermSrvWriteEventInd *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_WRITE_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_REQ
CsrSize CsrBtThermSrvUpdateBattLevelReqSizeof(void *msg)
{
    CsrBtThermSrvUpdateBattLevelReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_REQ
CsrUint8 *CsrBtThermSrvUpdateBattLevelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtThermSrvUpdateBattLevelReq *primitive;

    primitive = (CsrBtThermSrvUpdateBattLevelReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->battLevel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->battMask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serviceRequired);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_REQ
void *CsrBtThermSrvUpdateBattLevelReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtThermSrvUpdateBattLevelReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtThermSrvUpdateBattLevelReq *) CsrPmemZalloc(sizeof(CsrBtThermSrvUpdateBattLevelReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->battLevel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->battMask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serviceRequired, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_UPDATE_BATT_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_CFM
CsrSize CsrBtThermSrvActivateCfmSizeof(void *msg)
{
    CsrBtThermSrvActivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_CFM
CsrUint8 *CsrBtThermSrvActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtThermSrvActivateCfm *primitive;

    primitive = (CsrBtThermSrvActivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dbStartHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dbEndHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_CFM
void *CsrBtThermSrvActivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtThermSrvActivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtThermSrvActivateCfm *) CsrPmemZalloc(sizeof(CsrBtThermSrvActivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dbStartHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dbEndHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_THERM_SRV_ACTIVATE_CFM */

#endif /* EXCLUDE_CSR_BT_THERM_SRV_MODULE */
