#ifndef CSR_BT_TPT_TASKS_H__
#define CSR_BT_TPT_TASKS_H__

#include "csr_synergy.h"

/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

extern void CsrBtTptInit(void ** gash);
extern void CsrBtTptHandler(void ** gash);
#define TPT_INIT CsrBtTptInit
#define TPT_HANDLER CsrBtTptHandler
#ifdef ENABLE_SHUTDOWN
extern void CsrBtTptDeinit(void ** gash);
#define TPT_DEINIT CsrBtTptDeinit
#else
#define TPT_DEINIT NULL
#endif

#ifdef __cplusplus
}
#endif
#endif
