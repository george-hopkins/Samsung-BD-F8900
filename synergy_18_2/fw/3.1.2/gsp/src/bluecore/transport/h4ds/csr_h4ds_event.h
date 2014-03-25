#ifndef CSR_H4DS_EVENT_H__
#define CSR_H4DS_EVENT_H__
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

void CsrH4DSEvent(CsrUint8 e);

void CsrH4DSPanic(CsrUint32 p);

#ifdef __cplusplus
}
#endif

#endif
