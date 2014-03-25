#ifndef CSR_VM_HANDLER_H__
#define CSR_VM_HANDLER_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_usr_config.h"
#include "csr_types.h"
#include "csr_sched.h"

#ifdef __cplusplus
extern "C" {
#endif

void CsrVmFreeDownstreamMessageContents(CsrUint16 eventClass, void *message);

typedef struct
{
    CsrSchedQid phandle;
#ifdef CSR_BLUECORE_ONOFF
    CsrBool deactivating;
#endif
} CsrVmInstanceData;

#ifdef __cplusplus
}
#endif

#endif /* CSR_VM_HANDLER_H__ */
