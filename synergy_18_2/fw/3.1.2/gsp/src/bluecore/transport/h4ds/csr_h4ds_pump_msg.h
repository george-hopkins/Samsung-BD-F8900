#ifndef CSR_H4DS_PUMP_MSG_H__
#define CSR_H4DS_PUMP_MSG_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void CsrH4DSMsgPump(void *);

void CsrH4DSPumpTxMsgsReq(void);

void CsrH4PumpTxReq(void);

#ifdef __cplusplus
}
#endif

#endif
