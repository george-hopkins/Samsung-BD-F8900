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
#include "csr_bt_bsl_prim.h"

#ifndef EXCLUDE_CSR_BT_BSL_MODULE

void CsrBtBslPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_BSL_STATUS_IND
CsrSize CsrBtBslStatusIndSizeof(void *msg)
{
    CsrBtBslStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_BSL_STATUS_IND
CsrUint8 *CsrBtBslStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslStatusInd *primitive;

    primitive = (CsrBtBslStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->id);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->event);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_BSL_STATUS_IND
void *CsrBtBslStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslStatusInd *) CsrPmemZalloc(sizeof(CsrBtBslStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->event, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_CFM
CsrSize CsrBtBslServiceSearchCfmSizeof(void *msg)
{
    CsrBtBslServiceSearchCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_CFM
CsrUint8 *CsrBtBslServiceSearchCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslServiceSearchCfm *primitive;

    primitive = (CsrBtBslServiceSearchCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_CFM
void *CsrBtBslServiceSearchCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslServiceSearchCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslServiceSearchCfm *) CsrPmemZalloc(sizeof(CsrBtBslServiceSearchCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_BSL_REGISTER_REQ
CsrSize CsrBtBslRegisterReqSizeof(void *msg)
{
    CsrBtBslRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_REGISTER_REQ
CsrUint8 *CsrBtBslRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslRegisterReq *primitive;

    primitive = (CsrBtBslRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->connPhandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataPhandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_REGISTER_REQ
void *CsrBtBslRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslRegisterReq *) CsrPmemZalloc(sizeof(CsrBtBslRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->connPhandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataPhandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_CONNECT_REQ
CsrSize CsrBtBslConnectReqSizeof(void *msg)
{
    CsrBtBslConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_CONNECT_REQ
CsrUint8 *CsrBtBslConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslConnectReq *primitive;

    primitive = (CsrBtBslConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bdAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bdAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bdAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localRole);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remoteRole);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_CONNECT_REQ
void *CsrBtBslConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslConnectReq *) CsrPmemZalloc(sizeof(CsrBtBslConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bdAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bdAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bdAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localRole, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remoteRole, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_IN_REQ
CsrSize CsrBtBslSecurityInReqSizeof(void *msg)
{
    CsrBtBslSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_IN_REQ
CsrUint8 *CsrBtBslSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslSecurityInReq *primitive;

    primitive = (CsrBtBslSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_IN_REQ
void *CsrBtBslSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtBslSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_OUT_REQ
CsrSize CsrBtBslSecurityOutReqSizeof(void *msg)
{
    CsrBtBslSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_OUT_REQ
CsrUint8 *CsrBtBslSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslSecurityOutReq *primitive;

    primitive = (CsrBtBslSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_OUT_REQ
void *CsrBtBslSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtBslSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_DISCONNECT_IND
CsrSize CsrBtBslDisconnectIndSizeof(void *msg)
{
    CsrBtBslDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BSL_DISCONNECT_IND
CsrUint8 *CsrBtBslDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslDisconnectInd *primitive;

    primitive = (CsrBtBslDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->id);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BSL_DISCONNECT_IND
void *CsrBtBslDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtBslDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BSL_DISCONNECT_REQ
CsrSize CsrBtBslDisconnectReqSizeof(void *msg)
{
    CsrBtBslDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_DISCONNECT_REQ
CsrUint8 *CsrBtBslDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslDisconnectReq *primitive;

    primitive = (CsrBtBslDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->id);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_DISCONNECT_REQ
void *CsrBtBslDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtBslDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_CONNECT_IND
CsrSize CsrBtBslConnectIndSizeof(void *msg)
{
    CsrBtBslConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BSL_CONNECT_IND
CsrUint8 *CsrBtBslConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslConnectInd *primitive;

    primitive = (CsrBtBslConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->id);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bdAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bdAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bdAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localRole);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remoteRole);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BSL_CONNECT_IND
void *CsrBtBslConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslConnectInd *) CsrPmemZalloc(sizeof(CsrBtBslConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bdAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bdAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bdAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localRole, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remoteRole, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_BSL_ACTIVATE_REQ
CsrSize CsrBtBslActivateReqSizeof(void *msg)
{
    CsrBtBslActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_ACTIVATE_REQ
CsrUint8 *CsrBtBslActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslActivateReq *primitive;

    primitive = (CsrBtBslActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->singleUser);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localRole);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remoteRole);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_ACTIVATE_REQ
void *CsrBtBslActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslActivateReq *) CsrPmemZalloc(sizeof(CsrBtBslActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->singleUser, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localRole, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remoteRole, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_ACTIVATE_CFM
CsrSize CsrBtBslActivateCfmSizeof(void *msg)
{
    CsrBtBslActivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_BSL_ACTIVATE_CFM
CsrUint8 *CsrBtBslActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslActivateCfm *primitive;

    primitive = (CsrBtBslActivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_BSL_ACTIVATE_CFM
void *CsrBtBslActivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslActivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslActivateCfm *) CsrPmemZalloc(sizeof(CsrBtBslActivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_BSL_DATA_REQ
CsrSize CsrBtBslDataReqSizeof(void *msg)
{
    CsrBtBslDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_DATA_REQ
CsrUint8 *CsrBtBslDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslDataReq *primitive;

    primitive = (CsrBtBslDataReq *) msg;
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
#endif /* EXCLUDE_CSR_BT_BSL_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_DATA_REQ
void *CsrBtBslDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslDataReq *) CsrPmemZalloc(sizeof(CsrBtBslDataReq));

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
#endif /* EXCLUDE_CSR_BT_BSL_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_DATA_REQ
void CsrBtBslDataReqSerFree(void *msg)
{
    CsrBtBslDataReq *primitive;

    primitive = (CsrBtBslDataReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BSL_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND
CsrSize CsrBtBslServiceSearchResultIndSizeof(void *msg)
{
    CsrBtBslServiceSearchResultInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->searchResult.serviceName) + CsrCharStringSerLen((CsrCharString *) primitive->searchResult.serviceDescription) + (primitive->searchResult.languageElementsCount * sizeof(CsrBtLanguageElement)) + CsrCharStringSerLen((CsrCharString *) primitive->searchResult.ipv4Subnet) + CsrCharStringSerLen((CsrCharString *) primitive->searchResult.ipv6Subnet));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND
CsrUint8 *CsrBtBslServiceSearchResultIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslServiceSearchResultInd *primitive;

    primitive = (CsrBtBslServiceSearchResultInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->moreResults);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->searchRole);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->searchResult.protocolVersion);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->searchResult.loadFactorIncluded);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->searchResult.loadFactor);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->searchResult.bluetoothProfileVersion);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->searchResult.securityDescription);
    CsrMemCpySer(buffer, length, &primitive->searchResult.serviceName, sizeof(primitive->searchResult.serviceName));
    CsrMemCpySer(buffer, length, &primitive->searchResult.serviceDescription, sizeof(primitive->searchResult.serviceDescription));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->searchResult.languageElementsCount);
    CsrMemCpySer(buffer, length, &primitive->searchResult.languageElements, sizeof(primitive->searchResult.languageElements));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->searchResult.numOfSupportedNetwPacketTypes);
    CsrMemCpySer(buffer, length, ((const void *) primitive->searchResult.supportedNetwPacketType), ((CsrSize) 2 * (50)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->searchResult.netAccessTypeIncluded);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->searchResult.netAccessType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->searchResult.maxNetAccessRateIncluded);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->searchResult.maxNetAccessRate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->searchResult.ipv4SubnetIncluded);
    CsrMemCpySer(buffer, length, &primitive->searchResult.ipv4Subnet, sizeof(primitive->searchResult.ipv4Subnet));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->searchResult.ipv6SubnetIncluded);
    CsrMemCpySer(buffer, length, &primitive->searchResult.ipv6Subnet, sizeof(primitive->searchResult.ipv6Subnet));
    CsrCharStringSer(buffer, length, primitive->searchResult.serviceName);
    CsrCharStringSer(buffer, length, primitive->searchResult.serviceDescription);
    if(primitive->searchResult.languageElements)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->searchResult.languageElements), ((CsrSize) (primitive->searchResult.languageElementsCount * sizeof(CsrBtLanguageElement))));
    }
    CsrCharStringSer(buffer, length, primitive->searchResult.ipv4Subnet);
    CsrCharStringSer(buffer, length, primitive->searchResult.ipv6Subnet);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND
void *CsrBtBslServiceSearchResultIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslServiceSearchResultInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslServiceSearchResultInd *) CsrPmemZalloc(sizeof(CsrBtBslServiceSearchResultInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->moreResults, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->searchRole, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->searchResult.protocolVersion, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->searchResult.loadFactorIncluded, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->searchResult.loadFactor, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->searchResult.bluetoothProfileVersion, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->searchResult.securityDescription, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->searchResult.serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->searchResult.serviceName)));
    CsrMemCpyDes(((void *) &primitive->searchResult.serviceDescription), buffer, &offset, ((CsrSize) sizeof(primitive->searchResult.serviceDescription)));
    CsrUint16Des((CsrUint16 *) &primitive->searchResult.languageElementsCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->searchResult.languageElements), buffer, &offset, ((CsrSize) sizeof(primitive->searchResult.languageElements)));
    CsrUint16Des((CsrUint16 *) &primitive->searchResult.numOfSupportedNetwPacketTypes, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->searchResult.supportedNetwPacketType), buffer, &offset, ((CsrSize) 2 * (50)));
    CsrUint8Des((CsrUint8 *) &primitive->searchResult.netAccessTypeIncluded, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->searchResult.netAccessType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->searchResult.maxNetAccessRateIncluded, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->searchResult.maxNetAccessRate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->searchResult.ipv4SubnetIncluded, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->searchResult.ipv4Subnet), buffer, &offset, ((CsrSize) sizeof(primitive->searchResult.ipv4Subnet)));
    CsrUint8Des((CsrUint8 *) &primitive->searchResult.ipv6SubnetIncluded, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->searchResult.ipv6Subnet), buffer, &offset, ((CsrSize) sizeof(primitive->searchResult.ipv6Subnet)));
    CsrCharStringDes(&primitive->searchResult.serviceName, buffer, &offset);
    CsrCharStringDes(&primitive->searchResult.serviceDescription, buffer, &offset);
    if (primitive->searchResult.languageElementsCount)
    {
        primitive->searchResult.languageElements = CsrPmemZalloc(((CsrUint32) (primitive->searchResult.languageElementsCount * sizeof(CsrBtLanguageElement))));
        CsrMemCpyDes(((void *) primitive->searchResult.languageElements), buffer, &offset, ((CsrSize) (primitive->searchResult.languageElementsCount * sizeof(CsrBtLanguageElement))));
    }
    else
    {
        primitive->searchResult.languageElements = NULL;
    }
    CsrCharStringDes(&primitive->searchResult.ipv4Subnet, buffer, &offset);
    CsrCharStringDes(&primitive->searchResult.ipv6Subnet, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND
void CsrBtBslServiceSearchResultIndSerFree(void *msg)
{
    CsrBtBslServiceSearchResultInd *primitive;

    primitive = (CsrBtBslServiceSearchResultInd *) msg;

    if(primitive->searchResult.serviceName != NULL)
    {
        CsrPmemFree(primitive->searchResult.serviceName);
    }
    if(primitive->searchResult.serviceDescription != NULL)
    {
        CsrPmemFree(primitive->searchResult.serviceDescription);
    }
    if(primitive->searchResult.languageElements != NULL)
    {
        CsrPmemFree(primitive->searchResult.languageElements);
    }
    if(primitive->searchResult.ipv4Subnet != NULL)
    {
        CsrPmemFree(primitive->searchResult.ipv4Subnet);
    }
    if(primitive->searchResult.ipv6Subnet != NULL)
    {
        CsrPmemFree(primitive->searchResult.ipv6Subnet);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_RESULT_IND */

#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_IN_CFM
CsrSize CsrBtBslSecurityInCfmSizeof(void *msg)
{
    CsrBtBslSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_IN_CFM
CsrUint8 *CsrBtBslSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslSecurityInCfm *primitive;

    primitive = (CsrBtBslSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_IN_CFM
void *CsrBtBslSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtBslSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_OUT_CFM
CsrSize CsrBtBslSecurityOutCfmSizeof(void *msg)
{
    CsrBtBslSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_OUT_CFM
CsrUint8 *CsrBtBslSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslSecurityOutCfm *primitive;

    primitive = (CsrBtBslSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_BSL_SECURITY_OUT_CFM
void *CsrBtBslSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtBslSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_REQ
CsrSize CsrBtBslServiceSearchReqSizeof(void *msg)
{
    CsrBtBslServiceSearchReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_REQ
CsrUint8 *CsrBtBslServiceSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslServiceSearchReq *primitive;

    primitive = (CsrBtBslServiceSearchReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bd_addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bd_addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bd_addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->searchRole);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_REQ
void *CsrBtBslServiceSearchReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslServiceSearchReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslServiceSearchReq *) CsrPmemZalloc(sizeof(CsrBtBslServiceSearchReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bd_addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bd_addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bd_addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->searchRole, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_SERVICE_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_HOUSE_CLEANING
CsrSize CsrBtBslHouseCleaningSizeof(void *msg)
{
    CsrBtBslHouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_BSL_HOUSE_CLEANING
CsrUint8 *CsrBtBslHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslHouseCleaning *primitive;

    primitive = (CsrBtBslHouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_BSL_HOUSE_CLEANING
void *CsrBtBslHouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslHouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslHouseCleaning *) CsrPmemZalloc(sizeof(CsrBtBslHouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_BSL_MULTICAST_DATA_REQ
CsrSize CsrBtBslMulticastDataReqSizeof(void *msg)
{
    CsrBtBslMulticastDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_MULTICAST_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_MULTICAST_DATA_REQ
CsrUint8 *CsrBtBslMulticastDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslMulticastDataReq *primitive;

    primitive = (CsrBtBslMulticastDataReq *) msg;
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
#endif /* EXCLUDE_CSR_BT_BSL_MULTICAST_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_MULTICAST_DATA_REQ
void *CsrBtBslMulticastDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslMulticastDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslMulticastDataReq *) CsrPmemZalloc(sizeof(CsrBtBslMulticastDataReq));

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
#endif /* EXCLUDE_CSR_BT_BSL_MULTICAST_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_MULTICAST_DATA_REQ
void CsrBtBslMulticastDataReqSerFree(void *msg)
{
    CsrBtBslMulticastDataReq *primitive;

    primitive = (CsrBtBslMulticastDataReq *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BSL_MULTICAST_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_DISCONNECT_RES
CsrSize CsrBtBslDisconnectResSizeof(void *msg)
{
    CsrBtBslDisconnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_DISCONNECT_RES */

#ifndef EXCLUDE_CSR_BT_BSL_DISCONNECT_RES
CsrUint8 *CsrBtBslDisconnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslDisconnectRes *primitive;

    primitive = (CsrBtBslDisconnectRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->id);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_DISCONNECT_RES */

#ifndef EXCLUDE_CSR_BT_BSL_DISCONNECT_RES
void *CsrBtBslDisconnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslDisconnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslDisconnectRes *) CsrPmemZalloc(sizeof(CsrBtBslDisconnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->id, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_DISCONNECT_RES */

#ifndef EXCLUDE_CSR_BT_BSL_CANCEL_CONNECT_REQ
CsrSize CsrBtBslCancelConnectReqSizeof(void *msg)
{
    CsrBtBslCancelConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_CANCEL_CONNECT_REQ
CsrUint8 *CsrBtBslCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslCancelConnectReq *primitive;

    primitive = (CsrBtBslCancelConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bdAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bdAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bdAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_BSL_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_CANCEL_CONNECT_REQ
void *CsrBtBslCancelConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslCancelConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslCancelConnectReq *) CsrPmemZalloc(sizeof(CsrBtBslCancelConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bdAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bdAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bdAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_BSL_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_BSL_DATA_IND
CsrSize CsrBtBslDataIndSizeof(void *msg)
{
    CsrBtBslDataInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_BSL_DATA_IND */

#ifndef EXCLUDE_CSR_BT_BSL_DATA_IND
CsrUint8 *CsrBtBslDataIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtBslDataInd *primitive;

    primitive = (CsrBtBslDataInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_BSL_DATA_IND */

#ifndef EXCLUDE_CSR_BT_BSL_DATA_IND
void *CsrBtBslDataIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtBslDataInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtBslDataInd *) CsrPmemZalloc(sizeof(CsrBtBslDataInd));

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
#endif /* EXCLUDE_CSR_BT_BSL_DATA_IND */

#ifndef EXCLUDE_CSR_BT_BSL_DATA_IND
void CsrBtBslDataIndSerFree(void *msg)
{
    CsrBtBslDataInd *primitive;

    primitive = (CsrBtBslDataInd *) msg;

    if(primitive->payload != NULL)
    {
        CsrPmemFree(primitive->payload);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_BSL_DATA_IND */

#endif /* EXCLUDE_CSR_BT_BSL_MODULE */
