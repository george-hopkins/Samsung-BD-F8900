#ifndef CSR_LOG_FTS_H__
#define CSR_LOG_FTS_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_log_formats.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrLog *CsrLogFtsCreate(void *ltHdl);
void CsrLogFtsDestroy(CsrLog *l);

#ifdef __cplusplus
}
#endif

#endif
