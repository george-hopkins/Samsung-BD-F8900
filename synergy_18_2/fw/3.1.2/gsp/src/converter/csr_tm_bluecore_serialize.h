#ifndef CSR_TM_BLUECORE_SERIALIZE_H__
#define CSR_TM_BLUECORE_SERIALIZE_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrTm_bluecorePfree(void *ptr);

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_ARGS_IND
CsrSize CsrTmBluecoreCmStatusPanicArgsIndSizeof(void *msg);
CsrUint8 *CsrTmBluecoreCmStatusPanicArgsIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreCmStatusPanicArgsIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreCmStatusPanicArgsIndSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_ARGS_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_REQ
CsrSize CsrTmBluecoreCmStatusSubscribeReqSizeof(void *msg);
CsrUint8 *CsrTmBluecoreCmStatusSubscribeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreCmStatusSubscribeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreCmStatusSubscribeReqSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_PANIC_IND
CsrSize CsrTmBluecorePanicIndSizeof(void *msg);
CsrUint8 *CsrTmBluecorePanicIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecorePanicIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecorePanicIndSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_PANIC_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_CFM
CsrSize CsrTmBluecoreCmReplayRegisterCfmSizeof(void *msg);
CsrUint8 *CsrTmBluecoreCmReplayRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreCmReplayRegisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreCmReplayRegisterCfmSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_RES
CsrSize CsrTmBluecoreCmReplayStartResSizeof(void *msg);
CsrUint8 *CsrTmBluecoreCmReplayStartResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreCmReplayStartResDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreCmReplayStartResSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_RES */

#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND
CsrSize CsrTmBluecoreDeactivateTransportIndSizeof(void *msg);
CsrUint8 *CsrTmBluecoreDeactivateTransportIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreDeactivateTransportIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreDeactivateTransportIndSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_RES
CsrSize CsrTmBluecoreDeactivateTransportResSizeof(void *msg);
CsrUint8 *CsrTmBluecoreDeactivateTransportResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreDeactivateTransportResDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreDeactivateTransportResSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_RES */

#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_CFM
CsrSize CsrTmBluecoreDeactivateTransportCfmSizeof(void *msg);
CsrUint8 *CsrTmBluecoreDeactivateTransportCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreDeactivateTransportCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreDeactivateTransportCfmSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_CFM
CsrSize CsrTmBluecoreCmPingIntervalSetCfmSizeof(void *msg);
CsrUint8 *CsrTmBluecoreCmPingIntervalSetCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreCmPingIntervalSetCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreCmPingIntervalSetCfmSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_REQ
CsrSize CsrTmBluecoreCmPingIntervalSetReqSizeof(void *msg);
CsrUint8 *CsrTmBluecoreCmPingIntervalSetReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreCmPingIntervalSetReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreCmPingIntervalSetReqSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_PING_INTERVAL_SET_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_REQ
CsrSize CsrTmBluecoreCmReplayRegisterReqSizeof(void *msg);
CsrUint8 *CsrTmBluecoreCmReplayRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreCmReplayRegisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreCmReplayRegisterReqSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_REGISTER_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESTART_IND
CsrSize CsrTmBluecoreCmStatusRestartIndSizeof(void *msg);
CsrUint8 *CsrTmBluecoreCmStatusRestartIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreCmStatusRestartIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreCmStatusRestartIndSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESTART_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESET_IND
CsrSize CsrTmBluecoreCmStatusResetIndSizeof(void *msg);
CsrUint8 *CsrTmBluecoreCmStatusResetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreCmStatusResetIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreCmStatusResetIndSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_RESET_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_REPLAY_ERROR_IND
CsrSize CsrTmBluecoreCmStatusReplayErrorIndSizeof(void *msg);
CsrUint8 *CsrTmBluecoreCmStatusReplayErrorIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreCmStatusReplayErrorIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreCmStatusReplayErrorIndSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_REPLAY_ERROR_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_REGISTER_REQ
CsrSize CsrTmBluecoreRegisterReqSizeof(void *msg);
CsrUint8 *CsrTmBluecoreRegisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreRegisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreRegisterReqSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_REGISTER_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_IND
CsrSize CsrTmBluecoreCmReplayStartIndSizeof(void *msg);
CsrUint8 *CsrTmBluecoreCmReplayStartIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreCmReplayStartIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreCmReplayStartIndSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_START_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_CFM
CsrSize CsrTmBluecoreUnregisterCfmSizeof(void *msg);
CsrUint8 *CsrTmBluecoreUnregisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreUnregisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreUnregisterCfmSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_REQ
CsrSize CsrTmBluecoreDeactivateTransportReqSizeof(void *msg);
CsrUint8 *CsrTmBluecoreDeactivateTransportReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreDeactivateTransportReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreDeactivateTransportReqSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_DEACTIVATE_TRANSPORT_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_CFM
CsrSize CsrTmBluecoreCmStatusUnsubscribeCfmSizeof(void *msg);
CsrUint8 *CsrTmBluecoreCmStatusUnsubscribeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreCmStatusUnsubscribeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreCmStatusUnsubscribeCfmSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_REQ
CsrSize CsrTmBluecoreCmStatusUnsubscribeReqSizeof(void *msg);
CsrUint8 *CsrTmBluecoreCmStatusUnsubscribeReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreCmStatusUnsubscribeReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreCmStatusUnsubscribeReqSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_UNSUBSCRIBE_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_CFM
CsrSize CsrTmBluecoreCmStatusSubscribeCfmSizeof(void *msg);
CsrUint8 *CsrTmBluecoreCmStatusSubscribeCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreCmStatusSubscribeCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreCmStatusSubscribeCfmSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_SUBSCRIBE_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_RES
CsrSize CsrTmBluecoreActivateTransportResSizeof(void *msg);
CsrUint8 *CsrTmBluecoreActivateTransportResSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreActivateTransportResDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreActivateTransportResSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_RES */

