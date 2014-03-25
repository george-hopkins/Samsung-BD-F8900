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
#include "csr_bt_sc_private_prim.h"

#ifndef EXCLUDE_CSR_BT_SC_PRIVATE_MODULE

void CsrBtScPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_SC_SSP_PAIRING_RES
CsrSize CsrBtScSspPairingResSizeof(void *msg)
{
    CsrBtScSspPairingRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_PAIRING_RES */

#ifndef EXCLUDE_CSR_BT_SC_SSP_PAIRING_RES
CsrUint8 *CsrBtScSspPairingResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSspPairingRes *primitive;

    primitive = (CsrBtScSspPairingRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authorised);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addDevice);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_PAIRING_RES */

#ifndef EXCLUDE_CSR_BT_SC_SSP_PAIRING_RES
void *CsrBtScSspPairingResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSspPairingRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSspPairingRes *) CsrPmemZalloc(sizeof(CsrBtScSspPairingRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authorised, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addDevice, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_PAIRING_RES */

#ifndef EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_IND
CsrSize CsrBtScCancelAcceptBondIndSizeof(void *msg)
{
    CsrBtScCancelAcceptBondInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_IND */

#ifndef EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_IND
CsrUint8 *CsrBtScCancelAcceptBondIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScCancelAcceptBondInd *primitive;

    primitive = (CsrBtScCancelAcceptBondInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_IND */

#ifndef EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_IND
void *CsrBtScCancelAcceptBondIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScCancelAcceptBondInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScCancelAcceptBondInd *) CsrPmemZalloc(sizeof(CsrBtScCancelAcceptBondInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_IND */

#ifndef EXCLUDE_CSR_BT_SC_LE_SECURITY_CFM
CsrSize CsrBtScLeSecurityCfmSizeof(void *msg)
{
    CsrBtScLeSecurityCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_LE_SECURITY_CFM */

#ifndef EXCLUDE_CSR_BT_SC_LE_SECURITY_CFM
CsrUint8 *CsrBtScLeSecurityCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScLeSecurityCfm *primitive;

    primitive = (CsrBtScLeSecurityCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addedToScDbList);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_LE_SECURITY_CFM */

#ifndef EXCLUDE_CSR_BT_SC_LE_SECURITY_CFM
void *CsrBtScLeSecurityCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScLeSecurityCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScLeSecurityCfm *) CsrPmemZalloc(sizeof(CsrBtScLeSecurityCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addedToScDbList, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_LE_SECURITY_CFM */

#ifndef EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_REQ
CsrSize CsrBtScReadLocalOobDataReqSizeof(void *msg)
{
    CsrBtScReadLocalOobDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_REQ
CsrUint8 *CsrBtScReadLocalOobDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScReadLocalOobDataReq *primitive;

    primitive = (CsrBtScReadLocalOobDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_REQ
void *CsrBtScReadLocalOobDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScReadLocalOobDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScReadLocalOobDataReq *) CsrPmemZalloc(sizeof(CsrBtScReadLocalOobDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_SC_PASSKEY_RES
CsrSize CsrBtScPasskeyResSizeof(void *msg)
{
    CsrBtScPasskeyRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_PASSKEY_RES */

#ifndef EXCLUDE_CSR_BT_SC_PASSKEY_RES
CsrUint8 *CsrBtScPasskeyResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScPasskeyRes *primitive;

    primitive = (CsrBtScPasskeyRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authorised);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addDevice);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->passKeyLength);
    CsrMemCpySer(buffer, length, ((const void *) primitive->passKey), ((CsrSize) (16)));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_PASSKEY_RES */

#ifndef EXCLUDE_CSR_BT_SC_PASSKEY_RES
void *CsrBtScPasskeyResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScPasskeyRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScPasskeyRes *) CsrPmemZalloc(sizeof(CsrBtScPasskeyRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authorised, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addDevice, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->passKeyLength, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->passKey), buffer, &offset, ((CsrSize) (16)));
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_PASSKEY_RES */

#ifndef EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_CFM
CsrSize CsrBtScSetSecurityModeCfmSizeof(void *msg)
{
    CsrBtScSetSecurityModeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_CFM
CsrUint8 *CsrBtScSetSecurityModeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSetSecurityModeCfm *primitive;

    primitive = (CsrBtScSetSecurityModeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_CFM
void *CsrBtScSetSecurityModeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSetSecurityModeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSetSecurityModeCfm *) CsrPmemZalloc(sizeof(CsrBtScSetSecurityModeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_BOND_CFM
CsrSize CsrBtScBondCfmSizeof(void *msg)
{
    CsrBtScBondCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_BOND_CFM */

#ifndef EXCLUDE_CSR_BT_SC_BOND_CFM
CsrUint8 *CsrBtScBondCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScBondCfm *primitive;

    primitive = (CsrBtScBondCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->cod);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addedToScDbList);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_BOND_CFM */

#ifndef EXCLUDE_CSR_BT_SC_BOND_CFM
void *CsrBtScBondCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScBondCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScBondCfm *) CsrPmemZalloc(sizeof(CsrBtScBondCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->cod, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addedToScDbList, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_BOND_CFM */

#ifndef EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND
CsrSize CsrBtScSspKeypressNotificationIndSizeof(void *msg)
{
    CsrBtScSspKeypressNotificationInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND
CsrUint8 *CsrBtScSspKeypressNotificationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSspKeypressNotificationInd *primitive;

    primitive = (CsrBtScSspKeypressNotificationInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->notificationType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND
void *CsrBtScSspKeypressNotificationIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSspKeypressNotificationInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSspKeypressNotificationInd *) CsrPmemZalloc(sizeof(CsrBtScSspKeypressNotificationInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->notificationType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_COMPARE_RES
CsrSize CsrBtScSspCompareResSizeof(void *msg)
{
    CsrBtScSspCompareRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_COMPARE_RES */

#ifndef EXCLUDE_CSR_BT_SC_SSP_COMPARE_RES
CsrUint8 *CsrBtScSspCompareResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSspCompareRes *primitive;

    primitive = (CsrBtScSspCompareRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authorised);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addDevice);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_COMPARE_RES */

#ifndef EXCLUDE_CSR_BT_SC_SSP_COMPARE_RES
void *CsrBtScSspCompareResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSspCompareRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSspCompareRes *) CsrPmemZalloc(sizeof(CsrBtScSspCompareRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authorised, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addDevice, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_COMPARE_RES */

#ifndef EXCLUDE_CSR_BT_SC_SSP_COMPARE_IND
CsrSize CsrBtScSspCompareIndSizeof(void *msg)
{
    CsrBtScSspCompareInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->deviceName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_COMPARE_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_COMPARE_IND
CsrUint8 *CsrBtScSspCompareIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSspCompareInd *primitive;

    primitive = (CsrBtScSspCompareInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->cod);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->deviceName, sizeof(primitive->deviceName));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->paired);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->authRequirements);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->numericValue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->initiator);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->localAuthRequirements);
    CsrUtf8StringSer(buffer, length, primitive->deviceName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_COMPARE_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_COMPARE_IND
void *CsrBtScSspCompareIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSspCompareInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSspCompareInd *) CsrPmemZalloc(sizeof(CsrBtScSspCompareInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->cod, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->deviceName), buffer, &offset, ((CsrSize) sizeof(primitive->deviceName)));
    CsrUint8Des((CsrUint8 *) &primitive->paired, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->authRequirements, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->numericValue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->initiator, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->localAuthRequirements, buffer, &offset);
    CsrUtf8StringDes(&primitive->deviceName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_COMPARE_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_COMPARE_IND
void CsrBtScSspCompareIndSerFree(void *msg)
{
    CsrBtScSspCompareInd *primitive;

    primitive = (CsrBtScSspCompareInd *) msg;

    if(primitive->deviceName != NULL)
    {
        CsrPmemFree(primitive->deviceName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_COMPARE_IND */

#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_REQ
CsrSize CsrBtScReadDeviceRecordReqSizeof(void *msg)
{
    CsrBtScReadDeviceRecordReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_REQ
CsrUint8 *CsrBtScReadDeviceRecordReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScReadDeviceRecordReq *primitive;

    primitive = (CsrBtScReadDeviceRecordReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->maxNumOfBytes);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_REQ
void *CsrBtScReadDeviceRecordReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScReadDeviceRecordReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScReadDeviceRecordReq *) CsrPmemZalloc(sizeof(CsrBtScReadDeviceRecordReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->maxNumOfBytes, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_REQ */

#ifndef EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM
CsrSize CsrBtScReadLocalOobDataCfmSizeof(void *msg)
{
    CsrBtScReadLocalOobDataCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->oobHashCLength * sizeof(CsrUint8)) + (primitive->oobRandRLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM
CsrUint8 *CsrBtScReadLocalOobDataCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScReadLocalOobDataCfm *primitive;

    primitive = (CsrBtScReadLocalOobDataCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->oobHashCLength);
    CsrMemCpySer(buffer, length, &primitive->oobHashC, sizeof(primitive->oobHashC));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->oobRandRLength);
    CsrMemCpySer(buffer, length, &primitive->oobRandR, sizeof(primitive->oobRandR));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    if(primitive->oobHashC)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->oobHashC), ((CsrSize) (primitive->oobHashCLength * sizeof(CsrUint8))));
    }
    if(primitive->oobRandR)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->oobRandR), ((CsrSize) (primitive->oobRandRLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM
void *CsrBtScReadLocalOobDataCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScReadLocalOobDataCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScReadLocalOobDataCfm *) CsrPmemZalloc(sizeof(CsrBtScReadLocalOobDataCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->oobHashCLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->oobHashC), buffer, &offset, ((CsrSize) sizeof(primitive->oobHashC)));
    CsrUint32Des((CsrUint32 *) &primitive->oobRandRLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->oobRandR), buffer, &offset, ((CsrSize) sizeof(primitive->oobRandR)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    if (primitive->oobHashCLength)
    {
        primitive->oobHashC = CsrPmemZalloc(((CsrUint32) (primitive->oobHashCLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->oobHashC), buffer, &offset, ((CsrSize) (primitive->oobHashCLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->oobHashC = NULL;
    }
    if (primitive->oobRandRLength)
    {
        primitive->oobRandR = CsrPmemZalloc(((CsrUint32) (primitive->oobRandRLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->oobRandR), buffer, &offset, ((CsrSize) (primitive->oobRandRLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->oobRandR = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM
void CsrBtScReadLocalOobDataCfmSerFree(void *msg)
{
    CsrBtScReadLocalOobDataCfm *primitive;

    primitive = (CsrBtScReadLocalOobDataCfm *) msg;

    if(primitive->oobHashC != NULL)
    {
        CsrPmemFree(primitive->oobHashC);
    }
    if(primitive->oobRandR != NULL)
    {
        CsrPmemFree(primitive->oobRandR);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SC_READ_LOCAL_OOB_DATA_CFM */

#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_CFM
CsrSize CsrBtScReadDeviceRecordCfmSizeof(void *msg)
{
    CsrBtScReadDeviceRecordCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->devicePropertiesLength * sizeof(CsrBtDevicePropertiesType)) + (primitive->serviceChangedListLength * sizeof(CsrUint32)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_CFM
CsrUint8 *CsrBtScReadDeviceRecordCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScReadDeviceRecordCfm *primitive;

    primitive = (CsrBtScReadDeviceRecordCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->totalNumOfDevices);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->devicePropertiesLength);
    CsrMemCpySer(buffer, length, &primitive->deviceProperties, sizeof(primitive->deviceProperties));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceChangedListLength);
    CsrMemCpySer(buffer, length, &primitive->serviceChangedList, sizeof(primitive->serviceChangedList));
    if(primitive->deviceProperties)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->deviceProperties), ((CsrSize) (primitive->devicePropertiesLength * sizeof(CsrBtDevicePropertiesType))));
    }
    if(primitive->serviceChangedList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->serviceChangedList), ((CsrSize) (primitive->serviceChangedListLength * sizeof(CsrUint32))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_CFM
void *CsrBtScReadDeviceRecordCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScReadDeviceRecordCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScReadDeviceRecordCfm *) CsrPmemZalloc(sizeof(CsrBtScReadDeviceRecordCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->totalNumOfDevices, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->devicePropertiesLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->deviceProperties), buffer, &offset, ((CsrSize) sizeof(primitive->deviceProperties)));
    CsrUint32Des((CsrUint32 *) &primitive->serviceChangedListLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceChangedList), buffer, &offset, ((CsrSize) sizeof(primitive->serviceChangedList)));
    if (primitive->devicePropertiesLength)
    {
        primitive->deviceProperties = CsrPmemZalloc(((CsrUint32) (primitive->devicePropertiesLength * sizeof(CsrBtDevicePropertiesType))));
        CsrMemCpyDes(((void *) primitive->deviceProperties), buffer, &offset, ((CsrSize) (primitive->devicePropertiesLength * sizeof(CsrBtDevicePropertiesType))));
    }
    else
    {
        primitive->deviceProperties = NULL;
    }
    if (primitive->serviceChangedListLength)
    {
        primitive->serviceChangedList = CsrPmemZalloc(((CsrUint32) (primitive->serviceChangedListLength * sizeof(CsrUint32))));
        CsrMemCpyDes(((void *) primitive->serviceChangedList), buffer, &offset, ((CsrSize) (primitive->serviceChangedListLength * sizeof(CsrUint32))));
    }
    else
    {
        primitive->serviceChangedList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_CFM
void CsrBtScReadDeviceRecordCfmSerFree(void *msg)
{
    CsrBtScReadDeviceRecordCfm *primitive;

    primitive = (CsrBtScReadDeviceRecordCfm *) msg;

    if(primitive->deviceProperties != NULL)
    {
        CsrPmemFree(primitive->deviceProperties);
    }
    if(primitive->serviceChangedList != NULL)
    {
        CsrPmemFree(primitive->serviceChangedList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_CFM */

#ifndef EXCLUDE_CSR_BT_SC_SET_AUTH_REQUIREMENTS_REQ
CsrSize CsrBtScSetAuthRequirementsReqSizeof(void *msg)
{
    CsrBtScSetAuthRequirementsReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SET_AUTH_REQUIREMENTS_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SET_AUTH_REQUIREMENTS_REQ
CsrUint8 *CsrBtScSetAuthRequirementsReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSetAuthRequirementsReq *primitive;

    primitive = (CsrBtScSetAuthRequirementsReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authRequirements);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->leAuthRequirements);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SET_AUTH_REQUIREMENTS_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SET_AUTH_REQUIREMENTS_REQ
void *CsrBtScSetAuthRequirementsReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSetAuthRequirementsReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSetAuthRequirementsReq *) CsrPmemZalloc(sizeof(CsrBtScSetAuthRequirementsReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authRequirements, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->leAuthRequirements, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SET_AUTH_REQUIREMENTS_REQ */

#ifndef EXCLUDE_CSR_BT_SC_AUTHENTICATE_REQ
CsrSize CsrBtScAuthenticateReqSizeof(void *msg)
{
    CsrBtScAuthenticateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_AUTHENTICATE_REQ
CsrUint8 *CsrBtScAuthenticateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScAuthenticateReq *primitive;

    primitive = (CsrBtScAuthenticateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_AUTHENTICATE_REQ
void *CsrBtScAuthenticateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScAuthenticateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScAuthenticateReq *) CsrPmemZalloc(sizeof(CsrBtScAuthenticateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHENTICATE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_IND
CsrSize CsrBtScSspNotificationIndSizeof(void *msg)
{
    CsrBtScSspNotificationInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->deviceName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_IND
CsrUint8 *CsrBtScSspNotificationIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSspNotificationInd *primitive;

    primitive = (CsrBtScSspNotificationInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->cod);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->deviceName, sizeof(primitive->deviceName));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->paired);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->authRequirements);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->numericValue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->initiator);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->localAuthRequirements);
    CsrUtf8StringSer(buffer, length, primitive->deviceName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_IND
void *CsrBtScSspNotificationIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSspNotificationInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSspNotificationInd *) CsrPmemZalloc(sizeof(CsrBtScSspNotificationInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->cod, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->deviceName), buffer, &offset, ((CsrSize) sizeof(primitive->deviceName)));
    CsrUint8Des((CsrUint8 *) &primitive->paired, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->authRequirements, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->numericValue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->initiator, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->localAuthRequirements, buffer, &offset);
    CsrUtf8StringDes(&primitive->deviceName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_IND
void CsrBtScSspNotificationIndSerFree(void *msg)
{
    CsrBtScSspNotificationInd *primitive;

    primitive = (CsrBtScSspNotificationInd *) msg;

    if(primitive->deviceName != NULL)
    {
        CsrPmemFree(primitive->deviceName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_IND */

#ifndef EXCLUDE_CSR_BT_SC_MODE_REQ
CsrSize CsrBtScModeReqSizeof(void *msg)
{
    CsrBtScModeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_MODE_REQ
CsrUint8 *CsrBtScModeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScModeReq *primitive;

    primitive = (CsrBtScModeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->duration);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_MODE_REQ
void *CsrBtScModeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScModeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScModeReq *) CsrPmemZalloc(sizeof(CsrBtScModeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->duration, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_REQ
CsrSize CsrBtScCancelAcceptBondReqSizeof(void *msg)
{
    CsrBtScCancelAcceptBondReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_REQ
CsrUint8 *CsrBtScCancelAcceptBondReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScCancelAcceptBondReq *primitive;

    primitive = (CsrBtScCancelAcceptBondReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_REQ
void *CsrBtScCancelAcceptBondReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScCancelAcceptBondReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScCancelAcceptBondReq *) CsrPmemZalloc(sizeof(CsrBtScCancelAcceptBondReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_CANCEL_ACCEPT_BOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SSP_PASSKEY_RES
CsrSize CsrBtScSspPasskeyResSizeof(void *msg)
{
    CsrBtScSspPasskeyRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_PASSKEY_RES */

#ifndef EXCLUDE_CSR_BT_SC_SSP_PASSKEY_RES
CsrUint8 *CsrBtScSspPasskeyResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSspPasskeyRes *primitive;

    primitive = (CsrBtScSspPasskeyRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authorised);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addDevice);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->numericValue);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_PASSKEY_RES */

#ifndef EXCLUDE_CSR_BT_SC_SSP_PASSKEY_RES
void *CsrBtScSspPasskeyResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSspPasskeyRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSspPasskeyRes *) CsrPmemZalloc(sizeof(CsrBtScSspPasskeyRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authorised, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addDevice, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->numericValue, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_PASSKEY_RES */

#ifndef EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_REQ
CsrSize CsrBtScSetSecurityModeReqSizeof(void *msg)
{
    CsrBtScSetSecurityModeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_REQ
CsrUint8 *CsrBtScSetSecurityModeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSetSecurityModeReq *primitive;

    primitive = (CsrBtScSetSecurityModeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->securityMode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_REQ
void *CsrBtScSetSecurityModeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSetSecurityModeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSetSecurityModeReq *) CsrPmemZalloc(sizeof(CsrBtScSetSecurityModeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->securityMode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SET_SECURITY_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_BOND_IND
CsrSize CsrBtScBondIndSizeof(void *msg)
{
    CsrBtScBondInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_BOND_IND */

#ifndef EXCLUDE_CSR_BT_SC_BOND_IND
CsrUint8 *CsrBtScBondIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScBondInd *primitive;

    primitive = (CsrBtScBondInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->cod);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addedToScDbList);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_BOND_IND */

#ifndef EXCLUDE_CSR_BT_SC_BOND_IND
void *CsrBtScBondIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScBondInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScBondInd *) CsrPmemZalloc(sizeof(CsrBtScBondInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->cod, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addedToScDbList, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_BOND_IND */

#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_REQ
CsrSize CsrBtScEncryptionKeySizeReqSizeof(void *msg)
{
    CsrBtScEncryptionKeySizeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_REQ
CsrUint8 *CsrBtScEncryptionKeySizeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScEncryptionKeySizeReq *primitive;

    primitive = (CsrBtScEncryptionKeySizeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_REQ
void *CsrBtScEncryptionKeySizeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScEncryptionKeySizeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScEncryptionKeySizeReq *) CsrPmemZalloc(sizeof(CsrBtScEncryptionKeySizeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SET_IO_CAPABILITY_REQ
CsrSize CsrBtScSetIoCapabilityReqSizeof(void *msg)
{
    CsrBtScSetIoCapabilityReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SET_IO_CAPABILITY_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SET_IO_CAPABILITY_REQ
CsrUint8 *CsrBtScSetIoCapabilityReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSetIoCapabilityReq *primitive;

    primitive = (CsrBtScSetIoCapabilityReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ioCapability);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SET_IO_CAPABILITY_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SET_IO_CAPABILITY_REQ
void *CsrBtScSetIoCapabilityReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSetIoCapabilityReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSetIoCapabilityReq *) CsrPmemZalloc(sizeof(CsrBtScSetIoCapabilityReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ioCapability, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SET_IO_CAPABILITY_REQ */

#ifndef EXCLUDE_CSR_BT_SC_LE_SECURITY_REQ
CsrSize CsrBtScLeSecurityReqSizeof(void *msg)
{
    CsrBtScLeSecurityReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_LE_SECURITY_REQ */

#ifndef EXCLUDE_CSR_BT_SC_LE_SECURITY_REQ
CsrUint8 *CsrBtScLeSecurityReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScLeSecurityReq *primitive;

    primitive = (CsrBtScLeSecurityReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->securityRequirements);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->l2caConFlags);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_LE_SECURITY_REQ */

#ifndef EXCLUDE_CSR_BT_SC_LE_SECURITY_REQ
void *CsrBtScLeSecurityReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScLeSecurityReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScLeSecurityReq *) CsrPmemZalloc(sizeof(CsrBtScLeSecurityReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->securityRequirements, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->l2caConFlags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_LE_SECURITY_REQ */

#ifndef EXCLUDE_CSR_BT_SC_DEBUG_MODE_REQ
CsrSize CsrBtScDebugModeReqSizeof(void *msg)
{
    CsrBtScDebugModeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_DEBUG_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_DEBUG_MODE_REQ
CsrUint8 *CsrBtScDebugModeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScDebugModeReq *primitive;

    primitive = (CsrBtScDebugModeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enable);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_DEBUG_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_DEBUG_MODE_REQ
void *CsrBtScDebugModeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScDebugModeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScDebugModeReq *) CsrPmemZalloc(sizeof(CsrBtScDebugModeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enable, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_DEBUG_MODE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_CFM
CsrSize CsrBtScEncryptionCfmSizeof(void *msg)
{
    CsrBtScEncryptionCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_CFM */

#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_CFM
CsrUint8 *CsrBtScEncryptionCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScEncryptionCfm *primitive;

    primitive = (CsrBtScEncryptionCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->encEnabled);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_CFM */

#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_CFM
void *CsrBtScEncryptionCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScEncryptionCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScEncryptionCfm *) CsrPmemZalloc(sizeof(CsrBtScEncryptionCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->encEnabled, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_CFM */

#ifndef EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_REQ
CsrSize CsrBtScSspKeypressNotificationReqSizeof(void *msg)
{
    CsrBtScSspKeypressNotificationReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_REQ
CsrUint8 *CsrBtScSspKeypressNotificationReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSspKeypressNotificationReq *primitive;

    primitive = (CsrBtScSspKeypressNotificationReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->notificationType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_REQ
void *CsrBtScSspKeypressNotificationReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSspKeypressNotificationReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSspKeypressNotificationReq *) CsrPmemZalloc(sizeof(CsrBtScSspKeypressNotificationReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->notificationType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_KEYPRESS_NOTIFICATION_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ACCEPT_BOND_REQ
CsrSize CsrBtScAcceptBondReqSizeof(void *msg)
{
    CsrBtScAcceptBondReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_ACCEPT_BOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ACCEPT_BOND_REQ
CsrUint8 *CsrBtScAcceptBondReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScAcceptBondReq *primitive;

    primitive = (CsrBtScAcceptBondReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->time);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_ACCEPT_BOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ACCEPT_BOND_REQ
void *CsrBtScAcceptBondReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScAcceptBondReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScAcceptBondReq *) CsrPmemZalloc(sizeof(CsrBtScAcceptBondReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->time, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_ACCEPT_BOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SSP_PASSKEY_IND
CsrSize CsrBtScSspPasskeyIndSizeof(void *msg)
{
    CsrBtScSspPasskeyInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->deviceName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_PASSKEY_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_PASSKEY_IND
CsrUint8 *CsrBtScSspPasskeyIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSspPasskeyInd *primitive;

    primitive = (CsrBtScSspPasskeyInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->cod);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->deviceName, sizeof(primitive->deviceName));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->paired);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->authRequirements);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->initiator);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->localAuthRequirements);
    CsrUtf8StringSer(buffer, length, primitive->deviceName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_PASSKEY_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_PASSKEY_IND
void *CsrBtScSspPasskeyIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSspPasskeyInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSspPasskeyInd *) CsrPmemZalloc(sizeof(CsrBtScSspPasskeyInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->cod, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->deviceName), buffer, &offset, ((CsrSize) sizeof(primitive->deviceName)));
    CsrUint8Des((CsrUint8 *) &primitive->paired, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->authRequirements, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->initiator, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->localAuthRequirements, buffer, &offset);
    CsrUtf8StringDes(&primitive->deviceName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_PASSKEY_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_PASSKEY_IND
void CsrBtScSspPasskeyIndSerFree(void *msg)
{
    CsrBtScSspPasskeyInd *primitive;

    primitive = (CsrBtScSspPasskeyInd *) msg;

    if(primitive->deviceName != NULL)
    {
        CsrPmemFree(primitive->deviceName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_PASSKEY_IND */

#ifndef EXCLUDE_CSR_BT_SC_LE_SECURITY_IND
CsrSize CsrBtScLeSecurityIndSizeof(void *msg)
{
    CsrBtScLeSecurityInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_LE_SECURITY_IND */

#ifndef EXCLUDE_CSR_BT_SC_LE_SECURITY_IND
CsrUint8 *CsrBtScLeSecurityIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScLeSecurityInd *primitive;

    primitive = (CsrBtScLeSecurityInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addedToScDbList);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_LE_SECURITY_IND */

#ifndef EXCLUDE_CSR_BT_SC_LE_SECURITY_IND
void *CsrBtScLeSecurityIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScLeSecurityInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScLeSecurityInd *) CsrPmemZalloc(sizeof(CsrBtScLeSecurityInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addedToScDbList, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_LE_SECURITY_IND */

#ifndef EXCLUDE_CSR_BT_SC_LE_ADD_SERVICE_CHANGED_REQ
CsrSize CsrBtScLeAddServiceChangedReqSizeof(void *msg)
{
    CsrBtScLeAddServiceChangedReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_LE_ADD_SERVICE_CHANGED_REQ */

#ifndef EXCLUDE_CSR_BT_SC_LE_ADD_SERVICE_CHANGED_REQ
CsrUint8 *CsrBtScLeAddServiceChangedReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScLeAddServiceChangedReq *primitive;

    primitive = (CsrBtScLeAddServiceChangedReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceChangedHandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_LE_ADD_SERVICE_CHANGED_REQ */

#ifndef EXCLUDE_CSR_BT_SC_LE_ADD_SERVICE_CHANGED_REQ
void *CsrBtScLeAddServiceChangedReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScLeAddServiceChangedReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScLeAddServiceChangedReq *) CsrPmemZalloc(sizeof(CsrBtScLeAddServiceChangedReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->serviceChangedHandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_LE_ADD_SERVICE_CHANGED_REQ */

#ifndef EXCLUDE_CSR_BT_SC_REGISTER_REQ
CsrSize CsrBtScRegisterReqSizeof(void *msg)
{
    CsrBtScRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_SC_REGISTER_REQ
CsrUint8 *CsrBtScRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScRegisterReq *primitive;

    primitive = (CsrBtScRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->protocolId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->channel);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->profileUuid);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->outgoingOk);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_SC_REGISTER_REQ
void *CsrBtScRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScRegisterReq *) CsrPmemZalloc(sizeof(CsrBtScRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->protocolId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->channel, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->profileUuid, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->outgoingOk, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ACTIVATE_REQ
CsrSize CsrBtScActivateReqSizeof(void *msg)
{
    CsrBtScActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ACTIVATE_REQ
CsrUint8 *CsrBtScActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScActivateReq *primitive;

    primitive = (CsrBtScActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ACTIVATE_REQ
void *CsrBtScActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScActivateReq *) CsrPmemZalloc(sizeof(CsrBtScActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_REQ
CsrSize CsrBtScEncryptionReqSizeof(void *msg)
{
    CsrBtScEncryptionReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_REQ
CsrUint8 *CsrBtScEncryptionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScEncryptionReq *primitive;

    primitive = (CsrBtScEncryptionReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->encEnable);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_REQ
void *CsrBtScEncryptionReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScEncryptionReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScEncryptionReq *) CsrPmemZalloc(sizeof(CsrBtScEncryptionReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->encEnable, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_RES
CsrSize CsrBtScSspJustworksResSizeof(void *msg)
{
    CsrBtScSspJustworksRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_RES */

#ifndef EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_RES
CsrUint8 *CsrBtScSspJustworksResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSspJustworksRes *primitive;

    primitive = (CsrBtScSspJustworksRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authorised);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addDevice);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_RES */

#ifndef EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_RES
void *CsrBtScSspJustworksResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSspJustworksRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSspJustworksRes *) CsrPmemZalloc(sizeof(CsrBtScSspJustworksRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authorised, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addDevice, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_RES */

#ifndef EXCLUDE_CSR_BT_SC_REBOND_RES
CsrSize CsrBtScRebondResSizeof(void *msg)
{
    CsrBtScRebondRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_REBOND_RES */

#ifndef EXCLUDE_CSR_BT_SC_REBOND_RES
CsrUint8 *CsrBtScRebondResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScRebondRes *primitive;

    primitive = (CsrBtScRebondRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_REBOND_RES */

#ifndef EXCLUDE_CSR_BT_SC_REBOND_RES
void *CsrBtScRebondResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScRebondRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScRebondRes *) CsrPmemZalloc(sizeof(CsrBtScRebondRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_REBOND_RES */

#ifndef EXCLUDE_CSR_BT_SC_LE_KEY_DISTRIBUTION_REQ
CsrSize CsrBtScLeKeyDistributionReqSizeof(void *msg)
{
    CsrBtScLeKeyDistributionReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_LE_KEY_DISTRIBUTION_REQ */

#ifndef EXCLUDE_CSR_BT_SC_LE_KEY_DISTRIBUTION_REQ
CsrUint8 *CsrBtScLeKeyDistributionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScLeKeyDistributionReq *primitive;

    primitive = (CsrBtScLeKeyDistributionReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->keyDistribution);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_LE_KEY_DISTRIBUTION_REQ */

#ifndef EXCLUDE_CSR_BT_SC_LE_KEY_DISTRIBUTION_REQ
void *CsrBtScLeKeyDistributionReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScLeKeyDistributionReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScLeKeyDistributionReq *) CsrPmemZalloc(sizeof(CsrBtScLeKeyDistributionReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->keyDistribution, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_LE_KEY_DISTRIBUTION_REQ */

#ifndef EXCLUDE_CSR_BT_SC_AUTHENTICATE_CFM
CsrSize CsrBtScAuthenticateCfmSizeof(void *msg)
{
    CsrBtScAuthenticateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_AUTHENTICATE_CFM
CsrUint8 *CsrBtScAuthenticateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScAuthenticateCfm *primitive;

    primitive = (CsrBtScAuthenticateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_AUTHENTICATE_CFM
void *CsrBtScAuthenticateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScAuthenticateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScAuthenticateCfm *) CsrPmemZalloc(sizeof(CsrBtScAuthenticateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHENTICATE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_IND
CsrSize CsrBtScSspJustworksIndSizeof(void *msg)
{
    CsrBtScSspJustworksInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->deviceName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_IND
CsrUint8 *CsrBtScSspJustworksIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSspJustworksInd *primitive;

    primitive = (CsrBtScSspJustworksInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->cod);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->deviceName, sizeof(primitive->deviceName));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->paired);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->authRequirements);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->initiator);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->localAuthRequirements);
    CsrUtf8StringSer(buffer, length, primitive->deviceName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_IND
void *CsrBtScSspJustworksIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSspJustworksInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSspJustworksInd *) CsrPmemZalloc(sizeof(CsrBtScSspJustworksInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->cod, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->deviceName), buffer, &offset, ((CsrSize) sizeof(primitive->deviceName)));
    CsrUint8Des((CsrUint8 *) &primitive->paired, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->authRequirements, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->initiator, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->localAuthRequirements, buffer, &offset);
    CsrUtf8StringDes(&primitive->deviceName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_IND
void CsrBtScSspJustworksIndSerFree(void *msg)
{
    CsrBtScSspJustworksInd *primitive;

    primitive = (CsrBtScSspJustworksInd *) msg;

    if(primitive->deviceName != NULL)
    {
        CsrPmemFree(primitive->deviceName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_JUSTWORKS_IND */

#ifndef EXCLUDE_CSR_BT_SC_BOND_REQ
CsrSize CsrBtScBondReqSizeof(void *msg)
{
    CsrBtScBondReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_BOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_BOND_REQ
CsrUint8 *CsrBtScBondReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScBondReq *primitive;

    primitive = (CsrBtScBondReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_BOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_BOND_REQ
void *CsrBtScBondReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScBondReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScBondReq *) CsrPmemZalloc(sizeof(CsrBtScBondReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_BOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_PASSKEY_IND
CsrSize CsrBtScPasskeyIndSizeof(void *msg)
{
    CsrBtScPasskeyInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->deviceName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_PASSKEY_IND */

#ifndef EXCLUDE_CSR_BT_SC_PASSKEY_IND
CsrUint8 *CsrBtScPasskeyIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScPasskeyInd *primitive;

    primitive = (CsrBtScPasskeyInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->cod);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->deviceName, sizeof(primitive->deviceName));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->paired);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->initiator);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    CsrUtf8StringSer(buffer, length, primitive->deviceName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_PASSKEY_IND */

#ifndef EXCLUDE_CSR_BT_SC_PASSKEY_IND
void *CsrBtScPasskeyIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScPasskeyInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScPasskeyInd *) CsrPmemZalloc(sizeof(CsrBtScPasskeyInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->cod, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->deviceName), buffer, &offset, ((CsrSize) sizeof(primitive->deviceName)));
    CsrUint8Des((CsrUint8 *) &primitive->paired, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->initiator, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    CsrUtf8StringDes(&primitive->deviceName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_PASSKEY_IND */

#ifndef EXCLUDE_CSR_BT_SC_PASSKEY_IND
void CsrBtScPasskeyIndSerFree(void *msg)
{
    CsrBtScPasskeyInd *primitive;

    primitive = (CsrBtScPasskeyInd *) msg;

    if(primitive->deviceName != NULL)
    {
        CsrPmemFree(primitive->deviceName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SC_PASSKEY_IND */

#ifndef EXCLUDE_CSR_BT_SC_DATABASE_CFM
CsrSize CsrBtScDatabaseCfmSizeof(void *msg)
{
    CsrBtScDatabaseCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_DATABASE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_DATABASE_CFM
CsrUint8 *CsrBtScDatabaseCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScDatabaseCfm *primitive;

    primitive = (CsrBtScDatabaseCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->opcode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->entry);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->keyType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_DATABASE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_DATABASE_CFM
void *CsrBtScDatabaseCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScDatabaseCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScDatabaseCfm *) CsrPmemZalloc(sizeof(CsrBtScDatabaseCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->opcode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->entry, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->keyType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    if (primitive->length)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_DATABASE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_DATABASE_CFM
void CsrBtScDatabaseCfmSerFree(void *msg)
{
    CsrBtScDatabaseCfm *primitive;

    primitive = (CsrBtScDatabaseCfm *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SC_DATABASE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_SET_TRUST_LEVEL_REQ
CsrSize CsrBtScSetTrustLevelReqSizeof(void *msg)
{
    CsrBtScSetTrustLevelReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SET_TRUST_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SET_TRUST_LEVEL_REQ
CsrUint8 *CsrBtScSetTrustLevelReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSetTrustLevelReq *primitive;

    primitive = (CsrBtScSetTrustLevelReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authorised);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SET_TRUST_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SET_TRUST_LEVEL_REQ
void *CsrBtScSetTrustLevelReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSetTrustLevelReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSetTrustLevelReq *) CsrPmemZalloc(sizeof(CsrBtScSetTrustLevelReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authorised, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SET_TRUST_LEVEL_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SET_EVENT_MASK_REQ
CsrSize CsrBtScSetEventMaskReqSizeof(void *msg)
{
    CsrBtScSetEventMaskReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SET_EVENT_MASK_REQ
CsrUint8 *CsrBtScSetEventMaskReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSetEventMaskReq *primitive;

    primitive = (CsrBtScSetEventMaskReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->eventMask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SET_EVENT_MASK_REQ
void *CsrBtScSetEventMaskReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSetEventMaskReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSetEventMaskReq *) CsrPmemZalloc(sizeof(CsrBtScSetEventMaskReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->eventMask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SET_EVENT_MASK_REQ */

#ifndef EXCLUDE_CSR_BT_SC_DATABASE_REQ
CsrSize CsrBtScDatabaseReqSizeof(void *msg)
{
    CsrBtScDatabaseReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_DATABASE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_DATABASE_REQ
CsrUint8 *CsrBtScDatabaseReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScDatabaseReq *primitive;

    primitive = (CsrBtScDatabaseReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->opcode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->entry);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->keyType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_DATABASE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_DATABASE_REQ
void *CsrBtScDatabaseReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScDatabaseReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScDatabaseReq *) CsrPmemZalloc(sizeof(CsrBtScDatabaseReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->opcode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->entry, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->keyType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    if (primitive->length)
    {
        primitive->data = CsrPmemZalloc(((CsrUint32) (primitive->length * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->data), buffer, &offset, ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }
    else
    {
        primitive->data = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_DATABASE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_DATABASE_REQ
void CsrBtScDatabaseReqSerFree(void *msg)
{
    CsrBtScDatabaseReq *primitive;

    primitive = (CsrBtScDatabaseReq *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SC_DATABASE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SSP_PAIRING_IND
CsrSize CsrBtScSspPairingIndSizeof(void *msg)
{
    CsrBtScSspPairingInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->deviceName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_PAIRING_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_PAIRING_IND
CsrUint8 *CsrBtScSspPairingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSspPairingInd *primitive;

    primitive = (CsrBtScSspPairingInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->cod);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->deviceName, sizeof(primitive->deviceName));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authValid);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->authRequirements);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->paired);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->initiator);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->localAuthRequirements);
    CsrUtf8StringSer(buffer, length, primitive->deviceName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_PAIRING_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_PAIRING_IND
void *CsrBtScSspPairingIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSspPairingInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSspPairingInd *) CsrPmemZalloc(sizeof(CsrBtScSspPairingInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->cod, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->deviceName), buffer, &offset, ((CsrSize) sizeof(primitive->deviceName)));
    CsrUint8Des((CsrUint8 *) &primitive->authValid, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->authRequirements, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->paired, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->initiator, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->localAuthRequirements, buffer, &offset);
    CsrUtf8StringDes(&primitive->deviceName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_PAIRING_IND */

#ifndef EXCLUDE_CSR_BT_SC_SSP_PAIRING_IND
void CsrBtScSspPairingIndSerFree(void *msg)
{
    CsrBtScSspPairingInd *primitive;

    primitive = (CsrBtScSspPairingInd *) msg;

    if(primitive->deviceName != NULL)
    {
        CsrPmemFree(primitive->deviceName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_PAIRING_IND */

#ifndef EXCLUDE_CSR_BT_SC_DEBUG_MODE_CFM
CsrSize CsrBtScDebugModeCfmSizeof(void *msg)
{
    CsrBtScDebugModeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_DEBUG_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_DEBUG_MODE_CFM
CsrUint8 *CsrBtScDebugModeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScDebugModeCfm *primitive;

    primitive = (CsrBtScDebugModeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enabled);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_DEBUG_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_DEBUG_MODE_CFM
void *CsrBtScDebugModeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScDebugModeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScDebugModeCfm *) CsrPmemZalloc(sizeof(CsrBtScDebugModeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enabled, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_DEBUG_MODE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_DEBOND_REQ
CsrSize CsrBtScDebondReqSizeof(void *msg)
{
    CsrBtScDebondReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_DEBOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_DEBOND_REQ
CsrUint8 *CsrBtScDebondReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScDebondReq *primitive;

    primitive = (CsrBtScDebondReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_DEBOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_DEBOND_REQ
void *CsrBtScDebondReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScDebondReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScDebondReq *) CsrPmemZalloc(sizeof(CsrBtScDebondReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_DEBOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM
CsrSize CsrBtScEncryptionKeySizeCfmSizeof(void *msg)
{
    CsrBtScEncryptionKeySizeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM
CsrUint8 *CsrBtScEncryptionKeySizeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScEncryptionKeySizeCfm *primitive;

    primitive = (CsrBtScEncryptionKeySizeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->address.addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->address.addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->address.addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->keySize);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM
void *CsrBtScEncryptionKeySizeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScEncryptionKeySizeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScEncryptionKeySizeCfm *) CsrPmemZalloc(sizeof(CsrBtScEncryptionKeySizeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->address.addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->address.addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->address.addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->keySize, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_ENCRYPTION_KEY_SIZE_CFM */

#ifndef EXCLUDE_CSR_BT_SC_REBOND_IND
CsrSize CsrBtScRebondIndSizeof(void *msg)
{
    CsrBtScRebondInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->deviceName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_REBOND_IND */

#ifndef EXCLUDE_CSR_BT_SC_REBOND_IND
CsrUint8 *CsrBtScRebondIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScRebondInd *primitive;

    primitive = (CsrBtScRebondInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->cod);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->deviceName, sizeof(primitive->deviceName));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    CsrUtf8StringSer(buffer, length, primitive->deviceName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_REBOND_IND */

#ifndef EXCLUDE_CSR_BT_SC_REBOND_IND
void *CsrBtScRebondIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScRebondInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScRebondInd *) CsrPmemZalloc(sizeof(CsrBtScRebondInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->cod, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->deviceName), buffer, &offset, ((CsrSize) sizeof(primitive->deviceName)));
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    CsrUtf8StringDes(&primitive->deviceName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_REBOND_IND */

#ifndef EXCLUDE_CSR_BT_SC_REBOND_IND
void CsrBtScRebondIndSerFree(void *msg)
{
    CsrBtScRebondInd *primitive;

    primitive = (CsrBtScRebondInd *) msg;

    if(primitive->deviceName != NULL)
    {
        CsrPmemFree(primitive->deviceName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SC_REBOND_IND */

#ifndef EXCLUDE_CSR_BT_SC_DEBOND_CFM
CsrSize CsrBtScDebondCfmSizeof(void *msg)
{
    CsrBtScDebondCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_DEBOND_CFM */

#ifndef EXCLUDE_CSR_BT_SC_DEBOND_CFM
CsrUint8 *CsrBtScDebondCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScDebondCfm *primitive;

    primitive = (CsrBtScDebondCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_DEBOND_CFM */

#ifndef EXCLUDE_CSR_BT_SC_DEBOND_CFM
void *CsrBtScDebondCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScDebondCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScDebondCfm *) CsrPmemZalloc(sizeof(CsrBtScDebondCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_DEBOND_CFM */

#ifndef EXCLUDE_CSR_BT_SC_UPDATE_DEVICE_REQ
CsrSize CsrBtScUpdateDeviceReqSizeof(void *msg)
{
    CsrBtScUpdateDeviceReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->remoteName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_UPDATE_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_UPDATE_DEVICE_REQ
CsrUint8 *CsrBtScUpdateDeviceReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScUpdateDeviceReq *primitive;

    primitive = (CsrBtScUpdateDeviceReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->remoteName, sizeof(primitive->remoteName));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->classOfDevice);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->knownServices11_00_31);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->knownServices11_32_63);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->knownServices12_00_31);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->knownServices13_00_31);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authorised);
    CsrUtf8StringSer(buffer, length, primitive->remoteName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_UPDATE_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_UPDATE_DEVICE_REQ
void *CsrBtScUpdateDeviceReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScUpdateDeviceReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScUpdateDeviceReq *) CsrPmemZalloc(sizeof(CsrBtScUpdateDeviceReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->remoteName), buffer, &offset, ((CsrSize) sizeof(primitive->remoteName)));
    CsrUint32Des((CsrUint32 *) &primitive->classOfDevice, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->knownServices11_00_31, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->knownServices11_32_63, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->knownServices12_00_31, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->knownServices13_00_31, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authorised, buffer, &offset);
    CsrUtf8StringDes(&primitive->remoteName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_UPDATE_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_UPDATE_DEVICE_REQ
void CsrBtScUpdateDeviceReqSerFree(void *msg)
{
    CsrBtScUpdateDeviceReq *primitive;

    primitive = (CsrBtScUpdateDeviceReq *) msg;

    if(primitive->remoteName != NULL)
    {
        CsrPmemFree(primitive->remoteName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SC_UPDATE_DEVICE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_UPDATE_TRUST_LEVEL_IND
CsrSize CsrBtScUpdateTrustLevelIndSizeof(void *msg)
{
    CsrBtScUpdateTrustLevelInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_UPDATE_TRUST_LEVEL_IND */

#ifndef EXCLUDE_CSR_BT_SC_UPDATE_TRUST_LEVEL_IND
CsrUint8 *CsrBtScUpdateTrustLevelIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScUpdateTrustLevelInd *primitive;

    primitive = (CsrBtScUpdateTrustLevelInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authorised);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_UPDATE_TRUST_LEVEL_IND */

#ifndef EXCLUDE_CSR_BT_SC_UPDATE_TRUST_LEVEL_IND
void *CsrBtScUpdateTrustLevelIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScUpdateTrustLevelInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScUpdateTrustLevelInd *) CsrPmemZalloc(sizeof(CsrBtScUpdateTrustLevelInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authorised, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_UPDATE_TRUST_LEVEL_IND */

#ifndef EXCLUDE_CSR_BT_SC_SET_ENCRYPTION_KEY_SIZE_REQ
CsrSize CsrBtScSetEncryptionKeySizeReqSizeof(void *msg)
{
    CsrBtScSetEncryptionKeySizeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SET_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SET_ENCRYPTION_KEY_SIZE_REQ
CsrUint8 *CsrBtScSetEncryptionKeySizeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSetEncryptionKeySizeReq *primitive;

    primitive = (CsrBtScSetEncryptionKeySizeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->minKeySize);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->maxKeySize);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SET_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SET_ENCRYPTION_KEY_SIZE_REQ
void *CsrBtScSetEncryptionKeySizeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSetEncryptionKeySizeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSetEncryptionKeySizeReq *) CsrPmemZalloc(sizeof(CsrBtScSetEncryptionKeySizeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->minKeySize, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->maxKeySize, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SET_ENCRYPTION_KEY_SIZE_REQ */

#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_IND
CsrSize CsrBtScReadDeviceRecordIndSizeof(void *msg)
{
    CsrBtScReadDeviceRecordInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->devicePropertiesLength * sizeof(CsrBtDevicePropertiesType)) + (primitive->serviceChangedListLength * sizeof(CsrUint32)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_IND */

#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_IND
CsrUint8 *CsrBtScReadDeviceRecordIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScReadDeviceRecordInd *primitive;

    primitive = (CsrBtScReadDeviceRecordInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->devicePropertiesLength);
    CsrMemCpySer(buffer, length, &primitive->deviceProperties, sizeof(primitive->deviceProperties));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->serviceChangedListLength);
    CsrMemCpySer(buffer, length, &primitive->serviceChangedList, sizeof(primitive->serviceChangedList));
    if(primitive->deviceProperties)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->deviceProperties), ((CsrSize) (primitive->devicePropertiesLength * sizeof(CsrBtDevicePropertiesType))));
    }
    if(primitive->serviceChangedList)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->serviceChangedList), ((CsrSize) (primitive->serviceChangedListLength * sizeof(CsrUint32))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_IND */

#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_IND
void *CsrBtScReadDeviceRecordIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScReadDeviceRecordInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScReadDeviceRecordInd *) CsrPmemZalloc(sizeof(CsrBtScReadDeviceRecordInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->devicePropertiesLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->deviceProperties), buffer, &offset, ((CsrSize) sizeof(primitive->deviceProperties)));
    CsrUint32Des((CsrUint32 *) &primitive->serviceChangedListLength, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceChangedList), buffer, &offset, ((CsrSize) sizeof(primitive->serviceChangedList)));
    if (primitive->devicePropertiesLength)
    {
        primitive->deviceProperties = CsrPmemZalloc(((CsrUint32) (primitive->devicePropertiesLength * sizeof(CsrBtDevicePropertiesType))));
        CsrMemCpyDes(((void *) primitive->deviceProperties), buffer, &offset, ((CsrSize) (primitive->devicePropertiesLength * sizeof(CsrBtDevicePropertiesType))));
    }
    else
    {
        primitive->deviceProperties = NULL;
    }
    if (primitive->serviceChangedListLength)
    {
        primitive->serviceChangedList = CsrPmemZalloc(((CsrUint32) (primitive->serviceChangedListLength * sizeof(CsrUint32))));
        CsrMemCpyDes(((void *) primitive->serviceChangedList), buffer, &offset, ((CsrSize) (primitive->serviceChangedListLength * sizeof(CsrUint32))));
    }
    else
    {
        primitive->serviceChangedList = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_IND */

#ifndef EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_IND
void CsrBtScReadDeviceRecordIndSerFree(void *msg)
{
    CsrBtScReadDeviceRecordInd *primitive;

    primitive = (CsrBtScReadDeviceRecordInd *) msg;

    if(primitive->deviceProperties != NULL)
    {
        CsrPmemFree(primitive->deviceProperties);
    }
    if(primitive->serviceChangedList != NULL)
    {
        CsrPmemFree(primitive->serviceChangedList);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SC_READ_DEVICE_RECORD_IND */

#ifndef EXCLUDE_CSR_BT_SC_CONFIG_REQ
CsrSize CsrBtScConfigReqSizeof(void *msg)
{
    CsrBtScConfigReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_SC_CONFIG_REQ
CsrUint8 *CsrBtScConfigReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScConfigReq *primitive;

    primitive = (CsrBtScConfigReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->configMask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_SC_CONFIG_REQ
void *CsrBtScConfigReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScConfigReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScConfigReq *) CsrPmemZalloc(sizeof(CsrBtScConfigReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->configMask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_CONFIG_REQ */

#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_RES
CsrSize CsrBtScAuthoriseResSizeof(void *msg)
{
    CsrBtScAuthoriseRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_RES */

#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_RES
CsrUint8 *CsrBtScAuthoriseResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScAuthoriseRes *primitive;

    primitive = (CsrBtScAuthoriseRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authorised);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_RES */

#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_RES
void *CsrBtScAuthoriseResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScAuthoriseRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScAuthoriseRes *) CsrPmemZalloc(sizeof(CsrBtScAuthoriseRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authorised, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_RES */

#ifndef EXCLUDE_CSR_BT_SC_CANCEL_BOND_REQ
CsrSize CsrBtScCancelBondReqSizeof(void *msg)
{
    CsrBtScCancelBondReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_CANCEL_BOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_CANCEL_BOND_REQ
CsrUint8 *CsrBtScCancelBondReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScCancelBondReq *primitive;

    primitive = (CsrBtScCancelBondReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_CANCEL_BOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_CANCEL_BOND_REQ
void *CsrBtScCancelBondReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScCancelBondReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScCancelBondReq *) CsrPmemZalloc(sizeof(CsrBtScCancelBondReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_CANCEL_BOND_REQ */

#ifndef EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_RES
CsrSize CsrBtScSspNotificationResSizeof(void *msg)
{
    CsrBtScSspNotificationRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_RES */

#ifndef EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_RES
CsrUint8 *CsrBtScSspNotificationResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScSspNotificationRes *primitive;

    primitive = (CsrBtScSspNotificationRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->authorised);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addDevice);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_RES */

#ifndef EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_RES
void *CsrBtScSspNotificationResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScSspNotificationRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScSspNotificationRes *) CsrPmemZalloc(sizeof(CsrBtScSspNotificationRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->authorised, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addDevice, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_SSP_NOTIFICATION_RES */

#ifndef EXCLUDE_CSR_BT_SC_HOUSE_CLEANING
CsrSize CsrBtScHouseCleaningSizeof(void *msg)
{
    CsrBtScHouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SC_HOUSE_CLEANING
CsrUint8 *CsrBtScHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScHouseCleaning *primitive;

    primitive = (CsrBtScHouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SC_HOUSE_CLEANING
void *CsrBtScHouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScHouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScHouseCleaning *) CsrPmemZalloc(sizeof(CsrBtScHouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_SC_DEREGISTER_REQ
CsrSize CsrBtScDeregisterReqSizeof(void *msg)
{
    CsrBtScDeregisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_DEREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_SC_DEREGISTER_REQ
CsrUint8 *CsrBtScDeregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScDeregisterReq *primitive;

    primitive = (CsrBtScDeregisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->protocolId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->channel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_DEREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_SC_DEREGISTER_REQ
void *CsrBtScDeregisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScDeregisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScDeregisterReq *) CsrPmemZalloc(sizeof(CsrBtScDeregisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->protocolId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->channel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_DEREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ADD_REMOTE_OOB_DATA_REQ
CsrSize CsrBtScAddRemoteOobDataReqSizeof(void *msg)
{
    CsrBtScAddRemoteOobDataReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_ADD_REMOTE_OOB_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ADD_REMOTE_OOB_DATA_REQ
CsrUint8 *CsrBtScAddRemoteOobDataReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScAddRemoteOobDataReq *primitive;

    primitive = (CsrBtScAddRemoteOobDataReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, ((const void *) primitive->oobHashC), ((CsrSize) (16)));
    CsrMemCpySer(buffer, length, ((const void *) primitive->oobRandR), ((CsrSize) (16)));

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_ADD_REMOTE_OOB_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_SC_ADD_REMOTE_OOB_DATA_REQ
void *CsrBtScAddRemoteOobDataReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScAddRemoteOobDataReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScAddRemoteOobDataReq *) CsrPmemZalloc(sizeof(CsrBtScAddRemoteOobDataReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) primitive->oobHashC), buffer, &offset, ((CsrSize) (16)));
    CsrMemCpyDes(((void *) primitive->oobRandR), buffer, &offset, ((CsrSize) (16)));

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_ADD_REMOTE_OOB_DATA_REQ */

#ifndef EXCLUDE_CSR_BT_SC_MODE_IND
CsrSize CsrBtScModeIndSizeof(void *msg)
{
    CsrBtScModeInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_MODE_IND */

#ifndef EXCLUDE_CSR_BT_SC_MODE_IND
CsrUint8 *CsrBtScModeIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScModeInd *primitive;

    primitive = (CsrBtScModeInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_MODE_IND */

#ifndef EXCLUDE_CSR_BT_SC_MODE_IND
void *CsrBtScModeIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScModeInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScModeInd *) CsrPmemZalloc(sizeof(CsrBtScModeInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_MODE_IND */

#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_EXT_IND
CsrSize CsrBtScAuthoriseExtIndSizeof(void *msg)
{
    CsrBtScAuthoriseExtInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->deviceName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_EXT_IND */

#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_EXT_IND
CsrUint8 *CsrBtScAuthoriseExtIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScAuthoriseExtInd *primitive;

    primitive = (CsrBtScAuthoriseExtInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->deviceName, sizeof(primitive->deviceName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->serviceId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->protocolId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->channel);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->incoming);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    CsrUtf8StringSer(buffer, length, primitive->deviceName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_EXT_IND */

#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_EXT_IND
void *CsrBtScAuthoriseExtIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScAuthoriseExtInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScAuthoriseExtInd *) CsrPmemZalloc(sizeof(CsrBtScAuthoriseExtInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->deviceName), buffer, &offset, ((CsrSize) sizeof(primitive->deviceName)));
    CsrUint16Des((CsrUint16 *) &primitive->serviceId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->protocolId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->channel, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->incoming, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    CsrUtf8StringDes(&primitive->deviceName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_EXT_IND */

#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_EXT_IND
void CsrBtScAuthoriseExtIndSerFree(void *msg)
{
    CsrBtScAuthoriseExtInd *primitive;

    primitive = (CsrBtScAuthoriseExtInd *) msg;

    if(primitive->deviceName != NULL)
    {
        CsrPmemFree(primitive->deviceName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_EXT_IND */

#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_IND
CsrSize CsrBtScAuthoriseIndSizeof(void *msg)
{
    CsrBtScAuthoriseInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrUtf8StringSerLen((CsrUtf8String*) primitive->deviceName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_IND */

#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_IND
CsrUint8 *CsrBtScAuthoriseIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtScAuthoriseInd *primitive;

    primitive = (CsrBtScAuthoriseInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrMemCpySer(buffer, length, &primitive->deviceName, sizeof(primitive->deviceName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->serviceId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addressType);
    CsrUtf8StringSer(buffer, length, primitive->deviceName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_IND */

#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_IND
void *CsrBtScAuthoriseIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtScAuthoriseInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtScAuthoriseInd *) CsrPmemZalloc(sizeof(CsrBtScAuthoriseInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->deviceName), buffer, &offset, ((CsrSize) sizeof(primitive->deviceName)));
    CsrUint16Des((CsrUint16 *) &primitive->serviceId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addressType, buffer, &offset);
    CsrUtf8StringDes(&primitive->deviceName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_IND */

#ifndef EXCLUDE_CSR_BT_SC_AUTHORISE_IND
void CsrBtScAuthoriseIndSerFree(void *msg)
{
    CsrBtScAuthoriseInd *primitive;

    primitive = (CsrBtScAuthoriseInd *) msg;

    if(primitive->deviceName != NULL)
    {
        CsrPmemFree(primitive->deviceName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_SC_AUTHORISE_IND */

#endif /* EXCLUDE_CSR_BT_SC_PRIVATE_MODULE */
