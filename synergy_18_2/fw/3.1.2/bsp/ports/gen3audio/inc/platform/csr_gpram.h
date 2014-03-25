#ifndef CSR_GPRAM_H__
#define CSR_GPRAM_H__
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

/* Valid range of index: 0-15 */
#define GPRAM(index)       (*((CsrUint32 *) (0x44800080 + ((0xF & index) * 4))))

#ifdef __cplusplus
}
#endif

#endif
