#ifndef CSR_DELAY_H__
#define CSR_DELAY_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Blocks execution in a busy loop for a specified amount of micro seconds */
void DelayUs(CsrUint32 delay);

#ifdef __cplusplus
}
#endif

#endif
