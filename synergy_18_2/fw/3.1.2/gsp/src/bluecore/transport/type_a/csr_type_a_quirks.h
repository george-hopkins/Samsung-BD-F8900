#ifndef CSR_TYPE_A_QUIRKS_H__
#define CSR_TYPE_A_QUIRKS_H__
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

CsrBool CsrSdioAProtSetChipQuirks(CsrSdioAInstanceData *instData);

#ifdef __cplusplus
}
#endif

#endif
