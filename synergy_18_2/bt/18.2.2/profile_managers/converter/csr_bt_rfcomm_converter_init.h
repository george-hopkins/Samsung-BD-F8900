#ifndef CSR_BT_RFCOMM_CONVERTER_INIT_H__
#define CSR_BT_RFCOMM_CONVERTER_INIT_H__
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

#ifndef EXCLUDE_CSR_BT_RFCOMM_MODULE

#include "csr_msgconv.h"
#ifdef CSR_LOG_ENABLE
#include "csr_log.h"
#endif

void CsrBtRfcommConverterInit(void);
#ifdef CSR_LOG_ENABLE
CsrLogPrimitiveInformation* CsrBtRfcommTechInfoGet(void);
#endif

#else

#define CsrBtRfcommConverterInit()

#endif

#ifdef __cplusplus
}
#endif

#endif /* CSR_BT_RFCOMM_CONVERTER_INIT_H__ */

