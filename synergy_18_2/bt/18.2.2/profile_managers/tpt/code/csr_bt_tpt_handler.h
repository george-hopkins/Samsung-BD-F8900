#ifndef CSR_BT_TPT_HANDLER_H__
#define CSR_BT_TPT_HANDLER_H__

#include "csr_synergy.h"

/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


void tptCommandSend(CsrBtTptServerInstanceDataType *TptData, TptCommand_t *cmd);
void tptDelayedReportTransition(CsrUint16 unused, void *inst);
void tptHandleCommand(CsrBtTptServerInstanceDataType *TptData, TptCommand_t *cmd);
void tptSendResultData(CsrBtTptServerInstanceDataType *TptData);

void CsrBtTptHandler(void **gash);


#ifdef __cplusplus
}
#endif

#endif /* !TPT_HANDLER_H__ */
