#ifndef CSR_FRAMEWORK_EXT_TYPES_H__
#define CSR_FRAMEWORK_EXT_TYPES_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <windows.h>
#include "csr_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct CsrEvent
{
    HANDLE           event;
    CRITICAL_SECTION mutex;
    CsrUint32        waitingBits;
    CsrUint32        eventBits;
};
typedef struct CsrEvent CsrEventHandle;
typedef HANDLE CsrMutexHandle;
typedef DWORD CsrThreadHandle;

#ifdef __cplusplus
}
#endif

#endif
