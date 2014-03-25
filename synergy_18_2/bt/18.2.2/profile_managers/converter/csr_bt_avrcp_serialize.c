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
#include "csr_bt_avrcp_prim.h"

#ifndef EXCLUDE_CSR_BT_AVRCP_MODULE

void CsrBtAvrcpPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_REQ
CsrSize CsrBtAvrcpSecurityOutReqSizeof(void *msg)
{
    CsrBtAvrcpSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_REQ
CsrUint8 *CsrBtAvrcpSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpSecurityOutReq *primitive;

    primitive = (CsrBtAvrcpSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_REQ
void *CsrBtAvrcpSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_REQ
CsrSize CsrBtAvrcpCtChangePathReqSizeof(void *msg)
{
    CsrBtAvrcpCtChangePathReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_REQ
CsrUint8 *CsrBtAvrcpCtChangePathReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtChangePathReq *primitive;

    primitive = (CsrBtAvrcpCtChangePathReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->folderDir);
    CsrMemCpySer(buffer, length, ((const void *) primitive->folderUid), ((CsrSize) ((8))));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_REQ
void *CsrBtAvrcpCtChangePathReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtChangePathReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtChangePathReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtChangePathReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->folderDir, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->folderUid), buffer, &offset, ((CsrSize) ((8))));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_IND
CsrSize CsrBtAvrcpTgChangePathIndSizeof(void *msg)
{
    CsrBtAvrcpTgChangePathInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_IND
CsrUint8 *CsrBtAvrcpTgChangePathIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgChangePathInd *primitive;

    primitive = (CsrBtAvrcpTgChangePathInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->folderDir);
    CsrMemCpySer(buffer, length, ((const void *) primitive->folderUid), ((CsrSize) ((8))));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_IND
void *CsrBtAvrcpTgChangePathIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgChangePathInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgChangePathInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgChangePathInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->folderDir, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->folderUid), buffer, &offset, ((CsrSize) ((8))));
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PLAY_REQ
CsrSize CsrBtAvrcpCtPlayReqSizeof(void *msg)
{
    CsrBtAvrcpCtPlayReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PLAY_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PLAY_REQ
CsrUint8 *CsrBtAvrcpCtPlayReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPlayReq *primitive;

    primitive = (CsrBtAvrcpCtPlayReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scope);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uid), ((CsrSize) ((8))));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PLAY_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PLAY_REQ
void *CsrBtAvrcpCtPlayReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPlayReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPlayReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPlayReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scope, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uid), buffer, &offset, ((CsrSize) ((8))));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PLAY_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PLAY_IND
CsrSize CsrBtAvrcpTgPlayIndSizeof(void *msg)
{
    CsrBtAvrcpTgPlayInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PLAY_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PLAY_IND
CsrUint8 *CsrBtAvrcpTgPlayIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgPlayInd *primitive;

    primitive = (CsrBtAvrcpTgPlayInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scope);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uid), ((CsrSize) ((8))));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PLAY_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PLAY_IND
void *CsrBtAvrcpTgPlayIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgPlayInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgPlayInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgPlayInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scope, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uid), buffer, &offset, ((CsrSize) ((8))));
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PLAY_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_REQ
CsrSize CsrBtAvrcpCtGetAttributesReqSizeof(void *msg)
{
    CsrBtAvrcpCtGetAttributesReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_REQ
CsrUint8 *CsrBtAvrcpCtGetAttributesReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtGetAttributesReq *primitive;

    primitive = (CsrBtAvrcpCtGetAttributesReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scope);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uid), ((CsrSize) ((8))));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->attributeMask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_REQ
void *CsrBtAvrcpCtGetAttributesReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtGetAttributesReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtGetAttributesReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtGetAttributesReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scope, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uid), buffer, &offset, ((CsrSize) ((8))));
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->attributeMask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_DISCONNECT_REQ
CsrSize CsrBtAvrcpDisconnectReqSizeof(void *msg)
{
    CsrBtAvrcpDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_DISCONNECT_REQ
CsrUint8 *CsrBtAvrcpDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpDisconnectReq *primitive;

    primitive = (CsrBtAvrcpDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_DISCONNECT_REQ
void *CsrBtAvrcpDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONFIG_REQ
CsrSize CsrBtAvrcpConfigReqSizeof(void *msg)
{
    CsrBtAvrcpConfigReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->tgDetails.providerName) + CsrCharStringSerLen((CsrCharString *) primitive->tgDetails.serviceName) + CsrCharStringSerLen((CsrCharString *) primitive->ctDetails.providerName) + CsrCharStringSerLen((CsrCharString *) primitive->ctDetails.serviceName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONFIG_REQ
CsrUint8 *CsrBtAvrcpConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpConfigReq *primitive;

    primitive = (CsrBtAvrcpConfigReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->globalConfig);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->mtu);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tgDetails.roleSupported);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->tgDetails.roleConfig);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->tgDetails.srAvrcpVersion);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->tgDetails.srFeatures);
    CsrMemCpySer(buffer, length, &primitive->tgDetails.providerName, sizeof(primitive->tgDetails.providerName));
    CsrMemCpySer(buffer, length, &primitive->tgDetails.serviceName, sizeof(primitive->tgDetails.serviceName));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ctDetails.roleSupported);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ctDetails.roleConfig);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ctDetails.srAvrcpVersion);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ctDetails.srFeatures);
    CsrMemCpySer(buffer, length, &primitive->ctDetails.providerName, sizeof(primitive->ctDetails.providerName));
    CsrMemCpySer(buffer, length, &primitive->ctDetails.serviceName, sizeof(primitive->ctDetails.serviceName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCount);
    CsrCharStringSer(buffer, length, primitive->tgDetails.providerName);
    CsrCharStringSer(buffer, length, primitive->tgDetails.serviceName);
    CsrCharStringSer(buffer, length, primitive->ctDetails.providerName);
    CsrCharStringSer(buffer, length, primitive->ctDetails.serviceName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONFIG_REQ
void *CsrBtAvrcpConfigReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpConfigReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpConfigReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpConfigReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->globalConfig, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->mtu, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tgDetails.roleSupported, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->tgDetails.roleConfig, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->tgDetails.srAvrcpVersion, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->tgDetails.srFeatures, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->tgDetails.providerName), buffer, &offset, ((CsrSize) sizeof(primitive->tgDetails.providerName)));
    CsrMemCpyDes(((void *) &primitive->tgDetails.serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->tgDetails.serviceName)));
    CsrUint8Des((CsrUint8 *) &primitive->ctDetails.roleSupported, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ctDetails.roleConfig, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ctDetails.srAvrcpVersion, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ctDetails.srFeatures, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ctDetails.providerName), buffer, &offset, ((CsrSize) sizeof(primitive->ctDetails.providerName)));
    CsrMemCpyDes(((void *) &primitive->ctDetails.serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->ctDetails.serviceName)));
    CsrUint16Des((CsrUint16 *) &primitive->uidCount, buffer, &offset);
    CsrCharStringDes(&primitive->tgDetails.providerName, buffer, &offset);
    CsrCharStringDes(&primitive->tgDetails.serviceName, buffer, &offset);
    CsrCharStringDes(&primitive->ctDetails.providerName, buffer, &offset);
    CsrCharStringDes(&primitive->ctDetails.serviceName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONFIG_REQ
void CsrBtAvrcpConfigReqSerFree(void *msg)
{
    CsrBtAvrcpConfigReq *primitive;

    primitive = (CsrBtAvrcpConfigReq *) msg;

    if(primitive->tgDetails.providerName != NULL)
    {
        CsrPmemFree(primitive->tgDetails.providerName);
    }
    if(primitive->tgDetails.serviceName != NULL)
    {
        CsrPmemFree(primitive->tgDetails.serviceName);
    }
    if(primitive->ctDetails.providerName != NULL)
    {
        CsrPmemFree(primitive->ctDetails.providerName);
    }
    if(primitive->ctDetails.serviceName != NULL)
    {
        CsrPmemFree(primitive->ctDetails.serviceName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_CFM
CsrSize CsrBtAvrcpTgSetAddressedPlayerCfmSizeof(void *msg)
{
    CsrBtAvrcpTgSetAddressedPlayerCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_CFM
CsrUint8 *CsrBtAvrcpTgSetAddressedPlayerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgSetAddressedPlayerCfm *primitive;

    primitive = (CsrBtAvrcpTgSetAddressedPlayerCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_CFM
void *CsrBtAvrcpTgSetAddressedPlayerCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgSetAddressedPlayerCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgSetAddressedPlayerCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgSetAddressedPlayerCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_REQ
CsrSize CsrBtAvrcpTgSetAddressedPlayerReqSizeof(void *msg)
{
    CsrBtAvrcpTgSetAddressedPlayerReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_REQ
CsrUint8 *CsrBtAvrcpTgSetAddressedPlayerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgSetAddressedPlayerReq *primitive;

    primitive = (CsrBtAvrcpTgSetAddressedPlayerReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_REQ
void *CsrBtAvrcpTgSetAddressedPlayerReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgSetAddressedPlayerReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgSetAddressedPlayerReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgSetAddressedPlayerReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_IND
CsrSize CsrBtAvrcpTgSetBrowsedPlayerIndSizeof(void *msg)
{
    CsrBtAvrcpTgSetBrowsedPlayerInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_IND
CsrUint8 *CsrBtAvrcpTgSetBrowsedPlayerIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgSetBrowsedPlayerInd *primitive;

    primitive = (CsrBtAvrcpTgSetBrowsedPlayerInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_IND
void *CsrBtAvrcpTgSetBrowsedPlayerIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgSetBrowsedPlayerInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgSetBrowsedPlayerInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgSetBrowsedPlayerInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_CFM
CsrSize CsrBtAvrcpCtGetPlayStatusCfmSizeof(void *msg)
{
    CsrBtAvrcpCtGetPlayStatusCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_CFM
CsrUint8 *CsrBtAvrcpCtGetPlayStatusCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtGetPlayStatusCfm *primitive;

    primitive = (CsrBtAvrcpCtGetPlayStatusCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->songLength);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->songPosition);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->playStatus);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_CFM
void *CsrBtAvrcpCtGetPlayStatusCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtGetPlayStatusCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtGetPlayStatusCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtGetPlayStatusCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->songLength, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->songPosition, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->playStatus, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM
CsrSize CsrBtAvrcpCtAddToNowPlayingCfmSizeof(void *msg)
{
    CsrBtAvrcpCtAddToNowPlayingCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM
CsrUint8 *CsrBtAvrcpCtAddToNowPlayingCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtAddToNowPlayingCfm *primitive;

    primitive = (CsrBtAvrcpCtAddToNowPlayingCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scope);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uid), ((CsrSize) ((8))));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM
void *CsrBtAvrcpCtAddToNowPlayingCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtAddToNowPlayingCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtAddToNowPlayingCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtAddToNowPlayingCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scope, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uid), buffer, &offset, ((CsrSize) ((8))));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ
CsrSize CsrBtAvrcpCtPasValTxtReqSizeof(void *msg)
{
    CsrBtAvrcpCtPasValTxtReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->valIdCount * sizeof(CsrBtAvrcpPasValId)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ
CsrUint8 *CsrBtAvrcpCtPasValTxtReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasValTxtReq *primitive;

    primitive = (CsrBtAvrcpCtPasValTxtReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->valIdCount);
    CsrMemCpySer(buffer, length, &primitive->valId, sizeof(primitive->valId));
    if(primitive->valId)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->valId), ((CsrSize) (primitive->valIdCount * sizeof(CsrBtAvrcpPasValId))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ
void *CsrBtAvrcpCtPasValTxtReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasValTxtReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasValTxtReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasValTxtReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->attId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->valIdCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->valId), buffer, &offset, ((CsrSize) sizeof(primitive->valId)));
    if (primitive->valIdCount)
    {
        primitive->valId = CsrPmemZalloc(((CsrUint32) (primitive->valIdCount * sizeof(CsrBtAvrcpPasValId))));
        CsrMemCpyDes(((void *) primitive->valId), buffer, &offset, ((CsrSize) (primitive->valIdCount * sizeof(CsrBtAvrcpPasValId))));
    }
    else
    {
        primitive->valId = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ
void CsrBtAvrcpCtPasValTxtReqSerFree(void *msg)
{
    CsrBtAvrcpCtPasValTxtReq *primitive;

    primitive = (CsrBtAvrcpCtPasValTxtReq *) msg;

    if(primitive->valId != NULL)
    {
        CsrPmemFree(primitive->valId);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_REQ
CsrSize CsrBtAvrcpTgPasSetReqSizeof(void *msg)
{
    CsrBtAvrcpTgPasSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_REQ
CsrUint8 *CsrBtAvrcpTgPasSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgPasSetReq *primitive;

    primitive = (CsrBtAvrcpTgPasSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attValPairCount);
    CsrMemCpySer(buffer, length, &primitive->attValPair, sizeof(primitive->attValPair));
    if(primitive->attValPair)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attValPair), ((CsrSize) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_REQ
void *CsrBtAvrcpTgPasSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgPasSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgPasSetReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgPasSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->attValPairCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attValPair), buffer, &offset, ((CsrSize) sizeof(primitive->attValPair)));
    if (primitive->attValPairCount)
    {
        primitive->attValPair = CsrPmemZalloc(((CsrUint32) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
        CsrMemCpyDes(((void *) primitive->attValPair), buffer, &offset, ((CsrSize) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
    }
    else
    {
        primitive->attValPair = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_REQ
void CsrBtAvrcpTgPasSetReqSerFree(void *msg)
{
    CsrBtAvrcpTgPasSetReq *primitive;

    primitive = (CsrBtAvrcpTgPasSetReq *) msg;

    if(primitive->attValPair != NULL)
    {
        CsrPmemFree(primitive->attValPair);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_IND
CsrSize CsrBtAvrcpTgPassThroughIndSizeof(void *msg)
{
    CsrBtAvrcpTgPassThroughInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_IND
CsrUint8 *CsrBtAvrcpTgPassThroughIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgPassThroughInd *primitive;

    primitive = (CsrBtAvrcpTgPassThroughInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->operationId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->state);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_IND
void *CsrBtAvrcpTgPassThroughIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgPassThroughInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgPassThroughInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgPassThroughInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->operationId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->state, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_CFM
CsrSize CsrBtAvrcpCtPassThroughCfmSizeof(void *msg)
{
    CsrBtAvrcpCtPassThroughCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_CFM
CsrUint8 *CsrBtAvrcpCtPassThroughCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPassThroughCfm *primitive;

    primitive = (CsrBtAvrcpCtPassThroughCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->operationId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->state);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_CFM
void *CsrBtAvrcpCtPassThroughCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPassThroughCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPassThroughCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPassThroughCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->operationId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->state, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_CFM
CsrSize CsrBtAvrcpCtSetVolumeCfmSizeof(void *msg)
{
    CsrBtAvrcpCtSetVolumeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_CFM
CsrUint8 *CsrBtAvrcpCtSetVolumeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtSetVolumeCfm *primitive;

    primitive = (CsrBtAvrcpCtSetVolumeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->volume);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_CFM
void *CsrBtAvrcpCtSetVolumeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtSetVolumeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtSetVolumeCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtSetVolumeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->volume, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONFIG_CFM
CsrSize CsrBtAvrcpConfigCfmSizeof(void *msg)
{
    CsrBtAvrcpConfigCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONFIG_CFM
CsrUint8 *CsrBtAvrcpConfigCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpConfigCfm *primitive;

    primitive = (CsrBtAvrcpConfigCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONFIG_CFM
void *CsrBtAvrcpConfigCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpConfigCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpConfigCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpConfigCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONFIG_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_CFM
CsrSize CsrBtAvrcpTgNotiCfmSizeof(void *msg)
{
    CsrBtAvrcpTgNotiCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_CFM
CsrUint8 *CsrBtAvrcpTgNotiCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgNotiCfm *primitive;

    primitive = (CsrBtAvrcpTgNotiCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->notiId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_CFM
void *CsrBtAvrcpTgNotiCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgNotiCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgNotiCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgNotiCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->notiId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_IND
CsrSize CsrBtAvrcpTgSetVolumeIndSizeof(void *msg)
{
    CsrBtAvrcpTgSetVolumeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_IND
CsrUint8 *CsrBtAvrcpTgSetVolumeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgSetVolumeInd *primitive;

    primitive = (CsrBtAvrcpTgSetVolumeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->volume);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_IND
void *CsrBtAvrcpTgSetVolumeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgSetVolumeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgSetVolumeInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgSetVolumeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->volume, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_REQ
CsrSize CsrBtAvrcpCtGetFolderItemsReqSizeof(void *msg)
{
    CsrBtAvrcpCtGetFolderItemsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_REQ
CsrUint8 *CsrBtAvrcpCtGetFolderItemsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtGetFolderItemsReq *primitive;

    primitive = (CsrBtAvrcpCtGetFolderItemsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scope);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->startItem);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->endItem);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->attributeMask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_REQ
void *CsrBtAvrcpCtGetFolderItemsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtGetFolderItemsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtGetFolderItemsReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtGetFolderItemsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scope, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->startItem, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->endItem, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->attributeMask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND
CsrSize CsrBtAvrcpCtPasValTxtIndSizeof(void *msg)
{
    CsrBtAvrcpCtPasValTxtInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->pasDataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND
CsrUint8 *CsrBtAvrcpCtPasValTxtIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasValTxtInd *primitive;

    primitive = (CsrBtAvrcpCtPasValTxtInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pasDataLen);
    CsrMemCpySer(buffer, length, &primitive->pasData, sizeof(primitive->pasData));
    if(primitive->pasData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->pasData), ((CsrSize) (primitive->pasDataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND
void *CsrBtAvrcpCtPasValTxtIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasValTxtInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasValTxtInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasValTxtInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->attId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pasDataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->pasData), buffer, &offset, ((CsrSize) sizeof(primitive->pasData)));
    if (primitive->pasDataLen)
    {
        primitive->pasData = CsrPmemZalloc(((CsrUint32) (primitive->pasDataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->pasData), buffer, &offset, ((CsrSize) (primitive->pasDataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->pasData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND
void CsrBtAvrcpCtPasValTxtIndSerFree(void *msg)
{
    CsrBtAvrcpCtPasValTxtInd *primitive;

    primitive = (CsrBtAvrcpCtPasValTxtInd *) msg;

    if(primitive->pasData != NULL)
    {
        CsrPmemFree(primitive->pasData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_IND
CsrSize CsrBtAvrcpTgPasSetIndSizeof(void *msg)
{
    CsrBtAvrcpTgPasSetInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_IND
CsrUint8 *CsrBtAvrcpTgPasSetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgPasSetInd *primitive;

    primitive = (CsrBtAvrcpTgPasSetInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attValPairCount);
    CsrMemCpySer(buffer, length, &primitive->attValPair, sizeof(primitive->attValPair));
    if(primitive->attValPair)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attValPair), ((CsrSize) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_IND
void *CsrBtAvrcpTgPasSetIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgPasSetInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgPasSetInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgPasSetInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->attValPairCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attValPair), buffer, &offset, ((CsrSize) sizeof(primitive->attValPair)));
    if (primitive->attValPairCount)
    {
        primitive->attValPair = CsrPmemZalloc(((CsrUint32) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
        CsrMemCpyDes(((void *) primitive->attValPair), buffer, &offset, ((CsrSize) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
    }
    else
    {
        primitive->attValPair = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_IND
void CsrBtAvrcpTgPasSetIndSerFree(void *msg)
{
    CsrBtAvrcpTgPasSetInd *primitive;

    primitive = (CsrBtAvrcpTgPasSetInd *) msg;

    if(primitive->attValPair != NULL)
    {
        CsrPmemFree(primitive->attValPair);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND
CsrSize CsrBtAvrcpCtNotiPlaybackPosIndSizeof(void *msg)
{
    CsrBtAvrcpCtNotiPlaybackPosInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND
CsrUint8 *CsrBtAvrcpCtNotiPlaybackPosIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtNotiPlaybackPosInd *primitive;

    primitive = (CsrBtAvrcpCtNotiPlaybackPosInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playbackPos);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND
void *CsrBtAvrcpCtNotiPlaybackPosIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtNotiPlaybackPosInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtNotiPlaybackPosInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtNotiPlaybackPosInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playbackPos, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_POS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND
CsrSize CsrBtAvrcpCtSetAddressedPlayerIndSizeof(void *msg)
{
    CsrBtAvrcpCtSetAddressedPlayerInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND
CsrUint8 *CsrBtAvrcpCtSetAddressedPlayerIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtSetAddressedPlayerInd *primitive;

    primitive = (CsrBtAvrcpCtSetAddressedPlayerInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND
void *CsrBtAvrcpCtSetAddressedPlayerIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtSetAddressedPlayerInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtSetAddressedPlayerInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtSetAddressedPlayerInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_REQ
CsrSize CsrBtAvrcpTgMpUnregisterReqSizeof(void *msg)
{
    CsrBtAvrcpTgMpUnregisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_REQ
CsrUint8 *CsrBtAvrcpTgMpUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgMpUnregisterReq *primitive;

    primitive = (CsrBtAvrcpTgMpUnregisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_REQ
void *CsrBtAvrcpTgMpUnregisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgMpUnregisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgMpUnregisterReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgMpUnregisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_REQ
CsrSize CsrBtAvrcpCtInformBatteryStatusReqSizeof(void *msg)
{
    CsrBtAvrcpCtInformBatteryStatusReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_REQ
CsrUint8 *CsrBtAvrcpCtInformBatteryStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtInformBatteryStatusReq *primitive;

    primitive = (CsrBtAvrcpCtInformBatteryStatusReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->batStatus);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_REQ
void *CsrBtAvrcpCtInformBatteryStatusReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtInformBatteryStatusReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtInformBatteryStatusReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtInformBatteryStatusReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->batStatus, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CANCEL_CONNECT_REQ
CsrSize CsrBtAvrcpCancelConnectReqSizeof(void *msg)
{
    CsrBtAvrcpCancelConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CANCEL_CONNECT_REQ
CsrUint8 *CsrBtAvrcpCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCancelConnectReq *primitive;

    primitive = (CsrBtAvrcpCancelConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CANCEL_CONNECT_REQ
void *CsrBtAvrcpCancelConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCancelConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCancelConnectReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCancelConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_ACTIVATE_REQ
CsrSize CsrBtAvrcpActivateReqSizeof(void *msg)
{
    CsrBtAvrcpActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_ACTIVATE_REQ
CsrUint8 *CsrBtAvrcpActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpActivateReq *primitive;

    primitive = (CsrBtAvrcpActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->maxIncoming);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_ACTIVATE_REQ
void *CsrBtAvrcpActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpActivateReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->maxIncoming, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_REQ
CsrSize CsrBtAvrcpCtPassThroughReqSizeof(void *msg)
{
    CsrBtAvrcpCtPassThroughReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_REQ
CsrUint8 *CsrBtAvrcpCtPassThroughReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPassThroughReq *primitive;

    primitive = (CsrBtAvrcpCtPassThroughReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->operationId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->state);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_REQ
void *CsrBtAvrcpCtPassThroughReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPassThroughReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPassThroughReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPassThroughReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->operationId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->state, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PASS_THROUGH_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND
CsrSize CsrBtAvrcpCtNotiNowPlayingIndSizeof(void *msg)
{
    CsrBtAvrcpCtNotiNowPlayingInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND
CsrUint8 *CsrBtAvrcpCtNotiNowPlayingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtNotiNowPlayingInd *primitive;

    primitive = (CsrBtAvrcpCtNotiNowPlayingInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND
void *CsrBtAvrcpCtNotiNowPlayingIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtNotiNowPlayingInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtNotiNowPlayingInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtNotiNowPlayingInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_NOW_PLAYING_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_REQ
CsrSize CsrBtAvrcpCtSetAddressedPlayerReqSizeof(void *msg)
{
    CsrBtAvrcpCtSetAddressedPlayerReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_REQ
CsrUint8 *CsrBtAvrcpCtSetAddressedPlayerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtSetAddressedPlayerReq *primitive;

    primitive = (CsrBtAvrcpCtSetAddressedPlayerReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_REQ
void *CsrBtAvrcpCtSetAddressedPlayerReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtSetAddressedPlayerReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtSetAddressedPlayerReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtSetAddressedPlayerReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_RES
CsrSize CsrBtAvrcpTgAddToNowPlayingResSizeof(void *msg)
{
    CsrBtAvrcpTgAddToNowPlayingRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_RES
CsrUint8 *CsrBtAvrcpTgAddToNowPlayingResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgAddToNowPlayingRes *primitive;

    primitive = (CsrBtAvrcpTgAddToNowPlayingRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_RES
void *CsrBtAvrcpTgAddToNowPlayingResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgAddToNowPlayingRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgAddToNowPlayingRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgAddToNowPlayingRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND
CsrSize CsrBtAvrcpCtNotiBatteryStatusIndSizeof(void *msg)
{
    CsrBtAvrcpCtNotiBatteryStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND
CsrUint8 *CsrBtAvrcpCtNotiBatteryStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtNotiBatteryStatusInd *primitive;

    primitive = (CsrBtAvrcpCtNotiBatteryStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->batteryStatus);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND
void *CsrBtAvrcpCtNotiBatteryStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtNotiBatteryStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtNotiBatteryStatusInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtNotiBatteryStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->batteryStatus, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_BATTERY_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM
CsrSize CsrBtAvrcpCtInformBatteryStatusCfmSizeof(void *msg)
{
    CsrBtAvrcpCtInformBatteryStatusCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM
CsrUint8 *CsrBtAvrcpCtInformBatteryStatusCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtInformBatteryStatusCfm *primitive;

    primitive = (CsrBtAvrcpCtInformBatteryStatusCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM
void *CsrBtAvrcpCtInformBatteryStatusCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtInformBatteryStatusCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtInformBatteryStatusCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtInformBatteryStatusCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_BATTERY_STATUS_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_REQ
CsrSize CsrBtAvrcpCtPasValIdReqSizeof(void *msg)
{
    CsrBtAvrcpCtPasValIdReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_REQ
CsrUint8 *CsrBtAvrcpCtPasValIdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasValIdReq *primitive;

    primitive = (CsrBtAvrcpCtPasValIdReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_REQ
void *CsrBtAvrcpCtPasValIdReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasValIdReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasValIdReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasValIdReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->attId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM
CsrSize CsrBtAvrcpCtPasAttTxtCfmSizeof(void *msg)
{
    CsrBtAvrcpCtPasAttTxtCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->pasDataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM
CsrUint8 *CsrBtAvrcpCtPasAttTxtCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasAttTxtCfm *primitive;

    primitive = (CsrBtAvrcpCtPasAttTxtCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pasDataLen);
    CsrMemCpySer(buffer, length, &primitive->pasData, sizeof(primitive->pasData));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    if(primitive->pasData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->pasData), ((CsrSize) (primitive->pasDataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM
void *CsrBtAvrcpCtPasAttTxtCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasAttTxtCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasAttTxtCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasAttTxtCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pasDataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->pasData), buffer, &offset, ((CsrSize) sizeof(primitive->pasData)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    if (primitive->pasDataLen)
    {
        primitive->pasData = CsrPmemZalloc(((CsrUint32) (primitive->pasDataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->pasData), buffer, &offset, ((CsrSize) (primitive->pasDataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->pasData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM
void CsrBtAvrcpCtPasAttTxtCfmSerFree(void *msg)
{
    CsrBtAvrcpCtPasAttTxtCfm *primitive;

    primitive = (CsrBtAvrcpCtPasAttTxtCfm *) msg;

    if(primitive->pasData != NULL)
    {
        CsrPmemFree(primitive->pasData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM
CsrSize CsrBtAvrcpCtSetAddressedPlayerCfmSizeof(void *msg)
{
    CsrBtAvrcpCtSetAddressedPlayerCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM
CsrUint8 *CsrBtAvrcpCtSetAddressedPlayerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtSetAddressedPlayerCfm *primitive;

    primitive = (CsrBtAvrcpCtSetAddressedPlayerCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM
void *CsrBtAvrcpCtSetAddressedPlayerCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtSetAddressedPlayerCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtSetAddressedPlayerCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtSetAddressedPlayerCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_ADDRESSED_PLAYER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_IND
CsrSize CsrBtAvrcpTgPasCurrentIndSizeof(void *msg)
{
    CsrBtAvrcpTgPasCurrentInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attIdCount * sizeof(CsrBtAvrcpPasAttId)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_IND
CsrUint8 *CsrBtAvrcpTgPasCurrentIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgPasCurrentInd *primitive;

    primitive = (CsrBtAvrcpTgPasCurrentInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attIdCount);
    CsrMemCpySer(buffer, length, &primitive->attId, sizeof(primitive->attId));
    if(primitive->attId)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attId), ((CsrSize) (primitive->attIdCount * sizeof(CsrBtAvrcpPasAttId))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_IND
void *CsrBtAvrcpTgPasCurrentIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgPasCurrentInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgPasCurrentInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgPasCurrentInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->attIdCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attId), buffer, &offset, ((CsrSize) sizeof(primitive->attId)));
    if (primitive->attIdCount)
    {
        primitive->attId = CsrPmemZalloc(((CsrUint32) (primitive->attIdCount * sizeof(CsrBtAvrcpPasAttId))));
        CsrMemCpyDes(((void *) primitive->attId), buffer, &offset, ((CsrSize) (primitive->attIdCount * sizeof(CsrBtAvrcpPasAttId))));
    }
    else
    {
        primitive->attId = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_IND
void CsrBtAvrcpTgPasCurrentIndSerFree(void *msg)
{
    CsrBtAvrcpTgPasCurrentInd *primitive;

    primitive = (CsrBtAvrcpTgPasCurrentInd *) msg;

    if(primitive->attId != NULL)
    {
        CsrPmemFree(primitive->attId);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_RES
CsrSize CsrBtAvrcpCtPasValTxtResSizeof(void *msg)
{
    CsrBtAvrcpCtPasValTxtRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_RES
CsrUint8 *CsrBtAvrcpCtPasValTxtResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasValTxtRes *primitive;

    primitive = (CsrBtAvrcpCtPasValTxtRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->proceed);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_RES
void *CsrBtAvrcpCtPasValTxtResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasValTxtRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasValTxtRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasValTxtRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->proceed, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_RES
CsrSize CsrBtAvrcpTgGetPlayStatusResSizeof(void *msg)
{
    CsrBtAvrcpTgGetPlayStatusRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_RES
CsrUint8 *CsrBtAvrcpTgGetPlayStatusResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgGetPlayStatusRes *primitive;

    primitive = (CsrBtAvrcpTgGetPlayStatusRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->songLength);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->songPosition);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->playStatus);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_RES
void *CsrBtAvrcpTgGetPlayStatusResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgGetPlayStatusRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgGetPlayStatusRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgGetPlayStatusRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->songLength, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->songPosition, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->playStatus, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_REQ
CsrSize CsrBtAvrcpTgMpRegisterReqSizeof(void *msg)
{
    CsrBtAvrcpTgMpRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->pasLen * sizeof(CsrUint8)) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->playerName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_REQ
CsrUint8 *CsrBtAvrcpTgMpRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgMpRegisterReq *primitive;

    primitive = (CsrBtAvrcpTgMpRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->playerHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->notificationMask);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->configMask);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pasLen);
    CsrMemCpySer(buffer, length, &primitive->pas, sizeof(primitive->pas));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->majorType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->subType);
    CsrMemCpySer(buffer, length, ((const void *) primitive->featureMask), ((CsrSize) 4 * ((4))));
    CsrMemCpySer(buffer, length, &primitive->playerName, sizeof(primitive->playerName));
    if(primitive->pas)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->pas), ((CsrSize) (primitive->pasLen * sizeof(CsrUint8))));
    }
    CsrUtf8StringSer(buffer, length, primitive->playerName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_REQ
void *CsrBtAvrcpTgMpRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgMpRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgMpRegisterReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgMpRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->playerHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->notificationMask, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->configMask, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pasLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->pas), buffer, &offset, ((CsrSize) sizeof(primitive->pas)));
    CsrUint8Des((CsrUint8 *) &primitive->majorType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->subType, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->featureMask), buffer, &offset, ((CsrSize) 4 * ((4))));
    CsrMemCpyDes(((void *) &primitive->playerName), buffer, &offset, ((CsrSize) sizeof(primitive->playerName)));
    if (primitive->pasLen)
    {
        primitive->pas = CsrPmemZalloc(((CsrUint32) (primitive->pasLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->pas), buffer, &offset, ((CsrSize) (primitive->pasLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->pas = NULL;
    }
    CsrUtf8StringDes(&primitive->playerName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_REQ
void CsrBtAvrcpTgMpRegisterReqSerFree(void *msg)
{
    CsrBtAvrcpTgMpRegisterReq *primitive;

    primitive = (CsrBtAvrcpTgMpRegisterReq *) msg;

    if(primitive->pas != NULL)
    {
        CsrPmemFree(primitive->pas);
    }
    if(primitive->playerName != NULL)
    {
        CsrPmemFree(primitive->playerName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM
CsrSize CsrBtAvrcpCtInformDispCharsetCfmSizeof(void *msg)
{
    CsrBtAvrcpCtInformDispCharsetCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM
CsrUint8 *CsrBtAvrcpCtInformDispCharsetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtInformDispCharsetCfm *primitive;

    primitive = (CsrBtAvrcpCtInformDispCharsetCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM
void *CsrBtAvrcpCtInformDispCharsetCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtInformDispCharsetCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtInformDispCharsetCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtInformDispCharsetCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_REQ
CsrSize CsrBtAvrcpCtSetBrowsedPlayerReqSizeof(void *msg)
{
    CsrBtAvrcpCtSetBrowsedPlayerReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_REQ
CsrUint8 *CsrBtAvrcpCtSetBrowsedPlayerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtSetBrowsedPlayerReq *primitive;

    primitive = (CsrBtAvrcpCtSetBrowsedPlayerReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_REQ
void *CsrBtAvrcpCtSetBrowsedPlayerReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtSetBrowsedPlayerReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtSetBrowsedPlayerReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtSetBrowsedPlayerReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_CFM
CsrSize CsrBtAvrcpSecurityOutCfmSizeof(void *msg)
{
    CsrBtAvrcpSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_CFM
CsrUint8 *CsrBtAvrcpSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpSecurityOutCfm *primitive;

    primitive = (CsrBtAvrcpSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_CFM
void *CsrBtAvrcpSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ
CsrSize CsrBtAvrcpCtInformDispCharsetReqSizeof(void *msg)
{
    CsrBtAvrcpCtInformDispCharsetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->charsetCount * sizeof(CsrBtAvrcpCharSet)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ
CsrUint8 *CsrBtAvrcpCtInformDispCharsetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtInformDispCharsetReq *primitive;

    primitive = (CsrBtAvrcpCtInformDispCharsetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->charsetCount);
    CsrMemCpySer(buffer, length, &primitive->charset, sizeof(primitive->charset));
    if(primitive->charset)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->charset), ((CsrSize) (primitive->charsetCount * sizeof(CsrBtAvrcpCharSet))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ
void *CsrBtAvrcpCtInformDispCharsetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtInformDispCharsetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtInformDispCharsetReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtInformDispCharsetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->charsetCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->charset), buffer, &offset, ((CsrSize) sizeof(primitive->charset)));
    if (primitive->charsetCount)
    {
        primitive->charset = CsrPmemZalloc(((CsrUint32) (primitive->charsetCount * sizeof(CsrBtAvrcpCharSet))));
        CsrMemCpyDes(((void *) primitive->charset), buffer, &offset, ((CsrSize) (primitive->charsetCount * sizeof(CsrBtAvrcpCharSet))));
    }
    else
    {
        primitive->charset = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ
void CsrBtAvrcpCtInformDispCharsetReqSerFree(void *msg)
{
    CsrBtAvrcpCtInformDispCharsetReq *primitive;

    primitive = (CsrBtAvrcpCtInformDispCharsetReq *) msg;

    if(primitive->charset != NULL)
    {
        CsrPmemFree(primitive->charset);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_INFORM_DISP_CHARSET_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_IND
CsrSize CsrBtAvrcpTgGetPlayStatusIndSizeof(void *msg)
{
    CsrBtAvrcpTgGetPlayStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_IND
CsrUint8 *CsrBtAvrcpTgGetPlayStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgGetPlayStatusInd *primitive;

    primitive = (CsrBtAvrcpTgGetPlayStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_IND
void *CsrBtAvrcpTgGetPlayStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgGetPlayStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgGetPlayStatusInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgGetPlayStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_PLAY_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_CFM
CsrSize CsrBtAvrcpConnectCfmSizeof(void *msg)
{
    CsrBtAvrcpConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->tgFeatures.providerName) + CsrCharStringSerLen((CsrCharString *) primitive->tgFeatures.serviceName) + CsrCharStringSerLen((CsrCharString *) primitive->ctFeatures.providerName) + CsrCharStringSerLen((CsrCharString *) primitive->ctFeatures.serviceName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_CFM
CsrUint8 *CsrBtAvrcpConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpConnectCfm *primitive;

    primitive = (CsrBtAvrcpConnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tgFeatures.roleSupported);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->tgFeatures.roleConfig);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->tgFeatures.srAvrcpVersion);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->tgFeatures.srFeatures);
    CsrMemCpySer(buffer, length, &primitive->tgFeatures.providerName, sizeof(primitive->tgFeatures.providerName));
    CsrMemCpySer(buffer, length, &primitive->tgFeatures.serviceName, sizeof(primitive->tgFeatures.serviceName));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ctFeatures.roleSupported);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ctFeatures.roleConfig);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ctFeatures.srAvrcpVersion);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ctFeatures.srFeatures);
    CsrMemCpySer(buffer, length, &primitive->ctFeatures.providerName, sizeof(primitive->ctFeatures.providerName));
    CsrMemCpySer(buffer, length, &primitive->ctFeatures.serviceName, sizeof(primitive->ctFeatures.serviceName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrCharStringSer(buffer, length, primitive->tgFeatures.providerName);
    CsrCharStringSer(buffer, length, primitive->tgFeatures.serviceName);
    CsrCharStringSer(buffer, length, primitive->ctFeatures.providerName);
    CsrCharStringSer(buffer, length, primitive->ctFeatures.serviceName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_CFM
void *CsrBtAvrcpConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpConnectCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpConnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tgFeatures.roleSupported, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->tgFeatures.roleConfig, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->tgFeatures.srAvrcpVersion, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->tgFeatures.srFeatures, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->tgFeatures.providerName), buffer, &offset, ((CsrSize) sizeof(primitive->tgFeatures.providerName)));
    CsrMemCpyDes(((void *) &primitive->tgFeatures.serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->tgFeatures.serviceName)));
    CsrUint8Des((CsrUint8 *) &primitive->ctFeatures.roleSupported, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ctFeatures.roleConfig, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ctFeatures.srAvrcpVersion, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ctFeatures.srFeatures, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ctFeatures.providerName), buffer, &offset, ((CsrSize) sizeof(primitive->ctFeatures.providerName)));
    CsrMemCpyDes(((void *) &primitive->ctFeatures.serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->ctFeatures.serviceName)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrCharStringDes(&primitive->tgFeatures.providerName, buffer, &offset);
    CsrCharStringDes(&primitive->tgFeatures.serviceName, buffer, &offset);
    CsrCharStringDes(&primitive->ctFeatures.providerName, buffer, &offset);
    CsrCharStringDes(&primitive->ctFeatures.serviceName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_CFM
void CsrBtAvrcpConnectCfmSerFree(void *msg)
{
    CsrBtAvrcpConnectCfm *primitive;

    primitive = (CsrBtAvrcpConnectCfm *) msg;

    if(primitive->tgFeatures.providerName != NULL)
    {
        CsrPmemFree(primitive->tgFeatures.providerName);
    }
    if(primitive->tgFeatures.serviceName != NULL)
    {
        CsrPmemFree(primitive->tgFeatures.serviceName);
    }
    if(primitive->ctFeatures.providerName != NULL)
    {
        CsrPmemFree(primitive->ctFeatures.providerName);
    }
    if(primitive->ctFeatures.serviceName != NULL)
    {
        CsrPmemFree(primitive->ctFeatures.serviceName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_REQ
CsrSize CsrBtAvrcpCtNotiRegisterReqSizeof(void *msg)
{
    CsrBtAvrcpCtNotiRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_REQ
CsrUint8 *CsrBtAvrcpCtNotiRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtNotiRegisterReq *primitive;

    primitive = (CsrBtAvrcpCtNotiRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->notiMask);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playbackInterval);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_REQ
void *CsrBtAvrcpCtNotiRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtNotiRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtNotiRegisterReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtNotiRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->notiMask, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playbackInterval, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_VOLUME_IND
CsrSize CsrBtAvrcpCtNotiVolumeIndSizeof(void *msg)
{
    CsrBtAvrcpCtNotiVolumeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_VOLUME_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_VOLUME_IND
CsrUint8 *CsrBtAvrcpCtNotiVolumeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtNotiVolumeInd *primitive;

    primitive = (CsrBtAvrcpCtNotiVolumeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->volume);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_VOLUME_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_VOLUME_IND
void *CsrBtAvrcpCtNotiVolumeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtNotiVolumeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtNotiVolumeInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtNotiVolumeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->volume, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_VOLUME_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_REQ
CsrSize CsrBtAvrcpCtGetPlayStatusReqSizeof(void *msg)
{
    CsrBtAvrcpCtGetPlayStatusReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_REQ
CsrUint8 *CsrBtAvrcpCtGetPlayStatusReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtGetPlayStatusReq *primitive;

    primitive = (CsrBtAvrcpCtGetPlayStatusReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_REQ
void *CsrBtAvrcpCtGetPlayStatusReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtGetPlayStatusReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtGetPlayStatusReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtGetPlayStatusReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_PLAY_STATUS_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES
CsrSize CsrBtAvrcpTgGetAttributesResSizeof(void *msg)
{
    CsrBtAvrcpTgGetAttributesRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attribDataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES
CsrUint8 *CsrBtAvrcpTgGetAttributesResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgGetAttributesRes *primitive;

    primitive = (CsrBtAvrcpTgGetAttributesRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attribCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attribDataLen);
    CsrMemCpySer(buffer, length, &primitive->attribData, sizeof(primitive->attribData));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    if(primitive->attribData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attribData), ((CsrSize) (primitive->attribDataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES
void *CsrBtAvrcpTgGetAttributesResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgGetAttributesRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgGetAttributesRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgGetAttributesRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->attribCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attribDataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attribData), buffer, &offset, ((CsrSize) sizeof(primitive->attribData)));
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    if (primitive->attribDataLen)
    {
        primitive->attribData = CsrPmemZalloc(((CsrUint32) (primitive->attribDataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->attribData), buffer, &offset, ((CsrSize) (primitive->attribDataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->attribData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES
void CsrBtAvrcpTgGetAttributesResSerFree(void *msg)
{
    CsrBtAvrcpTgGetAttributesRes *primitive;

    primitive = (CsrBtAvrcpTgGetAttributesRes *) msg;

    if(primitive->attribData != NULL)
    {
        CsrPmemFree(primitive->attribData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_RES
CsrSize CsrBtAvrcpTgPassThroughResSizeof(void *msg)
{
    CsrBtAvrcpTgPassThroughRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_RES
CsrUint8 *CsrBtAvrcpTgPassThroughResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgPassThroughRes *primitive;

    primitive = (CsrBtAvrcpTgPassThroughRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_RES
void *CsrBtAvrcpTgPassThroughResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgPassThroughRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgPassThroughRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgPassThroughRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PASS_THROUGH_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_REQ
CsrSize CsrBtAvrcpTgNotiReqSizeof(void *msg)
{
    CsrBtAvrcpTgNotiReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_REQ
CsrUint8 *CsrBtAvrcpTgNotiReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgNotiReq *primitive;

    primitive = (CsrBtAvrcpTgNotiReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->notiId);
    CsrMemCpySer(buffer, length, ((const void *) primitive->notiData), ((CsrSize) (8)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_REQ
void *CsrBtAvrcpTgNotiReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgNotiReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgNotiReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgNotiReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->notiId, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->notiData), buffer, &offset, ((CsrSize) (8)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_UIDS_IND
CsrSize CsrBtAvrcpCtNotiUidsIndSizeof(void *msg)
{
    CsrBtAvrcpCtNotiUidsInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_UIDS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_UIDS_IND
CsrUint8 *CsrBtAvrcpCtNotiUidsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtNotiUidsInd *primitive;

    primitive = (CsrBtAvrcpCtNotiUidsInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_UIDS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_UIDS_IND
void *CsrBtAvrcpCtNotiUidsIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtNotiUidsInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtNotiUidsInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtNotiUidsInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_UIDS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_CFM
CsrSize CsrBtAvrcpDeactivateCfmSizeof(void *msg)
{
    CsrBtAvrcpDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_CFM
CsrUint8 *CsrBtAvrcpDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpDeactivateCfm *primitive;

    primitive = (CsrBtAvrcpDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_CFM
void *CsrBtAvrcpDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_RES
CsrSize CsrBtAvrcpTgPasSetResSizeof(void *msg)
{
    CsrBtAvrcpTgPasSetRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_RES
CsrUint8 *CsrBtAvrcpTgPasSetResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgPasSetRes *primitive;

    primitive = (CsrBtAvrcpTgPasSetRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_RES
void *CsrBtAvrcpTgPasSetResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgPasSetRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgPasSetRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgPasSetRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM
CsrSize CsrBtAvrcpCtNotiRegisterCfmSizeof(void *msg)
{
    CsrBtAvrcpCtNotiRegisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM
CsrUint8 *CsrBtAvrcpCtNotiRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtNotiRegisterCfm *primitive;

    primitive = (CsrBtAvrcpCtNotiRegisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->notiMask);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playbackInterval);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM
void *CsrBtAvrcpCtNotiRegisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtNotiRegisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtNotiRegisterCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtNotiRegisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->notiMask, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playbackInterval, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_RES
CsrSize CsrBtAvrcpTgSetVolumeResSizeof(void *msg)
{
    CsrBtAvrcpTgSetVolumeRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_RES
CsrUint8 *CsrBtAvrcpTgSetVolumeResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgSetVolumeRes *primitive;

    primitive = (CsrBtAvrcpTgSetVolumeRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->volume);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_RES
void *CsrBtAvrcpTgSetVolumeResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgSetVolumeRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgSetVolumeRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgSetVolumeRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->volume, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_VOLUME_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_REQ
CsrSize CsrBtAvrcpDeactivateReqSizeof(void *msg)
{
    CsrBtAvrcpDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_REQ
CsrUint8 *CsrBtAvrcpDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpDeactivateReq *primitive;

    primitive = (CsrBtAvrcpDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_REQ
void *CsrBtAvrcpDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES
CsrSize CsrBtAvrcpTgGetFolderItemsResSizeof(void *msg)
{
    CsrBtAvrcpTgGetFolderItemsRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->itemsLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES
CsrUint8 *CsrBtAvrcpTgGetFolderItemsResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgGetFolderItemsRes *primitive;

    primitive = (CsrBtAvrcpTgGetFolderItemsRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->itemsCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->itemsLen);
    CsrMemCpySer(buffer, length, &primitive->items, sizeof(primitive->items));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    if(primitive->items)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->items), ((CsrSize) (primitive->itemsLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES
void *CsrBtAvrcpTgGetFolderItemsResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgGetFolderItemsRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgGetFolderItemsRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgGetFolderItemsRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->itemsCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->itemsLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->items), buffer, &offset, ((CsrSize) sizeof(primitive->items)));
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    if (primitive->itemsLen)
    {
        primitive->items = CsrPmemZalloc(((CsrUint32) (primitive->itemsLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->items), buffer, &offset, ((CsrSize) (primitive->itemsLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->items = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES
void CsrBtAvrcpTgGetFolderItemsResSerFree(void *msg)
{
    CsrBtAvrcpTgGetFolderItemsRes *primitive;

    primitive = (CsrBtAvrcpTgGetFolderItemsRes *) msg;

    if(primitive->items != NULL)
    {
        CsrPmemFree(primitive->items);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_CFM
CsrSize CsrBtAvrcpCtSearchCfmSizeof(void *msg)
{
    CsrBtAvrcpCtSearchCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_CFM
CsrUint8 *CsrBtAvrcpCtSearchCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtSearchCfm *primitive;

    primitive = (CsrBtAvrcpCtSearchCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->numberOfItems);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_CFM
void *CsrBtAvrcpCtSearchCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtSearchCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtSearchCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtSearchCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->numberOfItems, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND
CsrSize CsrBtAvrcpCtNotiAvailablePlayersIndSizeof(void *msg)
{
    CsrBtAvrcpCtNotiAvailablePlayersInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND
CsrUint8 *CsrBtAvrcpCtNotiAvailablePlayersIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtNotiAvailablePlayersInd *primitive;

    primitive = (CsrBtAvrcpCtNotiAvailablePlayersInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND
void *CsrBtAvrcpCtNotiAvailablePlayersIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtNotiAvailablePlayersInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtNotiAvailablePlayersInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtNotiAvailablePlayersInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_AVAILABLE_PLAYERS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND
CsrSize CsrBtAvrcpCtNotiPlaybackStatusIndSizeof(void *msg)
{
    CsrBtAvrcpCtNotiPlaybackStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND
CsrUint8 *CsrBtAvrcpCtNotiPlaybackStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtNotiPlaybackStatusInd *primitive;

    primitive = (CsrBtAvrcpCtNotiPlaybackStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->playbackStatus);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND
void *CsrBtAvrcpCtNotiPlaybackStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtNotiPlaybackStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtNotiPlaybackStatusInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtNotiPlaybackStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->playbackStatus, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_PLAYBACK_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND
CsrSize CsrBtAvrcpCtPasAttTxtIndSizeof(void *msg)
{
    CsrBtAvrcpCtPasAttTxtInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->pasDataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND
CsrUint8 *CsrBtAvrcpCtPasAttTxtIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasAttTxtInd *primitive;

    primitive = (CsrBtAvrcpCtPasAttTxtInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pasDataLen);
    CsrMemCpySer(buffer, length, &primitive->pasData, sizeof(primitive->pasData));
    if(primitive->pasData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->pasData), ((CsrSize) (primitive->pasDataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND
void *CsrBtAvrcpCtPasAttTxtIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasAttTxtInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasAttTxtInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasAttTxtInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pasDataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->pasData), buffer, &offset, ((CsrSize) sizeof(primitive->pasData)));
    if (primitive->pasDataLen)
    {
        primitive->pasData = CsrPmemZalloc(((CsrUint32) (primitive->pasDataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->pasData), buffer, &offset, ((CsrSize) (primitive->pasDataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->pasData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND
void CsrBtAvrcpCtPasAttTxtIndSerFree(void *msg)
{
    CsrBtAvrcpCtPasAttTxtInd *primitive;

    primitive = (CsrBtAvrcpCtPasAttTxtInd *) msg;

    if(primitive->pasData != NULL)
    {
        CsrPmemFree(primitive->pasData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_REMOTE_FEATURES_IND
CsrSize CsrBtAvrcpRemoteFeaturesIndSizeof(void *msg)
{
    CsrBtAvrcpRemoteFeaturesInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->tgFeatures.providerName) + CsrCharStringSerLen((CsrCharString *) primitive->tgFeatures.serviceName) + CsrCharStringSerLen((CsrCharString *) primitive->ctFeatures.providerName) + CsrCharStringSerLen((CsrCharString *) primitive->ctFeatures.serviceName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_REMOTE_FEATURES_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_REMOTE_FEATURES_IND
CsrUint8 *CsrBtAvrcpRemoteFeaturesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpRemoteFeaturesInd *primitive;

    primitive = (CsrBtAvrcpRemoteFeaturesInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->tgFeatures.roleSupported);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->tgFeatures.roleConfig);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->tgFeatures.srAvrcpVersion);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->tgFeatures.srFeatures);
    CsrMemCpySer(buffer, length, &primitive->tgFeatures.providerName, sizeof(primitive->tgFeatures.providerName));
    CsrMemCpySer(buffer, length, &primitive->tgFeatures.serviceName, sizeof(primitive->tgFeatures.serviceName));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ctFeatures.roleSupported);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->ctFeatures.roleConfig);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ctFeatures.srAvrcpVersion);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->ctFeatures.srFeatures);
    CsrMemCpySer(buffer, length, &primitive->ctFeatures.providerName, sizeof(primitive->ctFeatures.providerName));
    CsrMemCpySer(buffer, length, &primitive->ctFeatures.serviceName, sizeof(primitive->ctFeatures.serviceName));
    CsrCharStringSer(buffer, length, primitive->tgFeatures.providerName);
    CsrCharStringSer(buffer, length, primitive->tgFeatures.serviceName);
    CsrCharStringSer(buffer, length, primitive->ctFeatures.providerName);
    CsrCharStringSer(buffer, length, primitive->ctFeatures.serviceName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_REMOTE_FEATURES_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_REMOTE_FEATURES_IND
void *CsrBtAvrcpRemoteFeaturesIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpRemoteFeaturesInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpRemoteFeaturesInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpRemoteFeaturesInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->tgFeatures.roleSupported, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->tgFeatures.roleConfig, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->tgFeatures.srAvrcpVersion, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->tgFeatures.srFeatures, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->tgFeatures.providerName), buffer, &offset, ((CsrSize) sizeof(primitive->tgFeatures.providerName)));
    CsrMemCpyDes(((void *) &primitive->tgFeatures.serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->tgFeatures.serviceName)));
    CsrUint8Des((CsrUint8 *) &primitive->ctFeatures.roleSupported, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->ctFeatures.roleConfig, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ctFeatures.srAvrcpVersion, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->ctFeatures.srFeatures, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->ctFeatures.providerName), buffer, &offset, ((CsrSize) sizeof(primitive->ctFeatures.providerName)));
    CsrMemCpyDes(((void *) &primitive->ctFeatures.serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->ctFeatures.serviceName)));
    CsrCharStringDes(&primitive->tgFeatures.providerName, buffer, &offset);
    CsrCharStringDes(&primitive->tgFeatures.serviceName, buffer, &offset);
    CsrCharStringDes(&primitive->ctFeatures.providerName, buffer, &offset);
    CsrCharStringDes(&primitive->ctFeatures.serviceName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_REMOTE_FEATURES_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_REMOTE_FEATURES_IND
void CsrBtAvrcpRemoteFeaturesIndSerFree(void *msg)
{
    CsrBtAvrcpRemoteFeaturesInd *primitive;

    primitive = (CsrBtAvrcpRemoteFeaturesInd *) msg;

    if(primitive->tgFeatures.providerName != NULL)
    {
        CsrPmemFree(primitive->tgFeatures.providerName);
    }
    if(primitive->tgFeatures.serviceName != NULL)
    {
        CsrPmemFree(primitive->tgFeatures.serviceName);
    }
    if(primitive->ctFeatures.providerName != NULL)
    {
        CsrPmemFree(primitive->ctFeatures.providerName);
    }
    if(primitive->ctFeatures.serviceName != NULL)
    {
        CsrPmemFree(primitive->ctFeatures.serviceName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_REMOTE_FEATURES_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM
CsrSize CsrBtAvrcpCtGetAttributesCfmSizeof(void *msg)
{
    CsrBtAvrcpCtGetAttributesCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attribDataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM
CsrUint8 *CsrBtAvrcpCtGetAttributesCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtGetAttributesCfm *primitive;

    primitive = (CsrBtAvrcpCtGetAttributesCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scope);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uid), ((CsrSize) ((8))));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attributeCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attribDataLen);
    CsrMemCpySer(buffer, length, &primitive->attribData, sizeof(primitive->attribData));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attribDataPayloadOffset);
    if(primitive->attribData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attribData), ((CsrSize) (primitive->attribDataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM
void *CsrBtAvrcpCtGetAttributesCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtGetAttributesCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtGetAttributesCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtGetAttributesCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scope, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uid), buffer, &offset, ((CsrSize) ((8))));
    CsrUint8Des((CsrUint8 *) &primitive->attributeCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attribDataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attribData), buffer, &offset, ((CsrSize) sizeof(primitive->attribData)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attribDataPayloadOffset, buffer, &offset);
    if (primitive->attribDataLen)
    {
        primitive->attribData = CsrPmemZalloc(((CsrUint32) (primitive->attribDataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->attribData), buffer, &offset, ((CsrSize) (primitive->attribDataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->attribData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM
void CsrBtAvrcpCtGetAttributesCfmSerFree(void *msg)
{
    CsrBtAvrcpCtGetAttributesCfm *primitive;

    primitive = (CsrBtAvrcpCtGetAttributesCfm *) msg;

    if(primitive->attribData != NULL)
    {
        CsrPmemFree(primitive->attribData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_RES
CsrSize CsrBtAvrcpCtPasAttTxtResSizeof(void *msg)
{
    CsrBtAvrcpCtPasAttTxtRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_RES
CsrUint8 *CsrBtAvrcpCtPasAttTxtResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasAttTxtRes *primitive;

    primitive = (CsrBtAvrcpCtPasAttTxtRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->proceed);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_RES
void *CsrBtAvrcpCtPasAttTxtResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasAttTxtRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasAttTxtRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasAttTxtRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->proceed, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND
CsrSize CsrBtAvrcpCtNotiTrackChangedIndSizeof(void *msg)
{
    CsrBtAvrcpCtNotiTrackChangedInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND
CsrUint8 *CsrBtAvrcpCtNotiTrackChangedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtNotiTrackChangedInd *primitive;

    primitive = (CsrBtAvrcpCtNotiTrackChangedInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrMemCpySer(buffer, length, ((const void *) primitive->trackUid), ((CsrSize) ((8))));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND
void *CsrBtAvrcpCtNotiTrackChangedIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtNotiTrackChangedInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtNotiTrackChangedInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtNotiTrackChangedInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->trackUid), buffer, &offset, ((CsrSize) ((8))));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_CHANGED_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_IND
CsrSize CsrBtAvrcpTgGetFolderItemsIndSizeof(void *msg)
{
    CsrBtAvrcpTgGetFolderItemsInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_IND
CsrUint8 *CsrBtAvrcpTgGetFolderItemsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgGetFolderItemsInd *primitive;

    primitive = (CsrBtAvrcpTgGetFolderItemsInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scope);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->startItem);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->endItem);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->attributeMask);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->maxData);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_IND
void *CsrBtAvrcpTgGetFolderItemsIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgGetFolderItemsInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgGetFolderItemsInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgGetFolderItemsInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scope, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->startItem, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->endItem, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->attributeMask, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->maxData, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_FOLDER_ITEMS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ
CsrSize CsrBtAvrcpCtUnitInfoCmdReqSizeof(void *msg)
{
    CsrBtAvrcpCtUnitInfoCmdReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->pDataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ
CsrUint8 *CsrBtAvrcpCtUnitInfoCmdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtUnitInfoCmdReq *primitive;

    primitive = (CsrBtAvrcpCtUnitInfoCmdReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pDataLen);
    CsrMemCpySer(buffer, length, &primitive->pData, sizeof(primitive->pData));
    if(primitive->pData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->pData), ((CsrSize) (primitive->pDataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ
void *CsrBtAvrcpCtUnitInfoCmdReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtUnitInfoCmdReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtUnitInfoCmdReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtUnitInfoCmdReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pDataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->pData), buffer, &offset, ((CsrSize) sizeof(primitive->pData)));
    if (primitive->pDataLen)
    {
        primitive->pData = CsrPmemZalloc(((CsrUint32) (primitive->pDataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->pData), buffer, &offset, ((CsrSize) (primitive->pDataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->pData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ
void CsrBtAvrcpCtUnitInfoCmdReqSerFree(void *msg)
{
    CsrBtAvrcpCtUnitInfoCmdReq *primitive;

    primitive = (CsrBtAvrcpCtUnitInfoCmdReq *) msg;

    if(primitive->pData != NULL)
    {
        CsrPmemFree(primitive->pData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_RES
CsrSize CsrBtAvrcpTgChangePathResSizeof(void *msg)
{
    CsrBtAvrcpTgChangePathRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_RES
CsrUint8 *CsrBtAvrcpTgChangePathResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgChangePathRes *primitive;

    primitive = (CsrBtAvrcpTgChangePathRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->itemsCount);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_RES
void *CsrBtAvrcpTgChangePathResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgChangePathRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgChangePathRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgChangePathRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->itemsCount, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_CHANGE_PATH_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_RES
CsrSize CsrBtAvrcpTgPasCurrentResSizeof(void *msg)
{
    CsrBtAvrcpTgPasCurrentRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_RES
CsrUint8 *CsrBtAvrcpTgPasCurrentResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgPasCurrentRes *primitive;

    primitive = (CsrBtAvrcpTgPasCurrentRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attValPairCount);
    CsrMemCpySer(buffer, length, &primitive->attValPair, sizeof(primitive->attValPair));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    if(primitive->attValPair)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attValPair), ((CsrSize) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_RES
void *CsrBtAvrcpTgPasCurrentResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgPasCurrentRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgPasCurrentRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgPasCurrentRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->attValPairCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attValPair), buffer, &offset, ((CsrSize) sizeof(primitive->attValPair)));
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    if (primitive->attValPairCount)
    {
        primitive->attValPair = CsrPmemZalloc(((CsrUint32) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
        CsrMemCpyDes(((void *) primitive->attValPair), buffer, &offset, ((CsrSize) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
    }
    else
    {
        primitive->attValPair = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_RES
void CsrBtAvrcpTgPasCurrentResSerFree(void *msg)
{
    CsrBtAvrcpTgPasCurrentRes *primitive;

    primitive = (CsrBtAvrcpTgPasCurrentRes *) msg;

    if(primitive->attValPair != NULL)
    {
        CsrPmemFree(primitive->attValPair);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_CURRENT_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_RES
CsrSize CsrBtAvrcpTgSetAddressedPlayerResSizeof(void *msg)
{
    CsrBtAvrcpTgSetAddressedPlayerRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_RES
CsrUint8 *CsrBtAvrcpTgSetAddressedPlayerResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgSetAddressedPlayerRes *primitive;

    primitive = (CsrBtAvrcpTgSetAddressedPlayerRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_RES
void *CsrBtAvrcpTgSetAddressedPlayerResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgSetAddressedPlayerRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgSetAddressedPlayerRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgSetAddressedPlayerRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_RES
CsrSize CsrBtAvrcpTgSearchResSizeof(void *msg)
{
    CsrBtAvrcpTgSearchRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_RES
CsrUint8 *CsrBtAvrcpTgSearchResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgSearchRes *primitive;

    primitive = (CsrBtAvrcpTgSearchRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->numberOfItems);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_RES
void *CsrBtAvrcpTgSearchResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgSearchRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgSearchRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgSearchRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->numberOfItems, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_CFM
CsrSize CsrBtAvrcpSecurityInCfmSizeof(void *msg)
{
    CsrBtAvrcpSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_CFM
CsrUint8 *CsrBtAvrcpSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpSecurityInCfm *primitive;

    primitive = (CsrBtAvrcpSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_CFM
void *CsrBtAvrcpSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM
CsrSize CsrBtAvrcpCtSetBrowsedPlayerCfmSizeof(void *msg)
{
    CsrBtAvrcpCtSetBrowsedPlayerCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->folderNamesLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM
CsrUint8 *CsrBtAvrcpCtSetBrowsedPlayerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtSetBrowsedPlayerCfm *primitive;

    primitive = (CsrBtAvrcpCtSetBrowsedPlayerCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->itemsCount);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->folderDepth);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->folderNamesLen);
    CsrMemCpySer(buffer, length, &primitive->folderNames, sizeof(primitive->folderNames));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    if(primitive->folderNames)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->folderNames), ((CsrSize) (primitive->folderNamesLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM
void *CsrBtAvrcpCtSetBrowsedPlayerCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtSetBrowsedPlayerCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtSetBrowsedPlayerCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtSetBrowsedPlayerCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->itemsCount, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->folderDepth, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->folderNamesLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->folderNames), buffer, &offset, ((CsrSize) sizeof(primitive->folderNames)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    if (primitive->folderNamesLen)
    {
        primitive->folderNames = CsrPmemZalloc(((CsrUint32) (primitive->folderNamesLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->folderNames), buffer, &offset, ((CsrSize) (primitive->folderNamesLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->folderNames = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM
void CsrBtAvrcpCtSetBrowsedPlayerCfmSerFree(void *msg)
{
    CsrBtAvrcpCtSetBrowsedPlayerCfm *primitive;

    primitive = (CsrBtAvrcpCtSetBrowsedPlayerCfm *) msg;

    if(primitive->folderNames != NULL)
    {
        CsrPmemFree(primitive->folderNames);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_BROWSED_PLAYER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_CFM
CsrSize CsrBtAvrcpCtChangePathCfmSizeof(void *msg)
{
    CsrBtAvrcpCtChangePathCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_CFM
CsrUint8 *CsrBtAvrcpCtChangePathCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtChangePathCfm *primitive;

    primitive = (CsrBtAvrcpCtChangePathCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->itemsCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_CFM
void *CsrBtAvrcpCtChangePathCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtChangePathCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtChangePathCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtChangePathCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->itemsCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_CHANGE_PATH_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND
CsrSize CsrBtAvrcpTgInformDispCharsetIndSizeof(void *msg)
{
    CsrBtAvrcpTgInformDispCharsetInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->charsetCount * sizeof(CsrBtAvrcpCharSet)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND
CsrUint8 *CsrBtAvrcpTgInformDispCharsetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgInformDispCharsetInd *primitive;

    primitive = (CsrBtAvrcpTgInformDispCharsetInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->charsetCount);
    CsrMemCpySer(buffer, length, &primitive->charset, sizeof(primitive->charset));
    if(primitive->charset)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->charset), ((CsrSize) (primitive->charsetCount * sizeof(CsrBtAvrcpCharSet))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND
void *CsrBtAvrcpTgInformDispCharsetIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgInformDispCharsetInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgInformDispCharsetInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgInformDispCharsetInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->charsetCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->charset), buffer, &offset, ((CsrSize) sizeof(primitive->charset)));
    if (primitive->charsetCount)
    {
        primitive->charset = CsrPmemZalloc(((CsrUint32) (primitive->charsetCount * sizeof(CsrBtAvrcpCharSet))));
        CsrMemCpyDes(((void *) primitive->charset), buffer, &offset, ((CsrSize) (primitive->charsetCount * sizeof(CsrBtAvrcpCharSet))));
    }
    else
    {
        primitive->charset = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND
void CsrBtAvrcpTgInformDispCharsetIndSerFree(void *msg)
{
    CsrBtAvrcpTgInformDispCharsetInd *primitive;

    primitive = (CsrBtAvrcpTgInformDispCharsetInd *) msg;

    if(primitive->charset != NULL)
    {
        CsrPmemFree(primitive->charset);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_INFORM_DISP_CHARSET_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_IND
CsrSize CsrBtAvrcpCtPasSetIndSizeof(void *msg)
{
    CsrBtAvrcpCtPasSetInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_IND
CsrUint8 *CsrBtAvrcpCtPasSetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasSetInd *primitive;

    primitive = (CsrBtAvrcpCtPasSetInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attValPairCount);
    CsrMemCpySer(buffer, length, &primitive->attValPair, sizeof(primitive->attValPair));
    if(primitive->attValPair)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attValPair), ((CsrSize) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_IND
void *CsrBtAvrcpCtPasSetIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasSetInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasSetInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasSetInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->attValPairCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attValPair), buffer, &offset, ((CsrSize) sizeof(primitive->attValPair)));
    if (primitive->attValPairCount)
    {
        primitive->attValPair = CsrPmemZalloc(((CsrUint32) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
        CsrMemCpyDes(((void *) primitive->attValPair), buffer, &offset, ((CsrSize) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
    }
    else
    {
        primitive->attValPair = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_IND
void CsrBtAvrcpCtPasSetIndSerFree(void *msg)
{
    CsrBtAvrcpCtPasSetInd *primitive;

    primitive = (CsrBtAvrcpCtPasSetInd *) msg;

    if(primitive->attValPair != NULL)
    {
        CsrPmemFree(primitive->attValPair);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_ACTIVATE_CFM
CsrSize CsrBtAvrcpActivateCfmSizeof(void *msg)
{
    CsrBtAvrcpActivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_ACTIVATE_CFM
CsrUint8 *CsrBtAvrcpActivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpActivateCfm *primitive;

    primitive = (CsrBtAvrcpActivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_ACTIVATE_CFM
void *CsrBtAvrcpActivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpActivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpActivateCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpActivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_ACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_IND
CsrSize CsrBtAvrcpConnectIndSizeof(void *msg)
{
    CsrBtAvrcpConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_IND
CsrUint8 *CsrBtAvrcpConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpConnectInd *primitive;

    primitive = (CsrBtAvrcpConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_IND
void *CsrBtAvrcpConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpConnectInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM
CsrSize CsrBtAvrcpCtUnitInfoCmdCfmSizeof(void *msg)
{
    CsrBtAvrcpCtUnitInfoCmdCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->pDataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM
CsrUint8 *CsrBtAvrcpCtUnitInfoCmdCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtUnitInfoCmdCfm *primitive;

    primitive = (CsrBtAvrcpCtUnitInfoCmdCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pDataLen);
    CsrMemCpySer(buffer, length, &primitive->pData, sizeof(primitive->pData));
    if(primitive->pData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->pData), ((CsrSize) (primitive->pDataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM
void *CsrBtAvrcpCtUnitInfoCmdCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtUnitInfoCmdCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtUnitInfoCmdCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtUnitInfoCmdCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pDataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->pData), buffer, &offset, ((CsrSize) sizeof(primitive->pData)));
    if (primitive->pDataLen)
    {
        primitive->pData = CsrPmemZalloc(((CsrUint32) (primitive->pDataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->pData), buffer, &offset, ((CsrSize) (primitive->pDataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->pData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM
void CsrBtAvrcpCtUnitInfoCmdCfmSerFree(void *msg)
{
    CsrBtAvrcpCtUnitInfoCmdCfm *primitive;

    primitive = (CsrBtAvrcpCtUnitInfoCmdCfm *) msg;

    if(primitive->pData != NULL)
    {
        CsrPmemFree(primitive->pData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_UNIT_INFO_CMD_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM
CsrSize CsrBtAvrcpCtPasValTxtCfmSizeof(void *msg)
{
    CsrBtAvrcpCtPasValTxtCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->pasDataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM
CsrUint8 *CsrBtAvrcpCtPasValTxtCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasValTxtCfm *primitive;

    primitive = (CsrBtAvrcpCtPasValTxtCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pasDataLen);
    CsrMemCpySer(buffer, length, &primitive->pasData, sizeof(primitive->pasData));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    if(primitive->pasData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->pasData), ((CsrSize) (primitive->pasDataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM
void *CsrBtAvrcpCtPasValTxtCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasValTxtCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasValTxtCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasValTxtCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->attId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pasDataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->pasData), buffer, &offset, ((CsrSize) sizeof(primitive->pasData)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    if (primitive->pasDataLen)
    {
        primitive->pasData = CsrPmemZalloc(((CsrUint32) (primitive->pasDataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->pasData), buffer, &offset, ((CsrSize) (primitive->pasDataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->pasData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM
void CsrBtAvrcpCtPasValTxtCfmSerFree(void *msg)
{
    CsrBtAvrcpCtPasValTxtCfm *primitive;

    primitive = (CsrBtAvrcpCtPasValTxtCfm *) msg;

    if(primitive->pasData != NULL)
    {
        CsrPmemFree(primitive->pasData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_TXT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM
CsrSize CsrBtAvrcpCtGetFolderItemsCfmSizeof(void *msg)
{
    CsrBtAvrcpCtGetFolderItemsCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->itemsDataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM
CsrUint8 *CsrBtAvrcpCtGetFolderItemsCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtGetFolderItemsCfm *primitive;

    primitive = (CsrBtAvrcpCtGetFolderItemsCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scope);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->startItem);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->endItem);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->itemsCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->itemsDataLen);
    CsrMemCpySer(buffer, length, &primitive->itemsData, sizeof(primitive->itemsData));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    if(primitive->itemsData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->itemsData), ((CsrSize) (primitive->itemsDataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM
void *CsrBtAvrcpCtGetFolderItemsCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtGetFolderItemsCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtGetFolderItemsCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtGetFolderItemsCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scope, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->startItem, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->endItem, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->itemsCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->itemsDataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->itemsData), buffer, &offset, ((CsrSize) sizeof(primitive->itemsData)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    if (primitive->itemsDataLen)
    {
        primitive->itemsData = CsrPmemZalloc(((CsrUint32) (primitive->itemsDataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->itemsData), buffer, &offset, ((CsrSize) (primitive->itemsDataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->itemsData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM
void CsrBtAvrcpCtGetFolderItemsCfmSerFree(void *msg)
{
    CsrBtAvrcpCtGetFolderItemsCfm *primitive;

    primitive = (CsrBtAvrcpCtGetFolderItemsCfm *) msg;

    if(primitive->itemsData != NULL)
    {
        CsrPmemFree(primitive->itemsData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_FOLDER_ITEMS_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM
CsrSize CsrBtAvrcpCtPasAttIdCfmSizeof(void *msg)
{
    CsrBtAvrcpCtPasAttIdCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attIdCount * sizeof(CsrBtAvrcpPasAttId)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM
CsrUint8 *CsrBtAvrcpCtPasAttIdCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasAttIdCfm *primitive;

    primitive = (CsrBtAvrcpCtPasAttIdCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attIdCount);
    CsrMemCpySer(buffer, length, &primitive->attId, sizeof(primitive->attId));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    if(primitive->attId)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attId), ((CsrSize) (primitive->attIdCount * sizeof(CsrBtAvrcpPasAttId))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM
void *CsrBtAvrcpCtPasAttIdCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasAttIdCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasAttIdCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasAttIdCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->attIdCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attId), buffer, &offset, ((CsrSize) sizeof(primitive->attId)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    if (primitive->attIdCount)
    {
        primitive->attId = CsrPmemZalloc(((CsrUint32) (primitive->attIdCount * sizeof(CsrBtAvrcpPasAttId))));
        CsrMemCpyDes(((void *) primitive->attId), buffer, &offset, ((CsrSize) (primitive->attIdCount * sizeof(CsrBtAvrcpPasAttId))));
    }
    else
    {
        primitive->attId = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM
void CsrBtAvrcpCtPasAttIdCfmSerFree(void *msg)
{
    CsrBtAvrcpCtPasAttIdCfm *primitive;

    primitive = (CsrBtAvrcpCtPasAttIdCfm *) msg;

    if(primitive->attId != NULL)
    {
        CsrPmemFree(primitive->attId);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_CFM
CsrSize CsrBtAvrcpCtPasCurrentCfmSizeof(void *msg)
{
    CsrBtAvrcpCtPasCurrentCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_CFM
CsrUint8 *CsrBtAvrcpCtPasCurrentCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasCurrentCfm *primitive;

    primitive = (CsrBtAvrcpCtPasCurrentCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attValPairCount);
    CsrMemCpySer(buffer, length, &primitive->attValPair, sizeof(primitive->attValPair));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    if(primitive->attValPair)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attValPair), ((CsrSize) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_CFM
void *CsrBtAvrcpCtPasCurrentCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasCurrentCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasCurrentCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasCurrentCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->attValPairCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attValPair), buffer, &offset, ((CsrSize) sizeof(primitive->attValPair)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    if (primitive->attValPairCount)
    {
        primitive->attValPair = CsrPmemZalloc(((CsrUint32) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
        CsrMemCpyDes(((void *) primitive->attValPair), buffer, &offset, ((CsrSize) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
    }
    else
    {
        primitive->attValPair = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_CFM
void CsrBtAvrcpCtPasCurrentCfmSerFree(void *msg)
{
    CsrBtAvrcpCtPasCurrentCfm *primitive;

    primitive = (CsrBtAvrcpCtPasCurrentCfm *) msg;

    if(primitive->attValPair != NULL)
    {
        CsrPmemFree(primitive->attValPair);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_CFM
CsrSize CsrBtAvrcpTgMpUnregisterCfmSizeof(void *msg)
{
    CsrBtAvrcpTgMpUnregisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_CFM
CsrUint8 *CsrBtAvrcpTgMpUnregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgMpUnregisterCfm *primitive;

    primitive = (CsrBtAvrcpTgMpUnregisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_CFM
void *CsrBtAvrcpTgMpUnregisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgMpUnregisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgMpUnregisterCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgMpUnregisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_HOUSE_CLEANING
CsrSize CsrBtAvrcpHouseCleaningSizeof(void *msg)
{
    CsrBtAvrcpHouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_AVRCP_HOUSE_CLEANING
CsrUint8 *CsrBtAvrcpHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpHouseCleaning *primitive;

    primitive = (CsrBtAvrcpHouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_AVRCP_HOUSE_CLEANING
void *CsrBtAvrcpHouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpHouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpHouseCleaning *) CsrPmemZalloc(sizeof(CsrBtAvrcpHouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_REQ
CsrSize CsrBtAvrcpConnectReqSizeof(void *msg)
{
    CsrBtAvrcpConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_REQ
CsrUint8 *CsrBtAvrcpConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpConnectReq *primitive;

    primitive = (CsrBtAvrcpConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CONNECT_REQ
void *CsrBtAvrcpConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpConnectReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_REQ
CsrSize CsrBtAvrcpCtSearchReqSizeof(void *msg)
{
    CsrBtAvrcpCtSearchReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->text));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_REQ
CsrUint8 *CsrBtAvrcpCtSearchReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtSearchReq *primitive;

    primitive = (CsrBtAvrcpCtSearchReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrMemCpySer(buffer, length, &primitive->text, sizeof(primitive->text));
    CsrUtf8StringSer(buffer, length, primitive->text);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_REQ
void *CsrBtAvrcpCtSearchReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtSearchReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtSearchReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtSearchReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->text), buffer, &offset, ((CsrSize) sizeof(primitive->text)));
    CsrUtf8StringDes(&primitive->text, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_REQ
void CsrBtAvrcpCtSearchReqSerFree(void *msg)
{
    CsrBtAvrcpCtSearchReq *primitive;

    primitive = (CsrBtAvrcpCtSearchReq *) msg;

    if(primitive->text != NULL)
    {
        CsrPmemFree(primitive->text);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SEARCH_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_REQ
CsrSize CsrBtAvrcpSecurityInReqSizeof(void *msg)
{
    CsrBtAvrcpSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_REQ
CsrUint8 *CsrBtAvrcpSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpSecurityInReq *primitive;

    primitive = (CsrBtAvrcpSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->config);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_REQ
void *CsrBtAvrcpSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->config, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_CFM
CsrSize CsrBtAvrcpTgPasSetCfmSizeof(void *msg)
{
    CsrBtAvrcpTgPasSetCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_CFM
CsrUint8 *CsrBtAvrcpTgPasSetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgPasSetCfm *primitive;

    primitive = (CsrBtAvrcpTgPasSetCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_CFM
void *CsrBtAvrcpTgPasSetCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgPasSetCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgPasSetCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgPasSetCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PAS_SET_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_DISCONNECT_IND
CsrSize CsrBtAvrcpDisconnectIndSizeof(void *msg)
{
    CsrBtAvrcpDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_DISCONNECT_IND
CsrUint8 *CsrBtAvrcpDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpDisconnectInd *primitive;

    primitive = (CsrBtAvrcpDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localTerminated);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_DISCONNECT_IND
void *CsrBtAvrcpDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localTerminated, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ
CsrSize CsrBtAvrcpCtSubUnitInfoCmdReqSizeof(void *msg)
{
    CsrBtAvrcpCtSubUnitInfoCmdReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->pDataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ
CsrUint8 *CsrBtAvrcpCtSubUnitInfoCmdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtSubUnitInfoCmdReq *primitive;

    primitive = (CsrBtAvrcpCtSubUnitInfoCmdReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pDataLen);
    CsrMemCpySer(buffer, length, &primitive->pData, sizeof(primitive->pData));
    if(primitive->pData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->pData), ((CsrSize) (primitive->pDataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ
void *CsrBtAvrcpCtSubUnitInfoCmdReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtSubUnitInfoCmdReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtSubUnitInfoCmdReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtSubUnitInfoCmdReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pDataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->pData), buffer, &offset, ((CsrSize) sizeof(primitive->pData)));
    if (primitive->pDataLen)
    {
        primitive->pData = CsrPmemZalloc(((CsrUint32) (primitive->pDataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->pData), buffer, &offset, ((CsrSize) (primitive->pDataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->pData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ
void CsrBtAvrcpCtSubUnitInfoCmdReqSerFree(void *msg)
{
    CsrBtAvrcpCtSubUnitInfoCmdReq *primitive;

    primitive = (CsrBtAvrcpCtSubUnitInfoCmdReq *) msg;

    if(primitive->pData != NULL)
    {
        CsrPmemFree(primitive->pData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_END_IND
CsrSize CsrBtAvrcpCtNotiTrackEndIndSizeof(void *msg)
{
    CsrBtAvrcpCtNotiTrackEndInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_END_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_END_IND
CsrUint8 *CsrBtAvrcpCtNotiTrackEndIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtNotiTrackEndInd *primitive;

    primitive = (CsrBtAvrcpCtNotiTrackEndInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_END_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_END_IND
void *CsrBtAvrcpCtNotiTrackEndIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtNotiTrackEndInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtNotiTrackEndInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtNotiTrackEndInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_END_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_CFM
CsrSize CsrBtAvrcpTgMpRegisterCfmSizeof(void *msg)
{
    CsrBtAvrcpTgMpRegisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_CFM
CsrUint8 *CsrBtAvrcpTgMpRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgMpRegisterCfm *primitive;

    primitive = (CsrBtAvrcpTgMpRegisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_CFM
void *CsrBtAvrcpTgMpRegisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgMpRegisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgMpRegisterCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgMpRegisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_MP_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_IND
CsrSize CsrBtAvrcpTgNotiIndSizeof(void *msg)
{
    CsrBtAvrcpTgNotiInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_IND
CsrUint8 *CsrBtAvrcpTgNotiIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgNotiInd *primitive;

    primitive = (CsrBtAvrcpTgNotiInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->notiId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playbackInterval);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_IND
void *CsrBtAvrcpTgNotiIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgNotiInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgNotiInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgNotiInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->notiId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playbackInterval, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_IND
CsrSize CsrBtAvrcpTgSearchIndSizeof(void *msg)
{
    CsrBtAvrcpTgSearchInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->text));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_IND
CsrUint8 *CsrBtAvrcpTgSearchIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgSearchInd *primitive;

    primitive = (CsrBtAvrcpTgSearchInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrMemCpySer(buffer, length, &primitive->text, sizeof(primitive->text));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);
    CsrCharStringSer(buffer, length, primitive->text);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_IND
void *CsrBtAvrcpTgSearchIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgSearchInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgSearchInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgSearchInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->text), buffer, &offset, ((CsrSize) sizeof(primitive->text)));
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);
    CsrCharStringDes(&primitive->text, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_IND
void CsrBtAvrcpTgSearchIndSerFree(void *msg)
{
    CsrBtAvrcpTgSearchInd *primitive;

    primitive = (CsrBtAvrcpTgSearchInd *) msg;

    if(primitive->text != NULL)
    {
        CsrPmemFree(primitive->text);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SEARCH_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_REQ
CsrSize CsrBtAvrcpCtPasCurrentReqSizeof(void *msg)
{
    CsrBtAvrcpCtPasCurrentReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attIdCount * sizeof(CsrBtAvrcpPasAttId)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_REQ
CsrUint8 *CsrBtAvrcpCtPasCurrentReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasCurrentReq *primitive;

    primitive = (CsrBtAvrcpCtPasCurrentReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attIdCount);
    CsrMemCpySer(buffer, length, &primitive->attId, sizeof(primitive->attId));
    if(primitive->attId)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attId), ((CsrSize) (primitive->attIdCount * sizeof(CsrBtAvrcpPasAttId))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_REQ
void *CsrBtAvrcpCtPasCurrentReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasCurrentReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasCurrentReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasCurrentReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->attIdCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attId), buffer, &offset, ((CsrSize) sizeof(primitive->attId)));
    if (primitive->attIdCount)
    {
        primitive->attId = CsrPmemZalloc(((CsrUint32) (primitive->attIdCount * sizeof(CsrBtAvrcpPasAttId))));
        CsrMemCpyDes(((void *) primitive->attId), buffer, &offset, ((CsrSize) (primitive->attIdCount * sizeof(CsrBtAvrcpPasAttId))));
    }
    else
    {
        primitive->attId = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_REQ
void CsrBtAvrcpCtPasCurrentReqSerFree(void *msg)
{
    CsrBtAvrcpCtPasCurrentReq *primitive;

    primitive = (CsrBtAvrcpCtPasCurrentReq *) msg;

    if(primitive->attId != NULL)
    {
        CsrPmemFree(primitive->attId);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_CURRENT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_START_IND
CsrSize CsrBtAvrcpCtNotiTrackStartIndSizeof(void *msg)
{
    CsrBtAvrcpCtNotiTrackStartInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_START_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_START_IND
CsrUint8 *CsrBtAvrcpCtNotiTrackStartIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtNotiTrackStartInd *primitive;

    primitive = (CsrBtAvrcpCtNotiTrackStartInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_START_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_START_IND
void *CsrBtAvrcpCtNotiTrackStartIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtNotiTrackStartInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtNotiTrackStartInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtNotiTrackStartInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_TRACK_START_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_RES
CsrSize CsrBtAvrcpCtGetAttributesResSizeof(void *msg)
{
    CsrBtAvrcpCtGetAttributesRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_RES
CsrUint8 *CsrBtAvrcpCtGetAttributesResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtGetAttributesRes *primitive;

    primitive = (CsrBtAvrcpCtGetAttributesRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->proceed);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_RES
void *CsrBtAvrcpCtGetAttributesResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtGetAttributesRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtGetAttributesRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtGetAttributesRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->proceed, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_REQ
CsrSize CsrBtAvrcpCtPasAttIdReqSizeof(void *msg)
{
    CsrBtAvrcpCtPasAttIdReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_REQ
CsrUint8 *CsrBtAvrcpCtPasAttIdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasAttIdReq *primitive;

    primitive = (CsrBtAvrcpCtPasAttIdReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_REQ
void *CsrBtAvrcpCtPasAttIdReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasAttIdReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasAttIdReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasAttIdReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_ID_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_IND
CsrSize CsrBtAvrcpTgSetAddressedPlayerIndSizeof(void *msg)
{
    CsrBtAvrcpTgSetAddressedPlayerInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_IND
CsrUint8 *CsrBtAvrcpTgSetAddressedPlayerIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgSetAddressedPlayerInd *primitive;

    primitive = (CsrBtAvrcpTgSetAddressedPlayerInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_IND
void *CsrBtAvrcpTgSetAddressedPlayerIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgSetAddressedPlayerInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgSetAddressedPlayerInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgSetAddressedPlayerInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_ADDRESSED_PLAYER_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_REQ
CsrSize CsrBtAvrcpCtAddToNowPlayingReqSizeof(void *msg)
{
    CsrBtAvrcpCtAddToNowPlayingReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_REQ
CsrUint8 *CsrBtAvrcpCtAddToNowPlayingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtAddToNowPlayingReq *primitive;

    primitive = (CsrBtAvrcpCtAddToNowPlayingReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scope);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uid), ((CsrSize) ((8))));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_REQ
void *CsrBtAvrcpCtAddToNowPlayingReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtAddToNowPlayingReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtAddToNowPlayingReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtAddToNowPlayingReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scope, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uid), buffer, &offset, ((CsrSize) ((8))));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_ADD_TO_NOW_PLAYING_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_IND
CsrSize CsrBtAvrcpTgAddToNowPlayingIndSizeof(void *msg)
{
    CsrBtAvrcpTgAddToNowPlayingInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_IND
CsrUint8 *CsrBtAvrcpTgAddToNowPlayingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgAddToNowPlayingInd *primitive;

    primitive = (CsrBtAvrcpTgAddToNowPlayingInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scope);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uid), ((CsrSize) ((8))));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_IND
void *CsrBtAvrcpTgAddToNowPlayingIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgAddToNowPlayingInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgAddToNowPlayingInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgAddToNowPlayingInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scope, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uid), buffer, &offset, ((CsrSize) ((8))));
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_ADD_TO_NOW_PLAYING_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND
CsrSize CsrBtAvrcpCtNotiSystemStatusIndSizeof(void *msg)
{
    CsrBtAvrcpCtNotiSystemStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND
CsrUint8 *CsrBtAvrcpCtNotiSystemStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtNotiSystemStatusInd *primitive;

    primitive = (CsrBtAvrcpCtNotiSystemStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->systemStatus);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND
void *CsrBtAvrcpCtNotiSystemStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtNotiSystemStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtNotiSystemStatusInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtNotiSystemStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->systemStatus, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_NOTI_SYSTEM_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND
CsrSize CsrBtAvrcpCtGetAttributesIndSizeof(void *msg)
{
    CsrBtAvrcpCtGetAttributesInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attribDataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND
CsrUint8 *CsrBtAvrcpCtGetAttributesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtGetAttributesInd *primitive;

    primitive = (CsrBtAvrcpCtGetAttributesInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scope);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uid), ((CsrSize) ((8))));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attributeCount);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attribDataLen);
    CsrMemCpySer(buffer, length, &primitive->attribData, sizeof(primitive->attribData));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->attribDataPayloadOffset);
    if(primitive->attribData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attribData), ((CsrSize) (primitive->attribDataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND
void *CsrBtAvrcpCtGetAttributesIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtGetAttributesInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtGetAttributesInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtGetAttributesInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->scope, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uid), buffer, &offset, ((CsrSize) ((8))));
    CsrUint8Des((CsrUint8 *) &primitive->attributeCount, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->attribDataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attribData), buffer, &offset, ((CsrSize) sizeof(primitive->attribData)));
    CsrUint16Des((CsrUint16 *) &primitive->attribDataPayloadOffset, buffer, &offset);
    if (primitive->attribDataLen)
    {
        primitive->attribData = CsrPmemZalloc(((CsrUint32) (primitive->attribDataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->attribData), buffer, &offset, ((CsrSize) (primitive->attribDataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->attribData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND
void CsrBtAvrcpCtGetAttributesIndSerFree(void *msg)
{
    CsrBtAvrcpCtGetAttributesInd *primitive;

    primitive = (CsrBtAvrcpCtGetAttributesInd *) msg;

    if(primitive->attribData != NULL)
    {
        CsrPmemFree(primitive->attribData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_GET_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_IND
CsrSize CsrBtAvrcpTgGetAttributesIndSizeof(void *msg)
{
    CsrBtAvrcpTgGetAttributesInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_IND
CsrUint8 *CsrBtAvrcpTgGetAttributesIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgGetAttributesInd *primitive;

    primitive = (CsrBtAvrcpTgGetAttributesInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uid), ((CsrSize) ((8))));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scope);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->attributeMask);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->maxData);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_IND
void *CsrBtAvrcpTgGetAttributesIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgGetAttributesInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgGetAttributesInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgGetAttributesInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uid), buffer, &offset, ((CsrSize) ((8))));
    CsrUint8Des((CsrUint8 *) &primitive->scope, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->attributeMask, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->maxData, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_GET_ATTRIBUTES_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PLAY_CFM
CsrSize CsrBtAvrcpCtPlayCfmSizeof(void *msg)
{
    CsrBtAvrcpCtPlayCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PLAY_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PLAY_CFM
CsrUint8 *CsrBtAvrcpCtPlayCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPlayCfm *primitive;

    primitive = (CsrBtAvrcpCtPlayCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uid), ((CsrSize) ((8))));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scope);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PLAY_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PLAY_CFM
void *CsrBtAvrcpCtPlayCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPlayCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPlayCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPlayCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uid), buffer, &offset, ((CsrSize) ((8))));
    CsrUint8Des((CsrUint8 *) &primitive->scope, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PLAY_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_REQ
CsrSize CsrBtAvrcpCtSetVolumeReqSizeof(void *msg)
{
    CsrBtAvrcpCtSetVolumeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_REQ
CsrUint8 *CsrBtAvrcpCtSetVolumeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtSetVolumeReq *primitive;

    primitive = (CsrBtAvrcpCtSetVolumeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->volume);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_REQ
void *CsrBtAvrcpCtSetVolumeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtSetVolumeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtSetVolumeReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtSetVolumeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->volume, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SET_VOLUME_REQ */


#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SS_SET_VOLUME_REQ
CsrSize CsrBtAvrcpCtSetSSVolumeReqSizeof(void *msg)
{
    CsrBtAvrcpCtSetSSVolumeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SS_SET_VOLUME_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SS_SET_VOLUME_REQ
CsrUint8 *CsrBtAvrcpCtSetSSVolumeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtSetSSVolumeReq *primitive;

    primitive = (CsrBtAvrcpCtSetSSVolumeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->volume);
	CsrUint8Ser(buffer, length, (CsrUint8) primitive->isMute);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SS_SET_VOLUME_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SS_SET_VOLUME_REQ
void *CsrBtAvrcpCtSetSSVolumeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtSetSSVolumeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtSetSSVolumeReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtSetSSVolumeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->volume, buffer, &offset);
	CsrUint8Des((CsrUint8 *) &primitive->isMute, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SS_SET_VOLUME_REQ */



#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_RES
CsrSize CsrBtAvrcpTgNotiResSizeof(void *msg)
{
    CsrBtAvrcpTgNotiRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_RES
CsrUint8 *CsrBtAvrcpTgNotiResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgNotiRes *primitive;

    primitive = (CsrBtAvrcpTgNotiRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->notiId);
    CsrMemCpySer(buffer, length, ((const void *) primitive->notiData), ((CsrSize) (8)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_NOTI_RES
void *CsrBtAvrcpTgNotiResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgNotiRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgNotiRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgNotiRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->notiId, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->notiData), buffer, &offset, ((CsrSize) (8)));
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_NOTI_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PLAY_RES
CsrSize CsrBtAvrcpTgPlayResSizeof(void *msg)
{
    CsrBtAvrcpTgPlayRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PLAY_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PLAY_RES
CsrUint8 *CsrBtAvrcpTgPlayResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgPlayRes *primitive;

    primitive = (CsrBtAvrcpTgPlayRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrMemCpySer(buffer, length, ((const void *) primitive->uid), ((CsrSize) ((8))));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->scope);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PLAY_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_PLAY_RES
void *CsrBtAvrcpTgPlayResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgPlayRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgPlayRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgPlayRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->uid), buffer, &offset, ((CsrSize) ((8))));
    CsrUint8Des((CsrUint8 *) &primitive->scope, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_PLAY_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_CFM
CsrSize CsrBtAvrcpCtPasSetCfmSizeof(void *msg)
{
    CsrBtAvrcpCtPasSetCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_CFM
CsrUint8 *CsrBtAvrcpCtPasSetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasSetCfm *primitive;

    primitive = (CsrBtAvrcpCtPasSetCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_CFM
void *CsrBtAvrcpCtPasSetCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasSetCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasSetCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasSetCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_INFORM_BATTERY_STATUS_IND
CsrSize CsrBtAvrcpTgInformBatteryStatusIndSizeof(void *msg)
{
    CsrBtAvrcpTgInformBatteryStatusInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_INFORM_BATTERY_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_INFORM_BATTERY_STATUS_IND
CsrUint8 *CsrBtAvrcpTgInformBatteryStatusIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgInformBatteryStatusInd *primitive;

    primitive = (CsrBtAvrcpTgInformBatteryStatusInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->batStatus);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_INFORM_BATTERY_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_INFORM_BATTERY_STATUS_IND
void *CsrBtAvrcpTgInformBatteryStatusIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgInformBatteryStatusInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgInformBatteryStatusInd *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgInformBatteryStatusInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->batStatus, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_INFORM_BATTERY_STATUS_IND */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES
CsrSize CsrBtAvrcpTgSetBrowsedPlayerResSizeof(void *msg)
{
    CsrBtAvrcpTgSetBrowsedPlayerRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->folderNamesLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES
CsrUint8 *CsrBtAvrcpTgSetBrowsedPlayerResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpTgSetBrowsedPlayerRes *primitive;

    primitive = (CsrBtAvrcpTgSetBrowsedPlayerRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->uidCounter);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->itemsCount);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->folderDepth);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->folderNamesLen);
    CsrMemCpySer(buffer, length, &primitive->folderNames, sizeof(primitive->folderNames));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->msgId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->playerId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->status);
    if(primitive->folderNames)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->folderNames), ((CsrSize) (primitive->folderNamesLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES
void *CsrBtAvrcpTgSetBrowsedPlayerResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpTgSetBrowsedPlayerRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpTgSetBrowsedPlayerRes *) CsrPmemZalloc(sizeof(CsrBtAvrcpTgSetBrowsedPlayerRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->uidCounter, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->itemsCount, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->folderDepth, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->folderNamesLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->folderNames), buffer, &offset, ((CsrSize) sizeof(primitive->folderNames)));
    CsrUint32Des((CsrUint32 *) &primitive->msgId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->playerId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->status, buffer, &offset);
    if (primitive->folderNamesLen)
    {
        primitive->folderNames = CsrPmemZalloc(((CsrUint32) (primitive->folderNamesLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->folderNames), buffer, &offset, ((CsrSize) (primitive->folderNamesLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->folderNames = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES
void CsrBtAvrcpTgSetBrowsedPlayerResSerFree(void *msg)
{
    CsrBtAvrcpTgSetBrowsedPlayerRes *primitive;

    primitive = (CsrBtAvrcpTgSetBrowsedPlayerRes *) msg;

    if(primitive->folderNames != NULL)
    {
        CsrPmemFree(primitive->folderNames);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_TG_SET_BROWSED_PLAYER_RES */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM
CsrSize CsrBtAvrcpCtSubUnitInfoCmdCfmSizeof(void *msg)
{
    CsrBtAvrcpCtSubUnitInfoCmdCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->pDataLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM
CsrUint8 *CsrBtAvrcpCtSubUnitInfoCmdCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtSubUnitInfoCmdCfm *primitive;

    primitive = (CsrBtAvrcpCtSubUnitInfoCmdCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->pDataLen);
    CsrMemCpySer(buffer, length, &primitive->pData, sizeof(primitive->pData));
    if(primitive->pData)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->pData), ((CsrSize) (primitive->pDataLen * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM
void *CsrBtAvrcpCtSubUnitInfoCmdCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtSubUnitInfoCmdCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtSubUnitInfoCmdCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtSubUnitInfoCmdCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->pDataLen, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->pData), buffer, &offset, ((CsrSize) sizeof(primitive->pData)));
    if (primitive->pDataLen)
    {
        primitive->pData = CsrPmemZalloc(((CsrUint32) (primitive->pDataLen * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->pData), buffer, &offset, ((CsrSize) (primitive->pDataLen * sizeof(CsrUint8))));
    }
    else
    {
        primitive->pData = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM
void CsrBtAvrcpCtSubUnitInfoCmdCfmSerFree(void *msg)
{
    CsrBtAvrcpCtSubUnitInfoCmdCfm *primitive;

    primitive = (CsrBtAvrcpCtSubUnitInfoCmdCfm *) msg;

    if(primitive->pData != NULL)
    {
        CsrPmemFree(primitive->pData);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_SUB_UNIT_INFO_CMD_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ
CsrSize CsrBtAvrcpCtPasAttTxtReqSizeof(void *msg)
{
    CsrBtAvrcpCtPasAttTxtReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attIdCount * sizeof(CsrBtAvrcpPasAttId)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ
CsrUint8 *CsrBtAvrcpCtPasAttTxtReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasAttTxtReq *primitive;

    primitive = (CsrBtAvrcpCtPasAttTxtReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attIdCount);
    CsrMemCpySer(buffer, length, &primitive->attId, sizeof(primitive->attId));
    if(primitive->attId)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attId), ((CsrSize) (primitive->attIdCount * sizeof(CsrBtAvrcpPasAttId))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ
void *CsrBtAvrcpCtPasAttTxtReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasAttTxtReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasAttTxtReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasAttTxtReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->attIdCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attId), buffer, &offset, ((CsrSize) sizeof(primitive->attId)));
    if (primitive->attIdCount)
    {
        primitive->attId = CsrPmemZalloc(((CsrUint32) (primitive->attIdCount * sizeof(CsrBtAvrcpPasAttId))));
        CsrMemCpyDes(((void *) primitive->attId), buffer, &offset, ((CsrSize) (primitive->attIdCount * sizeof(CsrBtAvrcpPasAttId))));
    }
    else
    {
        primitive->attId = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ
void CsrBtAvrcpCtPasAttTxtReqSerFree(void *msg)
{
    CsrBtAvrcpCtPasAttTxtReq *primitive;

    primitive = (CsrBtAvrcpCtPasAttTxtReq *) msg;

    if(primitive->attId != NULL)
    {
        CsrPmemFree(primitive->attId);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_ATT_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_REQ
CsrSize CsrBtAvrcpCtPasSetReqSizeof(void *msg)
{
    CsrBtAvrcpCtPasSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_REQ
CsrUint8 *CsrBtAvrcpCtPasSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasSetReq *primitive;

    primitive = (CsrBtAvrcpCtPasSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attValPairCount);
    CsrMemCpySer(buffer, length, &primitive->attValPair, sizeof(primitive->attValPair));
    if(primitive->attValPair)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->attValPair), ((CsrSize) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_REQ
void *CsrBtAvrcpCtPasSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasSetReq *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->attValPairCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->attValPair), buffer, &offset, ((CsrSize) sizeof(primitive->attValPair)));
    if (primitive->attValPairCount)
    {
        primitive->attValPair = CsrPmemZalloc(((CsrUint32) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
        CsrMemCpyDes(((void *) primitive->attValPair), buffer, &offset, ((CsrSize) (primitive->attValPairCount * sizeof(CsrBtAvrcpPasAttValPair))));
    }
    else
    {
        primitive->attValPair = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_REQ
void CsrBtAvrcpCtPasSetReqSerFree(void *msg)
{
    CsrBtAvrcpCtPasSetReq *primitive;

    primitive = (CsrBtAvrcpCtPasSetReq *) msg;

    if(primitive->attValPair != NULL)
    {
        CsrPmemFree(primitive->attValPair);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_SET_REQ */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM
CsrSize CsrBtAvrcpCtPasValIdCfmSizeof(void *msg)
{
    CsrBtAvrcpCtPasValIdCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->valIdCount * sizeof(CsrBtAvrcpPasValId)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM
CsrUint8 *CsrBtAvrcpCtPasValIdCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAvrcpCtPasValIdCfm *primitive;

    primitive = (CsrBtAvrcpCtPasValIdCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->attId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->valIdCount);
    CsrMemCpySer(buffer, length, &primitive->valId, sizeof(primitive->valId));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    if(primitive->valId)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->valId), ((CsrSize) (primitive->valIdCount * sizeof(CsrBtAvrcpPasValId))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM
void *CsrBtAvrcpCtPasValIdCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAvrcpCtPasValIdCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAvrcpCtPasValIdCfm *) CsrPmemZalloc(sizeof(CsrBtAvrcpCtPasValIdCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->attId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->valIdCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->valId), buffer, &offset, ((CsrSize) sizeof(primitive->valId)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    if (primitive->valIdCount)
    {
        primitive->valId = CsrPmemZalloc(((CsrUint32) (primitive->valIdCount * sizeof(CsrBtAvrcpPasValId))));
        CsrMemCpyDes(((void *) primitive->valId), buffer, &offset, ((CsrSize) (primitive->valIdCount * sizeof(CsrBtAvrcpPasValId))));
    }
    else
    {
        primitive->valId = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM */

#ifndef EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM
void CsrBtAvrcpCtPasValIdCfmSerFree(void *msg)
{
    CsrBtAvrcpCtPasValIdCfm *primitive;

    primitive = (CsrBtAvrcpCtPasValIdCfm *) msg;

    if(primitive->valId != NULL)
    {
        CsrPmemFree(primitive->valId);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AVRCP_CT_PAS_VAL_ID_CFM */

#endif /* EXCLUDE_CSR_BT_AVRCP_MODULE */
