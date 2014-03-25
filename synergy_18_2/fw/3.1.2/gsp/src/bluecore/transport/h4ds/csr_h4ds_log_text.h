#ifndef CSR_H4DS_LOG_TEXT_H__
#define CSR_H4DS_LOG_TEXT_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_log_text_2.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrH4dsLto);

/* Sub Origins */
#define CSR_H4DS_LTSO_H4    1
#define CSR_H4DS_LTSO_H4DS  2

#ifdef __cplusplus
}
#endif

#endif