#ifndef EXCLUDE_CSR_TM_BLUECORE_RESET_IND
CsrSize CsrTmBluecoreResetIndSizeof(void *msg);
CsrUint8 *CsrTmBluecoreResetIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreResetIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreResetIndSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_RESET_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_REQ
CsrSize CsrTmBluecoreUnregisterReqSizeof(void *msg);
CsrUint8 *CsrTmBluecoreUnregisterReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreUnregisterReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreUnregisterReqSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_UNREGISTER_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM
CsrSize CsrTmBluecoreActivateTransportCfmSizeof(void *msg);
CsrUint8 *CsrTmBluecoreActivateTransportCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreActivateTransportCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreActivateTransportCfmSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_CFM */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_ERROR_REQ
CsrSize CsrTmBluecoreCmReplayErrorReqSizeof(void *msg);
CsrUint8 *CsrTmBluecoreCmReplayErrorReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreCmReplayErrorReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreCmReplayErrorReqSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_REPLAY_ERROR_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND
CsrSize CsrTmBluecoreActivateTransportIndSizeof(void *msg);
CsrUint8 *CsrTmBluecoreActivateTransportIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreActivateTransportIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreActivateTransportIndSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_IND
CsrSize CsrTmBluecoreCmStatusPanicIndSizeof(void *msg);
CsrUint8 *CsrTmBluecoreCmStatusPanicIndSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreCmStatusPanicIndDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreCmStatusPanicIndSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_CM_STATUS_PANIC_IND */

#ifndef EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_REQ
CsrSize CsrTmBluecoreActivateTransportReqSizeof(void *msg);
CsrUint8 *CsrTmBluecoreActivateTransportReqSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreActivateTransportReqDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreActivateTransportReqSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_ACTIVATE_TRANSPORT_REQ */

#ifndef EXCLUDE_CSR_TM_BLUECORE_REGISTER_CFM
CsrSize CsrTmBluecoreRegisterCfmSizeof(void *msg);
CsrUint8 *CsrTmBluecoreRegisterCfmSer(CsrUint8 *buffer, CsrSize *length, void *msg);
void *CsrTmBluecoreRegisterCfmDes(CsrUint8 *buffer, CsrSize length);
#define CsrTmBluecoreRegisterCfmSerFree CsrTm_bluecorePfree
#endif /* EXCLUDE_CSR_TM_BLUECORE_REGISTER_CFM */

#ifdef __cplusplus
}
#endif

#endif
