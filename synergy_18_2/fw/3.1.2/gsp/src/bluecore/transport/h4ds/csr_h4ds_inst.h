#ifndef CSR_H4DS_INST_H__
#define CSR_H4DS_INST_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sched.h"

#ifdef __cplusplus
extern "C" {
#endif

#define H4DS_LL_REASSEMBLY_READ_HEADER_STATE      (0x00)
#define H4DS_LL_REASSEMBLY_READ_BODY_STATE        (0x01)
#define H4DS_LL_REASSEMBLY_READ_DS_HDR_STATE      (0x02)
#define H4DS_LL_REASSEMBLY_SCAN_DS_STATE          (0x03)

#define H4DS_HDR                                  (0x40)

#define H4DS_MAX_PACKET_SIZE                      (12)

/* Instance data for H4 */
typedef struct
{
    CsrSchedTid tsync_tid;
    CsrSchedTid tconf_tid;
    CsrSchedTid twu_tid;
    CsrSchedTid tflush_tid;
    CsrSchedTid uart_idle_tid;
    CsrSchedTid tx_tid;
    CsrUint8    pumpRequest;
    CsrBool     rxAlert;
    CsrSchedTid txBufNotReadyTid;

    /* Low level H4 packet reassembly variables */
    CsrUint8  reassemblyState;
    CsrUint8  headerBuf[H4DS_MAX_PACKET_SIZE];
    CsrUint16 bytesRead;
    CsrUint8 *writePtr;
    CsrUint16 payloadLength;
    CsrUint8 *payload;
    CsrUint8  packetType;
    CsrUint16 RemainingBytes;
    CsrUint16 totalHciMsgSize;

    /* CsrSchedBgintSet handles */
    CsrSchedBgint bgint_reassemble;
    CsrSchedBgint bgint_pump;

    void *uartHandle;
} H4DSInstanceData;

extern H4DSInstanceData CsrH4DSInst;

void CsrH4dsRegisterUartHandle(void *handle);

#ifdef __cplusplus
}
#endif

#endif
