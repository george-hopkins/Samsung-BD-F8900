#ifndef CSR_TM_BLUECORE_PRIVATE_LIB_H__
#define CSR_TM_BLUECORE_PRIVATE_LIB_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_tm_bluecore_private_prim.h"
#include "csr_tm_bluecore_task.h"
#include "csr_msg_transport.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrTmBluecoreResetInd *CsrTmBluecoreResetInd_struct(void);
#define CsrTmBluecoreResetIndSend(){            \
        CsrTmBluecoreResetInd *msg__; \
        msg__ = CsrTmBluecoreResetInd_struct(); \
        CsrMsgTransport(CSR_TM_BLUECORE_IFACEQUEUE, CSR_TM_BLUECORE_PRIM, msg__);}

CsrTmBluecorePanicInd *CsrTmBluecorePanicInd_struct(void);
#define CsrTmBluecorePanicIndSend(){            \
        CsrTmBluecorePanicInd *msg__; \
        msg__ = CsrTmBluecorePanicInd_struct(); \
        CsrMsgTransport(CSR_TM_BLUECORE_IFACEQUEUE, CSR_TM_BLUECORE_PRIM, msg__);}

void CsrTmBluecorePrivateFreeUpstreamMessageContents(CsrUint16 eventClass, void *message);
void CsrTmBluecorePrivateFreeDownstreamMessageContents(CsrUint16 eventClass, void *message);

#ifdef __cplusplus
}
#endif

#endif /* CSR_TM_BLUECORE_PRIVATE_LIB_H__ */
