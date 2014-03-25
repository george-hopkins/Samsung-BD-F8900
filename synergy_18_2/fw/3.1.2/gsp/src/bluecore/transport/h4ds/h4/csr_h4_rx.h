#ifndef CSR_H4_RX_H__
#define CSR_H4_RX_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_transport.h"

#ifdef __cplusplus
extern "C" {
#endif

CsrUint16 CsrH4RxMsgPut(CsrUint8 *msg, CsrUint8 chan, CsrUint16 len, CsrBool *flag);

void CsrH4RxMsgFree(MessageStructure *msg);

#ifdef __cplusplus
}
#endif

#endif
