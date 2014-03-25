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
#include "csr_bt_gatt_private_prim.h"

#ifndef EXCLUDE_CSR_BT_GATT_PRIVATE_MODULE

void CsrBtGattPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_GATT_READ_SERVER_CONFIGURATION_CFM
CsrSize CsrBtGattReadServerConfigurationCfmSizeof(void *msg)
{
    CsrBtGattReadServerConfigurationCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_SERVER_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_SERVER_CONFIGURATION_CFM
CsrUint8 *CsrBtGattReadServerConfigurationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattReadServerConfigurationCfm *primitive;

    primitive = (CsrBtGattReadServerConfigurationCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->configuration);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_SERVER_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_SERVER_CONFIGURATION_CFM
void *CsrBtGattReadServerConfigurationCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattReadServerConfigurationCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattReadServerConfigurationCfm *) CsrPmemZalloc(sizeof(CsrBtGattReadServerConfigurationCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->configuration, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_SERVER_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_REQ
CsrSize CsrBtGattDiscoverCharacReqSizeof(void *msg)
{
    CsrBtGattDiscoverCharacReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_REQ
CsrUint8 *CsrBtGattDiscoverCharacReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDiscoverCharacReq *primitive;

    primitive = (CsrBtGattDiscoverCharacReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->uuid.length);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uuid.uuid), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->startHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->endGroupHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_REQ
void *CsrBtGattDiscoverCharacReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDiscoverCharacReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDiscoverCharacReq *) CsrPmemZalloc(sizeof(CsrBtGattDiscoverCharacReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->uuid.length, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uuid.uuid), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->startHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->endGroupHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_REQ
CsrSize CsrBtGattParamConnectionReqSizeof(void *msg)
{
    CsrBtGattParamConnectionReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_REQ
CsrUint8 *CsrBtGattParamConnectionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattParamConnectionReq *primitive;

    primitive = (CsrBtGattParamConnectionReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scanInterval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scanWindow);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connIntervalMin);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connIntervalMax);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connLatency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supervisionTimeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connAttemptTimeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->advIntervalMin);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->advIntervalMax);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connLatencyMax);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supervisionTimeoutMin);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supervisionTimeoutMax);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_REQ
void *CsrBtGattParamConnectionReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattParamConnectionReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattParamConnectionReq *) CsrPmemZalloc(sizeof(CsrBtGattParamConnectionReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scanInterval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scanWindow, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connIntervalMin, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connIntervalMax, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connLatency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supervisionTimeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connAttemptTimeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->advIntervalMin, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->advIntervalMax, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connLatencyMax, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supervisionTimeoutMin, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supervisionTimeoutMax, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_CFM
CsrSize CsrBtGattWhitelistClearCfmSizeof(void *msg)
{
    CsrBtGattWhitelistClearCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_CFM
CsrUint8 *CsrBtGattWhitelistClearCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattWhitelistClearCfm *primitive;

    primitive = (CsrBtGattWhitelistClearCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_CFM
void *CsrBtGattWhitelistClearCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattWhitelistClearCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattWhitelistClearCfm *) CsrPmemZalloc(sizeof(CsrBtGattWhitelistClearCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_SERVICE_CHANGED_IND
CsrSize CsrBtGattServiceChangedIndSizeof(void *msg)
{
    CsrBtGattServiceChangedInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_SERVICE_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_GATT_SERVICE_CHANGED_IND
CsrUint8 *CsrBtGattServiceChangedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattServiceChangedInd *primitive;

    primitive = (CsrBtGattServiceChangedInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->startHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->endHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connInfo);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_SERVICE_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_GATT_SERVICE_CHANGED_IND
void *CsrBtGattServiceChangedIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattServiceChangedInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattServiceChangedInd *) CsrPmemZalloc(sizeof(CsrBtGattServiceChangedInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->startHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->endHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connInfo, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_SERVICE_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_GATT_SECURITY_CFM
CsrSize CsrBtGattSecurityCfmSizeof(void *msg)
{
    CsrBtGattSecurityCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_SECURITY_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_SECURITY_CFM
CsrUint8 *CsrBtGattSecurityCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattSecurityCfm *primitive;

    primitive = (CsrBtGattSecurityCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_SECURITY_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_SECURITY_CFM
void *CsrBtGattSecurityCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattSecurityCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattSecurityCfm *) CsrPmemZalloc(sizeof(CsrBtGattSecurityCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_SECURITY_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_READ_IND
CsrSize CsrBtGattDbAccessReadIndSizeof(void *msg)
{
    CsrBtGattDbAccessReadInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_READ_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_READ_IND
CsrUint8 *CsrBtGattDbAccessReadIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDbAccessReadInd *primitive;

    primitive = (CsrBtGattDbAccessReadInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attrHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->offset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxRspValueLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->check);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connInfo);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_READ_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_READ_IND
void *CsrBtGattDbAccessReadIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDbAccessReadInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDbAccessReadInd *) CsrPmemZalloc(sizeof(CsrBtGattDbAccessReadInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attrHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->offset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxRspValueLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->check, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connInfo, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_READ_IND */

#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_IND
CsrSize CsrBtGattReadByUuidIndSizeof(void *msg)
{
    CsrBtGattReadByUuidInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->valueLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_IND */

#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_IND
CsrUint8 *CsrBtGattReadByUuidIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattReadByUuidInd *primitive;

    primitive = (CsrBtGattReadByUuidInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->valueHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->valueLength);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->valueLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_IND */

#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_IND
void *CsrBtGattReadByUuidIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattReadByUuidInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattReadByUuidInd *) CsrPmemZalloc(sizeof(CsrBtGattReadByUuidInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->valueHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->valueLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->valueLength)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->valueLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->valueLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_IND */

#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_IND
void CsrBtGattReadByUuidIndSerFree(void *msg)
{
    CsrBtGattReadByUuidInd *primitive;

    primitive = (CsrBtGattReadByUuidInd *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_IND */

#ifndef EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_CFM
CsrSize CsrBtGattBredrAcceptCfmSizeof(void *msg)
{
    CsrBtGattBredrAcceptCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_CFM
CsrUint8 *CsrBtGattBredrAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattBredrAcceptCfm *primitive;

    primitive = (CsrBtGattBredrAcceptCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_CFM
void *CsrBtGattBredrAcceptCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattBredrAcceptCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattBredrAcceptCfm *) CsrPmemZalloc(sizeof(CsrBtGattBredrAcceptCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_WRITE_CFM
CsrSize CsrBtGattWriteCfmSizeof(void *msg)
{
    CsrBtGattWriteCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_WRITE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_WRITE_CFM
CsrUint8 *CsrBtGattWriteCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattWriteCfm *primitive;

    primitive = (CsrBtGattWriteCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_WRITE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_WRITE_CFM
void *CsrBtGattWriteCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattWriteCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattWriteCfm *) CsrPmemZalloc(sizeof(CsrBtGattWriteCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_WRITE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_CHANGED_IND
CsrSize CsrBtGattParamConnChangedIndSizeof(void *msg)
{
    CsrBtGattParamConnChangedInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_CHANGED_IND
CsrUint8 *CsrBtGattParamConnChangedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattParamConnChangedInd *primitive;

    primitive = (CsrBtGattParamConnChangedInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->latency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeout);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_CHANGED_IND
void *CsrBtGattParamConnChangedIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattParamConnChangedInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattParamConnChangedInd *) CsrPmemZalloc(sizeof(CsrBtGattParamConnChangedInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->latency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_REQ
CsrSize CsrBtGattBredrAcceptReqSizeof(void *msg)
{
    CsrBtGattBredrAcceptReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_REQ
CsrUint8 *CsrBtGattBredrAcceptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattBredrAcceptReq *primitive;

    primitive = (CsrBtGattBredrAcceptReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_REQ
void *CsrBtGattBredrAcceptReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattBredrAcceptReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattBredrAcceptReq *) CsrPmemZalloc(sizeof(CsrBtGattBredrAcceptReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DB_REMOVE_CFM
CsrSize CsrBtGattDbRemoveCfmSizeof(void *msg)
{
    CsrBtGattDbRemoveCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_REMOVE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_REMOVE_CFM
CsrUint8 *CsrBtGattDbRemoveCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDbRemoveCfm *primitive;

    primitive = (CsrBtGattDbRemoveCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->numOfAttr);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_REMOVE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_REMOVE_CFM
void *CsrBtGattDbRemoveCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDbRemoveCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDbRemoveCfm *) CsrPmemZalloc(sizeof(CsrBtGattDbRemoveCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->numOfAttr, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_REMOVE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PERIPHERAL_CFM
CsrSize CsrBtGattPeripheralCfmSizeof(void *msg)
{
    CsrBtGattPeripheralCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_PERIPHERAL_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PERIPHERAL_CFM
CsrUint8 *CsrBtGattPeripheralCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattPeripheralCfm *primitive;

    primitive = (CsrBtGattPeripheralCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_PERIPHERAL_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PERIPHERAL_CFM
void *CsrBtGattPeripheralCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattPeripheralCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattPeripheralCfm *) CsrPmemZalloc(sizeof(CsrBtGattPeripheralCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_PERIPHERAL_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DISCONNECT_REQ
CsrSize CsrBtGattDisconnectReqSizeof(void *msg)
{
    CsrBtGattDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DISCONNECT_REQ
CsrUint8 *CsrBtGattDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDisconnectReq *primitive;

    primitive = (CsrBtGattDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DISCONNECT_REQ
void *CsrBtGattDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtGattDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_RES
CsrSize CsrBtGattDbAccessResSizeof(void *msg)
{
    CsrBtGattDbAccessRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->valueLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_RES */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_RES
CsrUint8 *CsrBtGattDbAccessResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDbAccessRes *primitive;

    primitive = (CsrBtGattDbAccessRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attrHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->responseCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->valueLength);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->valueLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_RES */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_RES
void *CsrBtGattDbAccessResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDbAccessRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDbAccessRes *) CsrPmemZalloc(sizeof(CsrBtGattDbAccessRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attrHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->responseCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->valueLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->valueLength)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->valueLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->valueLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_RES */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_RES
void CsrBtGattDbAccessResSerFree(void *msg)
{
    CsrBtGattDbAccessRes *primitive;

    primitive = (CsrBtGattDbAccessRes *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_RES */

#ifndef EXCLUDE_CSR_BT_GATT_READ_PRESENTATION_FORMAT_CFM
CsrSize CsrBtGattReadPresentationFormatCfmSizeof(void *msg)
{
    CsrBtGattReadPresentationFormatCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_PRESENTATION_FORMAT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_PRESENTATION_FORMAT_CFM
CsrUint8 *CsrBtGattReadPresentationFormatCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattReadPresentationFormatCfm *primitive;

    primitive = (CsrBtGattReadPresentationFormatCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->format);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->exponent);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->unit);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->nameSpace);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->description);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_PRESENTATION_FORMAT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_PRESENTATION_FORMAT_CFM
void *CsrBtGattReadPresentationFormatCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattReadPresentationFormatCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattReadPresentationFormatCfm *) CsrPmemZalloc(sizeof(CsrBtGattReadPresentationFormatCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->format, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->exponent, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->unit, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->nameSpace, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->description, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_PRESENTATION_FORMAT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_EVENT_SEND_REQ
CsrSize CsrBtGattEventSendReqSizeof(void *msg)
{
    CsrBtGattEventSendReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->valueLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_EVENT_SEND_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_EVENT_SEND_REQ
CsrUint8 *CsrBtGattEventSendReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattEventSendReq *primitive;

    primitive = (CsrBtGattEventSendReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attrHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->endGroupHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->valueLength);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->valueLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_EVENT_SEND_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_EVENT_SEND_REQ
void *CsrBtGattEventSendReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattEventSendReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattEventSendReq *) CsrPmemZalloc(sizeof(CsrBtGattEventSendReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attrHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->endGroupHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->valueLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->valueLength)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->valueLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->valueLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_EVENT_SEND_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_EVENT_SEND_REQ
void CsrBtGattEventSendReqSerFree(void *msg)
{
    CsrBtGattEventSendReq *primitive;

    primitive = (CsrBtGattEventSendReq *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_GATT_EVENT_SEND_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_READ_EXTENDED_PROPERTIES_CFM
CsrSize CsrBtGattReadExtendedPropertiesCfmSizeof(void *msg)
{
    CsrBtGattReadExtendedPropertiesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_EXTENDED_PROPERTIES_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_EXTENDED_PROPERTIES_CFM
CsrUint8 *CsrBtGattReadExtendedPropertiesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattReadExtendedPropertiesCfm *primitive;

    primitive = (CsrBtGattReadExtendedPropertiesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->extProperties);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_EXTENDED_PROPERTIES_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_EXTENDED_PROPERTIES_CFM
void *CsrBtGattReadExtendedPropertiesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattReadExtendedPropertiesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattReadExtendedPropertiesCfm *) CsrPmemZalloc(sizeof(CsrBtGattReadExtendedPropertiesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->extProperties, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_EXTENDED_PROPERTIES_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_ADVERTISE_CFM
CsrSize CsrBtGattAdvertiseCfmSizeof(void *msg)
{
    CsrBtGattAdvertiseCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_ADVERTISE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_ADVERTISE_CFM
CsrUint8 *CsrBtGattAdvertiseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattAdvertiseCfm *primitive;

    primitive = (CsrBtGattAdvertiseCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_ADVERTISE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_ADVERTISE_CFM
void *CsrBtGattAdvertiseCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattAdvertiseCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattAdvertiseCfm *) CsrPmemZalloc(sizeof(CsrBtGattAdvertiseCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_ADVERTISE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_SCAN_REQ
CsrSize CsrBtGattParamScanReqSizeof(void *msg)
{
    CsrBtGattParamScanReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_SCAN_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_SCAN_REQ
CsrUint8 *CsrBtGattParamScanReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattParamScanReq *primitive;

    primitive = (CsrBtGattParamScanReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scanInterval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scanWindow);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_SCAN_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_SCAN_REQ
void *CsrBtGattParamScanReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattParamScanReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattParamScanReq *) CsrPmemZalloc(sizeof(CsrBtGattParamScanReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scanInterval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scanWindow, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_SCAN_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_IND
CsrSize CsrBtGattDiscoverCharacIndSizeof(void *msg)
{
    CsrBtGattDiscoverCharacInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_IND
CsrUint8 *CsrBtGattDiscoverCharacIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDiscoverCharacInd *primitive;

    primitive = (CsrBtGattDiscoverCharacInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->declarationHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->property);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->uuid.length);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uuid.uuid), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->valueHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_IND
void *CsrBtGattDiscoverCharacIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDiscoverCharacInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDiscoverCharacInd *) CsrPmemZalloc(sizeof(CsrBtGattDiscoverCharacInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->declarationHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->property, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->uuid.length, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uuid.uuid), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->valueHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_IND */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_CHANGE_IND
CsrSize CsrBtGattWhitelistChangeIndSizeof(void *msg)
{
    CsrBtGattWhitelistChangeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_CHANGE_IND
CsrUint8 *CsrBtGattWhitelistChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattWhitelistChangeInd *primitive;

    primitive = (CsrBtGattWhitelistChangeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_CHANGE_IND
void *CsrBtGattWhitelistChangeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattWhitelistChangeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattWhitelistChangeInd *) CsrPmemZalloc(sizeof(CsrBtGattWhitelistChangeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_REQ
CsrSize CsrBtGattReadByUuidReqSizeof(void *msg)
{
    CsrBtGattReadByUuidReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_REQ
CsrUint8 *CsrBtGattReadByUuidReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattReadByUuidReq *primitive;

    primitive = (CsrBtGattReadByUuidReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->startHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->endGroupHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->uuid.length);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uuid.uuid), ((CsrSize) (16)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_REQ
void *CsrBtGattReadByUuidReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattReadByUuidReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattReadByUuidReq *) CsrPmemZalloc(sizeof(CsrBtGattReadByUuidReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->startHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->endGroupHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->uuid.length, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uuid.uuid), buffer, &offset, ((CsrSize) (16)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_REQ
CsrSize CsrBtGattWhitelistAddReqSizeof(void *msg)
{
    CsrBtGattWhitelistAddReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->addressCount * sizeof(CsrBtTypedAddr)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_REQ
CsrUint8 *CsrBtGattWhitelistAddReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattWhitelistAddReq *primitive;

    primitive = (CsrBtGattWhitelistAddReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addressCount);
    CsrMemCpySer(buffer, length, &primitive->address, sizeof(primitive->address));
    if(primitive->address)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->address), ((CsrSize) (primitive->addressCount * sizeof(CsrBtTypedAddr))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_REQ
void *CsrBtGattWhitelistAddReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattWhitelistAddReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattWhitelistAddReq *) CsrPmemZalloc(sizeof(CsrBtGattWhitelistAddReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addressCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->address), buffer, &offset, ((CsrSize) sizeof(primitive->address)));
    if (primitive->addressCount)
    {
        primitive->address = CsrPmemZalloc(((CsrUint32) (primitive->addressCount * sizeof(CsrBtTypedAddr))));
        CsrMemCpyDes(((void *) primitive->address), buffer, &offset, ((CsrSize) (primitive->addressCount * sizeof(CsrBtTypedAddr))));
    }
    else
    {
        primitive->address = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_REQ
void CsrBtGattWhitelistAddReqSerFree(void *msg)
{
    CsrBtGattWhitelistAddReq *primitive;

    primitive = (CsrBtGattWhitelistAddReq *) msg;

    if(primitive->address != NULL)
    {
        CsrPmemFree(primitive->address);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_RES
CsrSize CsrBtGattParamConnUpdateResSizeof(void *msg)
{
    CsrBtGattParamConnUpdateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_RES */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_RES
CsrUint8 *CsrBtGattParamConnUpdateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattParamConnUpdateRes *primitive;

    primitive = (CsrBtGattParamConnUpdateRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->identifier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_RES */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_RES
void *CsrBtGattParamConnUpdateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattParamConnUpdateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattParamConnUpdateRes *) CsrPmemZalloc(sizeof(CsrBtGattParamConnUpdateRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->identifier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_RES */

#ifndef EXCLUDE_CSR_BT_GATT_EVENT_SEND_CFM
CsrSize CsrBtGattEventSendCfmSizeof(void *msg)
{
    CsrBtGattEventSendCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_EVENT_SEND_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_EVENT_SEND_CFM
CsrUint8 *CsrBtGattEventSendCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattEventSendCfm *primitive;

    primitive = (CsrBtGattEventSendCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_EVENT_SEND_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_EVENT_SEND_CFM
void *CsrBtGattEventSendCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattEventSendCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattEventSendCfm *) CsrPmemZalloc(sizeof(CsrBtGattEventSendCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_EVENT_SEND_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_EXCHANGE_MTU_REQ
CsrSize CsrBtGattExchangeMtuReqSizeof(void *msg)
{
    CsrBtGattExchangeMtuReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_EXCHANGE_MTU_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_EXCHANGE_MTU_REQ
CsrUint8 *CsrBtGattExchangeMtuReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattExchangeMtuReq *primitive;

    primitive = (CsrBtGattExchangeMtuReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mtu);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_EXCHANGE_MTU_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_EXCHANGE_MTU_REQ
void *CsrBtGattExchangeMtuReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattExchangeMtuReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattExchangeMtuReq *) CsrPmemZalloc(sizeof(CsrBtGattExchangeMtuReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mtu, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_EXCHANGE_MTU_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_CFM
CsrSize CsrBtGattDiscoverCharacCfmSizeof(void *msg)
{
    CsrBtGattDiscoverCharacCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_CFM
CsrUint8 *CsrBtGattDiscoverCharacCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDiscoverCharacCfm *primitive;

    primitive = (CsrBtGattDiscoverCharacCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_CFM
void *CsrBtGattDiscoverCharacCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDiscoverCharacCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDiscoverCharacCfm *) CsrPmemZalloc(sizeof(CsrBtGattDiscoverCharacCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_IND
CsrSize CsrBtGattFindInclServicesIndSizeof(void *msg)
{
    CsrBtGattFindInclServicesInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_IND */

#ifndef EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_IND
CsrUint8 *CsrBtGattFindInclServicesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattFindInclServicesInd *primitive;

    primitive = (CsrBtGattFindInclServicesInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attrHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->startHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->endGroupHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->uuid.length);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uuid.uuid), ((CsrSize) (16)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_IND */

#ifndef EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_IND
void *CsrBtGattFindInclServicesIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattFindInclServicesInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattFindInclServicesInd *) CsrPmemZalloc(sizeof(CsrBtGattFindInclServicesInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attrHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->startHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->endGroupHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->uuid.length, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uuid.uuid), buffer, &offset, ((CsrSize) (16)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM
CsrSize CsrBtGattDiscoverCharacDescriptorsCfmSizeof(void *msg)
{
    CsrBtGattDiscoverCharacDescriptorsCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM
CsrUint8 *CsrBtGattDiscoverCharacDescriptorsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDiscoverCharacDescriptorsCfm *primitive;

    primitive = (CsrBtGattDiscoverCharacDescriptorsCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM
void *CsrBtGattDiscoverCharacDescriptorsCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDiscoverCharacDescriptorsCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDiscoverCharacDescriptorsCfm *) CsrPmemZalloc(sizeof(CsrBtGattDiscoverCharacDescriptorsCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_ADVERTISE_REQ
CsrSize CsrBtGattAdvertiseReqSizeof(void *msg)
{
    CsrBtGattAdvertiseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->advertisingDataLength * sizeof(CsrUint8)) + (primitive->scanResponseDataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_ADVERTISE_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_ADVERTISE_REQ
CsrUint8 *CsrBtGattAdvertiseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattAdvertiseReq *primitive;

    primitive = (CsrBtGattAdvertiseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enable);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->advertisingFlags);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->advertisingDataLength);
    CsrMemCpySer(buffer, length, &primitive->advertisingData, sizeof(primitive->advertisingData));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scanResponseDataLength);
    CsrMemCpySer(buffer, length, &primitive->scanResponseData, sizeof(primitive->scanResponseData));
    if(primitive->advertisingData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->advertisingData), ((CsrSize) (primitive->advertisingDataLength * sizeof(CsrUint8))));
    }
    if(primitive->scanResponseData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->scanResponseData), ((CsrSize) (primitive->scanResponseDataLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_ADVERTISE_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_ADVERTISE_REQ
void *CsrBtGattAdvertiseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattAdvertiseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattAdvertiseReq *) CsrPmemZalloc(sizeof(CsrBtGattAdvertiseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enable, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->advertisingFlags, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->advertisingDataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->advertisingData), buffer, &offset, ((CsrSize) sizeof(primitive->advertisingData)));
    CsrUint8Des((CsrUint8 *) &primitive->scanResponseDataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->scanResponseData), buffer, &offset, ((CsrSize) sizeof(primitive->scanResponseData)));
    if (primitive->advertisingDataLength)
    {
        primitive->advertisingData = CsrPmemZalloc(((CsrUint32) (primitive->advertisingDataLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->advertisingData), buffer, &offset, ((CsrSize) (primitive->advertisingDataLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->advertisingData = NULL;
    }
    if (primitive->scanResponseDataLength)
    {
        primitive->scanResponseData = CsrPmemZalloc(((CsrUint32) (primitive->scanResponseDataLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->scanResponseData), buffer, &offset, ((CsrSize) (primitive->scanResponseDataLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->scanResponseData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_ADVERTISE_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_ADVERTISE_REQ
void CsrBtGattAdvertiseReqSerFree(void *msg)
{
    CsrBtGattAdvertiseReq *primitive;

    primitive = (CsrBtGattAdvertiseReq *) msg;

    if(primitive->advertisingData != NULL)
    {
        CsrPmemFree(primitive->advertisingData);
    }
    if(primitive->scanResponseData != NULL)
    {
        CsrPmemFree(primitive->scanResponseData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_GATT_ADVERTISE_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_SECURITY_REQ
CsrSize CsrBtGattSecurityReqSizeof(void *msg)
{
    CsrBtGattSecurityReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_SECURITY_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_SECURITY_REQ
CsrUint8 *CsrBtGattSecurityReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattSecurityReq *primitive;

    primitive = (CsrBtGattSecurityReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->securityRequirements);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_SECURITY_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_SECURITY_REQ
void *CsrBtGattSecurityReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattSecurityReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattSecurityReq *) CsrPmemZalloc(sizeof(CsrBtGattSecurityReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->securityRequirements, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_SECURITY_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_REQ
CsrSize CsrBtGattDiscoverServicesReqSizeof(void *msg)
{
    CsrBtGattDiscoverServicesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_REQ
CsrUint8 *CsrBtGattDiscoverServicesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDiscoverServicesReq *primitive;

    primitive = (CsrBtGattDiscoverServicesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->uuid.length);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uuid.uuid), ((CsrSize) (16)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_REQ
void *CsrBtGattDiscoverServicesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDiscoverServicesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDiscoverServicesReq *) CsrPmemZalloc(sizeof(CsrBtGattDiscoverServicesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->uuid.length, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uuid.uuid), buffer, &offset, ((CsrSize) (16)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DB_DEALLOC_CFM
CsrSize CsrBtGattDbDeallocCfmSizeof(void *msg)
{
    CsrBtGattDbDeallocCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_DEALLOC_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_DEALLOC_CFM
CsrUint8 *CsrBtGattDbDeallocCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDbDeallocCfm *primitive;

    primitive = (CsrBtGattDbDeallocCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->start);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->end);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_DEALLOC_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_DEALLOC_CFM
void *CsrBtGattDbDeallocCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDbDeallocCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDbDeallocCfm *) CsrPmemZalloc(sizeof(CsrBtGattDbDeallocCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->start, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->end, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_DEALLOC_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_REQ
CsrSize CsrBtGattBredrConnectReqSizeof(void *msg)
{
    CsrBtGattBredrConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_REQ
CsrUint8 *CsrBtGattBredrConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattBredrConnectReq *primitive;

    primitive = (CsrBtGattBredrConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_REQ
void *CsrBtGattBredrConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattBredrConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattBredrConnectReq *) CsrPmemZalloc(sizeof(CsrBtGattBredrConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_READ_CFM
CsrSize CsrBtGattReadCfmSizeof(void *msg)
{
    CsrBtGattReadCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->valueLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_CFM
CsrUint8 *CsrBtGattReadCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattReadCfm *primitive;

    primitive = (CsrBtGattReadCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->valueLength);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->valueLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_CFM
void *CsrBtGattReadCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattReadCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattReadCfm *) CsrPmemZalloc(sizeof(CsrBtGattReadCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->valueLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->valueLength)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->valueLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->valueLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_CFM
void CsrBtGattReadCfmSerFree(void *msg)
{
    CsrBtGattReadCfm *primitive;

    primitive = (CsrBtGattReadCfm *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_REGISTER_REQ
CsrSize CsrBtGattRegisterReqSizeof(void *msg)
{
    CsrBtGattRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_REGISTER_REQ
CsrUint8 *CsrBtGattRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattRegisterReq *primitive;

    primitive = (CsrBtGattRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_REGISTER_REQ
void *CsrBtGattRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattRegisterReq *) CsrPmemZalloc(sizeof(CsrBtGattRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_CFM
CsrSize CsrBtGattDiscoverServicesCfmSizeof(void *msg)
{
    CsrBtGattDiscoverServicesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_CFM
CsrUint8 *CsrBtGattDiscoverServicesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDiscoverServicesCfm *primitive;

    primitive = (CsrBtGattDiscoverServicesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_CFM
void *CsrBtGattDiscoverServicesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDiscoverServicesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDiscoverServicesCfm *) CsrPmemZalloc(sizeof(CsrBtGattDiscoverServicesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM
CsrSize CsrBtGattReadProfileDefinedDescriptorCfmSizeof(void *msg)
{
    CsrBtGattReadProfileDefinedDescriptorCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->valueLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM
CsrUint8 *CsrBtGattReadProfileDefinedDescriptorCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattReadProfileDefinedDescriptorCfm *primitive;

    primitive = (CsrBtGattReadProfileDefinedDescriptorCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->valueLength);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->valueLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM
void *CsrBtGattReadProfileDefinedDescriptorCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattReadProfileDefinedDescriptorCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattReadProfileDefinedDescriptorCfm *) CsrPmemZalloc(sizeof(CsrBtGattReadProfileDefinedDescriptorCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->valueLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->valueLength)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->valueLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->valueLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM
void CsrBtGattReadProfileDefinedDescriptorCfmSerFree(void *msg)
{
    CsrBtGattReadProfileDefinedDescriptorCfm *primitive;

    primitive = (CsrBtGattReadProfileDefinedDescriptorCfm *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_PROFILE_DEFINED_DESCRIPTOR_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_CLIENT_CONFIGURATION_CFM
CsrSize CsrBtGattReadClientConfigurationCfmSizeof(void *msg)
{
    CsrBtGattReadClientConfigurationCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_CLIENT_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_CLIENT_CONFIGURATION_CFM
CsrUint8 *CsrBtGattReadClientConfigurationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattReadClientConfigurationCfm *primitive;

    primitive = (CsrBtGattReadClientConfigurationCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->configuration);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_CLIENT_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_CLIENT_CONFIGURATION_CFM
void *CsrBtGattReadClientConfigurationCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattReadClientConfigurationCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattReadClientConfigurationCfm *) CsrPmemZalloc(sizeof(CsrBtGattReadClientConfigurationCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->configuration, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_CLIENT_CONFIGURATION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ADD_CFM
CsrSize CsrBtGattDbAddCfmSizeof(void *msg)
{
    CsrBtGattDbAddCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ADD_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ADD_CFM
CsrUint8 *CsrBtGattDbAddCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDbAddCfm *primitive;

    primitive = (CsrBtGattDbAddCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ADD_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ADD_CFM
void *CsrBtGattDbAddCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDbAddCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDbAddCfm *) CsrPmemZalloc(sizeof(CsrBtGattDbAddCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ADD_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_CFM
CsrSize CsrBtGattWhitelistAddCfmSizeof(void *msg)
{
    CsrBtGattWhitelistAddCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_CFM
CsrUint8 *CsrBtGattWhitelistAddCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattWhitelistAddCfm *primitive;

    primitive = (CsrBtGattWhitelistAddCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_CFM
void *CsrBtGattWhitelistAddCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattWhitelistAddCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattWhitelistAddCfm *) CsrPmemZalloc(sizeof(CsrBtGattWhitelistAddCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_ADD_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ALLOC_REQ
CsrSize CsrBtGattDbAllocReqSizeof(void *msg)
{
    CsrBtGattDbAllocReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ALLOC_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ALLOC_REQ
CsrUint8 *CsrBtGattDbAllocReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDbAllocReq *primitive;

    primitive = (CsrBtGattDbAllocReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->numOfAttrHandles);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->preferredStartHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ALLOC_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ALLOC_REQ
void *CsrBtGattDbAllocReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDbAllocReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDbAllocReq *) CsrPmemZalloc(sizeof(CsrBtGattDbAllocReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->numOfAttrHandles, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->preferredStartHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ALLOC_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_READ_CFM
CsrSize CsrBtGattWhitelistReadCfmSizeof(void *msg)
{
    CsrBtGattWhitelistReadCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->addressCount * sizeof(CsrBtTypedAddr)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_READ_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_READ_CFM
CsrUint8 *CsrBtGattWhitelistReadCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattWhitelistReadCfm *primitive;

    primitive = (CsrBtGattWhitelistReadCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addressCount);
    CsrMemCpySer(buffer, length, &primitive->address, sizeof(primitive->address));
    if(primitive->address)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->address), ((CsrSize) (primitive->addressCount * sizeof(CsrBtTypedAddr))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_READ_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_READ_CFM
void *CsrBtGattWhitelistReadCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattWhitelistReadCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattWhitelistReadCfm *) CsrPmemZalloc(sizeof(CsrBtGattWhitelistReadCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addressCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->address), buffer, &offset, ((CsrSize) sizeof(primitive->address)));
    if (primitive->addressCount)
    {
        primitive->address = CsrPmemZalloc(((CsrUint32) (primitive->addressCount * sizeof(CsrBtTypedAddr))));
        CsrMemCpyDes(((void *) primitive->address), buffer, &offset, ((CsrSize) (primitive->addressCount * sizeof(CsrBtTypedAddr))));
    }
    else
    {
        primitive->address = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_READ_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_READ_CFM
void CsrBtGattWhitelistReadCfmSerFree(void *msg)
{
    CsrBtGattWhitelistReadCfm *primitive;

    primitive = (CsrBtGattWhitelistReadCfm *) msg;

    if(primitive->address != NULL)
    {
        CsrPmemFree(primitive->address);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_READ_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ALLOC_CFM
CsrSize CsrBtGattDbAllocCfmSizeof(void *msg)
{
    CsrBtGattDbAllocCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ALLOC_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ALLOC_CFM
CsrUint8 *CsrBtGattDbAllocCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDbAllocCfm *primitive;

    primitive = (CsrBtGattDbAllocCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->start);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->end);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->preferredStartHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ALLOC_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ALLOC_CFM
void *CsrBtGattDbAllocCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDbAllocCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDbAllocCfm *) CsrPmemZalloc(sizeof(CsrBtGattDbAllocCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->start, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->end, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->preferredStartHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ALLOC_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_REQ
CsrSize CsrBtGattReadReqSizeof(void *msg)
{
    CsrBtGattReadReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_READ_REQ
CsrUint8 *CsrBtGattReadReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattReadReq *primitive;

    primitive = (CsrBtGattReadReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->offset);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_READ_REQ
void *CsrBtGattReadReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattReadReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattReadReq *) CsrPmemZalloc(sizeof(CsrBtGattReadReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->offset, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_CENTRAL_REQ
CsrSize CsrBtGattCentralReqSizeof(void *msg)
{
    CsrBtGattCentralReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_CENTRAL_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_CENTRAL_REQ
CsrUint8 *CsrBtGattCentralReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattCentralReq *primitive;

    primitive = (CsrBtGattCentralReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->preferredMtu);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_CENTRAL_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_CENTRAL_REQ
void *CsrBtGattCentralReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattCentralReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattCentralReq *) CsrPmemZalloc(sizeof(CsrBtGattCentralReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->preferredMtu, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_CENTRAL_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_REQ
CsrSize CsrBtGattParamAdvertiseReqSizeof(void *msg)
{
    CsrBtGattParamAdvertiseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_REQ
CsrUint8 *CsrBtGattParamAdvertiseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattParamAdvertiseReq *primitive;

    primitive = (CsrBtGattParamAdvertiseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->advIntervalMin);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->advIntervalMax);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->advertisingChannelMap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_REQ
void *CsrBtGattParamAdvertiseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattParamAdvertiseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattParamAdvertiseReq *) CsrPmemZalloc(sizeof(CsrBtGattParamAdvertiseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->advIntervalMin, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->advIntervalMax, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->advertisingChannelMap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM
CsrSize CsrBtGattReadAggregateFormatCfmSizeof(void *msg)
{
    CsrBtGattReadAggregateFormatCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->handlesCount * sizeof(CsrBtGattHandle)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM
CsrUint8 *CsrBtGattReadAggregateFormatCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattReadAggregateFormatCfm *primitive;

    primitive = (CsrBtGattReadAggregateFormatCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handlesCount);
    CsrMemCpySer(buffer, length, &primitive->handles, sizeof(primitive->handles));
    if(primitive->handles)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->handles), ((CsrSize) (primitive->handlesCount * sizeof(CsrBtGattHandle))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM
void *CsrBtGattReadAggregateFormatCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattReadAggregateFormatCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattReadAggregateFormatCfm *) CsrPmemZalloc(sizeof(CsrBtGattReadAggregateFormatCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handlesCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handles), buffer, &offset, ((CsrSize) sizeof(primitive->handles)));
    if (primitive->handlesCount)
    {
        primitive->handles = CsrPmemZalloc(((CsrUint32) (primitive->handlesCount * sizeof(CsrBtGattHandle))));
        CsrMemCpyDes(((void *) primitive->handles), buffer, &offset, ((CsrSize) (primitive->handlesCount * sizeof(CsrBtGattHandle))));
    }
    else
    {
        primitive->handles = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM
void CsrBtGattReadAggregateFormatCfmSerFree(void *msg)
{
    CsrBtGattReadAggregateFormatCfm *primitive;

    primitive = (CsrBtGattReadAggregateFormatCfm *) msg;

    if(primitive->handles != NULL)
    {
        CsrPmemFree(primitive->handles);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_AGGREGATE_FORMAT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_SCAN_CFM
CsrSize CsrBtGattScanCfmSizeof(void *msg)
{
    CsrBtGattScanCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_SCAN_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_SCAN_CFM
CsrUint8 *CsrBtGattScanCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattScanCfm *primitive;

    primitive = (CsrBtGattScanCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_SCAN_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_SCAN_CFM
void *CsrBtGattScanCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattScanCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattScanCfm *) CsrPmemZalloc(sizeof(CsrBtGattScanCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_SCAN_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_REPORT_IND
CsrSize CsrBtGattReportIndSizeof(void *msg)
{
    CsrBtGattReportInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_REPORT_IND */

#ifndef EXCLUDE_CSR_BT_GATT_REPORT_IND
CsrUint8 *CsrBtGattReportIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattReportInd *primitive;

    primitive = (CsrBtGattReportInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->eventType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->permanentAddress.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->permanentAddress.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->permanentAddress.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->permanentAddress.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->lengthData);
    CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (31)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rssi);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_REPORT_IND */

#ifndef EXCLUDE_CSR_BT_GATT_REPORT_IND
void *CsrBtGattReportIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattReportInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattReportInd *) CsrPmemZalloc(sizeof(CsrBtGattReportInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->eventType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->permanentAddress.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->permanentAddress.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->permanentAddress.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->permanentAddress.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->lengthData, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (31)));
    CsrUint8Des((CsrUint8 *) &primitive->rssi, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_REPORT_IND */

#ifndef EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_REQ
CsrSize CsrBtGattSubscriptionReqSizeof(void *msg)
{
    CsrBtGattSubscriptionReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_REQ
CsrUint8 *CsrBtGattSubscriptionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattSubscriptionReq *primitive;

    primitive = (CsrBtGattSubscriptionReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->valueHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->subscribe);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_REQ
void *CsrBtGattSubscriptionReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattSubscriptionReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattSubscriptionReq *) CsrPmemZalloc(sizeof(CsrBtGattSubscriptionReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->valueHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->subscribe, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_REQ
CsrSize CsrBtGattSetEventMaskReqSizeof(void *msg)
{
    CsrBtGattSetEventMaskReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_REQ
CsrUint8 *CsrBtGattSetEventMaskReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattSetEventMaskReq *primitive;

    primitive = (CsrBtGattSetEventMaskReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->eventMask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_REQ
void *CsrBtGattSetEventMaskReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattSetEventMaskReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattSetEventMaskReq *) CsrPmemZalloc(sizeof(CsrBtGattSetEventMaskReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->eventMask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_READ_USER_DESCRIPTION_CFM
CsrSize CsrBtGattReadUserDescriptionCfmSizeof(void *msg)
{
    CsrBtGattReadUserDescriptionCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->usrDescription));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_USER_DESCRIPTION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_USER_DESCRIPTION_CFM
CsrUint8 *CsrBtGattReadUserDescriptionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattReadUserDescriptionCfm *primitive;

    primitive = (CsrBtGattReadUserDescriptionCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrMemCpySer(buffer, length, &primitive->usrDescription, sizeof(primitive->usrDescription));
    CsrUtf8StringSer(buffer, length, primitive->usrDescription);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_USER_DESCRIPTION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_USER_DESCRIPTION_CFM
void *CsrBtGattReadUserDescriptionCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattReadUserDescriptionCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattReadUserDescriptionCfm *) CsrPmemZalloc(sizeof(CsrBtGattReadUserDescriptionCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->usrDescription), buffer, &offset, ((CsrSize) sizeof(primitive->usrDescription)));
    CsrUtf8StringDes(&primitive->usrDescription, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_USER_DESCRIPTION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_USER_DESCRIPTION_CFM
void CsrBtGattReadUserDescriptionCfmSerFree(void *msg)
{
    CsrBtGattReadUserDescriptionCfm *primitive;

    primitive = (CsrBtGattReadUserDescriptionCfm *) msg;

    if(primitive->usrDescription != NULL)
    {
        CsrPmemFree(primitive->usrDescription);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_USER_DESCRIPTION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_CANCEL_REQ
CsrSize CsrBtGattCancelReqSizeof(void *msg)
{
    CsrBtGattCancelReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_CANCEL_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_CANCEL_REQ
CsrUint8 *CsrBtGattCancelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattCancelReq *primitive;

    primitive = (CsrBtGattCancelReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_CANCEL_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_CANCEL_REQ
void *CsrBtGattCancelReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattCancelReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattCancelReq *) CsrPmemZalloc(sizeof(CsrBtGattCancelReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_CANCEL_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_CENTRAL_CFM
CsrSize CsrBtGattCentralCfmSizeof(void *msg)
{
    CsrBtGattCentralCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_CENTRAL_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_CENTRAL_CFM
CsrUint8 *CsrBtGattCentralCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattCentralCfm *primitive;

    primitive = (CsrBtGattCentralCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_CENTRAL_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_CENTRAL_CFM
void *CsrBtGattCentralCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattCentralCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattCentralCfm *) CsrPmemZalloc(sizeof(CsrBtGattCentralCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_CENTRAL_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_WRITE_CLIENT_CONFIGURATION_REQ
CsrSize CsrBtGattWriteClientConfigurationReqSizeof(void *msg)
{
    CsrBtGattWriteClientConfigurationReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_WRITE_CLIENT_CONFIGURATION_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WRITE_CLIENT_CONFIGURATION_REQ
CsrUint8 *CsrBtGattWriteClientConfigurationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattWriteClientConfigurationReq *primitive;

    primitive = (CsrBtGattWriteClientConfigurationReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->clientConfigHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->valueHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->configuration);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_WRITE_CLIENT_CONFIGURATION_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WRITE_CLIENT_CONFIGURATION_REQ
void *CsrBtGattWriteClientConfigurationReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattWriteClientConfigurationReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattWriteClientConfigurationReq *) CsrPmemZalloc(sizeof(CsrBtGattWriteClientConfigurationReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->clientConfigHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->valueHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->configuration, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_WRITE_CLIENT_CONFIGURATION_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_SDS_REQ
CsrSize CsrBtGattSdsReqSizeof(void *msg)
{
    CsrBtGattSdsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_SDS_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_SDS_REQ
CsrUint8 *CsrBtGattSdsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattSdsReq *primitive;

    primitive = (CsrBtGattSdsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_SDS_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_SDS_REQ
void *CsrBtGattSdsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattSdsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattSdsReq *) CsrPmemZalloc(sizeof(CsrBtGattSdsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_SDS_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PRIVATE_UNREGISTER_REQ
CsrSize CsrBtGattPrivateUnregisterReqSizeof(void *msg)
{
    CsrBtGattPrivateUnregisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_PRIVATE_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PRIVATE_UNREGISTER_REQ
CsrUint8 *CsrBtGattPrivateUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattPrivateUnregisterReq *primitive;

    primitive = (CsrBtGattPrivateUnregisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_PRIVATE_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PRIVATE_UNREGISTER_REQ
void *CsrBtGattPrivateUnregisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattPrivateUnregisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattPrivateUnregisterReq *) CsrPmemZalloc(sizeof(CsrBtGattPrivateUnregisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_PRIVATE_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_CFM
CsrSize CsrBtGattSetEventMaskCfmSizeof(void *msg)
{
    CsrBtGattSetEventMaskCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_CFM
CsrUint8 *CsrBtGattSetEventMaskCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattSetEventMaskCfm *primitive;

    primitive = (CsrBtGattSetEventMaskCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_CFM
void *CsrBtGattSetEventMaskCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattSetEventMaskCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattSetEventMaskCfm *) CsrPmemZalloc(sizeof(CsrBtGattSetEventMaskCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_REQ
CsrSize CsrBtGattFindInclServicesReqSizeof(void *msg)
{
    CsrBtGattFindInclServicesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_REQ
CsrUint8 *CsrBtGattFindInclServicesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattFindInclServicesReq *primitive;

    primitive = (CsrBtGattFindInclServicesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->startHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->endGroupHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_REQ
void *CsrBtGattFindInclServicesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattFindInclServicesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattFindInclServicesReq *) CsrPmemZalloc(sizeof(CsrBtGattFindInclServicesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->startHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->endGroupHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_REQ
CsrSize CsrBtGattWhitelistClearReqSizeof(void *msg)
{
    CsrBtGattWhitelistClearReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_REQ
CsrUint8 *CsrBtGattWhitelistClearReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattWhitelistClearReq *primitive;

    primitive = (CsrBtGattWhitelistClearReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_REQ
void *CsrBtGattWhitelistClearReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattWhitelistClearReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattWhitelistClearReq *) CsrPmemZalloc(sizeof(CsrBtGattWhitelistClearReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_CLEAR_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_MTU_CHANGED_IND
CsrSize CsrBtGattMtuChangedIndSizeof(void *msg)
{
    CsrBtGattMtuChangedInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_MTU_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_GATT_MTU_CHANGED_IND
CsrUint8 *CsrBtGattMtuChangedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattMtuChangedInd *primitive;

    primitive = (CsrBtGattMtuChangedInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mtu);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_MTU_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_GATT_MTU_CHANGED_IND
void *CsrBtGattMtuChangedIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattMtuChangedInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattMtuChangedInd *) CsrPmemZalloc(sizeof(CsrBtGattMtuChangedInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mtu, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_MTU_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_CFM
CsrSize CsrBtGattSubscriptionCfmSizeof(void *msg)
{
    CsrBtGattSubscriptionCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_CFM
CsrUint8 *CsrBtGattSubscriptionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattSubscriptionCfm *primitive;

    primitive = (CsrBtGattSubscriptionCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_CFM
void *CsrBtGattSubscriptionCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattSubscriptionCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattSubscriptionCfm *) CsrPmemZalloc(sizeof(CsrBtGattSubscriptionCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_SUBSCRIPTION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_CFM
CsrSize CsrBtGattReadByUuidCfmSizeof(void *msg)
{
    CsrBtGattReadByUuidCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_CFM
CsrUint8 *CsrBtGattReadByUuidCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattReadByUuidCfm *primitive;

    primitive = (CsrBtGattReadByUuidCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_BY_UUID_CFM
void *CsrBtGattReadByUuidCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattReadByUuidCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattReadByUuidCfm *) CsrPmemZalloc(sizeof(CsrBtGattReadByUuidCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_BY_UUID_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_COMPLETE_IND
CsrSize CsrBtGattDbAccessCompleteIndSizeof(void *msg)
{
    CsrBtGattDbAccessCompleteInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_COMPLETE_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_COMPLETE_IND
CsrUint8 *CsrBtGattDbAccessCompleteIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDbAccessCompleteInd *primitive;

    primitive = (CsrBtGattDbAccessCompleteInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connInfo);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attrHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->commit);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_COMPLETE_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DB_ACCESS_COMPLETE_IND
void *CsrBtGattDbAccessCompleteIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDbAccessCompleteInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDbAccessCompleteInd *) CsrPmemZalloc(sizeof(CsrBtGattDbAccessCompleteInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connInfo, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attrHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->commit, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_ACCESS_COMPLETE_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DISCONNECT_IND
CsrSize CsrBtGattDisconnectIndSizeof(void *msg)
{
    CsrBtGattDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DISCONNECT_IND
CsrUint8 *CsrBtGattDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDisconnectInd *primitive;

    primitive = (CsrBtGattDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connInfo);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DISCONNECT_IND
void *CsrBtGattDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtGattDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connInfo, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_GATT_READ_MULTI_REQ
CsrSize CsrBtGattReadMultiReqSizeof(void *msg)
{
    CsrBtGattReadMultiReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->handlesCount * sizeof(CsrBtGattHandle)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_MULTI_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_READ_MULTI_REQ
CsrUint8 *CsrBtGattReadMultiReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattReadMultiReq *primitive;

    primitive = (CsrBtGattReadMultiReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handlesCount);
    CsrMemCpySer(buffer, length, &primitive->handles, sizeof(primitive->handles));
    if(primitive->handles)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->handles), ((CsrSize) (primitive->handlesCount * sizeof(CsrBtGattHandle))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_MULTI_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_READ_MULTI_REQ
void *CsrBtGattReadMultiReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattReadMultiReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattReadMultiReq *) CsrPmemZalloc(sizeof(CsrBtGattReadMultiReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handlesCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->handles), buffer, &offset, ((CsrSize) sizeof(primitive->handles)));
    if (primitive->handlesCount)
    {
        primitive->handles = CsrPmemZalloc(((CsrUint32) (primitive->handlesCount * sizeof(CsrBtGattHandle))));
        CsrMemCpyDes(((void *) primitive->handles), buffer, &offset, ((CsrSize) (primitive->handlesCount * sizeof(CsrBtGattHandle))));
    }
    else
    {
        primitive->handles = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_MULTI_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_READ_MULTI_REQ
void CsrBtGattReadMultiReqSerFree(void *msg)
{
    CsrBtGattReadMultiReq *primitive;

    primitive = (CsrBtGattReadMultiReq *) msg;

    if(primitive->handles != NULL)
    {
        CsrPmemFree(primitive->handles);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_MULTI_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_CFM
CsrSize CsrBtGattParamAdvertiseCfmSizeof(void *msg)
{
    CsrBtGattParamAdvertiseCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_CFM
CsrUint8 *CsrBtGattParamAdvertiseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattParamAdvertiseCfm *primitive;

    primitive = (CsrBtGattParamAdvertiseCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_CFM
void *CsrBtGattParamAdvertiseCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattParamAdvertiseCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattParamAdvertiseCfm *) CsrPmemZalloc(sizeof(CsrBtGattParamAdvertiseCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_ADVERTISE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_IND
CsrSize CsrBtGattDiscoverCharacDescriptorsIndSizeof(void *msg)
{
    CsrBtGattDiscoverCharacDescriptorsInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_IND
CsrUint8 *CsrBtGattDiscoverCharacDescriptorsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDiscoverCharacDescriptorsInd *primitive;

    primitive = (CsrBtGattDiscoverCharacDescriptorsInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->uuid.length);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uuid.uuid), ((CsrSize) (16)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->descriptorHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_IND
void *CsrBtGattDiscoverCharacDescriptorsIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDiscoverCharacDescriptorsInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDiscoverCharacDescriptorsInd *) CsrPmemZalloc(sizeof(CsrBtGattDiscoverCharacDescriptorsInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->uuid.length, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uuid.uuid), buffer, &offset, ((CsrSize) (16)));
    CsrUint16Des((CsrUint16 *) &primitive->descriptorHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_IND */

#ifndef EXCLUDE_CSR_BT_GATT_READ_MULTI_CFM
CsrSize CsrBtGattReadMultiCfmSizeof(void *msg)
{
    CsrBtGattReadMultiCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->valueLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_MULTI_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_MULTI_CFM
CsrUint8 *CsrBtGattReadMultiCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattReadMultiCfm *primitive;

    primitive = (CsrBtGattReadMultiCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->valueLength);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->valueLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_MULTI_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_MULTI_CFM
void *CsrBtGattReadMultiCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattReadMultiCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattReadMultiCfm *) CsrPmemZalloc(sizeof(CsrBtGattReadMultiCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->valueLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    if (primitive->valueLength)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->valueLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->valueLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_MULTI_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_READ_MULTI_CFM
void CsrBtGattReadMultiCfmSerFree(void *msg)
{
    CsrBtGattReadMultiCfm *primitive;

    primitive = (CsrBtGattReadMultiCfm *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_GATT_READ_MULTI_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_NOTIFICATION_IND
CsrSize CsrBtGattNotificationIndSizeof(void *msg)
{
    CsrBtGattNotificationInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->valueLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_GATT_NOTIFICATION_IND
CsrUint8 *CsrBtGattNotificationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattNotificationInd *primitive;

    primitive = (CsrBtGattNotificationInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->valueHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->valueLength);
    CsrMemCpySer(buffer, length, &primitive->value, sizeof(primitive->value));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connInfo);
    if(primitive->value)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->value), ((CsrSize) (primitive->valueLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_GATT_NOTIFICATION_IND
void *CsrBtGattNotificationIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattNotificationInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattNotificationInd *) CsrPmemZalloc(sizeof(CsrBtGattNotificationInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->valueHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->valueLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->value), buffer, &offset, ((CsrSize) sizeof(primitive->value)));
    CsrUint32Des((CsrUint32 *) &primitive->connInfo, buffer, &offset);
    if (primitive->valueLength)
    {
        primitive->value = CsrPmemZalloc(((CsrUint32) (primitive->valueLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->value), buffer, &offset, ((CsrSize) (primitive->valueLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->value = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_GATT_NOTIFICATION_IND
void CsrBtGattNotificationIndSerFree(void *msg)
{
    CsrBtGattNotificationInd *primitive;

    primitive = (CsrBtGattNotificationInd *) msg;

    if(primitive->value != NULL)
    {
        CsrPmemFree(primitive->value);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_GATT_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_GATT_UNREGISTER_REQ
CsrSize CsrBtGattUnregisterReqSizeof(void *msg)
{
    CsrBtGattUnregisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_UNREGISTER_REQ
CsrUint8 *CsrBtGattUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattUnregisterReq *primitive;

    primitive = (CsrBtGattUnregisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_UNREGISTER_REQ
void *CsrBtGattUnregisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattUnregisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattUnregisterReq *) CsrPmemZalloc(sizeof(CsrBtGattUnregisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_UNREGISTER_CFM
CsrSize CsrBtGattUnregisterCfmSizeof(void *msg)
{
    CsrBtGattUnregisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_UNREGISTER_CFM
CsrUint8 *CsrBtGattUnregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattUnregisterCfm *primitive;

    primitive = (CsrBtGattUnregisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_UNREGISTER_CFM
void *CsrBtGattUnregisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattUnregisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattUnregisterCfm *) CsrPmemZalloc(sizeof(CsrBtGattUnregisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_IND
CsrSize CsrBtGattDiscoverServicesIndSizeof(void *msg)
{
    CsrBtGattDiscoverServicesInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_IND
CsrUint8 *CsrBtGattDiscoverServicesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDiscoverServicesInd *primitive;

    primitive = (CsrBtGattDiscoverServicesInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->startHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->endHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->uuid.length);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uuid.uuid), ((CsrSize) (16)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_IND
void *CsrBtGattDiscoverServicesIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDiscoverServicesInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDiscoverServicesInd *) CsrPmemZalloc(sizeof(CsrBtGattDiscoverServicesInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->startHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->endHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->uuid.length, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uuid.uuid), buffer, &offset, ((CsrSize) (16)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_SERVICES_IND */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_READ_REQ
CsrSize CsrBtGattWhitelistReadReqSizeof(void *msg)
{
    CsrBtGattWhitelistReadReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_READ_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_READ_REQ
CsrUint8 *CsrBtGattWhitelistReadReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattWhitelistReadReq *primitive;

    primitive = (CsrBtGattWhitelistReadReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_READ_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_WHITELIST_READ_REQ
void *CsrBtGattWhitelistReadReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattWhitelistReadReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattWhitelistReadReq *) CsrPmemZalloc(sizeof(CsrBtGattWhitelistReadReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_WHITELIST_READ_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_CONNECT_IND
CsrSize CsrBtGattConnectIndSizeof(void *msg)
{
    CsrBtGattConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_GATT_CONNECT_IND
CsrUint8 *CsrBtGattConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattConnectInd *primitive;

    primitive = (CsrBtGattConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connInfo);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mtu);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_GATT_CONNECT_IND
void *CsrBtGattConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattConnectInd *) CsrPmemZalloc(sizeof(CsrBtGattConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connInfo, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mtu, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_REQ
CsrSize CsrBtGattDiscoverCharacDescriptorsReqSizeof(void *msg)
{
    CsrBtGattDiscoverCharacDescriptorsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_REQ
CsrUint8 *CsrBtGattDiscoverCharacDescriptorsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDiscoverCharacDescriptorsReq *primitive;

    primitive = (CsrBtGattDiscoverCharacDescriptorsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->startHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->endGroupHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_REQ
void *CsrBtGattDiscoverCharacDescriptorsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDiscoverCharacDescriptorsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDiscoverCharacDescriptorsReq *) CsrPmemZalloc(sizeof(CsrBtGattDiscoverCharacDescriptorsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->startHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->endGroupHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DISCOVER_CHARAC_DESCRIPTORS_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DB_DEALLOC_REQ
CsrSize CsrBtGattDbDeallocReqSizeof(void *msg)
{
    CsrBtGattDbDeallocReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_DEALLOC_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DB_DEALLOC_REQ
CsrUint8 *CsrBtGattDbDeallocReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDbDeallocReq *primitive;

    primitive = (CsrBtGattDbDeallocReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_DEALLOC_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DB_DEALLOC_REQ
void *CsrBtGattDbDeallocReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDbDeallocReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDbDeallocReq *) CsrPmemZalloc(sizeof(CsrBtGattDbDeallocReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_DEALLOC_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_IND
CsrSize CsrBtGattParamConnUpdateIndSizeof(void *msg)
{
    CsrBtGattParamConnUpdateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_IND */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_IND
CsrUint8 *CsrBtGattParamConnUpdateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattParamConnUpdateInd *primitive;

    primitive = (CsrBtGattParamConnUpdateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connIntervalMax);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connIntervalMin);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connLatency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supervisionTimeout);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->incoming);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->identifier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_IND */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_IND
void *CsrBtGattParamConnUpdateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattParamConnUpdateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattParamConnUpdateInd *) CsrPmemZalloc(sizeof(CsrBtGattParamConnUpdateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connIntervalMax, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connIntervalMin, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connLatency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supervisionTimeout, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->incoming, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->identifier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_IND */

#ifndef EXCLUDE_CSR_BT_GATT_DB_REMOVE_REQ
CsrSize CsrBtGattDbRemoveReqSizeof(void *msg)
{
    CsrBtGattDbRemoveReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_REMOVE_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DB_REMOVE_REQ
CsrUint8 *CsrBtGattDbRemoveReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattDbRemoveReq *primitive;

    primitive = (CsrBtGattDbRemoveReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->start);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->end);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_REMOVE_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_DB_REMOVE_REQ
void *CsrBtGattDbRemoveReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattDbRemoveReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattDbRemoveReq *) CsrPmemZalloc(sizeof(CsrBtGattDbRemoveReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->start, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->end, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_DB_REMOVE_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_CFM
CsrSize CsrBtGattFindInclServicesCfmSizeof(void *msg)
{
    CsrBtGattFindInclServicesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_CFM
CsrUint8 *CsrBtGattFindInclServicesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattFindInclServicesCfm *primitive;

    primitive = (CsrBtGattFindInclServicesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_CFM
void *CsrBtGattFindInclServicesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattFindInclServicesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattFindInclServicesCfm *) CsrPmemZalloc(sizeof(CsrBtGattFindInclServicesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_FIND_INCL_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PERIPHERAL_REQ
CsrSize CsrBtGattPeripheralReqSizeof(void *msg)
{
    CsrBtGattPeripheralReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->advertisingDataLength * sizeof(CsrUint8)) + (primitive->scanResponseDataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_PERIPHERAL_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PERIPHERAL_REQ
CsrUint8 *CsrBtGattPeripheralReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattPeripheralReq *primitive;

    primitive = (CsrBtGattPeripheralReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->flags);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->advertisingDataLength);
    CsrMemCpySer(buffer, length, &primitive->advertisingData, sizeof(primitive->advertisingData));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scanResponseDataLength);
    CsrMemCpySer(buffer, length, &primitive->scanResponseData, sizeof(primitive->scanResponseData));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->preferredMtu);
    if(primitive->advertisingData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->advertisingData), ((CsrSize) (primitive->advertisingDataLength * sizeof(CsrUint8))));
    }
    if(primitive->scanResponseData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->scanResponseData), ((CsrSize) (primitive->scanResponseDataLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_PERIPHERAL_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PERIPHERAL_REQ
void *CsrBtGattPeripheralReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattPeripheralReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattPeripheralReq *) CsrPmemZalloc(sizeof(CsrBtGattPeripheralReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->flags, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->advertisingDataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->advertisingData), buffer, &offset, ((CsrSize) sizeof(primitive->advertisingData)));
    CsrUint8Des((CsrUint8 *) &primitive->scanResponseDataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->scanResponseData), buffer, &offset, ((CsrSize) sizeof(primitive->scanResponseData)));
    CsrUint16Des((CsrUint16 *) &primitive->preferredMtu, buffer, &offset);
    if (primitive->advertisingDataLength)
    {
        primitive->advertisingData = CsrPmemZalloc(((CsrUint32) (primitive->advertisingDataLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->advertisingData), buffer, &offset, ((CsrSize) (primitive->advertisingDataLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->advertisingData = NULL;
    }
    if (primitive->scanResponseDataLength)
    {
        primitive->scanResponseData = CsrPmemZalloc(((CsrUint32) (primitive->scanResponseDataLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->scanResponseData), buffer, &offset, ((CsrSize) (primitive->scanResponseDataLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->scanResponseData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_PERIPHERAL_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PERIPHERAL_REQ
void CsrBtGattPeripheralReqSerFree(void *msg)
{
    CsrBtGattPeripheralReq *primitive;

    primitive = (CsrBtGattPeripheralReq *) msg;

    if(primitive->advertisingData != NULL)
    {
        CsrPmemFree(primitive->advertisingData);
    }
    if(primitive->scanResponseData != NULL)
    {
        CsrPmemFree(primitive->scanResponseData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_GATT_PERIPHERAL_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_CFM
CsrSize CsrBtGattBredrConnectCfmSizeof(void *msg)
{
    CsrBtGattBredrConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_CFM
CsrUint8 *CsrBtGattBredrConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattBredrConnectCfm *primitive;

    primitive = (CsrBtGattBredrConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_CFM
void *CsrBtGattBredrConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattBredrConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattBredrConnectCfm *) CsrPmemZalloc(sizeof(CsrBtGattBredrConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_BREDR_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND
CsrSize CsrBtGattPhysicalLinkStatusIndSizeof(void *msg)
{
    CsrBtGattPhysicalLinkStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND
CsrUint8 *CsrBtGattPhysicalLinkStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattPhysicalLinkStatusInd *primitive;

    primitive = (CsrBtGattPhysicalLinkStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connInfo);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND
void *CsrBtGattPhysicalLinkStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattPhysicalLinkStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattPhysicalLinkStatusInd *) CsrPmemZalloc(sizeof(CsrBtGattPhysicalLinkStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connInfo, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_PHYSICAL_LINK_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_CFM
CsrSize CsrBtGattParamConnUpdateCfmSizeof(void *msg)
{
    CsrBtGattParamConnUpdateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_CFM
CsrUint8 *CsrBtGattParamConnUpdateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattParamConnUpdateCfm *primitive;

    primitive = (CsrBtGattParamConnUpdateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_CFM
void *CsrBtGattParamConnUpdateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattParamConnUpdateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattParamConnUpdateCfm *) CsrPmemZalloc(sizeof(CsrBtGattParamConnUpdateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_CFM
CsrSize CsrBtGattParamConnectionCfmSizeof(void *msg)
{
    CsrBtGattParamConnectionCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_CFM
CsrUint8 *CsrBtGattParamConnectionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattParamConnectionCfm *primitive;

    primitive = (CsrBtGattParamConnectionCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_CFM
void *CsrBtGattParamConnectionCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattParamConnectionCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattParamConnectionCfm *) CsrPmemZalloc(sizeof(CsrBtGattParamConnectionCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONNECTION_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_SCAN_CFM
CsrSize CsrBtGattParamScanCfmSizeof(void *msg)
{
    CsrBtGattParamScanCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_SCAN_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_SCAN_CFM
CsrUint8 *CsrBtGattParamScanCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattParamScanCfm *primitive;

    primitive = (CsrBtGattParamScanCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_SCAN_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_SCAN_CFM
void *CsrBtGattParamScanCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattParamScanCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattParamScanCfm *) CsrPmemZalloc(sizeof(CsrBtGattParamScanCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_SCAN_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_REGISTER_CFM
CsrSize CsrBtGattRegisterCfmSizeof(void *msg)
{
    CsrBtGattRegisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_REGISTER_CFM
CsrUint8 *CsrBtGattRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattRegisterCfm *primitive;

    primitive = (CsrBtGattRegisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_REGISTER_CFM
void *CsrBtGattRegisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattRegisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattRegisterCfm *) CsrPmemZalloc(sizeof(CsrBtGattRegisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_REQ
CsrSize CsrBtGattParamConnUpdateReqSizeof(void *msg)
{
    CsrBtGattParamConnUpdateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_REQ
CsrUint8 *CsrBtGattParamConnUpdateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtGattParamConnUpdateReq *primitive;

    primitive = (CsrBtGattParamConnUpdateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->gattId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connIntervalMin);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connIntervalMax);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connLatency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supervisionTimeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->minimumCeLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maximumCeLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->l2caSignalId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_REQ */

#ifndef EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_REQ
void *CsrBtGattParamConnUpdateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtGattParamConnUpdateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtGattParamConnUpdateReq *) CsrPmemZalloc(sizeof(CsrBtGattParamConnUpdateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->gattId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connIntervalMin, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connIntervalMax, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connLatency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supervisionTimeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->minimumCeLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maximumCeLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->l2caSignalId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_GATT_PARAM_CONN_UPDATE_REQ */

#endif /* EXCLUDE_CSR_BT_GATT_PRIVATE_MODULE */
