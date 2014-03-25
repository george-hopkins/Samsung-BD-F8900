#ifndef CSR_CONFDATA_R_H__
#define CSR_CONFDATA_R_H__
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

/* The block range reserved for Configuration Data */
#define CONFDATA_FIRSTBLOCK                 1
#define CONFDATA_LASTBLOCK                  7
#define CONFDATA_INVALIDBLOCK               (CONFDATA_LASTBLOCK + 1)

CsrUint32 ConfDataFindActiveBlock(void);

#ifdef __cplusplus
}
#endif

#endif
