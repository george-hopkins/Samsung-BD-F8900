#ifndef CSR_H4DS_TIMERS_H__
#define CSR_H4DS_TIMERS_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void CsrH4DSStartTsyncTimer(void);

void CsrH4DSCancelTsyncTimer(void);

void CsrH4DSStartTconfTimer(void);

void CsrH4DSCancelTconfTimer(void);

void CsrH4DSStartTwuTimer(void);

void CsrH4DSCancelTwuTimer(void);

void CsrH4DSStartTflushTimer(void);

void CsrH4DSCancelTflushTimer(void);

void CsrH4DSStartUartIdleTimer(void);

void CsrH4DSCancelUartIdleTimer(void);

#ifdef __cplusplus
}
#endif

#endif
