#ifndef CSR_H4DS_TX_HANDLE_H__
#define CSR_H4DS_TX_HANDLE_H__
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

CsrUint32 CsrH4DSUartBytesSend(const CsrUint8 *buf, CsrUint32 n);

#ifdef __cplusplus
}
#endif

#endif
