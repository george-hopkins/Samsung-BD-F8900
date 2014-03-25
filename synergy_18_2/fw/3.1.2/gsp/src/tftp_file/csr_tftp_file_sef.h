#ifndef CSR_TFTP_FILE_SEF_H__
#define CSR_TFTP_FILE_SEF_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_tftp_file_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void CsrTftpFileActivateHandler(CsrTftpFileInstanceData *instanceData);
extern void CsrTftpFileDeactivateHandler(CsrTftpFileInstanceData *instanceData);
extern void CsrTftpFileConnectHandler(CsrTftpFileInstanceData *instanceData);
extern void CsrTftpFileReadHandler(CsrTftpFileInstanceData *instanceData);
extern void CsrTftpFileWriteHandler(CsrTftpFileInstanceData *instanceData);
extern void CsrTftpFileDisconnectHandler(CsrTftpFileInstanceData *instanceData);

extern void CsrTftpFileFsalSessionCreateHandler(CsrTftpFileInstanceData *instanceData);
extern void CsrTftpFileFsalSessionDestroyHandler(CsrTftpFileInstanceData *instanceData);
extern void CsrTftpFileFsalOpenHandler(CsrTftpFileInstanceData *instanceData);
extern void CsrTftpFileFsalCloseHandler(CsrTftpFileInstanceData *instanceData);
extern void CsrTftpFileFsalReadHandler(CsrTftpFileInstanceData *instanceData);
extern void CsrTftpFileFsalWriteHandler(CsrTftpFileInstanceData *instanceData);

extern void CsrTftpFileFsalOpenDisconnectedHandler(CsrTftpFileInstanceData *instanceData);
extern void CsrTftpFileFsalReadDisconnectedHandler(CsrTftpFileInstanceData *instanceData);
extern void CsrTftpFileFsalWriteDisconnectedHandler(CsrTftpFileInstanceData *instanceData);

#ifdef __cplusplus
}
#endif

#endif
