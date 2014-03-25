#ifndef CSR_TM_BLUECORE_SEF_H__
#define CSR_TM_BLUECORE_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_log_text_2.h"
#include "csr_tm_bluecore_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrTmBluecoreLto);

#define CSR_BCCMD_PANIC_ARG_VARID             (0x6805)
void CsrTmBlueCoreBccmdMsgHandler(CsrTmBlueCoreInstanceData *tmBcInst);
void CsrTmBlueCoreTmBlueCoreMsgHandler(CsrTmBlueCoreInstanceData *tmBcInst);
void CsrTmBlueCoreHciMsgHandler(CsrTmBlueCoreInstanceData *tmBcInst);

#ifdef __cplusplus
}
#endif

#endif /* CSR_BCCMD_SEF_H__*/
