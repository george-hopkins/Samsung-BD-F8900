#ifndef CSR_TFTP_FILE_TASK_H__
#define CSR_TFTP_FILE_TASK_H__
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

/* Queue definition for CSR_TFTP_FILE */
extern CsrSchedQid CSR_TFTP_FILE_IFACEQUEUE;

/* Task definition for CSR_TFTP_FILE */
void CsrTftpFileInit(void **gash);
void CsrTftpFileDeinit(void **gash);
void CsrTftpFileHandler(void **gash);

/* CSR_TFTP_FILE */
#define CSR_TFTP_FILE_INIT CsrTftpFileInit
#define CSR_TFTP_FILE_HANDLER CsrTftpFileHandler
#ifdef ENABLE_SHUTDOWN
#define CSR_TFTP_FILE_DEINIT CsrTftpFileDeinit
#else
#define CSR_TFTP_FILE_DEINIT NULL
#endif

#ifdef __cplusplus
}
#endif

#endif
