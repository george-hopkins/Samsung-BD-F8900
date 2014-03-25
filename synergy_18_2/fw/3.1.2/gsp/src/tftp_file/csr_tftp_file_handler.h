#ifndef CSR_TFTP_FILE_HANDLER_H__
#define CSR_TFTP_FILE_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_fsal_lib.h"
#include "csr_tftp_lib.h"
#include "csr_log_text_2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrTftpFileLto);

#define CSR_TFTP_FILE_STATE_IDLE                          (0x00)
#define CSR_TFTP_FILE_STATE_OPEN                          (0x01)
#define CSR_TFTP_FILE_STATE_FILE_ACCESS                   (0x02)
#define CSR_TFTP_FILE_STATE_ACTIVE                        (0x03)
#define CSR_TFTP_FILE_STATE_DISCONNECTED                  (0x04)

#define CSR_TFTP_FILE_DEFAULT_BLKSIZE                     (512)

typedef struct
{
    void    *msg;
    CsrUint8 state;

    CsrFsalSession sessionId;
    CsrFsalHandle  handle;
} CsrTftpFileInstanceData;

#ifdef __cplusplus
}
#endif

#endif
