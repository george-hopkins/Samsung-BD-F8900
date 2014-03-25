#ifndef CSR_FSAL_SEF_UTILS_H__
#define CSR_FSAL_SEF_UTILS_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_fsal_prim.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrFileOpenFlags CsrFsalLookupOpenFlags(CsrFsalFileOpenFlags flags);
CsrFilePerms CsrFsalLookupPerms(CsrFsalFileOpenPerms perms);

CsrResult CsrFsalFileSize(const CsrUtf8String *file, CsrSize *filesize);

CsrResult CsrFsalRemoveRecursively(CsrUtf8String *path);

#ifdef __cplusplus
}
#endif

#endif
