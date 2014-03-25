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
#include "csr_bt_hfg_prim.h"

#ifndef EXCLUDE_CSR_BT_HFG_MODULE

void CsrBtHfgPfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_BT_HFG_CALL_WAITING_REQ
CsrSize CsrBtHfgCallWaitingReqSizeof(void *msg)
{
    CsrBtHfgCallWaitingReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->number) + CsrCharStringSerLen((CsrCharString *) primitive->name));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_WAITING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_WAITING_REQ
CsrUint8 *CsrBtHfgCallWaitingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgCallWaitingReq *primitive;

    primitive = (CsrBtHfgCallWaitingReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrMemCpySer(buffer, length, &primitive->number, sizeof(primitive->number));
    CsrMemCpySer(buffer, length, &primitive->name, sizeof(primitive->name));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->numType);
    CsrCharStringSer(buffer, length, primitive->number);
    CsrCharStringSer(buffer, length, primitive->name);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_WAITING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_WAITING_REQ
void *CsrBtHfgCallWaitingReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgCallWaitingReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgCallWaitingReq *) CsrPmemZalloc(sizeof(CsrBtHfgCallWaitingReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->number), buffer, &offset, ((CsrSize) sizeof(primitive->number)));
    CsrMemCpyDes(((void *) &primitive->name), buffer, &offset, ((CsrSize) sizeof(primitive->name)));
    CsrUint8Des((CsrUint8 *) &primitive->numType, buffer, &offset);
    CsrCharStringDes(&primitive->number, buffer, &offset);
    CsrCharStringDes(&primitive->name, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_WAITING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_WAITING_REQ
void CsrBtHfgCallWaitingReqSerFree(void *msg)
{
    CsrBtHfgCallWaitingReq *primitive;

    primitive = (CsrBtHfgCallWaitingReq *) msg;

    if(primitive->number != NULL)
    {
        CsrPmemFree(primitive->number);
    }
    if(primitive->name != NULL)
    {
        CsrPmemFree(primitive->name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_WAITING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SF_REQ
CsrSize CsrBtHfgC2cSfReqSizeof(void *msg)
{
    CsrBtHfgC2cSfReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SF_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SF_REQ
CsrUint8 *CsrBtHfgC2cSfReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgC2cSfReq *primitive;

    primitive = (CsrBtHfgC2cSfReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->number);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SF_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SF_REQ
void *CsrBtHfgC2cSfReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgC2cSfReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgC2cSfReq *) CsrPmemZalloc(sizeof(CsrBtHfgC2cSfReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->number, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SF_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_STATUS_LP_IND
CsrSize CsrBtHfgStatusLpIndSizeof(void *msg)
{
    CsrBtHfgStatusLpInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_LP_IND */

#ifndef EXCLUDE_CSR_BT_HFG_STATUS_LP_IND
CsrUint8 *CsrBtHfgStatusLpIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgStatusLpInd *primitive;

    primitive = (CsrBtHfgStatusLpInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_LP_IND */

#ifndef EXCLUDE_CSR_BT_HFG_STATUS_LP_IND
void *CsrBtHfgStatusLpIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgStatusLpInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgStatusLpInd *) CsrPmemZalloc(sizeof(CsrBtHfgStatusLpInd));

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
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_LP_IND */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_BATTERY_IND
CsrSize CsrBtHfgC2cBatteryIndSizeof(void *msg)
{
    CsrBtHfgC2cBatteryInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_BATTERY_IND */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_BATTERY_IND
CsrUint8 *CsrBtHfgC2cBatteryIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgC2cBatteryInd *primitive;

    primitive = (CsrBtHfgC2cBatteryInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->batteryLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_BATTERY_IND */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_BATTERY_IND
void *CsrBtHfgC2cBatteryIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgC2cBatteryInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgC2cBatteryInd *) CsrPmemZalloc(sizeof(CsrBtHfgC2cBatteryInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->batteryLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_BATTERY_IND */

#ifndef EXCLUDE_CSR_BT_HFG_OPERATOR_RES
CsrSize CsrBtHfgOperatorResSizeof(void *msg)
{
    CsrBtHfgOperatorRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->operatorName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_OPERATOR_RES */

#ifndef EXCLUDE_CSR_BT_HFG_OPERATOR_RES
CsrUint8 *CsrBtHfgOperatorResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgOperatorRes *primitive;

    primitive = (CsrBtHfgOperatorRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);
    CsrMemCpySer(buffer, length, &primitive->operatorName, sizeof(primitive->operatorName));
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeCode);
    CsrCharStringSer(buffer, length, primitive->operatorName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_OPERATOR_RES */

#ifndef EXCLUDE_CSR_BT_HFG_OPERATOR_RES
void *CsrBtHfgOperatorResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgOperatorRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgOperatorRes *) CsrPmemZalloc(sizeof(CsrBtHfgOperatorRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->operatorName), buffer, &offset, ((CsrSize) sizeof(primitive->operatorName)));
    CsrUint16Des((CsrUint16 *) &primitive->cmeeCode, buffer, &offset);
    CsrCharStringDes(&primitive->operatorName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_OPERATOR_RES */

#ifndef EXCLUDE_CSR_BT_HFG_OPERATOR_RES
void CsrBtHfgOperatorResSerFree(void *msg)
{
    CsrBtHfgOperatorRes *primitive;

    primitive = (CsrBtHfgOperatorRes *) msg;

    if(primitive->operatorName != NULL)
    {
        CsrPmemFree(primitive->operatorName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_OPERATOR_RES */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM
CsrSize CsrBtHfgConfigAtcmdHandlingCfmSizeof(void *msg)
{
    CsrBtHfgConfigAtcmdHandlingCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bitwiseIndicatorsLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM
CsrUint8 *CsrBtHfgConfigAtcmdHandlingCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgConfigAtcmdHandlingCfm *primitive;

    primitive = (CsrBtHfgConfigAtcmdHandlingCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);
    CsrMemCpySer(buffer, length, &primitive->bitwiseIndicators, sizeof(primitive->bitwiseIndicators));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bitwiseIndicatorsLength);
    if(primitive->bitwiseIndicators)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->bitwiseIndicators), ((CsrSize) (primitive->bitwiseIndicatorsLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM
void *CsrBtHfgConfigAtcmdHandlingCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgConfigAtcmdHandlingCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgConfigAtcmdHandlingCfm *) CsrPmemZalloc(sizeof(CsrBtHfgConfigAtcmdHandlingCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->bitwiseIndicators), buffer, &offset, ((CsrSize) sizeof(primitive->bitwiseIndicators)));
    CsrUint8Des((CsrUint8 *) &primitive->bitwiseIndicatorsLength, buffer, &offset);
    if (primitive->bitwiseIndicatorsLength)
    {
        primitive->bitwiseIndicators = CsrPmemZalloc(((CsrUint32) (primitive->bitwiseIndicatorsLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->bitwiseIndicators), buffer, &offset, ((CsrSize) (primitive->bitwiseIndicatorsLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->bitwiseIndicators = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM
void CsrBtHfgConfigAtcmdHandlingCfmSerFree(void *msg)
{
    CsrBtHfgConfigAtcmdHandlingCfm *primitive;

    primitive = (CsrBtHfgConfigAtcmdHandlingCfm *) msg;

    if(primitive->bitwiseIndicators != NULL)
    {
        CsrPmemFree(primitive->bitwiseIndicators);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_REQ
CsrSize CsrBtHfgDeregisterTimeReqSizeof(void *msg)
{
    CsrBtHfgDeregisterTimeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_REQ
CsrUint8 *CsrBtHfgDeregisterTimeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgDeregisterTimeReq *primitive;

    primitive = (CsrBtHfgDeregisterTimeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->waitSeconds);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_REQ
void *CsrBtHfgDeregisterTimeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgDeregisterTimeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgDeregisterTimeReq *) CsrPmemZalloc(sizeof(CsrBtHfgDeregisterTimeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->waitSeconds, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_ARRIVE_REQ
CsrSize CsrBtHfgC2cSmsArriveReqSizeof(void *msg)
{
    CsrBtHfgC2cSmsArriveReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->number) + CsrCharStringSerLen((CsrCharString *) primitive->name));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_ARRIVE_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_ARRIVE_REQ
CsrUint8 *CsrBtHfgC2cSmsArriveReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgC2cSmsArriveReq *primitive;

    primitive = (CsrBtHfgC2cSmsArriveReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->smsIndex);
    CsrMemCpySer(buffer, length, &primitive->number, sizeof(primitive->number));
    CsrMemCpySer(buffer, length, &primitive->name, sizeof(primitive->name));
    CsrCharStringSer(buffer, length, primitive->number);
    CsrCharStringSer(buffer, length, primitive->name);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_ARRIVE_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_ARRIVE_REQ
void *CsrBtHfgC2cSmsArriveReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgC2cSmsArriveReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgC2cSmsArriveReq *) CsrPmemZalloc(sizeof(CsrBtHfgC2cSmsArriveReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->smsIndex, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->number), buffer, &offset, ((CsrSize) sizeof(primitive->number)));
    CsrMemCpyDes(((void *) &primitive->name), buffer, &offset, ((CsrSize) sizeof(primitive->name)));
    CsrCharStringDes(&primitive->number, buffer, &offset);
    CsrCharStringDes(&primitive->name, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_ARRIVE_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_ARRIVE_REQ
void CsrBtHfgC2cSmsArriveReqSerFree(void *msg)
{
    CsrBtHfgC2cSmsArriveReq *primitive;

    primitive = (CsrBtHfgC2cSmsArriveReq *) msg;

    if(primitive->number != NULL)
    {
        CsrPmemFree(primitive->number);
    }
    if(primitive->name != NULL)
    {
        CsrPmemFree(primitive->name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_ARRIVE_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_DIAL_IND
CsrSize CsrBtHfgDialIndSizeof(void *msg)
{
    CsrBtHfgDialInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->number));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_DIAL_IND */

#ifndef EXCLUDE_CSR_BT_HFG_DIAL_IND
CsrUint8 *CsrBtHfgDialIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgDialInd *primitive;

    primitive = (CsrBtHfgDialInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->command);
    CsrMemCpySer(buffer, length, &primitive->number, sizeof(primitive->number));
    CsrCharStringSer(buffer, length, primitive->number);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_DIAL_IND */

#ifndef EXCLUDE_CSR_BT_HFG_DIAL_IND
void *CsrBtHfgDialIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgDialInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgDialInd *) CsrPmemZalloc(sizeof(CsrBtHfgDialInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->command, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->number), buffer, &offset, ((CsrSize) sizeof(primitive->number)));
    CsrCharStringDes(&primitive->number, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_DIAL_IND */

#ifndef EXCLUDE_CSR_BT_HFG_DIAL_IND
void CsrBtHfgDialIndSerFree(void *msg)
{
    CsrBtHfgDialInd *primitive;

    primitive = (CsrBtHfgDialInd *) msg;

    if(primitive->number != NULL)
    {
        CsrPmemFree(primitive->number);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_DIAL_IND */

#ifndef EXCLUDE_CSR_BT_HFG_INBAND_RINGING_REQ
CsrSize CsrBtHfgInbandRingingReqSizeof(void *msg)
{
    CsrBtHfgInbandRingingReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_INBAND_RINGING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_INBAND_RINGING_REQ
CsrUint8 *CsrBtHfgInbandRingingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgInbandRingingReq *primitive;

    primitive = (CsrBtHfgInbandRingingReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->inband);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_INBAND_RINGING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_INBAND_RINGING_REQ
void *CsrBtHfgInbandRingingReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgInbandRingingReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgInbandRingingReq *) CsrPmemZalloc(sizeof(CsrBtHfgInbandRingingReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->inband, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_INBAND_RINGING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_HANDLING_RES
CsrSize CsrBtHfgCallHandlingResSizeof(void *msg)
{
    CsrBtHfgCallHandlingRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_HANDLING_RES */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_HANDLING_RES
CsrUint8 *CsrBtHfgCallHandlingResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgCallHandlingRes *primitive;

    primitive = (CsrBtHfgCallHandlingRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeCode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->btrh);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_HANDLING_RES */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_HANDLING_RES
void *CsrBtHfgCallHandlingResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgCallHandlingRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgCallHandlingRes *) CsrPmemZalloc(sizeof(CsrBtHfgCallHandlingRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeCode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->btrh, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_HANDLING_RES */

#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_IN_REQ
CsrSize CsrBtHfgSecurityInReqSizeof(void *msg)
{
    CsrBtHfgSecurityInReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_IN_REQ
CsrUint8 *CsrBtHfgSecurityInReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgSecurityInReq *primitive;

    primitive = (CsrBtHfgSecurityInReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_IN_REQ
void *CsrBtHfgSecurityInReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgSecurityInReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgSecurityInReq *) CsrPmemZalloc(sizeof(CsrBtHfgSecurityInReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_IN_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_IND
CsrSize CsrBtHfgAudioDisconnectIndSizeof(void *msg)
{
    CsrBtHfgAudioDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_IND
CsrUint8 *CsrBtHfgAudioDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgAudioDisconnectInd *primitive;

    primitive = (CsrBtHfgAudioDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_IND
void *CsrBtHfgAudioDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgAudioDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgAudioDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtHfgAudioDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_STATUS_AUDIO_IND
CsrSize CsrBtHfgStatusAudioIndSizeof(void *msg)
{
    CsrBtHfgStatusAudioInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->audioSettingLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_AUDIO_IND */

#ifndef EXCLUDE_CSR_BT_HFG_STATUS_AUDIO_IND
CsrUint8 *CsrBtHfgStatusAudioIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgStatusAudioInd *primitive;

    primitive = (CsrBtHfgStatusAudioInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_AUDIO_IND */

#ifndef EXCLUDE_CSR_BT_HFG_STATUS_AUDIO_IND
void *CsrBtHfgStatusAudioIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgStatusAudioInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgStatusAudioInd *) CsrPmemZalloc(sizeof(CsrBtHfgStatusAudioInd));

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
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_AUDIO_IND */

#ifndef EXCLUDE_CSR_BT_HFG_STATUS_AUDIO_IND
void CsrBtHfgStatusAudioIndSerFree(void *msg)
{
    CsrBtHfgStatusAudioInd *primitive;

    primitive = (CsrBtHfgStatusAudioInd *) msg;

    if(primitive->audioSetting != NULL)
    {
        CsrPmemFree(primitive->audioSetting);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_AUDIO_IND */

#ifndef EXCLUDE_CSR_BT_HFG_RING_REQ
CsrSize CsrBtHfgRingReqSizeof(void *msg)
{
    CsrBtHfgRingReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->number) + CsrCharStringSerLen((CsrCharString *) primitive->name));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_RING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_RING_REQ
CsrUint8 *CsrBtHfgRingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgRingReq *primitive;

    primitive = (CsrBtHfgRingReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->repetitionRate);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->numOfRings);
    CsrMemCpySer(buffer, length, &primitive->number, sizeof(primitive->number));
    CsrMemCpySer(buffer, length, &primitive->name, sizeof(primitive->name));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->numType);
    CsrCharStringSer(buffer, length, primitive->number);
    CsrCharStringSer(buffer, length, primitive->name);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_RING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_RING_REQ
void *CsrBtHfgRingReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgRingReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgRingReq *) CsrPmemZalloc(sizeof(CsrBtHfgRingReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->repetitionRate, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->numOfRings, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->number), buffer, &offset, ((CsrSize) sizeof(primitive->number)));
    CsrMemCpyDes(((void *) &primitive->name), buffer, &offset, ((CsrSize) sizeof(primitive->name)));
    CsrUint8Des((CsrUint8 *) &primitive->numType, buffer, &offset);
    CsrCharStringDes(&primitive->number, buffer, &offset);
    CsrCharStringDes(&primitive->name, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_RING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_RING_REQ
void CsrBtHfgRingReqSerFree(void *msg)
{
    CsrBtHfgRingReq *primitive;

    primitive = (CsrBtHfgRingReq *) msg;

    if(primitive->number != NULL)
    {
        CsrPmemFree(primitive->number);
    }
    if(primitive->name != NULL)
    {
        CsrPmemFree(primitive->name);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_RING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_IND
CsrSize CsrBtHfgSpeakerGainIndSizeof(void *msg)
{
    CsrBtHfgSpeakerGainInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_IND */

#ifndef EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_IND
CsrUint8 *CsrBtHfgSpeakerGainIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgSpeakerGainInd *primitive;

    primitive = (CsrBtHfgSpeakerGainInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->gain);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_IND */

#ifndef EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_IND
void *CsrBtHfgSpeakerGainIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgSpeakerGainInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgSpeakerGainInd *) CsrPmemZalloc(sizeof(CsrBtHfgSpeakerGainInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->gain, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_IND */

#ifndef EXCLUDE_CSR_BT_HFG_HOUSE_CLEANING
CsrSize CsrBtHfgHouseCleaningSizeof(void *msg)
{
    CsrBtHfgHouseCleaning *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_HFG_HOUSE_CLEANING
CsrUint8 *CsrBtHfgHouseCleaningSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgHouseCleaning *primitive;

    primitive = (CsrBtHfgHouseCleaning *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->serverChannel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_HFG_HOUSE_CLEANING
void *CsrBtHfgHouseCleaningDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgHouseCleaning *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgHouseCleaning *) CsrPmemZalloc(sizeof(CsrBtHfgHouseCleaning));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->serverChannel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_HOUSE_CLEANING */

#ifndef EXCLUDE_CSR_BT_HFG_STATUS_INDICATOR_SET_REQ
CsrSize CsrBtHfgStatusIndicatorSetReqSizeof(void *msg)
{
    CsrBtHfgStatusIndicatorSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_INDICATOR_SET_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_STATUS_INDICATOR_SET_REQ
CsrUint8 *CsrBtHfgStatusIndicatorSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgStatusIndicatorSetReq *primitive;

    primitive = (CsrBtHfgStatusIndicatorSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->indicator);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_INDICATOR_SET_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_STATUS_INDICATOR_SET_REQ
void *CsrBtHfgStatusIndicatorSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgStatusIndicatorSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgStatusIndicatorSetReq *) CsrPmemZalloc(sizeof(CsrBtHfgStatusIndicatorSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->indicator, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_STATUS_INDICATOR_SET_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_DIAL_RES
CsrSize CsrBtHfgDialResSizeof(void *msg)
{
    CsrBtHfgDialRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_DIAL_RES */

#ifndef EXCLUDE_CSR_BT_HFG_DIAL_RES
CsrUint8 *CsrBtHfgDialResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgDialRes *primitive;

    primitive = (CsrBtHfgDialRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_DIAL_RES */

#ifndef EXCLUDE_CSR_BT_HFG_DIAL_RES
void *CsrBtHfgDialResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgDialRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgDialRes *) CsrPmemZalloc(sizeof(CsrBtHfgDialRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_DIAL_RES */

#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_OUT_CFM
CsrSize CsrBtHfgSecurityOutCfmSizeof(void *msg)
{
    CsrBtHfgSecurityOutCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_OUT_CFM
CsrUint8 *CsrBtHfgSecurityOutCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgSecurityOutCfm *primitive;

    primitive = (CsrBtHfgSecurityOutCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_OUT_CFM
void *CsrBtHfgSecurityOutCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgSecurityOutCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgSecurityOutCfm *) CsrPmemZalloc(sizeof(CsrBtHfgSecurityOutCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_OUT_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_IND
CsrSize CsrBtHfgAudioAcceptConnectIndSizeof(void *msg)
{
    CsrBtHfgAudioAcceptConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_IND
CsrUint8 *CsrBtHfgAudioAcceptConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgAudioAcceptConnectInd *primitive;

    primitive = (CsrBtHfgAudioAcceptConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->linkType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_IND
void *CsrBtHfgAudioAcceptConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgAudioAcceptConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgAudioAcceptConnectInd *) CsrPmemZalloc(sizeof(CsrBtHfgAudioAcceptConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->linkType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_REJECT_IND
CsrSize CsrBtHfgRejectIndSizeof(void *msg)
{
    CsrBtHfgRejectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_REJECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_REJECT_IND
CsrUint8 *CsrBtHfgRejectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgRejectInd *primitive;

    primitive = (CsrBtHfgRejectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_REJECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_REJECT_IND
void *CsrBtHfgRejectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgRejectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgRejectInd *) CsrPmemZalloc(sizeof(CsrBtHfgRejectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_REJECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SF_IND
CsrSize CsrBtHfgC2cSfIndSizeof(void *msg)
{
    CsrBtHfgC2cSfInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->indicatorsLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SF_IND */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SF_IND
CsrUint8 *CsrBtHfgC2cSfIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgC2cSfInd *primitive;

    primitive = (CsrBtHfgC2cSfInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SF_IND */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SF_IND
void *CsrBtHfgC2cSfIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgC2cSfInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgC2cSfInd *) CsrPmemZalloc(sizeof(CsrBtHfgC2cSfInd));

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
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SF_IND */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SF_IND
void CsrBtHfgC2cSfIndSerFree(void *msg)
{
    CsrBtHfgC2cSfInd *primitive;

    primitive = (CsrBtHfgC2cSfInd *) msg;

    if(primitive->indicators != NULL)
    {
        CsrPmemFree(primitive->indicators);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SF_IND */

#ifndef EXCLUDE_CSR_BT_HFG_CANCEL_CONNECT_REQ
CsrSize CsrBtHfgCancelConnectReqSizeof(void *msg)
{
    CsrBtHfgCancelConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CANCEL_CONNECT_REQ
CsrUint8 *CsrBtHfgCancelConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgCancelConnectReq *primitive;

    primitive = (CsrBtHfgCancelConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CANCEL_CONNECT_REQ
void *CsrBtHfgCancelConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgCancelConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgCancelConnectReq *) CsrPmemZalloc(sizeof(CsrBtHfgCancelConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_CANCEL_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_TXT_REQ
CsrSize CsrBtHfgC2cSmsTxtReqSizeof(void *msg)
{
    CsrBtHfgC2cSmsTxtReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->smsText));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_TXT_REQ
CsrUint8 *CsrBtHfgC2cSmsTxtReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgC2cSmsTxtReq *primitive;

    primitive = (CsrBtHfgC2cSmsTxtReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrMemCpySer(buffer, length, &primitive->smsText, sizeof(primitive->smsText));
    CsrCharStringSer(buffer, length, primitive->smsText);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_TXT_REQ
void *CsrBtHfgC2cSmsTxtReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgC2cSmsTxtReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgC2cSmsTxtReq *) CsrPmemZalloc(sizeof(CsrBtHfgC2cSmsTxtReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->smsText), buffer, &offset, ((CsrSize) sizeof(primitive->smsText)));
    CsrCharStringDes(&primitive->smsText, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_TXT_REQ
void CsrBtHfgC2cSmsTxtReqSerFree(void *msg)
{
    CsrBtHfgC2cSmsTxtReq *primitive;

    primitive = (CsrBtHfgC2cSmsTxtReq *) msg;

    if(primitive->smsText != NULL)
    {
        CsrPmemFree(primitive->smsText);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_CFM
CsrSize CsrBtHfgAudioDisconnectCfmSizeof(void *msg)
{
    CsrBtHfgAudioDisconnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_CFM
CsrUint8 *CsrBtHfgAudioDisconnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgAudioDisconnectCfm *primitive;

    primitive = (CsrBtHfgAudioDisconnectCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->scoHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_CFM
void *CsrBtHfgAudioDisconnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgAudioDisconnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgAudioDisconnectCfm *) CsrPmemZalloc(sizeof(CsrBtHfgAudioDisconnectCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->scoHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_VOICE_RECOG_RES
CsrSize CsrBtHfgVoiceRecogResSizeof(void *msg)
{
    CsrBtHfgVoiceRecogRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_VOICE_RECOG_RES */

#ifndef EXCLUDE_CSR_BT_HFG_VOICE_RECOG_RES
CsrUint8 *CsrBtHfgVoiceRecogResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgVoiceRecogRes *primitive;

    primitive = (CsrBtHfgVoiceRecogRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeCode);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_VOICE_RECOG_RES */

#ifndef EXCLUDE_CSR_BT_HFG_VOICE_RECOG_RES
void *CsrBtHfgVoiceRecogResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgVoiceRecogRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgVoiceRecogRes *) CsrPmemZalloc(sizeof(CsrBtHfgVoiceRecogRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeCode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_VOICE_RECOG_RES */

#ifndef EXCLUDE_CSR_BT_HFG_DISCONNECT_REQ
CsrSize CsrBtHfgDisconnectReqSizeof(void *msg)
{
    CsrBtHfgDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_DISCONNECT_REQ
CsrUint8 *CsrBtHfgDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgDisconnectReq *primitive;

    primitive = (CsrBtHfgDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_DISCONNECT_REQ
void *CsrBtHfgDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtHfgDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_REQ
CsrSize CsrBtHfgConfigAtcmdHandlingReqSizeof(void *msg)
{
    CsrBtHfgConfigAtcmdHandlingReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->bitwiseIndicatorsLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_REQ
CsrUint8 *CsrBtHfgConfigAtcmdHandlingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgConfigAtcmdHandlingReq *primitive;

    primitive = (CsrBtHfgConfigAtcmdHandlingReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrMemCpySer(buffer, length, &primitive->bitwiseIndicators, sizeof(primitive->bitwiseIndicators));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bitwiseIndicatorsLength);
    if(primitive->bitwiseIndicators)
    {
        CsrMemCpySer(buffer, length, ((const void *) primitive->bitwiseIndicators), ((CsrSize) (primitive->bitwiseIndicatorsLength * sizeof(CsrUint8))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_REQ
void *CsrBtHfgConfigAtcmdHandlingReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgConfigAtcmdHandlingReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgConfigAtcmdHandlingReq *) CsrPmemZalloc(sizeof(CsrBtHfgConfigAtcmdHandlingReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->bitwiseIndicators), buffer, &offset, ((CsrSize) sizeof(primitive->bitwiseIndicators)));
    CsrUint8Des((CsrUint8 *) &primitive->bitwiseIndicatorsLength, buffer, &offset);
    if (primitive->bitwiseIndicatorsLength)
    {
        primitive->bitwiseIndicators = CsrPmemZalloc(((CsrUint32) (primitive->bitwiseIndicatorsLength * sizeof(CsrUint8))));
        CsrMemCpyDes(((void *) primitive->bitwiseIndicators), buffer, &offset, ((CsrSize) (primitive->bitwiseIndicatorsLength * sizeof(CsrUint8))));
    }
    else
    {
        primitive->bitwiseIndicators = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_REQ
void CsrBtHfgConfigAtcmdHandlingReqSerFree(void *msg)
{
    CsrBtHfgConfigAtcmdHandlingReq *primitive;

    primitive = (CsrBtHfgConfigAtcmdHandlingReq *) msg;

    if(primitive->bitwiseIndicators != NULL)
    {
        CsrPmemFree(primitive->bitwiseIndicators);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_ATCMD_HANDLING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_IND
CsrSize CsrBtHfgManualIndicatorIndSizeof(void *msg)
{
    CsrBtHfgManualIndicatorInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_IND */

#ifndef EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_IND
CsrUint8 *CsrBtHfgManualIndicatorIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgManualIndicatorInd *primitive;

    primitive = (CsrBtHfgManualIndicatorInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_IND */

#ifndef EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_IND
void *CsrBtHfgManualIndicatorIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgManualIndicatorInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgManualIndicatorInd *) CsrPmemZalloc(sizeof(CsrBtHfgManualIndicatorInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_IND */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_LIST_RES
CsrSize CsrBtHfgCallListResSizeof(void *msg)
{
    CsrBtHfgCallListRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->number));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_LIST_RES */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_LIST_RES
CsrUint8 *CsrBtHfgCallListResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgCallListRes *primitive;

    primitive = (CsrBtHfgCallListRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->final);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->idx);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->dir);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->stat);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->mpy);
    CsrMemCpySer(buffer, length, &primitive->number, sizeof(primitive->number));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->numType);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeCode);
    CsrCharStringSer(buffer, length, primitive->number);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_LIST_RES */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_LIST_RES
void *CsrBtHfgCallListResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgCallListRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgCallListRes *) CsrPmemZalloc(sizeof(CsrBtHfgCallListRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->final, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->idx, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->dir, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->stat, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->mpy, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->number), buffer, &offset, ((CsrSize) sizeof(primitive->number)));
    CsrUint8Des((CsrUint8 *) &primitive->numType, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeCode, buffer, &offset);
    CsrCharStringDes(&primitive->number, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_LIST_RES */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_LIST_RES
void CsrBtHfgCallListResSerFree(void *msg)
{
    CsrBtHfgCallListRes *primitive;

    primitive = (CsrBtHfgCallListRes *) msg;

    if(primitive->number != NULL)
    {
        CsrPmemFree(primitive->number);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_LIST_RES */

#ifndef EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_REQ
CsrSize CsrBtHfgSpeakerGainReqSizeof(void *msg)
{
    CsrBtHfgSpeakerGainReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_REQ
CsrUint8 *CsrBtHfgSpeakerGainReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgSpeakerGainReq *primitive;

    primitive = (CsrBtHfgSpeakerGainReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->gain);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_REQ
void *CsrBtHfgSpeakerGainReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgSpeakerGainReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgSpeakerGainReq *) CsrPmemZalloc(sizeof(CsrBtHfgSpeakerGainReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->gain, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_SPEAKER_GAIN_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_IND
CsrSize CsrBtHfgAudioConnectIndSizeof(void *msg)
{
    CsrBtHfgAudioConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_IND
CsrUint8 *CsrBtHfgAudioConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgAudioConnectInd *primitive;

    primitive = (CsrBtHfgAudioConnectInd *) msg;
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
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_IND
void *CsrBtHfgAudioConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgAudioConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgAudioConnectInd *) CsrPmemZalloc(sizeof(CsrBtHfgAudioConnectInd));

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
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_NOISE_ECHO_IND
CsrSize CsrBtHfgNoiseEchoIndSizeof(void *msg)
{
    CsrBtHfgNoiseEchoInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_NOISE_ECHO_IND */

#ifndef EXCLUDE_CSR_BT_HFG_NOISE_ECHO_IND
CsrUint8 *CsrBtHfgNoiseEchoIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgNoiseEchoInd *primitive;

    primitive = (CsrBtHfgNoiseEchoInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->nrec);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_NOISE_ECHO_IND */

#ifndef EXCLUDE_CSR_BT_HFG_NOISE_ECHO_IND
void *CsrBtHfgNoiseEchoIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgNoiseEchoInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgNoiseEchoInd *) CsrPmemZalloc(sizeof(CsrBtHfgNoiseEchoInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->nrec, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_NOISE_ECHO_IND */

#ifndef EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_REQ
CsrSize CsrBtHfgServiceConnectReqSizeof(void *msg)
{
    CsrBtHfgServiceConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_REQ
CsrUint8 *CsrBtHfgServiceConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgServiceConnectReq *primitive;

    primitive = (CsrBtHfgServiceConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionType);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_REQ
void *CsrBtHfgServiceConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgServiceConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgServiceConnectReq *) CsrPmemZalloc(sizeof(CsrBtHfgServiceConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionType, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_TXT_REQ
CsrSize CsrBtHfgC2cTxtReqSizeof(void *msg)
{
    CsrBtHfgC2cTxtReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->text));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_TXT_REQ
CsrUint8 *CsrBtHfgC2cTxtReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgC2cTxtReq *primitive;

    primitive = (CsrBtHfgC2cTxtReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrMemCpySer(buffer, length, &primitive->text, sizeof(primitive->text));
    CsrCharStringSer(buffer, length, primitive->text);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_TXT_REQ
void *CsrBtHfgC2cTxtReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgC2cTxtReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgC2cTxtReq *) CsrPmemZalloc(sizeof(CsrBtHfgC2cTxtReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->text), buffer, &offset, ((CsrSize) sizeof(primitive->text)));
    CsrCharStringDes(&primitive->text, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_TXT_REQ
void CsrBtHfgC2cTxtReqSerFree(void *msg)
{
    CsrBtHfgC2cTxtReq *primitive;

    primitive = (CsrBtHfgC2cTxtReq *) msg;

    if(primitive->text != NULL)
    {
        CsrPmemFree(primitive->text);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_TXT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_HANDLING_REQ
CsrSize CsrBtHfgCallHandlingReqSizeof(void *msg)
{
    CsrBtHfgCallHandlingReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_HANDLING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_HANDLING_REQ
CsrUint8 *CsrBtHfgCallHandlingReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgCallHandlingReq *primitive;

    primitive = (CsrBtHfgCallHandlingReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->btrh);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_HANDLING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_HANDLING_REQ
void *CsrBtHfgCallHandlingReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgCallHandlingReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgCallHandlingReq *) CsrPmemZalloc(sizeof(CsrBtHfgCallHandlingReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->btrh, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_HANDLING_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_MIC_GAIN_REQ
CsrSize CsrBtHfgMicGainReqSizeof(void *msg)
{
    CsrBtHfgMicGainReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_MIC_GAIN_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_MIC_GAIN_REQ
CsrUint8 *CsrBtHfgMicGainReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgMicGainReq *primitive;

    primitive = (CsrBtHfgMicGainReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->gain);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_MIC_GAIN_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_MIC_GAIN_REQ
void *CsrBtHfgMicGainReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgMicGainReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgMicGainReq *) CsrPmemZalloc(sizeof(CsrBtHfgMicGainReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->gain, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_MIC_GAIN_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_DISCONNECT_IND
CsrSize CsrBtHfgDisconnectIndSizeof(void *msg)
{
    CsrBtHfgDisconnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_DISCONNECT_IND
CsrUint8 *CsrBtHfgDisconnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgDisconnectInd *primitive;

    primitive = (CsrBtHfgDisconnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->reasonSupplier);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->localTerminated);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_DISCONNECT_IND
void *CsrBtHfgDisconnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgDisconnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgDisconnectInd *) CsrPmemZalloc(sizeof(CsrBtHfgDisconnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->reasonSupplier, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->localTerminated, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_DISCONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_REQ
CsrSize CsrBtHfgAudioDisconnectReqSizeof(void *msg)
{
    CsrBtHfgAudioDisconnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_REQ
CsrUint8 *CsrBtHfgAudioDisconnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgAudioDisconnectReq *primitive;

    primitive = (CsrBtHfgAudioDisconnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_REQ
void *CsrBtHfgAudioDisconnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgAudioDisconnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgAudioDisconnectReq *) CsrPmemZalloc(sizeof(CsrBtHfgAudioDisconnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_DISCONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_MIC_GAIN_IND
CsrSize CsrBtHfgMicGainIndSizeof(void *msg)
{
    CsrBtHfgMicGainInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_MIC_GAIN_IND */

#ifndef EXCLUDE_CSR_BT_HFG_MIC_GAIN_IND
CsrUint8 *CsrBtHfgMicGainIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgMicGainInd *primitive;

    primitive = (CsrBtHfgMicGainInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->gain);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_MIC_GAIN_IND */

#ifndef EXCLUDE_CSR_BT_HFG_MIC_GAIN_IND
void *CsrBtHfgMicGainIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgMicGainInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgMicGainInd *) CsrPmemZalloc(sizeof(CsrBtHfgMicGainInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->gain, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_MIC_GAIN_IND */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_REQ
CsrSize CsrBtHfgAudioConnectReqSizeof(void *msg)
{
    CsrBtHfgAudioConnectReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_REQ
CsrUint8 *CsrBtHfgAudioConnectReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgAudioConnectReq *primitive;

    primitive = (CsrBtHfgAudioConnectReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmSlot);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->pcmRealloc);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_REQ
void *CsrBtHfgAudioConnectReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgAudioConnectReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgAudioConnectReq *) CsrPmemZalloc(sizeof(CsrBtHfgAudioConnectReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmSlot, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmRealloc, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_DEACTIVATE_CFM
CsrSize CsrBtHfgDeactivateCfmSizeof(void *msg)
{
    CsrBtHfgDeactivateCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_DEACTIVATE_CFM
CsrUint8 *CsrBtHfgDeactivateCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgDeactivateCfm *primitive;

    primitive = (CsrBtHfgDeactivateCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_DEACTIVATE_CFM
void *CsrBtHfgDeactivateCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgDeactivateCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgDeactivateCfm *) CsrPmemZalloc(sizeof(CsrBtHfgDeactivateCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_DEACTIVATE_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_RING_CFM
CsrSize CsrBtHfgRingCfmSizeof(void *msg)
{
    CsrBtHfgRingCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_RING_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_RING_CFM
CsrUint8 *CsrBtHfgRingCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgRingCfm *primitive;

    primitive = (CsrBtHfgRingCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_RING_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_RING_CFM
void *CsrBtHfgRingCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgRingCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgRingCfm *) CsrPmemZalloc(sizeof(CsrBtHfgRingCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_RING_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_OPERATOR_IND
CsrSize CsrBtHfgOperatorIndSizeof(void *msg)
{
    CsrBtHfgOperatorInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_OPERATOR_IND */

#ifndef EXCLUDE_CSR_BT_HFG_OPERATOR_IND
CsrUint8 *CsrBtHfgOperatorIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgOperatorInd *primitive;

    primitive = (CsrBtHfgOperatorInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_OPERATOR_IND */

#ifndef EXCLUDE_CSR_BT_HFG_OPERATOR_IND
void *CsrBtHfgOperatorIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgOperatorInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgOperatorInd *) CsrPmemZalloc(sizeof(CsrBtHfgOperatorInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_OPERATOR_IND */

#ifndef EXCLUDE_CSR_BT_HFG_BT_INPUT_RES
CsrSize CsrBtHfgBtInputResSizeof(void *msg)
{
    CsrBtHfgBtInputRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->response));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_BT_INPUT_RES */

#ifndef EXCLUDE_CSR_BT_HFG_BT_INPUT_RES
CsrUint8 *CsrBtHfgBtInputResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgBtInputRes *primitive;

    primitive = (CsrBtHfgBtInputRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeCode);
    CsrMemCpySer(buffer, length, &primitive->response, sizeof(primitive->response));
    CsrCharStringSer(buffer, length, primitive->response);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_BT_INPUT_RES */

#ifndef EXCLUDE_CSR_BT_HFG_BT_INPUT_RES
void *CsrBtHfgBtInputResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgBtInputRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgBtInputRes *) CsrPmemZalloc(sizeof(CsrBtHfgBtInputRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeCode, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->response), buffer, &offset, ((CsrSize) sizeof(primitive->response)));
    CsrCharStringDes(&primitive->response, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_BT_INPUT_RES */

#ifndef EXCLUDE_CSR_BT_HFG_BT_INPUT_RES
void CsrBtHfgBtInputResSerFree(void *msg)
{
    CsrBtHfgBtInputRes *primitive;

    primitive = (CsrBtHfgBtInputRes *) msg;

    if(primitive->response != NULL)
    {
        CsrPmemFree(primitive->response);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_BT_INPUT_RES */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_HANDLING_IND
CsrSize CsrBtHfgCallHandlingIndSizeof(void *msg)
{
    CsrBtHfgCallHandlingInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_HANDLING_IND */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_HANDLING_IND
CsrUint8 *CsrBtHfgCallHandlingIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgCallHandlingInd *primitive;

    primitive = (CsrBtHfgCallHandlingInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->value);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->index);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_HANDLING_IND */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_HANDLING_IND
void *CsrBtHfgCallHandlingIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgCallHandlingInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgCallHandlingInd *) CsrPmemZalloc(sizeof(CsrBtHfgCallHandlingInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->value, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->index, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_HANDLING_IND */

#ifndef EXCLUDE_CSR_BT_HFG_ANSWER_IND
CsrSize CsrBtHfgAnswerIndSizeof(void *msg)
{
    CsrBtHfgAnswerInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_ANSWER_IND */

#ifndef EXCLUDE_CSR_BT_HFG_ANSWER_IND
CsrUint8 *CsrBtHfgAnswerIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgAnswerInd *primitive;

    primitive = (CsrBtHfgAnswerInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_ANSWER_IND */

#ifndef EXCLUDE_CSR_BT_HFG_ANSWER_IND
void *CsrBtHfgAnswerIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgAnswerInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgAnswerInd *) CsrPmemZalloc(sizeof(CsrBtHfgAnswerInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_ANSWER_IND */

#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_IN_CFM
CsrSize CsrBtHfgSecurityInCfmSizeof(void *msg)
{
    CsrBtHfgSecurityInCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_IN_CFM
CsrUint8 *CsrBtHfgSecurityInCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgSecurityInCfm *primitive;

    primitive = (CsrBtHfgSecurityInCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_IN_CFM
void *CsrBtHfgSecurityInCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgSecurityInCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgSecurityInCfm *) CsrPmemZalloc(sizeof(CsrBtHfgSecurityInCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_IN_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_DEACTIVATE_REQ
CsrSize CsrBtHfgDeactivateReqSizeof(void *msg)
{
    CsrBtHfgDeactivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_DEACTIVATE_REQ
CsrUint8 *CsrBtHfgDeactivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgDeactivateReq *primitive;

    primitive = (CsrBtHfgDeactivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_DEACTIVATE_REQ
void *CsrBtHfgDeactivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgDeactivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgDeactivateReq *) CsrPmemZalloc(sizeof(CsrBtHfgDeactivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_DEACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_GET_IND
CsrSize CsrBtHfgC2cSmsGetIndSizeof(void *msg)
{
    CsrBtHfgC2cSmsGetInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_GET_IND */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_GET_IND
CsrUint8 *CsrBtHfgC2cSmsGetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgC2cSmsGetInd *primitive;

    primitive = (CsrBtHfgC2cSmsGetInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->smsIndex);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_GET_IND */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_SMS_GET_IND
void *CsrBtHfgC2cSmsGetIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgC2cSmsGetInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgC2cSmsGetInd *) CsrPmemZalloc(sizeof(CsrBtHfgC2cSmsGetInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->smsIndex, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_SMS_GET_IND */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_LIST_IND
CsrSize CsrBtHfgCallListIndSizeof(void *msg)
{
    CsrBtHfgCallListInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_LIST_IND */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_LIST_IND
CsrUint8 *CsrBtHfgCallListIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgCallListInd *primitive;

    primitive = (CsrBtHfgCallListInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_LIST_IND */

#ifndef EXCLUDE_CSR_BT_HFG_CALL_LIST_IND
void *CsrBtHfgCallListIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgCallListInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgCallListInd *) CsrPmemZalloc(sizeof(CsrBtHfgCallListInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_CALL_LIST_IND */

#ifndef EXCLUDE_CSR_BT_HFG_GENERATE_DTMF_IND
CsrSize CsrBtHfgGenerateDtmfIndSizeof(void *msg)
{
    CsrBtHfgGenerateDtmfInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_GENERATE_DTMF_IND */

#ifndef EXCLUDE_CSR_BT_HFG_GENERATE_DTMF_IND
CsrUint8 *CsrBtHfgGenerateDtmfIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgGenerateDtmfInd *primitive;

    primitive = (CsrBtHfgGenerateDtmfInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->dtmf);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_GENERATE_DTMF_IND */

#ifndef EXCLUDE_CSR_BT_HFG_GENERATE_DTMF_IND
void *CsrBtHfgGenerateDtmfIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgGenerateDtmfInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgGenerateDtmfInd *) CsrPmemZalloc(sizeof(CsrBtHfgGenerateDtmfInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->dtmf, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_GENERATE_DTMF_IND */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_CFM
CsrSize CsrBtHfgAudioConnectCfmSizeof(void *msg)
{
    CsrBtHfgAudioConnectCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_CFM
CsrUint8 *CsrBtHfgAudioConnectCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgAudioConnectCfm *primitive;

    primitive = (CsrBtHfgAudioConnectCfm *) msg;
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
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_CFM
void *CsrBtHfgAudioConnectCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgAudioConnectCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgAudioConnectCfm *) CsrPmemZalloc(sizeof(CsrBtHfgAudioConnectCfm));

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
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_CONNECT_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_AT_CMD_REQ
CsrSize CsrBtHfgAtCmdReqSizeof(void *msg)
{
    CsrBtHfgAtCmdReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->command));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_AT_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_AT_CMD_REQ
CsrUint8 *CsrBtHfgAtCmdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgAtCmdReq *primitive;

    primitive = (CsrBtHfgAtCmdReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrMemCpySer(buffer, length, &primitive->command, sizeof(primitive->command));
    CsrCharStringSer(buffer, length, primitive->command);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_AT_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_AT_CMD_REQ
void *CsrBtHfgAtCmdReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgAtCmdReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgAtCmdReq *) CsrPmemZalloc(sizeof(CsrBtHfgAtCmdReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->command), buffer, &offset, ((CsrSize) sizeof(primitive->command)));
    CsrCharStringDes(&primitive->command, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_AT_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_AT_CMD_REQ
void CsrBtHfgAtCmdReqSerFree(void *msg)
{
    CsrBtHfgAtCmdReq *primitive;

    primitive = (CsrBtHfgAtCmdReq *) msg;

    if(primitive->command != NULL)
    {
        CsrPmemFree(primitive->command);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_AT_CMD_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_OUT_REQ
CsrSize CsrBtHfgSecurityOutReqSizeof(void *msg)
{
    CsrBtHfgSecurityOutReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_OUT_REQ
CsrUint8 *CsrBtHfgSecurityOutReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgSecurityOutReq *primitive;

    primitive = (CsrBtHfgSecurityOutReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->appHandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->secLevel);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_SECURITY_OUT_REQ
void *CsrBtHfgSecurityOutReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgSecurityOutReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgSecurityOutReq *) CsrPmemZalloc(sizeof(CsrBtHfgSecurityOutReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->appHandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->secLevel, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_SECURITY_OUT_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_VOICE_RECOG_IND
CsrSize CsrBtHfgVoiceRecogIndSizeof(void *msg)
{
    CsrBtHfgVoiceRecogInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_VOICE_RECOG_IND */

#ifndef EXCLUDE_CSR_BT_HFG_VOICE_RECOG_IND
CsrUint8 *CsrBtHfgVoiceRecogIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgVoiceRecogInd *primitive;

    primitive = (CsrBtHfgVoiceRecogInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bvra);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_VOICE_RECOG_IND */

#ifndef EXCLUDE_CSR_BT_HFG_VOICE_RECOG_IND
void *CsrBtHfgVoiceRecogIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgVoiceRecogInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgVoiceRecogInd *) CsrPmemZalloc(sizeof(CsrBtHfgVoiceRecogInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bvra, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_VOICE_RECOG_IND */

#ifndef EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_IND
CsrSize CsrBtHfgServiceConnectIndSizeof(void *msg)
{
    CsrBtHfgServiceConnectInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->serviceName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_IND
CsrUint8 *CsrBtHfgServiceConnectIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgServiceConnectInd *primitive;

    primitive = (CsrBtHfgServiceConnectInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->connectionType);
    CsrMemCpySer(buffer, length, &primitive->serviceName, sizeof(primitive->serviceName));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->deviceAddr.lap);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->deviceAddr.uap);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->deviceAddr.nap);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->supportedFeatures);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->remoteVersion);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultCode);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->resultSupplier);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->btConnId);
    CsrCharStringSer(buffer, length, primitive->serviceName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_IND
void *CsrBtHfgServiceConnectIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgServiceConnectInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgServiceConnectInd *) CsrPmemZalloc(sizeof(CsrBtHfgServiceConnectInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->connectionType, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->serviceName)));
    CsrUint32Des((CsrUint32 *) &primitive->deviceAddr.lap, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->deviceAddr.uap, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->deviceAddr.nap, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->supportedFeatures, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->remoteVersion, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultCode, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->resultSupplier, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->btConnId, buffer, &offset);
    CsrCharStringDes(&primitive->serviceName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_IND
void CsrBtHfgServiceConnectIndSerFree(void *msg)
{
    CsrBtHfgServiceConnectInd *primitive;

    primitive = (CsrBtHfgServiceConnectInd *) msg;

    if(primitive->serviceName != NULL)
    {
        CsrPmemFree(primitive->serviceName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_SERVICE_CONNECT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ
CsrSize CsrBtHfgGetC2cAdpcmLocalSupportedReqSizeof(void *msg)
{
    CsrBtHfgGetC2cAdpcmLocalSupportedReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ
CsrUint8 *CsrBtHfgGetC2cAdpcmLocalSupportedReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgGetC2cAdpcmLocalSupportedReq *primitive;

    primitive = (CsrBtHfgGetC2cAdpcmLocalSupportedReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ
void *CsrBtHfgGetC2cAdpcmLocalSupportedReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgGetC2cAdpcmLocalSupportedReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgGetC2cAdpcmLocalSupportedReq *) CsrPmemZalloc(sizeof(CsrBtHfgGetC2cAdpcmLocalSupportedReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_AT_CMD_IND
CsrSize CsrBtHfgAtCmdIndSizeof(void *msg)
{
    CsrBtHfgAtCmdInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->command));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_AT_CMD_IND */

#ifndef EXCLUDE_CSR_BT_HFG_AT_CMD_IND
CsrUint8 *CsrBtHfgAtCmdIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgAtCmdInd *primitive;

    primitive = (CsrBtHfgAtCmdInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->cmeeEnabled);
    CsrMemCpySer(buffer, length, &primitive->command, sizeof(primitive->command));
    CsrCharStringSer(buffer, length, primitive->command);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_AT_CMD_IND */

#ifndef EXCLUDE_CSR_BT_HFG_AT_CMD_IND
void *CsrBtHfgAtCmdIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgAtCmdInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgAtCmdInd *) CsrPmemZalloc(sizeof(CsrBtHfgAtCmdInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->cmeeEnabled, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->command), buffer, &offset, ((CsrSize) sizeof(primitive->command)));
    CsrCharStringDes(&primitive->command, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_AT_CMD_IND */

#ifndef EXCLUDE_CSR_BT_HFG_AT_CMD_IND
void CsrBtHfgAtCmdIndSerFree(void *msg)
{
    CsrBtHfgAtCmdInd *primitive;

    primitive = (CsrBtHfgAtCmdInd *) msg;

    if(primitive->command != NULL)
    {
        CsrPmemFree(primitive->command);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_AT_CMD_IND */

#ifndef EXCLUDE_CSR_BT_HFG_VOICE_RECOG_REQ
CsrSize CsrBtHfgVoiceRecogReqSizeof(void *msg)
{
    CsrBtHfgVoiceRecogReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_VOICE_RECOG_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_VOICE_RECOG_REQ
CsrUint8 *CsrBtHfgVoiceRecogReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgVoiceRecogReq *primitive;

    primitive = (CsrBtHfgVoiceRecogReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->bvra);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_VOICE_RECOG_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_VOICE_RECOG_REQ
void *CsrBtHfgVoiceRecogReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgVoiceRecogReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgVoiceRecogReq *) CsrPmemZalloc(sizeof(CsrBtHfgVoiceRecogReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->bvra, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_VOICE_RECOG_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_BT_INPUT_IND
CsrSize CsrBtHfgBtInputIndSizeof(void *msg)
{
    CsrBtHfgBtInputInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_BT_INPUT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_BT_INPUT_IND
CsrUint8 *CsrBtHfgBtInputIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgBtInputInd *primitive;

    primitive = (CsrBtHfgBtInputInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->request);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_BT_INPUT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_BT_INPUT_IND
void *CsrBtHfgBtInputIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgBtInputInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgBtInputInd *) CsrPmemZalloc(sizeof(CsrBtHfgBtInputInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->request, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_BT_INPUT_IND */

#ifndef EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_CFM
CsrSize CsrBtHfgDeregisterTimeCfmSizeof(void *msg)
{
    CsrBtHfgDeregisterTimeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_CFM
CsrUint8 *CsrBtHfgDeregisterTimeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgDeregisterTimeCfm *primitive;

    primitive = (CsrBtHfgDeregisterTimeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_CFM
void *CsrBtHfgDeregisterTimeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgDeregisterTimeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgDeregisterTimeCfm *) CsrPmemZalloc(sizeof(CsrBtHfgDeregisterTimeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_DEREGISTER_TIME_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND
CsrSize CsrBtHfgGetC2cAdpcmLocalSupportedIndSizeof(void *msg)
{
    CsrBtHfgGetC2cAdpcmLocalSupportedInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND */

#ifndef EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND
CsrUint8 *CsrBtHfgGetC2cAdpcmLocalSupportedIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgGetC2cAdpcmLocalSupportedInd *primitive;

    primitive = (CsrBtHfgGetC2cAdpcmLocalSupportedInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND */

#ifndef EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND
void *CsrBtHfgGetC2cAdpcmLocalSupportedIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgGetC2cAdpcmLocalSupportedInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgGetC2cAdpcmLocalSupportedInd *) CsrPmemZalloc(sizeof(CsrBtHfgGetC2cAdpcmLocalSupportedInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_GET_C2C_ADPCM_LOCAL_SUPPORTED_IND */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_SNIFF_REQ
CsrSize CsrBtHfgConfigSniffReqSizeof(void *msg)
{
    CsrBtHfgConfigSniffReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_SNIFF_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_SNIFF_REQ
CsrUint8 *CsrBtHfgConfigSniffReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgConfigSniffReq *primitive;

    primitive = (CsrBtHfgConfigSniffReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->mask);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_SNIFF_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_SNIFF_REQ
void *CsrBtHfgConfigSniffReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgConfigSniffReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgConfigSniffReq *) CsrPmemZalloc(sizeof(CsrBtHfgConfigSniffReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->mask, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_SNIFF_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_POWER_IND
CsrSize CsrBtHfgC2cPowerIndSizeof(void *msg)
{
    CsrBtHfgC2cPowerInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_POWER_IND */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_POWER_IND
CsrUint8 *CsrBtHfgC2cPowerIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgC2cPowerInd *primitive;

    primitive = (CsrBtHfgC2cPowerInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->powerStatus);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_POWER_IND */

#ifndef EXCLUDE_CSR_BT_HFG_C2C_POWER_IND
void *CsrBtHfgC2cPowerIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgC2cPowerInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgC2cPowerInd *) CsrPmemZalloc(sizeof(CsrBtHfgC2cPowerInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->powerStatus, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_C2C_POWER_IND */

#ifndef EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_IND
CsrSize CsrBtHfgSubscriberNumberIndSizeof(void *msg)
{
    CsrBtHfgSubscriberNumberInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_IND */

#ifndef EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_IND
CsrUint8 *CsrBtHfgSubscriberNumberIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgSubscriberNumberInd *primitive;

    primitive = (CsrBtHfgSubscriberNumberInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_IND */

#ifndef EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_IND
void *CsrBtHfgSubscriberNumberIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgSubscriberNumberInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgSubscriberNumberInd *) CsrPmemZalloc(sizeof(CsrBtHfgSubscriberNumberInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_IND */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_AUDIO_REQ
CsrSize CsrBtHfgConfigAudioReqSizeof(void *msg)
{
    CsrBtHfgConfigAudioReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->audioSettingLen * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_AUDIO_REQ
CsrUint8 *CsrBtHfgConfigAudioReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgConfigAudioReq *primitive;

    primitive = (CsrBtHfgConfigAudioReq *) msg;
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
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_AUDIO_REQ
void *CsrBtHfgConfigAudioReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgConfigAudioReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgConfigAudioReq *) CsrPmemZalloc(sizeof(CsrBtHfgConfigAudioReq));

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
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_AUDIO_REQ
void CsrBtHfgConfigAudioReqSerFree(void *msg)
{
    CsrBtHfgConfigAudioReq *primitive;

    primitive = (CsrBtHfgConfigAudioReq *) msg;

    if(primitive->audioSetting != NULL)
    {
        CsrPmemFree(primitive->audioSetting);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_AUDIO_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_RES
CsrSize CsrBtHfgManualIndicatorResSizeof(void *msg)
{
    CsrBtHfgManualIndicatorRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->indicatorsLength * sizeof(CsrUint8)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_RES */

#ifndef EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_RES
CsrUint8 *CsrBtHfgManualIndicatorResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgManualIndicatorRes *primitive;

    primitive = (CsrBtHfgManualIndicatorRes *) msg;
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
#endif /* EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_RES */

#ifndef EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_RES
void *CsrBtHfgManualIndicatorResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgManualIndicatorRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgManualIndicatorRes *) CsrPmemZalloc(sizeof(CsrBtHfgManualIndicatorRes));

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
#endif /* EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_RES */

#ifndef EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_RES
void CsrBtHfgManualIndicatorResSerFree(void *msg)
{
    CsrBtHfgManualIndicatorRes *primitive;

    primitive = (CsrBtHfgManualIndicatorRes *) msg;

    if(primitive->indicators != NULL)
    {
        CsrPmemFree(primitive->indicators);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_MANUAL_INDICATOR_RES */

#ifndef EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_RES
CsrSize CsrBtHfgSubscriberNumberResSizeof(void *msg)
{
    CsrBtHfgSubscriberNumberRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->number));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_RES */

#ifndef EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_RES
CsrUint8 *CsrBtHfgSubscriberNumberResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgSubscriberNumberRes *primitive;

    primitive = (CsrBtHfgSubscriberNumberRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->connectionId);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->final);
    CsrMemCpySer(buffer, length, &primitive->number, sizeof(primitive->number));
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->numType);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->service);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->cmeeCode);
    CsrCharStringSer(buffer, length, primitive->number);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_RES */

#ifndef EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_RES
void *CsrBtHfgSubscriberNumberResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgSubscriberNumberRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgSubscriberNumberRes *) CsrPmemZalloc(sizeof(CsrBtHfgSubscriberNumberRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->final, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->number), buffer, &offset, ((CsrSize) sizeof(primitive->number)));
    CsrUint8Des((CsrUint8 *) &primitive->numType, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->service, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->cmeeCode, buffer, &offset);
    CsrCharStringDes(&primitive->number, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_RES */

#ifndef EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_RES
void CsrBtHfgSubscriberNumberResSerFree(void *msg)
{
    CsrBtHfgSubscriberNumberRes *primitive;

    primitive = (CsrBtHfgSubscriberNumberRes *) msg;

    if(primitive->number != NULL)
    {
        CsrPmemFree(primitive->number);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_SUBSCRIBER_NUMBER_RES */

#ifndef EXCLUDE_CSR_BT_HFG_ACTIVATE_REQ
CsrSize CsrBtHfgActivateReqSizeof(void *msg)
{
    CsrBtHfgActivateReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + CsrCharStringSerLen((CsrCharString *) primitive->serviceName));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_ACTIVATE_REQ
CsrUint8 *CsrBtHfgActivateReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgActivateReq *primitive;

    primitive = (CsrBtHfgActivateReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->atMode);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->numConnections);
    CsrMemCpySer(buffer, length, &primitive->serviceName, sizeof(primitive->serviceName));
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->supportedFeatures);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->callConfig);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->hfgConfig);
    CsrCharStringSer(buffer, length, primitive->serviceName);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_ACTIVATE_REQ
void *CsrBtHfgActivateReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgActivateReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgActivateReq *) CsrPmemZalloc(sizeof(CsrBtHfgActivateReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->atMode, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->numConnections, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->serviceName), buffer, &offset, ((CsrSize) sizeof(primitive->serviceName)));
    CsrUint32Des((CsrUint32 *) &primitive->supportedFeatures, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->callConfig, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->hfgConfig, buffer, &offset);
    CsrCharStringDes(&primitive->serviceName, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_ACTIVATE_REQ
void CsrBtHfgActivateReqSerFree(void *msg)
{
    CsrBtHfgActivateReq *primitive;

    primitive = (CsrBtHfgActivateReq *) msg;

    if(primitive->serviceName != NULL)
    {
        CsrPmemFree(primitive->serviceName);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_ACTIVATE_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_REQ
CsrSize CsrBtHfgConfigSingleAtcmdReqSizeof(void *msg)
{
    CsrBtHfgConfigSingleAtcmdReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_REQ
CsrUint8 *CsrBtHfgConfigSingleAtcmdReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgConfigSingleAtcmdReq *primitive;

    primitive = (CsrBtHfgConfigSingleAtcmdReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->idx);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->sendToApp);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_REQ
void *CsrBtHfgConfigSingleAtcmdReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgConfigSingleAtcmdReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgConfigSingleAtcmdReq *) CsrPmemZalloc(sizeof(CsrBtHfgConfigSingleAtcmdReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->idx, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->sendToApp, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_REQ */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_CFM
CsrSize CsrBtHfgConfigSingleAtcmdCfmSizeof(void *msg)
{
    CsrBtHfgConfigSingleAtcmdCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_CFM
CsrUint8 *CsrBtHfgConfigSingleAtcmdCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgConfigSingleAtcmdCfm *primitive;

    primitive = (CsrBtHfgConfigSingleAtcmdCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_CFM
void *CsrBtHfgConfigSingleAtcmdCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgConfigSingleAtcmdCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgConfigSingleAtcmdCfm *) CsrPmemZalloc(sizeof(CsrBtHfgConfigSingleAtcmdCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_CONFIG_SINGLE_ATCMD_CFM */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_RES
CsrSize CsrBtHfgAudioAcceptConnectResSizeof(void *msg)
{
    CsrBtHfgAudioAcceptConnectRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive) + (primitive->acceptParametersLength * sizeof(CsrBtHfgAudioIncomingAcceptParameters)));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_RES
CsrUint8 *CsrBtHfgAudioAcceptConnectResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrBtHfgAudioAcceptConnectRes *primitive;

    primitive = (CsrBtHfgAudioAcceptConnectRes *) msg;
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
        CsrMemCpySer(buffer, length, ((const void *) primitive->acceptParameters), ((CsrSize) (primitive->acceptParametersLength * sizeof(CsrBtHfgAudioIncomingAcceptParameters))));
    }

    return buffer;
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_RES
void *CsrBtHfgAudioAcceptConnectResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrBtHfgAudioAcceptConnectRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrBtHfgAudioAcceptConnectRes *) CsrPmemZalloc(sizeof(CsrBtHfgAudioAcceptConnectRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->connectionId, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->acceptResponse, buffer, &offset);
    CsrMemCpyDes(((void *) &primitive->acceptParameters), buffer, &offset, ((CsrSize) sizeof(primitive->acceptParameters)));
    CsrUint8Des((CsrUint8 *) &primitive->acceptParametersLength, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmSlot, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->pcmReassign, buffer, &offset);
    if (primitive->acceptParametersLength)
    {
        primitive->acceptParameters = CsrPmemZalloc(((CsrUint32) (primitive->acceptParametersLength * sizeof(CsrBtHfgAudioIncomingAcceptParameters))));
        CsrMemCpyDes(((void *) primitive->acceptParameters), buffer, &offset, ((CsrSize) (primitive->acceptParametersLength * sizeof(CsrBtHfgAudioIncomingAcceptParameters))));
    }
    else
    {
        primitive->acceptParameters = NULL;
    }

    return primitive;
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_RES */

#ifndef EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_RES
void CsrBtHfgAudioAcceptConnectResSerFree(void *msg)
{
    CsrBtHfgAudioAcceptConnectRes *primitive;

    primitive = (CsrBtHfgAudioAcceptConnectRes *) msg;

    if(primitive->acceptParameters != NULL)
    {
        CsrPmemFree(primitive->acceptParameters);
    }
    CsrPmemFree(primitive);
}
#endif /* EXCLUDE_CSR_BT_HFG_AUDIO_ACCEPT_CONNECT_RES */

#endif /* EXCLUDE_CSR_BT_HFG_MODULE */
