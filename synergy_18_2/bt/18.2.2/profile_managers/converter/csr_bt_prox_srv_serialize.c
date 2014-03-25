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
#include "csr_bt_prox_srv_prim.h"

#ifndef EXCLUDE_CSR_BT_PROX_SRV_MODULE

void CsrBtProx_srvPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_CFM
CsrSize CsrBtProxSrvUpdateTxPowerCfmSizeof(void *msg)
{
    CsrBtProxSrvUpdateTxPowerCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_CFM
CsrUint8 *CsrBtProxSrvUpdateTxPowerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtProxSrvUpdateTxPowerCfm *primitive;

    primitive = (CsrBtProxSrvUpdateTxPowerCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_CFM
void *CsrBtProxSrvUpdateTxPowerCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtProxSrvUpdateTxPowerCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtProxSrvUpdateTxPowerCfm *) CsrPmemZalloc(sizeof(CsrBtProxSrvUpdateTxPowerCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_REQ
CsrSize CsrBtProxSrvDeactivateReqSizeof(void *msg)
{
    CsrBtProxSrvDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_REQ
CsrUint8 *CsrBtProxSrvDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtProxSrvDeactivateReq *primitive;

    primitive = (CsrBtProxSrvDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_REQ
void *CsrBtProxSrvDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtProxSrvDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtProxSrvDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtProxSrvDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_DISCONNECT_IND
CsrSize CsrBtProxSrvDisconnectIndSizeof(void *msg)
{
    CsrBtProxSrvDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_DISCONNECT_IND
CsrUint8 *CsrBtProxSrvDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtProxSrvDisconnectInd *primitive;

    primitive = (CsrBtProxSrvDisconnectInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_PROX_SRV_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_DISCONNECT_IND
void *CsrBtProxSrvDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtProxSrvDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtProxSrvDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtProxSrvDisconnectInd));

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
#endif /* EXCLUDE_CSR_BT_PROX_SRV_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_ALERT_EVENT_IND
CsrSize CsrBtProxSrvAlertEventIndSizeof(void *msg)
{
    CsrBtProxSrvAlertEventInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ALERT_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_ALERT_EVENT_IND
CsrUint8 *CsrBtProxSrvAlertEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtProxSrvAlertEventInd *primitive;

    primitive = (CsrBtProxSrvAlertEventInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->alertType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->alertLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ALERT_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_ALERT_EVENT_IND
void *CsrBtProxSrvAlertEventIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtProxSrvAlertEventInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtProxSrvAlertEventInd *) CsrPmemZalloc(sizeof(CsrBtProxSrvAlertEventInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->alertType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->alertLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ALERT_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_CFM
CsrSize CsrBtProxSrvActivateCfmSizeof(void *msg)
{
    CsrBtProxSrvActivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_CFM
CsrUint8 *CsrBtProxSrvActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtProxSrvActivateCfm *primitive;

    primitive = (CsrBtProxSrvActivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dbStartHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dbEndHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_CFM
void *CsrBtProxSrvActivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtProxSrvActivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtProxSrvActivateCfm *) CsrPmemZalloc(sizeof(CsrBtProxSrvActivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dbStartHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dbEndHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_WRITE_EVENT_IND
CsrSize CsrBtProxSrvWriteEventIndSizeof(void *msg)
{
    CsrBtProxSrvWriteEventInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->valueSize * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_WRITE_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_WRITE_EVENT_IND
CsrUint8 *CsrBtProxSrvWriteEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtProxSrvWriteEventInd *primitive;

    primitive = (CsrBtProxSrvWriteEventInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_PROX_SRV_WRITE_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_WRITE_EVENT_IND
void *CsrBtProxSrvWriteEventIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtProxSrvWriteEventInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtProxSrvWriteEventInd *) CsrPmemZalloc(sizeof(CsrBtProxSrvWriteEventInd));

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
#endif /* EXCLUDE_CSR_BT_PROX_SRV_WRITE_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_WRITE_EVENT_IND
void CsrBtProxSrvWriteEventIndSerFree(void *msg)
{
    CsrBtProxSrvWriteEventInd *primitive;

    primitive = (CsrBtProxSrvWriteEventInd *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_WRITE_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_CFM
CsrSize CsrBtProxSrvUpdateBattLevelCfmSizeof(void *msg)
{
    CsrBtProxSrvUpdateBattLevelCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_CFM
CsrUint8 *CsrBtProxSrvUpdateBattLevelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtProxSrvUpdateBattLevelCfm *primitive;

    primitive = (CsrBtProxSrvUpdateBattLevelCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_CFM
void *CsrBtProxSrvUpdateBattLevelCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtProxSrvUpdateBattLevelCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtProxSrvUpdateBattLevelCfm *) CsrPmemZalloc(sizeof(CsrBtProxSrvUpdateBattLevelCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_REQ
CsrSize CsrBtProxSrvActivateReqSizeof(void *msg)
{
    CsrBtProxSrvActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->clientConfigSize * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_REQ
CsrUint8 *CsrBtProxSrvActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtProxSrvActivateReq *primitive;

    primitive = (CsrBtProxSrvActivateReq *) msg;
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
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_REQ
void *CsrBtProxSrvActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtProxSrvActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtProxSrvActivateReq *) CsrPmemZalloc(sizeof(CsrBtProxSrvActivateReq));

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
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_REQ
void CsrBtProxSrvActivateReqSerFree(void *msg)
{
    CsrBtProxSrvActivateReq *primitive;

    primitive = (CsrBtProxSrvActivateReq *) msg;

    if(primitive->clientConfig != NULL)
    {
        CsrPmemFree(primitive->clientConfig);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_CFM
CsrSize CsrBtProxSrvDeactivateCfmSizeof(void *msg)
{
    CsrBtProxSrvDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->clientConfigSize * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_CFM
CsrUint8 *CsrBtProxSrvDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtProxSrvDeactivateCfm *primitive;

    primitive = (CsrBtProxSrvDeactivateCfm *) msg;
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
#endif /* EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_CFM
void *CsrBtProxSrvDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtProxSrvDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtProxSrvDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtProxSrvDeactivateCfm));

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
#endif /* EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_CFM
void CsrBtProxSrvDeactivateCfmSerFree(void *msg)
{
    CsrBtProxSrvDeactivateCfm *primitive;

    primitive = (CsrBtProxSrvDeactivateCfm *) msg;

    if(primitive->clientConfig != NULL)
    {
        CsrPmemFree(primitive->clientConfig);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_REQ
CsrSize CsrBtProxSrvUpdateTxPowerReqSizeof(void *msg)
{
    CsrBtProxSrvUpdateTxPowerReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_REQ */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_REQ
CsrUint8 *CsrBtProxSrvUpdateTxPowerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtProxSrvUpdateTxPowerReq *primitive;

    primitive = (CsrBtProxSrvUpdateTxPowerReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->txPower);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_REQ */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_REQ
void *CsrBtProxSrvUpdateTxPowerReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtProxSrvUpdateTxPowerReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtProxSrvUpdateTxPowerReq *) CsrPmemZalloc(sizeof(CsrBtProxSrvUpdateTxPowerReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->txPower, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_TX_POWER_REQ */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_CONNECT_IND
CsrSize CsrBtProxSrvConnectIndSizeof(void *msg)
{
    CsrBtProxSrvConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_CONNECT_IND
CsrUint8 *CsrBtProxSrvConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtProxSrvConnectInd *primitive;

    primitive = (CsrBtProxSrvConnectInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_PROX_SRV_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_CONNECT_IND
void *CsrBtProxSrvConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtProxSrvConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtProxSrvConnectInd *) CsrPmemZalloc(sizeof(CsrBtProxSrvConnectInd));

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
#endif /* EXCLUDE_CSR_BT_PROX_SRV_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_TX_POWER_CHANGED_EVENT_IND
CsrSize CsrBtProxSrvTxPowerChangedEventIndSizeof(void *msg)
{
    CsrBtProxSrvTxPowerChangedEventInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_TX_POWER_CHANGED_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_TX_POWER_CHANGED_EVENT_IND
CsrUint8 *CsrBtProxSrvTxPowerChangedEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtProxSrvTxPowerChangedEventInd *primitive;

    primitive = (CsrBtProxSrvTxPowerChangedEventInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->txPower);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_TX_POWER_CHANGED_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_TX_POWER_CHANGED_EVENT_IND
void *CsrBtProxSrvTxPowerChangedEventIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtProxSrvTxPowerChangedEventInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtProxSrvTxPowerChangedEventInd *) CsrPmemZalloc(sizeof(CsrBtProxSrvTxPowerChangedEventInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->txPower, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_TX_POWER_CHANGED_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_REQ
CsrSize CsrBtProxSrvUpdateBattLevelReqSizeof(void *msg)
{
    CsrBtProxSrvUpdateBattLevelReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_REQ
CsrUint8 *CsrBtProxSrvUpdateBattLevelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtProxSrvUpdateBattLevelReq *primitive;

    primitive = (CsrBtProxSrvUpdateBattLevelReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->battLevel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->battMask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serviceRequired);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_REQ
void *CsrBtProxSrvUpdateBattLevelReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtProxSrvUpdateBattLevelReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtProxSrvUpdateBattLevelReq *) CsrPmemZalloc(sizeof(CsrBtProxSrvUpdateBattLevelReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->battLevel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->battMask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serviceRequired, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_PROX_SRV_UPDATE_BATT_LEVEL_REQ */

#endif /* EXCLUDE_CSR_BT_PROX_SRV_MODULE */
