/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_msgconv.h"
#include "csr_autogen.h"
#include "csr_unicode.h"
#include "csr_tm_bluecore_private_prim.h"

#ifndef EXCLUDE_CSR_TM_BLUECORE_PRIVATE_MODULE

void CsrTm_bluecorePfree(void *ptr)
{
    CsrPmemFree(ptr);
}

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_ARGS_IND
CsrSize CsrTmBluecoreCmStatusPanicArgsIndSizeof(void *msg)
{
    CsrTmBluecoreCmStatusPanicArgsInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_ARGS_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_ARGS_IND
CsrUint8 *CsrTmBluecoreCmStatusPanicArgsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreCmStatusPanicArgsInd *primitive;

    primitive = (CsrTmBluecoreCmStatusPanicArgsInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->panicStatus);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->panicArg);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->faultStatus);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->faultArg);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_ARGS_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_ARGS_IND
void *CsrTmBluecoreCmStatusPanicArgsIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreCmStatusPanicArgsInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreCmStatusPanicArgsInd *) CsrPmemZalloc(sizeof(CsrTmBluecoreCmStatusPanicArgsInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->panicStatus, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->panicArg, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->faultStatus, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->faultArg, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_ARGS_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_REQ
CsrSize CsrTmBluecoreCmStatusSubscribeReqSizeof(void *msg)
{
    CsrTmBluecoreCmStatusSubscribeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_REQ
CsrUint8 *CsrTmBluecoreCmStatusSubscribeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreCmStatusSubscribeReq *primitive;

    primitive = (CsrTmBluecoreCmStatusSubscribeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_REQ
void *CsrTmBluecoreCmStatusSubscribeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreCmStatusSubscribeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreCmStatusSubscribeReq *) CsrPmemZalloc(sizeof(CsrTmBluecoreCmStatusSubscribeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_PANIC_IND
CsrSize CsrTmBluecorePanicIndSizeof(void *msg)
{
    CsrTmBluecorePanicInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_PANIC_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_PANIC_IND
CsrUint8 *CsrTmBluecorePanicIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecorePanicInd *primitive;

    primitive = (CsrTmBluecorePanicInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_PANIC_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_PANIC_IND
void *CsrTmBluecorePanicIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecorePanicInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecorePanicInd *) CsrPmemZalloc(sizeof(CsrTmBluecorePanicInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_PANIC_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_CFM
CsrSize CsrTmBluecoreCmReplayRegisterCfmSizeof(void *msg)
{
    CsrTmBluecoreCmReplayRegisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_CFM
CsrUint8 *CsrTmBluecoreCmReplayRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreCmReplayRegisterCfm *primitive;

    primitive = (CsrTmBluecoreCmReplayRegisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_CFM
void *CsrTmBluecoreCmReplayRegisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreCmReplayRegisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreCmReplayRegisterCfm *) CsrPmemZalloc(sizeof(CsrTmBluecoreCmReplayRegisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_RES
CsrSize CsrTmBluecoreCmReplayStartResSizeof(void *msg)
{
    CsrTmBluecoreCmReplayStartRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_RES */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_RES
CsrUint8 *CsrTmBluecoreCmReplayStartResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreCmReplayStartRes *primitive;

    primitive = (CsrTmBluecoreCmReplayStartRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_RES */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_RES
void *CsrTmBluecoreCmReplayStartResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreCmReplayStartRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreCmReplayStartRes *) CsrPmemZalloc(sizeof(CsrTmBluecoreCmReplayStartRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_RES */

#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND
CsrSize CsrTmBluecoreDeactivateTransportIndSizeof(void *msg)
{
    CsrTmBluecoreDeactivateTransportInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND
CsrUint8 *CsrTmBluecoreDeactivateTransportIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreDeactivateTransportInd *primitive;

    primitive = (CsrTmBluecoreDeactivateTransportInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->active);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND
void *CsrTmBluecoreDeactivateTransportIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreDeactivateTransportInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreDeactivateTransportInd *) CsrPmemZalloc(sizeof(CsrTmBluecoreDeactivateTransportInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->active, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_RES
CsrSize CsrTmBluecoreDeactivateTransportResSizeof(void *msg)
{
    CsrTmBluecoreDeactivateTransportRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_RES */

#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_RES
CsrUint8 *CsrTmBluecoreDeactivateTransportResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreDeactivateTransportRes *primitive;

    primitive = (CsrTmBluecoreDeactivateTransportRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_RES */

#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_RES
void *CsrTmBluecoreDeactivateTransportResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreDeactivateTransportRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreDeactivateTransportRes *) CsrPmemZalloc(sizeof(CsrTmBluecoreDeactivateTransportRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_RES */

#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_CFM
CsrSize CsrTmBluecoreDeactivateTransportCfmSizeof(void *msg)
{
    CsrTmBluecoreDeactivateTransportCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_CFM
CsrUint8 *CsrTmBluecoreDeactivateTransportCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreDeactivateTransportCfm *primitive;

    primitive = (CsrTmBluecoreDeactivateTransportCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_CFM
void *CsrTmBluecoreDeactivateTransportCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreDeactivateTransportCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreDeactivateTransportCfm *) CsrPmemZalloc(sizeof(CsrTmBluecoreDeactivateTransportCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_CFM
CsrSize CsrTmBluecoreCmPingIntervalSetCfmSizeof(void *msg)
{
    CsrTmBluecoreCmPingIntervalSetCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_CFM
CsrUint8 *CsrTmBluecoreCmPingIntervalSetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreCmPingIntervalSetCfm *primitive;

    primitive = (CsrTmBluecoreCmPingIntervalSetCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_CFM
void *CsrTmBluecoreCmPingIntervalSetCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreCmPingIntervalSetCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreCmPingIntervalSetCfm *) CsrPmemZalloc(sizeof(CsrTmBluecoreCmPingIntervalSetCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_REQ
CsrSize CsrTmBluecoreCmPingIntervalSetReqSizeof(void *msg)
{
    CsrTmBluecoreCmPingIntervalSetReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_REQ
CsrUint8 *CsrTmBluecoreCmPingIntervalSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreCmPingIntervalSetReq *primitive;

    primitive = (CsrTmBluecoreCmPingIntervalSetReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);
    CsrUint32Ser(buffer, length, (CsrUint32) primitive->interval);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_REQ
void *CsrTmBluecoreCmPingIntervalSetReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreCmPingIntervalSetReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreCmPingIntervalSetReq *) CsrPmemZalloc(sizeof(CsrTmBluecoreCmPingIntervalSetReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);
    CsrUint32Des((CsrUint32 *) &primitive->interval, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_REQ
CsrSize CsrTmBluecoreCmReplayRegisterReqSizeof(void *msg)
{
    CsrTmBluecoreCmReplayRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_REQ
CsrUint8 *CsrTmBluecoreCmReplayRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreCmReplayRegisterReq *primitive;

    primitive = (CsrTmBluecoreCmReplayRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_REQ
void *CsrTmBluecoreCmReplayRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreCmReplayRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreCmReplayRegisterReq *) CsrPmemZalloc(sizeof(CsrTmBluecoreCmReplayRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESTART_IND
CsrSize CsrTmBluecoreCmStatusRestartIndSizeof(void *msg)
{
    CsrTmBluecoreCmStatusRestartInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESTART_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESTART_IND
CsrUint8 *CsrTmBluecoreCmStatusRestartIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreCmStatusRestartInd *primitive;

    primitive = (CsrTmBluecoreCmStatusRestartInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESTART_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESTART_IND
void *CsrTmBluecoreCmStatusRestartIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreCmStatusRestartInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreCmStatusRestartInd *) CsrPmemZalloc(sizeof(CsrTmBluecoreCmStatusRestartInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESTART_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESET_IND
CsrSize CsrTmBluecoreCmStatusResetIndSizeof(void *msg)
{
    CsrTmBluecoreCmStatusResetInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESET_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESET_IND
CsrUint8 *CsrTmBluecoreCmStatusResetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreCmStatusResetInd *primitive;

    primitive = (CsrTmBluecoreCmStatusResetInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESET_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESET_IND
void *CsrTmBluecoreCmStatusResetIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreCmStatusResetInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreCmStatusResetInd *) CsrPmemZalloc(sizeof(CsrTmBluecoreCmStatusResetInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESET_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_REPLAY_ERROR_IND
CsrSize CsrTmBluecoreCmStatusReplayErrorIndSizeof(void *msg)
{
    CsrTmBluecoreCmStatusReplayErrorInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_REPLAY_ERROR_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_REPLAY_ERROR_IND
CsrUint8 *CsrTmBluecoreCmStatusReplayErrorIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreCmStatusReplayErrorInd *primitive;

    primitive = (CsrTmBluecoreCmStatusReplayErrorInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->hciOpcode);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_REPLAY_ERROR_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_REPLAY_ERROR_IND
void *CsrTmBluecoreCmStatusReplayErrorIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreCmStatusReplayErrorInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreCmStatusReplayErrorInd *) CsrPmemZalloc(sizeof(CsrTmBluecoreCmStatusReplayErrorInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->hciOpcode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_REPLAY_ERROR_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_REGISTER_REQ
CsrSize CsrTmBluecoreRegisterReqSizeof(void *msg)
{
    CsrTmBluecoreRegisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_REGISTER_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_REGISTER_REQ
CsrUint8 *CsrTmBluecoreRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreRegisterReq *primitive;

    primitive = (CsrTmBluecoreRegisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_REGISTER_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_REGISTER_REQ
void *CsrTmBluecoreRegisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreRegisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreRegisterReq *) CsrPmemZalloc(sizeof(CsrTmBluecoreRegisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_REGISTER_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_IND
CsrSize CsrTmBluecoreCmReplayStartIndSizeof(void *msg)
{
    CsrTmBluecoreCmReplayStartInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_IND
CsrUint8 *CsrTmBluecoreCmReplayStartIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreCmReplayStartInd *primitive;

    primitive = (CsrTmBluecoreCmReplayStartInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_IND
void *CsrTmBluecoreCmReplayStartIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreCmReplayStartInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreCmReplayStartInd *) CsrPmemZalloc(sizeof(CsrTmBluecoreCmReplayStartInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_CFM
CsrSize CsrTmBluecoreUnregisterCfmSizeof(void *msg)
{
    CsrTmBluecoreUnregisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_CFM
CsrUint8 *CsrTmBluecoreUnregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreUnregisterCfm *primitive;

    primitive = (CsrTmBluecoreUnregisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_CFM
void *CsrTmBluecoreUnregisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreUnregisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreUnregisterCfm *) CsrPmemZalloc(sizeof(CsrTmBluecoreUnregisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_REQ
CsrSize CsrTmBluecoreDeactivateTransportReqSizeof(void *msg)
{
    CsrTmBluecoreDeactivateTransportReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_REQ
CsrUint8 *CsrTmBluecoreDeactivateTransportReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreDeactivateTransportReq *primitive;

    primitive = (CsrTmBluecoreDeactivateTransportReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_REQ
void *CsrTmBluecoreDeactivateTransportReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreDeactivateTransportReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreDeactivateTransportReq *) CsrPmemZalloc(sizeof(CsrTmBluecoreDeactivateTransportReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_CFM
CsrSize CsrTmBluecoreCmStatusUnsubscribeCfmSizeof(void *msg)
{
    CsrTmBluecoreCmStatusUnsubscribeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_CFM
CsrUint8 *CsrTmBluecoreCmStatusUnsubscribeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreCmStatusUnsubscribeCfm *primitive;

    primitive = (CsrTmBluecoreCmStatusUnsubscribeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_CFM
void *CsrTmBluecoreCmStatusUnsubscribeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreCmStatusUnsubscribeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreCmStatusUnsubscribeCfm *) CsrPmemZalloc(sizeof(CsrTmBluecoreCmStatusUnsubscribeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_REQ
CsrSize CsrTmBluecoreCmStatusUnsubscribeReqSizeof(void *msg)
{
    CsrTmBluecoreCmStatusUnsubscribeReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_REQ
CsrUint8 *CsrTmBluecoreCmStatusUnsubscribeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreCmStatusUnsubscribeReq *primitive;

    primitive = (CsrTmBluecoreCmStatusUnsubscribeReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_REQ
void *CsrTmBluecoreCmStatusUnsubscribeReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreCmStatusUnsubscribeReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreCmStatusUnsubscribeReq *) CsrPmemZalloc(sizeof(CsrTmBluecoreCmStatusUnsubscribeReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_CFM
CsrSize CsrTmBluecoreCmStatusSubscribeCfmSizeof(void *msg)
{
    CsrTmBluecoreCmStatusSubscribeCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_CFM
CsrUint8 *CsrTmBluecoreCmStatusSubscribeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreCmStatusSubscribeCfm *primitive;

    primitive = (CsrTmBluecoreCmStatusSubscribeCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_CFM
void *CsrTmBluecoreCmStatusSubscribeCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreCmStatusSubscribeCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreCmStatusSubscribeCfm *) CsrPmemZalloc(sizeof(CsrTmBluecoreCmStatusSubscribeCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_RES
CsrSize CsrTmBluecoreActivateTransportResSizeof(void *msg)
{
    CsrTmBluecoreActivateTransportRes *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_RES */

#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_RES
CsrUint8 *CsrTmBluecoreActivateTransportResSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreActivateTransportRes *primitive;

    primitive = (CsrTmBluecoreActivateTransportRes *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_RES */

#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_RES
void *CsrTmBluecoreActivateTransportResDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreActivateTransportRes *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreActivateTransportRes *) CsrPmemZalloc(sizeof(CsrTmBluecoreActivateTransportRes));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_RES */

#ifndef EXCLUDE_CSR_TM_BLUECORE_RESET_IND
CsrSize CsrTmBluecoreResetIndSizeof(void *msg)
{
    CsrTmBluecoreResetInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_RESET_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_RESET_IND
CsrUint8 *CsrTmBluecoreResetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreResetInd *primitive;

    primitive = (CsrTmBluecoreResetInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_RESET_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_RESET_IND
void *CsrTmBluecoreResetIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreResetInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreResetInd *) CsrPmemZalloc(sizeof(CsrTmBluecoreResetInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_RESET_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_REQ
CsrSize CsrTmBluecoreUnregisterReqSizeof(void *msg)
{
    CsrTmBluecoreUnregisterReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_REQ
CsrUint8 *CsrTmBluecoreUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreUnregisterReq *primitive;

    primitive = (CsrTmBluecoreUnregisterReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_REQ
void *CsrTmBluecoreUnregisterReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreUnregisterReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreUnregisterReq *) CsrPmemZalloc(sizeof(CsrTmBluecoreUnregisterReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM
CsrSize CsrTmBluecoreActivateTransportCfmSizeof(void *msg)
{
    CsrTmBluecoreActivateTransportCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM
CsrUint8 *CsrTmBluecoreActivateTransportCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreActivateTransportCfm *primitive;

    primitive = (CsrTmBluecoreActivateTransportCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->result);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM
void *CsrTmBluecoreActivateTransportCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreActivateTransportCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreActivateTransportCfm *) CsrPmemZalloc(sizeof(CsrTmBluecoreActivateTransportCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->result, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_ERROR_REQ
CsrSize CsrTmBluecoreCmReplayErrorReqSizeof(void *msg)
{
    CsrTmBluecoreCmReplayErrorReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_ERROR_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_ERROR_REQ
CsrUint8 *CsrTmBluecoreCmReplayErrorReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreCmReplayErrorReq *primitive;

    primitive = (CsrTmBluecoreCmReplayErrorReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->hciOpcode);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_ERROR_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_ERROR_REQ
void *CsrTmBluecoreCmReplayErrorReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreCmReplayErrorReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreCmReplayErrorReq *) CsrPmemZalloc(sizeof(CsrTmBluecoreCmReplayErrorReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->hciOpcode, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_ERROR_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND
CsrSize CsrTmBluecoreActivateTransportIndSizeof(void *msg)
{
    CsrTmBluecoreActivateTransportInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND
CsrUint8 *CsrTmBluecoreActivateTransportIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreActivateTransportInd *primitive;

    primitive = (CsrTmBluecoreActivateTransportInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND
void *CsrTmBluecoreActivateTransportIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreActivateTransportInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreActivateTransportInd *) CsrPmemZalloc(sizeof(CsrTmBluecoreActivateTransportInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_IND
CsrSize CsrTmBluecoreCmStatusPanicIndSizeof(void *msg)
{
    CsrTmBluecoreCmStatusPanicInd *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_IND
CsrUint8 *CsrTmBluecoreCmStatusPanicIndSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreCmStatusPanicInd *primitive;

    primitive = (CsrTmBluecoreCmStatusPanicInd *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_IND
void *CsrTmBluecoreCmStatusPanicIndDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreCmStatusPanicInd *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreCmStatusPanicInd *) CsrPmemZalloc(sizeof(CsrTmBluecoreCmStatusPanicInd));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_REQ
CsrSize CsrTmBluecoreActivateTransportReqSizeof(void *msg)
{
    CsrTmBluecoreActivateTransportReq *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_REQ
CsrUint8 *CsrTmBluecoreActivateTransportReqSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreActivateTransportReq *primitive;

    primitive = (CsrTmBluecoreActivateTransportReq *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->phandle);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_REQ
void *CsrTmBluecoreActivateTransportReqDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreActivateTransportReq *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreActivateTransportReq *) CsrPmemZalloc(sizeof(CsrTmBluecoreActivateTransportReq));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint16Des((CsrUint16 *) &primitive->phandle, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_REGISTER_CFM
CsrSize CsrTmBluecoreRegisterCfmSizeof(void *msg)
{
    CsrTmBluecoreRegisterCfm *primitive = msg;

    if (primitive)
    {
        return (sizeof(*primitive));
    }
    else
    {
        return (0);
    }
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_REGISTER_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_REGISTER_CFM
CsrUint8 *CsrTmBluecoreRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg)
{
    CsrTmBluecoreRegisterCfm *primitive;

    primitive = (CsrTmBluecoreRegisterCfm *) msg;
    *length = 0;
    CsrUint16Ser(buffer, length, (CsrUint16) primitive->type);
    CsrUint8Ser(buffer, length, (CsrUint8) primitive->active);

    return buffer;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_REGISTER_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_REGISTER_CFM
void *CsrTmBluecoreRegisterCfmDes(CsrUint8 *buffer, CsrSize length)
{
    CsrTmBluecoreRegisterCfm *primitive;
    CsrSize offset;
    offset = 0;
    primitive = (CsrTmBluecoreRegisterCfm *) CsrPmemZalloc(sizeof(CsrTmBluecoreRegisterCfm));

    CsrUint16Des((CsrUint16 *) &primitive->type, buffer, &offset);
    CsrUint8Des((CsrUint8 *) &primitive->active, buffer, &offset);

    return primitive;
}
#endif /* EXCLUDE_CSR_TM_BLUECORE_REGISTER_CFM */

#endif /* EXCLUDE_CSR_TM_BLUECORE_PRIVATE_MODULE */
