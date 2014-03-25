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
#include "csr_bt_sd_private_prim.h"

#ifndef EXCLUDE_CSR_BT_SD_PRIVATE_MODULE

void CsrBtSdPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_REQ
CsrSize CsrBtSdReadServicesCancelReqSizeof(void *msg)
{
    CsrBtSdReadServicesCancelReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_REQ
CsrUint8 *CsrBtSdReadServicesCancelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdReadServicesCancelReq *primitive;

    primitive = (CsrBtSdReadServicesCancelReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_REQ
void *CsrBtSdReadServicesCancelReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdReadServicesCancelReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdReadServicesCancelReq *) CsrPmemZalloc(sizeof(CsrBtSdReadServicesCancelReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_REQ */

#ifndef EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM
CsrSize CsrBtSdCancelReadAvailableServicesCfmSizeof(void *msg)
{
    CsrBtSdCancelReadAvailableServicesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM
CsrUint8 *CsrBtSdCancelReadAvailableServicesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdCancelReadAvailableServicesCfm *primitive;

    primitive = (CsrBtSdCancelReadAvailableServicesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM
void *CsrBtSdCancelReadAvailableServicesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdCancelReadAvailableServicesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdCancelReadAvailableServicesCfm *) CsrPmemZalloc(sizeof(CsrBtSdCancelReadAvailableServicesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM
CsrSize CsrBtSdRegisterServiceRecordCfmSizeof(void *msg)
{
    CsrBtSdRegisterServiceRecordCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM
CsrUint8 *CsrBtSdRegisterServiceRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdRegisterServiceRecordCfm *primitive;

    primitive = (CsrBtSdRegisterServiceRecordCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM
void *CsrBtSdRegisterServiceRecordCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdRegisterServiceRecordCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdRegisterServiceRecordCfm *) CsrPmemZalloc(sizeof(CsrBtSdRegisterServiceRecordCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_IND
CsrSize CsrBtSdReadServiceRecordIndSizeof(void *msg)
{
    CsrBtSdReadServiceRecordInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_IND */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_IND
CsrUint8 *CsrBtSdReadServiceRecordIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdReadServiceRecordInd *primitive;

    primitive = (CsrBtSdReadServiceRecordInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLen);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_IND */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_IND
void *CsrBtSdReadServiceRecordIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdReadServiceRecordInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdReadServiceRecordInd *) CsrPmemZalloc(sizeof(CsrBtSdReadServiceRecordInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_IND */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_IND
void CsrBtSdReadServiceRecordIndSerFree(void *msg)
{
    CsrBtSdReadServiceRecordInd *primitive;

    primitive = (CsrBtSdReadServiceRecordInd *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_IND */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_RESULT_IND
CsrSize CsrBtSdSearchResultIndSizeof(void *msg)
{
    CsrBtSdSearchResultInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->infoLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_RESULT_IND
CsrUint8 *CsrBtSdSearchResultIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdSearchResultInd *primitive;

    primitive = (CsrBtSdSearchResultInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceClass);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rssi);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->infoLen);
    CsrMemCpySer(buffer, length, &primitive->info, sizeof(primitive->info));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceStatus);
    if(primitive->info)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->info), ((CsrSize) (primitive->infoLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_RESULT_IND
void *CsrBtSdSearchResultIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdSearchResultInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdSearchResultInd *) CsrPmemZalloc(sizeof(CsrBtSdSearchResultInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceClass, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->rssi, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->infoLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->info), buffer, &offset, ((CsrSize) sizeof(primitive->info)));
    CsrUint32Des((CsrUint32 *) &primitive->deviceStatus, buffer, &offset);
    if (primitive->infoLen)
    {
        primitive->info = CsrPmemZalloc(((CsrUint32) (primitive->infoLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->info), buffer, &offset, ((CsrSize) (primitive->infoLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->info = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_RESULT_IND
void CsrBtSdSearchResultIndSerFree(void *msg)
{
    CsrBtSdSearchResultInd *primitive;

    primitive = (CsrBtSdSearchResultInd *) msg;

    if(primitive->info != NULL)
    {
        CsrPmemFree(primitive->info);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_SD_CLEANUP_REQ
CsrSize CsrBtSdCleanupReqSizeof(void *msg)
{
    CsrBtSdCleanupReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_CLEANUP_REQ */

#ifndef EXCLUDE_CSR_BT_SD_CLEANUP_REQ
CsrUint8 *CsrBtSdCleanupReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdCleanupReq *primitive;

    primitive = (CsrBtSdCleanupReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->cleanupMode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_CLEANUP_REQ */

#ifndef EXCLUDE_CSR_BT_SD_CLEANUP_REQ
void *CsrBtSdCleanupReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdCleanupReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdCleanupReq *) CsrPmemZalloc(sizeof(CsrBtSdCleanupReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->cleanupMode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_CLEANUP_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_REQ
CsrSize CsrBtSdReadServicesReqSizeof(void *msg)
{
    CsrBtSdReadServicesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->filterLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_REQ
CsrUint8 *CsrBtSdReadServicesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdReadServicesReq *primitive;

    primitive = (CsrBtSdReadServicesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceConfig);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->filterLen);
    CsrMemCpySer(buffer, length, &primitive->filter, sizeof(primitive->filter));
    if(primitive->filter)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->filter), ((CsrSize) (primitive->filterLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_REQ
void *CsrBtSdReadServicesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdReadServicesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdReadServicesReq *) CsrPmemZalloc(sizeof(CsrBtSdReadServicesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceConfig, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->filterLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->filter), buffer, &offset, ((CsrSize) sizeof(primitive->filter)));
    if (primitive->filterLen)
    {
        primitive->filter = CsrPmemZalloc(((CsrUint32) (primitive->filterLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->filter), buffer, &offset, ((CsrSize) (primitive->filterLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->filter = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_REQ
void CsrBtSdReadServicesReqSerFree(void *msg)
{
    CsrBtSdReadServicesReq *primitive;

    primitive = (CsrBtSdReadServicesReq *) msg;

    if(primitive->filter != NULL)
    {
        CsrPmemFree(primitive->filter);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_REQ
CsrSize CsrBtSdReadDeviceInfoReqSizeof(void *msg)
{
    CsrBtSdReadDeviceInfoReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_REQ
CsrUint8 *CsrBtSdReadDeviceInfoReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdReadDeviceInfoReq *primitive;

    primitive = (CsrBtSdReadDeviceInfoReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_REQ
void *CsrBtSdReadDeviceInfoReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdReadDeviceInfoReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdReadDeviceInfoReq *) CsrPmemZalloc(sizeof(CsrBtSdReadDeviceInfoReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_SD_CANCEL_SEARCH_REQ
CsrSize CsrBtSdCancelSearchReqSizeof(void *msg)
{
    CsrBtSdCancelSearchReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_SD_CANCEL_SEARCH_REQ
CsrUint8 *CsrBtSdCancelSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdCancelSearchReq *primitive;

    primitive = (CsrBtSdCancelSearchReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_SD_CANCEL_SEARCH_REQ
void *CsrBtSdCancelSearchReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdCancelSearchReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdCancelSearchReq *) CsrPmemZalloc(sizeof(CsrBtSdCancelSearchReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND
CsrSize CsrBtSdImmediateSearchResultIndSizeof(void *msg)
{
    CsrBtSdImmediateSearchResultInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->infoLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND
CsrUint8 *CsrBtSdImmediateSearchResultIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdImmediateSearchResultInd *primitive;

    primitive = (CsrBtSdImmediateSearchResultInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceClass);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rssi);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->infoLen);
    CsrMemCpySer(buffer, length, &primitive->info, sizeof(primitive->info));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceStatus);
    if(primitive->info)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->info), ((CsrSize) (primitive->infoLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND
void *CsrBtSdImmediateSearchResultIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdImmediateSearchResultInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdImmediateSearchResultInd *) CsrPmemZalloc(sizeof(CsrBtSdImmediateSearchResultInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceClass, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->rssi, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->infoLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->info), buffer, &offset, ((CsrSize) sizeof(primitive->info)));
    CsrUint32Des((CsrUint32 *) &primitive->deviceStatus, buffer, &offset);
    if (primitive->infoLen)
    {
        primitive->info = CsrPmemZalloc(((CsrUint32) (primitive->infoLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->info), buffer, &offset, ((CsrSize) (primitive->infoLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->info = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND
void CsrBtSdImmediateSearchResultIndSerFree(void *msg)
{
    CsrBtSdImmediateSearchResultInd *primitive;

    primitive = (CsrBtSdImmediateSearchResultInd *) msg;

    if(primitive->info != NULL)
    {
        CsrPmemFree(primitive->info);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SD_IMMEDIATE_SEARCH_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_SD_CLEANUP_CFM
CsrSize CsrBtSdCleanupCfmSizeof(void *msg)
{
    CsrBtSdCleanupCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_CLEANUP_CFM */

#ifndef EXCLUDE_CSR_BT_SD_CLEANUP_CFM
CsrUint8 *CsrBtSdCleanupCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdCleanupCfm *primitive;

    primitive = (CsrBtSdCleanupCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->cleanupMode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_CLEANUP_CFM */

#ifndef EXCLUDE_CSR_BT_SD_CLEANUP_CFM
void *CsrBtSdCleanupCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdCleanupCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdCleanupCfm *) CsrPmemZalloc(sizeof(CsrBtSdCleanupCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->cleanupMode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_CLEANUP_CFM */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_REQ
CsrSize CsrBtSdSearchReqSizeof(void *msg)
{
    CsrBtSdSearchReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->filterLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_REQ
CsrUint8 *CsrBtSdSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdSearchReq *primitive;

    primitive = (CsrBtSdSearchReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->searchConfig);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->rssiBufferTime);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalSearchTime);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rssiThreshold);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceClass);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceClassMask);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->inquiryAccessCode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->filterLen);
    CsrMemCpySer(buffer, length, &primitive->filter, sizeof(primitive->filter));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->inquiryTxPowerLevel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->leRssiThreshold);
    if(primitive->filter)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->filter), ((CsrSize) (primitive->filterLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_REQ
void *CsrBtSdSearchReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdSearchReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdSearchReq *) CsrPmemZalloc(sizeof(CsrBtSdSearchReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->searchConfig, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->rssiBufferTime, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalSearchTime, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->rssiThreshold, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceClass, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceClassMask, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->inquiryAccessCode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->filterLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->filter), buffer, &offset, ((CsrSize) sizeof(primitive->filter)));
    CsrUint8Des((CsrUint8 *) &primitive->inquiryTxPowerLevel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->leRssiThreshold, buffer, &offset);
    if (primitive->filterLen)
    {
        primitive->filter = CsrPmemZalloc(((CsrUint32) (primitive->filterLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->filter), buffer, &offset, ((CsrSize) (primitive->filterLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->filter = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_REQ
void CsrBtSdSearchReqSerFree(void *msg)
{
    CsrBtSdSearchReq *primitive;

    primitive = (CsrBtSdSearchReq *) msg;

    if(primitive->filter != NULL)
    {
        CsrPmemFree(primitive->filter);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_SD_HOUSE_CLEANING
CsrSize CsrBtSdHouseCleaningSizeof(void *msg)
{
    CsrBtSdHouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SD_HOUSE_CLEANING
CsrUint8 *CsrBtSdHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdHouseCleaning *primitive;

    primitive = (CsrBtSdHouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SD_HOUSE_CLEANING
void *CsrBtSdHouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdHouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdHouseCleaning *) CsrPmemZalloc(sizeof(CsrBtSdHouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_REQ
CsrSize CsrBtSdMemoryConfigReqSizeof(void *msg)
{
    CsrBtSdMemoryConfigReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_REQ
CsrUint8 *CsrBtSdMemoryConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdMemoryConfigReq *primitive;

    primitive = (CsrBtSdMemoryConfigReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->memoryConfig);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceListMax);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceListInfoMax);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_REQ
void *CsrBtSdMemoryConfigReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdMemoryConfigReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdMemoryConfigReq *) CsrPmemZalloc(sizeof(CsrBtSdMemoryConfigReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->memoryConfig, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceListMax, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceListInfoMax, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CFM
CsrSize CsrBtSdReadServicesCfmSizeof(void *msg)
{
    CsrBtSdReadServicesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->listCount * sizeof(CsrBtUuid)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CFM
CsrUint8 *CsrBtSdReadServicesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdReadServicesCfm *primitive;

    primitive = (CsrBtSdReadServicesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceClass);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->listCount);
    CsrMemCpySer(buffer, length, &primitive->list, sizeof(primitive->list));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceStatus);
    if(primitive->list)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->list), ((CsrSize) (primitive->listCount * sizeof(CsrBtUuid))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CFM
void *CsrBtSdReadServicesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdReadServicesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdReadServicesCfm *) CsrPmemZalloc(sizeof(CsrBtSdReadServicesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceClass, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->listCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->list), buffer, &offset, ((CsrSize) sizeof(primitive->list)));
    CsrUint32Des((CsrUint32 *) &primitive->deviceStatus, buffer, &offset);
    if (primitive->listCount)
    {
        primitive->list = CsrPmemZalloc(((CsrUint32) (primitive->listCount * sizeof(CsrBtUuid))));
        CsrMemCpyDes(((void *) primitive->list), buffer, &offset, ((CsrSize) (primitive->listCount * sizeof(CsrBtUuid))));
    }
    else
    {
        primitive->list = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CFM
void CsrBtSdReadServicesCfmSerFree(void *msg)
{
    CsrBtSdReadServicesCfm *primitive;

    primitive = (CsrBtSdReadServicesCfm *) msg;

    if(primitive->list != NULL)
    {
        CsrPmemFree(primitive->list);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_REQ
CsrSize CsrBtSdSearchConfigReqSizeof(void *msg)
{
    CsrBtSdSearchConfigReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_REQ
CsrUint8 *CsrBtSdSearchConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdSearchConfigReq *primitive;

    primitive = (CsrBtSdSearchConfigReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->searchConfig);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->readNameTimeout);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->maxSearchResults);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_REQ
void *CsrBtSdSearchConfigReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdSearchConfigReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdSearchConfigReq *) CsrPmemZalloc(sizeof(CsrBtSdSearchConfigReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->searchConfig, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->readNameTimeout, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->maxSearchResults, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM
CsrSize CsrBtSdUnregisterServiceRecordCfmSizeof(void *msg)
{
    CsrBtSdUnregisterServiceRecordCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM
CsrUint8 *CsrBtSdUnregisterServiceRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdUnregisterServiceRecordCfm *primitive;

    primitive = (CsrBtSdUnregisterServiceRecordCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM
void *CsrBtSdUnregisterServiceRecordCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdUnregisterServiceRecordCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdUnregisterServiceRecordCfm *) CsrPmemZalloc(sizeof(CsrBtSdUnregisterServiceRecordCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ
CsrSize CsrBtSdCancelReadAvailableServicesReqSizeof(void *msg)
{
    CsrBtSdCancelReadAvailableServicesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ
CsrUint8 *CsrBtSdCancelReadAvailableServicesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdCancelReadAvailableServicesReq *primitive;

    primitive = (CsrBtSdCancelReadAvailableServicesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ
void *CsrBtSdCancelReadAvailableServicesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdCancelReadAvailableServicesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdCancelReadAvailableServicesReq *) CsrPmemZalloc(sizeof(CsrBtSdCancelReadAvailableServicesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_READ_AVAILABLE_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_CFM
CsrSize CsrBtSdReadServiceRecordCfmSizeof(void *msg)
{
    CsrBtSdReadServiceRecordCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_CFM
CsrUint8 *CsrBtSdReadServiceRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdReadServiceRecordCfm *primitive;

    primitive = (CsrBtSdReadServiceRecordCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_CFM
void *CsrBtSdReadServiceRecordCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdReadServiceRecordCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdReadServiceRecordCfm *) CsrPmemZalloc(sizeof(CsrBtSdReadServiceRecordCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_REQ
CsrSize CsrBtSdReadDeviceListReqSizeof(void *msg)
{
    CsrBtSdReadDeviceListReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_REQ
CsrUint8 *CsrBtSdReadDeviceListReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdReadDeviceListReq *primitive;

    primitive = (CsrBtSdReadDeviceListReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->maxNumOfBytesInEachResult);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceListConfig);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_REQ
void *CsrBtSdReadDeviceListReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdReadDeviceListReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdReadDeviceListReq *) CsrPmemZalloc(sizeof(CsrBtSdReadDeviceListReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->maxNumOfBytesInEachResult, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceListConfig, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_CFM
CsrSize CsrBtSdReadServicesCancelCfmSizeof(void *msg)
{
    CsrBtSdReadServicesCancelCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_CFM
CsrUint8 *CsrBtSdReadServicesCancelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdReadServicesCancelCfm *primitive;

    primitive = (CsrBtSdReadServicesCancelCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_CFM
void *CsrBtSdReadServicesCancelCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdReadServicesCancelCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdReadServicesCancelCfm *) CsrPmemZalloc(sizeof(CsrBtSdReadServicesCancelCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICES_CANCEL_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_REQ
CsrSize CsrBtSdReadServiceRecordReqSizeof(void *msg)
{
    CsrBtSdReadServiceRecordReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_REQ
CsrUint8 *CsrBtSdReadServiceRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdReadServiceRecordReq *primitive;

    primitive = (CsrBtSdReadServiceRecordReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLen);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_REQ
void *CsrBtSdReadServiceRecordReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdReadServiceRecordReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdReadServiceRecordReq *) CsrPmemZalloc(sizeof(CsrBtSdReadServiceRecordReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->flags, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_REQ
void CsrBtSdReadServiceRecordReqSerFree(void *msg)
{
    CsrBtSdReadServiceRecordReq *primitive;

    primitive = (CsrBtSdReadServiceRecordReq *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SD_READ_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM
CsrSize CsrBtSdReadAvailableServicesCfmSizeof(void *msg)
{
    CsrBtSdReadAvailableServicesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->infoLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM
CsrUint8 *CsrBtSdReadAvailableServicesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdReadAvailableServicesCfm *primitive;

    primitive = (CsrBtSdReadAvailableServicesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceClass);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->infoLen);
    CsrMemCpySer(buffer, length, &primitive->info, sizeof(primitive->info));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceStatus);
    if(primitive->info)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->info), ((CsrSize) (primitive->infoLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM
void *CsrBtSdReadAvailableServicesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdReadAvailableServicesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdReadAvailableServicesCfm *) CsrPmemZalloc(sizeof(CsrBtSdReadAvailableServicesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceClass, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->infoLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->info), buffer, &offset, ((CsrSize) sizeof(primitive->info)));
    CsrUint32Des((CsrUint32 *) &primitive->deviceStatus, buffer, &offset);
    if (primitive->infoLen)
    {
        primitive->info = CsrPmemZalloc(((CsrUint32) (primitive->infoLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->info), buffer, &offset, ((CsrSize) (primitive->infoLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->info = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM
void CsrBtSdReadAvailableServicesCfmSerFree(void *msg)
{
    CsrBtSdReadAvailableServicesCfm *primitive;

    primitive = (CsrBtSdReadAvailableServicesCfm *) msg;

    if(primitive->info != NULL)
    {
        CsrPmemFree(primitive->info);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_CFM */

#ifndef EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ
CsrSize CsrBtSdUnregisterServiceRecordReqSizeof(void *msg)
{
    CsrBtSdUnregisterServiceRecordReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ
CsrUint8 *CsrBtSdUnregisterServiceRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdUnregisterServiceRecordReq *primitive;

    primitive = (CsrBtSdUnregisterServiceRecordReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->flags);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ
void *CsrBtSdUnregisterServiceRecordReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdUnregisterServiceRecordReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdUnregisterServiceRecordReq *) CsrPmemZalloc(sizeof(CsrBtSdUnregisterServiceRecordReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->flags, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_UNREGISTER_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_SYNCHRONIZE_REQ
CsrSize CsrBtSdSynchronizeReqSizeof(void *msg)
{
    CsrBtSdSynchronizeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_SYNCHRONIZE_REQ */

#ifndef EXCLUDE_CSR_BT_SD_SYNCHRONIZE_REQ
CsrUint8 *CsrBtSdSynchronizeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdSynchronizeReq *primitive;

    primitive = (CsrBtSdSynchronizeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_SYNCHRONIZE_REQ */

#ifndef EXCLUDE_CSR_BT_SD_SYNCHRONIZE_REQ
void *CsrBtSdSynchronizeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdSynchronizeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdSynchronizeReq *) CsrPmemZalloc(sizeof(CsrBtSdSynchronizeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_SYNCHRONIZE_REQ */

#ifndef EXCLUDE_CSR_BT_SD_SYNCHRONIZE_CFM
CsrSize CsrBtSdSynchronizeCfmSizeof(void *msg)
{
    CsrBtSdSynchronizeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_SYNCHRONIZE_CFM */

#ifndef EXCLUDE_CSR_BT_SD_SYNCHRONIZE_CFM
CsrUint8 *CsrBtSdSynchronizeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdSynchronizeCfm *primitive;

    primitive = (CsrBtSdSynchronizeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalNumOfDevices);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_SYNCHRONIZE_CFM */

#ifndef EXCLUDE_CSR_BT_SD_SYNCHRONIZE_CFM
void *CsrBtSdSynchronizeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdSynchronizeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdSynchronizeCfm *) CsrPmemZalloc(sizeof(CsrBtSdSynchronizeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalNumOfDevices, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_SYNCHRONIZE_CFM */

#ifndef EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_CFM
CsrSize CsrBtSdMemoryConfigCfmSizeof(void *msg)
{
    CsrBtSdMemoryConfigCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_CFM
CsrUint8 *CsrBtSdMemoryConfigCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdMemoryConfigCfm *primitive;

    primitive = (CsrBtSdMemoryConfigCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->memoryConfig);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceListMax);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceListInfoMax);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_CFM
void *CsrBtSdMemoryConfigCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdMemoryConfigCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdMemoryConfigCfm *) CsrPmemZalloc(sizeof(CsrBtSdMemoryConfigCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->memoryConfig, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceListMax, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceListInfoMax, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_MEMORY_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_CFM
CsrSize CsrBtSdReadDeviceInfoCfmSizeof(void *msg)
{
    CsrBtSdReadDeviceInfoCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->infoLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_CFM
CsrUint8 *CsrBtSdReadDeviceInfoCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdReadDeviceInfoCfm *primitive;

    primitive = (CsrBtSdReadDeviceInfoCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceClass);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->infoLen);
    CsrMemCpySer(buffer, length, &primitive->info, sizeof(primitive->info));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceStatus);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceChangedHandle);
    if(primitive->info)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->info), ((CsrSize) (primitive->infoLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_CFM
void *CsrBtSdReadDeviceInfoCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdReadDeviceInfoCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdReadDeviceInfoCfm *) CsrPmemZalloc(sizeof(CsrBtSdReadDeviceInfoCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceClass, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->infoLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->info), buffer, &offset, ((CsrSize) sizeof(primitive->info)));
    CsrUint32Des((CsrUint32 *) &primitive->deviceStatus, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceChangedHandle, buffer, &offset);
    if (primitive->infoLen)
    {
        primitive->info = CsrPmemZalloc(((CsrUint32) (primitive->infoLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->info), buffer, &offset, ((CsrSize) (primitive->infoLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->info = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_CFM
void CsrBtSdReadDeviceInfoCfmSerFree(void *msg)
{
    CsrBtSdReadDeviceInfoCfm *primitive;

    primitive = (CsrBtSdReadDeviceInfoCfm *) msg;

    if(primitive->info != NULL)
    {
        CsrPmemFree(primitive->info);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SD_READ_DEVICE_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ
CsrSize CsrBtSdCancelReadServiceRecordReqSizeof(void *msg)
{
    CsrBtSdCancelReadServiceRecordReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ
CsrUint8 *CsrBtSdCancelReadServiceRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdCancelReadServiceRecordReq *primitive;

    primitive = (CsrBtSdCancelReadServiceRecordReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ
void *CsrBtSdCancelReadServiceRecordReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdCancelReadServiceRecordReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdCancelReadServiceRecordReq *) CsrPmemZalloc(sizeof(CsrBtSdCancelReadServiceRecordReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_CANCEL_READ_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ
CsrSize CsrBtSdReadAvailableServicesReqSizeof(void *msg)
{
    CsrBtSdReadAvailableServicesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->filterLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ
CsrUint8 *CsrBtSdReadAvailableServicesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdReadAvailableServicesReq *primitive;

    primitive = (CsrBtSdReadAvailableServicesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceConfig);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->filterLen);
    CsrMemCpySer(buffer, length, &primitive->filter, sizeof(primitive->filter));
    if(primitive->filter)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->filter), ((CsrSize) (primitive->filterLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ
void *CsrBtSdReadAvailableServicesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdReadAvailableServicesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdReadAvailableServicesReq *) CsrPmemZalloc(sizeof(CsrBtSdReadAvailableServicesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceConfig, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->filterLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->filter), buffer, &offset, ((CsrSize) sizeof(primitive->filter)));
    if (primitive->filterLen)
    {
        primitive->filter = CsrPmemZalloc(((CsrUint32) (primitive->filterLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->filter), buffer, &offset, ((CsrSize) (primitive->filterLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->filter = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ
void CsrBtSdReadAvailableServicesReqSerFree(void *msg)
{
    CsrBtSdReadAvailableServicesReq *primitive;

    primitive = (CsrBtSdReadAvailableServicesReq *) msg;

    if(primitive->filter != NULL)
    {
        CsrPmemFree(primitive->filter);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SD_READ_AVAILABLE_SERVICES_REQ */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_CFM
CsrSize CsrBtSdSearchConfigCfmSizeof(void *msg)
{
    CsrBtSdSearchConfigCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_CFM
CsrUint8 *CsrBtSdSearchConfigCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdSearchConfigCfm *primitive;

    primitive = (CsrBtSdSearchConfigCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->searchConfig);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->readNameTimeout);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->maxSearchResults);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_CFM
void *CsrBtSdSearchConfigCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdSearchConfigCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdSearchConfigCfm *) CsrPmemZalloc(sizeof(CsrBtSdSearchConfigCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->searchConfig, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->readNameTimeout, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->maxSearchResults, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_SEARCH_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_SD_UPDATE_DEVICE_REQ
CsrSize CsrBtSdUpdateDeviceReqSizeof(void *msg)
{
    CsrBtSdUpdateDeviceReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_UPDATE_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_SD_UPDATE_DEVICE_REQ
CsrUint8 *CsrBtSdUpdateDeviceReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdUpdateDeviceReq *primitive;

    primitive = (CsrBtSdUpdateDeviceReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->cod);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->details);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceChangeHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_UPDATE_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_SD_UPDATE_DEVICE_REQ
void *CsrBtSdUpdateDeviceReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdUpdateDeviceReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdUpdateDeviceReq *) CsrPmemZalloc(sizeof(CsrBtSdUpdateDeviceReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->cod, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->details, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceChangeHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_UPDATE_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ
CsrSize CsrBtSdRegisterAvailableServiceReqSizeof(void *msg)
{
    CsrBtSdRegisterAvailableServiceReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ */

#ifndef EXCLUDE_CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ
CsrUint8 *CsrBtSdRegisterAvailableServiceReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdRegisterAvailableServiceReq *primitive;

    primitive = (CsrBtSdRegisterAvailableServiceReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->service);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ */

#ifndef EXCLUDE_CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ
void *CsrBtSdRegisterAvailableServiceReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdRegisterAvailableServiceReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdRegisterAvailableServiceReq *) CsrPmemZalloc(sizeof(CsrBtSdRegisterAvailableServiceReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->service, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_AVAILABLE_SERVICE_REQ */

#ifndef EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ
CsrSize CsrBtSdRegisterServiceRecordReqSizeof(void *msg)
{
    CsrBtSdRegisterServiceRecordReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->dataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ
CsrUint8 *CsrBtSdRegisterServiceRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdRegisterServiceRecordReq *primitive;

    primitive = (CsrBtSdRegisterServiceRecordReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataLen);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->dataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ
void *CsrBtSdRegisterServiceRecordReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdRegisterServiceRecordReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdRegisterServiceRecordReq *) CsrPmemZalloc(sizeof(CsrBtSdRegisterServiceRecordReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->flags, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ
void CsrBtSdRegisterServiceRecordReqSerFree(void *msg)
{
    CsrBtSdRegisterServiceRecordReq *primitive;

    primitive = (CsrBtSdRegisterServiceRecordReq *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SD_REGISTER_SERVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SD_CLOSE_SEARCH_IND
CsrSize CsrBtSdCloseSearchIndSizeof(void *msg)
{
    CsrBtSdCloseSearchInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SD_CLOSE_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_SD_CLOSE_SEARCH_IND
CsrUint8 *CsrBtSdCloseSearchIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtSdCloseSearchInd *primitive;

    primitive = (CsrBtSdCloseSearchInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SD_CLOSE_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_SD_CLOSE_SEARCH_IND
void *CsrBtSdCloseSearchIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtSdCloseSearchInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtSdCloseSearchInd *) CsrPmemZalloc(sizeof(CsrBtSdCloseSearchInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SD_CLOSE_SEARCH_IND */

#endif /* EXCLUDE_CSR_BT_SD_PRIVATE_MODULE */
