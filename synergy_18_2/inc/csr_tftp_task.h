#ifndef CSR_TFTP_TASK_H__
#define CSR_TFTP_TASK_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Queue definition for CSR_TFTP */
extern CsrSchedQid CSR_TFTP_IFACEQUEUE;

/* Task definition for CSR_TFTP */
void CsrTftpInit(void **gash);
void CsrTftpDeinit(void **gash);
void CsrTftpHandler(void **gash);

/* CSR_TFTP */
#define CSR_TFTP_INIT CsrTftpInit
#define CSR_TFTP_HANDLER CsrTftpHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_TFTP_DEINIT CsrTftpDeinit
#else
#define CSR_TFTP_DEINIT NULL
#endif

#ifdef __cplusplus
}
#endif

#endif
