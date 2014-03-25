#ifndef CSR_H4DS_LOW_LEVEL_TRANSPORT_H__
#define CSR_H4DS_LOW_LEVEL_TRANSPORT_H__
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

void CsrH4DSLowLevelPacketReassembly(void *);

void CsrH4DSAlertArmRxUartActivity(void);

void CsrH4DSAlertCancelRxUartActivity(void);

void CsrH4DSAlertArmTxBufferEmpty(void);

void CsrH4DSAlertCancelTxBufferEmpty(void);

CsrUint32 CsrH4DSUartBuffersEmpty(void);

#ifdef __cplusplus
}
#endif

#endif
