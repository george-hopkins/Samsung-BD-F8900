/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_panic.h"

void *CsrUiException(const CsrCharString *exception)
{
    CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_EXCEPTION, NULL);
    return NULL;
}
