/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_msgconv.h"
#include "csr_pmem.h"
#include "csr_util.h"
#include "csr_hand_coded.h"
#include "csr_tm_bluecore_converter_init.h"
#ifndef EXCLUDE_CSR_TM_BLUECORE_MODULE
#include "csr_tm_bluecore_serialize.h"
#include "csr_tm_bluecore_private_prim.h"

static CsrMsgConvMsgEntry csr_tm_bluecore_conv_lut[] = {
#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM
    { CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM, CsrTmBluecoreActivateTransportCfmSizeof, CsrTmBluecoreActivateTransportCfmSer, CsrTmBluecoreActivateTransportCfmDes, CsrTmBluecoreActivateTransportCfmSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM */
#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND
    { CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND, CsrTmBluecoreActivateTransportIndSizeof, CsrTmBluecoreActivateTransportIndSer, CsrTmBluecoreActivateTransportIndDes, CsrTmBluecoreActivateTransportIndSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND */
#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_REQ
    { CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_REQ, CsrTmBluecoreActivateTransportReqSizeof, CsrTmBluecoreActivateTransportReqSer, CsrTmBluecoreActivateTransportReqDes, CsrTmBluecoreActivateTransportReqSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_REQ */
#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_RES
    { CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_RES, CsrTmBluecoreActivateTransportResSizeof, CsrTmBluecoreActivateTransportResSer, CsrTmBluecoreActivateTransportResDes, CsrTmBluecoreActivateTransportResSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_RES */
#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_CFM
    { CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_CFM, CsrTmBluecoreCmPingIntervalSetCfmSizeof, CsrTmBluecoreCmPingIntervalSetCfmSer, CsrTmBluecoreCmPingIntervalSetCfmDes, CsrTmBluecoreCmPingIntervalSetCfmSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_CFM */
#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_REQ
    { CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_REQ, CsrTmBluecoreCmPingIntervalSetReqSizeof, CsrTmBluecoreCmPingIntervalSetReqSer, CsrTmBluecoreCmPingIntervalSetReqDes, CsrTmBluecoreCmPingIntervalSetReqSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_REQ */
#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_ERROR_REQ
    { CSR_TM_BLUECORE_CM_REPLAY_ERROR_REQ, CsrTmBluecoreCmReplayErrorReqSizeof, CsrTmBluecoreCmReplayErrorReqSer, CsrTmBluecoreCmReplayErrorReqDes, CsrTmBluecoreCmReplayErrorReqSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_ERROR_REQ */
#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_CFM
    { CSR_TM_BLUECORE_CM_REPLAY_REGISTER_CFM, CsrTmBluecoreCmReplayRegisterCfmSizeof, CsrTmBluecoreCmReplayRegisterCfmSer, CsrTmBluecoreCmReplayRegisterCfmDes, CsrTmBluecoreCmReplayRegisterCfmSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_CFM */
#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_REQ
    { CSR_TM_BLUECORE_CM_REPLAY_REGISTER_REQ, CsrTmBluecoreCmReplayRegisterReqSizeof, CsrTmBluecoreCmReplayRegisterReqSer, CsrTmBluecoreCmReplayRegisterReqDes, CsrTmBluecoreCmReplayRegisterReqSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_REQ */
#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_IND
    { CSR_TM_BLUECORE_CM_REPLAY_START_IND, CsrTmBluecoreCmReplayStartIndSizeof, CsrTmBluecoreCmReplayStartIndSer, CsrTmBluecoreCmReplayStartIndDes, CsrTmBluecoreCmReplayStartIndSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_IND */
#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_RES
    { CSR_TM_BLUECORE_CM_REPLAY_START_RES, CsrTmBluecoreCmReplayStartResSizeof, CsrTmBluecoreCmReplayStartResSer, CsrTmBluecoreCmReplayStartResDes, CsrTmBluecoreCmReplayStartResSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_RES */
#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_ARGS_IND
    { CSR_TM_BLUECORE_CM_STATUS_PANIC_ARGS_IND, CsrTmBluecoreCmStatusPanicArgsIndSizeof, CsrTmBluecoreCmStatusPanicArgsIndSer, CsrTmBluecoreCmStatusPanicArgsIndDes, CsrTmBluecoreCmStatusPanicArgsIndSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_ARGS_IND */
#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_IND
    { CSR_TM_BLUECORE_CM_STATUS_PANIC_IND, CsrTmBluecoreCmStatusPanicIndSizeof, CsrTmBluecoreCmStatusPanicIndSer, CsrTmBluecoreCmStatusPanicIndDes, CsrTmBluecoreCmStatusPanicIndSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_IND */
#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_REPLAY_ERROR_IND
    { CSR_TM_BLUECORE_CM_STATUS_REPLAY_ERROR_IND, CsrTmBluecoreCmStatusReplayErrorIndSizeof, CsrTmBluecoreCmStatusReplayErrorIndSer, CsrTmBluecoreCmStatusReplayErrorIndDes, CsrTmBluecoreCmStatusReplayErrorIndSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_REPLAY_ERROR_IND */
#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESET_IND
    { CSR_TM_BLUECORE_CM_STATUS_RESET_IND, CsrTmBluecoreCmStatusResetIndSizeof, CsrTmBluecoreCmStatusResetIndSer, CsrTmBluecoreCmStatusResetIndDes, CsrTmBluecoreCmStatusResetIndSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESET_IND */
#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESTART_IND
    { CSR_TM_BLUECORE_CM_STATUS_RESTART_IND, CsrTmBluecoreCmStatusRestartIndSizeof, CsrTmBluecoreCmStatusRestartIndSer, CsrTmBluecoreCmStatusRestartIndDes, CsrTmBluecoreCmStatusRestartIndSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESTART_IND */
#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_CFM
    { CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_CFM, CsrTmBluecoreCmStatusSubscribeCfmSizeof, CsrTmBluecoreCmStatusSubscribeCfmSer, CsrTmBluecoreCmStatusSubscribeCfmDes, CsrTmBluecoreCmStatusSubscribeCfmSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_CFM */
#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_REQ
    { CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_REQ, CsrTmBluecoreCmStatusSubscribeReqSizeof, CsrTmBluecoreCmStatusSubscribeReqSer, CsrTmBluecoreCmStatusSubscribeReqDes, CsrTmBluecoreCmStatusSubscribeReqSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_REQ */
#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_CFM
    { CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_CFM, CsrTmBluecoreCmStatusUnsubscribeCfmSizeof, CsrTmBluecoreCmStatusUnsubscribeCfmSer, CsrTmBluecoreCmStatusUnsubscribeCfmDes, CsrTmBluecoreCmStatusUnsubscribeCfmSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_CFM */
#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_REQ
    { CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_REQ, CsrTmBluecoreCmStatusUnsubscribeReqSizeof, CsrTmBluecoreCmStatusUnsubscribeReqSer, CsrTmBluecoreCmStatusUnsubscribeReqDes, CsrTmBluecoreCmStatusUnsubscribeReqSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_REQ */
#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_CFM
    { CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_CFM, CsrTmBluecoreDeactivateTransportCfmSizeof, CsrTmBluecoreDeactivateTransportCfmSer, CsrTmBluecoreDeactivateTransportCfmDes, CsrTmBluecoreDeactivateTransportCfmSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_CFM */
#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND
    { CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND, CsrTmBluecoreDeactivateTransportIndSizeof, CsrTmBluecoreDeactivateTransportIndSer, CsrTmBluecoreDeactivateTransportIndDes, CsrTmBluecoreDeactivateTransportIndSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND */
#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_REQ
    { CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_REQ, CsrTmBluecoreDeactivateTransportReqSizeof, CsrTmBluecoreDeactivateTransportReqSer, CsrTmBluecoreDeactivateTransportReqDes, CsrTmBluecoreDeactivateTransportReqSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_REQ */
#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_RES
    { CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_RES, CsrTmBluecoreDeactivateTransportResSizeof, CsrTmBluecoreDeactivateTransportResSer, CsrTmBluecoreDeactivateTransportResDes, CsrTmBluecoreDeactivateTransportResSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_RES */
#ifndef EXCLUDE_CSR_TM_BLUECORE_PANIC_IND
    { CSR_TM_BLUECORE_PANIC_IND, CsrTmBluecorePanicIndSizeof, CsrTmBluecorePanicIndSer, CsrTmBluecorePanicIndDes, CsrTmBluecorePanicIndSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_PANIC_IND */
#ifndef EXCLUDE_CSR_TM_BLUECORE_REGISTER_CFM
    { CSR_TM_BLUECORE_REGISTER_CFM, CsrTmBluecoreRegisterCfmSizeof, CsrTmBluecoreRegisterCfmSer, CsrTmBluecoreRegisterCfmDes, CsrTmBluecoreRegisterCfmSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_REGISTER_CFM */
#ifndef EXCLUDE_CSR_TM_BLUECORE_REGISTER_REQ
    { CSR_TM_BLUECORE_REGISTER_REQ, CsrTmBluecoreRegisterReqSizeof, CsrTmBluecoreRegisterReqSer, CsrTmBluecoreRegisterReqDes, CsrTmBluecoreRegisterReqSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_REGISTER_REQ */
#ifndef EXCLUDE_CSR_TM_BLUECORE_RESET_IND
    { CSR_TM_BLUECORE_RESET_IND, CsrTmBluecoreResetIndSizeof, CsrTmBluecoreResetIndSer, CsrTmBluecoreResetIndDes, CsrTmBluecoreResetIndSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_RESET_IND */
#ifndef EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_CFM
    { CSR_TM_BLUECORE_UNREGISTER_CFM, CsrTmBluecoreUnregisterCfmSizeof, CsrTmBluecoreUnregisterCfmSer, CsrTmBluecoreUnregisterCfmDes, CsrTmBluecoreUnregisterCfmSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_CFM */
#ifndef EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_REQ
    { CSR_TM_BLUECORE_UNREGISTER_REQ, CsrTmBluecoreUnregisterReqSizeof, CsrTmBluecoreUnregisterReqSer, CsrTmBluecoreUnregisterReqDes, CsrTmBluecoreUnregisterReqSerFree },
#endif /* EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_REQ */
    { 0, NULL, NULL, NULL, NULL }
};

void CsrTmBluecoreConverterInit(void)
{
    CsrMsgConvInsert(CSR_TM_BLUECORE_PRIM, csr_tm_bluecore_conv_lut);
}

#ifdef CSR_LOG_ENABLE
static CsrLogPrimitiveInformation csr_tm_bluecore_conv_info = {
    CSR_TM_BLUECORE_PRIM,
    "CSR_TM_BLUECORE_PRIM",
    csr_tm_bluecore_conv_lut
};

CsrLogPrimitiveInformation* CsrTmBluecoreTechInfoGet(void)
{
    return &csr_tm_bluecore_conv_info;
}
#endif
#endif

