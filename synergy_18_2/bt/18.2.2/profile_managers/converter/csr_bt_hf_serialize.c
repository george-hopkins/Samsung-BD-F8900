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
#include "csr_bt_hf_prim.h"

#ifndef EXCLUDE_CSR_BT_HF_MODULE

void CsrBtHfPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_CFM
CsrSize CsrBtHfServiceConnectCfmSizeof(void *msg)
{
    CsrBtHfServiceConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->serviceName) + CsrCharStringSerLen((CsrCharString *) primitive->indicatorSupported) + CsrCharStringSerLen((CsrCharString *) primitive->indicatorValue) + CsrCharStringSerLen((CsrCharString *) primitive->chldString));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_CFM
CsrUint8 *CsrBtHfServiceConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfServiceConnectCfm *primitive;

    primitive = (CsrBtHfServiceConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->supportedFeatures);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->network);
    CsrMemCpySer(buffer, length, &primitive->serviceName, sizeof(primitive->serviceName));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionType);
    CsrMemCpySer(buffer, length, &primitive->indicatorSupported, sizeof(primitive->indicatorSupported));
    CsrMemCpySer(buffer, length, &primitive->indicatorValue, sizeof(primitive->indicatorValue));
    CsrMemCpySer(buffer, length, &primitive->chldString, sizeof(primitive->chldString));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remoteVersion);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrCharStringSer(buffer, length, primitive->serviceName);
    CsrCharStringSer(buffer, length, primitive->indicatorSupported);
    CsrCharStringSer(buffer, length, primitive->indicatorValue);
    CsrCharStringSer(buffer, length, primitive->chldString);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_CFM
