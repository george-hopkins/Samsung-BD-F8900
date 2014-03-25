#ifndef CSR_BCCMD_SEF_H__
#define CSR_BCCMD_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_bccmd_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrBccmdRemoveBootType(CsrBccmdBoot **bootVar);
void csrBccmdPfreePointerArray(void **bccmdPtrArray, CsrUint16 numOfIndices);
void CsrBccmdReqHandler(CsrBccmdInstanceData *bccmdInst);
void CsrBccmdFlushReqHandler(CsrBccmdInstanceData *bccmdInst);
void CsrBccmdBootstrapReqHandler(CsrBccmdInstanceData *bccmdInst);
void CsrBccmdResetResHandler(CsrBccmdInstanceData *bccmdInst);
void CsrBccmdGetBuildIdHandler(CsrBccmdInstanceData *bccmdInst);
CsrBool CsrBccmdHciVendorSpecificEventIndHandler(CsrBccmdInstanceData *bccmdInst);
void CsrBccmdRestoreMessageHandler(CsrBccmdInstanceData *bccmdInst);
void CsrTmBlueCoreActivateTransportCfmHandler(CsrBccmdInstanceData *bccmdInst);
void CsrBccmdCfmSend(CsrSchedQid phandle, CsrUint8 *payload, CsrUint16 payloadLength);
#ifdef CSR_CHIP_MANAGER_ENABLE
void CsrTmBlueCoreCmReplayRegisterCfmHandler(CsrBccmdInstanceData *bccmdInst);
void CsrTmBlueCoreCmReplayStartIndHandler(CsrBccmdInstanceData *bccmdInst);
void CsrTmBlueCoreCmStatusResetIndHandler(CsrBccmdInstanceData *bccmdInst);
void CsrBccmdPanicArgsHandler(CsrBccmdInstanceData *bccmdInst);
#endif

#ifdef __cplusplus
}
#endif

#endif /* CSR_BCCMD_SEF_H__*/
