#ifndef CSR_ENV_CONVERTER_INIT_H__
#define CSR_ENV_CONVERTER_INIT_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

/* Note: this is an auto-generated file. */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EXCLUDE_CSR_ENV_MODULE

#include "csr_msgconv.h"
#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#endif

void CsrEnvConverterInit(void);
#ifdef CSR_LOG_ENABLE
CsrLogPrimitiveInformation* CsrEnvTechInfoGet(void);
#endif

#else

#define CsrEnvConverterInit()

#endif

#ifdef __cplusplus
}
#endif

#endif /* CSR_ENV_CONVERTER_INIT_H__ */