void *CsrBtHfServiceConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfServiceConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfServiceConnectCfm *) CsrPmemZalloc(sizeof(CsrBtHfServiceConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->supportedFeatures, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->network, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->serviceName)));
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionType, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->indicatorSupported), buffer, &offset, ((CsrSize) sizeof(primitive->indicatorSupported)));
    CsrMemCpyDes(((void *) &primitive->indicatorValue), buffer, &offset, ((CsrSize) sizeof(primitive->indicatorValue)));
    CsrMemCpyDes(((void *) &primitive->chldString), buffer, &offset, ((CsrSize) sizeof(primitive->chldString)));
    CsrUint16Des((CsrUint16 *) &primitive->remoteVersion, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrCharStringDes(&primitive->serviceName, buffer, &offset);
    CsrCharStringDes(&primitive->indicatorSupported, buffer, &offset);
    CsrCharStringDes(&primitive->indicatorValue, buffer, &offset);
    CsrCharStringDes(&primitive->chldString, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_CFM
void CsrBtHfServiceConnectCfmSerFree(void *msg)
{
    CsrBtHfServiceConnectCfm *primitive;

    primitive = (CsrBtHfServiceConnectCfm *) msg;

    if(primitive->serviceName != NULL)
    {
        CsrPmemFree(primitive->serviceName);
    }
    if(primitive->indicatorSupported != NULL)
    {
        CsrPmemFree(primitive->indicatorSupported);
    }
    if(primitive->indicatorValue != NULL)
    {
        CsrPmemFree(primitive->indicatorValue);
    }
    if(primitive->chldString != NULL)
    {
        CsrPmemFree(primitive->chldString);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_REQ
CsrSize CsrBtHfIndicatorActivationReqSizeof(void *msg)
{
    CsrBtHfIndicatorActivationReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_REQ
CsrUint8 *CsrBtHfIndicatorActivationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfIndicatorActivationReq *primitive;

    primitive = (CsrBtHfIndicatorActivationReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->indicatorBitMask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_REQ
void *CsrBtHfIndicatorActivationReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfIndicatorActivationReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfIndicatorActivationReq *) CsrPmemZalloc(sizeof(CsrBtHfIndicatorActivationReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->indicatorBitMask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DEACTIVATE_CFM
CsrSize CsrBtHfDeactivateCfmSizeof(void *msg)
{
    CsrBtHfDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_DEACTIVATE_CFM
CsrUint8 *CsrBtHfDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfDeactivateCfm *primitive;

    primitive = (CsrBtHfDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_DEACTIVATE_CFM
void *CsrBtHfDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtHfDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_IND
CsrSize CsrBtHfServiceConnectIndSizeof(void *msg)
{
    CsrBtHfServiceConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->serviceName) + CsrCharStringSerLen((CsrCharString *) primitive->indicatorSupported) + CsrCharStringSerLen((CsrCharString *) primitive->indicatorValue) + CsrCharStringSerLen((CsrCharString *) primitive->chldString));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_IND
CsrUint8 *CsrBtHfServiceConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfServiceConnectInd *primitive;

    primitive = (CsrBtHfServiceConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->supportedFeatures);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->network);
    CsrMemCpySer(buffer, length, &primitive->serviceName, sizeof(primitive->serviceName));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionType);
    CsrMemCpySer(buffer, length, &primitive->indicatorSupported, sizeof(primitive->indicatorSupported));
    CsrMemCpySer(buffer, length, &primitive->indicatorValue, sizeof(primitive->indicatorValue));
    CsrMemCpySer(buffer, length, &primitive->chldString, sizeof(primitive->chldString));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remoteVersion);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrCharStringSer(buffer, length, primitive->serviceName);
    CsrCharStringSer(buffer, length, primitive->indicatorSupported);
    CsrCharStringSer(buffer, length, primitive->indicatorValue);
    CsrCharStringSer(buffer, length, primitive->chldString);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_IND
void *CsrBtHfServiceConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfServiceConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfServiceConnectInd *) CsrPmemZalloc(sizeof(CsrBtHfServiceConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->supportedFeatures, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->network, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->serviceName)));
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionType, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->indicatorSupported), buffer, &offset, ((CsrSize) sizeof(primitive->indicatorSupported)));
    CsrMemCpyDes(((void *) &primitive->indicatorValue), buffer, &offset, ((CsrSize) sizeof(primitive->indicatorValue)));
    CsrMemCpyDes(((void *) &primitive->chldString), buffer, &offset, ((CsrSize) sizeof(primitive->chldString)));
    CsrUint16Des((CsrUint16 *) &primitive->remoteVersion, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrCharStringDes(&primitive->serviceName, buffer, &offset);
    CsrCharStringDes(&primitive->indicatorSupported, buffer, &offset);
    CsrCharStringDes(&primitive->indicatorValue, buffer, &offset);
    CsrCharStringDes(&primitive->chldString, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_IND
void CsrBtHfServiceConnectIndSerFree(void *msg)
{
    CsrBtHfServiceConnectInd *primitive;

    primitive = (CsrBtHfServiceConnectInd *) msg;

    if(primitive->serviceName != NULL)
    {
        CsrPmemFree(primitive->serviceName);
    }
    if(primitive->indicatorSupported != NULL)
    {
        CsrPmemFree(primitive->indicatorSupported);
    }
    if(primitive->indicatorValue != NULL)
    {
        CsrPmemFree(primitive->indicatorValue);
    }
    if(primitive->chldString != NULL)
    {
        CsrPmemFree(primitive->chldString);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ
CsrSize CsrBtHfGetAllStatusIndicatorsReqSizeof(void *msg)
{
    CsrBtHfGetAllStatusIndicatorsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ
CsrUint8 *CsrBtHfGetAllStatusIndicatorsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfGetAllStatusIndicatorsReq *primitive;

    primitive = (CsrBtHfGetAllStatusIndicatorsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ
void *CsrBtHfGetAllStatusIndicatorsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfGetAllStatusIndicatorsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfGetAllStatusIndicatorsReq *) CsrPmemZalloc(sizeof(CsrBtHfGetAllStatusIndicatorsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM
CsrSize CsrBtHfSetCallWaitingNotificationCfmSizeof(void *msg)
{
    CsrBtHfSetCallWaitingNotificationCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM
CsrUint8 *CsrBtHfSetCallWaitingNotificationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetCallWaitingNotificationCfm *primitive;

    primitive = (CsrBtHfSetCallWaitingNotificationCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM
void *CsrBtHfSetCallWaitingNotificationCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetCallWaitingNotificationCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetCallWaitingNotificationCfm *) CsrPmemZalloc(sizeof(CsrBtHfSetCallWaitingNotificationCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GENERATE_DTMF_REQ
CsrSize CsrBtHfGenerateDtmfReqSizeof(void *msg)
{
    CsrBtHfGenerateDtmfReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_GENERATE_DTMF_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GENERATE_DTMF_REQ
CsrUint8 *CsrBtHfGenerateDtmfReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfGenerateDtmfReq *primitive;

    primitive = (CsrBtHfGenerateDtmfReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->dtmf);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_GENERATE_DTMF_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GENERATE_DTMF_REQ
void *CsrBtHfGenerateDtmfReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfGenerateDtmfReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfGenerateDtmfReq *) CsrPmemZalloc(sizeof(CsrBtHfGenerateDtmfReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->dtmf, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_GENERATE_DTMF_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ
CsrSize CsrBtHfSetExtendedAgErrorResultCodeReqSizeof(void *msg)
{
    CsrBtHfSetExtendedAgErrorResultCodeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ
CsrUint8 *CsrBtHfSetExtendedAgErrorResultCodeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetExtendedAgErrorResultCodeReq *primitive;

    primitive = (CsrBtHfSetExtendedAgErrorResultCodeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enable);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ
void *CsrBtHfSetExtendedAgErrorResultCodeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetExtendedAgErrorResultCodeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetExtendedAgErrorResultCodeReq *) CsrPmemZalloc(sizeof(CsrBtHfSetExtendedAgErrorResultCodeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_IND
CsrSize CsrBtHfSetVoiceRecognitionIndSizeof(void *msg)
{
    CsrBtHfSetVoiceRecognitionInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_IND */

#ifndef EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_IND
CsrUint8 *CsrBtHfSetVoiceRecognitionIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetVoiceRecognitionInd *primitive;

    primitive = (CsrBtHfSetVoiceRecognitionInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->started);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_IND */

#ifndef EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_IND
void *CsrBtHfSetVoiceRecognitionIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetVoiceRecognitionInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetVoiceRecognitionInd *) CsrPmemZalloc(sizeof(CsrBtHfSetVoiceRecognitionInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->started, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_IND */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_IND
CsrSize CsrBtHfAudioDisconnectIndSizeof(void *msg)
{
    CsrBtHfAudioDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_IND
CsrUint8 *CsrBtHfAudioDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfAudioDisconnectInd *primitive;

    primitive = (CsrBtHfAudioDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_IND
void *CsrBtHfAudioDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfAudioDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfAudioDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtHfAudioDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND
CsrSize CsrBtHfStatusIndicatorUpdateIndSizeof(void *msg)
{
    CsrBtHfStatusIndicatorUpdateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->name));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND */

#ifndef EXCLUDE_CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND
CsrUint8 *CsrBtHfStatusIndicatorUpdateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfStatusIndicatorUpdateInd *primitive;

    primitive = (CsrBtHfStatusIndicatorUpdateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->index);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);
    CsrMemCpySer(buffer, length, &primitive->name, sizeof(primitive->name));
    CsrCharStringSer(buffer, length, primitive->name);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND */

#ifndef EXCLUDE_CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND
void *CsrBtHfStatusIndicatorUpdateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfStatusIndicatorUpdateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfStatusIndicatorUpdateInd *) CsrPmemZalloc(sizeof(CsrBtHfStatusIndicatorUpdateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->index, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->name), buffer, &offset, ((CsrSize) sizeof(primitive->name)));
    CsrCharStringDes(&primitive->name, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND */

#ifndef EXCLUDE_CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND
void CsrBtHfStatusIndicatorUpdateIndSerFree(void *msg)
{
    CsrBtHfStatusIndicatorUpdateInd *primitive;

    primitive = (CsrBtHfStatusIndicatorUpdateInd *) msg;

    if(primitive->name != NULL)
    {
        CsrPmemFree(primitive->name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_STATUS_INDICATOR_UPDATE_IND */

#ifndef EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_REQ
CsrSize CsrBtHfMicGainStatusReqSizeof(void *msg)
{
    CsrBtHfMicGainStatusReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_REQ
CsrUint8 *CsrBtHfMicGainStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfMicGainStatusReq *primitive;

    primitive = (CsrBtHfMicGainStatusReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->gain);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_REQ
void *CsrBtHfMicGainStatusReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfMicGainStatusReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfMicGainStatusReq *) CsrPmemZalloc(sizeof(CsrBtHfMicGainStatusReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->gain, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM
CsrSize CsrBtHfSetCallNotificationIndicationCfmSizeof(void *msg)
{
    CsrBtHfSetCallNotificationIndicationCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM
CsrUint8 *CsrBtHfSetCallNotificationIndicationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetCallNotificationIndicationCfm *primitive;

    primitive = (CsrBtHfSetCallNotificationIndicationCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM
void *CsrBtHfSetCallNotificationIndicationCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetCallNotificationIndicationCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetCallNotificationIndicationCfm *) CsrPmemZalloc(sizeof(CsrBtHfSetCallNotificationIndicationCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_MIC_GAIN_IND
CsrSize CsrBtHfMicGainIndSizeof(void *msg)
{
    CsrBtHfMicGainInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_MIC_GAIN_IND */

#ifndef EXCLUDE_CSR_BT_HF_MIC_GAIN_IND
CsrUint8 *CsrBtHfMicGainIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfMicGainInd *primitive;

    primitive = (CsrBtHfMicGainInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->gain);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_MIC_GAIN_IND */

#ifndef EXCLUDE_CSR_BT_HF_MIC_GAIN_IND
void *CsrBtHfMicGainIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfMicGainInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfMicGainInd *) CsrPmemZalloc(sizeof(CsrBtHfMicGainInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->gain, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_MIC_GAIN_IND */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_REQ
CsrSize CsrBtHfAudioDisconnectReqSizeof(void *msg)
{
    CsrBtHfAudioDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_REQ
CsrUint8 *CsrBtHfAudioDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfAudioDisconnectReq *primitive;

    primitive = (CsrBtHfAudioDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_REQ
void *CsrBtHfAudioDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfAudioDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfAudioDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtHfAudioDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_REQ
CsrSize CsrBtHfAtCmdReqSizeof(void *msg)
{
    CsrBtHfAtCmdReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->atCmdString));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_REQ
CsrUint8 *CsrBtHfAtCmdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfAtCmdReq *primitive;

    primitive = (CsrBtHfAtCmdReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrMemCpySer(buffer, length, &primitive->atCmdString, sizeof(primitive->atCmdString));
    CsrCharStringSer(buffer, length, primitive->atCmdString);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_REQ
void *CsrBtHfAtCmdReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfAtCmdReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfAtCmdReq *) CsrPmemZalloc(sizeof(CsrBtHfAtCmdReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->atCmdString), buffer, &offset, ((CsrSize) sizeof(primitive->atCmdString)));
    CsrCharStringDes(&primitive->atCmdString, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_REQ
void CsrBtHfAtCmdReqSerFree(void *msg)
{
    CsrBtHfAtCmdReq *primitive;

    primitive = (CsrBtHfAtCmdReq *) msg;

    if(primitive->atCmdString != NULL)
    {
        CsrPmemFree(primitive->atCmdString);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CALL_HANDLING_CFM
CsrSize CsrBtHfCallHandlingCfmSizeof(void *msg)
{
    CsrBtHfCallHandlingCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_HANDLING_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CALL_HANDLING_CFM
CsrUint8 *CsrBtHfCallHandlingCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfCallHandlingCfm *primitive;

    primitive = (CsrBtHfCallHandlingCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_HANDLING_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CALL_HANDLING_CFM
void *CsrBtHfCallHandlingCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfCallHandlingCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfCallHandlingCfm *) CsrPmemZalloc(sizeof(CsrBtHfCallHandlingCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_HANDLING_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_REQ
CsrSize CsrBtHfConfigLowPowerReqSizeof(void *msg)
{
    CsrBtHfConfigLowPowerReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_REQ
CsrUint8 *CsrBtHfConfigLowPowerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfConfigLowPowerReq *primitive;

    primitive = (CsrBtHfConfigLowPowerReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->mask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_REQ
void *CsrBtHfConfigLowPowerReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfConfigLowPowerReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfConfigLowPowerReq *) CsrPmemZalloc(sizeof(CsrBtHfConfigLowPowerReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->mask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_REQ */

#ifndef EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_CFM
CsrSize CsrBtHfMicGainStatusCfmSizeof(void *msg)
{
    CsrBtHfMicGainStatusCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_CFM
CsrUint8 *CsrBtHfMicGainStatusCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfMicGainStatusCfm *primitive;

    primitive = (CsrBtHfMicGainStatusCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_CFM
void *CsrBtHfMicGainStatusCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfMicGainStatusCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfMicGainStatusCfm *) CsrPmemZalloc(sizeof(CsrBtHfMicGainStatusCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_MIC_GAIN_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_C2C_SF_REQ
CsrSize CsrBtHfC2cSfReqSizeof(void *msg)
{
    CsrBtHfC2cSfReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_SF_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_SF_REQ
CsrUint8 *CsrBtHfC2cSfReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfC2cSfReq *primitive;

    primitive = (CsrBtHfC2cSfReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->number);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_SF_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_SF_REQ
void *CsrBtHfC2cSfReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfC2cSfReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfC2cSfReq *) CsrPmemZalloc(sizeof(CsrBtHfC2cSfReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->number, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_SF_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_REQ
CsrSize CsrBtHfDeregisterTimeReqSizeof(void *msg)
{
    CsrBtHfDeregisterTimeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_REQ
CsrUint8 *CsrBtHfDeregisterTimeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfDeregisterTimeReq *primitive;

    primitive = (CsrBtHfDeregisterTimeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->waitSeconds);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_REQ
void *CsrBtHfDeregisterTimeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfDeregisterTimeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfDeregisterTimeReq *) CsrPmemZalloc(sizeof(CsrBtHfDeregisterTimeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->waitSeconds, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_IND
CsrSize CsrBtHfAudioConnectIndSizeof(void *msg)
{
    CsrBtHfAudioConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_IND
CsrUint8 *CsrBtHfAudioConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfAudioConnectInd *primitive;

    primitive = (CsrBtHfAudioConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmSlot);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->linkType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->txInterval);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->weSco);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rxPacketLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->txPacketLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->airMode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->codecUsed);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sampleRate);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_IND
void *CsrBtHfAudioConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfAudioConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfAudioConnectInd *) CsrPmemZalloc(sizeof(CsrBtHfAudioConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmSlot, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->linkType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->txInterval, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->weSco, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rxPacketLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->txPacketLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->airMode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->codecUsed, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->sampleRate, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_CALL_RINGING_IND
CsrSize CsrBtHfCallRingingIndSizeof(void *msg)
{
    CsrBtHfCallRingingInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_RINGING_IND */

#ifndef EXCLUDE_CSR_BT_HF_CALL_RINGING_IND
CsrUint8 *CsrBtHfCallRingingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfCallRingingInd *primitive;

    primitive = (CsrBtHfCallRingingInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_RINGING_IND */

#ifndef EXCLUDE_CSR_BT_HF_CALL_RINGING_IND
void *CsrBtHfCallRingingIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfCallRingingInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfCallRingingInd *) CsrPmemZalloc(sizeof(CsrBtHfCallRingingInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_RINGING_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_BATT_IND
CsrSize CsrBtHfC2cBattIndSizeof(void *msg)
{
    CsrBtHfC2cBattInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_BATT_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_BATT_IND
CsrUint8 *CsrBtHfC2cBattIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfC2cBattInd *primitive;

    primitive = (CsrBtHfC2cBattInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_BATT_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_BATT_IND
void *CsrBtHfC2cBattIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfC2cBattInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfC2cBattInd *) CsrPmemZalloc(sizeof(CsrBtHfC2cBattInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_BATT_IND */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_CFM
CsrSize CsrBtHfConfigAudioCfmSizeof(void *msg)
{
    CsrBtHfConfigAudioCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_CFM
CsrUint8 *CsrBtHfConfigAudioCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfConfigAudioCfm *primitive;

    primitive = (CsrBtHfConfigAudioCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_CFM
void *CsrBtHfConfigAudioCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfConfigAudioCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfConfigAudioCfm *) CsrPmemZalloc(sizeof(CsrBtHfConfigAudioCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ
CsrSize CsrBtHfSetStatusIndicatorUpdateReqSizeof(void *msg)
{
    CsrBtHfSetStatusIndicatorUpdateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ
CsrUint8 *CsrBtHfSetStatusIndicatorUpdateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetStatusIndicatorUpdateReq *primitive;

    primitive = (CsrBtHfSetStatusIndicatorUpdateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enable);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ
void *CsrBtHfSetStatusIndicatorUpdateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetStatusIndicatorUpdateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetStatusIndicatorUpdateReq *) CsrPmemZalloc(sizeof(CsrBtHfSetStatusIndicatorUpdateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SECURITY_OUT_REQ
CsrSize CsrBtHfSecurityOutReqSizeof(void *msg)
{
    CsrBtHfSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SECURITY_OUT_REQ
CsrUint8 *CsrBtHfSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSecurityOutReq *primitive;

    primitive = (CsrBtHfSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SECURITY_OUT_REQ
void *CsrBtHfSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtHfSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_SF_IND
CsrSize CsrBtHfC2cSfIndSizeof(void *msg)
{
    CsrBtHfC2cSfInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->indicatorsLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_SF_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_SF_IND
CsrUint8 *CsrBtHfC2cSfIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfC2cSfInd *primitive;

    primitive = (CsrBtHfC2cSfInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrMemCpySer(buffer, length, &primitive->indicators, sizeof(primitive->indicators));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->indicatorsLength);
    if(primitive->indicators)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->indicators), ((CsrSize) (primitive->indicatorsLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_SF_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_SF_IND
void *CsrBtHfC2cSfIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfC2cSfInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfC2cSfInd *) CsrPmemZalloc(sizeof(CsrBtHfC2cSfInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->indicators), buffer, &offset, ((CsrSize) sizeof(primitive->indicators)));
    CsrUint16Des((CsrUint16 *) &primitive->indicatorsLength, buffer, &offset);
    if (primitive->indicatorsLength)
    {
        primitive->indicators = CsrPmemZalloc(((CsrUint32) (primitive->indicatorsLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->indicators), buffer, &offset, ((CsrSize) (primitive->indicatorsLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->indicators = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_SF_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_SF_IND
void CsrBtHfC2cSfIndSerFree(void *msg)
{
    CsrBtHfC2cSfInd *primitive;

    primitive = (CsrBtHfC2cSfInd *) msg;

    if(primitive->indicators != NULL)
    {
        CsrPmemFree(primitive->indicators);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_SF_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_GET_SMS_CFM
CsrSize CsrBtHfC2cGetSmsCfmSizeof(void *msg)
{
    CsrBtHfC2cGetSmsCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->smsString));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_GET_SMS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_C2C_GET_SMS_CFM
CsrUint8 *CsrBtHfC2cGetSmsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfC2cGetSmsCfm *primitive;

    primitive = (CsrBtHfC2cGetSmsCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);
    CsrMemCpySer(buffer, length, &primitive->smsString, sizeof(primitive->smsString));
    CsrCharStringSer(buffer, length, primitive->smsString);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_GET_SMS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_C2C_GET_SMS_CFM
void *CsrBtHfC2cGetSmsCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfC2cGetSmsCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfC2cGetSmsCfm *) CsrPmemZalloc(sizeof(CsrBtHfC2cGetSmsCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->smsString), buffer, &offset, ((CsrSize) sizeof(primitive->smsString)));
    CsrCharStringDes(&primitive->smsString, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_GET_SMS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_C2C_GET_SMS_CFM
void CsrBtHfC2cGetSmsCfmSerFree(void *msg)
{
    CsrBtHfC2cGetSmsCfm *primitive;

    primitive = (CsrBtHfC2cGetSmsCfm *) msg;

    if(primitive->smsString != NULL)
    {
        CsrPmemFree(primitive->smsString);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_GET_SMS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_IND
CsrSize CsrBtHfSetC2cSampleRateIndSizeof(void *msg)
{
    CsrBtHfSetC2cSampleRateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_IND */

#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_IND
CsrUint8 *CsrBtHfSetC2cSampleRateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetC2cSampleRateInd *primitive;

    primitive = (CsrBtHfSetC2cSampleRateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_IND */

#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_IND
void *CsrBtHfSetC2cSampleRateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetC2cSampleRateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetC2cSampleRateInd *) CsrPmemZalloc(sizeof(CsrBtHfSetC2cSampleRateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_IND */

#ifndef EXCLUDE_CSR_BT_HF_STATUS_LOW_POWER_IND
CsrSize CsrBtHfStatusLowPowerIndSizeof(void *msg)
{
    CsrBtHfStatusLowPowerInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_STATUS_LOW_POWER_IND */

#ifndef EXCLUDE_CSR_BT_HF_STATUS_LOW_POWER_IND
CsrUint8 *CsrBtHfStatusLowPowerIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfStatusLowPowerInd *primitive;

    primitive = (CsrBtHfStatusLowPowerInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->currentMode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->oldMode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->wantedMode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->remoteReq);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_STATUS_LOW_POWER_IND */

#ifndef EXCLUDE_CSR_BT_HF_STATUS_LOW_POWER_IND
void *CsrBtHfStatusLowPowerIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfStatusLowPowerInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfStatusLowPowerInd *) CsrPmemZalloc(sizeof(CsrBtHfStatusLowPowerInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->currentMode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->oldMode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->wantedMode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->remoteReq, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_STATUS_LOW_POWER_IND */

#ifndef EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_REQ
CsrSize CsrBtHfUpdateSupportedCodecReqSizeof(void *msg)
{
    CsrBtHfUpdateSupportedCodecReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_REQ */

#ifndef EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_REQ
CsrUint8 *CsrBtHfUpdateSupportedCodecReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfUpdateSupportedCodecReq *primitive;

    primitive = (CsrBtHfUpdateSupportedCodecReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->codecMask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enable);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sendUpdate);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_REQ */

#ifndef EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_REQ
void *CsrBtHfUpdateSupportedCodecReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfUpdateSupportedCodecReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfUpdateSupportedCodecReq *) CsrPmemZalloc(sizeof(CsrBtHfUpdateSupportedCodecReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->codecMask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enable, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->sendUpdate, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DISCONNECT_IND
CsrSize CsrBtHfDisconnectIndSizeof(void *msg)
{
    CsrBtHfDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_DISCONNECT_IND
CsrUint8 *CsrBtHfDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfDisconnectInd *primitive;

    primitive = (CsrBtHfDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_DISCONNECT_IND
void *CsrBtHfDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtHfDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_IND
CsrSize CsrBtHfSetC2cAudioCodecIndSizeof(void *msg)
{
    CsrBtHfSetC2cAudioCodecInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_IND */

#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_IND
CsrUint8 *CsrBtHfSetC2cAudioCodecIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetC2cAudioCodecInd *primitive;

    primitive = (CsrBtHfSetC2cAudioCodecInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_IND */

#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_IND
void *CsrBtHfSetC2cAudioCodecIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetC2cAudioCodecInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetC2cAudioCodecInd *) CsrPmemZalloc(sizeof(CsrBtHfSetC2cAudioCodecInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_IND */

#ifndef EXCLUDE_CSR_BT_HF_CALL_NOTIFICATION_IND
CsrSize CsrBtHfCallNotificationIndSizeof(void *msg)
{
    CsrBtHfCallNotificationInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->clipString));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_HF_CALL_NOTIFICATION_IND
CsrUint8 *CsrBtHfCallNotificationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfCallNotificationInd *primitive;

    primitive = (CsrBtHfCallNotificationInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrMemCpySer(buffer, length, &primitive->clipString, sizeof(primitive->clipString));
    CsrCharStringSer(buffer, length, primitive->clipString);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_HF_CALL_NOTIFICATION_IND
void *CsrBtHfCallNotificationIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfCallNotificationInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfCallNotificationInd *) CsrPmemZalloc(sizeof(CsrBtHfCallNotificationInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->clipString), buffer, &offset, ((CsrSize) sizeof(primitive->clipString)));
    CsrCharStringDes(&primitive->clipString, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_HF_CALL_NOTIFICATION_IND
void CsrBtHfCallNotificationIndSerFree(void *msg)
{
    CsrBtHfCallNotificationInd *primitive;

    primitive = (CsrBtHfCallNotificationInd *) msg;

    if(primitive->clipString != NULL)
    {
        CsrPmemFree(primitive->clipString);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_SMS_IND
CsrSize CsrBtHfC2cSmsIndSizeof(void *msg)
{
    CsrBtHfC2cSmsInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_SMS_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_SMS_IND
CsrUint8 *CsrBtHfC2cSmsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfC2cSmsInd *primitive;

    primitive = (CsrBtHfC2cSmsInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->index);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_SMS_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_SMS_IND
void *CsrBtHfC2cSmsIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfC2cSmsInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfC2cSmsInd *) CsrPmemZalloc(sizeof(CsrBtHfC2cSmsInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->index, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_SMS_IND */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES
CsrSize CsrBtHfAudioAcceptConnectResSizeof(void *msg)
{
    CsrBtHfAudioAcceptConnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->acceptParametersLength * sizeof(CsrBtHfAudioIncomingAcceptParameters)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES
CsrUint8 *CsrBtHfAudioAcceptConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfAudioAcceptConnectRes *primitive;

    primitive = (CsrBtHfAudioAcceptConnectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->acceptResponse);
    CsrMemCpySer(buffer, length, &primitive->acceptParameters, sizeof(primitive->acceptParameters));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->acceptParametersLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmSlot);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmReassign);
    if(primitive->acceptParameters)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->acceptParameters), ((CsrSize) (primitive->acceptParametersLength * sizeof(CsrBtHfAudioIncomingAcceptParameters))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES
void *CsrBtHfAudioAcceptConnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfAudioAcceptConnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfAudioAcceptConnectRes *) CsrPmemZalloc(sizeof(CsrBtHfAudioAcceptConnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->acceptResponse, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->acceptParameters), buffer, &offset, ((CsrSize) sizeof(primitive->acceptParameters)));
    CsrUint8Des((CsrUint8 *) &primitive->acceptParametersLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmSlot, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmReassign, buffer, &offset);
    if (primitive->acceptParametersLength)
    {
        primitive->acceptParameters = CsrPmemZalloc(((CsrUint32) (primitive->acceptParametersLength * sizeof(CsrBtHfAudioIncomingAcceptParameters))));
        CsrMemCpyDes(((void *) primitive->acceptParameters), buffer, &offset, ((CsrSize) (primitive->acceptParametersLength * sizeof(CsrBtHfAudioIncomingAcceptParameters))));
    }
    else
    {
        primitive->acceptParameters = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES
void CsrBtHfAudioAcceptConnectResSerFree(void *msg)
{
    CsrBtHfAudioAcceptConnectRes *primitive;

    primitive = (CsrBtHfAudioAcceptConnectRes *) msg;

    if(primitive->acceptParameters != NULL)
    {
        CsrPmemFree(primitive->acceptParameters);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_REQ
CsrSize CsrBtHfServiceConnectReqSizeof(void *msg)
{
    CsrBtHfServiceConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_REQ
CsrUint8 *CsrBtHfServiceConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfServiceConnectReq *primitive;

    primitive = (CsrBtHfServiceConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_REQ
void *CsrBtHfServiceConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfServiceConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfServiceConnectReq *) CsrPmemZalloc(sizeof(CsrBtHfServiceConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SERVICE_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_CFM
CsrSize CsrBtHfAtCmdCfmSizeof(void *msg)
{
    CsrBtHfAtCmdCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_CFM */

#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_CFM
CsrUint8 *CsrBtHfAtCmdCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfAtCmdCfm *primitive;

    primitive = (CsrBtHfAtCmdCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_CFM */

#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_CFM
void *CsrBtHfAtCmdCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfAtCmdCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfAtCmdCfm *) CsrPmemZalloc(sizeof(CsrBtHfAtCmdCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SECURITY_IN_CFM
CsrSize CsrBtHfSecurityInCfmSizeof(void *msg)
{
    CsrBtHfSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SECURITY_IN_CFM
CsrUint8 *CsrBtHfSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSecurityInCfm *primitive;

    primitive = (CsrBtHfSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SECURITY_IN_CFM
void *CsrBtHfSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtHfSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ
CsrSize CsrBtHfSpeakerGainStatusReqSizeof(void *msg)
{
    CsrBtHfSpeakerGainStatusReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ
CsrUint8 *CsrBtHfSpeakerGainStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSpeakerGainStatusReq *primitive;

    primitive = (CsrBtHfSpeakerGainStatusReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->gain);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ
void *CsrBtHfSpeakerGainStatusReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSpeakerGainStatusReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSpeakerGainStatusReq *) CsrPmemZalloc(sizeof(CsrBtHfSpeakerGainStatusReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->gain, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DIAL_REQ
CsrSize CsrBtHfDialReqSizeof(void *msg)
{
    CsrBtHfDialReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->number));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_DIAL_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DIAL_REQ
CsrUint8 *CsrBtHfDialReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfDialReq *primitive;

    primitive = (CsrBtHfDialReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->command);
    CsrMemCpySer(buffer, length, &primitive->number, sizeof(primitive->number));
    CsrCharStringSer(buffer, length, primitive->number);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_DIAL_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DIAL_REQ
void *CsrBtHfDialReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfDialReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfDialReq *) CsrPmemZalloc(sizeof(CsrBtHfDialReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->command, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->number), buffer, &offset, ((CsrSize) sizeof(primitive->number)));
    CsrCharStringDes(&primitive->number, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_DIAL_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DIAL_REQ
void CsrBtHfDialReqSerFree(void *msg)
{
    CsrBtHfDialReq *primitive;

    primitive = (CsrBtHfDialReq *) msg;

    if(primitive->number != NULL)
    {
        CsrPmemFree(primitive->number);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_DIAL_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_IND
CsrSize CsrBtHfGetCurrentCallListIndSizeof(void *msg)
{
    CsrBtHfGetCurrentCallListInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->clccString));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_IND */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_IND
CsrUint8 *CsrBtHfGetCurrentCallListIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfGetCurrentCallListInd *primitive;

    primitive = (CsrBtHfGetCurrentCallListInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrMemCpySer(buffer, length, &primitive->clccString, sizeof(primitive->clccString));
    CsrCharStringSer(buffer, length, primitive->clccString);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_IND */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_IND
void *CsrBtHfGetCurrentCallListIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfGetCurrentCallListInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfGetCurrentCallListInd *) CsrPmemZalloc(sizeof(CsrBtHfGetCurrentCallListInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->clccString), buffer, &offset, ((CsrSize) sizeof(primitive->clccString)));
    CsrCharStringDes(&primitive->clccString, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_IND */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_IND
void CsrBtHfGetCurrentCallListIndSerFree(void *msg)
{
    CsrBtHfGetCurrentCallListInd *primitive;

    primitive = (CsrBtHfGetCurrentCallListInd *) msg;

    if(primitive->clccString != NULL)
    {
        CsrPmemFree(primitive->clccString);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_IND */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ
CsrSize CsrBtHfGetCurrentOperatorSelectionReqSizeof(void *msg)
{
    CsrBtHfGetCurrentOperatorSelectionReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ
CsrUint8 *CsrBtHfGetCurrentOperatorSelectionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfGetCurrentOperatorSelectionReq *primitive;

    primitive = (CsrBtHfGetCurrentOperatorSelectionReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->format);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->forceResendingFormat);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ
void *CsrBtHfGetCurrentOperatorSelectionReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfGetCurrentOperatorSelectionReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfGetCurrentOperatorSelectionReq *) CsrPmemZalloc(sizeof(CsrBtHfGetCurrentOperatorSelectionReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->format, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->forceResendingFormat, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_CFM
CsrSize CsrBtHfAudioConnectCfmSizeof(void *msg)
{
    CsrBtHfAudioConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_CFM
CsrUint8 *CsrBtHfAudioConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfAudioConnectCfm *primitive;

    primitive = (CsrBtHfAudioConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmSlot);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->linkType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->txInterval);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->weSco);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rxPacketLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->txPacketLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->airMode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->codecUsed);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sampleRate);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_CFM
void *CsrBtHfAudioConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfAudioConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfAudioConnectCfm *) CsrPmemZalloc(sizeof(CsrBtHfAudioConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmSlot, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->linkType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->txInterval, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->weSco, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rxPacketLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->txPacketLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->airMode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->codecUsed, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->sampleRate, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_CFM
CsrSize CsrBtHfSetEchoAndNoiseCfmSizeof(void *msg)
{
    CsrBtHfSetEchoAndNoiseCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_CFM
CsrUint8 *CsrBtHfSetEchoAndNoiseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetEchoAndNoiseCfm *primitive;

    primitive = (CsrBtHfSetEchoAndNoiseCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_CFM
void *CsrBtHfSetEchoAndNoiseCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetEchoAndNoiseCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetEchoAndNoiseCfm *) CsrPmemZalloc(sizeof(CsrBtHfSetEchoAndNoiseCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_BT_INPUT_CFM
CsrSize CsrBtHfBtInputCfmSizeof(void *msg)
{
    CsrBtHfBtInputCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->dataRespString));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_BT_INPUT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_BT_INPUT_CFM
CsrUint8 *CsrBtHfBtInputCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfBtInputCfm *primitive;

    primitive = (CsrBtHfBtInputCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrMemCpySer(buffer, length, &primitive->dataRespString, sizeof(primitive->dataRespString));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);
    CsrCharStringSer(buffer, length, primitive->dataRespString);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_BT_INPUT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_BT_INPUT_CFM
void *CsrBtHfBtInputCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfBtInputCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfBtInputCfm *) CsrPmemZalloc(sizeof(CsrBtHfBtInputCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->dataRespString), buffer, &offset, ((CsrSize) sizeof(primitive->dataRespString)));
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);
    CsrCharStringDes(&primitive->dataRespString, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_BT_INPUT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_BT_INPUT_CFM
void CsrBtHfBtInputCfmSerFree(void *msg)
{
    CsrBtHfBtInputCfm *primitive;

    primitive = (CsrBtHfBtInputCfm *) msg;

    if(primitive->dataRespString != NULL)
    {
        CsrPmemFree(primitive->dataRespString);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_BT_INPUT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SECURITY_IN_REQ
CsrSize CsrBtHfSecurityInReqSizeof(void *msg)
{
    CsrBtHfSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SECURITY_IN_REQ
CsrUint8 *CsrBtHfSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSecurityInReq *primitive;

    primitive = (CsrBtHfSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SECURITY_IN_REQ
void *CsrBtHfSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtHfSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ
CsrSize CsrBtHfGetCurrentCallListReqSizeof(void *msg)
{
    CsrBtHfGetCurrentCallListReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ
CsrUint8 *CsrBtHfGetCurrentCallListReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfGetCurrentCallListReq *primitive;

    primitive = (CsrBtHfGetCurrentCallListReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ
void *CsrBtHfGetCurrentCallListReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfGetCurrentCallListReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfGetCurrentCallListReq *) CsrPmemZalloc(sizeof(CsrBtHfGetCurrentCallListReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ
CsrSize CsrBtHfGetC2cAdpcmLocalSupportedReqSizeof(void *msg)
{
    CsrBtHfGetC2cAdpcmLocalSupportedReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ
CsrUint8 *CsrBtHfGetC2cAdpcmLocalSupportedReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfGetC2cAdpcmLocalSupportedReq *primitive;

    primitive = (CsrBtHfGetC2cAdpcmLocalSupportedReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ
void *CsrBtHfGetC2cAdpcmLocalSupportedReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfGetC2cAdpcmLocalSupportedReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfGetC2cAdpcmLocalSupportedReq *) CsrPmemZalloc(sizeof(CsrBtHfGetC2cAdpcmLocalSupportedReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_CFM
CsrSize CsrBtHfConfigLowPowerCfmSizeof(void *msg)
{
    CsrBtHfConfigLowPowerCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_CFM
CsrUint8 *CsrBtHfConfigLowPowerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfConfigLowPowerCfm *primitive;

    primitive = (CsrBtHfConfigLowPowerCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_CFM
void *CsrBtHfConfigLowPowerCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfConfigLowPowerCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfConfigLowPowerCfm *) CsrPmemZalloc(sizeof(CsrBtHfConfigLowPowerCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_LOW_POWER_CFM */

#ifndef EXCLUDE_CSR_BT_HF_DISCONNECT_REQ
CsrSize CsrBtHfDisconnectReqSizeof(void *msg)
{
    CsrBtHfDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DISCONNECT_REQ
CsrUint8 *CsrBtHfDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfDisconnectReq *primitive;

    primitive = (CsrBtHfDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DISCONNECT_REQ
void *CsrBtHfDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtHfDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_CFM
CsrSize CsrBtHfAudioDisconnectCfmSizeof(void *msg)
{
    CsrBtHfAudioDisconnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_CFM
CsrUint8 *CsrBtHfAudioDisconnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfAudioDisconnectCfm *primitive;

    primitive = (CsrBtHfAudioDisconnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_CFM
void *CsrBtHfAudioDisconnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfAudioDisconnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfAudioDisconnectCfm *) CsrPmemZalloc(sizeof(CsrBtHfAudioDisconnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_CFM
CsrSize CsrBtHfSetVoiceRecognitionCfmSizeof(void *msg)
{
    CsrBtHfSetVoiceRecognitionCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_CFM
CsrUint8 *CsrBtHfSetVoiceRecognitionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetVoiceRecognitionCfm *primitive;

    primitive = (CsrBtHfSetVoiceRecognitionCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_CFM
void *CsrBtHfSetVoiceRecognitionCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetVoiceRecognitionCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetVoiceRecognitionCfm *) CsrPmemZalloc(sizeof(CsrBtHfSetVoiceRecognitionCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_HOUSE_CLEANING
CsrSize CsrBtHfHouseCleaningSizeof(void *msg)
{
    CsrBtHfHouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_HF_HOUSE_CLEANING
CsrUint8 *CsrBtHfHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfHouseCleaning *primitive;

    primitive = (CsrBtHfHouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_HF_HOUSE_CLEANING
void *CsrBtHfHouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfHouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfHouseCleaning *) CsrPmemZalloc(sizeof(CsrBtHfHouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_CFM
CsrSize CsrBtHfSetStatusIndicatorUpdateCfmSizeof(void *msg)
{
    CsrBtHfSetStatusIndicatorUpdateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_CFM
CsrUint8 *CsrBtHfSetStatusIndicatorUpdateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetStatusIndicatorUpdateCfm *primitive;

    primitive = (CsrBtHfSetStatusIndicatorUpdateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_CFM
void *CsrBtHfSetStatusIndicatorUpdateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetStatusIndicatorUpdateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetStatusIndicatorUpdateCfm *) CsrPmemZalloc(sizeof(CsrBtHfSetStatusIndicatorUpdateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_STATUS_INDICATOR_UPDATE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND
CsrSize CsrBtHfGetC2cAdpcmLocalSupportedIndSizeof(void *msg)
{
    CsrBtHfGetC2cAdpcmLocalSupportedInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND */

#ifndef EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND
CsrUint8 *CsrBtHfGetC2cAdpcmLocalSupportedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfGetC2cAdpcmLocalSupportedInd *primitive;

    primitive = (CsrBtHfGetC2cAdpcmLocalSupportedInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND */

#ifndef EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND
void *CsrBtHfGetC2cAdpcmLocalSupportedIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfGetC2cAdpcmLocalSupportedInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfGetC2cAdpcmLocalSupportedInd *) CsrPmemZalloc(sizeof(CsrBtHfGetC2cAdpcmLocalSupportedInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND */

#ifndef EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_CFM
CsrSize CsrBtHfUpdateSupportedCodecCfmSizeof(void *msg)
{
    CsrBtHfUpdateSupportedCodecCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_CFM */

#ifndef EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_CFM
CsrUint8 *CsrBtHfUpdateSupportedCodecCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfUpdateSupportedCodecCfm *primitive;

    primitive = (CsrBtHfUpdateSupportedCodecCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_CFM */

#ifndef EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_CFM
void *CsrBtHfUpdateSupportedCodecCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfUpdateSupportedCodecCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfUpdateSupportedCodecCfm *) CsrPmemZalloc(sizeof(CsrBtHfUpdateSupportedCodecCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_UPDATE_SUPPORTED_CODEC_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_REQ
CsrSize CsrBtHfConfigAudioReqSizeof(void *msg)
{
    CsrBtHfConfigAudioReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->audioSettingLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_REQ
CsrUint8 *CsrBtHfConfigAudioReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfConfigAudioReq *primitive;

    primitive = (CsrBtHfConfigAudioReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->audioType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->audioSettingLen);
    CsrMemCpySer(buffer, length, &primitive->audioSetting, sizeof(primitive->audioSetting));
    if(primitive->audioSetting)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->audioSetting), ((CsrSize) (primitive->audioSettingLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_REQ
void *CsrBtHfConfigAudioReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfConfigAudioReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfConfigAudioReq *) CsrPmemZalloc(sizeof(CsrBtHfConfigAudioReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->audioType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->audioSettingLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->audioSetting), buffer, &offset, ((CsrSize) sizeof(primitive->audioSetting)));
    if (primitive->audioSettingLen)
    {
        primitive->audioSetting = CsrPmemZalloc(((CsrUint32) (primitive->audioSettingLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->audioSetting), buffer, &offset, ((CsrSize) (primitive->audioSettingLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->audioSetting = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_REQ
void CsrBtHfConfigAudioReqSerFree(void *msg)
{
    CsrBtHfConfigAudioReq *primitive;

    primitive = (CsrBtHfConfigAudioReq *) msg;

    if(primitive->audioSetting != NULL)
    {
        CsrPmemFree(primitive->audioSetting);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_CONFIG_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_IND
CsrSize CsrBtHfSpeakerGainIndSizeof(void *msg)
{
    CsrBtHfSpeakerGainInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_IND */

#ifndef EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_IND
CsrUint8 *CsrBtHfSpeakerGainIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSpeakerGainInd *primitive;

    primitive = (CsrBtHfSpeakerGainInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->gain);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_IND */

#ifndef EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_IND
void *CsrBtHfSpeakerGainIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSpeakerGainInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSpeakerGainInd *) CsrPmemZalloc(sizeof(CsrBtHfSpeakerGainInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->gain, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_IND */

#ifndef EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_CFM
CsrSize CsrBtHfDeregisterTimeCfmSizeof(void *msg)
{
    CsrBtHfDeregisterTimeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_CFM */

#ifndef EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_CFM
CsrUint8 *CsrBtHfDeregisterTimeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfDeregisterTimeCfm *primitive;

    primitive = (CsrBtHfDeregisterTimeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_CFM */

#ifndef EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_CFM
void *CsrBtHfDeregisterTimeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfDeregisterTimeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfDeregisterTimeCfm *) CsrPmemZalloc(sizeof(CsrBtHfDeregisterTimeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_DEREGISTER_TIME_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CANCEL_CONNECT_REQ
CsrSize CsrBtHfCancelConnectReqSizeof(void *msg)
{
    CsrBtHfCancelConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CANCEL_CONNECT_REQ
CsrUint8 *CsrBtHfCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfCancelConnectReq *primitive;

    primitive = (CsrBtHfCancelConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CANCEL_CONNECT_REQ
void *CsrBtHfCancelConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfCancelConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfCancelConnectReq *) CsrPmemZalloc(sizeof(CsrBtHfCancelConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_ACTIVATE_CFM
CsrSize CsrBtHfActivateCfmSizeof(void *msg)
{
    CsrBtHfActivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_ACTIVATE_CFM
CsrUint8 *CsrBtHfActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfActivateCfm *primitive;

    primitive = (CsrBtHfActivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_ACTIVATE_CFM
void *CsrBtHfActivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfActivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfActivateCfm *) CsrPmemZalloc(sizeof(CsrBtHfActivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM
CsrSize CsrBtHfGetSubscriberNumberInformationCfmSizeof(void *msg)
{
    CsrBtHfGetSubscriberNumberInformationCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM
CsrUint8 *CsrBtHfGetSubscriberNumberInformationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfGetSubscriberNumberInformationCfm *primitive;

    primitive = (CsrBtHfGetSubscriberNumberInformationCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM
void *CsrBtHfGetSubscriberNumberInformationCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfGetSubscriberNumberInformationCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfGetSubscriberNumberInformationCfm *) CsrPmemZalloc(sizeof(CsrBtHfGetSubscriberNumberInformationCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND
CsrSize CsrBtHfCallWaitingNotificationIndSizeof(void *msg)
{
    CsrBtHfCallWaitingNotificationInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->ccwaString));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND
CsrUint8 *CsrBtHfCallWaitingNotificationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfCallWaitingNotificationInd *primitive;

    primitive = (CsrBtHfCallWaitingNotificationInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrMemCpySer(buffer, length, &primitive->ccwaString, sizeof(primitive->ccwaString));
    CsrCharStringSer(buffer, length, primitive->ccwaString);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND
void *CsrBtHfCallWaitingNotificationIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfCallWaitingNotificationInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfCallWaitingNotificationInd *) CsrPmemZalloc(sizeof(CsrBtHfCallWaitingNotificationInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ccwaString), buffer, &offset, ((CsrSize) sizeof(primitive->ccwaString)));
    CsrCharStringDes(&primitive->ccwaString, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND
void CsrBtHfCallWaitingNotificationIndSerFree(void *msg)
{
    CsrBtHfCallWaitingNotificationInd *primitive;

    primitive = (CsrBtHfCallWaitingNotificationInd *) msg;

    if(primitive->ccwaString != NULL)
    {
        CsrPmemFree(primitive->ccwaString);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_WAITING_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_CFM
CsrSize CsrBtHfIndicatorActivationCfmSizeof(void *msg)
{
    CsrBtHfIndicatorActivationCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_CFM
CsrUint8 *CsrBtHfIndicatorActivationCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfIndicatorActivationCfm *primitive;

    primitive = (CsrBtHfIndicatorActivationCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_CFM
void *CsrBtHfIndicatorActivationCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfIndicatorActivationCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfIndicatorActivationCfm *) CsrPmemZalloc(sizeof(CsrBtHfIndicatorActivationCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_INDICATOR_ACTIVATION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CALL_END_CFM
CsrSize CsrBtHfCallEndCfmSizeof(void *msg)
{
    CsrBtHfCallEndCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_END_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CALL_END_CFM
CsrUint8 *CsrBtHfCallEndCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfCallEndCfm *primitive;

    primitive = (CsrBtHfCallEndCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_END_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CALL_END_CFM
void *CsrBtHfCallEndCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfCallEndCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfCallEndCfm *) CsrPmemZalloc(sizeof(CsrBtHfCallEndCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_END_CFM */

#ifndef EXCLUDE_CSR_BT_HF_ACTIVATE_REQ
CsrSize CsrBtHfActivateReqSizeof(void *msg)
{
    CsrBtHfActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_ACTIVATE_REQ
CsrUint8 *CsrBtHfActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfActivateReq *primitive;

    primitive = (CsrBtHfActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->maxHFConnections);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->maxHSConnections);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->maxSimultaneousConnections);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->supportedFeatures);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->hfConfig);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->atResponseTime);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_ACTIVATE_REQ
void *CsrBtHfActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfActivateReq *) CsrPmemZalloc(sizeof(CsrBtHfActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->maxHFConnections, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->maxHSConnections, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->maxSimultaneousConnections, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->supportedFeatures, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->hfConfig, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->atResponseTime, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ
CsrSize CsrBtHfGetSubscriberNumberInformationReqSizeof(void *msg)
{
    CsrBtHfGetSubscriberNumberInformationReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ
CsrUint8 *CsrBtHfGetSubscriberNumberInformationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfGetSubscriberNumberInformationReq *primitive;

    primitive = (CsrBtHfGetSubscriberNumberInformationReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ
void *CsrBtHfGetSubscriberNumberInformationReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfGetSubscriberNumberInformationReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfGetSubscriberNumberInformationReq *) CsrPmemZalloc(sizeof(CsrBtHfGetSubscriberNumberInformationReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM
CsrSize CsrBtHfGetCurrentOperatorSelectionCfmSizeof(void *msg)
{
    CsrBtHfGetCurrentOperatorSelectionCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->copsString));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM
CsrUint8 *CsrBtHfGetCurrentOperatorSelectionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfGetCurrentOperatorSelectionCfm *primitive;

    primitive = (CsrBtHfGetCurrentOperatorSelectionCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrMemCpySer(buffer, length, &primitive->copsString, sizeof(primitive->copsString));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);
    CsrCharStringSer(buffer, length, primitive->copsString);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM
void *CsrBtHfGetCurrentOperatorSelectionCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfGetCurrentOperatorSelectionCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfGetCurrentOperatorSelectionCfm *) CsrPmemZalloc(sizeof(CsrBtHfGetCurrentOperatorSelectionCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->copsString), buffer, &offset, ((CsrSize) sizeof(primitive->copsString)));
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);
    CsrCharStringDes(&primitive->copsString, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM
void CsrBtHfGetCurrentOperatorSelectionCfmSerFree(void *msg)
{
    CsrBtHfGetCurrentOperatorSelectionCfm *primitive;

    primitive = (CsrBtHfGetCurrentOperatorSelectionCfm *) msg;

    if(primitive->copsString != NULL)
    {
        CsrPmemFree(primitive->copsString);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_OPERATOR_SELECTION_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ
CsrSize CsrBtHfSetC2cSampleRateReqSizeof(void *msg)
{
    CsrBtHfSetC2cSampleRateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ
CsrUint8 *CsrBtHfSetC2cSampleRateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetC2cSampleRateReq *primitive;

    primitive = (CsrBtHfSetC2cSampleRateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ
void *CsrBtHfSetC2cSampleRateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetC2cSampleRateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetC2cSampleRateReq *) CsrPmemZalloc(sizeof(CsrBtHfSetC2cSampleRateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_SAMPLE_RATE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_CFM
CsrSize CsrBtHfSpeakerGainStatusCfmSizeof(void *msg)
{
    CsrBtHfSpeakerGainStatusCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_CFM
CsrUint8 *CsrBtHfSpeakerGainStatusCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSpeakerGainStatusCfm *primitive;

    primitive = (CsrBtHfSpeakerGainStatusCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_CFM
void *CsrBtHfSpeakerGainStatusCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSpeakerGainStatusCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSpeakerGainStatusCfm *) CsrPmemZalloc(sizeof(CsrBtHfSpeakerGainStatusCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SPEAKER_GAIN_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ
CsrSize CsrBtHfSetCallWaitingNotificationReqSizeof(void *msg)
{
    CsrBtHfSetCallWaitingNotificationReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ
CsrUint8 *CsrBtHfSetCallWaitingNotificationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetCallWaitingNotificationReq *primitive;

    primitive = (CsrBtHfSetCallWaitingNotificationReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enable);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ
void *CsrBtHfSetCallWaitingNotificationReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetCallWaitingNotificationReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetCallWaitingNotificationReq *) CsrPmemZalloc(sizeof(CsrBtHfSetCallWaitingNotificationReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_WAITING_NOTIFICATION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_IND
CsrSize CsrBtHfAtCmdIndSizeof(void *msg)
{
    CsrBtHfAtCmdInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->atCmdString));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_IND */

#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_IND
CsrUint8 *CsrBtHfAtCmdIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfAtCmdInd *primitive;

    primitive = (CsrBtHfAtCmdInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrMemCpySer(buffer, length, &primitive->atCmdString, sizeof(primitive->atCmdString));
    CsrCharStringSer(buffer, length, primitive->atCmdString);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_IND */

#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_IND
void *CsrBtHfAtCmdIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfAtCmdInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfAtCmdInd *) CsrPmemZalloc(sizeof(CsrBtHfAtCmdInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->atCmdString), buffer, &offset, ((CsrSize) sizeof(primitive->atCmdString)));
    CsrCharStringDes(&primitive->atCmdString, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_IND */

#ifndef EXCLUDE_CSR_BT_HF_AT_CMD_IND
void CsrBtHfAtCmdIndSerFree(void *msg)
{
    CsrBtHfAtCmdInd *primitive;

    primitive = (CsrBtHfAtCmdInd *) msg;

    if(primitive->atCmdString != NULL)
    {
        CsrPmemFree(primitive->atCmdString);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_AT_CMD_IND */

#ifndef EXCLUDE_CSR_BT_HF_CALL_ANSWER_CFM
CsrSize CsrBtHfCallAnswerCfmSizeof(void *msg)
{
    CsrBtHfCallAnswerCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_ANSWER_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CALL_ANSWER_CFM
CsrUint8 *CsrBtHfCallAnswerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfCallAnswerCfm *primitive;

    primitive = (CsrBtHfCallAnswerCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_ANSWER_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CALL_ANSWER_CFM
void *CsrBtHfCallAnswerCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfCallAnswerCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfCallAnswerCfm *) CsrPmemZalloc(sizeof(CsrBtHfCallAnswerCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_ANSWER_CFM */

#ifndef EXCLUDE_CSR_BT_HF_C2C_PWR_CFM
CsrSize CsrBtHfC2cPwrCfmSizeof(void *msg)
{
    CsrBtHfC2cPwrCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_PWR_CFM */

#ifndef EXCLUDE_CSR_BT_HF_C2C_PWR_CFM
CsrUint8 *CsrBtHfC2cPwrCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfC2cPwrCfm *primitive;

    primitive = (CsrBtHfC2cPwrCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_PWR_CFM */

#ifndef EXCLUDE_CSR_BT_HF_C2C_PWR_CFM
void *CsrBtHfC2cPwrCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfC2cPwrCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfC2cPwrCfm *) CsrPmemZalloc(sizeof(CsrBtHfC2cPwrCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_PWR_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ
CsrSize CsrBtHfSetCallNotificationIndicationReqSizeof(void *msg)
{
    CsrBtHfSetCallNotificationIndicationReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ
CsrUint8 *CsrBtHfSetCallNotificationIndicationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetCallNotificationIndicationReq *primitive;

    primitive = (CsrBtHfSetCallNotificationIndicationReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enable);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ
void *CsrBtHfSetCallNotificationIndicationReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetCallNotificationIndicationReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetCallNotificationIndicationReq *) CsrPmemZalloc(sizeof(CsrBtHfSetCallNotificationIndicationReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_CALL_NOTIFICATION_INDICATION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_PWR_REQ
CsrSize CsrBtHfC2cPwrReqSizeof(void *msg)
{
    CsrBtHfC2cPwrReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_PWR_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_PWR_REQ
CsrUint8 *CsrBtHfC2cPwrReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfC2cPwrReq *primitive;

    primitive = (CsrBtHfC2cPwrReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_PWR_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_PWR_REQ
void *CsrBtHfC2cPwrReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfC2cPwrReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfC2cPwrReq *) CsrPmemZalloc(sizeof(CsrBtHfC2cPwrReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_PWR_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GENERATE_DTMF_CFM
CsrSize CsrBtHfGenerateDtmfCfmSizeof(void *msg)
{
    CsrBtHfGenerateDtmfCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_GENERATE_DTMF_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GENERATE_DTMF_CFM
CsrUint8 *CsrBtHfGenerateDtmfCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfGenerateDtmfCfm *primitive;

    primitive = (CsrBtHfGenerateDtmfCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_GENERATE_DTMF_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GENERATE_DTMF_CFM
void *CsrBtHfGenerateDtmfCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfGenerateDtmfCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfGenerateDtmfCfm *) CsrPmemZalloc(sizeof(CsrBtHfGenerateDtmfCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_GENERATE_DTMF_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_CFM
CsrSize CsrBtHfGetCurrentCallListCfmSizeof(void *msg)
{
    CsrBtHfGetCurrentCallListCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_CFM
CsrUint8 *CsrBtHfGetCurrentCallListCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfGetCurrentCallListCfm *primitive;

    primitive = (CsrBtHfGetCurrentCallListCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_CFM
void *CsrBtHfGetCurrentCallListCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfGetCurrentCallListCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfGetCurrentCallListCfm *) CsrPmemZalloc(sizeof(CsrBtHfGetCurrentCallListCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_CURRENT_CALL_LIST_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_REQ
CsrSize CsrBtHfSetEchoAndNoiseReqSizeof(void *msg)
{
    CsrBtHfSetEchoAndNoiseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_REQ
CsrUint8 *CsrBtHfSetEchoAndNoiseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetEchoAndNoiseReq *primitive;

    primitive = (CsrBtHfSetEchoAndNoiseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enable);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_REQ
void *CsrBtHfSetEchoAndNoiseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetEchoAndNoiseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetEchoAndNoiseReq *) CsrPmemZalloc(sizeof(CsrBtHfSetEchoAndNoiseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_ECHO_AND_NOISE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CALL_END_REQ
CsrSize CsrBtHfCallEndReqSizeof(void *msg)
{
    CsrBtHfCallEndReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_END_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CALL_END_REQ
CsrUint8 *CsrBtHfCallEndReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfCallEndReq *primitive;

    primitive = (CsrBtHfCallEndReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_END_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CALL_END_REQ
void *CsrBtHfCallEndReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfCallEndReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfCallEndReq *) CsrPmemZalloc(sizeof(CsrBtHfCallEndReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_END_REQ */

#ifndef EXCLUDE_CSR_BT_HF_INBAND_RING_SETTING_CHANGED_IND
CsrSize CsrBtHfInbandRingSettingChangedIndSizeof(void *msg)
{
    CsrBtHfInbandRingSettingChangedInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_INBAND_RING_SETTING_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_HF_INBAND_RING_SETTING_CHANGED_IND
CsrUint8 *CsrBtHfInbandRingSettingChangedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfInbandRingSettingChangedInd *primitive;

    primitive = (CsrBtHfInbandRingSettingChangedInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->inbandRingingActivated);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_INBAND_RING_SETTING_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_HF_INBAND_RING_SETTING_CHANGED_IND
void *CsrBtHfInbandRingSettingChangedIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfInbandRingSettingChangedInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfInbandRingSettingChangedInd *) CsrPmemZalloc(sizeof(CsrBtHfInbandRingSettingChangedInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->inbandRingingActivated, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_INBAND_RING_SETTING_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_GET_SMS_REQ
CsrSize CsrBtHfC2cGetSmsReqSizeof(void *msg)
{
    CsrBtHfC2cGetSmsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_GET_SMS_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_GET_SMS_REQ
CsrUint8 *CsrBtHfC2cGetSmsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfC2cGetSmsReq *primitive;

    primitive = (CsrBtHfC2cGetSmsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->index);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_GET_SMS_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_GET_SMS_REQ
void *CsrBtHfC2cGetSmsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfC2cGetSmsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfC2cGetSmsReq *) CsrPmemZalloc(sizeof(CsrBtHfC2cGetSmsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->index, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_GET_SMS_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_REQ
CsrSize CsrBtHfSetVoiceRecognitionReqSizeof(void *msg)
{
    CsrBtHfSetVoiceRecognitionReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_REQ
CsrUint8 *CsrBtHfSetVoiceRecognitionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetVoiceRecognitionReq *primitive;

    primitive = (CsrBtHfSetVoiceRecognitionReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->start);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_REQ
void *CsrBtHfSetVoiceRecognitionReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetVoiceRecognitionReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetVoiceRecognitionReq *) CsrPmemZalloc(sizeof(CsrBtHfSetVoiceRecognitionReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->start, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_VOICE_RECOGNITION_REQ */

#ifndef EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM
CsrSize CsrBtHfGetAllStatusIndicatorsCfmSizeof(void *msg)
{
    CsrBtHfGetAllStatusIndicatorsCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->indicatorSupported) + CsrCharStringSerLen((CsrCharString *) primitive->indicatorValue));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM
CsrUint8 *CsrBtHfGetAllStatusIndicatorsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfGetAllStatusIndicatorsCfm *primitive;

    primitive = (CsrBtHfGetAllStatusIndicatorsCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrMemCpySer(buffer, length, &primitive->indicatorSupported, sizeof(primitive->indicatorSupported));
    CsrMemCpySer(buffer, length, &primitive->indicatorValue, sizeof(primitive->indicatorValue));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);
    CsrCharStringSer(buffer, length, primitive->indicatorSupported);
    CsrCharStringSer(buffer, length, primitive->indicatorValue);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM
void *CsrBtHfGetAllStatusIndicatorsCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfGetAllStatusIndicatorsCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfGetAllStatusIndicatorsCfm *) CsrPmemZalloc(sizeof(CsrBtHfGetAllStatusIndicatorsCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->indicatorSupported), buffer, &offset, ((CsrSize) sizeof(primitive->indicatorSupported)));
    CsrMemCpyDes(((void *) &primitive->indicatorValue), buffer, &offset, ((CsrSize) sizeof(primitive->indicatorValue)));
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);
    CsrCharStringDes(&primitive->indicatorSupported, buffer, &offset);
    CsrCharStringDes(&primitive->indicatorValue, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM
void CsrBtHfGetAllStatusIndicatorsCfmSerFree(void *msg)
{
    CsrBtHfGetAllStatusIndicatorsCfm *primitive;

    primitive = (CsrBtHfGetAllStatusIndicatorsCfm *) msg;

    if(primitive->indicatorSupported != NULL)
    {
        CsrPmemFree(primitive->indicatorSupported);
    }
    if(primitive->indicatorValue != NULL)
    {
        CsrPmemFree(primitive->indicatorValue);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_GET_ALL_STATUS_INDICATORS_CFM */

#ifndef EXCLUDE_CSR_BT_HF_CALL_HANDLING_IND
CsrSize CsrBtHfCallHandlingIndSizeof(void *msg)
{
    CsrBtHfCallHandlingInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_HANDLING_IND */

#ifndef EXCLUDE_CSR_BT_HF_CALL_HANDLING_IND
CsrUint8 *CsrBtHfCallHandlingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfCallHandlingInd *primitive;

    primitive = (CsrBtHfCallHandlingInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->event);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_HANDLING_IND */

#ifndef EXCLUDE_CSR_BT_HF_CALL_HANDLING_IND
void *CsrBtHfCallHandlingIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfCallHandlingInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfCallHandlingInd *) CsrPmemZalloc(sizeof(CsrBtHfCallHandlingInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->event, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_HANDLING_IND */

#ifndef EXCLUDE_CSR_BT_HF_DISCONNECT_CFM
CsrSize CsrBtHfDisconnectCfmSizeof(void *msg)
{
    CsrBtHfDisconnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_DISCONNECT_CFM
CsrUint8 *CsrBtHfDisconnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfDisconnectCfm *primitive;

    primitive = (CsrBtHfDisconnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_DISCONNECT_CFM
void *CsrBtHfDisconnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfDisconnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfDisconnectCfm *) CsrPmemZalloc(sizeof(CsrBtHfDisconnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM
CsrSize CsrBtHfSetExtendedAgErrorResultCodeCfmSizeof(void *msg)
{
    CsrBtHfSetExtendedAgErrorResultCodeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM
CsrUint8 *CsrBtHfSetExtendedAgErrorResultCodeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetExtendedAgErrorResultCodeCfm *primitive;

    primitive = (CsrBtHfSetExtendedAgErrorResultCodeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM
void *CsrBtHfSetExtendedAgErrorResultCodeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetExtendedAgErrorResultCodeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetExtendedAgErrorResultCodeCfm *) CsrPmemZalloc(sizeof(CsrBtHfSetExtendedAgErrorResultCodeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_EXTENDED_AG_ERROR_RESULT_CODE_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SECURITY_OUT_CFM
CsrSize CsrBtHfSecurityOutCfmSizeof(void *msg)
{
    CsrBtHfSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SECURITY_OUT_CFM
CsrUint8 *CsrBtHfSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSecurityOutCfm *primitive;

    primitive = (CsrBtHfSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SECURITY_OUT_CFM
void *CsrBtHfSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtHfSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND
CsrSize CsrBtHfGetSubscriberNumberInformationIndSizeof(void *msg)
{
    CsrBtHfGetSubscriberNumberInformationInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->cnumString));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND */

#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND
CsrUint8 *CsrBtHfGetSubscriberNumberInformationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfGetSubscriberNumberInformationInd *primitive;

    primitive = (CsrBtHfGetSubscriberNumberInformationInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrMemCpySer(buffer, length, &primitive->cnumString, sizeof(primitive->cnumString));
    CsrCharStringSer(buffer, length, primitive->cnumString);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND */

#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND
void *CsrBtHfGetSubscriberNumberInformationIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfGetSubscriberNumberInformationInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfGetSubscriberNumberInformationInd *) CsrPmemZalloc(sizeof(CsrBtHfGetSubscriberNumberInformationInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->cnumString), buffer, &offset, ((CsrSize) sizeof(primitive->cnumString)));
    CsrCharStringDes(&primitive->cnumString, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND */

#ifndef EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND
void CsrBtHfGetSubscriberNumberInformationIndSerFree(void *msg)
{
    CsrBtHfGetSubscriberNumberInformationInd *primitive;

    primitive = (CsrBtHfGetSubscriberNumberInformationInd *) msg;

    if(primitive->cnumString != NULL)
    {
        CsrPmemFree(primitive->cnumString);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_GET_SUBSCRIBER_NUMBER_INFORMATION_IND */

#ifndef EXCLUDE_CSR_BT_HF_DIAL_CFM
CsrSize CsrBtHfDialCfmSizeof(void *msg)
{
    CsrBtHfDialCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_DIAL_CFM */

#ifndef EXCLUDE_CSR_BT_HF_DIAL_CFM
CsrUint8 *CsrBtHfDialCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfDialCfm *primitive;

    primitive = (CsrBtHfDialCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_DIAL_CFM */

#ifndef EXCLUDE_CSR_BT_HF_DIAL_CFM
void *CsrBtHfDialCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfDialCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfDialCfm *) CsrPmemZalloc(sizeof(CsrBtHfDialCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_DIAL_CFM */

#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_REQ
CsrSize CsrBtHfSetC2cAudioCodecReqSizeof(void *msg)
{
    CsrBtHfSetC2cAudioCodecReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_REQ
CsrUint8 *CsrBtHfSetC2cAudioCodecReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfSetC2cAudioCodecReq *primitive;

    primitive = (CsrBtHfSetC2cAudioCodecReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_REQ */

#ifndef EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_REQ
void *CsrBtHfSetC2cAudioCodecReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfSetC2cAudioCodecReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfSetC2cAudioCodecReq *) CsrPmemZalloc(sizeof(CsrBtHfSetC2cAudioCodecReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_SET_C2C_AUDIO_CODEC_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_BATT_REQ
CsrSize CsrBtHfC2cBattReqSizeof(void *msg)
{
    CsrBtHfC2cBattReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_BATT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_BATT_REQ
CsrUint8 *CsrBtHfC2cBattReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfC2cBattReq *primitive;

    primitive = (CsrBtHfC2cBattReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_BATT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_BATT_REQ
void *CsrBtHfC2cBattReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfC2cBattReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfC2cBattReq *) CsrPmemZalloc(sizeof(CsrBtHfC2cBattReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_BATT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_TXT_IND
CsrSize CsrBtHfC2cTxtIndSizeof(void *msg)
{
    CsrBtHfC2cTxtInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->txtString));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_TXT_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_TXT_IND
CsrUint8 *CsrBtHfC2cTxtIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfC2cTxtInd *primitive;

    primitive = (CsrBtHfC2cTxtInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrMemCpySer(buffer, length, &primitive->txtString, sizeof(primitive->txtString));
    CsrCharStringSer(buffer, length, primitive->txtString);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_TXT_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_TXT_IND
void *CsrBtHfC2cTxtIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfC2cTxtInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfC2cTxtInd *) CsrPmemZalloc(sizeof(CsrBtHfC2cTxtInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->txtString), buffer, &offset, ((CsrSize) sizeof(primitive->txtString)));
    CsrCharStringDes(&primitive->txtString, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_TXT_IND */

#ifndef EXCLUDE_CSR_BT_HF_C2C_TXT_IND
void CsrBtHfC2cTxtIndSerFree(void *msg)
{
    CsrBtHfC2cTxtInd *primitive;

    primitive = (CsrBtHfC2cTxtInd *) msg;

    if(primitive->txtString != NULL)
    {
        CsrPmemFree(primitive->txtString);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_TXT_IND */

#ifndef EXCLUDE_CSR_BT_HF_BT_INPUT_REQ
CsrSize CsrBtHfBtInputReqSizeof(void *msg)
{
    CsrBtHfBtInputReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_BT_INPUT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_BT_INPUT_REQ
CsrUint8 *CsrBtHfBtInputReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfBtInputReq *primitive;

    primitive = (CsrBtHfBtInputReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->dataRequest);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_BT_INPUT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_BT_INPUT_REQ
void *CsrBtHfBtInputReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfBtInputReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfBtInputReq *) CsrPmemZalloc(sizeof(CsrBtHfBtInputReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->dataRequest, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_BT_INPUT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_IND
CsrSize CsrBtHfAudioAcceptConnectIndSizeof(void *msg)
{
    CsrBtHfAudioAcceptConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_IND
CsrUint8 *CsrBtHfAudioAcceptConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfAudioAcceptConnectInd *primitive;

    primitive = (CsrBtHfAudioAcceptConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->linkType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_IND
void *CsrBtHfAudioAcceptConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfAudioAcceptConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfAudioAcceptConnectInd *) CsrPmemZalloc(sizeof(CsrBtHfAudioAcceptConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->linkType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_ACCEPT_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HF_DEACTIVATE_REQ
CsrSize CsrBtHfDeactivateReqSizeof(void *msg)
{
    CsrBtHfDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DEACTIVATE_REQ
CsrUint8 *CsrBtHfDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfDeactivateReq *primitive;

    primitive = (CsrBtHfDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_DEACTIVATE_REQ
void *CsrBtHfDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtHfDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CALL_HANDLING_REQ
CsrSize CsrBtHfCallHandlingReqSizeof(void *msg)
{
    CsrBtHfCallHandlingReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_HANDLING_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CALL_HANDLING_REQ
CsrUint8 *CsrBtHfCallHandlingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfCallHandlingReq *primitive;

    primitive = (CsrBtHfCallHandlingReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->command);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->index);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_HANDLING_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CALL_HANDLING_REQ
void *CsrBtHfCallHandlingReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfCallHandlingReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfCallHandlingReq *) CsrPmemZalloc(sizeof(CsrBtHfCallHandlingReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->command, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->index, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_HANDLING_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CALL_ANSWER_REQ
CsrSize CsrBtHfCallAnswerReqSizeof(void *msg)
{
    CsrBtHfCallAnswerReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_ANSWER_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CALL_ANSWER_REQ
CsrUint8 *CsrBtHfCallAnswerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfCallAnswerReq *primitive;

    primitive = (CsrBtHfCallAnswerReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_ANSWER_REQ */

#ifndef EXCLUDE_CSR_BT_HF_CALL_ANSWER_REQ
void *CsrBtHfCallAnswerReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfCallAnswerReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfCallAnswerReq *) CsrPmemZalloc(sizeof(CsrBtHfCallAnswerReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_CALL_ANSWER_REQ */

#ifndef EXCLUDE_CSR_BT_HF_C2C_BATT_CFM
CsrSize CsrBtHfC2cBattCfmSizeof(void *msg)
{
    CsrBtHfC2cBattCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_BATT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_C2C_BATT_CFM
CsrUint8 *CsrBtHfC2cBattCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfC2cBattCfm *primitive;

    primitive = (CsrBtHfC2cBattCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeResultCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_BATT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_C2C_BATT_CFM
void *CsrBtHfC2cBattCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfC2cBattCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfC2cBattCfm *) CsrPmemZalloc(sizeof(CsrBtHfC2cBattCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeResultCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_C2C_BATT_CFM */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_REQ
CsrSize CsrBtHfAudioConnectReqSizeof(void *msg)
{
    CsrBtHfAudioConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->audioParametersLength * sizeof(CsrBtHfAudioLinkParameterListConfig)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_REQ
CsrUint8 *CsrBtHfAudioConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfAudioConnectReq *primitive;

    primitive = (CsrBtHfAudioConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->audioParametersLength);
    CsrMemCpySer(buffer, length, &primitive->audioParameters, sizeof(primitive->audioParameters));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmSlot);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmRealloc);
    if(primitive->audioParameters)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->audioParameters), ((CsrSize) (primitive->audioParametersLength * sizeof(CsrBtHfAudioLinkParameterListConfig))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_REQ
void *CsrBtHfAudioConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfAudioConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfAudioConnectReq *) CsrPmemZalloc(sizeof(CsrBtHfAudioConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->audioParametersLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->audioParameters), buffer, &offset, ((CsrSize) sizeof(primitive->audioParameters)));
    CsrUint8Des((CsrUint8 *) &primitive->pcmSlot, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmRealloc, buffer, &offset);
    if (primitive->audioParametersLength)
    {
        primitive->audioParameters = CsrPmemZalloc(((CsrUint32) (primitive->audioParametersLength * sizeof(CsrBtHfAudioLinkParameterListConfig))));
        CsrMemCpyDes(((void *) primitive->audioParameters), buffer, &offset, ((CsrSize) (primitive->audioParametersLength * sizeof(CsrBtHfAudioLinkParameterListConfig))));
    }
    else
    {
        primitive->audioParameters = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_REQ
void CsrBtHfAudioConnectReqSerFree(void *msg)
{
    CsrBtHfAudioConnectReq *primitive;

    primitive = (CsrBtHfAudioConnectReq *) msg;

    if(primitive->audioParameters != NULL)
    {
        CsrPmemFree(primitive->audioParameters);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HF_AUDIO_CONNECT_REQ */

#endif /* EXCLUDE_CSR_BT_HF_MODULE */
