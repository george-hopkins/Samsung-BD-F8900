#ifndef CSR_AM_SEF_H__
#define CSR_AM_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_result.h"
#include "csr_am_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrAmSaveMessage(AmInstanceDataType *inst);
void CsrAmDiscardMessage(AmInstanceDataType *inst);

/* Request handlers */
void CsrAmInitReqHandler(AmInstanceDataType *inst);
void CsrAmInitReqBusyHandler(AmInstanceDataType *inst);
void CsrAmAudioPathConnectReqHandler(AmInstanceDataType *inst);
void CsrAmAudioPathAddSinkReqHandler(AmInstanceDataType *inst);
void CsrAmAudioPathRemoveSinkReqHandler(AmInstanceDataType *inst);
void CsrAmAudioPathConfigReqHandler(AmInstanceDataType *inst);
void CsrAmAudioPathReleaseReqHandler(AmInstanceDataType *inst);

/* Confirmations */
void CsrAmInitCfmSend(AmInstanceDataType *inst, CsrSchedQid appHandle, CsrResult result);
void CsrAmAudioPathConnectCfmSend(AmInstanceDataType *inst, CsrResult result, CsrUint16 connId);
void CsrAmAudioPathAddSinkCfmSend(AmInstanceDataType *inst, CsrResult result, CsrUint16 connId);
void CsrAmAudioPathRemoveSinkCfmSend(AmInstanceDataType *inst, CsrResult result, CsrUint16 connId);
void CsrAmAudioPathConfigCfmSend(AmInstanceDataType *inst, CsrResult result, CsrUint16 connId);
void CsrAmAudioPathReleaseCfmSend(AmInstanceDataType *inst, CsrResult result, CsrUint16 connId);
void CsrAmAudioPathReleaseIndSend(AmInstanceDataType *inst, CsrUint16 connId);


#ifdef CSR_CHIP_MANAGER_ENABLE

void CsrAmResetIndHandler(AmInstanceDataType *inst);
void CsrAmRestartIndHandler(AmInstanceDataType *inst);

#endif

#ifdef __cplusplus
}
#endif

#endif /* CSR_AM_SEF_H__ */
