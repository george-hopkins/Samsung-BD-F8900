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
#include "csr_bt_ampm_prim.h"

#ifndef EXCLUDE_CSR_BT_AMPM_MODULE

void CsrBtAmpmPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_CHANGE_AMP_KEY_REQ
CsrSize CsrBtAmpmTestChangeAmpKeyReqSizeof(void *msg)
{
    CsrBtAmpmTestChangeAmpKeyReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_CHANGE_AMP_KEY_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_CHANGE_AMP_KEY_REQ
CsrUint8 *CsrBtAmpmTestChangeAmpKeyReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmTestChangeAmpKeyReq *primitive;

    primitive = (CsrBtAmpmTestChangeAmpKeyReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enabled);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ampType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->changeTimes);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_CHANGE_AMP_KEY_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_CHANGE_AMP_KEY_REQ
void *CsrBtAmpmTestChangeAmpKeyReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmTestChangeAmpKeyReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmTestChangeAmpKeyReq *) CsrPmemZalloc(sizeof(CsrBtAmpmTestChangeAmpKeyReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enabled, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ampType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->changeTimes, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_CHANGE_AMP_KEY_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_CFM
CsrSize CsrBtAmpmControllerCfmSizeof(void *msg)
{
    CsrBtAmpmControllerCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->ampsCount * sizeof(CsrBtAmpmControllerEntry)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_CFM
CsrUint8 *CsrBtAmpmControllerCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmControllerCfm *primitive;

    primitive = (CsrBtAmpmControllerCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ampsCount);
    CsrMemCpySer(buffer, length, &primitive->amps, sizeof(primitive->amps));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    if(primitive->amps)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->amps), ((CsrSize) (primitive->ampsCount * sizeof(CsrBtAmpmControllerEntry))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_CFM
void *CsrBtAmpmControllerCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmControllerCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmControllerCfm *) CsrPmemZalloc(sizeof(CsrBtAmpmControllerCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ampsCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->amps), buffer, &offset, ((CsrSize) sizeof(primitive->amps)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    if (primitive->ampsCount)
    {
        primitive->amps = CsrPmemZalloc(((CsrUint32) (primitive->ampsCount * sizeof(CsrBtAmpmControllerEntry))));
        CsrMemCpyDes(((void *) primitive->amps), buffer, &offset, ((CsrSize) (primitive->ampsCount * sizeof(CsrBtAmpmControllerEntry))));
    }
    else
    {
        primitive->amps = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_CFM
void CsrBtAmpmControllerCfmSerFree(void *msg)
{
    CsrBtAmpmControllerCfm *primitive;

    primitive = (CsrBtAmpmControllerCfm *) msg;

    if(primitive->amps != NULL)
    {
        CsrPmemFree(primitive->amps);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_PHY_COLLISION_REQ
CsrSize CsrBtAmpmTestPhyCollisionReqSizeof(void *msg)
{
    CsrBtAmpmTestPhyCollisionReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_PHY_COLLISION_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_PHY_COLLISION_REQ
CsrUint8 *CsrBtAmpmTestPhyCollisionReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmTestPhyCollisionReq *primitive;

    primitive = (CsrBtAmpmTestPhyCollisionReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enabled);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_PHY_COLLISION_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_PHY_COLLISION_REQ
void *CsrBtAmpmTestPhyCollisionReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmTestPhyCollisionReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmTestPhyCollisionReq *) CsrPmemZalloc(sizeof(CsrBtAmpmTestPhyCollisionReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enabled, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_PHY_COLLISION_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_REQ
CsrSize CsrBtAmpmPalRegisterReqSizeof(void *msg)
{
    CsrBtAmpmPalRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_REQ
CsrUint8 *CsrBtAmpmPalRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmPalRegisterReq *primitive;

    primitive = (CsrBtAmpmPalRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ampType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->controlQid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataQid);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_REQ
void *CsrBtAmpmPalRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmPalRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmPalRegisterReq *) CsrPmemZalloc(sizeof(CsrBtAmpmPalRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ampType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->controlQid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataQid, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_POWER_ON_RES
CsrSize CsrBtAmpmPowerOnResSizeof(void *msg)
{
    CsrBtAmpmPowerOnRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_POWER_ON_RES */

#ifndef EXCLUDE_CSR_BT_AMPM_POWER_ON_RES
CsrUint8 *CsrBtAmpmPowerOnResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmPowerOnRes *primitive;

    primitive = (CsrBtAmpmPowerOnRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ampType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->complete);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_POWER_ON_RES */

#ifndef EXCLUDE_CSR_BT_AMPM_POWER_ON_RES
void *CsrBtAmpmPowerOnResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmPowerOnRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmPowerOnRes *) CsrPmemZalloc(sizeof(CsrBtAmpmPowerOnRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ampType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->complete, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_POWER_ON_RES */

#ifndef EXCLUDE_CSR_BT_AMPM_POWER_ON_IND
CsrSize CsrBtAmpmPowerOnIndSizeof(void *msg)
{
    CsrBtAmpmPowerOnInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_POWER_ON_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_POWER_ON_IND
CsrUint8 *CsrBtAmpmPowerOnIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmPowerOnInd *primitive;

    primitive = (CsrBtAmpmPowerOnInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ampType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localInit);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_POWER_ON_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_POWER_ON_IND
void *CsrBtAmpmPowerOnIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmPowerOnInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmPowerOnInd *) CsrPmemZalloc(sizeof(CsrBtAmpmPowerOnInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ampType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localInit, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_POWER_ON_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_HCI_IND
CsrSize CsrBtAmpmTestHciIndSizeof(void *msg)
{
    CsrBtAmpmTestHciInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_HCI_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_HCI_IND
CsrUint8 *CsrBtAmpmTestHciIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmTestHciInd *primitive;

    primitive = (CsrBtAmpmTestHciInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ampId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_HCI_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_HCI_IND
void *CsrBtAmpmTestHciIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmTestHciInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmTestHciInd *) CsrPmemZalloc(sizeof(CsrBtAmpmTestHciInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ampId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
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
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_HCI_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_HCI_IND
void CsrBtAmpmTestHciIndSerFree(void *msg)
{
    CsrBtAmpmTestHciInd *primitive;

    primitive = (CsrBtAmpmTestHciInd *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_HCI_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_IGNORE_SDU_REQ
CsrSize CsrBtAmpmTestIgnoreSduReqSizeof(void *msg)
{
    CsrBtAmpmTestIgnoreSduReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_IGNORE_SDU_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_IGNORE_SDU_REQ
CsrUint8 *CsrBtAmpmTestIgnoreSduReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmTestIgnoreSduReq *primitive;

    primitive = (CsrBtAmpmTestIgnoreSduReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enabled);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sduId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ignoreTimes);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_IGNORE_SDU_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_IGNORE_SDU_REQ
void *CsrBtAmpmTestIgnoreSduReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmTestIgnoreSduReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmTestIgnoreSduReq *) CsrPmemZalloc(sizeof(CsrBtAmpmTestIgnoreSduReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enabled, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->sduId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ignoreTimes, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_IGNORE_SDU_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_ROUTE_RECEIVED_SDU_REQ
CsrSize CsrBtAmpmTestRouteReceivedSduReqSizeof(void *msg)
{
    CsrBtAmpmTestRouteReceivedSduReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_ROUTE_RECEIVED_SDU_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_ROUTE_RECEIVED_SDU_REQ
CsrUint8 *CsrBtAmpmTestRouteReceivedSduReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmTestRouteReceivedSduReq *primitive;

    primitive = (CsrBtAmpmTestRouteReceivedSduReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->rxSduControl);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qid);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_ROUTE_RECEIVED_SDU_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_ROUTE_RECEIVED_SDU_REQ
void *CsrBtAmpmTestRouteReceivedSduReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmTestRouteReceivedSduReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmTestRouteReceivedSduReq *) CsrPmemZalloc(sizeof(CsrBtAmpmTestRouteReceivedSduReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->rxSduControl, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_ROUTE_RECEIVED_SDU_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_REQ
CsrSize CsrBtAmpmControllerReqSizeof(void *msg)
{
    CsrBtAmpmControllerReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_REQ
CsrUint8 *CsrBtAmpmControllerReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmControllerReq *primitive;

    primitive = (CsrBtAmpmControllerReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qid);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_REQ
void *CsrBtAmpmControllerReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmControllerReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmControllerReq *) CsrPmemZalloc(sizeof(CsrBtAmpmControllerReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_CANCEL_CREATE_PHYSICAL_LINK_REQ
CsrSize CsrBtAmpmTestCancelCreatePhysicalLinkReqSizeof(void *msg)
{
    CsrBtAmpmTestCancelCreatePhysicalLinkReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_CANCEL_CREATE_PHYSICAL_LINK_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_CANCEL_CREATE_PHYSICAL_LINK_REQ
CsrUint8 *CsrBtAmpmTestCancelCreatePhysicalLinkReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmTestCancelCreatePhysicalLinkReq *primitive;

    primitive = (CsrBtAmpmTestCancelCreatePhysicalLinkReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enabled);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sendAfterResponse);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->invalidResponderAmpId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->delayAfterReq);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_CANCEL_CREATE_PHYSICAL_LINK_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_CANCEL_CREATE_PHYSICAL_LINK_REQ
void *CsrBtAmpmTestCancelCreatePhysicalLinkReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmTestCancelCreatePhysicalLinkReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmTestCancelCreatePhysicalLinkReq *) CsrPmemZalloc(sizeof(CsrBtAmpmTestCancelCreatePhysicalLinkReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enabled, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->sendAfterResponse, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->invalidResponderAmpId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->delayAfterReq, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_CANCEL_CREATE_PHYSICAL_LINK_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_LOG_LINK_REQ
CsrSize CsrBtAmpmTestDontStartLogLinkReqSizeof(void *msg)
{
    CsrBtAmpmTestDontStartLogLinkReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_LOG_LINK_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_LOG_LINK_REQ
CsrUint8 *CsrBtAmpmTestDontStartLogLinkReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmTestDontStartLogLinkReq *primitive;

    primitive = (CsrBtAmpmTestDontStartLogLinkReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enabled);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_LOG_LINK_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_LOG_LINK_REQ
void *CsrBtAmpmTestDontStartLogLinkReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmTestDontStartLogLinkReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmTestDontStartLogLinkReq *) CsrPmemZalloc(sizeof(CsrBtAmpmTestDontStartLogLinkReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enabled, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_LOG_LINK_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_DEREGISTER_REQ
CsrSize CsrBtAmpmDeregisterReqSizeof(void *msg)
{
    CsrBtAmpmDeregisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_DEREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_DEREGISTER_REQ
CsrUint8 *CsrBtAmpmDeregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmDeregisterReq *primitive;

    primitive = (CsrBtAmpmDeregisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qid);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_DEREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_DEREGISTER_REQ
void *CsrBtAmpmDeregisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmDeregisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmDeregisterReq *) CsrPmemZalloc(sizeof(CsrBtAmpmDeregisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_DEREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_REGISTER_POWER_ON_REQ
CsrSize CsrBtAmpmRegisterPowerOnReqSizeof(void *msg)
{
    CsrBtAmpmRegisterPowerOnReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_REGISTER_POWER_ON_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_REGISTER_POWER_ON_REQ
CsrUint8 *CsrBtAmpmRegisterPowerOnReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmRegisterPowerOnReq *primitive;

    primitive = (CsrBtAmpmRegisterPowerOnReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qid);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_REGISTER_POWER_ON_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_REGISTER_POWER_ON_REQ
void *CsrBtAmpmRegisterPowerOnReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmRegisterPowerOnReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmRegisterPowerOnReq *) CsrPmemZalloc(sizeof(CsrBtAmpmRegisterPowerOnReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_REGISTER_POWER_ON_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_RES
CsrSize CsrBtAmpmMoveResSizeof(void *msg)
{
    CsrBtAmpmMoveRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_RES */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_RES
CsrUint8 *CsrBtAmpmMoveResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmMoveRes *primitive;

    primitive = (CsrBtAmpmMoveRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->accept);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_RES */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_RES
void *CsrBtAmpmMoveResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmMoveRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmMoveRes *) CsrPmemZalloc(sizeof(CsrBtAmpmMoveRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->accept, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_RES */

#ifndef EXCLUDE_CSR_BT_AMPM_PRF_MULTI_CONN_REQ
CsrSize CsrBtAmpmPrfMultiConnReqSizeof(void *msg)
{
    CsrBtAmpmPrfMultiConnReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->slavesCount * sizeof(CsrBtConnId)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_PRF_MULTI_CONN_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PRF_MULTI_CONN_REQ
CsrUint8 *CsrBtAmpmPrfMultiConnReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmPrfMultiConnReq *primitive;

    primitive = (CsrBtAmpmPrfMultiConnReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->master);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->slavesCount);
    CsrMemCpySer(buffer, length, &primitive->slaves, sizeof(primitive->slaves));
    if(primitive->slaves)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->slaves), ((CsrSize) (primitive->slavesCount * sizeof(CsrBtConnId))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_PRF_MULTI_CONN_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PRF_MULTI_CONN_REQ
void *CsrBtAmpmPrfMultiConnReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmPrfMultiConnReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmPrfMultiConnReq *) CsrPmemZalloc(sizeof(CsrBtAmpmPrfMultiConnReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->master, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->slavesCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->slaves), buffer, &offset, ((CsrSize) sizeof(primitive->slaves)));
    if (primitive->slavesCount)
    {
        primitive->slaves = CsrPmemZalloc(((CsrUint32) (primitive->slavesCount * sizeof(CsrBtConnId))));
        CsrMemCpyDes(((void *) primitive->slaves), buffer, &offset, ((CsrSize) (primitive->slavesCount * sizeof(CsrBtConnId))));
    }
    else
    {
        primitive->slaves = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_PRF_MULTI_CONN_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PRF_MULTI_CONN_REQ
void CsrBtAmpmPrfMultiConnReqSerFree(void *msg)
{
    CsrBtAmpmPrfMultiConnReq *primitive;

    primitive = (CsrBtAmpmPrfMultiConnReq *) msg;

    if(primitive->slaves != NULL)
    {
        CsrPmemFree(primitive->slaves);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AMPM_PRF_MULTI_CONN_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_HCI_EVENT_REQ
CsrSize CsrBtAmpmPalHciEventReqSizeof(void *msg)
{
    CsrBtAmpmPalHciEventReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_HCI_EVENT_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_HCI_EVENT_REQ
CsrUint8 *CsrBtAmpmPalHciEventReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmPalHciEventReq *primitive;

    primitive = (CsrBtAmpmPalHciEventReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ampId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_HCI_EVENT_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_HCI_EVENT_REQ
void *CsrBtAmpmPalHciEventReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmPalHciEventReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmPalHciEventReq *) CsrPmemZalloc(sizeof(CsrBtAmpmPalHciEventReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ampId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
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
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_HCI_EVENT_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_HCI_EVENT_REQ
void CsrBtAmpmPalHciEventReqSerFree(void *msg)
{
    CsrBtAmpmPalHciEventReq *primitive;

    primitive = (CsrBtAmpmPalHciEventReq *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_HCI_EVENT_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_PHY_LINK_REQ
CsrSize CsrBtAmpmTestDontStartPhyLinkReqSizeof(void *msg)
{
    CsrBtAmpmTestDontStartPhyLinkReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_PHY_LINK_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_PHY_LINK_REQ
CsrUint8 *CsrBtAmpmTestDontStartPhyLinkReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmTestDontStartPhyLinkReq *primitive;

    primitive = (CsrBtAmpmTestDontStartPhyLinkReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enabled);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ampType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_PHY_LINK_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_PHY_LINK_REQ
void *CsrBtAmpmTestDontStartPhyLinkReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmTestDontStartPhyLinkReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmTestDontStartPhyLinkReq *) CsrPmemZalloc(sizeof(CsrBtAmpmTestDontStartPhyLinkReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enabled, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ampType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_DONT_START_PHY_LINK_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_DEREGISTER_CFM
CsrSize CsrBtAmpmDeregisterCfmSizeof(void *msg)
{
    CsrBtAmpmDeregisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_DEREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_DEREGISTER_CFM
CsrUint8 *CsrBtAmpmDeregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmDeregisterCfm *primitive;

    primitive = (CsrBtAmpmDeregisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_DEREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_DEREGISTER_CFM
void *CsrBtAmpmDeregisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmDeregisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmDeregisterCfm *) CsrPmemZalloc(sizeof(CsrBtAmpmDeregisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_DEREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_GET_ASSOC_REQ
CsrSize CsrBtAmpmTestGetAssocReqSizeof(void *msg)
{
    CsrBtAmpmTestGetAssocReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_GET_ASSOC_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_GET_ASSOC_REQ
CsrUint8 *CsrBtAmpmTestGetAssocReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmTestGetAssocReq *primitive;

    primitive = (CsrBtAmpmTestGetAssocReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->signalId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ampId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bdAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bdAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bdAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_GET_ASSOC_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_GET_ASSOC_REQ
void *CsrBtAmpmTestGetAssocReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmTestGetAssocReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmTestGetAssocReq *) CsrPmemZalloc(sizeof(CsrBtAmpmTestGetAssocReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->signalId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ampId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bdAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bdAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bdAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_GET_ASSOC_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_CFM
CsrSize CsrBtAmpmPalDeregisterCfmSizeof(void *msg)
{
    CsrBtAmpmPalDeregisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_CFM
CsrUint8 *CsrBtAmpmPalDeregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmPalDeregisterCfm *primitive;

    primitive = (CsrBtAmpmPalDeregisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ampId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_CFM
void *CsrBtAmpmPalDeregisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmPalDeregisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmPalDeregisterCfm *) CsrPmemZalloc(sizeof(CsrBtAmpmPalDeregisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ampId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_HCI_COMMAND_IND
CsrSize CsrBtAmpmPalHciCommandIndSizeof(void *msg)
{
    CsrBtAmpmPalHciCommandInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_HCI_COMMAND_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_HCI_COMMAND_IND
CsrUint8 *CsrBtAmpmPalHciCommandIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmPalHciCommandInd *primitive;

    primitive = (CsrBtAmpmPalHciCommandInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ampId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_HCI_COMMAND_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_HCI_COMMAND_IND
void *CsrBtAmpmPalHciCommandIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmPalHciCommandInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmPalHciCommandInd *) CsrPmemZalloc(sizeof(CsrBtAmpmPalHciCommandInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ampId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
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
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_HCI_COMMAND_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_HCI_COMMAND_IND
void CsrBtAmpmPalHciCommandIndSerFree(void *msg)
{
    CsrBtAmpmPalHciCommandInd *primitive;

    primitive = (CsrBtAmpmPalHciCommandInd *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_HCI_COMMAND_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_RECEIVED_SDU_IND
CsrSize CsrBtAmpmTestReceivedSduIndSizeof(void *msg)
{
    CsrBtAmpmTestReceivedSduInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_RECEIVED_SDU_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_RECEIVED_SDU_IND
CsrUint8 *CsrBtAmpmTestReceivedSduIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmTestReceivedSduInd *primitive;

    primitive = (CsrBtAmpmTestReceivedSduInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bdAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bdAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bdAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_RECEIVED_SDU_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_RECEIVED_SDU_IND
void *CsrBtAmpmTestReceivedSduIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmTestReceivedSduInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmTestReceivedSduInd *) CsrPmemZalloc(sizeof(CsrBtAmpmTestReceivedSduInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bdAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bdAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bdAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
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
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_RECEIVED_SDU_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_RECEIVED_SDU_IND
void CsrBtAmpmTestReceivedSduIndSerFree(void *msg)
{
    CsrBtAmpmTestReceivedSduInd *primitive;

    primitive = (CsrBtAmpmTestReceivedSduInd *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_RECEIVED_SDU_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_IND
CsrSize CsrBtAmpmControllerIndSizeof(void *msg)
{
    CsrBtAmpmControllerInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->ampsCount * sizeof(CsrBtAmpmControllerEntry)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_IND
CsrUint8 *CsrBtAmpmControllerIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmControllerInd *primitive;

    primitive = (CsrBtAmpmControllerInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ampsCount);
    CsrMemCpySer(buffer, length, &primitive->amps, sizeof(primitive->amps));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    if(primitive->amps)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->amps), ((CsrSize) (primitive->ampsCount * sizeof(CsrBtAmpmControllerEntry))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_IND
void *CsrBtAmpmControllerIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmControllerInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmControllerInd *) CsrPmemZalloc(sizeof(CsrBtAmpmControllerInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ampsCount, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->amps), buffer, &offset, ((CsrSize) sizeof(primitive->amps)));
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    if (primitive->ampsCount)
    {
        primitive->amps = CsrPmemZalloc(((CsrUint32) (primitive->ampsCount * sizeof(CsrBtAmpmControllerEntry))));
        CsrMemCpyDes(((void *) primitive->amps), buffer, &offset, ((CsrSize) (primitive->ampsCount * sizeof(CsrBtAmpmControllerEntry))));
    }
    else
    {
        primitive->amps = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_CONTROLLER_IND
void CsrBtAmpmControllerIndSerFree(void *msg)
{
    CsrBtAmpmControllerInd *primitive;

    primitive = (CsrBtAmpmControllerInd *) msg;

    if(primitive->amps != NULL)
    {
        CsrPmemFree(primitive->amps);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AMPM_CONTROLLER_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_SDU_REQ
CsrSize CsrBtAmpmTestSduReqSizeof(void *msg)
{
    CsrBtAmpmTestSduReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_SDU_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_SDU_REQ
CsrUint8 *CsrBtAmpmTestSduReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmTestSduReq *primitive;

    primitive = (CsrBtAmpmTestSduReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->bdAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bdAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->bdAddr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_SDU_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_SDU_REQ
void *CsrBtAmpmTestSduReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmTestSduReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmTestSduReq *) CsrPmemZalloc(sizeof(CsrBtAmpmTestSduReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->bdAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bdAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->bdAddr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
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
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_SDU_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_SDU_REQ
void CsrBtAmpmTestSduReqSerFree(void *msg)
{
    CsrBtAmpmTestSduReq *primitive;

    primitive = (CsrBtAmpmTestSduReq *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_SDU_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_CFM
CsrSize CsrBtAmpmPalRegisterCfmSizeof(void *msg)
{
    CsrBtAmpmPalRegisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_CFM
CsrUint8 *CsrBtAmpmPalRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmPalRegisterCfm *primitive;

    primitive = (CsrBtAmpmPalRegisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ampType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ampId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->controlQid);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->dataQid);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->flags);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_CFM
void *CsrBtAmpmPalRegisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmPalRegisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmPalRegisterCfm *) CsrPmemZalloc(sizeof(CsrBtAmpmPalRegisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ampType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ampId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->controlQid, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->dataQid, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->flags, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_CFM
CsrSize CsrBtAmpmMoveCfmSizeof(void *msg)
{
    CsrBtAmpmMoveCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_CFM
CsrUint8 *CsrBtAmpmMoveCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmMoveCfm *primitive;

    primitive = (CsrBtAmpmMoveCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_CFM
void *CsrBtAmpmMoveCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmMoveCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmMoveCfm *) CsrPmemZalloc(sizeof(CsrBtAmpmMoveCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_CMP_IND
CsrSize CsrBtAmpmMoveCmpIndSizeof(void *msg)
{
    CsrBtAmpmMoveCmpInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_CMP_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_CMP_IND
CsrUint8 *CsrBtAmpmMoveCmpIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmMoveCmpInd *primitive;

    primitive = (CsrBtAmpmMoveCmpInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_CMP_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_CMP_IND
void *CsrBtAmpmMoveCmpIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmMoveCmpInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmMoveCmpInd *) CsrPmemZalloc(sizeof(CsrBtAmpmMoveCmpInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_CMP_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_AUTO_MOVE_CMP_IND
CsrSize CsrBtAmpmAutoMoveCmpIndSizeof(void *msg)
{
    CsrBtAmpmAutoMoveCmpInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_AUTO_MOVE_CMP_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_AUTO_MOVE_CMP_IND
CsrUint8 *CsrBtAmpmAutoMoveCmpIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmAutoMoveCmpInd *primitive;

    primitive = (CsrBtAmpmAutoMoveCmpInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_AUTO_MOVE_CMP_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_AUTO_MOVE_CMP_IND
void *CsrBtAmpmAutoMoveCmpIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmAutoMoveCmpInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmAutoMoveCmpInd *) CsrPmemZalloc(sizeof(CsrBtAmpmAutoMoveCmpInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_AUTO_MOVE_CMP_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_REGISTER_REQ
CsrSize CsrBtAmpmRegisterReqSizeof(void *msg)
{
    CsrBtAmpmRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_REGISTER_REQ
CsrUint8 *CsrBtAmpmRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmRegisterReq *primitive;

    primitive = (CsrBtAmpmRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->qid);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->flags);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->eventMask);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_REGISTER_REQ
void *CsrBtAmpmRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmRegisterReq *) CsrPmemZalloc(sizeof(CsrBtAmpmRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->qid, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->flags, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->eventMask, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_REGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_REQ
CsrSize CsrBtAmpmMoveReqSizeof(void *msg)
{
    CsrBtAmpmMoveReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_REQ
CsrUint8 *CsrBtAmpmMoveReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmMoveReq *primitive;

    primitive = (CsrBtAmpmMoveReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remoteId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_REQ
void *CsrBtAmpmMoveReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmMoveReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmMoveReq *) CsrPmemZalloc(sizeof(CsrBtAmpmMoveReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remoteId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_REQ
CsrSize CsrBtAmpmPalDeregisterReqSizeof(void *msg)
{
    CsrBtAmpmPalDeregisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_REQ
CsrUint8 *CsrBtAmpmPalDeregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmPalDeregisterReq *primitive;

    primitive = (CsrBtAmpmPalDeregisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ampId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_REQ
void *CsrBtAmpmPalDeregisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmPalDeregisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmPalDeregisterReq *) CsrPmemZalloc(sizeof(CsrBtAmpmPalDeregisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ampId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_PAL_DEREGISTER_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_HCI_REQ
CsrSize CsrBtAmpmTestHciReqSizeof(void *msg)
{
    CsrBtAmpmTestHciReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->length * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_HCI_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_HCI_REQ
CsrUint8 *CsrBtAmpmTestHciReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmTestHciReq *primitive;

    primitive = (CsrBtAmpmTestHciReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->ampId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->length);
    CsrMemCpySer(buffer, length, &primitive->data, sizeof(primitive->data));
    if(primitive->data)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->data), ((CsrSize) (primitive->length * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_HCI_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_HCI_REQ
void *CsrBtAmpmTestHciReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmTestHciReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmTestHciReq *) CsrPmemZalloc(sizeof(CsrBtAmpmTestHciReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->ampId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->length, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->data), buffer, &offset, ((CsrSize) sizeof(primitive->data)));
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
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_HCI_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_HCI_REQ
void CsrBtAmpmTestHciReqSerFree(void *msg)
{
    CsrBtAmpmTestHciReq *primitive;

    primitive = (CsrBtAmpmTestHciReq *) msg;

    if(primitive->data != NULL)
    {
        CsrPmemFree(primitive->data);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_HCI_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_TIMER_CONTROL_REQ
CsrSize CsrBtAmpmTestTimerControlReqSizeof(void *msg)
{
    CsrBtAmpmTestTimerControlReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_TIMER_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_TIMER_CONTROL_REQ
CsrUint8 *CsrBtAmpmTestTimerControlReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmTestTimerControlReq *primitive;

    primitive = (CsrBtAmpmTestTimerControlReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->enabled);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_TIMER_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_TEST_TIMER_CONTROL_REQ
void *CsrBtAmpmTestTimerControlReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmTestTimerControlReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmTestTimerControlReq *) CsrPmemZalloc(sizeof(CsrBtAmpmTestTimerControlReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->enabled, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_TEST_TIMER_CONTROL_REQ */

#ifndef EXCLUDE_CSR_BT_AMPM_REGISTER_CFM
CsrSize CsrBtAmpmRegisterCfmSizeof(void *msg)
{
    CsrBtAmpmRegisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_REGISTER_CFM
CsrUint8 *CsrBtAmpmRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmRegisterCfm *primitive;

    primitive = (CsrBtAmpmRegisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->eventMask);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->addr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->addr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->addr.nap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_REGISTER_CFM
void *CsrBtAmpmRegisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmRegisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmRegisterCfm *) CsrPmemZalloc(sizeof(CsrBtAmpmRegisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->eventMask, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->addr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->addr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->addr.nap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_REGISTER_CFM */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_IND
CsrSize CsrBtAmpmMoveIndSizeof(void *msg)
{
    CsrBtAmpmMoveInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_IND
CsrUint8 *CsrBtAmpmMoveIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtAmpmMoveInd *primitive;

    primitive = (CsrBtAmpmMoveInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->localId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_IND */

#ifndef EXCLUDE_CSR_BT_AMPM_MOVE_IND
void *CsrBtAmpmMoveIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtAmpmMoveInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtAmpmMoveInd *) CsrPmemZalloc(sizeof(CsrBtAmpmMoveInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->localId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_AMPM_MOVE_IND */

#endif /* EXCLUDE_CSR_BT_AMPM_MODULE */
