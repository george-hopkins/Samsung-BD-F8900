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
#include "csr_bt_cm_prim.h"

#ifndef EXCLUDE_CSR_BT_CM_MODULE

void CsrBtCmPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_CFM
CsrSize CsrBtCmWriteAfhChannelAssessmentModeCfmSizeof(void *msg)
{
    CsrBtCmWriteAfhChannelAssessmentModeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_CFM
CsrUint8 *CsrBtCmWriteAfhChannelAssessmentModeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteAfhChannelAssessmentModeCfm *primitive;

    primitive = (CsrBtCmWriteAfhChannelAssessmentModeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_CFM
void *CsrBtCmWriteAfhChannelAssessmentModeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteAfhChannelAssessmentModeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteAfhChannelAssessmentModeCfm *) CsrPmemZalloc(sizeof(CsrBtCmWriteAfhChannelAssessmentModeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_REQ
CsrSize CsrBtCmWriteAfhChannelAssessmentModeReqSizeof(void *msg)
{
    CsrBtCmWriteAfhChannelAssessmentModeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_REQ
CsrUint8 *CsrBtCmWriteAfhChannelAssessmentModeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteAfhChannelAssessmentModeReq *primitive;

    primitive = (CsrBtCmWriteAfhChannelAssessmentModeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->classMode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_REQ
void *CsrBtCmWriteAfhChannelAssessmentModeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteAfhChannelAssessmentModeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteAfhChannelAssessmentModeReq *) CsrPmemZalloc(sizeof(CsrBtCmWriteAfhChannelAssessmentModeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->classMode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_AFH_CHANNEL_ASSESSMENT_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_IND
CsrSize CsrBtCmSdcUuid128SearchIndSizeof(void *msg)
{
    CsrBtCmSdcUuid128SearchInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->serviceHandleListCount * sizeof(CsrBtUuid32)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_IND
CsrUint8 *CsrBtCmSdcUuid128SearchIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcUuid128SearchInd *primitive;

    primitive = (CsrBtCmSdcUuid128SearchInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->service), ((CsrSize) (16)));
    CsrMemCpySer(buffer, length, &primitive->serviceHandleList, sizeof(primitive->serviceHandleList));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->serviceHandleListCount);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    if(primitive->serviceHandleList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->serviceHandleList), ((CsrSize) (primitive->serviceHandleListCount * sizeof(CsrBtUuid32))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_IND
void *CsrBtCmSdcUuid128SearchIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcUuid128SearchInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcUuid128SearchInd *) CsrPmemZalloc(sizeof(CsrBtCmSdcUuid128SearchInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->service), buffer, &offset, ((CsrSize) (16)));
    CsrMemCpyDes(((void *) &primitive->serviceHandleList), buffer, &offset, ((CsrSize) sizeof(primitive->serviceHandleList)));
    CsrUint16Des((CsrUint16 *) &primitive->serviceHandleListCount, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    if (primitive->serviceHandleListCount)
    {
        primitive->serviceHandleList = CsrPmemZalloc(((CsrUint32) (primitive->serviceHandleListCount * sizeof(CsrBtUuid32))));
        CsrMemCpyDes(((void *) primitive->serviceHandleList), buffer, &offset, ((CsrSize) (primitive->serviceHandleListCount * sizeof(CsrBtUuid32))));
    }
    else
    {
        primitive->serviceHandleList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_IND
void CsrBtCmSdcUuid128SearchIndSerFree(void *msg)
{
    CsrBtCmSdcUuid128SearchInd *primitive;

    primitive = (CsrBtCmSdcUuid128SearchInd *) msg;

    if(primitive->serviceHandleList != NULL)
    {
        CsrPmemFree(primitive->serviceHandleList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_REQ
CsrSize CsrBtCmWriteVoiceSettingsReqSizeof(void *msg)
{
    CsrBtCmWriteVoiceSettingsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_REQ
CsrUint8 *CsrBtCmWriteVoiceSettingsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteVoiceSettingsReq *primitive;

    primitive = (CsrBtCmWriteVoiceSettingsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->voiceSettings);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_REQ
void *CsrBtCmWriteVoiceSettingsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteVoiceSettingsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteVoiceSettingsReq *) CsrPmemZalloc(sizeof(CsrBtCmWriteVoiceSettingsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->voiceSettings, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_RES
CsrSize CsrBtCmSmUserPasskeyRequestResSizeof(void *msg)
{
    CsrBtCmSmUserPasskeyRequestRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_RES
CsrUint8 *CsrBtCmSmUserPasskeyRequestResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmUserPasskeyRequestRes *primitive;

    primitive = (CsrBtCmSmUserPasskeyRequestRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->numericValue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_RES
void *CsrBtCmSmUserPasskeyRequestResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmUserPasskeyRequestRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmUserPasskeyRequestRes *) CsrPmemZalloc(sizeof(CsrBtCmSmUserPasskeyRequestRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->numericValue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_BONDING_CANCEL_REQ
CsrSize CsrBtCmSmBondingCancelReqSizeof(void *msg)
{
    CsrBtCmSmBondingCancelReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_BONDING_CANCEL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_BONDING_CANCEL_REQ
CsrUint8 *CsrBtCmSmBondingCancelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmBondingCancelReq *primitive;

    primitive = (CsrBtCmSmBondingCancelReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->force);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_BONDING_CANCEL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_BONDING_CANCEL_REQ
void *CsrBtCmSmBondingCancelReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmBondingCancelReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmBondingCancelReq *) CsrPmemZalloc(sizeof(CsrBtCmSmBondingCancelReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->force, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_BONDING_CANCEL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_IND
CsrSize CsrBtCmA2dpBitRateIndSizeof(void *msg)
{
    CsrBtCmA2dpBitRateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_IND */

#ifndef EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_IND
CsrUint8 *CsrBtCmA2dpBitRateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmA2dpBitRateInd *primitive;

    primitive = (CsrBtCmA2dpBitRateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->streamIdx);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bitRate);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_IND */

#ifndef EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_IND
void *CsrBtCmA2dpBitRateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmA2dpBitRateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmA2dpBitRateInd *) CsrPmemZalloc(sizeof(CsrBtCmA2dpBitRateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->streamIdx, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bitRate, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_CFM
CsrSize CsrBtCmReadRemoteVersionCfmSizeof(void *msg)
{
    CsrBtCmReadRemoteVersionCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_CFM
CsrUint8 *CsrBtCmReadRemoteVersionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadRemoteVersionCfm *primitive;

    primitive = (CsrBtCmReadRemoteVersionCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->lmpVersion);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->manufacturerName);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->lmpSubversion);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_CFM
void *CsrBtCmReadRemoteVersionCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadRemoteVersionCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadRemoteVersionCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadRemoteVersionCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->lmpVersion, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->manufacturerName, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->lmpSubversion, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM
CsrSize CsrBtCmL2caConnectAcceptCfmSizeof(void *msg)
{
    CsrBtCmL2caConnectAcceptCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM
CsrUint8 *CsrBtCmL2caConnectAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caConnectAcceptCfm *primitive;

    primitive = (CsrBtCmL2caConnectAcceptCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remotePsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localMtu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM
void *CsrBtCmL2caConnectAcceptCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caConnectAcceptCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caConnectAcceptCfm *) CsrPmemZalloc(sizeof(CsrBtCmL2caConnectAcceptCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remotePsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localMtu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ
CsrSize CsrBtCmEirUpdateManufacturerDataReqSizeof(void *msg)
{
    CsrBtCmEirUpdateManufacturerDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->manufacturerDataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ
CsrUint8 *CsrBtCmEirUpdateManufacturerDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmEirUpdateManufacturerDataReq *primitive;

    primitive = (CsrBtCmEirUpdateManufacturerDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->manufacturerDataSettings);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->manufacturerDataLength);
    CsrMemCpySer(buffer, length, &primitive->manufacturerData, sizeof(primitive->manufacturerData));
    if(primitive->manufacturerData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->manufacturerData), ((CsrSize) (primitive->manufacturerDataLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ
void *CsrBtCmEirUpdateManufacturerDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmEirUpdateManufacturerDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmEirUpdateManufacturerDataReq *) CsrPmemZalloc(sizeof(CsrBtCmEirUpdateManufacturerDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->manufacturerDataSettings, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->manufacturerDataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->manufacturerData), buffer, &offset, ((CsrSize) sizeof(primitive->manufacturerData)));
    if (primitive->manufacturerDataLength)
    {
        primitive->manufacturerData = CsrPmemZalloc(((CsrUint32) (primitive->manufacturerDataLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->manufacturerData), buffer, &offset, ((CsrSize) (primitive->manufacturerDataLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->manufacturerData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ
void CsrBtCmEirUpdateManufacturerDataReqSerFree(void *msg)
{
    CsrBtCmEirUpdateManufacturerDataReq *primitive;

    primitive = (CsrBtCmEirUpdateManufacturerDataReq *) msg;

    if(primitive->manufacturerData != NULL)
    {
        CsrPmemFree(primitive->manufacturerData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_CFM
CsrSize CsrBtCmL2caConnectionlessDataCfmSizeof(void *msg)
{
    CsrBtCmL2caConnectionlessDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_CFM
CsrUint8 *CsrBtCmL2caConnectionlessDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caConnectionlessDataCfm *primitive;

    primitive = (CsrBtCmL2caConnectionlessDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connectionlessPsm);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_CFM
void *CsrBtCmL2caConnectionlessDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caConnectionlessDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caConnectionlessDataCfm *) CsrPmemZalloc(sizeof(CsrBtCmL2caConnectionlessDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connectionlessPsm, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_CM_ROLE_SWITCH_CONFIG_REQ
CsrSize CsrBtCmRoleSwitchConfigReqSizeof(void *msg)
{
    CsrBtCmRoleSwitchConfigReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_ROLE_SWITCH_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ROLE_SWITCH_CONFIG_REQ
CsrUint8 *CsrBtCmRoleSwitchConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmRoleSwitchConfigReq *primitive;

    primitive = (CsrBtCmRoleSwitchConfigReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_ROLE_SWITCH_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ROLE_SWITCH_CONFIG_REQ
void *CsrBtCmRoleSwitchConfigReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmRoleSwitchConfigReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmRoleSwitchConfigReq *) CsrPmemZalloc(sizeof(CsrBtCmRoleSwitchConfigReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_ROLE_SWITCH_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_CFM
CsrSize CsrBtCmWritePageToCfmSizeof(void *msg)
{
    CsrBtCmWritePageToCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_CFM
CsrUint8 *CsrBtCmWritePageToCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWritePageToCfm *primitive;

    primitive = (CsrBtCmWritePageToCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_CFM
void *CsrBtCmWritePageToCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWritePageToCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWritePageToCfm *) CsrPmemZalloc(sizeof(CsrBtCmWritePageToCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_CFM */

#ifndef EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_RES
CsrSize CsrBtCmMapScoPcmResSizeof(void *msg)
{
    CsrBtCmMapScoPcmRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->parmsLen * sizeof(CsrBtCmScoCommonParms)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_RES */

#ifndef EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_RES
CsrUint8 *CsrBtCmMapScoPcmResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmMapScoPcmRes *primitive;

    primitive = (CsrBtCmMapScoPcmRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->acceptResponse);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->parmsLen);
    CsrMemCpySer(buffer, length, &primitive->parms, sizeof(primitive->parms));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmSlot);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmReassign);
    if(primitive->parms)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->parms), ((CsrSize) (primitive->parmsLen * sizeof(CsrBtCmScoCommonParms))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_RES */

#ifndef EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_RES
void *CsrBtCmMapScoPcmResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmMapScoPcmRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmMapScoPcmRes *) CsrPmemZalloc(sizeof(CsrBtCmMapScoPcmRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->acceptResponse, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->parmsLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->parms), buffer, &offset, ((CsrSize) sizeof(primitive->parms)));
    CsrUint8Des((CsrUint8 *) &primitive->pcmSlot, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmReassign, buffer, &offset);
    if (primitive->parmsLen)
    {
        primitive->parms = CsrPmemZalloc(((CsrUint32) (primitive->parmsLen * sizeof(CsrBtCmScoCommonParms))));
        CsrMemCpyDes(((void *) primitive->parms), buffer, &offset, ((CsrSize) (primitive->parmsLen * sizeof(CsrBtCmScoCommonParms))));
    }
    else
    {
        primitive->parms = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_RES */

#ifndef EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_RES
void CsrBtCmMapScoPcmResSerFree(void *msg)
{
    CsrBtCmMapScoPcmRes *primitive;

    primitive = (CsrBtCmMapScoPcmRes *) msg;

    if(primitive->parms != NULL)
    {
        CsrPmemFree(primitive->parms);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_RES */

#ifndef EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPE_REQ
CsrSize CsrBtCmLogicalChannelTypeReqSizeof(void *msg)
{
    CsrBtCmLogicalChannelTypeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPE_REQ
CsrUint8 *CsrBtCmLogicalChannelTypeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLogicalChannelTypeReq *primitive;

    primitive = (CsrBtCmLogicalChannelTypeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->logicalChannelTypeMask);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPE_REQ
void *CsrBtCmLogicalChannelTypeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLogicalChannelTypeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLogicalChannelTypeReq *) CsrPmemZalloc(sizeof(CsrBtCmLogicalChannelTypeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->logicalChannelTypeMask, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_CFM
CsrSize CsrBtCmModeChangeCfmSizeof(void *msg)
{
    CsrBtCmModeChangeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_CFM
CsrUint8 *CsrBtCmModeChangeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmModeChangeCfm *primitive;

    primitive = (CsrBtCmModeChangeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_CFM
void *CsrBtCmModeChangeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmModeChangeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmModeChangeCfm *) CsrPmemZalloc(sizeof(CsrBtCmModeChangeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_REQ
CsrSize CsrBtCmL2caGetChannelInfoReqSizeof(void *msg)
{
    CsrBtCmL2caGetChannelInfoReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_REQ
CsrUint8 *CsrBtCmL2caGetChannelInfoReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caGetChannelInfoReq *primitive;

    primitive = (CsrBtCmL2caGetChannelInfoReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_REQ
void *CsrBtCmL2caGetChannelInfoReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caGetChannelInfoReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caGetChannelInfoReq *) CsrPmemZalloc(sizeof(CsrBtCmL2caGetChannelInfoReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_CFM
CsrSize CsrBtCmReadLinkPolicyCfmSizeof(void *msg)
{
    CsrBtCmReadLinkPolicyCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_CFM
CsrUint8 *CsrBtCmReadLinkPolicyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadLinkPolicyCfm *primitive;

    primitive = (CsrBtCmReadLinkPolicyCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->actualMode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->linkPolicySetting);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sniffSettings.max_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sniffSettings.min_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sniffSettings.attempt);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sniffSettings.timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->parkSettings.max_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->parkSettings.min_interval);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->parkSettings.park_idle_time);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_CFM
void *CsrBtCmReadLinkPolicyCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadLinkPolicyCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadLinkPolicyCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadLinkPolicyCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->actualMode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->linkPolicySetting, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sniffSettings.max_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sniffSettings.min_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sniffSettings.attempt, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sniffSettings.timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->parkSettings.max_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->parkSettings.min_interval, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->parkSettings.park_idle_time, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_REQ
CsrSize CsrBtCmSetAfhChannelClassReqSizeof(void *msg)
{
    CsrBtCmSetAfhChannelClassReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_REQ
CsrUint8 *CsrBtCmSetAfhChannelClassReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSetAfhChannelClassReq *primitive;

    primitive = (CsrBtCmSetAfhChannelClassReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrMemCpySer(buffer, length, ((const void *) primitive->map), ((CsrSize) (10)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_REQ
void *CsrBtCmSetAfhChannelClassReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSetAfhChannelClassReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSetAfhChannelClassReq *) CsrPmemZalloc(sizeof(CsrBtCmSetAfhChannelClassReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->map), buffer, &offset, ((CsrSize) (10)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_IND
CsrSize CsrBtCmScoDisconnectIndSizeof(void *msg)
{
    CsrBtCmScoDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_IND
CsrUint8 *CsrBtCmScoDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmScoDisconnectInd *primitive;

    primitive = (CsrBtCmScoDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->eScoHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_IND
void *CsrBtCmScoDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmScoDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmScoDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtCmScoDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->eScoHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_CFM
CsrSize CsrBtCmDataBufferEmptyCfmSizeof(void *msg)
{
    CsrBtCmDataBufferEmptyCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_CFM
CsrUint8 *CsrBtCmDataBufferEmptyCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDataBufferEmptyCfm *primitive;

    primitive = (CsrBtCmDataBufferEmptyCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_CFM
void *CsrBtCmDataBufferEmptyCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDataBufferEmptyCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDataBufferEmptyCfm *) CsrPmemZalloc(sizeof(CsrBtCmDataBufferEmptyCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CMP_IND
CsrSize CsrBtCmMoveChannelCmpIndSizeof(void *msg)
{
    CsrBtCmMoveChannelCmpInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CMP_IND */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CMP_IND
CsrUint8 *CsrBtCmMoveChannelCmpIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmMoveChannelCmpInd *primitive;

    primitive = (CsrBtCmMoveChannelCmpInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localControl);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CMP_IND */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CMP_IND
void *CsrBtCmMoveChannelCmpIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmMoveChannelCmpInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmMoveChannelCmpInd *) CsrPmemZalloc(sizeof(CsrBtCmMoveChannelCmpInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localControl, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CMP_IND */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_CLASS_OF_DEVICE_REQ
CsrSize CsrBtCmWriteClassOfDeviceReqSizeof(void *msg)
{
    CsrBtCmWriteClassOfDeviceReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_CLASS_OF_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_CLASS_OF_DEVICE_REQ
CsrUint8 *CsrBtCmWriteClassOfDeviceReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteClassOfDeviceReq *primitive;

    primitive = (CsrBtCmWriteClassOfDeviceReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->activePlayer);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_CLASS_OF_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_CLASS_OF_DEVICE_REQ
void *CsrBtCmWriteClassOfDeviceReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteClassOfDeviceReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteClassOfDeviceReq *) CsrPmemZalloc(sizeof(CsrBtCmWriteClassOfDeviceReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->activePlayer, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_CLASS_OF_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_REQ
CsrSize CsrBtCmConnectAcceptReqSizeof(void *msg)
{
    CsrBtCmConnectAcceptReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_REQ
CsrUint8 *CsrBtCmConnectAcceptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmConnectAcceptReq *primitive;

    primitive = (CsrBtCmConnectAcceptReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->profileMaxFrameSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->classOfDevice);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->profileUuid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modemStatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->breakSignal);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mscTimeout);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_REQ
void *CsrBtCmConnectAcceptReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmConnectAcceptReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmConnectAcceptReq *) CsrPmemZalloc(sizeof(CsrBtCmConnectAcceptReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->profileMaxFrameSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->classOfDevice, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->profileUuid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->modemStatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->breakSignal, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mscTimeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_CLOCK_REQ
CsrSize CsrBtCmReadClockReqSizeof(void *msg)
{
    CsrBtCmReadClockReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_CLOCK_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_CLOCK_REQ
CsrUint8 *CsrBtCmReadClockReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadClockReq *primitive;

    primitive = (CsrBtCmReadClockReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->whichClock);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_CLOCK_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_CLOCK_REQ
void *CsrBtCmReadClockReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadClockReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadClockReq *) CsrPmemZalloc(sizeof(CsrBtCmReadClockReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->whichClock, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_CLOCK_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_DELETE_STORE_LINK_KEY_REQ
CsrSize CsrBtCmSmDeleteStoreLinkKeyReqSizeof(void *msg)
{
    CsrBtCmSmDeleteStoreLinkKeyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_DELETE_STORE_LINK_KEY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_DELETE_STORE_LINK_KEY_REQ
CsrUint8 *CsrBtCmSmDeleteStoreLinkKeyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmDeleteStoreLinkKeyReq *primitive;

    primitive = (CsrBtCmSmDeleteStoreLinkKeyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flag);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_DELETE_STORE_LINK_KEY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_DELETE_STORE_LINK_KEY_REQ
void *CsrBtCmSmDeleteStoreLinkKeyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmDeleteStoreLinkKeyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmDeleteStoreLinkKeyReq *) CsrPmemZalloc(sizeof(CsrBtCmSmDeleteStoreLinkKeyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->flag, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_DELETE_STORE_LINK_KEY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_REGISTER_REQ
CsrSize CsrBtCmRegisterReqSizeof(void *msg)
{
    CsrBtCmRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_REGISTER_REQ
CsrUint8 *CsrBtCmRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmRegisterReq *primitive;

    primitive = (CsrBtCmRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_REGISTER_REQ
void *CsrBtCmRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmRegisterReq *) CsrPmemZalloc(sizeof(CsrBtCmRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_CHECK_SSR_REQ
CsrSize CsrBtCmDmCheckSsrReqSizeof(void *msg)
{
    CsrBtCmDmCheckSsrReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DM_CHECK_SSR_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_CHECK_SSR_REQ
CsrUint8 *CsrBtCmDmCheckSsrReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDmCheckSsrReq *primitive;

    primitive = (CsrBtCmDmCheckSsrReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_CHECK_SSR_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_CHECK_SSR_REQ
void *CsrBtCmDmCheckSsrReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDmCheckSsrReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDmCheckSsrReq *) CsrPmemZalloc(sizeof(CsrBtCmDmCheckSsrReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_CHECK_SSR_REQ */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_CFM
CsrSize CsrBtCmInquiryCfmSizeof(void *msg)
{
    CsrBtCmInquiryCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_CFM
CsrUint8 *CsrBtCmInquiryCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmInquiryCfm *primitive;

    primitive = (CsrBtCmInquiryCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_CFM
void *CsrBtCmInquiryCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmInquiryCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmInquiryCfm *) CsrPmemZalloc(sizeof(CsrBtCmInquiryCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_REQ
CsrSize CsrBtCmWriteInquiryscanSettingsReqSizeof(void *msg)
{
    CsrBtCmWriteInquiryscanSettingsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_REQ
CsrUint8 *CsrBtCmWriteInquiryscanSettingsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteInquiryscanSettingsReq *primitive;

    primitive = (CsrBtCmWriteInquiryscanSettingsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->window);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_REQ
void *CsrBtCmWriteInquiryscanSettingsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteInquiryscanSettingsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteInquiryscanSettingsReq *) CsrPmemZalloc(sizeof(CsrBtCmWriteInquiryscanSettingsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->window, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_REQ
CsrSize CsrBtCmReadLocalVersionReqSizeof(void *msg)
{
    CsrBtCmReadLocalVersionReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_REQ
CsrUint8 *CsrBtCmReadLocalVersionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadLocalVersionReq *primitive;

    primitive = (CsrBtCmReadLocalVersionReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_REQ
void *CsrBtCmReadLocalVersionReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadLocalVersionReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadLocalVersionReq *) CsrPmemZalloc(sizeof(CsrBtCmReadLocalVersionReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDS_EXT_REGISTER_CFM
CsrSize CsrBtCmSdsExtRegisterCfmSizeof(void *msg)
{
    CsrBtCmSdsExtRegisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_EXT_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDS_EXT_REGISTER_CFM
CsrUint8 *CsrBtCmSdsExtRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdsExtRegisterCfm *primitive;

    primitive = (CsrBtCmSdsExtRegisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceRecHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_EXT_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDS_EXT_REGISTER_CFM
void *CsrBtCmSdsExtRegisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdsExtRegisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdsExtRegisterCfm *) CsrPmemZalloc(sizeof(CsrBtCmSdsExtRegisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceRecHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_EXT_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_CFM
CsrSize CsrBtCmReadAdvertisingChTxPowerCfmSizeof(void *msg)
{
    CsrBtCmReadAdvertisingChTxPowerCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_CFM
CsrUint8 *CsrBtCmReadAdvertisingChTxPowerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadAdvertisingChTxPowerCfm *primitive;

    primitive = (CsrBtCmReadAdvertisingChTxPowerCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->txPower);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_CFM
void *CsrBtCmReadAdvertisingChTxPowerCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadAdvertisingChTxPowerCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadAdvertisingChTxPowerCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadAdvertisingChTxPowerCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->txPower, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_RES
CsrSize CsrBtCmMoveChannelResSizeof(void *msg)
{
    CsrBtCmMoveChannelRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_RES */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_RES
CsrUint8 *CsrBtCmMoveChannelResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmMoveChannelRes *primitive;

    primitive = (CsrBtCmMoveChannelRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_RES */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_RES
void *CsrBtCmMoveChannelResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmMoveChannelRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmMoveChannelRes *) CsrPmemZalloc(sizeof(CsrBtCmMoveChannelRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_RES */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_CFM
CsrSize CsrBtCmSetEventFilterCodCfmSizeof(void *msg)
{
    CsrBtCmSetEventFilterCodCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_CFM
CsrUint8 *CsrBtCmSetEventFilterCodCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSetEventFilterCodCfm *primitive;

    primitive = (CsrBtCmSetEventFilterCodCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_CFM
void *CsrBtCmSetEventFilterCodCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSetEventFilterCodCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSetEventFilterCodCfm *) CsrPmemZalloc(sizeof(CsrBtCmSetEventFilterCodCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_CFM */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_IND
CsrSize CsrBtCmModeChangeIndSizeof(void *msg)
{
    CsrBtCmModeChangeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_IND
CsrUint8 *CsrBtCmModeChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmModeChangeInd *primitive;

    primitive = (CsrBtCmModeChangeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_IND
void *CsrBtCmModeChangeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmModeChangeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmModeChangeInd *) CsrPmemZalloc(sizeof(CsrBtCmModeChangeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_SM_UNREGISTER_REQ
CsrSize CsrBtCmSmUnregisterReqSizeof(void *msg)
{
    CsrBtCmSmUnregisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_UNREGISTER_REQ
CsrUint8 *CsrBtCmSmUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmUnregisterReq *primitive;

    primitive = (CsrBtCmSmUnregisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->protocolId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->channel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_UNREGISTER_REQ
void *CsrBtCmSmUnregisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmUnregisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmUnregisterReq *) CsrPmemZalloc(sizeof(CsrBtCmSmUnregisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->protocolId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->channel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_CFM
CsrSize CsrBtCmReadAfhChannelAssessmentModeCfmSizeof(void *msg)
{
    CsrBtCmReadAfhChannelAssessmentModeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_CFM
CsrUint8 *CsrBtCmReadAfhChannelAssessmentModeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadAfhChannelAssessmentModeCfm *primitive;

    primitive = (CsrBtCmReadAfhChannelAssessmentModeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->classMode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_CFM
void *CsrBtCmReadAfhChannelAssessmentModeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadAfhChannelAssessmentModeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadAfhChannelAssessmentModeCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadAfhChannelAssessmentModeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->classMode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_CFM
CsrSize CsrBtCmReadFailedContactCounterCfmSizeof(void *msg)
{
    CsrBtCmReadFailedContactCounterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_CFM
CsrUint8 *CsrBtCmReadFailedContactCounterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadFailedContactCounterCfm *primitive;

    primitive = (CsrBtCmReadFailedContactCounterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->failedContactCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_CFM
void *CsrBtCmReadFailedContactCounterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadFailedContactCounterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadFailedContactCounterCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadFailedContactCounterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->failedContactCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DATA_CFM
CsrSize CsrBtCmDataCfmSizeof(void *msg)
{
    CsrBtCmDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DATA_CFM
CsrUint8 *CsrBtCmDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDataCfm *primitive;

    primitive = (CsrBtCmDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DATA_CFM
void *CsrBtCmDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDataCfm *) CsrPmemZalloc(sizeof(CsrBtCmDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_REQ
CsrSize CsrBtCmBnepDisconnectReqSizeof(void *msg)
{
    CsrBtCmBnepDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_REQ
CsrUint8 *CsrBtCmBnepDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBnepDisconnectReq *primitive;

    primitive = (CsrBtCmBnepDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->id);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_REQ
void *CsrBtCmBnepDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtCmBnepDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SYNC_CONNECT_IND
CsrSize CsrBtCmSyncConnectIndSizeof(void *msg)
{
    CsrBtCmSyncConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SYNC_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_SYNC_CONNECT_IND
CsrUint8 *CsrBtCmSyncConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSyncConnectInd *primitive;

    primitive = (CsrBtCmSyncConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->incoming);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->syncHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->linkType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->txInterval);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->weSco);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rxPacketLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->txPacketLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->airMode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->packetType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->txBdw);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->rxBdw);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxLatency);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reTxEffort);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->voiceSettings);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SYNC_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_SYNC_CONNECT_IND
void *CsrBtCmSyncConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSyncConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSyncConnectInd *) CsrPmemZalloc(sizeof(CsrBtCmSyncConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->incoming, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->syncHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->linkType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->txInterval, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->weSco, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rxPacketLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->txPacketLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->airMode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->packetType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->txBdw, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->rxBdw, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxLatency, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reTxEffort, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->voiceSettings, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SYNC_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_CFM
CsrSize CsrBtCmReadLocalNameCfmSizeof(void *msg)
{
    CsrBtCmReadLocalNameCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->localName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_CFM
CsrUint8 *CsrBtCmReadLocalNameCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadLocalNameCfm *primitive;

    primitive = (CsrBtCmReadLocalNameCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->localName, sizeof(primitive->localName));
    CsrUtf8StringSer(buffer, length, primitive->localName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_CFM
void *CsrBtCmReadLocalNameCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadLocalNameCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadLocalNameCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadLocalNameCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->localName), buffer, &offset, ((CsrSize) sizeof(primitive->localName)));
    CsrUtf8StringDes(&primitive->localName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_CFM
void CsrBtCmReadLocalNameCfmSerFree(void *msg)
{
    CsrBtCmReadLocalNameCfm *primitive;

    primitive = (CsrBtCmReadLocalNameCfm *) msg;

    if(primitive->localName != NULL)
    {
        CsrPmemFree(primitive->localName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_CM_REMOTE_VERSION_IND
CsrSize CsrBtCmRemoteVersionIndSizeof(void *msg)
{
    CsrBtCmRemoteVersionInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_REMOTE_VERSION_IND */

#ifndef EXCLUDE_CSR_BT_CM_REMOTE_VERSION_IND
CsrUint8 *CsrBtCmRemoteVersionIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmRemoteVersionInd *primitive;

    primitive = (CsrBtCmRemoteVersionInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->lmpVersion);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->manufacturerName);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->lmpSubversion);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_REMOTE_VERSION_IND */

#ifndef EXCLUDE_CSR_BT_CM_REMOTE_VERSION_IND
void *CsrBtCmRemoteVersionIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmRemoteVersionInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmRemoteVersionInd *) CsrPmemZalloc(sizeof(CsrBtCmRemoteVersionInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->lmpVersion, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->manufacturerName, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->lmpSubversion, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_REMOTE_VERSION_IND */

#ifndef EXCLUDE_CSR_BT_CM_DM_WRITE_CACHE_PARAMS_REQ
CsrSize CsrBtCmDmWriteCacheParamsReqSizeof(void *msg)
{
    CsrBtCmDmWriteCacheParamsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DM_WRITE_CACHE_PARAMS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_WRITE_CACHE_PARAMS_REQ
CsrUint8 *CsrBtCmDmWriteCacheParamsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDmWriteCacheParamsReq *primitive;

    primitive = (CsrBtCmDmWriteCacheParamsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->activePlayer);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->devAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->devAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->devAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->clockOffset);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pageScanMode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pageScanRepMode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_WRITE_CACHE_PARAMS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_WRITE_CACHE_PARAMS_REQ
void *CsrBtCmDmWriteCacheParamsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDmWriteCacheParamsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDmWriteCacheParamsReq *) CsrPmemZalloc(sizeof(CsrBtCmDmWriteCacheParamsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->activePlayer, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->devAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->devAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->devAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->clockOffset, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pageScanMode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pageScanRepMode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_WRITE_CACHE_PARAMS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_EVENT_CONNECTION_IND
CsrSize CsrBtCmLeEventConnectionIndSizeof(void *msg)
{
    CsrBtCmLeEventConnectionInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_EVENT_CONNECTION_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_EVENT_CONNECTION_IND
CsrUint8 *CsrBtCmLeEventConnectionIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeEventConnectionInd *primitive;

    primitive = (CsrBtCmLeEventConnectionInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->event);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->latency);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accuracy);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_EVENT_CONNECTION_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_EVENT_CONNECTION_IND
void *CsrBtCmLeEventConnectionIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeEventConnectionInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeEventConnectionInd *) CsrPmemZalloc(sizeof(CsrBtCmLeEventConnectionInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->event, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->latency, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accuracy, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_EVENT_CONNECTION_IND */

#ifndef EXCLUDE_CSR_BT_CM_REGISTER_CFM
CsrSize CsrBtCmRegisterCfmSizeof(void *msg)
{
    CsrBtCmRegisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_REGISTER_CFM
CsrUint8 *CsrBtCmRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmRegisterCfm *primitive;

    primitive = (CsrBtCmRegisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_REGISTER_CFM
void *CsrBtCmRegisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmRegisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmRegisterCfm *) CsrPmemZalloc(sizeof(CsrBtCmRegisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SM_ENCRYPTION_REQ
CsrSize CsrBtCmSmEncryptionReqSizeof(void *msg)
{
    CsrBtCmSmEncryptionReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_ENCRYPTION_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_ENCRYPTION_REQ
CsrUint8 *CsrBtCmSmEncryptionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmEncryptionReq *primitive;

    primitive = (CsrBtCmSmEncryptionReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->encryptionMode);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_ENCRYPTION_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_ENCRYPTION_REQ
void *CsrBtCmSmEncryptionReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmEncryptionReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmEncryptionReq *) CsrPmemZalloc(sizeof(CsrBtCmSmEncryptionReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->encryptionMode, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_ENCRYPTION_REQ */

#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_CFM
CsrSize CsrBtCmPortnegCfmSizeof(void *msg)
{
    CsrBtCmPortnegCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_CFM */

#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_CFM
CsrUint8 *CsrBtCmPortnegCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmPortnegCfm *primitive;

    primitive = (CsrBtCmPortnegCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->portPar.parameter_mask);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_CFM */

#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_CFM
void *CsrBtCmPortnegCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmPortnegCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmPortnegCfm *) CsrPmemZalloc(sizeof(CsrBtCmPortnegCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->portPar.parameter_mask, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_TEST_END_REQ
CsrSize CsrBtCmLeTestEndReqSizeof(void *msg)
{
    CsrBtCmLeTestEndReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_TEST_END_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_TEST_END_REQ
CsrUint8 *CsrBtCmLeTestEndReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeTestEndReq *primitive;

    primitive = (CsrBtCmLeTestEndReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_TEST_END_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_TEST_END_REQ
void *CsrBtCmLeTestEndReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeTestEndReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeTestEndReq *) CsrPmemZalloc(sizeof(CsrBtCmLeTestEndReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_TEST_END_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_READ_DEVICE_REQ
CsrSize CsrBtCmSmReadDeviceReqSizeof(void *msg)
{
    CsrBtCmSmReadDeviceReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_READ_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_READ_DEVICE_REQ
CsrUint8 *CsrBtCmSmReadDeviceReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmReadDeviceReq *primitive;

    primitive = (CsrBtCmSmReadDeviceReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_READ_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_READ_DEVICE_REQ
void *CsrBtCmSmReadDeviceReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmReadDeviceReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmReadDeviceReq *) CsrPmemZalloc(sizeof(CsrBtCmSmReadDeviceReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_READ_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ
CsrSize CsrBtCmBnepExtendedMulticastDataReqSizeof(void *msg)
{
    CsrBtCmBnepExtendedMulticastDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ
CsrUint8 *CsrBtCmBnepExtendedMulticastDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBnepExtendedMulticastDataReq *primitive;

    primitive = (CsrBtCmBnepExtendedMulticastDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->idNot);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->etherType);
    CsrMemCpySer(buffer, length, ((const void *) primitive->dstAddr.w), ((CsrSize) 2 * (3)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->srcAddr.w), ((CsrSize) 2 * (3)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ
void *CsrBtCmBnepExtendedMulticastDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepExtendedMulticastDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepExtendedMulticastDataReq *) CsrPmemZalloc(sizeof(CsrBtCmBnepExtendedMulticastDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->idNot, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->etherType, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->dstAddr.w), buffer, &offset, ((CsrSize) 2 * (3)));
    CsrMemCpyDes(((void *) primitive->srcAddr.w), buffer, &offset, ((CsrSize) 2 * (3)));
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->length)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ
void CsrBtCmBnepExtendedMulticastDataReqSerFree(void *msg)
{
    CsrBtCmBnepExtendedMulticastDataReq *primitive;

    primitive = (CsrBtCmBnepExtendedMulticastDataReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_MULTICAST_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM
CsrSize CsrBtCmReadLocalBdAddrCfmSizeof(void *msg)
{
    CsrBtCmReadLocalBdAddrCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM
CsrUint8 *CsrBtCmReadLocalBdAddrCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadLocalBdAddrCfm *primitive;

    primitive = (CsrBtCmReadLocalBdAddrCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM
void *CsrBtCmReadLocalBdAddrCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadLocalBdAddrCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadLocalBdAddrCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadLocalBdAddrCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_REQ
CsrSize CsrBtCmScoDisconnectReqSizeof(void *msg)
{
    CsrBtCmScoDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_REQ
CsrUint8 *CsrBtCmScoDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmScoDisconnectReq *primitive;

    primitive = (CsrBtCmScoDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_REQ
void *CsrBtCmScoDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmScoDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmScoDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtCmScoDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_REQ
CsrSize CsrBtCmConnectReqSizeof(void *msg)
{
    CsrBtCmConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_REQ
CsrUint8 *CsrBtCmConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmConnectReq *primitive;

    primitive = (CsrBtCmConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerCh);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->profileMaxFrameSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->requestPortPar);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->validPortPar);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->portPar.parameter_mask);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modemStatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->breakSignal);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mscTimeout);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_REQ
void *CsrBtCmConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmConnectReq *) CsrPmemZalloc(sizeof(CsrBtCmConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerCh, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->profileMaxFrameSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->requestPortPar, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->validPortPar, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->portPar.parameter_mask, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->modemStatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->breakSignal, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mscTimeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_REQ
CsrSize CsrBtCmGetLinkQualityReqSizeof(void *msg)
{
    CsrBtCmGetLinkQualityReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_REQ
CsrUint8 *CsrBtCmGetLinkQualityReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmGetLinkQualityReq *primitive;

    primitive = (CsrBtCmGetLinkQualityReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_REQ
void *CsrBtCmGetLinkQualityReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmGetLinkQualityReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmGetLinkQualityReq *) CsrPmemZalloc(sizeof(CsrBtCmGetLinkQualityReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_REQ
CsrSize CsrBtCmSetLocalNameReqSizeof(void *msg)
{
    CsrBtCmSetLocalNameReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->friendlyName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_REQ
CsrUint8 *CsrBtCmSetLocalNameReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSetLocalNameReq *primitive;

    primitive = (CsrBtCmSetLocalNameReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrMemCpySer(buffer, length, &primitive->friendlyName, sizeof(primitive->friendlyName));
    CsrUtf8StringSer(buffer, length, primitive->friendlyName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_REQ
void *CsrBtCmSetLocalNameReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSetLocalNameReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSetLocalNameReq *) CsrPmemZalloc(sizeof(CsrBtCmSetLocalNameReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->friendlyName), buffer, &offset, ((CsrSize) sizeof(primitive->friendlyName)));
    CsrUtf8StringDes(&primitive->friendlyName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_REQ
void CsrBtCmSetLocalNameReqSerFree(void *msg)
{
    CsrBtCmSetLocalNameReq *primitive;

    primitive = (CsrBtCmSetLocalNameReq *) msg;

    if(primitive->friendlyName != NULL)
    {
        CsrPmemFree(primitive->friendlyName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RFC_SEARCH_REQ
CsrSize CsrBtCmSdcRfcSearchReqSizeof(void *msg)
{
    CsrBtCmSdcRfcSearchReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->serviceListSize * sizeof(CsrBtUuid32)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_RFC_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RFC_SEARCH_REQ
CsrUint8 *CsrBtCmSdcRfcSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcRfcSearchReq *primitive;

    primitive = (CsrBtCmSdcRfcSearchReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->serviceList, sizeof(primitive->serviceList));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serviceListSize);
    if(primitive->serviceList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->serviceList), ((CsrSize) (primitive->serviceListSize * sizeof(CsrBtUuid32))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_RFC_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RFC_SEARCH_REQ
void *CsrBtCmSdcRfcSearchReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcRfcSearchReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcRfcSearchReq *) CsrPmemZalloc(sizeof(CsrBtCmSdcRfcSearchReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceList), buffer, &offset, ((CsrSize) sizeof(primitive->serviceList)));
    CsrUint8Des((CsrUint8 *) &primitive->serviceListSize, buffer, &offset);
    if (primitive->serviceListSize)
    {
        primitive->serviceList = CsrPmemZalloc(((CsrUint32) (primitive->serviceListSize * sizeof(CsrBtUuid32))));
        CsrMemCpyDes(((void *) primitive->serviceList), buffer, &offset, ((CsrSize) (primitive->serviceListSize * sizeof(CsrBtUuid32))));
    }
    else
    {
        primitive->serviceList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_RFC_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RFC_SEARCH_REQ
void CsrBtCmSdcRfcSearchReqSerFree(void *msg)
{
    CsrBtCmSdcRfcSearchReq *primitive;

    primitive = (CsrBtCmSdcRfcSearchReq *) msg;

    if(primitive->serviceList != NULL)
    {
        CsrPmemFree(primitive->serviceList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_RFC_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_REQ
CsrSize CsrBtCmRoleDiscoveryReqSizeof(void *msg)
{
    CsrBtCmRoleDiscoveryReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_REQ
CsrUint8 *CsrBtCmRoleDiscoveryReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmRoleDiscoveryReq *primitive;

    primitive = (CsrBtCmRoleDiscoveryReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_REQ
void *CsrBtCmRoleDiscoveryReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmRoleDiscoveryReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmRoleDiscoveryReq *) CsrPmemZalloc(sizeof(CsrBtCmRoleDiscoveryReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_PIN_REQUEST_RES
CsrSize CsrBtCmSmPinRequestResSizeof(void *msg)
{
    CsrBtCmSmPinRequestRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_PIN_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_PIN_REQUEST_RES
CsrUint8 *CsrBtCmSmPinRequestResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmPinRequestRes *primitive;

    primitive = (CsrBtCmSmPinRequestRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pinLength);
    CsrMemCpySer(buffer, length, ((const void *) primitive->pin), ((CsrSize) (16)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_PIN_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_PIN_REQUEST_RES
void *CsrBtCmSmPinRequestResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmPinRequestRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmPinRequestRes *) CsrPmemZalloc(sizeof(CsrBtCmSmPinRequestRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pinLength, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->pin), buffer, &offset, ((CsrSize) (16)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_PIN_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SDC_OPEN_REQ
CsrSize CsrBtCmSdcOpenReqSizeof(void *msg)
{
    CsrBtCmSdcOpenReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_OPEN_REQ
CsrUint8 *CsrBtCmSdcOpenReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcOpenReq *primitive;

    primitive = (CsrBtCmSdcOpenReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_OPEN_REQ
void *CsrBtCmSdcOpenReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcOpenReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcOpenReq *) CsrPmemZalloc(sizeof(CsrBtCmSdcOpenReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_REQ
CsrSize CsrBtCmWriteInquiryscanTypeReqSizeof(void *msg)
{
    CsrBtCmWriteInquiryscanTypeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_REQ
CsrUint8 *CsrBtCmWriteInquiryscanTypeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteInquiryscanTypeReq *primitive;

    primitive = (CsrBtCmWriteInquiryscanTypeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scanType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_REQ
void *CsrBtCmWriteInquiryscanTypeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteInquiryscanTypeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteInquiryscanTypeReq *) CsrPmemZalloc(sizeof(CsrBtCmWriteInquiryscanTypeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scanType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_CFM
CsrSize CsrBtCmReadScanEnableCfmSizeof(void *msg)
{
    CsrBtCmReadScanEnableCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_CFM
CsrUint8 *CsrBtCmReadScanEnableCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadScanEnableCfm *primitive;

    primitive = (CsrBtCmReadScanEnableCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scanEnable);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_CFM
void *CsrBtCmReadScanEnableCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadScanEnableCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadScanEnableCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadScanEnableCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scanEnable, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_REQ
CsrSize CsrBtCmDisableDutModeReqSizeof(void *msg)
{
    CsrBtCmDisableDutModeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_REQ
CsrUint8 *CsrBtCmDisableDutModeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDisableDutModeReq *primitive;

    primitive = (CsrBtCmDisableDutModeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_REQ
void *CsrBtCmDisableDutModeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDisableDutModeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDisableDutModeReq *) CsrPmemZalloc(sizeof(CsrBtCmDisableDutModeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_NEG_RES
CsrSize CsrBtCmSmUserPasskeyRequestNegResSizeof(void *msg)
{
    CsrBtCmSmUserPasskeyRequestNegRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_NEG_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_NEG_RES
CsrUint8 *CsrBtCmSmUserPasskeyRequestNegResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmUserPasskeyRequestNegRes *primitive;

    primitive = (CsrBtCmSmUserPasskeyRequestNegRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->numericValue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_NEG_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_NEG_RES
void *CsrBtCmSmUserPasskeyRequestNegResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmUserPasskeyRequestNegRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmUserPasskeyRequestNegRes *) CsrPmemZalloc(sizeof(CsrBtCmSmUserPasskeyRequestNegRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->numericValue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_PASSKEY_REQUEST_NEG_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_HOUSE_CLEANING
CsrSize CsrBtCmSmHouseCleaningSizeof(void *msg)
{
    CsrBtCmSmHouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_CM_SM_HOUSE_CLEANING
CsrUint8 *CsrBtCmSmHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmHouseCleaning *primitive;

    primitive = (CsrBtCmSmHouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_CM_SM_HOUSE_CLEANING
void *CsrBtCmSmHouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmHouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmHouseCleaning *) CsrPmemZalloc(sizeof(CsrBtCmSmHouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_EXT_REQ
CsrSize CsrBtCmConnectExtReqSizeof(void *msg)
{
    CsrBtCmConnectExtReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_EXT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_EXT_REQ
CsrUint8 *CsrBtCmConnectExtReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmConnectExtReq *primitive;

    primitive = (CsrBtCmConnectExtReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerCh);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->remoteServerCh);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->profileMaxFrameSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->requestPortPar);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->validPortPar);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->portPar.parameter_mask);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modemStatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->breakSignal);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mscTimeout);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_EXT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_EXT_REQ
void *CsrBtCmConnectExtReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmConnectExtReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmConnectExtReq *) CsrPmemZalloc(sizeof(CsrBtCmConnectExtReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerCh, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->remoteServerCh, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->profileMaxFrameSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->requestPortPar, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->validPortPar, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->portPar.parameter_mask, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->modemStatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->breakSignal, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mscTimeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_EXT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_CFM
CsrSize CsrBtCmSetEventMaskCfmSizeof(void *msg)
{
    CsrBtCmSetEventMaskCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_CFM
CsrUint8 *CsrBtCmSetEventMaskCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSetEventMaskCfm *primitive;

    primitive = (CsrBtCmSetEventMaskCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->eventMask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_CFM
void *CsrBtCmSetEventMaskCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSetEventMaskCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSetEventMaskCfm *) CsrPmemZalloc(sizeof(CsrBtCmSetEventMaskCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->eventMask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_REQ
CsrSize CsrBtCmDataBufferEmptyReqSizeof(void *msg)
{
    CsrBtCmDataBufferEmptyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_REQ
CsrUint8 *CsrBtCmDataBufferEmptyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDataBufferEmptyReq *primitive;

    primitive = (CsrBtCmDataBufferEmptyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_REQ
void *CsrBtCmDataBufferEmptyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDataBufferEmptyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDataBufferEmptyReq *) CsrPmemZalloc(sizeof(CsrBtCmDataBufferEmptyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_BUFFER_EMPTY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_CFM
CsrSize CsrBtCmSdcSearchCfmSizeof(void *msg)
{
    CsrBtCmSdcSearchCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_CFM
CsrUint8 *CsrBtCmSdcSearchCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcSearchCfm *primitive;

    primitive = (CsrBtCmSdcSearchCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_CFM
void *CsrBtCmSdcSearchCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcSearchCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcSearchCfm *) CsrPmemZalloc(sizeof(CsrBtCmSdcSearchCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_REQ
CsrSize CsrBtCmSdcSearchReqSizeof(void *msg)
{
    CsrBtCmSdcSearchReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->serviceListSize * sizeof(CsrBtUuid32)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_REQ
CsrUint8 *CsrBtCmSdcSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcSearchReq *primitive;

    primitive = (CsrBtCmSdcSearchReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->serviceList, sizeof(primitive->serviceList));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serviceListSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->extendedUuidSearch);
    if(primitive->serviceList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->serviceList), ((CsrSize) (primitive->serviceListSize * sizeof(CsrBtUuid32))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_REQ
void *CsrBtCmSdcSearchReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcSearchReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcSearchReq *) CsrPmemZalloc(sizeof(CsrBtCmSdcSearchReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceList), buffer, &offset, ((CsrSize) sizeof(primitive->serviceList)));
    CsrUint8Des((CsrUint8 *) &primitive->serviceListSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->extendedUuidSearch, buffer, &offset);
    if (primitive->serviceListSize)
    {
        primitive->serviceList = CsrPmemZalloc(((CsrUint32) (primitive->serviceListSize * sizeof(CsrBtUuid32))));
        CsrMemCpyDes(((void *) primitive->serviceList), buffer, &offset, ((CsrSize) (primitive->serviceListSize * sizeof(CsrBtUuid32))));
    }
    else
    {
        primitive->serviceList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_REQ
void CsrBtCmSdcSearchReqSerFree(void *msg)
{
    CsrBtCmSdcSearchReq *primitive;

    primitive = (CsrBtCmSdcSearchReq *) msg;

    if(primitive->serviceList != NULL)
    {
        CsrPmemFree(primitive->serviceList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_RES
CsrSize CsrBtCmLeAcceptConnparamUpdateResSizeof(void *msg)
{
    CsrBtCmLeAcceptConnparamUpdateRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_RES */

#ifndef EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_RES
CsrUint8 *CsrBtCmLeAcceptConnparamUpdateResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeAcceptConnparamUpdateRes *primitive;

    primitive = (CsrBtCmLeAcceptConnparamUpdateRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->l2caSignalId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connIntervalMin);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connIntervalMax);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connLatency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supervisionTimeout);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_RES */

#ifndef EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_RES
void *CsrBtCmLeAcceptConnparamUpdateResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeAcceptConnparamUpdateRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeAcceptConnparamUpdateRes *) CsrPmemZalloc(sizeof(CsrBtCmLeAcceptConnparamUpdateRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->l2caSignalId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connIntervalMin, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connIntervalMax, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connLatency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supervisionTimeout, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_RES */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_REQ
CsrSize CsrBtCmWriteLinkPolicyReqSizeof(void *msg)
{
    CsrBtCmWriteLinkPolicyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->sniffSettingsCount * sizeof(CsrBtSniffSettings)) + (primitive->parkSettingsCount * sizeof(CsrBtParkSettings)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_REQ
CsrUint8 *CsrBtCmWriteLinkPolicyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteLinkPolicyReq *primitive;

    primitive = (CsrBtCmWriteLinkPolicyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->setupLinkPolicySetting);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->linkPolicySetting);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sniffSettingsCount);
    CsrMemCpySer(buffer, length, &primitive->sniffSettings, sizeof(primitive->sniffSettings));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parkSettingsCount);
    CsrMemCpySer(buffer, length, &primitive->parkSettings, sizeof(primitive->parkSettings));
    if(primitive->sniffSettings)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->sniffSettings), ((CsrSize) (primitive->sniffSettingsCount * sizeof(CsrBtSniffSettings))));
    }
    if(primitive->parkSettings)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->parkSettings), ((CsrSize) (primitive->parkSettingsCount * sizeof(CsrBtParkSettings))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_REQ
void *CsrBtCmWriteLinkPolicyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteLinkPolicyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteLinkPolicyReq *) CsrPmemZalloc(sizeof(CsrBtCmWriteLinkPolicyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->setupLinkPolicySetting, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->linkPolicySetting, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->sniffSettingsCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->sniffSettings), buffer, &offset, ((CsrSize) sizeof(primitive->sniffSettings)));
    CsrUint8Des((CsrUint8 *) &primitive->parkSettingsCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->parkSettings), buffer, &offset, ((CsrSize) sizeof(primitive->parkSettings)));
    if (primitive->sniffSettingsCount)
    {
        primitive->sniffSettings = CsrPmemZalloc(((CsrUint32) (primitive->sniffSettingsCount * sizeof(CsrBtSniffSettings))));
        CsrMemCpyDes(((void *) primitive->sniffSettings), buffer, &offset, ((CsrSize) (primitive->sniffSettingsCount * sizeof(CsrBtSniffSettings))));
    }
    else
    {
        primitive->sniffSettings = NULL;
    }
    if (primitive->parkSettingsCount)
    {
        primitive->parkSettings = CsrPmemZalloc(((CsrUint32) (primitive->parkSettingsCount * sizeof(CsrBtParkSettings))));
        CsrMemCpyDes(((void *) primitive->parkSettings), buffer, &offset, ((CsrSize) (primitive->parkSettingsCount * sizeof(CsrBtParkSettings))));
    }
    else
    {
        primitive->parkSettings = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_REQ
void CsrBtCmWriteLinkPolicyReqSerFree(void *msg)
{
    CsrBtCmWriteLinkPolicyReq *primitive;

    primitive = (CsrBtCmWriteLinkPolicyReq *) msg;

    if(primitive->sniffSettings != NULL)
    {
        CsrPmemFree(primitive->sniffSettings);
    }
    if(primitive->parkSettings != NULL)
    {
        CsrPmemFree(primitive->parkSettings);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_REGISTER_HANDLER_REQ
CsrSize CsrBtCmRegisterHandlerReqSizeof(void *msg)
{
    CsrBtCmRegisterHandlerReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_REGISTER_HANDLER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_REGISTER_HANDLER_REQ
CsrUint8 *CsrBtCmRegisterHandlerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmRegisterHandlerReq *primitive;

    primitive = (CsrBtCmRegisterHandlerReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->handlerType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->handle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_REGISTER_HANDLER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_REGISTER_HANDLER_REQ
void *CsrBtCmRegisterHandlerReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmRegisterHandlerReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmRegisterHandlerReq *) CsrPmemZalloc(sizeof(CsrBtCmRegisterHandlerReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->handlerType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->handle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_REGISTER_HANDLER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_OPEN_CFM
CsrSize CsrBtCmSdcOpenCfmSizeof(void *msg)
{
    CsrBtCmSdcOpenCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_OPEN_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_OPEN_CFM
CsrUint8 *CsrBtCmSdcOpenCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcOpenCfm *primitive;

    primitive = (CsrBtCmSdcOpenCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_OPEN_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_OPEN_CFM
void *CsrBtCmSdcOpenCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcOpenCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcOpenCfm *) CsrPmemZalloc(sizeof(CsrBtCmSdcOpenCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_OPEN_CFM */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_IND
CsrSize CsrBtCmBnepDisconnectIndSizeof(void *msg)
{
    CsrBtCmBnepDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_IND
CsrUint8 *CsrBtCmBnepDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBnepDisconnectInd *primitive;

    primitive = (CsrBtCmBnepDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_IND
void *CsrBtCmBnepDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtCmBnepDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_REQ
CsrSize CsrBtCmBnepModeChangeReqSizeof(void *msg)
{
    CsrBtCmBnepModeChangeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_REQ
CsrUint8 *CsrBtCmBnepModeChangeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBnepModeChangeReq *primitive;

    primitive = (CsrBtCmBnepModeChangeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->requestedMode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_REQ
void *CsrBtCmBnepModeChangeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepModeChangeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepModeChangeReq *) CsrPmemZalloc(sizeof(CsrBtCmBnepModeChangeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->requestedMode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_COD_REQ
CsrSize CsrBtCmWriteCodReqSizeof(void *msg)
{
    CsrBtCmWriteCodReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_COD_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_COD_REQ
CsrUint8 *CsrBtCmWriteCodReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteCodReq *primitive;

    primitive = (CsrBtCmWriteCodReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->updateFlags);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceClassOfDevice);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->majorClassOfDevice);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->minorClassOfDevice);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_COD_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_COD_REQ
void *CsrBtCmWriteCodReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteCodReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteCodReq *) CsrPmemZalloc(sizeof(CsrBtCmWriteCodReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->updateFlags, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceClassOfDevice, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->majorClassOfDevice, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->minorClassOfDevice, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_COD_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ACL_CONNECT_IND
CsrSize CsrBtCmAclConnectIndSizeof(void *msg)
{
    CsrBtCmAclConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_ACL_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_ACL_CONNECT_IND
CsrUint8 *CsrBtCmAclConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmAclConnectInd *primitive;

    primitive = (CsrBtCmAclConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->incoming);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->cod);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->aclConnHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_ACL_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_ACL_CONNECT_IND
void *CsrBtCmAclConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmAclConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmAclConnectInd *) CsrPmemZalloc(sizeof(CsrBtCmAclConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->incoming, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->cod, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->aclConnHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_ACL_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_CFM
CsrSize CsrBtCmScoRenegotiateCfmSizeof(void *msg)
{
    CsrBtCmScoRenegotiateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_CFM
CsrUint8 *CsrBtCmScoRenegotiateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmScoRenegotiateCfm *primitive;

    primitive = (CsrBtCmScoRenegotiateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->eScoHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_CFM
void *CsrBtCmScoRenegotiateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmScoRenegotiateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmScoRenegotiateCfm *) CsrPmemZalloc(sizeof(CsrBtCmScoRenegotiateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->eScoHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DATA_IND
CsrSize CsrBtCmDataIndSizeof(void *msg)
{
    CsrBtCmDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_DATA_IND
CsrUint8 *CsrBtCmDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDataInd *primitive;

    primitive = (CsrBtCmDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_DATA_IND
void *CsrBtCmDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDataInd *) CsrPmemZalloc(sizeof(CsrBtCmDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->payloadLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_CM_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_DATA_IND
void CsrBtCmDataIndSerFree(void *msg)
{
    CsrBtCmDataInd *primitive;

    primitive = (CsrBtCmDataInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_NEG_RES
CsrSize CsrBtCmSmUserConfirmationRequestNegResSizeof(void *msg)
{
    CsrBtCmSmUserConfirmationRequestNegRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_NEG_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_NEG_RES
CsrUint8 *CsrBtCmSmUserConfirmationRequestNegResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmUserConfirmationRequestNegRes *primitive;

    primitive = (CsrBtCmSmUserConfirmationRequestNegRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reason);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_NEG_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_NEG_RES
void *CsrBtCmSmUserConfirmationRequestNegResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmUserConfirmationRequestNegRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmUserConfirmationRequestNegRes *) CsrPmemZalloc(sizeof(CsrBtCmSmUserConfirmationRequestNegRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reason, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_NEG_RES */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_RES
CsrSize CsrBtCmBnepDisconnectResSizeof(void *msg)
{
    CsrBtCmBnepDisconnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_RES */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_RES
CsrUint8 *CsrBtCmBnepDisconnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBnepDisconnectRes *primitive;

    primitive = (CsrBtCmBnepDisconnectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->id);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_RES */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_RES
void *CsrBtCmBnepDisconnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepDisconnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepDisconnectRes *) CsrPmemZalloc(sizeof(CsrBtCmBnepDisconnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_DISCONNECT_RES */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_REQ
CsrSize CsrBtCmL2caDataAbortReqSizeof(void *msg)
{
    CsrBtCmL2caDataAbortReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_REQ
CsrUint8 *CsrBtCmL2caDataAbortReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caDataAbortReq *primitive;

    primitive = (CsrBtCmL2caDataAbortReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_REQ
void *CsrBtCmL2caDataAbortReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caDataAbortReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caDataAbortReq *) CsrPmemZalloc(sizeof(CsrBtCmL2caDataAbortReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDS_REGISTER_REQ
CsrSize CsrBtCmSdsRegisterReqSizeof(void *msg)
{
    CsrBtCmSdsRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->serviceRecordSize * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDS_REGISTER_REQ
CsrUint8 *CsrBtCmSdsRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdsRegisterReq *primitive;

    primitive = (CsrBtCmSdsRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrMemCpySer(buffer, length, &primitive->serviceRecord, sizeof(primitive->serviceRecord));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->serviceRecordSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    if(primitive->serviceRecord)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->serviceRecord), ((CsrSize) (primitive->serviceRecordSize * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDS_REGISTER_REQ
void *CsrBtCmSdsRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdsRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdsRegisterReq *) CsrPmemZalloc(sizeof(CsrBtCmSdsRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceRecord), buffer, &offset, ((CsrSize) sizeof(primitive->serviceRecord)));
    CsrUint16Des((CsrUint16 *) &primitive->serviceRecordSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    if (primitive->serviceRecordSize)
    {
        primitive->serviceRecord = CsrPmemZalloc(((CsrUint32) (primitive->serviceRecordSize * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->serviceRecord), buffer, &offset, ((CsrSize) (primitive->serviceRecordSize * sizeof(CsrUint8))));
    }
    else
    {
        primitive->serviceRecord = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDS_REGISTER_REQ
void CsrBtCmSdsRegisterReqSerFree(void *msg)
{
    CsrBtCmSdsRegisterReq *primitive;

    primitive = (CsrBtCmSdsRegisterReq *) msg;

    if(primitive->serviceRecord != NULL)
    {
        CsrPmemFree(primitive->serviceRecord);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_REQ
CsrSize CsrBtCmInquiryReqSizeof(void *msg)
{
    CsrBtCmInquiryReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_REQ
CsrUint8 *CsrBtCmInquiryReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmInquiryReq *primitive;

    primitive = (CsrBtCmInquiryReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->configMask);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->inquiryAccessCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->inquiryTxPowerLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_REQ
void *CsrBtCmInquiryReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmInquiryReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmInquiryReq *) CsrPmemZalloc(sizeof(CsrBtCmInquiryReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->configMask, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->inquiryAccessCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->inquiryTxPowerLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_ACL_OPEN_REQ
CsrSize CsrBtCmSmAclOpenReqSizeof(void *msg)
{
    CsrBtCmSmAclOpenReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_ACL_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_ACL_OPEN_REQ
CsrUint8 *CsrBtCmSmAclOpenReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmAclOpenReq *primitive;

    primitive = (CsrBtCmSmAclOpenReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_ACL_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_ACL_OPEN_REQ
void *CsrBtCmSmAclOpenReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmAclOpenReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmAclOpenReq *) CsrPmemZalloc(sizeof(CsrBtCmSmAclOpenReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_ACL_OPEN_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SET_ENCRYPTION_KEY_SIZE_REQ
CsrSize CsrBtCmSmSetEncryptionKeySizeReqSizeof(void *msg)
{
    CsrBtCmSmSetEncryptionKeySizeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_SET_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SET_ENCRYPTION_KEY_SIZE_REQ
CsrUint8 *CsrBtCmSmSetEncryptionKeySizeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmSetEncryptionKeySizeReq *primitive;

    primitive = (CsrBtCmSmSetEncryptionKeySizeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->minKeySize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->maxKeySize);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_SET_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SET_ENCRYPTION_KEY_SIZE_REQ
void *CsrBtCmSmSetEncryptionKeySizeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmSetEncryptionKeySizeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmSetEncryptionKeySizeReq *) CsrPmemZalloc(sizeof(CsrBtCmSmSetEncryptionKeySizeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->minKeySize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->maxKeySize, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_SET_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_REQ
CsrSize CsrBtCmRfcModeChangeReqSizeof(void *msg)
{
    CsrBtCmRfcModeChangeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_REQ
CsrUint8 *CsrBtCmRfcModeChangeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmRfcModeChangeReq *primitive;

    primitive = (CsrBtCmRfcModeChangeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->requestedMode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->forceSniff);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_REQ
void *CsrBtCmRfcModeChangeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmRfcModeChangeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmRfcModeChangeReq *) CsrPmemZalloc(sizeof(CsrBtCmRfcModeChangeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->requestedMode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->forceSniff, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_REQ
CsrSize CsrBtCmL2caDataReqSizeof(void *msg)
{
    CsrBtCmL2caDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrMblkGetLength(primitive->payload) + sizeof(CsrUint16));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_REQ
CsrUint8 *CsrBtCmL2caDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caDataReq *primitive;

    primitive = (CsrBtCmL2caDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    if (primitive->payload)
    {
        CsrUint16 mblkLength = CsrMblkGetLength(primitive->payload);
        CsrUint16Ser(buffer, length, mblkLength);
        CsrMblkCopyToMemory(primitive->payload, 0, mblkLength, &buffer[*length]);
        *length += mblkLength;
    }
    else
    {
        buffer[*length] = 0;
        buffer[*length + 1] = 0;
        *length += 2;
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_REQ
void *CsrBtCmL2caDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caDataReq *) CsrPmemZalloc(sizeof(CsrBtCmL2caDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    {
        CsrUint8 *mblkData;
        CsrUint16 mblkLength;
        CsrUint16Des((CsrUint16 *) &mblkLength, buffer, &offset);
        primitive->payload = CsrMblkMallocCreate((void **) &mblkData, mblkLength);
        CsrMemCpy(mblkData,&buffer[offset], mblkLength);
        offset += mblkLength;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_REQ
void CsrBtCmL2caDataReqSerFree(void *msg)
{
    CsrBtCmL2caDataReq *primitive;

    primitive = (CsrBtCmL2caDataReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrMblkDestroy(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SWITCH_ROLE_CFM
CsrSize CsrBtCmSwitchRoleCfmSizeof(void *msg)
{
    CsrBtCmSwitchRoleCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SWITCH_ROLE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SWITCH_ROLE_CFM
CsrUint8 *CsrBtCmSwitchRoleCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSwitchRoleCfm *primitive;

    primitive = (CsrBtCmSwitchRoleCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->roleType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SWITCH_ROLE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SWITCH_ROLE_CFM
void *CsrBtCmSwitchRoleCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSwitchRoleCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSwitchRoleCfm *) CsrPmemZalloc(sizeof(CsrBtCmSwitchRoleCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->roleType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SWITCH_ROLE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM
CsrSize CsrBtCmL2caCancelConnectAcceptCfmSizeof(void *msg)
{
    CsrBtCmL2caCancelConnectAcceptCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM
CsrUint8 *CsrBtCmL2caCancelConnectAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caCancelConnectAcceptCfm *primitive;

    primitive = (CsrBtCmL2caCancelConnectAcceptCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM
void *CsrBtCmL2caCancelConnectAcceptCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caCancelConnectAcceptCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caCancelConnectAcceptCfm *) CsrPmemZalloc(sizeof(CsrBtCmL2caCancelConnectAcceptCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_REQ
CsrSize CsrBtCmWritePagescanSettingsReqSizeof(void *msg)
{
    CsrBtCmWritePagescanSettingsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_REQ
CsrUint8 *CsrBtCmWritePagescanSettingsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWritePagescanSettingsReq *primitive;

    primitive = (CsrBtCmWritePagescanSettingsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->window);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_REQ
void *CsrBtCmWritePagescanSettingsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWritePagescanSettingsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWritePagescanSettingsReq *) CsrPmemZalloc(sizeof(CsrBtCmWritePagescanSettingsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->window, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_REQ
CsrSize CsrBtCmScoRenegotiateReqSizeof(void *msg)
{
    CsrBtCmScoRenegotiateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_REQ
CsrUint8 *CsrBtCmScoRenegotiateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmScoRenegotiateReq *primitive;

    primitive = (CsrBtCmScoRenegotiateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->audioQuality);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxLatency);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reTxEffort);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_REQ
void *CsrBtCmScoRenegotiateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmScoRenegotiateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmScoRenegotiateReq *) CsrPmemZalloc(sizeof(CsrBtCmScoRenegotiateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->audioQuality, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxLatency, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reTxEffort, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_READ_REMOTE_NAME_REQ
CsrSize CsrBtCmCancelReadRemoteNameReqSizeof(void *msg)
{
    CsrBtCmCancelReadRemoteNameReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_READ_REMOTE_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_READ_REMOTE_NAME_REQ
CsrUint8 *CsrBtCmCancelReadRemoteNameReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmCancelReadRemoteNameReq *primitive;

    primitive = (CsrBtCmCancelReadRemoteNameReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_READ_REMOTE_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_READ_REMOTE_NAME_REQ
void *CsrBtCmCancelReadRemoteNameReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmCancelReadRemoteNameReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmCancelReadRemoteNameReq *) CsrPmemZalloc(sizeof(CsrBtCmCancelReadRemoteNameReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_READ_REMOTE_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_REQ
CsrSize CsrBtCmReadTxPowerLevelReqSizeof(void *msg)
{
    CsrBtCmReadTxPowerLevelReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_REQ
CsrUint8 *CsrBtCmReadTxPowerLevelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadTxPowerLevelReq *primitive;

    primitive = (CsrBtCmReadTxPowerLevelReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->levelType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_REQ
void *CsrBtCmReadTxPowerLevelReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadTxPowerLevelReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadTxPowerLevelReq *) CsrPmemZalloc(sizeof(CsrBtCmReadTxPowerLevelReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->levelType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_CFM
CsrSize CsrBtCmWriteInquiryscanSettingsCfmSizeof(void *msg)
{
    CsrBtCmWriteInquiryscanSettingsCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_CFM
CsrUint8 *CsrBtCmWriteInquiryscanSettingsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteInquiryscanSettingsCfm *primitive;

    primitive = (CsrBtCmWriteInquiryscanSettingsCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_CFM
void *CsrBtCmWriteInquiryscanSettingsCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteInquiryscanSettingsCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteInquiryscanSettingsCfm *) CsrPmemZalloc(sizeof(CsrBtCmWriteInquiryscanSettingsCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_SETTINGS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ
CsrSize CsrBtCmSdcRfcExtendedSearchReqSizeof(void *msg)
{
    CsrBtCmSdcRfcExtendedSearchReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->serviceListSize * sizeof(CsrBtUuid32)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ
CsrUint8 *CsrBtCmSdcRfcExtendedSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcRfcExtendedSearchReq *primitive;

    primitive = (CsrBtCmSdcRfcExtendedSearchReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->serviceList, sizeof(primitive->serviceList));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serviceListSize);
    if(primitive->serviceList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->serviceList), ((CsrSize) (primitive->serviceListSize * sizeof(CsrBtUuid32))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ
void *CsrBtCmSdcRfcExtendedSearchReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcRfcExtendedSearchReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcRfcExtendedSearchReq *) CsrPmemZalloc(sizeof(CsrBtCmSdcRfcExtendedSearchReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceList), buffer, &offset, ((CsrSize) sizeof(primitive->serviceList)));
    CsrUint8Des((CsrUint8 *) &primitive->serviceListSize, buffer, &offset);
    if (primitive->serviceListSize)
    {
        primitive->serviceList = CsrPmemZalloc(((CsrUint32) (primitive->serviceListSize * sizeof(CsrBtUuid32))));
        CsrMemCpyDes(((void *) primitive->serviceList), buffer, &offset, ((CsrSize) (primitive->serviceListSize * sizeof(CsrBtUuid32))));
    }
    else
    {
        primitive->serviceList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ
void CsrBtCmSdcRfcExtendedSearchReqSerFree(void *msg)
{
    CsrBtCmSdcRfcExtendedSearchReq *primitive;

    primitive = (CsrBtCmSdcRfcExtendedSearchReq *) msg;

    if(primitive->serviceList != NULL)
    {
        CsrPmemFree(primitive->serviceList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_RFC_EXTENDED_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_REQ
CsrSize CsrBtCmSdsUnregisterReqSizeof(void *msg)
{
    CsrBtCmSdsUnregisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_REQ
CsrUint8 *CsrBtCmSdsUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdsUnregisterReq *primitive;

    primitive = (CsrBtCmSdsUnregisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceRecHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_REQ
void *CsrBtCmSdsUnregisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdsUnregisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdsUnregisterReq *) CsrPmemZalloc(sizeof(CsrBtCmSdsUnregisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceRecHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_PRIVATE_SWITCH_ROLE_REQ
CsrSize CsrBtCmPrivateSwitchRoleReqSizeof(void *msg)
{
    CsrBtCmPrivateSwitchRoleReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_PRIVATE_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_PRIVATE_SWITCH_ROLE_REQ
CsrUint8 *CsrBtCmPrivateSwitchRoleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmPrivateSwitchRoleReq *primitive;

    primitive = (CsrBtCmPrivateSwitchRoleReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_PRIVATE_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_PRIVATE_SWITCH_ROLE_REQ
void *CsrBtCmPrivateSwitchRoleReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmPrivateSwitchRoleReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmPrivateSwitchRoleReq *) CsrPmemZalloc(sizeof(CsrBtCmPrivateSwitchRoleReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_PRIVATE_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_CANCEL_SEARCH_REQ
CsrSize CsrBtCmSdcCancelSearchReqSizeof(void *msg)
{
    CsrBtCmSdcCancelSearchReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_CANCEL_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_CANCEL_SEARCH_REQ
CsrUint8 *CsrBtCmSdcCancelSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcCancelSearchReq *primitive;

    primitive = (CsrBtCmSdcCancelSearchReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->typeToCancel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_CANCEL_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_CANCEL_SEARCH_REQ
void *CsrBtCmSdcCancelSearchReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcCancelSearchReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcCancelSearchReq *) CsrPmemZalloc(sizeof(CsrBtCmSdcCancelSearchReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->typeToCancel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_CANCEL_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_EN_ENABLE_ENHANCEMENTS_REQ
CsrSize CsrBtCmEnEnableEnhancementsReqSizeof(void *msg)
{
    CsrBtCmEnEnableEnhancementsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_EN_ENABLE_ENHANCEMENTS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_EN_ENABLE_ENHANCEMENTS_REQ
CsrUint8 *CsrBtCmEnEnableEnhancementsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmEnEnableEnhancementsReq *primitive;

    primitive = (CsrBtCmEnEnableEnhancementsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->enhancements);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_EN_ENABLE_ENHANCEMENTS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_EN_ENABLE_ENHANCEMENTS_REQ
void *CsrBtCmEnEnableEnhancementsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmEnEnableEnhancementsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmEnEnableEnhancementsReq *) CsrPmemZalloc(sizeof(CsrBtCmEnEnableEnhancementsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->enhancements, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_EN_ENABLE_ENHANCEMENTS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ACCEPT_CONNECT_TIMEOUT
CsrSize CsrBtCmAcceptConnectTimeoutSizeof(void *msg)
{
    CsrBtCmAcceptConnectTimeout *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_ACCEPT_CONNECT_TIMEOUT */

#ifndef EXCLUDE_CSR_BT_CM_ACCEPT_CONNECT_TIMEOUT
CsrUint8 *CsrBtCmAcceptConnectTimeoutSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmAcceptConnectTimeout *primitive;

    primitive = (CsrBtCmAcceptConnectTimeout *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_ACCEPT_CONNECT_TIMEOUT */

#ifndef EXCLUDE_CSR_BT_CM_ACCEPT_CONNECT_TIMEOUT
void *CsrBtCmAcceptConnectTimeoutDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmAcceptConnectTimeout *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmAcceptConnectTimeout *) CsrPmemZalloc(sizeof(CsrBtCmAcceptConnectTimeout));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_ACCEPT_CONNECT_TIMEOUT */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_REQ
CsrSize CsrBtCmModeChangeConfigReqSizeof(void *msg)
{
    CsrBtCmModeChangeConfigReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_REQ
CsrUint8 *CsrBtCmModeChangeConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmModeChangeConfigReq *primitive;

    primitive = (CsrBtCmModeChangeConfigReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_REQ
void *CsrBtCmModeChangeConfigReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmModeChangeConfigReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmModeChangeConfigReq *) CsrPmemZalloc(sizeof(CsrBtCmModeChangeConfigReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_CFM
CsrSize CsrBtCmL2caConnectCfmSizeof(void *msg)
{
    CsrBtCmL2caConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_CFM
CsrUint8 *CsrBtCmL2caConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caConnectCfm *primitive;

    primitive = (CsrBtCmL2caConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localMtu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_CFM
void *CsrBtCmL2caConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caConnectCfm *) CsrPmemZalloc(sizeof(CsrBtCmL2caConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localMtu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_REQ
CsrSize CsrBtCmReadLocalBdAddrReqSizeof(void *msg)
{
    CsrBtCmReadLocalBdAddrReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_REQ
CsrUint8 *CsrBtCmReadLocalBdAddrReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadLocalBdAddrReq *primitive;

    primitive = (CsrBtCmReadLocalBdAddrReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_REQ
void *CsrBtCmReadLocalBdAddrReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadLocalBdAddrReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadLocalBdAddrReq *) CsrPmemZalloc(sizeof(CsrBtCmReadLocalBdAddrReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_BD_ADDR_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ
CsrSize CsrBtCmSdcUuid128RfcSearchReqSizeof(void *msg)
{
    CsrBtCmSdcUuid128RfcSearchReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->serviceListSize * sizeof(CsrBtUuid128)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ
CsrUint8 *CsrBtCmSdcUuid128RfcSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcUuid128RfcSearchReq *primitive;

    primitive = (CsrBtCmSdcUuid128RfcSearchReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->serviceList, sizeof(primitive->serviceList));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serviceListSize);
    if(primitive->serviceList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->serviceList), ((CsrSize) (primitive->serviceListSize * sizeof(CsrBtUuid128))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ
void *CsrBtCmSdcUuid128RfcSearchReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcUuid128RfcSearchReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcUuid128RfcSearchReq *) CsrPmemZalloc(sizeof(CsrBtCmSdcUuid128RfcSearchReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceList), buffer, &offset, ((CsrSize) sizeof(primitive->serviceList)));
    CsrUint8Des((CsrUint8 *) &primitive->serviceListSize, buffer, &offset);
    if (primitive->serviceListSize)
    {
        primitive->serviceList = CsrPmemZalloc(((CsrUint32) (primitive->serviceListSize * sizeof(CsrBtUuid128))));
        CsrMemCpyDes(((void *) primitive->serviceList), buffer, &offset, ((CsrSize) (primitive->serviceListSize * sizeof(CsrBtUuid128))));
    }
    else
    {
        primitive->serviceList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ
void CsrBtCmSdcUuid128RfcSearchReqSerFree(void *msg)
{
    CsrBtCmSdcUuid128RfcSearchReq *primitive;

    primitive = (CsrBtCmSdcUuid128RfcSearchReq *) msg;

    if(primitive->serviceList != NULL)
    {
        CsrPmemFree(primitive->serviceList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_RFC_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONTROL_REQ
CsrSize CsrBtCmControlReqSizeof(void *msg)
{
    CsrBtCmControlReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONTROL_REQ
CsrUint8 *CsrBtCmControlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmControlReq *primitive;

    primitive = (CsrBtCmControlReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modemstatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->break_signal);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONTROL_REQ
void *CsrBtCmControlReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmControlReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmControlReq *) CsrPmemZalloc(sizeof(CsrBtCmControlReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->modemstatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->break_signal, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ
CsrSize CsrBtCmL2caConnectAcceptReqSizeof(void *msg)
{
    CsrBtCmL2caConnectAcceptReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->conftabCount * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ
CsrUint8 *CsrBtCmL2caConnectAcceptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caConnectAcceptReq *primitive;

    primitive = (CsrBtCmL2caConnectAcceptReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->classOfDevice);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->primaryAcceptor);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->profileUuid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conftabCount);
    CsrMemCpySer(buffer, length, &primitive->conftab, sizeof(primitive->conftab));
    if(primitive->conftab)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->conftab), ((CsrSize) (primitive->conftabCount * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ
void *CsrBtCmL2caConnectAcceptReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caConnectAcceptReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caConnectAcceptReq *) CsrPmemZalloc(sizeof(CsrBtCmL2caConnectAcceptReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->classOfDevice, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->primaryAcceptor, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->profileUuid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conftabCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conftab), buffer, &offset, ((CsrSize) sizeof(primitive->conftab)));
    if (primitive->conftabCount)
    {
        primitive->conftab = CsrPmemZalloc(((CsrUint32) (primitive->conftabCount * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->conftab), buffer, &offset, ((CsrSize) (primitive->conftabCount * sizeof(CsrUint16))));
    }
    else
    {
        primitive->conftab = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ
void CsrBtCmL2caConnectAcceptReqSerFree(void *msg)
{
    CsrBtCmL2caConnectAcceptReq *primitive;

    primitive = (CsrBtCmL2caConnectAcceptReq *) msg;

    if(primitive->conftab != NULL)
    {
        CsrPmemFree(primitive->conftab);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ
CsrSize CsrBtCmL2caConnectionlessDataReqSizeof(void *msg)
{
    CsrBtCmL2caConnectionlessDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ
CsrUint8 *CsrBtCmL2caConnectionlessDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caConnectionlessDataReq *primitive;

    primitive = (CsrBtCmL2caConnectionlessDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connectionlessPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ
void *CsrBtCmL2caConnectionlessDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caConnectionlessDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caConnectionlessDataReq *) CsrPmemZalloc(sizeof(CsrBtCmL2caConnectionlessDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connectionlessPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    if (primitive->length)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ
void CsrBtCmL2caConnectionlessDataReqSerFree(void *msg)
{
    CsrBtCmL2caConnectionlessDataReq *primitive;

    primitive = (CsrBtCmL2caConnectionlessDataReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_PHYSICAL_LINK_STATUS_REQ
CsrSize CsrBtCmLePhysicalLinkStatusReqSizeof(void *msg)
{
    CsrBtCmLePhysicalLinkStatusReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_PHYSICAL_LINK_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_PHYSICAL_LINK_STATUS_REQ
CsrUint8 *CsrBtCmLePhysicalLinkStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLePhysicalLinkStatusReq *primitive;

    primitive = (CsrBtCmLePhysicalLinkStatusReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->radioType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_PHYSICAL_LINK_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_PHYSICAL_LINK_STATUS_REQ
void *CsrBtCmLePhysicalLinkStatusReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLePhysicalLinkStatusReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLePhysicalLinkStatusReq *) CsrPmemZalloc(sizeof(CsrBtCmLePhysicalLinkStatusReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->radioType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_PHYSICAL_LINK_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_AUTHENTICATE_REQ
CsrSize CsrBtCmSmAuthenticateReqSizeof(void *msg)
{
    CsrBtCmSmAuthenticateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_AUTHENTICATE_REQ
CsrUint8 *CsrBtCmSmAuthenticateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmAuthenticateReq *primitive;

    primitive = (CsrBtCmSmAuthenticateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_AUTHENTICATE_REQ
void *CsrBtCmSmAuthenticateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmAuthenticateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmAuthenticateReq *) CsrPmemZalloc(sizeof(CsrBtCmSmAuthenticateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_REQ
CsrSize CsrBtCmSetEventFilterBdaddrReqSizeof(void *msg)
{
    CsrBtCmSetEventFilterBdaddrReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_REQ
CsrUint8 *CsrBtCmSetEventFilterBdaddrReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSetEventFilterBdaddrReq *primitive;

    primitive = (CsrBtCmSetEventFilterBdaddrReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->selectInquiryFilter);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->autoAccept);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_REQ
void *CsrBtCmSetEventFilterBdaddrReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSetEventFilterBdaddrReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSetEventFilterBdaddrReq *) CsrPmemZalloc(sizeof(CsrBtCmSetEventFilterBdaddrReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->selectInquiryFilter, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->autoAccept, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_CONNECT_REQ
CsrSize CsrBtCmScoConnectReqSizeof(void *msg)
{
    CsrBtCmScoConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->parmsLen * sizeof(CsrBtCmScoCommonParms)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_CONNECT_REQ
CsrUint8 *CsrBtCmScoConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmScoConnectReq *primitive;

    primitive = (CsrBtCmScoConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmSlot);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmReassign);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->parmsLen);
    CsrMemCpySer(buffer, length, &primitive->parms, sizeof(primitive->parms));
    if(primitive->parms)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->parms), ((CsrSize) (primitive->parmsLen * sizeof(CsrBtCmScoCommonParms))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_CONNECT_REQ
void *CsrBtCmScoConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmScoConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmScoConnectReq *) CsrPmemZalloc(sizeof(CsrBtCmScoConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmSlot, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmReassign, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->parmsLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->parms), buffer, &offset, ((CsrSize) sizeof(primitive->parms)));
    if (primitive->parmsLen)
    {
        primitive->parms = CsrPmemZalloc(((CsrUint32) (primitive->parmsLen * sizeof(CsrBtCmScoCommonParms))));
        CsrMemCpyDes(((void *) primitive->parms), buffer, &offset, ((CsrSize) (primitive->parmsLen * sizeof(CsrBtCmScoCommonParms))));
    }
    else
    {
        primitive->parms = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_CONNECT_REQ
void CsrBtCmScoConnectReqSerFree(void *msg)
{
    CsrBtCmScoConnectReq *primitive;

    primitive = (CsrBtCmScoConnectReq *) msg;

    if(primitive->parms != NULL)
    {
        CsrPmemFree(primitive->parms);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_CFM
CsrSize CsrBtCmModeChangeConfigCfmSizeof(void *msg)
{
    CsrBtCmModeChangeConfigCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_CFM
CsrUint8 *CsrBtCmModeChangeConfigCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmModeChangeConfigCfm *primitive;

    primitive = (CsrBtCmModeChangeConfigCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_CFM
void *CsrBtCmModeChangeConfigCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmModeChangeConfigCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmModeChangeConfigCfm *) CsrPmemZalloc(sizeof(CsrBtCmModeChangeConfigCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_IND
CsrSize CsrBtCmScoRenegotiateIndSizeof(void *msg)
{
    CsrBtCmScoRenegotiateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_IND */

#ifndef EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_IND
CsrUint8 *CsrBtCmScoRenegotiateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmScoRenegotiateInd *primitive;

    primitive = (CsrBtCmScoRenegotiateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->eScoHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_IND */

#ifndef EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_IND
void *CsrBtCmScoRenegotiateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmScoRenegotiateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmScoRenegotiateInd *) CsrPmemZalloc(sizeof(CsrBtCmScoRenegotiateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->eScoHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_RENEGOTIATE_IND */

#ifndef EXCLUDE_CSR_BT_CM_EIR_FLAGS_CFM
CsrSize CsrBtCmEirFlagsCfmSizeof(void *msg)
{
    CsrBtCmEirFlagsCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_EIR_FLAGS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_EIR_FLAGS_CFM
CsrUint8 *CsrBtCmEirFlagsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmEirFlagsCfm *primitive;

    primitive = (CsrBtCmEirFlagsCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_EIR_FLAGS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_EIR_FLAGS_CFM
void *CsrBtCmEirFlagsCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmEirFlagsCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmEirFlagsCfm *) CsrPmemZalloc(sizeof(CsrBtCmEirFlagsCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_EIR_FLAGS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SCO_CANCEL_ACCEPT_CONNECT_REQ
CsrSize CsrBtCmScoCancelAcceptConnectReqSizeof(void *msg)
{
    CsrBtCmScoCancelAcceptConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_CANCEL_ACCEPT_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_CANCEL_ACCEPT_CONNECT_REQ
CsrUint8 *CsrBtCmScoCancelAcceptConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmScoCancelAcceptConnectReq *primitive;

    primitive = (CsrBtCmScoCancelAcceptConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_CANCEL_ACCEPT_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_CANCEL_ACCEPT_CONNECT_REQ
void *CsrBtCmScoCancelAcceptConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmScoCancelAcceptConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmScoCancelAcceptConnectReq *) CsrPmemZalloc(sizeof(CsrBtCmScoCancelAcceptConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_CANCEL_ACCEPT_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_CFM
CsrSize CsrBtCmWriteVoiceSettingsCfmSizeof(void *msg)
{
    CsrBtCmWriteVoiceSettingsCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_CFM
CsrUint8 *CsrBtCmWriteVoiceSettingsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteVoiceSettingsCfm *primitive;

    primitive = (CsrBtCmWriteVoiceSettingsCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_CFM
void *CsrBtCmWriteVoiceSettingsCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteVoiceSettingsCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteVoiceSettingsCfm *) CsrPmemZalloc(sizeof(CsrBtCmWriteVoiceSettingsCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_VOICE_SETTINGS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_ADVERTISE_CFM
CsrSize CsrBtCmLeAdvertiseCfmSizeof(void *msg)
{
    CsrBtCmLeAdvertiseCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_ADVERTISE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_ADVERTISE_CFM
CsrUint8 *CsrBtCmLeAdvertiseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeAdvertiseCfm *primitive;

    primitive = (CsrBtCmLeAdvertiseCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_ADVERTISE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_ADVERTISE_CFM
void *CsrBtCmLeAdvertiseCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeAdvertiseCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeAdvertiseCfm *) CsrPmemZalloc(sizeof(CsrBtCmLeAdvertiseCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_ADVERTISE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DM_MODE_CHANGE_REQ
CsrSize CsrBtCmDmModeChangeReqSizeof(void *msg)
{
    CsrBtCmDmModeChangeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DM_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_MODE_CHANGE_REQ
CsrUint8 *CsrBtCmDmModeChangeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDmModeChangeReq *primitive;

    primitive = (CsrBtCmDmModeChangeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->theMode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->thePlayer);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->theIndex);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_MODE_CHANGE_REQ
void *CsrBtCmDmModeChangeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDmModeChangeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDmModeChangeReq *) CsrPmemZalloc(sizeof(CsrBtCmDmModeChangeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->theMode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->thePlayer, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->theIndex, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_CFM
CsrSize CsrBtCmL2caDataAbortCfmSizeof(void *msg)
{
    CsrBtCmL2caDataAbortCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_CFM
CsrUint8 *CsrBtCmL2caDataAbortCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caDataAbortCfm *primitive;

    primitive = (CsrBtCmL2caDataAbortCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_CFM
void *CsrBtCmL2caDataAbortCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caDataAbortCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caDataAbortCfm *) CsrPmemZalloc(sizeof(CsrBtCmL2caDataAbortCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_ABORT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPES_IND
CsrSize CsrBtCmLogicalChannelTypesIndSizeof(void *msg)
{
    CsrBtCmLogicalChannelTypesInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPES_IND */

#ifndef EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPES_IND
CsrUint8 *CsrBtCmLogicalChannelTypesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLogicalChannelTypesInd *primitive;

    primitive = (CsrBtCmLogicalChannelTypesInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->logicalChannelTypeMask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->numberOfGuaranteedLogicalChannels);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPES_IND */

#ifndef EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPES_IND
void *CsrBtCmLogicalChannelTypesIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLogicalChannelTypesInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLogicalChannelTypesInd *) CsrPmemZalloc(sizeof(CsrBtCmLogicalChannelTypesInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->logicalChannelTypeMask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->numberOfGuaranteedLogicalChannels, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LOGICAL_CHANNEL_TYPES_IND */

#ifndef EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_CFM
CsrSize CsrBtCmSdcAttributeCfmSizeof(void *msg)
{
    CsrBtCmSdcAttributeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attributeListSize * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_CFM
CsrUint8 *CsrBtCmSdcAttributeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcAttributeCfm *primitive;

    primitive = (CsrBtCmSdcAttributeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->attributeList, sizeof(primitive->attributeList));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attributeListSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    if(primitive->attributeList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attributeList), ((CsrSize) (primitive->attributeListSize * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_CFM
void *CsrBtCmSdcAttributeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcAttributeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcAttributeCfm *) CsrPmemZalloc(sizeof(CsrBtCmSdcAttributeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attributeList), buffer, &offset, ((CsrSize) sizeof(primitive->attributeList)));
    CsrUint16Des((CsrUint16 *) &primitive->attributeListSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    if (primitive->attributeListSize)
    {
        primitive->attributeList = CsrPmemZalloc(((CsrUint32) (primitive->attributeListSize * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->attributeList), buffer, &offset, ((CsrSize) (primitive->attributeListSize * sizeof(CsrUint8))));
    }
    else
    {
        primitive->attributeList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_CFM
void CsrBtCmSdcAttributeCfmSerFree(void *msg)
{
    CsrBtCmSdcAttributeCfm *primitive;

    primitive = (CsrBtCmSdcAttributeCfm *) msg;

    if(primitive->attributeList != NULL)
    {
        CsrPmemFree(primitive->attributeList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CFM
CsrSize CsrBtCmMoveChannelCfmSizeof(void *msg)
{
    CsrBtCmMoveChannelCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CFM
CsrUint8 *CsrBtCmMoveChannelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmMoveChannelCfm *primitive;

    primitive = (CsrBtCmMoveChannelCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localControl);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CFM
void *CsrBtCmMoveChannelCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmMoveChannelCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmMoveChannelCfm *) CsrPmemZalloc(sizeof(CsrBtCmMoveChannelCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localControl, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_CFM */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_L2CA_CONNECT_REQ
CsrSize CsrBtCmCancelL2caConnectReqSizeof(void *msg)
{
    CsrBtCmCancelL2caConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_L2CA_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_L2CA_CONNECT_REQ
CsrUint8 *CsrBtCmCancelL2caConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmCancelL2caConnectReq *primitive;

    primitive = (CsrBtCmCancelL2caConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_L2CA_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_L2CA_CONNECT_REQ
void *CsrBtCmCancelL2caConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmCancelL2caConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmCancelL2caConnectReq *) CsrPmemZalloc(sizeof(CsrBtCmCancelL2caConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_L2CA_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_RES
CsrSize CsrBtCmSmIoCapabilityRequestResSizeof(void *msg)
{
    CsrBtCmSmIoCapabilityRequestRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_RES
CsrUint8 *CsrBtCmSmIoCapabilityRequestResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmIoCapabilityRequestRes *primitive;

    primitive = (CsrBtCmSmIoCapabilityRequestRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ioCapability);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authenticationRequirements);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->oobDataPresent);
    CsrMemCpySer(buffer, length, ((const void *) primitive->oobHashC), ((CsrSize) (16)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->oobRandR), ((CsrSize) (16)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->keyDistribution);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_RES
void *CsrBtCmSmIoCapabilityRequestResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmIoCapabilityRequestRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmIoCapabilityRequestRes *) CsrPmemZalloc(sizeof(CsrBtCmSmIoCapabilityRequestRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ioCapability, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authenticationRequirements, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->oobDataPresent, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->oobHashC), buffer, &offset, ((CsrSize) (16)));
    CsrMemCpyDes(((void *) primitive->oobRandR), buffer, &offset, ((CsrSize) (16)));
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->keyDistribution, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_ACL_DISCONNECT_IND
CsrSize CsrBtCmAclDisconnectIndSizeof(void *msg)
{
    CsrBtCmAclDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_ACL_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_ACL_DISCONNECT_IND
CsrUint8 *CsrBtCmAclDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmAclDisconnectInd *primitive;

    primitive = (CsrBtCmAclDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_ACL_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_ACL_DISCONNECT_IND
void *CsrBtCmAclDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmAclDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmAclDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtCmAclDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_ACL_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_REQ
CsrSize CsrBtCmLeWhitelistSetReqSizeof(void *msg)
{
    CsrBtCmLeWhitelistSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_REQ
CsrUint8 *CsrBtCmLeWhitelistSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeWhitelistSetReq *primitive;

    primitive = (CsrBtCmLeWhitelistSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->operation);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_REQ
void *CsrBtCmLeWhitelistSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeWhitelistSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeWhitelistSetReq *) CsrPmemZalloc(sizeof(CsrBtCmLeWhitelistSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->operation, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_REQ
CsrSize CsrBtCmL2caDisconnectReqSizeof(void *msg)
{
    CsrBtCmL2caDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_REQ
CsrUint8 *CsrBtCmL2caDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caDisconnectReq *primitive;

    primitive = (CsrBtCmL2caDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_REQ
void *CsrBtCmL2caDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtCmL2caDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_CFM
CsrSize CsrBtCmAlwaysMasterDevicesCfmSizeof(void *msg)
{
    CsrBtCmAlwaysMasterDevicesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_CFM */

#ifndef EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_CFM
CsrUint8 *CsrBtCmAlwaysMasterDevicesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmAlwaysMasterDevicesCfm *primitive;

    primitive = (CsrBtCmAlwaysMasterDevicesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_CFM */

#ifndef EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_CFM
void *CsrBtCmAlwaysMasterDevicesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmAlwaysMasterDevicesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmAlwaysMasterDevicesCfm *) CsrPmemZalloc(sizeof(CsrBtCmAlwaysMasterDevicesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_REQ
CsrSize CsrBtCmSdcUuid128SearchReqSizeof(void *msg)
{
    CsrBtCmSdcUuid128SearchReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->serviceListSize * sizeof(CsrBtUuid128)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_REQ
CsrUint8 *CsrBtCmSdcUuid128SearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcUuid128SearchReq *primitive;

    primitive = (CsrBtCmSdcUuid128SearchReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->serviceList, sizeof(primitive->serviceList));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serviceListSize);
    if(primitive->serviceList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->serviceList), ((CsrSize) (primitive->serviceListSize * sizeof(CsrBtUuid128))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_REQ
void *CsrBtCmSdcUuid128SearchReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcUuid128SearchReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcUuid128SearchReq *) CsrPmemZalloc(sizeof(CsrBtCmSdcUuid128SearchReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceList), buffer, &offset, ((CsrSize) sizeof(primitive->serviceList)));
    CsrUint8Des((CsrUint8 *) &primitive->serviceListSize, buffer, &offset);
    if (primitive->serviceListSize)
    {
        primitive->serviceList = CsrPmemZalloc(((CsrUint32) (primitive->serviceListSize * sizeof(CsrBtUuid128))));
        CsrMemCpyDes(((void *) primitive->serviceList), buffer, &offset, ((CsrSize) (primitive->serviceListSize * sizeof(CsrBtUuid128))));
    }
    else
    {
        primitive->serviceList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_REQ
void CsrBtCmSdcUuid128SearchReqSerFree(void *msg)
{
    CsrBtCmSdcUuid128SearchReq *primitive;

    primitive = (CsrBtCmSdcUuid128SearchReq *) msg;

    if(primitive->serviceList != NULL)
    {
        CsrPmemFree(primitive->serviceList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_UUID128_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_CLOCK_CFM
CsrSize CsrBtCmReadClockCfmSizeof(void *msg)
{
    CsrBtCmReadClockCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_CLOCK_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_CLOCK_CFM
CsrUint8 *CsrBtCmReadClockCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadClockCfm *primitive;

    primitive = (CsrBtCmReadClockCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->clock);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->accuracy);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_CLOCK_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_CLOCK_CFM
void *CsrBtCmReadClockCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadClockCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadClockCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadClockCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->clock, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->accuracy, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_CLOCK_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_CFM
CsrSize CsrBtCmL2caDataCfmSizeof(void *msg)
{
    CsrBtCmL2caDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_CFM
CsrUint8 *CsrBtCmL2caDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caDataCfm *primitive;

    primitive = (CsrBtCmL2caDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_CFM
void *CsrBtCmL2caDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caDataCfm *) CsrPmemZalloc(sizeof(CsrBtCmL2caDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_REQ
CsrSize CsrBtCmReadRemoteNameReqSizeof(void *msg)
{
    CsrBtCmReadRemoteNameReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_REQ
CsrUint8 *CsrBtCmReadRemoteNameReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadRemoteNameReq *primitive;

    primitive = (CsrBtCmReadRemoteNameReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_REQ
void *CsrBtCmReadRemoteNameReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadRemoteNameReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadRemoteNameReq *) CsrPmemZalloc(sizeof(CsrBtCmReadRemoteNameReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_FEATURES_REQ
CsrSize CsrBtCmReadRemoteFeaturesReqSizeof(void *msg)
{
    CsrBtCmReadRemoteFeaturesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_FEATURES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_FEATURES_REQ
CsrUint8 *CsrBtCmReadRemoteFeaturesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadRemoteFeaturesReq *primitive;

    primitive = (CsrBtCmReadRemoteFeaturesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_FEATURES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_FEATURES_REQ
void *CsrBtCmReadRemoteFeaturesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadRemoteFeaturesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadRemoteFeaturesReq *) CsrPmemZalloc(sizeof(CsrBtCmReadRemoteFeaturesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_FEATURES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_COD_CFM
CsrSize CsrBtCmWriteCodCfmSizeof(void *msg)
{
    CsrBtCmWriteCodCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_COD_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_COD_CFM
CsrUint8 *CsrBtCmWriteCodCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteCodCfm *primitive;

    primitive = (CsrBtCmWriteCodCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_COD_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_COD_CFM
void *CsrBtCmWriteCodCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteCodCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteCodCfm *) CsrPmemZalloc(sizeof(CsrBtCmWriteCodCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_COD_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LOCAL_NAME_CHANGE_IND
CsrSize CsrBtCmLocalNameChangeIndSizeof(void *msg)
{
    CsrBtCmLocalNameChangeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->localName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LOCAL_NAME_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_LOCAL_NAME_CHANGE_IND
CsrUint8 *CsrBtCmLocalNameChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLocalNameChangeInd *primitive;

    primitive = (CsrBtCmLocalNameChangeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, &primitive->localName, sizeof(primitive->localName));
    CsrUtf8StringSer(buffer, length, primitive->localName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LOCAL_NAME_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_LOCAL_NAME_CHANGE_IND
void *CsrBtCmLocalNameChangeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLocalNameChangeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLocalNameChangeInd *) CsrPmemZalloc(sizeof(CsrBtCmLocalNameChangeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->localName), buffer, &offset, ((CsrSize) sizeof(primitive->localName)));
    CsrUtf8StringDes(&primitive->localName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LOCAL_NAME_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_LOCAL_NAME_CHANGE_IND
void CsrBtCmLocalNameChangeIndSerFree(void *msg)
{
    CsrBtCmLocalNameChangeInd *primitive;

    primitive = (CsrBtCmLocalNameChangeInd *) msg;

    if(primitive->localName != NULL)
    {
        CsrPmemFree(primitive->localName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_LOCAL_NAME_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_REQ
CsrSize CsrBtCmReadLocalNameReqSizeof(void *msg)
{
    CsrBtCmReadLocalNameReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_REQ
CsrUint8 *CsrBtCmReadLocalNameReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadLocalNameReq *primitive;

    primitive = (CsrBtCmReadLocalNameReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_REQ
void *CsrBtCmReadLocalNameReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadLocalNameReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadLocalNameReq *) CsrPmemZalloc(sizeof(CsrBtCmReadLocalNameReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_NAME_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_IND
CsrSize CsrBtCmReadRemoteNameIndSizeof(void *msg)
{
    CsrBtCmReadRemoteNameInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->friendlyName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_IND
CsrUint8 *CsrBtCmReadRemoteNameIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadRemoteNameInd *primitive;

    primitive = (CsrBtCmReadRemoteNameInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->friendlyName, sizeof(primitive->friendlyName));
    CsrUtf8StringSer(buffer, length, primitive->friendlyName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_IND
void *CsrBtCmReadRemoteNameIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadRemoteNameInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadRemoteNameInd *) CsrPmemZalloc(sizeof(CsrBtCmReadRemoteNameInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->friendlyName), buffer, &offset, ((CsrSize) sizeof(primitive->friendlyName)));
    CsrUtf8StringDes(&primitive->friendlyName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_IND
void CsrBtCmReadRemoteNameIndSerFree(void *msg)
{
    CsrBtCmReadRemoteNameInd *primitive;

    primitive = (CsrBtCmReadRemoteNameInd *) msg;

    if(primitive->friendlyName != NULL)
    {
        CsrPmemFree(primitive->friendlyName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_IND */

#ifndef EXCLUDE_CSR_BT_CM_SM_LINK_KEY_REQUEST_RES
CsrSize CsrBtCmSmLinkKeyRequestResSizeof(void *msg)
{
    CsrBtCmSmLinkKeyRequestRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->keyLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_LINK_KEY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_LINK_KEY_REQUEST_RES
CsrUint8 *CsrBtCmSmLinkKeyRequestResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmLinkKeyRequestRes *primitive;

    primitive = (CsrBtCmSmLinkKeyRequestRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->keyType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->keyLength);
    CsrMemCpySer(buffer, length, &primitive->key, sizeof(primitive->key));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    if(primitive->key)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->key), ((CsrSize) (primitive->keyLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_LINK_KEY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_LINK_KEY_REQUEST_RES
void *CsrBtCmSmLinkKeyRequestResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmLinkKeyRequestRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmLinkKeyRequestRes *) CsrPmemZalloc(sizeof(CsrBtCmSmLinkKeyRequestRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->keyType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->keyLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->key), buffer, &offset, ((CsrSize) sizeof(primitive->key)));
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    if (primitive->keyLength)
    {
        primitive->key = CsrPmemZalloc(((CsrUint32) (primitive->keyLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->key), buffer, &offset, ((CsrSize) (primitive->keyLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->key = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_LINK_KEY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_LINK_KEY_REQUEST_RES
void CsrBtCmSmLinkKeyRequestResSerFree(void *msg)
{
    CsrBtCmSmLinkKeyRequestRes *primitive;

    primitive = (CsrBtCmSmLinkKeyRequestRes *) msg;

    if(primitive->key != NULL)
    {
        CsrPmemFree(primitive->key);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_SM_LINK_KEY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_ACL_DETACH_REQ
CsrSize CsrBtCmAclDetachReqSizeof(void *msg)
{
    CsrBtCmAclDetachReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_ACL_DETACH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ACL_DETACH_REQ
CsrUint8 *CsrBtCmAclDetachReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmAclDetachReq *primitive;

    primitive = (CsrBtCmAclDetachReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->flags);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_ACL_DETACH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ACL_DETACH_REQ
void *CsrBtCmAclDetachReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmAclDetachReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmAclDetachReq *) CsrPmemZalloc(sizeof(CsrBtCmAclDetachReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->flags, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_ACL_DETACH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_REQ
CsrSize CsrBtCmReadScanEnableReqSizeof(void *msg)
{
    CsrBtCmReadScanEnableReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_REQ
CsrUint8 *CsrBtCmReadScanEnableReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadScanEnableReq *primitive;

    primitive = (CsrBtCmReadScanEnableReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_REQ
void *CsrBtCmReadScanEnableReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadScanEnableReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadScanEnableReq *) CsrPmemZalloc(sizeof(CsrBtCmReadScanEnableReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_CFM
CsrSize CsrBtCmWriteScanEnableCfmSizeof(void *msg)
{
    CsrBtCmWriteScanEnableCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_CFM
CsrUint8 *CsrBtCmWriteScanEnableCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteScanEnableCfm *primitive;

    primitive = (CsrBtCmWriteScanEnableCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_CFM
void *CsrBtCmWriteScanEnableCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteScanEnableCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteScanEnableCfm *) CsrPmemZalloc(sizeof(CsrBtCmWriteScanEnableCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_CFM
CsrSize CsrBtCmWriteInquiryscanTypeCfmSizeof(void *msg)
{
    CsrBtCmWriteInquiryscanTypeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_CFM
CsrUint8 *CsrBtCmWriteInquiryscanTypeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteInquiryscanTypeCfm *primitive;

    primitive = (CsrBtCmWriteInquiryscanTypeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_CFM
void *CsrBtCmWriteInquiryscanTypeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteInquiryscanTypeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteInquiryscanTypeCfm *) CsrPmemZalloc(sizeof(CsrBtCmWriteInquiryscanTypeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_INQUIRYSCAN_TYPE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_PAGE_EVENT_IND
CsrSize CsrBtCmInquiryPageEventIndSizeof(void *msg)
{
    CsrBtCmInquiryPageEventInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_PAGE_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_PAGE_EVENT_IND
CsrUint8 *CsrBtCmInquiryPageEventIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmInquiryPageEventInd *primitive;

    primitive = (CsrBtCmInquiryPageEventInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->inquiry);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->paging);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_PAGE_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_PAGE_EVENT_IND
void *CsrBtCmInquiryPageEventIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmInquiryPageEventInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmInquiryPageEventInd *) CsrPmemZalloc(sizeof(CsrBtCmInquiryPageEventInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->inquiry, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->paging, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_PAGE_EVENT_IND */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_REQ
CsrSize CsrBtCmL2caModeChangeReqSizeof(void *msg)
{
    CsrBtCmL2caModeChangeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_REQ
CsrUint8 *CsrBtCmL2caModeChangeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caModeChangeReq *primitive;

    primitive = (CsrBtCmL2caModeChangeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->requestedMode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->forceSniff);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_REQ
void *CsrBtCmL2caModeChangeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caModeChangeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caModeChangeReq *) CsrPmemZalloc(sizeof(CsrBtCmL2caModeChangeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->requestedMode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->forceSniff, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SWITCH_ROLE_REQ
CsrSize CsrBtCmSwitchRoleReqSizeof(void *msg)
{
    CsrBtCmSwitchRoleReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SWITCH_ROLE_REQ
CsrUint8 *CsrBtCmSwitchRoleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSwitchRoleReq *primitive;

    primitive = (CsrBtCmSwitchRoleReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->roleType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SWITCH_ROLE_REQ
void *CsrBtCmSwitchRoleReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSwitchRoleReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSwitchRoleReq *) CsrPmemZalloc(sizeof(CsrBtCmSwitchRoleReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->roleType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_REQ
CsrSize CsrBtCmL2caConnectReqSizeof(void *msg)
{
    CsrBtCmL2caConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->conftabCount * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_REQ
CsrUint8 *CsrBtCmL2caConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caConnectReq *primitive;

    primitive = (CsrBtCmL2caConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remotePsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->conftabCount);
    CsrMemCpySer(buffer, length, &primitive->conftab, sizeof(primitive->conftab));
    if(primitive->conftab)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->conftab), ((CsrSize) (primitive->conftabCount * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_REQ
void *CsrBtCmL2caConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caConnectReq *) CsrPmemZalloc(sizeof(CsrBtCmL2caConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remotePsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->conftabCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->conftab), buffer, &offset, ((CsrSize) sizeof(primitive->conftab)));
    if (primitive->conftabCount)
    {
        primitive->conftab = CsrPmemZalloc(((CsrUint32) (primitive->conftabCount * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->conftab), buffer, &offset, ((CsrSize) (primitive->conftabCount * sizeof(CsrUint16))));
    }
    else
    {
        primitive->conftab = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECT_REQ
void CsrBtCmL2caConnectReqSerFree(void *msg)
{
    CsrBtCmL2caConnectReq *primitive;

    primitive = (CsrBtCmL2caConnectReq *) msg;

    if(primitive->conftab != NULL)
    {
        CsrPmemFree(primitive->conftab);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_CFM
CsrSize CsrBtCmSetAfhChannelClassCfmSizeof(void *msg)
{
    CsrBtCmSetAfhChannelClassCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_CFM
CsrUint8 *CsrBtCmSetAfhChannelClassCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSetAfhChannelClassCfm *primitive;

    primitive = (CsrBtCmSetAfhChannelClassCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_CFM
void *CsrBtCmSetAfhChannelClassCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSetAfhChannelClassCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSetAfhChannelClassCfm *) CsrPmemZalloc(sizeof(CsrBtCmSetAfhChannelClassCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_AFH_CHANNEL_CLASS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_CFM
CsrSize CsrBtCmWritePagescanSettingsCfmSizeof(void *msg)
{
    CsrBtCmWritePagescanSettingsCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_CFM
CsrUint8 *CsrBtCmWritePagescanSettingsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWritePagescanSettingsCfm *primitive;

    primitive = (CsrBtCmWritePagescanSettingsCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_CFM
void *CsrBtCmWritePagescanSettingsCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWritePagescanSettingsCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWritePagescanSettingsCfm *) CsrPmemZalloc(sizeof(CsrBtCmWritePagescanSettingsCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_SETTINGS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_IND
CsrSize CsrBtCmSdcSearchIndSizeof(void *msg)
{
    CsrBtCmSdcSearchInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->serviceHandleListCount * sizeof(CsrBtUuid32)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_IND
CsrUint8 *CsrBtCmSdcSearchIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcSearchInd *primitive;

    primitive = (CsrBtCmSdcSearchInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->service);
    CsrMemCpySer(buffer, length, &primitive->serviceHandleList, sizeof(primitive->serviceHandleList));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->serviceHandleListCount);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    if(primitive->serviceHandleList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->serviceHandleList), ((CsrSize) (primitive->serviceHandleListCount * sizeof(CsrBtUuid32))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_IND
void *CsrBtCmSdcSearchIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcSearchInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcSearchInd *) CsrPmemZalloc(sizeof(CsrBtCmSdcSearchInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->service, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceHandleList), buffer, &offset, ((CsrSize) sizeof(primitive->serviceHandleList)));
    CsrUint16Des((CsrUint16 *) &primitive->serviceHandleListCount, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    if (primitive->serviceHandleListCount)
    {
        primitive->serviceHandleList = CsrPmemZalloc(((CsrUint32) (primitive->serviceHandleListCount * sizeof(CsrBtUuid32))));
        CsrMemCpyDes(((void *) primitive->serviceHandleList), buffer, &offset, ((CsrSize) (primitive->serviceHandleListCount * sizeof(CsrBtUuid32))));
    }
    else
    {
        primitive->serviceHandleList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SEARCH_IND
void CsrBtCmSdcSearchIndSerFree(void *msg)
{
    CsrBtCmSdcSearchInd *primitive;

    primitive = (CsrBtCmSdcSearchInd *) msg;

    if(primitive->serviceHandleList != NULL)
    {
        CsrPmemFree(primitive->serviceHandleList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_CM_UNREGISTER_REQ
CsrSize CsrBtCmUnregisterReqSizeof(void *msg)
{
    CsrBtCmUnregisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_UNREGISTER_REQ
CsrUint8 *CsrBtCmUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmUnregisterReq *primitive;

    primitive = (CsrBtCmUnregisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_UNREGISTER_REQ
void *CsrBtCmUnregisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmUnregisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmUnregisterReq *) CsrPmemZalloc(sizeof(CsrBtCmUnregisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_BONDING_REQ
CsrSize CsrBtCmSmBondingReqSizeof(void *msg)
{
    CsrBtCmSmBondingReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_BONDING_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_BONDING_REQ
CsrUint8 *CsrBtCmSmBondingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmBondingReq *primitive;

    primitive = (CsrBtCmSmBondingReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_BONDING_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_BONDING_REQ
void *CsrBtCmSmBondingReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmBondingReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmBondingReq *) CsrPmemZalloc(sizeof(CsrBtCmSmBondingReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_BONDING_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_LE_SECURITY_REQ
CsrSize CsrBtCmSmLeSecurityReqSizeof(void *msg)
{
    CsrBtCmSmLeSecurityReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_LE_SECURITY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_LE_SECURITY_REQ
CsrUint8 *CsrBtCmSmLeSecurityReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmLeSecurityReq *primitive;

    primitive = (CsrBtCmSmLeSecurityReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addr.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addr.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addr.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addr.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->securityRequirements);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->l2caConFlags);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_LE_SECURITY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_LE_SECURITY_REQ
void *CsrBtCmSmLeSecurityReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmLeSecurityReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmLeSecurityReq *) CsrPmemZalloc(sizeof(CsrBtCmSmLeSecurityReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addr.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addr.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addr.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addr.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->securityRequirements, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->l2caConFlags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_LE_SECURITY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_EVENT_ADVERTISING_IND
CsrSize CsrBtCmLeEventAdvertisingIndSizeof(void *msg)
{
    CsrBtCmLeEventAdvertisingInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_EVENT_ADVERTISING_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_EVENT_ADVERTISING_IND
CsrUint8 *CsrBtCmLeEventAdvertisingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeEventAdvertisingInd *primitive;

    primitive = (CsrBtCmLeEventAdvertisingInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->event);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->advType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->intervalMin);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->intervalMax);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->channelMap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_EVENT_ADVERTISING_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_EVENT_ADVERTISING_IND
void *CsrBtCmLeEventAdvertisingIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeEventAdvertisingInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeEventAdvertisingInd *) CsrPmemZalloc(sizeof(CsrBtCmLeEventAdvertisingInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->event, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->advType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->intervalMin, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->intervalMax, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->channelMap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_EVENT_ADVERTISING_IND */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_CFM
CsrSize CsrBtCmWritePagescanTypeCfmSizeof(void *msg)
{
    CsrBtCmWritePagescanTypeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_CFM
CsrUint8 *CsrBtCmWritePagescanTypeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWritePagescanTypeCfm *primitive;

    primitive = (CsrBtCmWritePagescanTypeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_CFM
void *CsrBtCmWritePagescanTypeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWritePagescanTypeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWritePagescanTypeCfm *) CsrPmemZalloc(sizeof(CsrBtCmWritePagescanTypeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_BLUECORE_INITIALIZED_IND
CsrSize CsrBtCmBluecoreInitializedIndSizeof(void *msg)
{
    CsrBtCmBluecoreInitializedInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BLUECORE_INITIALIZED_IND */

#ifndef EXCLUDE_CSR_BT_CM_BLUECORE_INITIALIZED_IND
CsrUint8 *CsrBtCmBluecoreInitializedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBluecoreInitializedInd *primitive;

    primitive = (CsrBtCmBluecoreInitializedInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BLUECORE_INITIALIZED_IND */

#ifndef EXCLUDE_CSR_BT_CM_BLUECORE_INITIALIZED_IND
void *CsrBtCmBluecoreInitializedIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBluecoreInitializedInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBluecoreInitializedInd *) CsrPmemZalloc(sizeof(CsrBtCmBluecoreInitializedInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BLUECORE_INITIALIZED_IND */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM
CsrSize CsrBtCmWriteLinkSupervTimeoutCfmSizeof(void *msg)
{
    CsrBtCmWriteLinkSupervTimeoutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM
CsrUint8 *CsrBtCmWriteLinkSupervTimeoutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteLinkSupervTimeoutCfm *primitive;

    primitive = (CsrBtCmWriteLinkSupervTimeoutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM
void *CsrBtCmWriteLinkSupervTimeoutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteLinkSupervTimeoutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteLinkSupervTimeoutCfm *) CsrPmemZalloc(sizeof(CsrBtCmWriteLinkSupervTimeoutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_REQ
CsrSize CsrBtCmL2caCancelConnectAcceptReqSizeof(void *msg)
{
    CsrBtCmL2caCancelConnectAcceptReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_REQ
CsrUint8 *CsrBtCmL2caCancelConnectAcceptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caCancelConnectAcceptReq *primitive;

    primitive = (CsrBtCmL2caCancelConnectAcceptReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_REQ
void *CsrBtCmL2caCancelConnectAcceptReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caCancelConnectAcceptReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caCancelConnectAcceptReq *) CsrPmemZalloc(sizeof(CsrBtCmL2caCancelConnectAcceptReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CANCEL_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_REQ
CsrSize CsrBtCmLeConnparamReqSizeof(void *msg)
{
    CsrBtCmLeConnparamReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_REQ
CsrUint8 *CsrBtCmLeConnparamReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeConnparamReq *primitive;

    primitive = (CsrBtCmLeConnparamReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
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
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_REQ
void *CsrBtCmLeConnparamReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeConnparamReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeConnparamReq *) CsrPmemZalloc(sizeof(CsrBtCmLeConnparamReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_SWITCH_ROLE_REQ
CsrSize CsrBtCmDmSwitchRoleReqSizeof(void *msg)
{
    CsrBtCmDmSwitchRoleReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DM_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_SWITCH_ROLE_REQ
CsrUint8 *CsrBtCmDmSwitchRoleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDmSwitchRoleReq *primitive;

    primitive = (CsrBtCmDmSwitchRoleReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->activePlayer);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_SWITCH_ROLE_REQ
void *CsrBtCmDmSwitchRoleReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDmSwitchRoleReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDmSwitchRoleReq *) CsrPmemZalloc(sizeof(CsrBtCmDmSwitchRoleReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->activePlayer, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_WRITE_AUTO_FLUSH_TIMEOUT_REQ
CsrSize CsrBtCmDmWriteAutoFlushTimeoutReqSizeof(void *msg)
{
    CsrBtCmDmWriteAutoFlushTimeoutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DM_WRITE_AUTO_FLUSH_TIMEOUT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_WRITE_AUTO_FLUSH_TIMEOUT_REQ
CsrUint8 *CsrBtCmDmWriteAutoFlushTimeoutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDmWriteAutoFlushTimeoutReq *primitive;

    primitive = (CsrBtCmDmWriteAutoFlushTimeoutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flushTo);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_WRITE_AUTO_FLUSH_TIMEOUT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_WRITE_AUTO_FLUSH_TIMEOUT_REQ
void *CsrBtCmDmWriteAutoFlushTimeoutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDmWriteAutoFlushTimeoutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDmWriteAutoFlushTimeoutReq *) CsrPmemZalloc(sizeof(CsrBtCmDmWriteAutoFlushTimeoutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flushTo, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_WRITE_AUTO_FLUSH_TIMEOUT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_REQ
CsrSize CsrBtCmBnepExtendedDataReqSizeof(void *msg)
{
    CsrBtCmBnepExtendedDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_REQ
CsrUint8 *CsrBtCmBnepExtendedDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBnepExtendedDataReq *primitive;

    primitive = (CsrBtCmBnepExtendedDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->etherType);
    CsrMemCpySer(buffer, length, ((const void *) primitive->dstAddr.w), ((CsrSize) 2 * (3)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->srcAddr.w), ((CsrSize) 2 * (3)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_REQ
void *CsrBtCmBnepExtendedDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepExtendedDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepExtendedDataReq *) CsrPmemZalloc(sizeof(CsrBtCmBnepExtendedDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->etherType, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->dstAddr.w), buffer, &offset, ((CsrSize) 2 * (3)));
    CsrMemCpyDes(((void *) primitive->srcAddr.w), buffer, &offset, ((CsrSize) 2 * (3)));
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->length)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_REQ
void CsrBtCmBnepExtendedDataReqSerFree(void *msg)
{
    CsrBtCmBnepExtendedDataReq *primitive;

    primitive = (CsrBtCmBnepExtendedDataReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DATA_RES
CsrSize CsrBtCmDataResSizeof(void *msg)
{
    CsrBtCmDataRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_RES */

#ifndef EXCLUDE_CSR_BT_CM_DATA_RES
CsrUint8 *CsrBtCmDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDataRes *primitive;

    primitive = (CsrBtCmDataRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_RES */

#ifndef EXCLUDE_CSR_BT_CM_DATA_RES
void *CsrBtCmDataResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDataRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDataRes *) CsrPmemZalloc(sizeof(CsrBtCmDataRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_RES */

#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_CFM
CsrSize CsrBtCmReadEncryptionKeySizeCfmSizeof(void *msg)
{
    CsrBtCmReadEncryptionKeySizeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_CFM
CsrUint8 *CsrBtCmReadEncryptionKeySizeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadEncryptionKeySizeCfm *primitive;

    primitive = (CsrBtCmReadEncryptionKeySizeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->keySize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_CFM
void *CsrBtCmReadEncryptionKeySizeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadEncryptionKeySizeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadEncryptionKeySizeCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadEncryptionKeySizeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->keySize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_IAC_REQ
CsrSize CsrBtCmReadIacReqSizeof(void *msg)
{
    CsrBtCmReadIacReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_IAC_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_IAC_REQ
CsrUint8 *CsrBtCmReadIacReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadIacReq *primitive;

    primitive = (CsrBtCmReadIacReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_IAC_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_IAC_REQ
void *CsrBtCmReadIacReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadIacReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadIacReq *) CsrPmemZalloc(sizeof(CsrBtCmReadIacReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_IAC_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_REQ
CsrSize CsrBtCmSdcServiceSearchReqSizeof(void *msg)
{
    CsrBtCmSdcServiceSearchReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->uuidSetLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_REQ
CsrUint8 *CsrBtCmSdcServiceSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcServiceSearchReq *primitive;

    primitive = (CsrBtCmSdcServiceSearchReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uuidSetLength);
    CsrMemCpySer(buffer, length, &primitive->uuidSet, sizeof(primitive->uuidSet));
    if(primitive->uuidSet)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->uuidSet), ((CsrSize) (primitive->uuidSetLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_REQ
void *CsrBtCmSdcServiceSearchReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcServiceSearchReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcServiceSearchReq *) CsrPmemZalloc(sizeof(CsrBtCmSdcServiceSearchReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uuidSetLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->uuidSet), buffer, &offset, ((CsrSize) sizeof(primitive->uuidSet)));
    if (primitive->uuidSetLength)
    {
        primitive->uuidSet = CsrPmemZalloc(((CsrUint32) (primitive->uuidSetLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->uuidSet), buffer, &offset, ((CsrSize) (primitive->uuidSetLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->uuidSet = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_REQ
void CsrBtCmSdcServiceSearchReqSerFree(void *msg)
{
    CsrBtCmSdcServiceSearchReq *primitive;

    primitive = (CsrBtCmSdcServiceSearchReq *) msg;

    if(primitive->uuidSet != NULL)
    {
        CsrPmemFree(primitive->uuidSet);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_REQ
CsrSize CsrBtCmLeTransmitterTestReqSizeof(void *msg)
{
    CsrBtCmLeTransmitterTestReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_REQ
CsrUint8 *CsrBtCmLeTransmitterTestReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeTransmitterTestReq *primitive;

    primitive = (CsrBtCmLeTransmitterTestReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->txFrequency);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->lengthOfTestData);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->packetPayload);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_REQ
void *CsrBtCmLeTransmitterTestReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeTransmitterTestReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeTransmitterTestReq *) CsrPmemZalloc(sizeof(CsrBtCmLeTransmitterTestReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->txFrequency, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->lengthOfTestData, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->packetPayload, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_IND
CsrSize CsrBtCmL2caDisconnectIndSizeof(void *msg)
{
    CsrBtCmL2caDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_IND
CsrUint8 *CsrBtCmL2caDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caDisconnectInd *primitive;

    primitive = (CsrBtCmL2caDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localTerminated);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_IND
void *CsrBtCmL2caDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtCmL2caDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localTerminated, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_RES
CsrSize CsrBtCmPortnegResSizeof(void *msg)
{
    CsrBtCmPortnegRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_RES
CsrUint8 *CsrBtCmPortnegResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmPortnegRes *primitive;

    primitive = (CsrBtCmPortnegRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->portPar.parameter_mask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_RES
void *CsrBtCmPortnegResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmPortnegRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmPortnegRes *) CsrPmemZalloc(sizeof(CsrBtCmPortnegRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->portPar.parameter_mask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_RES */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_CFM
CsrSize CsrBtCmReadLocalVersionCfmSizeof(void *msg)
{
    CsrBtCmReadLocalVersionCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_CFM
CsrUint8 *CsrBtCmReadLocalVersionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadLocalVersionCfm *primitive;

    primitive = (CsrBtCmReadLocalVersionCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->lmpVersion);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_CFM
void *CsrBtCmReadLocalVersionCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadLocalVersionCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadLocalVersionCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadLocalVersionCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->lmpVersion, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_VERSION_CFM */

#ifndef EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_REQ
CsrSize CsrBtCmEnableDutModeReqSizeof(void *msg)
{
    CsrBtCmEnableDutModeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_REQ
CsrUint8 *CsrBtCmEnableDutModeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmEnableDutModeReq *primitive;

    primitive = (CsrBtCmEnableDutModeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_REQ
void *CsrBtCmEnableDutModeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmEnableDutModeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmEnableDutModeReq *) CsrPmemZalloc(sizeof(CsrBtCmEnableDutModeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_CFM
CsrSize CsrBtCmLeReceiverTestCfmSizeof(void *msg)
{
    CsrBtCmLeReceiverTestCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_CFM
CsrUint8 *CsrBtCmLeReceiverTestCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeReceiverTestCfm *primitive;

    primitive = (CsrBtCmLeReceiverTestCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_CFM
void *CsrBtCmLeReceiverTestCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeReceiverTestCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeReceiverTestCfm *) CsrPmemZalloc(sizeof(CsrBtCmLeReceiverTestCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_CFM */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_INQUIRY_REQ
CsrSize CsrBtCmCancelInquiryReqSizeof(void *msg)
{
    CsrBtCmCancelInquiryReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_INQUIRY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_INQUIRY_REQ
CsrUint8 *CsrBtCmCancelInquiryReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmCancelInquiryReq *primitive;

    primitive = (CsrBtCmCancelInquiryReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_INQUIRY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_INQUIRY_REQ
void *CsrBtCmCancelInquiryReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmCancelInquiryReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmCancelInquiryReq *) CsrPmemZalloc(sizeof(CsrBtCmCancelInquiryReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_INQUIRY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_REQ
CsrSize CsrBtCmSdcAttributeReqSizeof(void *msg)
{
    CsrBtCmSdcAttributeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_REQ
CsrUint8 *CsrBtCmSdcAttributeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcAttributeReq *primitive;

    primitive = (CsrBtCmSdcAttributeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attributeIdentifier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->upperRangeAttributeIdentifier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxBytesToReturn);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_REQ
void *CsrBtCmSdcAttributeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcAttributeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcAttributeReq *) CsrPmemZalloc(sizeof(CsrBtCmSdcAttributeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attributeIdentifier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->upperRangeAttributeIdentifier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxBytesToReturn, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_ATTRIBUTE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_CLOSE_REQ
CsrSize CsrBtCmSdcCloseReqSizeof(void *msg)
{
    CsrBtCmSdcCloseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_CLOSE_REQ
CsrUint8 *CsrBtCmSdcCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcCloseReq *primitive;

    primitive = (CsrBtCmSdcCloseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_CLOSE_REQ
void *CsrBtCmSdcCloseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcCloseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcCloseReq *) CsrPmemZalloc(sizeof(CsrBtCmSdcCloseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ENCRYPT_CHANGE_IND
CsrSize CsrBtCmEncryptChangeIndSizeof(void *msg)
{
    CsrBtCmEncryptChangeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_ENCRYPT_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_ENCRYPT_CHANGE_IND
CsrUint8 *CsrBtCmEncryptChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmEncryptChangeInd *primitive;

    primitive = (CsrBtCmEncryptChangeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->encrypted);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_ENCRYPT_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_ENCRYPT_CHANGE_IND
void *CsrBtCmEncryptChangeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmEncryptChangeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmEncryptChangeInd *) CsrPmemZalloc(sizeof(CsrBtCmEncryptChangeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->encrypted, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_ENCRYPT_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_INCOMING_SCO_REQ
CsrSize CsrBtCmIncomingScoReqSizeof(void *msg)
{
    CsrBtCmIncomingScoReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_INCOMING_SCO_REQ */

#ifndef EXCLUDE_CSR_BT_CM_INCOMING_SCO_REQ
CsrUint8 *CsrBtCmIncomingScoReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmIncomingScoReq *primitive;

    primitive = (CsrBtCmIncomingScoReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->linkType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->protocolId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_INCOMING_SCO_REQ */

#ifndef EXCLUDE_CSR_BT_CM_INCOMING_SCO_REQ
void *CsrBtCmIncomingScoReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmIncomingScoReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmIncomingScoReq *) CsrPmemZalloc(sizeof(CsrBtCmIncomingScoReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->linkType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->protocolId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_INCOMING_SCO_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONTROL_IND
CsrSize CsrBtCmControlIndSizeof(void *msg)
{
    CsrBtCmControlInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_CM_CONTROL_IND
CsrUint8 *CsrBtCmControlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmControlInd *primitive;

    primitive = (CsrBtCmControlInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->modemstatus);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->break_signal);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_CM_CONTROL_IND
void *CsrBtCmControlIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmControlInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmControlInd *) CsrPmemZalloc(sizeof(CsrBtCmControlInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->modemstatus, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->break_signal, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CONTROL_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_REQ
CsrSize CsrBtCmReadEncryptionStatusReqSizeof(void *msg)
{
    CsrBtCmReadEncryptionStatusReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_REQ
CsrUint8 *CsrBtCmReadEncryptionStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadEncryptionStatusReq *primitive;

    primitive = (CsrBtCmReadEncryptionStatusReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_REQ
void *CsrBtCmReadEncryptionStatusReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadEncryptionStatusReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadEncryptionStatusReq *) CsrPmemZalloc(sizeof(CsrBtCmReadEncryptionStatusReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_REQ
CsrSize CsrBtCmReadEncryptionKeySizeReqSizeof(void *msg)
{
    CsrBtCmReadEncryptionKeySizeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_REQ
CsrUint8 *CsrBtCmReadEncryptionKeySizeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadEncryptionKeySizeReq *primitive;

    primitive = (CsrBtCmReadEncryptionKeySizeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_REQ
void *CsrBtCmReadEncryptionKeySizeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadEncryptionKeySizeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadEncryptionKeySizeReq *) CsrPmemZalloc(sizeof(CsrBtCmReadEncryptionKeySizeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ROLE_CHANGE_IND
CsrSize CsrBtCmRoleChangeIndSizeof(void *msg)
{
    CsrBtCmRoleChangeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_ROLE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_ROLE_CHANGE_IND
CsrUint8 *CsrBtCmRoleChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmRoleChangeInd *primitive;

    primitive = (CsrBtCmRoleChangeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_ROLE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_ROLE_CHANGE_IND
void *CsrBtCmRoleChangeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmRoleChangeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmRoleChangeInd *) CsrPmemZalloc(sizeof(CsrBtCmRoleChangeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_ROLE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_IND
CsrSize CsrBtCmL2caDataIndSizeof(void *msg)
{
    CsrBtCmL2caDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_IND
CsrUint8 *CsrBtCmL2caDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caDataInd *primitive;

    primitive = (CsrBtCmL2caDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_IND
void *CsrBtCmL2caDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caDataInd *) CsrPmemZalloc(sizeof(CsrBtCmL2caDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);
    if (primitive->length)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_IND
void CsrBtCmL2caDataIndSerFree(void *msg)
{
    CsrBtCmL2caDataInd *primitive;

    primitive = (CsrBtCmL2caDataInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_REQ
CsrSize CsrBtCmCancelAcceptConnectReqSizeof(void *msg)
{
    CsrBtCmCancelAcceptConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_REQ
CsrUint8 *CsrBtCmCancelAcceptConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmCancelAcceptConnectReq *primitive;

    primitive = (CsrBtCmCancelAcceptConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_REQ
void *CsrBtCmCancelAcceptConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmCancelAcceptConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmCancelAcceptConnectReq *) CsrPmemZalloc(sizeof(CsrBtCmCancelAcceptConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_BNEP_CONNECT_REQ
CsrSize CsrBtCmCancelBnepConnectReqSizeof(void *msg)
{
    CsrBtCmCancelBnepConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_BNEP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_BNEP_CONNECT_REQ
CsrUint8 *CsrBtCmCancelBnepConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmCancelBnepConnectReq *primitive;

    primitive = (CsrBtCmCancelBnepConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrMemCpySer(buffer, length, ((const void *) primitive->rem_addr.w), ((CsrSize) 2 * (3)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_BNEP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_BNEP_CONNECT_REQ
void *CsrBtCmCancelBnepConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmCancelBnepConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmCancelBnepConnectReq *) CsrPmemZalloc(sizeof(CsrBtCmCancelBnepConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->rem_addr.w), buffer, &offset, ((CsrSize) 2 * (3)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_BNEP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_CFM
CsrSize CsrBtCmL2caGetChannelInfoCfmSizeof(void *msg)
{
    CsrBtCmL2caGetChannelInfoCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_CFM
CsrUint8 *CsrBtCmL2caGetChannelInfoCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caGetChannelInfoCfm *primitive;

    primitive = (CsrBtCmL2caGetChannelInfoCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->aclHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remoteCid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_CFM
void *CsrBtCmL2caGetChannelInfoCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caGetChannelInfoCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caGetChannelInfoCfm *) CsrPmemZalloc(sizeof(CsrBtCmL2caGetChannelInfoCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->aclHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remoteCid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_GET_CHANNEL_INFO_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDS_EXT_UNREGISTER_CFM
CsrSize CsrBtCmSdsExtUnregisterCfmSizeof(void *msg)
{
    CsrBtCmSdsExtUnregisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_EXT_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDS_EXT_UNREGISTER_CFM
CsrUint8 *CsrBtCmSdsExtUnregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdsExtUnregisterCfm *primitive;

    primitive = (CsrBtCmSdsExtUnregisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceRecHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_EXT_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDS_EXT_UNREGISTER_CFM
void *CsrBtCmSdsExtUnregisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdsExtUnregisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdsExtUnregisterCfm *) CsrPmemZalloc(sizeof(CsrBtCmSdsExtUnregisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceRecHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_EXT_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_EVENT_SCAN_IND
CsrSize CsrBtCmLeEventScanIndSizeof(void *msg)
{
    CsrBtCmLeEventScanInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_EVENT_SCAN_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_EVENT_SCAN_IND
CsrUint8 *CsrBtCmLeEventScanIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeEventScanInd *primitive;

    primitive = (CsrBtCmLeEventScanInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->event);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scanType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->window);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_EVENT_SCAN_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_EVENT_SCAN_IND
void *CsrBtCmLeEventScanIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeEventScanInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeEventScanInd *) CsrPmemZalloc(sizeof(CsrBtCmLeEventScanInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->event, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scanType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->window, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_EVENT_SCAN_IND */

#ifndef EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_IND
CsrSize CsrBtCmRfcModeChangeIndSizeof(void *msg)
{
    CsrBtCmRfcModeChangeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_IND
CsrUint8 *CsrBtCmRfcModeChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmRfcModeChangeInd *primitive;

    primitive = (CsrBtCmRfcModeChangeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_IND
void *CsrBtCmRfcModeChangeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmRfcModeChangeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmRfcModeChangeInd *) CsrPmemZalloc(sizeof(CsrBtCmRfcModeChangeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_RFC_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_DISCONNECT_REQ
CsrSize CsrBtCmDisconnectReqSizeof(void *msg)
{
    CsrBtCmDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DISCONNECT_REQ
CsrUint8 *CsrBtCmDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDisconnectReq *primitive;

    primitive = (CsrBtCmDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DISCONNECT_REQ
void *CsrBtCmDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtCmDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM
CsrSize CsrBtCmScoAcceptConnectCfmSizeof(void *msg)
{
    CsrBtCmScoAcceptConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM
CsrUint8 *CsrBtCmScoAcceptConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmScoAcceptConnectCfm *primitive;

    primitive = (CsrBtCmScoAcceptConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->linkType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->txInterval);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->weSco);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rxPacketLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->txPacketLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->airMode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->eScoHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmSlot);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM
void *CsrBtCmScoAcceptConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmScoAcceptConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmScoAcceptConnectCfm *) CsrPmemZalloc(sizeof(CsrBtCmScoAcceptConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->linkType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->txInterval, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->weSco, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rxPacketLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->txPacketLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->airMode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->eScoHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmSlot, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DM_WRITE_LP_SETTINGS_REQ
CsrSize CsrBtCmDmWriteLpSettingsReqSizeof(void *msg)
{
    CsrBtCmDmWriteLpSettingsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DM_WRITE_LP_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_WRITE_LP_SETTINGS_REQ
CsrUint8 *CsrBtCmDmWriteLpSettingsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDmWriteLpSettingsReq *primitive;

    primitive = (CsrBtCmDmWriteLpSettingsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->link_policy_settings);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->activePlayer);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_WRITE_LP_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_WRITE_LP_SETTINGS_REQ
void *CsrBtCmDmWriteLpSettingsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDmWriteLpSettingsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDmWriteLpSettingsReq *) CsrPmemZalloc(sizeof(CsrBtCmDmWriteLpSettingsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->link_policy_settings, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->activePlayer, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_WRITE_LP_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_IND
CsrSize CsrBtCmBnepSwitchRoleIndSizeof(void *msg)
{
    CsrBtCmBnepSwitchRoleInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_IND
CsrUint8 *CsrBtCmBnepSwitchRoleIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBnepSwitchRoleInd *primitive;

    primitive = (CsrBtCmBnepSwitchRoleInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_IND
void *CsrBtCmBnepSwitchRoleIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepSwitchRoleInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepSwitchRoleInd *) CsrPmemZalloc(sizeof(CsrBtCmBnepSwitchRoleInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_ADVERTISE_REQ
CsrSize CsrBtCmLeAdvertiseReqSizeof(void *msg)
{
    CsrBtCmLeAdvertiseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->advertisingDataLength * sizeof(CsrUint8)) + (primitive->scanResponseDataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_ADVERTISE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_ADVERTISE_REQ
CsrUint8 *CsrBtCmLeAdvertiseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeAdvertiseReq *primitive;

    primitive = (CsrBtCmLeAdvertiseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->paramChange);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->advertisingDataLength);
    CsrMemCpySer(buffer, length, &primitive->advertisingData, sizeof(primitive->advertisingData));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scanResponseDataLength);
    CsrMemCpySer(buffer, length, &primitive->scanResponseData, sizeof(primitive->scanResponseData));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->advIntervalMin);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->advIntervalMax);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->advertisingType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->directAddress.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->directAddress.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->directAddress.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->directAddress.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->advertisingChannelMap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->advertisingFilterPolicy);
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
#endif /* EXCLUDE_CSR_BT_CM_LE_ADVERTISE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_ADVERTISE_REQ
void *CsrBtCmLeAdvertiseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeAdvertiseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeAdvertiseReq *) CsrPmemZalloc(sizeof(CsrBtCmLeAdvertiseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->paramChange, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->advertisingDataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->advertisingData), buffer, &offset, ((CsrSize) sizeof(primitive->advertisingData)));
    CsrUint8Des((CsrUint8 *) &primitive->scanResponseDataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->scanResponseData), buffer, &offset, ((CsrSize) sizeof(primitive->scanResponseData)));
    CsrUint16Des((CsrUint16 *) &primitive->advIntervalMin, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->advIntervalMax, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->advertisingType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->directAddress.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->directAddress.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->directAddress.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->directAddress.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->advertisingChannelMap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->advertisingFilterPolicy, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_CM_LE_ADVERTISE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_ADVERTISE_REQ
void CsrBtCmLeAdvertiseReqSerFree(void *msg)
{
    CsrBtCmLeAdvertiseReq *primitive;

    primitive = (CsrBtCmLeAdvertiseReq *) msg;

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
#endif /* EXCLUDE_CSR_BT_CM_LE_ADVERTISE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_CFM
CsrSize CsrBtCmConnectCfmSizeof(void *msg)
{
    CsrBtCmConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_CFM
CsrUint8 *CsrBtCmConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmConnectCfm *primitive;

    primitive = (CsrBtCmConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->profileMaxFrameSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->validPortPar);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->portPar.parameter_mask);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_CFM
void *CsrBtCmConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmConnectCfm *) CsrPmemZalloc(sizeof(CsrBtCmConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->profileMaxFrameSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->validPortPar, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->portPar.parameter_mask, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_REQ
CsrSize CsrBtCmReadLocalExtFeaturesReqSizeof(void *msg)
{
    CsrBtCmReadLocalExtFeaturesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_REQ
CsrUint8 *CsrBtCmReadLocalExtFeaturesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadLocalExtFeaturesReq *primitive;

    primitive = (CsrBtCmReadLocalExtFeaturesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pageNum);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_REQ
void *CsrBtCmReadLocalExtFeaturesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadLocalExtFeaturesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadLocalExtFeaturesReq *) CsrPmemZalloc(sizeof(CsrBtCmReadLocalExtFeaturesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pageNum, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_REQ
CsrSize CsrBtCmSetEventMaskReqSizeof(void *msg)
{
    CsrBtCmSetEventMaskReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_REQ
CsrUint8 *CsrBtCmSetEventMaskReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSetEventMaskReq *primitive;

    primitive = (CsrBtCmSetEventMaskReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->eventMask);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->conditionMask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_REQ
void *CsrBtCmSetEventMaskReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSetEventMaskReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSetEventMaskReq *) CsrPmemZalloc(sizeof(CsrBtCmSetEventMaskReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->eventMask, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->conditionMask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_REQ
CsrSize CsrBtCmReadFailedContactCounterReqSizeof(void *msg)
{
    CsrBtCmReadFailedContactCounterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_REQ
CsrUint8 *CsrBtCmReadFailedContactCounterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadFailedContactCounterReq *primitive;

    primitive = (CsrBtCmReadFailedContactCounterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_REQ
void *CsrBtCmReadFailedContactCounterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadFailedContactCounterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadFailedContactCounterReq *) CsrPmemZalloc(sizeof(CsrBtCmReadFailedContactCounterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_FAILED_CONTACT_COUNTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_CONNECT_IND
CsrSize CsrBtCmBnepConnectIndSizeof(void *msg)
{
    CsrBtCmBnepConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_CONNECT_IND
CsrUint8 *CsrBtCmBnepConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBnepConnectInd *primitive;

    primitive = (CsrBtCmBnepConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->id);
    CsrMemCpySer(buffer, length, ((const void *) primitive->rem_addr.w), ((CsrSize) 2 * (3)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rem_uuid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->loc_uuid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->profileMaxFrameSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_CONNECT_IND
void *CsrBtCmBnepConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepConnectInd *) CsrPmemZalloc(sizeof(CsrBtCmBnepConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->rem_addr.w), buffer, &offset, ((CsrSize) 2 * (3)));
    CsrUint16Des((CsrUint16 *) &primitive->rem_uuid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->loc_uuid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->profileMaxFrameSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_IND
CsrSize CsrBtCmMapScoPcmIndSizeof(void *msg)
{
    CsrBtCmMapScoPcmInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_IND */

#ifndef EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_IND
CsrUint8 *CsrBtCmMapScoPcmIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmMapScoPcmInd *primitive;

    primitive = (CsrBtCmMapScoPcmInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->linkType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_IND */

#ifndef EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_IND
void *CsrBtCmMapScoPcmIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmMapScoPcmInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmMapScoPcmInd *) CsrPmemZalloc(sizeof(CsrBtCmMapScoPcmInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->linkType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_MAP_SCO_PCM_IND */

#ifndef EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_CFM
CsrSize CsrBtCmGetLinkQualityCfmSizeof(void *msg)
{
    CsrBtCmGetLinkQualityCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_CFM
CsrUint8 *CsrBtCmGetLinkQualityCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmGetLinkQualityCfm *primitive;

    primitive = (CsrBtCmGetLinkQualityCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->linkQuality);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_CFM
void *CsrBtCmGetLinkQualityCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmGetLinkQualityCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmGetLinkQualityCfm *) CsrPmemZalloc(sizeof(CsrBtCmGetLinkQualityCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->linkQuality, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_GET_LINK_QUALITY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_REQ
CsrSize CsrBtCmReadRemoteVersionReqSizeof(void *msg)
{
    CsrBtCmReadRemoteVersionReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_REQ
CsrUint8 *CsrBtCmReadRemoteVersionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadRemoteVersionReq *primitive;

    primitive = (CsrBtCmReadRemoteVersionReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_REQ
void *CsrBtCmReadRemoteVersionReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadRemoteVersionReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadRemoteVersionReq *) CsrPmemZalloc(sizeof(CsrBtCmReadRemoteVersionReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_VERSION_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_CFM
CsrSize CsrBtCmReadAfhChannelMapCfmSizeof(void *msg)
{
    CsrBtCmReadAfhChannelMapCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_CFM
CsrUint8 *CsrBtCmReadAfhChannelMapCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadAfhChannelMapCfm *primitive;

    primitive = (CsrBtCmReadAfhChannelMapCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);
    CsrMemCpySer(buffer, length, ((const void *) primitive->afhMap), ((CsrSize) (10)));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_CFM
void *CsrBtCmReadAfhChannelMapCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadAfhChannelMapCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadAfhChannelMapCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadAfhChannelMapCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->afhMap), buffer, &offset, ((CsrSize) (10)));
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_CFM */

#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_IND
CsrSize CsrBtCmPortnegIndSizeof(void *msg)
{
    CsrBtCmPortnegInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_IND
CsrUint8 *CsrBtCmPortnegIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmPortnegInd *primitive;

    primitive = (CsrBtCmPortnegInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->portPar.parameter_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->request);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_IND
void *CsrBtCmPortnegIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmPortnegInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmPortnegInd *) CsrPmemZalloc(sizeof(CsrBtCmPortnegInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->portPar.parameter_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->request, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_IND */

#ifndef EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_CFM
CsrSize CsrBtCmRoleDiscoveryCfmSizeof(void *msg)
{
    CsrBtCmRoleDiscoveryCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_CFM
CsrUint8 *CsrBtCmRoleDiscoveryCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmRoleDiscoveryCfm *primitive;

    primitive = (CsrBtCmRoleDiscoveryCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_CFM
void *CsrBtCmRoleDiscoveryCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmRoleDiscoveryCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmRoleDiscoveryCfm *) CsrPmemZalloc(sizeof(CsrBtCmRoleDiscoveryCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_ROLE_DISCOVERY_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND
CsrSize CsrBtCmL2caConnectionlessDataIndSizeof(void *msg)
{
    CsrBtCmL2caConnectionlessDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND
CsrUint8 *CsrBtCmL2caConnectionlessDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caConnectionlessDataInd *primitive;

    primitive = (CsrBtCmL2caConnectionlessDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connectionlessPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND
void *CsrBtCmL2caConnectionlessDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caConnectionlessDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caConnectionlessDataInd *) CsrPmemZalloc(sizeof(CsrBtCmL2caConnectionlessDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connectionlessPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    if (primitive->length)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND
void CsrBtCmL2caConnectionlessDataIndSerFree(void *msg)
{
    CsrBtCmL2caConnectionlessDataInd *primitive;

    primitive = (CsrBtCmL2caConnectionlessDataInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_CONNECTIONLESS_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_REQ
CsrSize CsrBtCmLeReceiverTestReqSizeof(void *msg)
{
    CsrBtCmLeReceiverTestReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_REQ
CsrUint8 *CsrBtCmLeReceiverTestReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeReceiverTestReq *primitive;

    primitive = (CsrBtCmLeReceiverTestReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rxFrequency);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_REQ
void *CsrBtCmLeReceiverTestReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeReceiverTestReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeReceiverTestReq *) CsrPmemZalloc(sizeof(CsrBtCmLeReceiverTestReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->rxFrequency, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_RECEIVER_TEST_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_REQ
CsrSize CsrBtCmReadAdvertisingChTxPowerReqSizeof(void *msg)
{
    CsrBtCmReadAdvertisingChTxPowerReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_REQ
CsrUint8 *CsrBtCmReadAdvertisingChTxPowerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadAdvertisingChTxPowerReq *primitive;

    primitive = (CsrBtCmReadAdvertisingChTxPowerReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_REQ
void *CsrBtCmReadAdvertisingChTxPowerReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadAdvertisingChTxPowerReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadAdvertisingChTxPowerReq *) CsrPmemZalloc(sizeof(CsrBtCmReadAdvertisingChTxPowerReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ADVERTISING_CH_TX_POWER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_REQ
CsrSize CsrBtCmModeChangeReqSizeof(void *msg)
{
    CsrBtCmModeChangeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_REQ
CsrUint8 *CsrBtCmModeChangeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmModeChangeReq *primitive;

    primitive = (CsrBtCmModeChangeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->forceSniffSettings);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sniffSettings.max_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sniffSettings.min_interval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sniffSettings.attempt);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->sniffSettings.timeout);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_MODE_CHANGE_REQ
void *CsrBtCmModeChangeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmModeChangeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmModeChangeReq *) CsrPmemZalloc(sizeof(CsrBtCmModeChangeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->forceSniffSettings, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sniffSettings.max_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sniffSettings.min_interval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sniffSettings.attempt, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->sniffSettings.timeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_MODE_CHANGE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_RES
CsrSize CsrBtCmL2caDataResSizeof(void *msg)
{
    CsrBtCmL2caDataRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_RES */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_RES
CsrUint8 *CsrBtCmL2caDataResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caDataRes *primitive;

    primitive = (CsrBtCmL2caDataRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_RES */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_DATA_RES
void *CsrBtCmL2caDataResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caDataRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caDataRes *) CsrPmemZalloc(sizeof(CsrBtCmL2caDataRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_DATA_RES */

#ifndef EXCLUDE_CSR_BT_CM_SYNC_DISCONNECT_IND
CsrSize CsrBtCmSyncDisconnectIndSizeof(void *msg)
{
    CsrBtCmSyncDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SYNC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_SYNC_DISCONNECT_IND
CsrUint8 *CsrBtCmSyncDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSyncDisconnectInd *primitive;

    primitive = (CsrBtCmSyncDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->syncHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reason);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SYNC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_SYNC_DISCONNECT_IND
void *CsrBtCmSyncDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSyncDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSyncDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtCmSyncDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->syncHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reason, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SYNC_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_COD_CFM
CsrSize CsrBtCmReadCodCfmSizeof(void *msg)
{
    CsrBtCmReadCodCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_COD_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_COD_CFM
CsrUint8 *CsrBtCmReadCodCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadCodCfm *primitive;

    primitive = (CsrBtCmReadCodCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->classOfDevice);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_COD_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_COD_CFM
void *CsrBtCmReadCodCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadCodCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadCodCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadCodCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->classOfDevice, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_COD_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SNIFF_SUB_RATING_IND
CsrSize CsrBtCmSniffSubRatingIndSizeof(void *msg)
{
    CsrBtCmSniffSubRatingInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SNIFF_SUB_RATING_IND */

#ifndef EXCLUDE_CSR_BT_CM_SNIFF_SUB_RATING_IND
CsrUint8 *CsrBtCmSniffSubRatingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSniffSubRatingInd *primitive;

    primitive = (CsrBtCmSniffSubRatingInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxTxLatency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxRxLatency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->minRemoteTimeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->minLocalTimeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SNIFF_SUB_RATING_IND */

#ifndef EXCLUDE_CSR_BT_CM_SNIFF_SUB_RATING_IND
void *CsrBtCmSniffSubRatingIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSniffSubRatingInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSniffSubRatingInd *) CsrPmemZalloc(sizeof(CsrBtCmSniffSubRatingInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxTxLatency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxRxLatency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->minRemoteTimeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->minLocalTimeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SNIFF_SUB_RATING_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_CFM
CsrSize CsrBtCmLeConnparamCfmSizeof(void *msg)
{
    CsrBtCmLeConnparamCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_CFM
CsrUint8 *CsrBtCmLeConnparamCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeConnparamCfm *primitive;

    primitive = (CsrBtCmLeConnparamCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_CFM
void *CsrBtCmLeConnparamCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeConnparamCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeConnparamCfm *) CsrPmemZalloc(sizeof(CsrBtCmLeConnparamCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_SCAN_REQ
CsrSize CsrBtCmLeScanReqSizeof(void *msg)
{
    CsrBtCmLeScanReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_SCAN_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_SCAN_REQ
CsrUint8 *CsrBtCmLeScanReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeScanReq *primitive;

    primitive = (CsrBtCmLeScanReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->paramChange);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scanType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scanInterval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scanWindow);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scanningFilterPolicy);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->filterDuplicates);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_SCAN_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_SCAN_REQ
void *CsrBtCmLeScanReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeScanReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeScanReq *) CsrPmemZalloc(sizeof(CsrBtCmLeScanReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->paramChange, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scanType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scanInterval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scanWindow, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scanningFilterPolicy, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->filterDuplicates, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_SCAN_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_UNREGISTER_REQ
CsrSize CsrBtCmL2caUnregisterReqSizeof(void *msg)
{
    CsrBtCmL2caUnregisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_UNREGISTER_REQ
CsrUint8 *CsrBtCmL2caUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caUnregisterReq *primitive;

    primitive = (CsrBtCmL2caUnregisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_UNREGISTER_REQ
void *CsrBtCmL2caUnregisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caUnregisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caUnregisterReq *) CsrPmemZalloc(sizeof(CsrBtCmL2caUnregisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_IND
CsrSize CsrBtCmGetSecurityConfIndSizeof(void *msg)
{
    CsrBtCmGetSecurityConfInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_IND */

#ifndef EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_IND
CsrUint8 *CsrBtCmGetSecurityConfIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmGetSecurityConfInd *primitive;

    primitive = (CsrBtCmGetSecurityConfInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->lmpVersion);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_IND */

#ifndef EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_IND
void *CsrBtCmGetSecurityConfIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmGetSecurityConfInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmGetSecurityConfInd *) CsrPmemZalloc(sizeof(CsrBtCmGetSecurityConfInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->lmpVersion, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_CFM
CsrSize CsrBtCmReadLocalExtFeaturesCfmSizeof(void *msg)
{
    CsrBtCmReadLocalExtFeaturesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_CFM
CsrUint8 *CsrBtCmReadLocalExtFeaturesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadLocalExtFeaturesCfm *primitive;

    primitive = (CsrBtCmReadLocalExtFeaturesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pageNum);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->maxPageNum);
    CsrMemCpySer(buffer, length, ((const void *) primitive->extLmpFeatures), ((CsrSize) (8)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_CFM
void *CsrBtCmReadLocalExtFeaturesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadLocalExtFeaturesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadLocalExtFeaturesCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadLocalExtFeaturesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pageNum, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->maxPageNum, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->extLmpFeatures), buffer, &offset, ((CsrSize) (8)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LOCAL_EXT_FEATURES_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_REQ
CsrSize CsrBtCmLeConnparamUpdateReqSizeof(void *msg)
{
    CsrBtCmLeConnparamUpdateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_REQ
CsrUint8 *CsrBtCmLeConnparamUpdateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeConnparamUpdateReq *primitive;

    primitive = (CsrBtCmLeConnparamUpdateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connIntervalMin);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connIntervalMax);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connLatency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supervisionTimeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->minimumCeLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maximumCeLength);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_REQ
void *CsrBtCmLeConnparamUpdateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeConnparamUpdateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeConnparamUpdateReq *) CsrPmemZalloc(sizeof(CsrBtCmLeConnparamUpdateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connIntervalMin, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connIntervalMax, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connLatency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supervisionTimeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->minimumCeLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maximumCeLength, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ALWAYS_SUPPORT_MASTER_ROLE_REQ
CsrSize CsrBtCmAlwaysSupportMasterRoleReqSizeof(void *msg)
{
    CsrBtCmAlwaysSupportMasterRoleReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_ALWAYS_SUPPORT_MASTER_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ALWAYS_SUPPORT_MASTER_ROLE_REQ
CsrUint8 *CsrBtCmAlwaysSupportMasterRoleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmAlwaysSupportMasterRoleReq *primitive;

    primitive = (CsrBtCmAlwaysSupportMasterRoleReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->alwaysSupportMasterRole);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_ALWAYS_SUPPORT_MASTER_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ALWAYS_SUPPORT_MASTER_ROLE_REQ
void *CsrBtCmAlwaysSupportMasterRoleReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmAlwaysSupportMasterRoleReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmAlwaysSupportMasterRoleReq *) CsrPmemZalloc(sizeof(CsrBtCmAlwaysSupportMasterRoleReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->alwaysSupportMasterRole, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_ALWAYS_SUPPORT_MASTER_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_REQ
CsrSize CsrBtCmReadAfhChannelAssessmentModeReqSizeof(void *msg)
{
    CsrBtCmReadAfhChannelAssessmentModeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_REQ
CsrUint8 *CsrBtCmReadAfhChannelAssessmentModeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadAfhChannelAssessmentModeReq *primitive;

    primitive = (CsrBtCmReadAfhChannelAssessmentModeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_REQ
void *CsrBtCmReadAfhChannelAssessmentModeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadAfhChannelAssessmentModeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadAfhChannelAssessmentModeReq *) CsrPmemZalloc(sizeof(CsrBtCmReadAfhChannelAssessmentModeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_ASSESSMENT_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_READ_LOCAL_OOB_DATA_REQ
CsrSize CsrBtCmSmReadLocalOobDataReqSizeof(void *msg)
{
    CsrBtCmSmReadLocalOobDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_READ_LOCAL_OOB_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_READ_LOCAL_OOB_DATA_REQ
CsrUint8 *CsrBtCmSmReadLocalOobDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmReadLocalOobDataReq *primitive;

    primitive = (CsrBtCmSmReadLocalOobDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_READ_LOCAL_OOB_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_READ_LOCAL_OOB_DATA_REQ
void *CsrBtCmSmReadLocalOobDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmReadLocalOobDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmReadLocalOobDataReq *) CsrPmemZalloc(sizeof(CsrBtCmSmReadLocalOobDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_READ_LOCAL_OOB_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SEND_KEYPRESS_NOTIFICATION_REQ
CsrSize CsrBtCmSmSendKeypressNotificationReqSizeof(void *msg)
{
    CsrBtCmSmSendKeypressNotificationReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_SEND_KEYPRESS_NOTIFICATION_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SEND_KEYPRESS_NOTIFICATION_REQ
CsrUint8 *CsrBtCmSmSendKeypressNotificationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmSendKeypressNotificationReq *primitive;

    primitive = (CsrBtCmSmSendKeypressNotificationReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->notificationType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_SEND_KEYPRESS_NOTIFICATION_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SEND_KEYPRESS_NOTIFICATION_REQ
void *CsrBtCmSmSendKeypressNotificationReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmSendKeypressNotificationReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmSendKeypressNotificationReq *) CsrPmemZalloc(sizeof(CsrBtCmSmSendKeypressNotificationReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->notificationType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_SEND_KEYPRESS_NOTIFICATION_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONNECTION_REJ_SECURITY_IND
CsrSize CsrBtCmConnectionRejSecurityIndSizeof(void *msg)
{
    CsrBtCmConnectionRejSecurityInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECTION_REJ_SECURITY_IND */

#ifndef EXCLUDE_CSR_BT_CM_CONNECTION_REJ_SECURITY_IND
CsrUint8 *CsrBtCmConnectionRejSecurityIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmConnectionRejSecurityInd *primitive;

    primitive = (CsrBtCmConnectionRejSecurityInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->cancelInitiated);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECTION_REJ_SECURITY_IND */

#ifndef EXCLUDE_CSR_BT_CM_CONNECTION_REJ_SECURITY_IND
void *CsrBtCmConnectionRejSecurityIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmConnectionRejSecurityInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmConnectionRejSecurityInd *) CsrPmemZalloc(sizeof(CsrBtCmConnectionRejSecurityInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cancelInitiated, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECTION_REJ_SECURITY_IND */

#ifndef EXCLUDE_CSR_BT_CM_DM_UPDATE_AND_CLEAR_CACHED_PARAM_REQ
CsrSize CsrBtCmDmUpdateAndClearCachedParamReqSizeof(void *msg)
{
    CsrBtCmDmUpdateAndClearCachedParamReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DM_UPDATE_AND_CLEAR_CACHED_PARAM_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_UPDATE_AND_CLEAR_CACHED_PARAM_REQ
CsrUint8 *CsrBtCmDmUpdateAndClearCachedParamReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDmUpdateAndClearCachedParamReq *primitive;

    primitive = (CsrBtCmDmUpdateAndClearCachedParamReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->devAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->devAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->devAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_UPDATE_AND_CLEAR_CACHED_PARAM_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_UPDATE_AND_CLEAR_CACHED_PARAM_REQ
void *CsrBtCmDmUpdateAndClearCachedParamReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDmUpdateAndClearCachedParamReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDmUpdateAndClearCachedParamReq *) CsrPmemZalloc(sizeof(CsrBtCmDmUpdateAndClearCachedParamReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->devAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->devAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->devAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_UPDATE_AND_CLEAR_CACHED_PARAM_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_REQ
CsrSize CsrBtCmWriteScanEnableReqSizeof(void *msg)
{
    CsrBtCmWriteScanEnableReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_REQ
CsrUint8 *CsrBtCmWriteScanEnableReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteScanEnableReq *primitive;

    primitive = (CsrBtCmWriteScanEnableReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->disableInquiryScan);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->disablePageScan);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_REQ
void *CsrBtCmWriteScanEnableReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteScanEnableReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteScanEnableReq *) CsrPmemZalloc(sizeof(CsrBtCmWriteScanEnableReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->disableInquiryScan, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->disablePageScan, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_SCAN_ENABLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_REQ
CsrSize CsrBtCmScoAcceptConnectReqSizeof(void *msg)
{
    CsrBtCmScoAcceptConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_REQ
CsrUint8 *CsrBtCmScoAcceptConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmScoAcceptConnectReq *primitive;

    primitive = (CsrBtCmScoAcceptConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->audioQuality);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->txBandwidth);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->rxBandwidth);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxLatency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->voiceSettings);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reTxEffort);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_REQ
void *CsrBtCmScoAcceptConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmScoAcceptConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmScoAcceptConnectReq *) CsrPmemZalloc(sizeof(CsrBtCmScoAcceptConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->audioQuality, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->txBandwidth, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->rxBandwidth, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxLatency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->voiceSettings, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reTxEffort, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_ACCEPT_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_CANCEL_CONNECT_REQ
CsrSize CsrBtCmSmCancelConnectReqSizeof(void *msg)
{
    CsrBtCmSmCancelConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_CANCEL_CONNECT_REQ
CsrUint8 *CsrBtCmSmCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmCancelConnectReq *primitive;

    primitive = (CsrBtCmSmCancelConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_CANCEL_CONNECT_REQ
void *CsrBtCmSmCancelConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmCancelConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmCancelConnectReq *) CsrPmemZalloc(sizeof(CsrBtCmSmCancelConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_REQ
CsrSize CsrBtCmSdcReleaseResourcesReqSizeof(void *msg)
{
    CsrBtCmSdcReleaseResourcesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_REQ
CsrUint8 *CsrBtCmSdcReleaseResourcesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcReleaseResourcesReq *primitive;

    primitive = (CsrBtCmSdcReleaseResourcesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_REQ
void *CsrBtCmSdcReleaseResourcesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcReleaseResourcesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcReleaseResourcesReq *) CsrPmemZalloc(sizeof(CsrBtCmSdcReleaseResourcesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_CFM
CsrSize CsrBtCmLeWhitelistSetCfmSizeof(void *msg)
{
    CsrBtCmLeWhitelistSetCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_CFM
CsrUint8 *CsrBtCmLeWhitelistSetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeWhitelistSetCfm *primitive;

    primitive = (CsrBtCmLeWhitelistSetCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->operation);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_CFM
void *CsrBtCmLeWhitelistSetCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeWhitelistSetCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeWhitelistSetCfm *) CsrPmemZalloc(sizeof(CsrBtCmLeWhitelistSetCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->operation, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_WHITELIST_SET_CFM */

#ifndef EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_CFM
CsrSize CsrBtCmEirUpdateManufacturerDataCfmSizeof(void *msg)
{
    CsrBtCmEirUpdateManufacturerDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_CFM
CsrUint8 *CsrBtCmEirUpdateManufacturerDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmEirUpdateManufacturerDataCfm *primitive;

    primitive = (CsrBtCmEirUpdateManufacturerDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_CFM
void *CsrBtCmEirUpdateManufacturerDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmEirUpdateManufacturerDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmEirUpdateManufacturerDataCfm *) CsrPmemZalloc(sizeof(CsrBtCmEirUpdateManufacturerDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_EIR_UPDATE_MANUFACTURER_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_REQ
CsrSize CsrBtCmWritePageToReqSizeof(void *msg)
{
    CsrBtCmWritePageToReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_REQ
CsrUint8 *CsrBtCmWritePageToReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWritePageToReq *primitive;

    primitive = (CsrBtCmWritePageToReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pageTimeout);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_REQ
void *CsrBtCmWritePageToReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWritePageToReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWritePageToReq *) CsrPmemZalloc(sizeof(CsrBtCmWritePageToReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pageTimeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGE_TO_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_REQ
CsrSize CsrBtCmBnepCancelConnectAcceptReqSizeof(void *msg)
{
    CsrBtCmBnepCancelConnectAcceptReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_REQ
CsrUint8 *CsrBtCmBnepCancelConnectAcceptReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBnepCancelConnectAcceptReq *primitive;

    primitive = (CsrBtCmBnepCancelConnectAcceptReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_REQ
void *CsrBtCmBnepCancelConnectAcceptReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepCancelConnectAcceptReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepCancelConnectAcceptReq *) CsrPmemZalloc(sizeof(CsrBtCmBnepCancelConnectAcceptReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SET_DEFAULT_SEC_LEVEL_REQ
CsrSize CsrBtCmSmSetDefaultSecLevelReqSizeof(void *msg)
{
    CsrBtCmSmSetDefaultSecLevelReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_SET_DEFAULT_SEC_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SET_DEFAULT_SEC_LEVEL_REQ
CsrUint8 *CsrBtCmSmSetDefaultSecLevelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmSetDefaultSecLevelReq *primitive;

    primitive = (CsrBtCmSmSetDefaultSecLevelReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->seclDefault);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_SET_DEFAULT_SEC_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SET_DEFAULT_SEC_LEVEL_REQ
void *CsrBtCmSmSetDefaultSecLevelReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmSetDefaultSecLevelReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmSetDefaultSecLevelReq *) CsrPmemZalloc(sizeof(CsrBtCmSmSetDefaultSecLevelReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->seclDefault, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_SET_DEFAULT_SEC_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ
CsrSize CsrBtCmDmL2caModeSettingsReqSizeof(void *msg)
{
    CsrBtCmDmL2caModeSettingsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->sniffSettingsSize * sizeof(CsrBtSniffSettings)) + (primitive->parkSettingsSize * sizeof(CsrBtParkSettings)) + (primitive->ssrSettingsSize * sizeof(CsrBtSsrSettingsDownstream)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ
CsrUint8 *CsrBtCmDmL2caModeSettingsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDmL2caModeSettingsReq *primitive;

    primitive = (CsrBtCmDmL2caModeSettingsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrMemCpySer(buffer, length, &primitive->sniffSettings, sizeof(primitive->sniffSettings));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sniffSettingsSize);
    CsrMemCpySer(buffer, length, &primitive->parkSettings, sizeof(primitive->parkSettings));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parkSettingsSize);
    CsrMemCpySer(buffer, length, &primitive->ssrSettings, sizeof(primitive->ssrSettings));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ssrSettingsSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->lowPowerPriority);
    if(primitive->sniffSettings)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->sniffSettings), ((CsrSize) (primitive->sniffSettingsSize * sizeof(CsrBtSniffSettings))));
    }
    if(primitive->parkSettings)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->parkSettings), ((CsrSize) (primitive->parkSettingsSize * sizeof(CsrBtParkSettings))));
    }
    if(primitive->ssrSettings)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->ssrSettings), ((CsrSize) (primitive->ssrSettingsSize * sizeof(CsrBtSsrSettingsDownstream))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ
void *CsrBtCmDmL2caModeSettingsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDmL2caModeSettingsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDmL2caModeSettingsReq *) CsrPmemZalloc(sizeof(CsrBtCmDmL2caModeSettingsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->sniffSettings), buffer, &offset, ((CsrSize) sizeof(primitive->sniffSettings)));
    CsrUint8Des((CsrUint8 *) &primitive->sniffSettingsSize, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->parkSettings), buffer, &offset, ((CsrSize) sizeof(primitive->parkSettings)));
    CsrUint8Des((CsrUint8 *) &primitive->parkSettingsSize, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ssrSettings), buffer, &offset, ((CsrSize) sizeof(primitive->ssrSettings)));
    CsrUint8Des((CsrUint8 *) &primitive->ssrSettingsSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->lowPowerPriority, buffer, &offset);
    if (primitive->sniffSettingsSize)
    {
        primitive->sniffSettings = CsrPmemZalloc(((CsrUint32) (primitive->sniffSettingsSize * sizeof(CsrBtSniffSettings))));
        CsrMemCpyDes(((void *) primitive->sniffSettings), buffer, &offset, ((CsrSize) (primitive->sniffSettingsSize * sizeof(CsrBtSniffSettings))));
    }
    else
    {
        primitive->sniffSettings = NULL;
    }
    if (primitive->parkSettingsSize)
    {
        primitive->parkSettings = CsrPmemZalloc(((CsrUint32) (primitive->parkSettingsSize * sizeof(CsrBtParkSettings))));
        CsrMemCpyDes(((void *) primitive->parkSettings), buffer, &offset, ((CsrSize) (primitive->parkSettingsSize * sizeof(CsrBtParkSettings))));
    }
    else
    {
        primitive->parkSettings = NULL;
    }
    if (primitive->ssrSettingsSize)
    {
        primitive->ssrSettings = CsrPmemZalloc(((CsrUint32) (primitive->ssrSettingsSize * sizeof(CsrBtSsrSettingsDownstream))));
        CsrMemCpyDes(((void *) primitive->ssrSettings), buffer, &offset, ((CsrSize) (primitive->ssrSettingsSize * sizeof(CsrBtSsrSettingsDownstream))));
    }
    else
    {
        primitive->ssrSettings = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ
void CsrBtCmDmL2caModeSettingsReqSerFree(void *msg)
{
    CsrBtCmDmL2caModeSettingsReq *primitive;

    primitive = (CsrBtCmDmL2caModeSettingsReq *) msg;

    if(primitive->sniffSettings != NULL)
    {
        CsrPmemFree(primitive->sniffSettings);
    }
    if(primitive->parkSettings != NULL)
    {
        CsrPmemFree(primitive->parkSettings);
    }
    if(primitive->ssrSettings != NULL)
    {
        CsrPmemFree(primitive->ssrSettings);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_DM_L2CA_MODE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_CMP_IND
CsrSize CsrBtCmLeConnparamUpdateCmpIndSizeof(void *msg)
{
    CsrBtCmLeConnparamUpdateCmpInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_CMP_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_CMP_IND
CsrUint8 *CsrBtCmLeConnparamUpdateCmpIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeConnparamUpdateCmpInd *primitive;

    primitive = (CsrBtCmLeConnparamUpdateCmpInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_CMP_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_CMP_IND
void *CsrBtCmLeConnparamUpdateCmpIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeConnparamUpdateCmpInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeConnparamUpdateCmpInd *) CsrPmemZalloc(sizeof(CsrBtCmLeConnparamUpdateCmpInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_CONNPARAM_UPDATE_CMP_IND */

#ifndef EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_CFM
CsrSize CsrBtCmReadTxPowerLevelCfmSizeof(void *msg)
{
    CsrBtCmReadTxPowerLevelCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_CFM
CsrUint8 *CsrBtCmReadTxPowerLevelCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadTxPowerLevelCfm *primitive;

    primitive = (CsrBtCmReadTxPowerLevelCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->powerLevel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_CFM
void *CsrBtCmReadTxPowerLevelCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadTxPowerLevelCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadTxPowerLevelCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadTxPowerLevelCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->powerLevel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_TX_POWER_LEVEL_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_CFM
CsrSize CsrBtCmSdsUnregisterCfmSizeof(void *msg)
{
    CsrBtCmSdsUnregisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_CFM
CsrUint8 *CsrBtCmSdsUnregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdsUnregisterCfm *primitive;

    primitive = (CsrBtCmSdsUnregisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceRecHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_CFM
void *CsrBtCmSdsUnregisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdsUnregisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdsUnregisterCfm *) CsrPmemZalloc(sizeof(CsrBtCmSdsUnregisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceRecHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_CFM
CsrSize CsrBtCmConnectAcceptCfmSizeof(void *msg)
{
    CsrBtCmConnectAcceptCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_CFM
CsrUint8 *CsrBtCmConnectAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmConnectAcceptCfm *primitive;

    primitive = (CsrBtCmConnectAcceptCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->profileMaxFrameSize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_CFM
void *CsrBtCmConnectAcceptCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmConnectAcceptCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmConnectAcceptCfm *) CsrPmemZalloc(sizeof(CsrBtCmConnectAcceptCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->profileMaxFrameSize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_REQ
CsrSize CsrBtCmClearEventFilterReqSizeof(void *msg)
{
    CsrBtCmClearEventFilterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_REQ
CsrUint8 *CsrBtCmClearEventFilterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmClearEventFilterReq *primitive;

    primitive = (CsrBtCmClearEventFilterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->filter);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_REQ
void *CsrBtCmClearEventFilterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmClearEventFilterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmClearEventFilterReq *) CsrPmemZalloc(sizeof(CsrBtCmClearEventFilterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->filter, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_RESULT_IND
CsrSize CsrBtCmInquiryResultIndSizeof(void *msg)
{
    CsrBtCmInquiryResultInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->eirDataLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_RESULT_IND
CsrUint8 *CsrBtCmInquiryResultIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmInquiryResultInd *primitive;

    primitive = (CsrBtCmInquiryResultInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->classOfDevice);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rssi);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->eirDataLength);
    CsrMemCpySer(buffer, length, &primitive->eirData, sizeof(primitive->eirData));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->status);
    if(primitive->eirData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->eirData), ((CsrSize) (primitive->eirDataLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_RESULT_IND
void *CsrBtCmInquiryResultIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmInquiryResultInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmInquiryResultInd *) CsrPmemZalloc(sizeof(CsrBtCmInquiryResultInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->classOfDevice, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->rssi, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->eirDataLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->eirData), buffer, &offset, ((CsrSize) sizeof(primitive->eirData)));
    CsrUint32Des((CsrUint32 *) &primitive->status, buffer, &offset);
    if (primitive->eirDataLength)
    {
        primitive->eirData = CsrPmemZalloc(((CsrUint32) (primitive->eirDataLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->eirData), buffer, &offset, ((CsrSize) (primitive->eirDataLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->eirData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_CM_INQUIRY_RESULT_IND
void CsrBtCmInquiryResultIndSerFree(void *msg)
{
    CsrBtCmInquiryResultInd *primitive;

    primitive = (CsrBtCmInquiryResultInd *) msg;

    if(primitive->eirData != NULL)
    {
        CsrPmemFree(primitive->eirData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_INQUIRY_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ
CsrSize CsrBtCmDmBnepModeSettingsReqSizeof(void *msg)
{
    CsrBtCmDmBnepModeSettingsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->sniffSettingsSize * sizeof(CsrBtSniffSettings)) + (primitive->parkSettingsSize * sizeof(CsrBtParkSettings)) + (primitive->ssrSettingsSize * sizeof(CsrBtSsrSettingsDownstream)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ
CsrUint8 *CsrBtCmDmBnepModeSettingsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDmBnepModeSettingsReq *primitive;

    primitive = (CsrBtCmDmBnepModeSettingsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->id);
    CsrMemCpySer(buffer, length, &primitive->sniffSettings, sizeof(primitive->sniffSettings));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sniffSettingsSize);
    CsrMemCpySer(buffer, length, &primitive->parkSettings, sizeof(primitive->parkSettings));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parkSettingsSize);
    CsrMemCpySer(buffer, length, &primitive->ssrSettings, sizeof(primitive->ssrSettings));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ssrSettingsSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->lowPowerPriority);
    if(primitive->sniffSettings)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->sniffSettings), ((CsrSize) (primitive->sniffSettingsSize * sizeof(CsrBtSniffSettings))));
    }
    if(primitive->parkSettings)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->parkSettings), ((CsrSize) (primitive->parkSettingsSize * sizeof(CsrBtParkSettings))));
    }
    if(primitive->ssrSettings)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->ssrSettings), ((CsrSize) (primitive->ssrSettingsSize * sizeof(CsrBtSsrSettingsDownstream))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ
void *CsrBtCmDmBnepModeSettingsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDmBnepModeSettingsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDmBnepModeSettingsReq *) CsrPmemZalloc(sizeof(CsrBtCmDmBnepModeSettingsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->sniffSettings), buffer, &offset, ((CsrSize) sizeof(primitive->sniffSettings)));
    CsrUint8Des((CsrUint8 *) &primitive->sniffSettingsSize, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->parkSettings), buffer, &offset, ((CsrSize) sizeof(primitive->parkSettings)));
    CsrUint8Des((CsrUint8 *) &primitive->parkSettingsSize, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ssrSettings), buffer, &offset, ((CsrSize) sizeof(primitive->ssrSettings)));
    CsrUint8Des((CsrUint8 *) &primitive->ssrSettingsSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->lowPowerPriority, buffer, &offset);
    if (primitive->sniffSettingsSize)
    {
        primitive->sniffSettings = CsrPmemZalloc(((CsrUint32) (primitive->sniffSettingsSize * sizeof(CsrBtSniffSettings))));
        CsrMemCpyDes(((void *) primitive->sniffSettings), buffer, &offset, ((CsrSize) (primitive->sniffSettingsSize * sizeof(CsrBtSniffSettings))));
    }
    else
    {
        primitive->sniffSettings = NULL;
    }
    if (primitive->parkSettingsSize)
    {
        primitive->parkSettings = CsrPmemZalloc(((CsrUint32) (primitive->parkSettingsSize * sizeof(CsrBtParkSettings))));
        CsrMemCpyDes(((void *) primitive->parkSettings), buffer, &offset, ((CsrSize) (primitive->parkSettingsSize * sizeof(CsrBtParkSettings))));
    }
    else
    {
        primitive->parkSettings = NULL;
    }
    if (primitive->ssrSettingsSize)
    {
        primitive->ssrSettings = CsrPmemZalloc(((CsrUint32) (primitive->ssrSettingsSize * sizeof(CsrBtSsrSettingsDownstream))));
        CsrMemCpyDes(((void *) primitive->ssrSettings), buffer, &offset, ((CsrSize) (primitive->ssrSettingsSize * sizeof(CsrBtSsrSettingsDownstream))));
    }
    else
    {
        primitive->ssrSettings = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ
void CsrBtCmDmBnepModeSettingsReqSerFree(void *msg)
{
    CsrBtCmDmBnepModeSettingsReq *primitive;

    primitive = (CsrBtCmDmBnepModeSettingsReq *) msg;

    if(primitive->sniffSettings != NULL)
    {
        CsrPmemFree(primitive->sniffSettings);
    }
    if(primitive->parkSettings != NULL)
    {
        CsrPmemFree(primitive->parkSettings);
    }
    if(primitive->ssrSettings != NULL)
    {
        CsrPmemFree(primitive->ssrSettings);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_DM_BNEP_MODE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SEC_MODE_CONFIG_REQ
CsrSize CsrBtCmSmSecModeConfigReqSizeof(void *msg)
{
    CsrBtCmSmSecModeConfigReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_SEC_MODE_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SEC_MODE_CONFIG_REQ
CsrUint8 *CsrBtCmSmSecModeConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmSecModeConfigReq *primitive;

    primitive = (CsrBtCmSmSecModeConfigReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->writeAuthEnable);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_SEC_MODE_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SEC_MODE_CONFIG_REQ
void *CsrBtCmSmSecModeConfigReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmSecModeConfigReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmSecModeConfigReq *) CsrPmemZalloc(sizeof(CsrBtCmSmSecModeConfigReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->writeAuthEnable, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_SEC_MODE_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_REGISTER_REQ
CsrSize CsrBtCmL2caRegisterReqSizeof(void *msg)
{
    CsrBtCmL2caRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_REGISTER_REQ
CsrUint8 *CsrBtCmL2caRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caRegisterReq *primitive;

    primitive = (CsrBtCmL2caRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mode_mask);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_REGISTER_REQ
void *CsrBtCmL2caRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caRegisterReq *) CsrPmemZalloc(sizeof(CsrBtCmL2caRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mode_mask, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDS_REGISTER_CFM
CsrSize CsrBtCmSdsRegisterCfmSizeof(void *msg)
{
    CsrBtCmSdsRegisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDS_REGISTER_CFM
CsrUint8 *CsrBtCmSdsRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdsRegisterCfm *primitive;

    primitive = (CsrBtCmSdsRegisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceRecHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDS_REGISTER_CFM
void *CsrBtCmSdsRegisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdsRegisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdsRegisterCfm *) CsrPmemZalloc(sizeof(CsrBtCmSdsRegisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceRecHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDS_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_RSSI_REQ
CsrSize CsrBtCmReadRssiReqSizeof(void *msg)
{
    CsrBtCmReadRssiReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_RSSI_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_RSSI_REQ
CsrUint8 *CsrBtCmReadRssiReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadRssiReq *primitive;

    primitive = (CsrBtCmReadRssiReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_RSSI_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_RSSI_REQ
void *CsrBtCmReadRssiReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadRssiReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadRssiReq *) CsrPmemZalloc(sizeof(CsrBtCmReadRssiReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_RSSI_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_REQ
CsrSize CsrBtCmReadLinkPolicyReqSizeof(void *msg)
{
    CsrBtCmReadLinkPolicyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_REQ
CsrUint8 *CsrBtCmReadLinkPolicyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadLinkPolicyReq *primitive;

    primitive = (CsrBtCmReadLinkPolicyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_REQ
void *CsrBtCmReadLinkPolicyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadLinkPolicyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadLinkPolicyReq *) CsrPmemZalloc(sizeof(CsrBtCmReadLinkPolicyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_LINK_POLICY_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SET_SEC_MODE_REQ
CsrSize CsrBtCmSmSetSecModeReqSizeof(void *msg)
{
    CsrBtCmSmSetSecModeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_SET_SEC_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SET_SEC_MODE_REQ
CsrUint8 *CsrBtCmSmSetSecModeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmSetSecModeReq *primitive;

    primitive = (CsrBtCmSmSetSecModeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode3Enc);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_SET_SEC_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_SET_SEC_MODE_REQ
void *CsrBtCmSmSetSecModeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmSetSecModeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmSetSecModeReq *) CsrPmemZalloc(sizeof(CsrBtCmSmSetSecModeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode3Enc, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_SET_SEC_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_RSSI_CFM
CsrSize CsrBtCmReadRssiCfmSizeof(void *msg)
{
    CsrBtCmReadRssiCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_RSSI_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_RSSI_CFM
CsrUint8 *CsrBtCmReadRssiCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadRssiCfm *primitive;

    primitive = (CsrBtCmReadRssiCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->rssi);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_RSSI_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_RSSI_CFM
void *CsrBtCmReadRssiCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadRssiCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadRssiCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadRssiCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->rssi, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_RSSI_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_REQ
CsrSize CsrBtCmSetEventFilterCodReqSizeof(void *msg)
{
    CsrBtCmSetEventFilterCodReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_REQ
CsrUint8 *CsrBtCmSetEventFilterCodReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSetEventFilterCodReq *primitive;

    primitive = (CsrBtCmSetEventFilterCodReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->selectInquiryFilter);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->autoAccept);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->cod);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->codMask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_REQ
void *CsrBtCmSetEventFilterCodReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSetEventFilterCodReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSetEventFilterCodReq *) CsrPmemZalloc(sizeof(CsrBtCmSetEventFilterCodReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->selectInquiryFilter, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->autoAccept, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->cod, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->codMask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_COD_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_REPAIR_RES
CsrSize CsrBtCmSmRepairResSizeof(void *msg)
{
    CsrBtCmSmRepairRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_REPAIR_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_REPAIR_RES
CsrUint8 *CsrBtCmSmRepairResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmRepairRes *primitive;

    primitive = (CsrBtCmSmRepairRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->repairId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_REPAIR_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_REPAIR_RES
void *CsrBtCmSmRepairResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmRepairRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmRepairRes *) CsrPmemZalloc(sizeof(CsrBtCmSmRepairRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->repairId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_REPAIR_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_REPAIR_IND
CsrSize CsrBtCmSmRepairIndSizeof(void *msg)
{
    CsrBtCmSmRepairInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_REPAIR_IND */

#ifndef EXCLUDE_CSR_BT_CM_SM_REPAIR_IND
CsrUint8 *CsrBtCmSmRepairIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmRepairInd *primitive;

    primitive = (CsrBtCmSmRepairInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->repairId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->linkKeyType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_REPAIR_IND */

#ifndef EXCLUDE_CSR_BT_CM_SM_REPAIR_IND
void *CsrBtCmSmRepairIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmRepairInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmRepairInd *) CsrPmemZalloc(sizeof(CsrBtCmSmRepairInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->repairId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->linkKeyType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_REPAIR_IND */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_CONNECT_REQ
CsrSize CsrBtCmCancelConnectReqSizeof(void *msg)
{
    CsrBtCmCancelConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_CONNECT_REQ
CsrUint8 *CsrBtCmCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmCancelConnectReq *primitive;

    primitive = (CsrBtCmCancelConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerCh);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->typeToCancel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_CONNECT_REQ
void *CsrBtCmCancelConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmCancelConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmCancelConnectReq *) CsrPmemZalloc(sizeof(CsrBtCmCancelConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerCh, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->typeToCancel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM
CsrSize CsrBtCmCancelAcceptConnectCfmSizeof(void *msg)
{
    CsrBtCmCancelAcceptConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM
CsrUint8 *CsrBtCmCancelAcceptConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmCancelAcceptConnectCfm *primitive;

    primitive = (CsrBtCmCancelAcceptConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM
void *CsrBtCmCancelAcceptConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmCancelAcceptConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmCancelAcceptConnectCfm *) CsrPmemZalloc(sizeof(CsrBtCmCancelAcceptConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CANCEL_ACCEPT_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_COD_REQ
CsrSize CsrBtCmReadCodReqSizeof(void *msg)
{
    CsrBtCmReadCodReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_COD_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_COD_REQ
CsrUint8 *CsrBtCmReadCodReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadCodReq *primitive;

    primitive = (CsrBtCmReadCodReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_COD_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_COD_REQ
void *CsrBtCmReadCodReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadCodReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadCodReq *) CsrPmemZalloc(sizeof(CsrBtCmReadCodReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_COD_REQ */

#ifndef EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_RES
CsrSize CsrBtCmGetSecurityConfResSizeof(void *msg)
{
    CsrBtCmGetSecurityConfRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->leErCount * sizeof(CsrUint16)) + (primitive->leIrCount * sizeof(CsrUint16)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_RES */

#ifndef EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_RES
CsrUint8 *CsrBtCmGetSecurityConfResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmGetSecurityConfRes *primitive;

    primitive = (CsrBtCmGetSecurityConfRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->options);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->securityMode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->securityLevelDefault);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->config);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->writeAuthEnable);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode3enc);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->leErCount);
    CsrMemCpySer(buffer, length, &primitive->leEr, sizeof(primitive->leEr));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->leIrCount);
    CsrMemCpySer(buffer, length, &primitive->leIr, sizeof(primitive->leIr));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->leSmDivState);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->leSmSignCounter);
    if(primitive->leEr)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->leEr), ((CsrSize) (primitive->leErCount * sizeof(CsrUint16))));
    }
    if(primitive->leIr)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->leIr), ((CsrSize) (primitive->leIrCount * sizeof(CsrUint16))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_RES */

#ifndef EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_RES
void *CsrBtCmGetSecurityConfResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmGetSecurityConfRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmGetSecurityConfRes *) CsrPmemZalloc(sizeof(CsrBtCmGetSecurityConfRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->options, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->securityMode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->securityLevelDefault, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->config, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->writeAuthEnable, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode3enc, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->leErCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->leEr), buffer, &offset, ((CsrSize) sizeof(primitive->leEr)));
    CsrUint8Des((CsrUint8 *) &primitive->leIrCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->leIr), buffer, &offset, ((CsrSize) sizeof(primitive->leIr)));
    CsrUint16Des((CsrUint16 *) &primitive->leSmDivState, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->leSmSignCounter, buffer, &offset);
    if (primitive->leErCount)
    {
        primitive->leEr = CsrPmemZalloc(((CsrUint32) (primitive->leErCount * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->leEr), buffer, &offset, ((CsrSize) (primitive->leErCount * sizeof(CsrUint16))));
    }
    else
    {
        primitive->leEr = NULL;
    }
    if (primitive->leIrCount)
    {
        primitive->leIr = CsrPmemZalloc(((CsrUint32) (primitive->leIrCount * sizeof(CsrUint16))));
        CsrMemCpyDes(((void *) primitive->leIr), buffer, &offset, ((CsrSize) (primitive->leIrCount * sizeof(CsrUint16))));
    }
    else
    {
        primitive->leIr = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_RES */

#ifndef EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_RES
void CsrBtCmGetSecurityConfResSerFree(void *msg)
{
    CsrBtCmGetSecurityConfRes *primitive;

    primitive = (CsrBtCmGetSecurityConfRes *) msg;

    if(primitive->leEr != NULL)
    {
        CsrPmemFree(primitive->leEr);
    }
    if(primitive->leIr != NULL)
    {
        CsrPmemFree(primitive->leIr);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_GET_SECURITY_CONF_RES */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_REQ
CsrSize CsrBtCmBnepSwitchRoleReqSizeof(void *msg)
{
    CsrBtCmBnepSwitchRoleReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_REQ
CsrUint8 *CsrBtCmBnepSwitchRoleReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBnepSwitchRoleReq *primitive;

    primitive = (CsrBtCmBnepSwitchRoleReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->role);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->id);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_REQ
void *CsrBtCmBnepSwitchRoleReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepSwitchRoleReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepSwitchRoleReq *) CsrPmemZalloc(sizeof(CsrBtCmBnepSwitchRoleReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->role, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_SWITCH_ROLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_REPORT_IND
CsrSize CsrBtCmLeReportIndSizeof(void *msg)
{
    CsrBtCmLeReportInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_REPORT_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_REPORT_IND
CsrUint8 *CsrBtCmLeReportIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeReportInd *primitive;

    primitive = (CsrBtCmLeReportInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
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
#endif /* EXCLUDE_CSR_BT_CM_LE_REPORT_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_REPORT_IND
void *CsrBtCmLeReportIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeReportInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeReportInd *) CsrPmemZalloc(sizeof(CsrBtCmLeReportInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_CM_LE_REPORT_IND */

#ifndef EXCLUDE_CSR_BT_CM_CONNECTABLE_REQ
CsrSize CsrBtCmConnectableReqSizeof(void *msg)
{
    CsrBtCmConnectableReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECTABLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONNECTABLE_REQ
CsrUint8 *CsrBtCmConnectableReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmConnectableReq *primitive;

    primitive = (CsrBtCmConnectableReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectAble);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECTABLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CONNECTABLE_REQ
void *CsrBtCmConnectableReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmConnectableReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmConnectableReq *) CsrPmemZalloc(sizeof(CsrBtCmConnectableReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectAble, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CONNECTABLE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_NEG_RES
CsrSize CsrBtCmSmIoCapabilityRequestNegResSizeof(void *msg)
{
    CsrBtCmSmIoCapabilityRequestNegRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_NEG_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_NEG_RES
CsrUint8 *CsrBtCmSmIoCapabilityRequestNegResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmIoCapabilityRequestNegRes *primitive;

    primitive = (CsrBtCmSmIoCapabilityRequestNegRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reason);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_NEG_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_NEG_RES
void *CsrBtCmSmIoCapabilityRequestNegResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmIoCapabilityRequestNegRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmIoCapabilityRequestNegRes *) CsrPmemZalloc(sizeof(CsrBtCmSmIoCapabilityRequestNegRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reason, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_IO_CAPABILITY_REQUEST_NEG_RES */

#ifndef EXCLUDE_CSR_BT_CM_DM_HOUSE_CLEANING
CsrSize CsrBtCmDmHouseCleaningSizeof(void *msg)
{
    CsrBtCmDmHouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DM_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_CM_DM_HOUSE_CLEANING
CsrUint8 *CsrBtCmDmHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDmHouseCleaning *primitive;

    primitive = (CsrBtCmDmHouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_CM_DM_HOUSE_CLEANING
void *CsrBtCmDmHouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDmHouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDmHouseCleaning *) CsrPmemZalloc(sizeof(CsrBtCmDmHouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_REQ
CsrSize CsrBtCmAlwaysMasterDevicesReqSizeof(void *msg)
{
    CsrBtCmAlwaysMasterDevicesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_REQ
CsrUint8 *CsrBtCmAlwaysMasterDevicesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmAlwaysMasterDevicesReq *primitive;

    primitive = (CsrBtCmAlwaysMasterDevicesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->operation);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_REQ
void *CsrBtCmAlwaysMasterDevicesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmAlwaysMasterDevicesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmAlwaysMasterDevicesReq *) CsrPmemZalloc(sizeof(CsrBtCmAlwaysMasterDevicesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->operation, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_ALWAYS_MASTER_DEVICES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_REQ
CsrSize CsrBtCmMoveChannelReqSizeof(void *msg)
{
    CsrBtCmMoveChannelReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_REQ
CsrUint8 *CsrBtCmMoveChannelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmMoveChannelReq *primitive;

    primitive = (CsrBtCmMoveChannelReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remoteControl);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localControl);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_REQ
void *CsrBtCmMoveChannelReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmMoveChannelReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmMoveChannelReq *) CsrPmemZalloc(sizeof(CsrBtCmMoveChannelReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remoteControl, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localControl, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_REQ */

#ifndef EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_CFM
CsrSize CsrBtCmClearEventFilterCfmSizeof(void *msg)
{
    CsrBtCmClearEventFilterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_CFM
CsrUint8 *CsrBtCmClearEventFilterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmClearEventFilterCfm *primitive;

    primitive = (CsrBtCmClearEventFilterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_CFM
void *CsrBtCmClearEventFilterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmClearEventFilterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmClearEventFilterCfm *) CsrPmemZalloc(sizeof(CsrBtCmClearEventFilterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_CLEAR_EVENT_FILTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_REQ
CsrSize CsrBtCmPortnegReqSizeof(void *msg)
{
    CsrBtCmPortnegReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_REQ */

#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_REQ
CsrUint8 *CsrBtCmPortnegReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmPortnegReq *primitive;

    primitive = (CsrBtCmPortnegReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.baud_rate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.data_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.stop_bits);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.parity_type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.flow_ctrl_mask);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xon);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->portPar.xoff);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->portPar.parameter_mask);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_REQ */

#ifndef EXCLUDE_CSR_BT_CM_PORTNEG_REQ
void *CsrBtCmPortnegReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmPortnegReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmPortnegReq *) CsrPmemZalloc(sizeof(CsrBtCmPortnegReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.baud_rate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.data_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.stop_bits, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.parity_type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.flow_ctrl_mask, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xon, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->portPar.xoff, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->portPar.parameter_mask, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_PORTNEG_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_IND
CsrSize CsrBtCmLeAcceptConnparamUpdateIndSizeof(void *msg)
{
    CsrBtCmLeAcceptConnparamUpdateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_IND
CsrUint8 *CsrBtCmLeAcceptConnparamUpdateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeAcceptConnparamUpdateInd *primitive;

    primitive = (CsrBtCmLeAcceptConnparamUpdateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->l2caSignalId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connIntervalMin);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connIntervalMax);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connLatency);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->supervisionTimeout);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_IND */

#ifndef EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_IND
void *CsrBtCmLeAcceptConnparamUpdateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeAcceptConnparamUpdateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeAcceptConnparamUpdateInd *) CsrPmemZalloc(sizeof(CsrBtCmLeAcceptConnparamUpdateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->l2caSignalId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connIntervalMin, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connIntervalMax, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connLatency, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->supervisionTimeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_ACCEPT_CONNPARAM_UPDATE_IND */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_REQ
CsrSize CsrBtCmWritePagescanTypeReqSizeof(void *msg)
{
    CsrBtCmWritePagescanTypeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_REQ
CsrUint8 *CsrBtCmWritePagescanTypeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWritePagescanTypeReq *primitive;

    primitive = (CsrBtCmWritePagescanTypeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scanType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_REQ
void *CsrBtCmWritePagescanTypeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWritePagescanTypeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWritePagescanTypeReq *) CsrPmemZalloc(sizeof(CsrBtCmWritePagescanTypeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scanType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_PAGESCAN_TYPE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM
CsrSize CsrBtCmSdcReleaseResourcesCfmSizeof(void *msg)
{
    CsrBtCmSdcReleaseResourcesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM
CsrUint8 *CsrBtCmSdcReleaseResourcesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcReleaseResourcesCfm *primitive;

    primitive = (CsrBtCmSdcReleaseResourcesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM
void *CsrBtCmSdcReleaseResourcesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcReleaseResourcesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcReleaseResourcesCfm *) CsrPmemZalloc(sizeof(CsrBtCmSdcReleaseResourcesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_RELEASE_RESOURCES_CFM */

#ifndef EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_CFM
CsrSize CsrBtCmEnableDutModeCfmSizeof(void *msg)
{
    CsrBtCmEnableDutModeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_CFM
CsrUint8 *CsrBtCmEnableDutModeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmEnableDutModeCfm *primitive;

    primitive = (CsrBtCmEnableDutModeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->stepNumber);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_CFM
void *CsrBtCmEnableDutModeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmEnableDutModeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmEnableDutModeCfm *) CsrPmemZalloc(sizeof(CsrBtCmEnableDutModeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->stepNumber, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_ENABLE_DUT_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_ACL_DETACH_CFM
CsrSize CsrBtCmAclDetachCfmSizeof(void *msg)
{
    CsrBtCmAclDetachCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_ACL_DETACH_CFM */

#ifndef EXCLUDE_CSR_BT_CM_ACL_DETACH_CFM
CsrUint8 *CsrBtCmAclDetachCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmAclDetachCfm *primitive;

    primitive = (CsrBtCmAclDetachCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_ACL_DETACH_CFM */

#ifndef EXCLUDE_CSR_BT_CM_ACL_DETACH_CFM
void *CsrBtCmAclDetachCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmAclDetachCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmAclDetachCfm *) CsrPmemZalloc(sizeof(CsrBtCmAclDetachCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_ACL_DETACH_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ
CsrSize CsrBtCmWriteLinkSupervTimeoutReqSizeof(void *msg)
{
    CsrBtCmWriteLinkSupervTimeoutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ
CsrUint8 *CsrBtCmWriteLinkSupervTimeoutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteLinkSupervTimeoutReq *primitive;

    primitive = (CsrBtCmWriteLinkSupervTimeoutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeout);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ
void *CsrBtCmWriteLinkSupervTimeoutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteLinkSupervTimeoutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteLinkSupervTimeoutReq *) CsrPmemZalloc(sizeof(CsrBtCmWriteLinkSupervTimeoutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeout, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_SUPERV_TIMEOUT_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DATA_REQ
CsrSize CsrBtCmDataReqSizeof(void *msg)
{
    CsrBtCmDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->payloadLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DATA_REQ
CsrUint8 *CsrBtCmDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDataReq *primitive;

    primitive = (CsrBtCmDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->payloadLength);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->payloadLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DATA_REQ
void *CsrBtCmDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDataReq *) CsrPmemZalloc(sizeof(CsrBtCmDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
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
#endif /* EXCLUDE_CSR_BT_CM_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DATA_REQ
void CsrBtCmDataReqSerFree(void *msg)
{
    CsrBtCmDataReq *primitive;

    primitive = (CsrBtCmDataReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_CFM
CsrSize CsrBtCmDisableDutModeCfmSizeof(void *msg)
{
    CsrBtCmDisableDutModeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_CFM
CsrUint8 *CsrBtCmDisableDutModeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDisableDutModeCfm *primitive;

    primitive = (CsrBtCmDisableDutModeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_CFM
void *CsrBtCmDisableDutModeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDisableDutModeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDisableDutModeCfm *) CsrPmemZalloc(sizeof(CsrBtCmDisableDutModeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DISABLE_DUT_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_REQ
CsrSize CsrBtCmReadAfhChannelMapReqSizeof(void *msg)
{
    CsrBtCmReadAfhChannelMapReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_REQ
CsrUint8 *CsrBtCmReadAfhChannelMapReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadAfhChannelMapReq *primitive;

    primitive = (CsrBtCmReadAfhChannelMapReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_REQ
void *CsrBtCmReadAfhChannelMapReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadAfhChannelMapReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadAfhChannelMapReq *) CsrPmemZalloc(sizeof(CsrBtCmReadAfhChannelMapReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_AFH_CHANNEL_MAP_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_IAC_CFM
CsrSize CsrBtCmReadIacCfmSizeof(void *msg)
{
    CsrBtCmReadIacCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_IAC_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_IAC_CFM
CsrUint8 *CsrBtCmReadIacCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadIacCfm *primitive;

    primitive = (CsrBtCmReadIacCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->iac);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_IAC_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_IAC_CFM
void *CsrBtCmReadIacCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadIacCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadIacCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadIacCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->iac, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_IAC_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DISCONNECT_IND
CsrSize CsrBtCmDisconnectIndSizeof(void *msg)
{
    CsrBtCmDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_DISCONNECT_IND
CsrUint8 *CsrBtCmDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDisconnectInd *primitive;

    primitive = (CsrBtCmDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localTerminated);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_DISCONNECT_IND
void *CsrBtCmDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtCmDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localTerminated, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_SYNC_RENEGOTIATE_IND
CsrSize CsrBtCmSyncRenegotiateIndSizeof(void *msg)
{
    CsrBtCmSyncRenegotiateInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SYNC_RENEGOTIATE_IND */

#ifndef EXCLUDE_CSR_BT_CM_SYNC_RENEGOTIATE_IND
CsrUint8 *CsrBtCmSyncRenegotiateIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSyncRenegotiateInd *primitive;

    primitive = (CsrBtCmSyncRenegotiateInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->incoming);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->syncHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->linkType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->txInterval);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->weSco);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rxPacketLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->txPacketLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->airMode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->packetType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->txBdw);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->rxBdw);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxLatency);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reTxEffort);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->voiceSettings);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SYNC_RENEGOTIATE_IND */

#ifndef EXCLUDE_CSR_BT_CM_SYNC_RENEGOTIATE_IND
void *CsrBtCmSyncRenegotiateIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSyncRenegotiateInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSyncRenegotiateInd *) CsrPmemZalloc(sizeof(CsrBtCmSyncRenegotiateInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->incoming, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->syncHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->linkType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->txInterval, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->weSco, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rxPacketLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->txPacketLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->airMode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->packetType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->txBdw, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->rxBdw, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxLatency, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reTxEffort, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->voiceSettings, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SYNC_RENEGOTIATE_IND */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_CFM
CsrSize CsrBtCmSdcServiceSearchCfmSizeof(void *msg)
{
    CsrBtCmSdcServiceSearchCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->recListLength * sizeof(CsrBtUuid32)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_CFM
CsrUint8 *CsrBtCmSdcServiceSearchCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcServiceSearchCfm *primitive;

    primitive = (CsrBtCmSdcServiceSearchCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->recListLength);
    CsrMemCpySer(buffer, length, &primitive->recList, sizeof(primitive->recList));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    if(primitive->recList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->recList), ((CsrSize) (primitive->recListLength * sizeof(CsrBtUuid32))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_CFM
void *CsrBtCmSdcServiceSearchCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcServiceSearchCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcServiceSearchCfm *) CsrPmemZalloc(sizeof(CsrBtCmSdcServiceSearchCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->recListLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->recList), buffer, &offset, ((CsrSize) sizeof(primitive->recList)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    if (primitive->recListLength)
    {
        primitive->recList = CsrPmemZalloc(((CsrUint32) (primitive->recListLength * sizeof(CsrBtUuid32))));
        CsrMemCpyDes(((void *) primitive->recList), buffer, &offset, ((CsrSize) (primitive->recListLength * sizeof(CsrBtUuid32))));
    }
    else
    {
        primitive->recList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_CFM
void CsrBtCmSdcServiceSearchCfmSerFree(void *msg)
{
    CsrBtCmSdcServiceSearchCfm *primitive;

    primitive = (CsrBtCmSdcServiceSearchCfm *) msg;

    if(primitive->recList != NULL)
    {
        CsrPmemFree(primitive->recList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_SERVICE_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_CM_DM_MODE_SETTINGS_REQ
CsrSize CsrBtCmDmModeSettingsReqSizeof(void *msg)
{
    CsrBtCmDmModeSettingsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->sniffSettingsSize * sizeof(CsrBtSniffSettings)) + (primitive->parkSettingsSize * sizeof(CsrBtParkSettings)) + (primitive->ssrSettingsSize * sizeof(CsrBtSsrSettingsDownstream)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_DM_MODE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_MODE_SETTINGS_REQ
CsrUint8 *CsrBtCmDmModeSettingsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmDmModeSettingsReq *primitive;

    primitive = (CsrBtCmDmModeSettingsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrMemCpySer(buffer, length, &primitive->sniffSettings, sizeof(primitive->sniffSettings));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sniffSettingsSize);
    CsrMemCpySer(buffer, length, &primitive->parkSettings, sizeof(primitive->parkSettings));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->parkSettingsSize);
    CsrMemCpySer(buffer, length, &primitive->ssrSettings, sizeof(primitive->ssrSettings));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ssrSettingsSize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->lowPowerPriority);
    if(primitive->sniffSettings)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->sniffSettings), ((CsrSize) (primitive->sniffSettingsSize * sizeof(CsrBtSniffSettings))));
    }
    if(primitive->parkSettings)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->parkSettings), ((CsrSize) (primitive->parkSettingsSize * sizeof(CsrBtParkSettings))));
    }
    if(primitive->ssrSettings)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->ssrSettings), ((CsrSize) (primitive->ssrSettingsSize * sizeof(CsrBtSsrSettingsDownstream))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_MODE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_MODE_SETTINGS_REQ
void *CsrBtCmDmModeSettingsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmDmModeSettingsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmDmModeSettingsReq *) CsrPmemZalloc(sizeof(CsrBtCmDmModeSettingsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->sniffSettings), buffer, &offset, ((CsrSize) sizeof(primitive->sniffSettings)));
    CsrUint8Des((CsrUint8 *) &primitive->sniffSettingsSize, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->parkSettings), buffer, &offset, ((CsrSize) sizeof(primitive->parkSettings)));
    CsrUint8Des((CsrUint8 *) &primitive->parkSettingsSize, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ssrSettings), buffer, &offset, ((CsrSize) sizeof(primitive->ssrSettings)));
    CsrUint8Des((CsrUint8 *) &primitive->ssrSettingsSize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->lowPowerPriority, buffer, &offset);
    if (primitive->sniffSettingsSize)
    {
        primitive->sniffSettings = CsrPmemZalloc(((CsrUint32) (primitive->sniffSettingsSize * sizeof(CsrBtSniffSettings))));
        CsrMemCpyDes(((void *) primitive->sniffSettings), buffer, &offset, ((CsrSize) (primitive->sniffSettingsSize * sizeof(CsrBtSniffSettings))));
    }
    else
    {
        primitive->sniffSettings = NULL;
    }
    if (primitive->parkSettingsSize)
    {
        primitive->parkSettings = CsrPmemZalloc(((CsrUint32) (primitive->parkSettingsSize * sizeof(CsrBtParkSettings))));
        CsrMemCpyDes(((void *) primitive->parkSettings), buffer, &offset, ((CsrSize) (primitive->parkSettingsSize * sizeof(CsrBtParkSettings))));
    }
    else
    {
        primitive->parkSettings = NULL;
    }
    if (primitive->ssrSettingsSize)
    {
        primitive->ssrSettings = CsrPmemZalloc(((CsrUint32) (primitive->ssrSettingsSize * sizeof(CsrBtSsrSettingsDownstream))));
        CsrMemCpyDes(((void *) primitive->ssrSettings), buffer, &offset, ((CsrSize) (primitive->ssrSettingsSize * sizeof(CsrBtSsrSettingsDownstream))));
    }
    else
    {
        primitive->ssrSettings = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_DM_MODE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_DM_MODE_SETTINGS_REQ
void CsrBtCmDmModeSettingsReqSerFree(void *msg)
{
    CsrBtCmDmModeSettingsReq *primitive;

    primitive = (CsrBtCmDmModeSettingsReq *) msg;

    if(primitive->sniffSettings != NULL)
    {
        CsrPmemFree(primitive->sniffSettings);
    }
    if(primitive->parkSettings != NULL)
    {
        CsrPmemFree(primitive->parkSettings);
    }
    if(primitive->ssrSettings != NULL)
    {
        CsrPmemFree(primitive->ssrSettings);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_DM_MODE_SETTINGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_SCAN_CFM
CsrSize CsrBtCmLeScanCfmSizeof(void *msg)
{
    CsrBtCmLeScanCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_SCAN_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_SCAN_CFM
CsrUint8 *CsrBtCmLeScanCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeScanCfm *primitive;

    primitive = (CsrBtCmLeScanCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_SCAN_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_SCAN_CFM
void *CsrBtCmLeScanCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeScanCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeScanCfm *) CsrPmemZalloc(sizeof(CsrBtCmLeScanCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_SCAN_CFM */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_IND
CsrSize CsrBtCmBnepExtendedDataIndSizeof(void *msg)
{
    CsrBtCmBnepExtendedDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_IND
CsrUint8 *CsrBtCmBnepExtendedDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBnepExtendedDataInd *primitive;

    primitive = (CsrBtCmBnepExtendedDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->etherType);
    CsrMemCpySer(buffer, length, ((const void *) primitive->dstAddr.w), ((CsrSize) 2 * (3)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->srcAddr.w), ((CsrSize) 2 * (3)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_IND
void *CsrBtCmBnepExtendedDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepExtendedDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepExtendedDataInd *) CsrPmemZalloc(sizeof(CsrBtCmBnepExtendedDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->etherType, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->dstAddr.w), buffer, &offset, ((CsrSize) 2 * (3)));
    CsrMemCpyDes(((void *) primitive->srcAddr.w), buffer, &offset, ((CsrSize) 2 * (3)));
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->length)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_IND
void CsrBtCmBnepExtendedDataIndSerFree(void *msg)
{
    CsrBtCmBnepExtendedDataInd *primitive;

    primitive = (CsrBtCmBnepExtendedDataInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_EXTENDED_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_REGISTER_CFM
CsrSize CsrBtCmL2caRegisterCfmSizeof(void *msg)
{
    CsrBtCmL2caRegisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_REGISTER_CFM
CsrUint8 *CsrBtCmL2caRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caRegisterCfm *primitive;

    primitive = (CsrBtCmL2caRegisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mode_mask);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_REGISTER_CFM
void *CsrBtCmL2caRegisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caRegisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caRegisterCfm *) CsrPmemZalloc(sizeof(CsrBtCmL2caRegisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mode_mask, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_IND
CsrSize CsrBtCmL2caModeChangeIndSizeof(void *msg)
{
    CsrBtCmL2caModeChangeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_IND
CsrUint8 *CsrBtCmL2caModeChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmL2caModeChangeInd *primitive;

    primitive = (CsrBtCmL2caModeChangeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->context);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_IND
void *CsrBtCmL2caModeChangeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmL2caModeChangeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmL2caModeChangeInd *) CsrPmemZalloc(sizeof(CsrBtCmL2caModeChangeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->context, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_L2CA_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_SM_REGISTER_REQ
CsrSize CsrBtCmSmRegisterReqSizeof(void *msg)
{
    CsrBtCmSmRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_REGISTER_REQ
CsrUint8 *CsrBtCmSmRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmRegisterReq *primitive;

    primitive = (CsrBtCmSmRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->protocolId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->outgoingOk);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->securityLevel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_REGISTER_REQ
void *CsrBtCmSmRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmRegisterReq *) CsrPmemZalloc(sizeof(CsrBtCmSmRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->protocolId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->outgoingOk, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->securityLevel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LE_TEST_END_CFM
CsrSize CsrBtCmLeTestEndCfmSizeof(void *msg)
{
    CsrBtCmLeTestEndCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_TEST_END_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_TEST_END_CFM
CsrUint8 *CsrBtCmLeTestEndCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeTestEndCfm *primitive;

    primitive = (CsrBtCmLeTestEndCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->numberOfPackets);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_TEST_END_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_TEST_END_CFM
void *CsrBtCmLeTestEndCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeTestEndCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeTestEndCfm *) CsrPmemZalloc(sizeof(CsrBtCmLeTestEndCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->numberOfPackets, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_TEST_END_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_REQ
CsrSize CsrBtCmReadRemoteExtFeaturesReqSizeof(void *msg)
{
    CsrBtCmReadRemoteExtFeaturesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_REQ
CsrUint8 *CsrBtCmReadRemoteExtFeaturesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadRemoteExtFeaturesReq *primitive;

    primitive = (CsrBtCmReadRemoteExtFeaturesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pageNum);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_REQ
void *CsrBtCmReadRemoteExtFeaturesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadRemoteExtFeaturesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadRemoteExtFeaturesReq *) CsrPmemZalloc(sizeof(CsrBtCmReadRemoteExtFeaturesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pageNum, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_REQ */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_CFM
CsrSize CsrBtCmReadRemoteExtFeaturesCfmSizeof(void *msg)
{
    CsrBtCmReadRemoteExtFeaturesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_CFM
CsrUint8 *CsrBtCmReadRemoteExtFeaturesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadRemoteExtFeaturesCfm *primitive;

    primitive = (CsrBtCmReadRemoteExtFeaturesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pageNum);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->maxPageNum);
    CsrMemCpySer(buffer, length, ((const void *) primitive->extLmpFeatures), ((CsrSize) 2 * (4)));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_CFM
void *CsrBtCmReadRemoteExtFeaturesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadRemoteExtFeaturesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadRemoteExtFeaturesCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadRemoteExtFeaturesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pageNum, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->maxPageNum, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->extLmpFeatures), buffer, &offset, ((CsrSize) 2 * (4)));
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_EXT_FEATURES_CFM */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_IND
CsrSize CsrBtCmBnepModeChangeIndSizeof(void *msg)
{
    CsrBtCmBnepModeChangeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_IND
CsrUint8 *CsrBtCmBnepModeChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBnepModeChangeInd *primitive;

    primitive = (CsrBtCmBnepModeChangeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_IND
void *CsrBtCmBnepModeChangeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepModeChangeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepModeChangeInd *) CsrPmemZalloc(sizeof(CsrBtCmBnepModeChangeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_MODE_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_SM_AUTHORISE_RES
CsrSize CsrBtCmSmAuthoriseResSizeof(void *msg)
{
    CsrBtCmSmAuthoriseRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_AUTHORISE_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_AUTHORISE_RES
CsrUint8 *CsrBtCmSmAuthoriseResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmAuthoriseRes *primitive;

    primitive = (CsrBtCmSmAuthoriseRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->protocolId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->incoming);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->authorisation);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_AUTHORISE_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_AUTHORISE_RES
void *CsrBtCmSmAuthoriseResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmAuthoriseRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmAuthoriseRes *) CsrPmemZalloc(sizeof(CsrBtCmSmAuthoriseRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->protocolId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->incoming, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->authorisation, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_AUTHORISE_RES */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_IAC_IND
CsrSize CsrBtCmWriteIacIndSizeof(void *msg)
{
    CsrBtCmWriteIacInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_IAC_IND */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_IAC_IND
CsrUint8 *CsrBtCmWriteIacIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteIacInd *primitive;

    primitive = (CsrBtCmWriteIacInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->iac);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_IAC_IND */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_IAC_IND
void *CsrBtCmWriteIacIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteIacInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteIacInd *) CsrPmemZalloc(sizeof(CsrBtCmWriteIacInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->iac, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_IAC_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM
CsrSize CsrBtCmBnepCancelConnectAcceptCfmSizeof(void *msg)
{
    CsrBtCmBnepCancelConnectAcceptCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM
CsrUint8 *CsrBtCmBnepCancelConnectAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBnepCancelConnectAcceptCfm *primitive;

    primitive = (CsrBtCmBnepCancelConnectAcceptCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM
void *CsrBtCmBnepCancelConnectAcceptCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepCancelConnectAcceptCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepCancelConnectAcceptCfm *) CsrPmemZalloc(sizeof(CsrBtCmBnepCancelConnectAcceptCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CANCEL_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_CFM
CsrSize CsrBtCmLeTransmitterTestCfmSizeof(void *msg)
{
    CsrBtCmLeTransmitterTestCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_CFM
CsrUint8 *CsrBtCmLeTransmitterTestCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLeTransmitterTestCfm *primitive;

    primitive = (CsrBtCmLeTransmitterTestCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_CFM */

#ifndef EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_CFM
void *CsrBtCmLeTransmitterTestCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLeTransmitterTestCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLeTransmitterTestCfm *) CsrPmemZalloc(sizeof(CsrBtCmLeTransmitterTestCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LE_TRANSMITTER_TEST_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_CFM
CsrSize CsrBtCmReadRemoteNameCfmSizeof(void *msg)
{
    CsrBtCmReadRemoteNameCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->friendlyName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_CFM
CsrUint8 *CsrBtCmReadRemoteNameCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadRemoteNameCfm *primitive;

    primitive = (CsrBtCmReadRemoteNameCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->friendlyName, sizeof(primitive->friendlyName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUtf8StringSer(buffer, length, primitive->friendlyName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_CFM
void *CsrBtCmReadRemoteNameCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadRemoteNameCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadRemoteNameCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadRemoteNameCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->friendlyName), buffer, &offset, ((CsrSize) sizeof(primitive->friendlyName)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUtf8StringDes(&primitive->friendlyName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_CFM
void CsrBtCmReadRemoteNameCfmSerFree(void *msg)
{
    CsrBtCmReadRemoteNameCfm *primitive;

    primitive = (CsrBtCmReadRemoteNameCfm *) msg;

    if(primitive->friendlyName != NULL)
    {
        CsrPmemFree(primitive->friendlyName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_READ_REMOTE_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_ERROR_IND
CsrSize CsrBtCmWriteLinkPolicyErrorIndSizeof(void *msg)
{
    CsrBtCmWriteLinkPolicyErrorInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_ERROR_IND */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_ERROR_IND
CsrUint8 *CsrBtCmWriteLinkPolicyErrorIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteLinkPolicyErrorInd *primitive;

    primitive = (CsrBtCmWriteLinkPolicyErrorInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_ERROR_IND */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_ERROR_IND
void *CsrBtCmWriteLinkPolicyErrorIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteLinkPolicyErrorInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteLinkPolicyErrorInd *) CsrPmemZalloc(sizeof(CsrBtCmWriteLinkPolicyErrorInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_LINK_POLICY_ERROR_IND */

#ifndef EXCLUDE_CSR_BT_CM_SM_KEY_REQUEST_RES
CsrSize CsrBtCmSmKeyRequestResSizeof(void *msg)
{
    CsrBtCmSmKeyRequestRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->keySize * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_KEY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_KEY_REQUEST_RES
CsrUint8 *CsrBtCmSmKeyRequestResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmKeyRequestRes *primitive;

    primitive = (CsrBtCmSmKeyRequestRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->securityRequirements);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->keyType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->keySize);
    CsrMemCpySer(buffer, length, &primitive->key, sizeof(primitive->key));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    if(primitive->key)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->key), ((CsrSize) (primitive->keySize * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_KEY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_KEY_REQUEST_RES
void *CsrBtCmSmKeyRequestResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmKeyRequestRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmKeyRequestRes *) CsrPmemZalloc(sizeof(CsrBtCmSmKeyRequestRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->securityRequirements, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->keyType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->keySize, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->key), buffer, &offset, ((CsrSize) sizeof(primitive->key)));
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    if (primitive->keySize)
    {
        primitive->key = CsrPmemZalloc(((CsrUint32) (primitive->keySize * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->key), buffer, &offset, ((CsrSize) (primitive->keySize * sizeof(CsrUint8))));
    }
    else
    {
        primitive->key = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_KEY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_KEY_REQUEST_RES
void CsrBtCmSmKeyRequestResSerFree(void *msg)
{
    CsrBtCmSmKeyRequestRes *primitive;

    primitive = (CsrBtCmSmKeyRequestRes *) msg;

    if(primitive->key != NULL)
    {
        CsrPmemFree(primitive->key);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_CM_SM_KEY_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_CFM
CsrSize CsrBtCmReadEncryptionStatusCfmSizeof(void *msg)
{
    CsrBtCmReadEncryptionStatusCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_CFM
CsrUint8 *CsrBtCmReadEncryptionStatusCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmReadEncryptionStatusCfm *primitive;

    primitive = (CsrBtCmReadEncryptionStatusCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->encrypted);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_CFM
void *CsrBtCmReadEncryptionStatusCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmReadEncryptionStatusCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmReadEncryptionStatusCfm *) CsrPmemZalloc(sizeof(CsrBtCmReadEncryptionStatusCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->encrypted, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_READ_ENCRYPTION_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SM_ACL_CLOSE_REQ
CsrSize CsrBtCmSmAclCloseReqSizeof(void *msg)
{
    CsrBtCmSmAclCloseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_ACL_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_ACL_CLOSE_REQ
CsrUint8 *CsrBtCmSmAclCloseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmAclCloseReq *primitive;

    primitive = (CsrBtCmSmAclCloseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_ACL_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_ACL_CLOSE_REQ
void *CsrBtCmSmAclCloseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmAclCloseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmAclCloseReq *) CsrPmemZalloc(sizeof(CsrBtCmSmAclCloseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_ACL_CLOSE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_ACCESS_REQ
CsrSize CsrBtCmSmAccessReqSizeof(void *msg)
{
    CsrBtCmSmAccessReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_ACCESS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_ACCESS_REQ
CsrUint8 *CsrBtCmSmAccessReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmAccessReq *primitive;

    primitive = (CsrBtCmSmAccessReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_ACCESS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_ACCESS_REQ
void *CsrBtCmSmAccessReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmAccessReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmAccessReq *) CsrPmemZalloc(sizeof(CsrBtCmSmAccessReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_ACCESS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_REQ
CsrSize CsrBtCmA2dpBitRateReqSizeof(void *msg)
{
    CsrBtCmA2dpBitRateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_REQ
CsrUint8 *CsrBtCmA2dpBitRateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmA2dpBitRateReq *primitive;

    primitive = (CsrBtCmA2dpBitRateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->streamIdx);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bitRate);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_REQ
void *CsrBtCmA2dpBitRateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmA2dpBitRateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmA2dpBitRateReq *) CsrPmemZalloc(sizeof(CsrBtCmA2dpBitRateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->streamIdx, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bitRate, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_A2DP_BIT_RATE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_CONNECT_ACCEPT_CFM
CsrSize CsrBtCmBnepConnectAcceptCfmSizeof(void *msg)
{
    CsrBtCmBnepConnectAcceptCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_CONNECT_ACCEPT_CFM
CsrUint8 *CsrBtCmBnepConnectAcceptCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBnepConnectAcceptCfm *primitive;

    primitive = (CsrBtCmBnepConnectAcceptCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_CONNECT_ACCEPT_CFM
void *CsrBtCmBnepConnectAcceptCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepConnectAcceptCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepConnectAcceptCfm *) CsrPmemZalloc(sizeof(CsrBtCmBnepConnectAcceptCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_CONNECT_ACCEPT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_CFM
CsrSize CsrBtCmSetLocalNameCfmSizeof(void *msg)
{
    CsrBtCmSetLocalNameCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_CFM
CsrUint8 *CsrBtCmSetLocalNameCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSetLocalNameCfm *primitive;

    primitive = (CsrBtCmSetLocalNameCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_CFM
void *CsrBtCmSetLocalNameCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSetLocalNameCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSetLocalNameCfm *) CsrPmemZalloc(sizeof(CsrBtCmSetLocalNameCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_LOCAL_NAME_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SDC_CLOSE_IND
CsrSize CsrBtCmSdcCloseIndSizeof(void *msg)
{
    CsrBtCmSdcCloseInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_CLOSE_IND */

#ifndef EXCLUDE_CSR_BT_CM_SDC_CLOSE_IND
CsrUint8 *CsrBtCmSdcCloseIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSdcCloseInd *primitive;

    primitive = (CsrBtCmSdcCloseInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localServerChannel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_CLOSE_IND */

#ifndef EXCLUDE_CSR_BT_CM_SDC_CLOSE_IND
void *CsrBtCmSdcCloseIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSdcCloseInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSdcCloseInd *) CsrPmemZalloc(sizeof(CsrBtCmSdcCloseInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localServerChannel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SDC_CLOSE_IND */

#ifndef EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_RES
CsrSize CsrBtCmSmUserConfirmationRequestResSizeof(void *msg)
{
    CsrBtCmSmUserConfirmationRequestRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_RES
CsrUint8 *CsrBtCmSmUserConfirmationRequestResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmUserConfirmationRequestRes *primitive;

    primitive = (CsrBtCmSmUserConfirmationRequestRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reason);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_RES
void *CsrBtCmSmUserConfirmationRequestResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmUserConfirmationRequestRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmUserConfirmationRequestRes *) CsrPmemZalloc(sizeof(CsrBtCmSmUserConfirmationRequestRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reason, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_USER_CONFIRMATION_REQUEST_RES */

#ifndef EXCLUDE_CSR_BT_CM_REJECT_RFC_CONNECTION_IND
CsrSize CsrBtCmRejectRfcConnectionIndSizeof(void *msg)
{
    CsrBtCmRejectRfcConnectionInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_REJECT_RFC_CONNECTION_IND */

#ifndef EXCLUDE_CSR_BT_CM_REJECT_RFC_CONNECTION_IND
CsrUint8 *CsrBtCmRejectRfcConnectionIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmRejectRfcConnectionInd *primitive;

    primitive = (CsrBtCmRejectRfcConnectionInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_REJECT_RFC_CONNECTION_IND */

#ifndef EXCLUDE_CSR_BT_CM_REJECT_RFC_CONNECTION_IND
void *CsrBtCmRejectRfcConnectionIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmRejectRfcConnectionInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmRejectRfcConnectionInd *) CsrPmemZalloc(sizeof(CsrBtCmRejectRfcConnectionInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_REJECT_RFC_CONNECTION_IND */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_IAC_REQ
CsrSize CsrBtCmWriteIacReqSizeof(void *msg)
{
    CsrBtCmWriteIacReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_IAC_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_IAC_REQ
CsrUint8 *CsrBtCmWriteIacReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmWriteIacReq *primitive;

    primitive = (CsrBtCmWriteIacReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->iac);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_IAC_REQ */

#ifndef EXCLUDE_CSR_BT_CM_WRITE_IAC_REQ
void *CsrBtCmWriteIacReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmWriteIacReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmWriteIacReq *) CsrPmemZalloc(sizeof(CsrBtCmWriteIacReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->iac, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_WRITE_IAC_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BLE_CONNECTION_IND
CsrSize CsrBtCmBleConnectionIndSizeof(void *msg)
{
    CsrBtCmBleConnectionInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BLE_CONNECTION_IND */

#ifndef EXCLUDE_CSR_BT_CM_BLE_CONNECTION_IND
CsrUint8 *CsrBtCmBleConnectionIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBleConnectionInd *primitive;

    primitive = (CsrBtCmBleConnectionInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bleUsed);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bleTiming);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BLE_CONNECTION_IND */

#ifndef EXCLUDE_CSR_BT_CM_BLE_CONNECTION_IND
void *CsrBtCmBleConnectionIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBleConnectionInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBleConnectionInd *) CsrPmemZalloc(sizeof(CsrBtCmBleConnectionInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bleUsed, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bleTiming, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BLE_CONNECTION_IND */

#ifndef EXCLUDE_CSR_BT_CM_EXT_SYNC_CONNECT_IND
CsrSize CsrBtCmExtSyncConnectIndSizeof(void *msg)
{
    CsrBtCmExtSyncConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_EXT_SYNC_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_EXT_SYNC_CONNECT_IND
CsrUint8 *CsrBtCmExtSyncConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmExtSyncConnectInd *primitive;

    primitive = (CsrBtCmExtSyncConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->incoming);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->syncHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->linkType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->txInterval);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->weSco);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reservedSlots);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rxPacketLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->txPacketLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->airMode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->packetType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->txBdw);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->rxBdw);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->maxLatency);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->reTxEffort);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->voiceSettings);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_EXT_SYNC_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_EXT_SYNC_CONNECT_IND
void *CsrBtCmExtSyncConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmExtSyncConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmExtSyncConnectInd *) CsrPmemZalloc(sizeof(CsrBtCmExtSyncConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->incoming, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->syncHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->linkType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->txInterval, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->weSco, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reservedSlots, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rxPacketLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->txPacketLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->airMode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->packetType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->txBdw, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->rxBdw, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->maxLatency, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->reTxEffort, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->voiceSettings, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_EXT_SYNC_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_REGISTER_REQ
CsrSize CsrBtCmBnepRegisterReqSizeof(void *msg)
{
    CsrBtCmBnepRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_REGISTER_REQ
CsrUint8 *CsrBtCmBnepRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmBnepRegisterReq *primitive;

    primitive = (CsrBtCmBnepRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->disableExtended);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->manualBridge);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->disableStack);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_BNEP_REGISTER_REQ
void *CsrBtCmBnepRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmBnepRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmBnepRegisterReq *) CsrPmemZalloc(sizeof(CsrBtCmBnepRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->disableExtended, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->manualBridge, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->disableStack, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_BNEP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_CFM
CsrSize CsrBtCmSetEventFilterBdaddrCfmSizeof(void *msg)
{
    CsrBtCmSetEventFilterBdaddrCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_CFM
CsrUint8 *CsrBtCmSetEventFilterBdaddrCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSetEventFilterBdaddrCfm *primitive;

    primitive = (CsrBtCmSetEventFilterBdaddrCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_CFM
void *CsrBtCmSetEventFilterBdaddrCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSetEventFilterBdaddrCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSetEventFilterBdaddrCfm *) CsrPmemZalloc(sizeof(CsrBtCmSetEventFilterBdaddrCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SET_EVENT_FILTER_BDADDR_CFM */

#ifndef EXCLUDE_CSR_BT_CM_EIR_FLAGS_REQ
CsrSize CsrBtCmEirFlagsReqSizeof(void *msg)
{
    CsrBtCmEirFlagsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_EIR_FLAGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_EIR_FLAGS_REQ
CsrUint8 *CsrBtCmEirFlagsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmEirFlagsReq *primitive;

    primitive = (CsrBtCmEirFlagsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->eirFlags);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_EIR_FLAGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_EIR_FLAGS_REQ
void *CsrBtCmEirFlagsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmEirFlagsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmEirFlagsReq *) CsrPmemZalloc(sizeof(CsrBtCmEirFlagsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->eirFlags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_EIR_FLAGS_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_REMOVE_DEVICE_REQ
CsrSize CsrBtCmSmRemoveDeviceReqSizeof(void *msg)
{
    CsrBtCmSmRemoveDeviceReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SM_REMOVE_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_REMOVE_DEVICE_REQ
CsrUint8 *CsrBtCmSmRemoveDeviceReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmSmRemoveDeviceReq *primitive;

    primitive = (CsrBtCmSmRemoveDeviceReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_REMOVE_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_SM_REMOVE_DEVICE_REQ
void *CsrBtCmSmRemoveDeviceReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmSmRemoveDeviceReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmSmRemoveDeviceReq *) CsrPmemZalloc(sizeof(CsrBtCmSmRemoveDeviceReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SM_REMOVE_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_CM_LSTO_CHANGE_IND
CsrSize CsrBtCmLstoChangeIndSizeof(void *msg)
{
    CsrBtCmLstoChangeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_LSTO_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_LSTO_CHANGE_IND
CsrUint8 *CsrBtCmLstoChangeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmLstoChangeInd *primitive;

    primitive = (CsrBtCmLstoChangeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->lsto);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_LSTO_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_LSTO_CHANGE_IND
void *CsrBtCmLstoChangeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmLstoChangeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmLstoChangeInd *) CsrPmemZalloc(sizeof(CsrBtCmLstoChangeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->lsto, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_LSTO_CHANGE_IND */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_IND
CsrSize CsrBtCmMoveChannelIndSizeof(void *msg)
{
    CsrBtCmMoveChannelInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_IND
CsrUint8 *CsrBtCmMoveChannelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmMoveChannelInd *primitive;

    primitive = (CsrBtCmMoveChannelInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localControl);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_IND
void *CsrBtCmMoveChannelIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmMoveChannelInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmMoveChannelInd *) CsrPmemZalloc(sizeof(CsrBtCmMoveChannelInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localControl, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_MOVE_CHANNEL_IND */

#ifndef EXCLUDE_CSR_BT_CM_SCO_CONNECT_CFM
CsrSize CsrBtCmScoConnectCfmSizeof(void *msg)
{
    CsrBtCmScoConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SCO_CONNECT_CFM
CsrUint8 *CsrBtCmScoConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmScoConnectCfm *primitive;

    primitive = (CsrBtCmScoConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->linkType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->txInterval);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->weSco);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rxPacketLength);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->txPacketLength);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->airMode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->eScoHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmSlot);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_SCO_CONNECT_CFM
void *CsrBtCmScoConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmScoConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmScoConnectCfm *) CsrPmemZalloc(sizeof(CsrBtCmScoConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->linkType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->txInterval, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->weSco, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rxPacketLength, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->txPacketLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->airMode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->eScoHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmSlot, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_SCO_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_CM_REMOTE_FEATURES_IND
CsrSize CsrBtCmRemoteFeaturesIndSizeof(void *msg)
{
    CsrBtCmRemoteFeaturesInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_REMOTE_FEATURES_IND */

#ifndef EXCLUDE_CSR_BT_CM_REMOTE_FEATURES_IND
CsrUint8 *CsrBtCmRemoteFeaturesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmRemoteFeaturesInd *primitive;

    primitive = (CsrBtCmRemoteFeaturesInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, ((const void *) primitive->remoteLmpFeatures), ((CsrSize) (8)));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_REMOTE_FEATURES_IND */

#ifndef EXCLUDE_CSR_BT_CM_REMOTE_FEATURES_IND
void *CsrBtCmRemoteFeaturesIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmRemoteFeaturesInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmRemoteFeaturesInd *) CsrPmemZalloc(sizeof(CsrBtCmRemoteFeaturesInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->remoteLmpFeatures), buffer, &offset, ((CsrSize) (8)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_REMOTE_FEATURES_IND */

#ifndef EXCLUDE_CSR_BT_CM_HIGH_PRIORITY_DATA_IND
CsrSize CsrBtCmHighPriorityDataIndSizeof(void *msg)
{
    CsrBtCmHighPriorityDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_CM_HIGH_PRIORITY_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_HIGH_PRIORITY_DATA_IND
CsrUint8 *CsrBtCmHighPriorityDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtCmHighPriorityDataInd *primitive;

    primitive = (CsrBtCmHighPriorityDataInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->start);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_CM_HIGH_PRIORITY_DATA_IND */

#ifndef EXCLUDE_CSR_BT_CM_HIGH_PRIORITY_DATA_IND
void *CsrBtCmHighPriorityDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtCmHighPriorityDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtCmHighPriorityDataInd *) CsrPmemZalloc(sizeof(CsrBtCmHighPriorityDataInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->start, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_CM_HIGH_PRIORITY_DATA_IND */

#endif /* EXCLUDE_CSR_BT_CM_MODULE */
