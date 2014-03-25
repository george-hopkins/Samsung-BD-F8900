#ifndef CSR_ABCSP_PANICS_H__
#define CSR_ABCSP_PANICS_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_panic.h"

#ifdef __cplusplus
extern "C" {
#endif

/* The abcsp library's transmit path checks that each message payload does
not exceed the limit #defined by ABCSP_MAX_MSG_LEN, set in config_txmsg.h.
This panic code is emitted if the limit is exceeded. */

#define ABCSP_PANIC_BCSP_MSG_OVERSIZE    (CSR_PANIC_FW_OVERSIZE_ABCSP_PRIM)

#ifdef __cplusplus
}
#endif

#endif
