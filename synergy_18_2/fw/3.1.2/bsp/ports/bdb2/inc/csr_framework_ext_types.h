#ifndef CSR_FRAMEWORK_EXT_TYPES_H__
#define CSR_FRAMEWORK_EXT_TYPES_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "nucleus.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef NU_EVENT_GROUP CsrEventHandle;
typedef NU_SEMAPHORE CsrMutexHandle;
typedef NU_TASK *CsrThreadHandle;

#ifdef __cplusplus
}
#endif

#endif
