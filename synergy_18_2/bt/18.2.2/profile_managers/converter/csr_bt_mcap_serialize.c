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
#include "csr_bt_mcap_private_prim.h"

#ifndef EXCLUDE_CSR_BT_MCAP_PRIVATE_MODULE

void CsrBtMcapPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_RES
CsrSize CsrBtMcapSyncSetResSizeof(void *msg)
{
    CsrBtMcapSyncSetRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_RES
CsrUint8 *CsrBtMcapSyncSetResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapSyncSetRes *primitive;

    primitive = (CsrBtMcapSyncSetRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mcapResponse);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btClkSyncTime);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->timeStampSyncTimeLsb);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->timeStampSyncTimeMsb);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeStampSampleAccuracy);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_RES
void *CsrBtMcapSyncSetResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapSyncSetRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapSyncSetRes *) CsrPmemZalloc(sizeof(CsrBtMcapSyncSetRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mcapResponse, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btClkSyncTime, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->timeStampSyncTimeLsb, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->timeStampSyncTimeMsb, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeStampSampleAccuracy, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_CFM
CsrSize CsrBtMcapDisconnectMdlCfmSizeof(void *msg)
{
    CsrBtMcapDisconnectMdlCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_CFM
CsrUint8 *CsrBtMcapDisconnectMdlCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapDisconnectMdlCfm *primitive;

    primitive = (CsrBtMcapDisconnectMdlCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_CFM
void *CsrBtMcapDisconnectMdlCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapDisconnectMdlCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapDisconnectMdlCfm *) CsrPmemZalloc(sizeof(CsrBtMcapDisconnectMdlCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_CFM
CsrSize CsrBtMcapConnectMclCfmSizeof(void *msg)
{
    CsrBtMcapConnectMclCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_CFM
CsrUint8 *CsrBtMcapConnectMclCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapConnectMclCfm *primitive;

    primitive = (CsrBtMcapConnectMclCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_CFM
void *CsrBtMcapConnectMclCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapConnectMclCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapConnectMclCfm *) CsrPmemZalloc(sizeof(CsrBtMcapConnectMclCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_CFM
CsrSize CsrBtMcapReconnectMdlCfmSizeof(void *msg)
{
    CsrBtMcapReconnectMdlCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_CFM
CsrUint8 *CsrBtMcapReconnectMdlCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapReconnectMdlCfm *primitive;

    primitive = (CsrBtMcapReconnectMdlCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_CFM
void *CsrBtMcapReconnectMdlCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapReconnectMdlCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapReconnectMdlCfm *) CsrPmemZalloc(sizeof(CsrBtMcapReconnectMdlCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_INFO_IND
CsrSize CsrBtMcapSyncInfoIndSizeof(void *msg)
{
    CsrBtMcapSyncInfoInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_INFO_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_INFO_IND
CsrUint8 *CsrBtMcapSyncInfoIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapSyncInfoInd *primitive;

    primitive = (CsrBtMcapSyncInfoInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btClkSyncTime);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->timeStampSyncTimeLsb);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->timeStampSyncTimeMsb);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeStampSampleAccuracy);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_INFO_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_INFO_IND
void *CsrBtMcapSyncInfoIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapSyncInfoInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapSyncInfoInd *) CsrPmemZalloc(sizeof(CsrBtMcapSyncInfoInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btClkSyncTime, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->timeStampSyncTimeLsb, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->timeStampSyncTimeMsb, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeStampSampleAccuracy, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_INFO_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_REQ
CsrSize CsrBtMcapConnectMdlReqSizeof(void *msg)
{
    CsrBtMcapConnectMdlReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->qosCount * sizeof(L2CA_QOS_T)) + (primitive->flowCount * sizeof(L2CA_FLOW_T)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_REQ
CsrUint8 *CsrBtMcapConnectMdlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapConnectMdlReq *primitive;

    primitive = (CsrBtMcapConnectMdlReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->configuration);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remotePsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mtu);
    CsrMemCpySer(buffer, length, &primitive->qos, sizeof(primitive->qos));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->qosCount);
    CsrMemCpySer(buffer, length, &primitive->flow, sizeof(primitive->flow));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flowCount);
    if(primitive->qos)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->qos), ((CsrSize) (primitive->qosCount * sizeof(L2CA_QOS_T))));
    }
    if(primitive->flow)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->flow), ((CsrSize) (primitive->flowCount * sizeof(L2CA_FLOW_T))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_REQ
void *CsrBtMcapConnectMdlReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapConnectMdlReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapConnectMdlReq *) CsrPmemZalloc(sizeof(CsrBtMcapConnectMdlReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->configuration, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remotePsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mtu, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->qos), buffer, &offset, ((CsrSize) sizeof(primitive->qos)));
    CsrUint8Des((CsrUint8 *) &primitive->qosCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->flow), buffer, &offset, ((CsrSize) sizeof(primitive->flow)));
    CsrUint8Des((CsrUint8 *) &primitive->flowCount, buffer, &offset);
    if (primitive->qosCount)
    {
        primitive->qos = CsrPmemZalloc(((CsrUint32) (primitive->qosCount * sizeof(L2CA_QOS_T))));
        CsrMemCpyDes(((void *) primitive->qos), buffer, &offset, ((CsrSize) (primitive->qosCount * sizeof(L2CA_QOS_T))));
    }
    else
    {
        primitive->qos = NULL;
    }
    if (primitive->flowCount)
    {
        primitive->flow = CsrPmemZalloc(((CsrUint32) (primitive->flowCount * sizeof(L2CA_FLOW_T))));
        CsrMemCpyDes(((void *) primitive->flow), buffer, &offset, ((CsrSize) (primitive->flowCount * sizeof(L2CA_FLOW_T))));
    }
    else
    {
        primitive->flow = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_REQ
void CsrBtMcapConnectMdlReqSerFree(void *msg)
{
    CsrBtMcapConnectMdlReq *primitive;

    primitive = (CsrBtMcapConnectMdlReq *) msg;

    if(primitive->qos != NULL)
    {
        CsrPmemFree(primitive->qos);
    }
    if(primitive->flow != NULL)
    {
        CsrPmemFree(primitive->flow);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_CFM
CsrSize CsrBtMcapDeleteMdlCfmSizeof(void *msg)
{
    CsrBtMcapDeleteMdlCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_CFM
CsrUint8 *CsrBtMcapDeleteMdlCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapDeleteMdlCfm *primitive;

    primitive = (CsrBtMcapDeleteMdlCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mcapResponse);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_CFM
void *CsrBtMcapDeleteMdlCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapDeleteMdlCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapDeleteMdlCfm *) CsrPmemZalloc(sizeof(CsrBtMcapDeleteMdlCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mcapResponse, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_CREATE_MDL_IND
CsrSize CsrBtMcapCreateMdlIndSizeof(void *msg)
{
    CsrBtMcapCreateMdlInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_CREATE_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_CREATE_MDL_IND
CsrUint8 *CsrBtMcapCreateMdlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapCreateMdlInd *primitive;

    primitive = (CsrBtMcapCreateMdlInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mdepId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->configuration);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_CREATE_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_CREATE_MDL_IND
void *CsrBtMcapCreateMdlIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapCreateMdlInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapCreateMdlInd *) CsrPmemZalloc(sizeof(CsrBtMcapCreateMdlInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mdepId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->configuration, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_CREATE_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_IND
CsrSize CsrBtMcapReconnectMdlIndSizeof(void *msg)
{
    CsrBtMcapReconnectMdlInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_IND
CsrUint8 *CsrBtMcapReconnectMdlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapReconnectMdlInd *primitive;

    primitive = (CsrBtMcapReconnectMdlInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remotePsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localMtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remoteMtu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_IND
void *CsrBtMcapReconnectMdlIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapReconnectMdlInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapReconnectMdlInd *) CsrPmemZalloc(sizeof(CsrBtMcapReconnectMdlInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remotePsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localMtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remoteMtu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_CFM
CsrSize CsrBtMcapSyncSetCfmSizeof(void *msg)
{
    CsrBtMcapSyncSetCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_CFM
CsrUint8 *CsrBtMcapSyncSetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapSyncSetCfm *primitive;

    primitive = (CsrBtMcapSyncSetCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mcapResponse);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btClkSyncTime);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->timeStampSyncTimeLsb);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->timeStampSyncTimeMsb);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeStampSampleAccuracy);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_CFM
void *CsrBtMcapSyncSetCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapSyncSetCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapSyncSetCfm *) CsrPmemZalloc(sizeof(CsrBtMcapSyncSetCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mcapResponse, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btClkSyncTime, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->timeStampSyncTimeLsb, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->timeStampSyncTimeMsb, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeStampSampleAccuracy, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_CREATE_MDL_RES
CsrSize CsrBtMcapCreateMdlResSizeof(void *msg)
{
    CsrBtMcapCreateMdlRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->qosCount * sizeof(L2CA_QOS_T)) + (primitive->flowCount * sizeof(L2CA_FLOW_T)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_CREATE_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_CREATE_MDL_RES
CsrUint8 *CsrBtMcapCreateMdlResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapCreateMdlRes *primitive;

    primitive = (CsrBtMcapCreateMdlRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mcapResponse);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->configResponse);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mtu);
    CsrMemCpySer(buffer, length, &primitive->qos, sizeof(primitive->qos));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->qosCount);
    CsrMemCpySer(buffer, length, &primitive->flow, sizeof(primitive->flow));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flowCount);
    if(primitive->qos)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->qos), ((CsrSize) (primitive->qosCount * sizeof(L2CA_QOS_T))));
    }
    if(primitive->flow)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->flow), ((CsrSize) (primitive->flowCount * sizeof(L2CA_FLOW_T))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_CREATE_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_CREATE_MDL_RES
void *CsrBtMcapCreateMdlResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapCreateMdlRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapCreateMdlRes *) CsrPmemZalloc(sizeof(CsrBtMcapCreateMdlRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mcapResponse, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->configResponse, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mtu, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->qos), buffer, &offset, ((CsrSize) sizeof(primitive->qos)));
    CsrUint8Des((CsrUint8 *) &primitive->qosCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->flow), buffer, &offset, ((CsrSize) sizeof(primitive->flow)));
    CsrUint8Des((CsrUint8 *) &primitive->flowCount, buffer, &offset);
    if (primitive->qosCount)
    {
        primitive->qos = CsrPmemZalloc(((CsrUint32) (primitive->qosCount * sizeof(L2CA_QOS_T))));
        CsrMemCpyDes(((void *) primitive->qos), buffer, &offset, ((CsrSize) (primitive->qosCount * sizeof(L2CA_QOS_T))));
    }
    else
    {
        primitive->qos = NULL;
    }
    if (primitive->flowCount)
    {
        primitive->flow = CsrPmemZalloc(((CsrUint32) (primitive->flowCount * sizeof(L2CA_FLOW_T))));
        CsrMemCpyDes(((void *) primitive->flow), buffer, &offset, ((CsrSize) (primitive->flowCount * sizeof(L2CA_FLOW_T))));
    }
    else
    {
        primitive->flow = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_CREATE_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_CREATE_MDL_RES
void CsrBtMcapCreateMdlResSerFree(void *msg)
{
    CsrBtMcapCreateMdlRes *primitive;

    primitive = (CsrBtMcapCreateMdlRes *) msg;

    if(primitive->qos != NULL)
    {
        CsrPmemFree(primitive->qos);
    }
    if(primitive->flow != NULL)
    {
        CsrPmemFree(primitive->flow);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MCAP_CREATE_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_CFM
CsrSize CsrBtMcapConnectMdlCfmSizeof(void *msg)
{
    CsrBtMcapConnectMdlCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_CFM
CsrUint8 *CsrBtMcapConnectMdlCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapConnectMdlCfm *primitive;

    primitive = (CsrBtMcapConnectMdlCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remotePsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localMtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remoteMtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_CFM
void *CsrBtMcapConnectMdlCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapConnectMdlCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapConnectMdlCfm *) CsrPmemZalloc(sizeof(CsrBtMcapConnectMdlCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remotePsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localMtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remoteMtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_CFM
CsrSize CsrBtMcapDisconnectMclCfmSizeof(void *msg)
{
    CsrBtMcapDisconnectMclCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_CFM
CsrUint8 *CsrBtMcapDisconnectMclCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapDisconnectMclCfm *primitive;

    primitive = (CsrBtMcapDisconnectMclCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_CFM
void *CsrBtMcapDisconnectMclCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapDisconnectMclCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapDisconnectMclCfm *) CsrPmemZalloc(sizeof(CsrBtMcapDisconnectMclCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_DEACTIVATE_CFM
CsrSize CsrBtMcapDeactivateCfmSizeof(void *msg)
{
    CsrBtMcapDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_DEACTIVATE_CFM
CsrUint8 *CsrBtMcapDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapDeactivateCfm *primitive;

    primitive = (CsrBtMcapDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_DEACTIVATE_CFM
void *CsrBtMcapDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtMcapDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_ABORT_MDL_RES
CsrSize CsrBtMcapAbortMdlResSizeof(void *msg)
{
    CsrBtMcapAbortMdlRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_ABORT_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_ABORT_MDL_RES
CsrUint8 *CsrBtMcapAbortMdlResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapAbortMdlRes *primitive;

    primitive = (CsrBtMcapAbortMdlRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mcapResponse);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_ABORT_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_ABORT_MDL_RES
void *CsrBtMcapAbortMdlResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapAbortMdlRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapAbortMdlRes *) CsrPmemZalloc(sizeof(CsrBtMcapAbortMdlRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mcapResponse, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_ABORT_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_RES
CsrSize CsrBtMcapReconnectMdlResSizeof(void *msg)
{
    CsrBtMcapReconnectMdlRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->qosCount * sizeof(L2CA_QOS_T)) + (primitive->flowCount * sizeof(L2CA_FLOW_T)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_RES
CsrUint8 *CsrBtMcapReconnectMdlResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapReconnectMdlRes *primitive;

    primitive = (CsrBtMcapReconnectMdlRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mcapResponse);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mtu);
    CsrMemCpySer(buffer, length, &primitive->qos, sizeof(primitive->qos));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->qosCount);
    CsrMemCpySer(buffer, length, &primitive->flow, sizeof(primitive->flow));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->flowCount);
    if(primitive->qos)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->qos), ((CsrSize) (primitive->qosCount * sizeof(L2CA_QOS_T))));
    }
    if(primitive->flow)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->flow), ((CsrSize) (primitive->flowCount * sizeof(L2CA_FLOW_T))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_RES
void *CsrBtMcapReconnectMdlResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapReconnectMdlRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapReconnectMdlRes *) CsrPmemZalloc(sizeof(CsrBtMcapReconnectMdlRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mcapResponse, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mtu, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->qos), buffer, &offset, ((CsrSize) sizeof(primitive->qos)));
    CsrUint8Des((CsrUint8 *) &primitive->qosCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->flow), buffer, &offset, ((CsrSize) sizeof(primitive->flow)));
    CsrUint8Des((CsrUint8 *) &primitive->flowCount, buffer, &offset);
    if (primitive->qosCount)
    {
        primitive->qos = CsrPmemZalloc(((CsrUint32) (primitive->qosCount * sizeof(L2CA_QOS_T))));
        CsrMemCpyDes(((void *) primitive->qos), buffer, &offset, ((CsrSize) (primitive->qosCount * sizeof(L2CA_QOS_T))));
    }
    else
    {
        primitive->qos = NULL;
    }
    if (primitive->flowCount)
    {
        primitive->flow = CsrPmemZalloc(((CsrUint32) (primitive->flowCount * sizeof(L2CA_FLOW_T))));
        CsrMemCpyDes(((void *) primitive->flow), buffer, &offset, ((CsrSize) (primitive->flowCount * sizeof(L2CA_FLOW_T))));
    }
    else
    {
        primitive->flow = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_RES
void CsrBtMcapReconnectMdlResSerFree(void *msg)
{
    CsrBtMcapReconnectMdlRes *primitive;

    primitive = (CsrBtMcapReconnectMdlRes *) msg;

    if(primitive->qos != NULL)
    {
        CsrPmemFree(primitive->qos);
    }
    if(primitive->flow != NULL)
    {
        CsrPmemFree(primitive->flow);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_DEACTIVATE_REQ
CsrSize CsrBtMcapDeactivateReqSizeof(void *msg)
{
    CsrBtMcapDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DEACTIVATE_REQ
CsrUint8 *CsrBtMcapDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapDeactivateReq *primitive;

    primitive = (CsrBtMcapDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DEACTIVATE_REQ
void *CsrBtMcapDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtMcapDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_RES
CsrSize CsrBtMcapSyncCapResSizeof(void *msg)
{
    CsrBtMcapSyncCapRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_RES
CsrUint8 *CsrBtMcapSyncCapResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapSyncCapRes *primitive;

    primitive = (CsrBtMcapSyncCapRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mcapResponse);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->btClkAccessRes);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->syncLeadTime);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeStampNativeRes);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeStampNativeAccuracy);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_RES
void *CsrBtMcapSyncCapResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapSyncCapRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapSyncCapRes *) CsrPmemZalloc(sizeof(CsrBtMcapSyncCapRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mcapResponse, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->btClkAccessRes, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->syncLeadTime, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeStampNativeRes, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeStampNativeAccuracy, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_REQ
CsrSize CsrBtMcapDisconnectMclReqSizeof(void *msg)
{
    CsrBtMcapDisconnectMclReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_REQ
CsrUint8 *CsrBtMcapDisconnectMclReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapDisconnectMclReq *primitive;

    primitive = (CsrBtMcapDisconnectMclReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_REQ
void *CsrBtMcapDisconnectMclReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapDisconnectMclReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapDisconnectMclReq *) CsrPmemZalloc(sizeof(CsrBtMcapDisconnectMclReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_IND
CsrSize CsrBtMcapDisconnectMdlIndSizeof(void *msg)
{
    CsrBtMcapDisconnectMdlInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_IND
CsrUint8 *CsrBtMcapDisconnectMdlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapDisconnectMdlInd *primitive;

    primitive = (CsrBtMcapDisconnectMdlInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_IND
void *CsrBtMcapDisconnectMdlIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapDisconnectMdlInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapDisconnectMdlInd *) CsrPmemZalloc(sizeof(CsrBtMcapDisconnectMdlInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_REQ
CsrSize CsrBtMcapSyncSetReqSizeof(void *msg)
{
    CsrBtMcapSyncSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_REQ
CsrUint8 *CsrBtMcapSyncSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapSyncSetReq *primitive;

    primitive = (CsrBtMcapSyncSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->timeStampUpdateInfo);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btClkSyncTime);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->timeStampSyncTimeLsb);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->timeStampSyncTimeMsb);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_REQ
void *CsrBtMcapSyncSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapSyncSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapSyncSetReq *) CsrPmemZalloc(sizeof(CsrBtMcapSyncSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->timeStampUpdateInfo, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btClkSyncTime, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->timeStampSyncTimeLsb, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->timeStampSyncTimeMsb, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_REQ
CsrSize CsrBtMcapReconnectMdlReqSizeof(void *msg)
{
    CsrBtMcapReconnectMdlReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_REQ
CsrUint8 *CsrBtMcapReconnectMdlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapReconnectMdlReq *primitive;

    primitive = (CsrBtMcapReconnectMdlReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remotePsm);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_REQ
void *CsrBtMcapReconnectMdlReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapReconnectMdlReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapReconnectMdlReq *) CsrPmemZalloc(sizeof(CsrBtMcapReconnectMdlReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remotePsm, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_RECONNECT_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_IND
CsrSize CsrBtMcapConnectMclIndSizeof(void *msg)
{
    CsrBtMcapConnectMclInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_IND
CsrUint8 *CsrBtMcapConnectMclIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapConnectMclInd *primitive;

    primitive = (CsrBtMcapConnectMclInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remotePsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_IND
void *CsrBtMcapConnectMclIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapConnectMclInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapConnectMclInd *) CsrPmemZalloc(sizeof(CsrBtMcapConnectMclInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remotePsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_CFM
CsrSize CsrBtMcapSyncCapCfmSizeof(void *msg)
{
    CsrBtMcapSyncCapCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_CFM
CsrUint8 *CsrBtMcapSyncCapCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapSyncCapCfm *primitive;

    primitive = (CsrBtMcapSyncCapCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mcapResponse);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->btClkAccessRes);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->syncLeadTime);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeStampNativeRes);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeStampNativeAccuracy);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_CFM
void *CsrBtMcapSyncCapCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapSyncCapCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapSyncCapCfm *) CsrPmemZalloc(sizeof(CsrBtMcapSyncCapCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mcapResponse, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->btClkAccessRes, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->syncLeadTime, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeStampNativeRes, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeStampNativeAccuracy, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_CFM
CsrSize CsrBtMcapDataSendCfmSizeof(void *msg)
{
    CsrBtMcapDataSendCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_CFM
CsrUint8 *CsrBtMcapDataSendCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapDataSendCfm *primitive;

    primitive = (CsrBtMcapDataSendCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_CFM
void *CsrBtMcapDataSendCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapDataSendCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapDataSendCfm *) CsrPmemZalloc(sizeof(CsrBtMcapDataSendCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_IND
CsrSize CsrBtMcapSyncSetIndSizeof(void *msg)
{
    CsrBtMcapSyncSetInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_IND
CsrUint8 *CsrBtMcapSyncSetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapSyncSetInd *primitive;

    primitive = (CsrBtMcapSyncSetInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->timeStampUpdateInfo);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btClkSyncTime);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->timeStampSyncTimeLsb);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->timeStampSyncTimeMsb);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_SET_IND
void *CsrBtMcapSyncSetIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapSyncSetInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapSyncSetInd *) CsrPmemZalloc(sizeof(CsrBtMcapSyncSetInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->timeStampUpdateInfo, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btClkSyncTime, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->timeStampSyncTimeLsb, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->timeStampSyncTimeMsb, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_SET_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_ACTIVATE_REQ
CsrSize CsrBtMcapActivateReqSizeof(void *msg)
{
    CsrBtMcapActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_ACTIVATE_REQ
CsrUint8 *CsrBtMcapActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapActivateReq *primitive;

    primitive = (CsrBtMcapActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->classOfDevice);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mtu);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sniffTimeout);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uuid);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->noOfConnections);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_ACTIVATE_REQ
void *CsrBtMcapActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapActivateReq *) CsrPmemZalloc(sizeof(CsrBtMcapActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->classOfDevice, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mtu, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sniffTimeout, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uuid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->noOfConnections, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_IND
CsrSize CsrBtMcapConnectMdlIndSizeof(void *msg)
{
    CsrBtMcapConnectMdlInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_IND
CsrUint8 *CsrBtMcapConnectMdlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapConnectMdlInd *primitive;

    primitive = (CsrBtMcapConnectMdlInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remotePsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localMtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remoteMtu);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_IND
void *CsrBtMcapConnectMdlIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapConnectMdlInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapConnectMdlInd *) CsrPmemZalloc(sizeof(CsrBtMcapConnectMdlInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remotePsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localMtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remoteMtu, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_REQ
CsrSize CsrBtMcapDisconnectMdlReqSizeof(void *msg)
{
    CsrBtMcapDisconnectMdlReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_REQ
CsrUint8 *CsrBtMcapDisconnectMdlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapDisconnectMdlReq *primitive;

    primitive = (CsrBtMcapDisconnectMdlReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_REQ
void *CsrBtMcapDisconnectMdlReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapDisconnectMdlReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapDisconnectMdlReq *) CsrPmemZalloc(sizeof(CsrBtMcapDisconnectMdlReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_REQ
CsrSize CsrBtMcapConnectMclReqSizeof(void *msg)
{
    CsrBtMcapConnectMclReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_REQ
CsrUint8 *CsrBtMcapConnectMclReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapConnectMclReq *primitive;

    primitive = (CsrBtMcapConnectMclReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localPsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remotePsm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->sniffTimeout);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_REQ
void *CsrBtMcapConnectMclReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapConnectMclReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapConnectMclReq *) CsrPmemZalloc(sizeof(CsrBtMcapConnectMclReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localPsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remotePsm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->sniffTimeout, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_CONNECT_MCL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_IND
CsrSize CsrBtMcapDeleteMdlIndSizeof(void *msg)
{
    CsrBtMcapDeleteMdlInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_IND
CsrUint8 *CsrBtMcapDeleteMdlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapDeleteMdlInd *primitive;

    primitive = (CsrBtMcapDeleteMdlInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_IND
void *CsrBtMcapDeleteMdlIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapDeleteMdlInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapDeleteMdlInd *) CsrPmemZalloc(sizeof(CsrBtMcapDeleteMdlInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_REQ
CsrSize CsrBtMcapDataSendReqSizeof(void *msg)
{
    CsrBtMcapDataSendReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + primitive->length);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_REQ
CsrUint8 *CsrBtMcapDataSendReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapDataSendReq *primitive;

    primitive = (CsrBtMcapDataSendReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->length)));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_REQ
void *CsrBtMcapDataSendReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapDataSendReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapDataSendReq *) CsrPmemZalloc(sizeof(CsrBtMcapDataSendReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->length)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->length)));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->length)));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_REQ
void CsrBtMcapDataSendReqSerFree(void *msg)
{
    CsrBtMcapDataSendReq *primitive;

    primitive = (CsrBtMcapDataSendReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_INFO_REQ
CsrSize CsrBtMcapSyncInfoReqSizeof(void *msg)
{
    CsrBtMcapSyncInfoReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_INFO_REQ
CsrUint8 *CsrBtMcapSyncInfoReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapSyncInfoReq *primitive;

    primitive = (CsrBtMcapSyncInfoReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btClkSyncTime);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->timeStampSyncTimeLsb);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->timeStampSyncTimeMsb);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeStampSampleAccuracy);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_INFO_REQ
void *CsrBtMcapSyncInfoReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapSyncInfoReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapSyncInfoReq *) CsrPmemZalloc(sizeof(CsrBtMcapSyncInfoReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btClkSyncTime, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->timeStampSyncTimeLsb, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->timeStampSyncTimeMsb, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeStampSampleAccuracy, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_INFO_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_REQ
CsrSize CsrBtMcapDeleteMdlReqSizeof(void *msg)
{
    CsrBtMcapDeleteMdlReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_REQ
CsrUint8 *CsrBtMcapDeleteMdlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapDeleteMdlReq *primitive;

    primitive = (CsrBtMcapDeleteMdlReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_REQ
void *CsrBtMcapDeleteMdlReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapDeleteMdlReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapDeleteMdlReq *) CsrPmemZalloc(sizeof(CsrBtMcapDeleteMdlReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_REQ
CsrSize CsrBtMcapSyncCapReqSizeof(void *msg)
{
    CsrBtMcapSyncCapReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_REQ
CsrUint8 *CsrBtMcapSyncCapReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapSyncCapReq *primitive;

    primitive = (CsrBtMcapSyncCapReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeStampRequiredAccuracy);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_REQ
void *CsrBtMcapSyncCapReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapSyncCapReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapSyncCapReq *) CsrPmemZalloc(sizeof(CsrBtMcapSyncCapReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeStampRequiredAccuracy, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_REQ */

#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_RES
CsrSize CsrBtMcapDeleteMdlResSizeof(void *msg)
{
    CsrBtMcapDeleteMdlRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_RES
CsrUint8 *CsrBtMcapDeleteMdlResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapDeleteMdlRes *primitive;

    primitive = (CsrBtMcapDeleteMdlRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mcapResponse);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_DELETE_MDL_RES
void *CsrBtMcapDeleteMdlResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapDeleteMdlRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapDeleteMdlRes *) CsrPmemZalloc(sizeof(CsrBtMcapDeleteMdlRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mcapResponse, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DELETE_MDL_RES */

#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_IND
CsrSize CsrBtMcapDataSendIndSizeof(void *msg)
{
    CsrBtMcapDataSendInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + primitive->length);
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_IND
CsrUint8 *CsrBtMcapDataSendIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapDataSendInd *primitive;

    primitive = (CsrBtMcapDataSendInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->payload, sizeof(primitive->payload));
    if(primitive->payload)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->payload), ((CsrSize) (primitive->length)));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_IND
void *CsrBtMcapDataSendIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapDataSendInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapDataSendInd *) CsrPmemZalloc(sizeof(CsrBtMcapDataSendInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->payload), buffer, &offset, ((CsrSize) sizeof(primitive->payload)));
    if (primitive->length)
    {
        primitive->payload = CsrPmemZalloc(((CsrUint32) (primitive->length)));
        CsrMemCpyDes(((void *) primitive->payload), buffer, &offset, ((CsrSize) (primitive->length)));
    }
    else
    {
        primitive->payload = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_DATA_SEND_IND
void CsrBtMcapDataSendIndSerFree(void *msg)
{
    CsrBtMcapDataSendInd *primitive;

    primitive = (CsrBtMcapDataSendInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_MCAP_DATA_SEND_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_IND
CsrSize CsrBtMcapSyncCapIndSizeof(void *msg)
{
    CsrBtMcapSyncCapInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_IND
CsrUint8 *CsrBtMcapSyncCapIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapSyncCapInd *primitive;

    primitive = (CsrBtMcapSyncCapInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->timeStampRequiredAccuracy);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_SYNC_CAP_IND
void *CsrBtMcapSyncCapIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapSyncCapInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapSyncCapInd *) CsrPmemZalloc(sizeof(CsrBtMcapSyncCapInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->timeStampRequiredAccuracy, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_SYNC_CAP_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_ACTIVATE_CFM
CsrSize CsrBtMcapActivateCfmSizeof(void *msg)
{
    CsrBtMcapActivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_ACTIVATE_CFM
CsrUint8 *CsrBtMcapActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapActivateCfm *primitive;

    primitive = (CsrBtMcapActivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->psm);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_ACTIVATE_CFM
void *CsrBtMcapActivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapActivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapActivateCfm *) CsrPmemZalloc(sizeof(CsrBtMcapActivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->psm, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_MCAP_ABORT_MDL_IND
CsrSize CsrBtMcapAbortMdlIndSizeof(void *msg)
{
    CsrBtMcapAbortMdlInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_ABORT_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_ABORT_MDL_IND
CsrUint8 *CsrBtMcapAbortMdlIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapAbortMdlInd *primitive;

    primitive = (CsrBtMcapAbortMdlInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mdlId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_ABORT_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_ABORT_MDL_IND
void *CsrBtMcapAbortMdlIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapAbortMdlInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapAbortMdlInd *) CsrPmemZalloc(sizeof(CsrBtMcapAbortMdlInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mdlId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_ABORT_MDL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_IND
CsrSize CsrBtMcapDisconnectMclIndSizeof(void *msg)
{
    CsrBtMcapDisconnectMclInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_IND
CsrUint8 *CsrBtMcapDisconnectMclIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtMcapDisconnectMclInd *primitive;

    primitive = (CsrBtMcapDisconnectMclInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mclId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_IND */

#ifndef EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_IND
void *CsrBtMcapDisconnectMclIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtMcapDisconnectMclInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtMcapDisconnectMclInd *) CsrPmemZalloc(sizeof(CsrBtMcapDisconnectMclInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mclId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_MCAP_DISCONNECT_MCL_IND */

#endif /* EXCLUDE_CSR_BT_MCAP_PRIVATE_MODULE */
