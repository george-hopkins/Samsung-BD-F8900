#ifndef CSR_H4_TX_H__
#define CSR_H4_TX_H__
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

CsrUint32 CsrH4MsgSend(H4InstanceData *h4inst, void *msg, CsrUint32 chan);

void CsrH4TxMsgsPump(void);

#ifdef __cplusplus
}
#endif

#endif
