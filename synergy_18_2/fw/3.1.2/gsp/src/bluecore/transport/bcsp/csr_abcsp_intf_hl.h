#ifndef CSR_ABCSP_INTF_HL_H__
#define CSR_ABCSP_INTF_HL_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_abcsp.h"
#include "csr_abcsp_events.h"
#include "csr_abcsp_panics.h"
#include "csr_abcsp_config_le.h"
#include "csr_abcsp_config_event.h"
#include "csr_abcsp_config_rxmsg.h"
#include "csr_abcsp_config_timer.h"
#include "csr_abcsp_config_txmsg.h"
#include "csr_abcsp_config_panic.h"

#ifdef __cplusplus
extern "C" {
#endif

/* This instance of the program. */

CsrUint8 uart_tx_buffer_empty(void);
void CsrAbcspEvent(CsrUint8 e);
void CsrAbcspReqPumpTxMsgs(void);
void abcsp_panic(CsrUint32 e);
void temp_abcsp_rxmsg_destroy(ABCSP_RXMSG *m);
void CsrAbcspCancelBcspTimer(void);
void CsrAbcspCancelTshyTimer(void);
void CsrAbcspCancelTconfTimer(void);
void CsrAbcspStartTshyTimer(void);
void CsrAbcspStartTconfTimer(void);
void CsrAbcspTxMsgInitRead(ABCSP_TXMSG *msg);
CsrUint32 CsrAbcspTxMsgLength(ABCSP_TXMSG *msg);
CsrUint8 *CsrAbcspTxMsgBufGet(ABCSP_TXMSG *msg, CsrUint32 *buflen);
void CsrAbcspTxMsgTaken(ABCSP_TXMSG *msg, CsrUint32 ntaken);
void CsrAbcspTxMsgDone(ABCSP_TXMSG *msg);
void CsrAbcspUartBytesSend(CsrUint8 *buf, CsrUint32 n);
void CsrAbcspTxMsgDone(ABCSP_TXMSG *msg);
void CsrAbcspPumpTxMsgsOut(void);            /*Called by bg_int to do pump*/

#ifdef __cplusplus
}
#endif

#endif
