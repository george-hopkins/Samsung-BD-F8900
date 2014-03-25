#ifndef CSR_H4DS_PANICS_H__
#define CSR_H4DS_PANICS_H__
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


/* This panic code indicates the h4ds library has detected that something
"impossible" has happened, normally that a state machine's state variable has
been set to an "impossible" value.  This signals gross failure of the h4ds
library; external code should terminate h4ds operation. */

#define H4DS_PANIC_INT_CORRUPTION       ((CsrUint32) (1))


#ifdef __cplusplus
}
#endif

#endif
