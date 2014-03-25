/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_h4.h"
#include "csr_h4_tx.h"
#include "csr_h4ds.h"
#include "csr_h4ds_inst.h"
#include "csr_sched.h"
#include "csr_queue_lib.h"
#include "csr_h4ds_pump_msg.h"
#include "csr_h4ds_low_level_transport.h"
#include "csr_serial_com.h"

#ifdef __cplusplus
extern "C" {
#endif

H4InstanceData CsrH4Inst;
H4DSInstanceData CsrH4DSInst;

void CsrH4dsRegisterUartHandle(void *handle)
{
    CsrH4DSInst.uartHandle = handle;
}

/*----------------------------------------------------------------------------*
* NAME
*     register_uartDrv_handlers
*
* DESCRIPTION
*
*   This function is called to register bg_int for uart 0
*   and for uart 1. driver.
*
*    This function MUST be called before CsrUartDrvStart.
*
* RETURNS
*    None
*
*/
static void register_uartDrv_handlers(void)
{
    CsrH4DSInst.bgint_reassemble = CsrSchedBgintReg(CsrH4DSLowLevelPacketReassembly, NULL, "h4ds_reassemble");
    CsrH4DSInst.bgint_pump = CsrSchedBgintReg(CsrH4DSMsgPump, NULL, "CsrH4DSPump"); /*The bg int needed for abcsp pumps*/
    CsrUartDrvRegister(CsrH4DSInst.uartHandle, NULL, CsrH4DSInst.bgint_reassemble);
}

static void freeTxmsg(TXMSG *msg)
{
    CsrPmemFree(msg->m.buf);
    CsrPmemFree(msg);
}

void CsrH4DSInitialize(void)
{
    CsrUint8 i;

    register_uartDrv_handlers();

    H4_EVENT(H4_EVT_START);

    CsrH4Inst.sendQ = NULL;
    CsrH4Inst.currMsg = NULL;
    CsrH4Inst.scoData[0] = NULL;
    CsrH4Inst.scoData[1] = NULL;
    CsrH4Inst.scoIndex = 0;
    CsrH4Inst.state = H4_INIT_STATE;
    CsrH4Inst.txState = H4_TX_HDR_STATE;
    CsrH4Inst.txBufFullWaitTime = 0;
    CsrH4Inst.txBufFullTid = 0;
    CsrH4Inst.txBufSize = 0;
    CsrH4Inst.pumpRequest = 0;

    CsrH4Inst.eventMsg = NULL;
    CsrH4Inst.eventMsgOffset = 0;
    CsrH4Inst.eventChannel = 0;
    for (i = 0; i < EVENT_BUFFER_SIZE; i++)
    {
        CsrH4Inst.eventBuf[i].msg = NULL;
        CsrH4Inst.eventBuf[i].len = 0;
    }
    CsrH4Inst.eventBufCurr = 0;

    H4_EVENT(H4_EVT_INITIATED);

    CsrH4DSInit();

    CsrH4DSInst.tsync_tid = 0;
    CsrH4DSInst.txBufNotReadyTid = 0;
    CsrH4DSInst.tconf_tid = 0;
    CsrH4DSInst.twu_tid = 0;
    CsrH4DSInst.tflush_tid = 0;
    CsrH4DSInst.uart_idle_tid = 0;
    CsrH4DSInst.tx_tid = 0;
    CsrH4DSInst.pumpRequest = 1;
    CsrH4DSInst.reassemblyState = H4DS_LL_REASSEMBLY_READ_HEADER_STATE;
    CsrH4DSInst.bytesRead = 0;
    CsrH4DSInst.writePtr = &(CsrH4DSInst.headerBuf[0]);
    CsrH4DSInst.payloadLength = 0;
    CsrH4DSInst.payload = NULL;
    CsrH4DSInst.packetType = 0;
    CsrH4DSInst.RemainingBytes = 1;
    CsrH4DSInst.totalHciMsgSize = 0;
}

void CsrH4DSDeinitialize(void)
{
    CsrUint8 i = 0;

    CsrSchedBgintUnreg(CsrH4DSInst.bgint_reassemble);
    CsrSchedBgintUnreg(CsrH4DSInst.bgint_pump);

    if (CsrH4Inst.state == H4_INIT_STATE)
    {
        if (CsrH4Inst.currMsg != NULL)
        {
            freeTxmsg(CsrH4Inst.currMsg);
            CsrH4Inst.currMsg = NULL;
        }

        if (CsrH4Inst.sendQ != NULL)
        {
            void *msg;
            CsrUint8 ch;
            CsrUint8 rel;

            CsrQueueDestroy(&CsrH4Inst.sendQ, &msg, &ch, &rel);
        }

        while (i < CsrH4Inst.eventBufCurr)
        {
            CsrPmemFree(CsrH4Inst.eventBuf[i].msg);
            CsrH4Inst.eventBuf[i].msg = NULL;
            CsrH4Inst.eventBuf[i].len = 0;
            i++;
        }
        CsrH4Inst.eventBufCurr = 0;

        if (CsrH4Inst.scoData[0] != NULL)
        {
            CsrPmemFree(CsrH4Inst.scoData[0]);
            CsrH4Inst.scoData[0] = NULL;
        }
        if (CsrH4Inst.scoData[1] != NULL)
        {
            CsrPmemFree(CsrH4Inst.scoData[1]);
            CsrH4Inst.scoData[1] = NULL;
        }

        CsrH4Inst.state = H4_UNINIT_STATE;
    }
    H4_EVENT(H4_EVT_DEINITIATED);

    CsrH4DSInst.writePtr = NULL;

    if (CsrH4DSInst.payload != NULL)
    {
        CsrPmemFree(CsrH4DSInst.payload);
        CsrH4DSInst.payload = NULL;
    }

    if (CsrH4DSInst.tsync_tid != 0)
    {
        CsrSchedTimerCancel(CsrH4DSInst.tsync_tid, 0, NULL);
        CsrH4DSInst.tsync_tid = 0;
    }

    if (CsrH4DSInst.tconf_tid != 0)
    {
        CsrSchedTimerCancel(CsrH4DSInst.tconf_tid, 0, NULL);
        CsrH4DSInst.tconf_tid = 0;
    }

    if (CsrH4DSInst.twu_tid != 0)
    {
        CsrSchedTimerCancel(CsrH4DSInst.twu_tid, 0, NULL);
        CsrH4DSInst.twu_tid = 0;
    }

    if (CsrH4DSInst.tflush_tid != 0)
    {
        CsrSchedTimerCancel(CsrH4DSInst.tflush_tid, 0, NULL);
        CsrH4DSInst.tflush_tid = 0;
    }

    if (CsrH4DSInst.uart_idle_tid != 0)
    {
        CsrSchedTimerCancel(CsrH4DSInst.uart_idle_tid, 0, NULL);
        CsrH4DSInst.uart_idle_tid = 0;
    }

    if (CsrH4DSInst.tx_tid != 0)
    {
        CsrSchedTimerCancel(CsrH4DSInst.tx_tid, 0, NULL);
        CsrH4DSInst.tx_tid = 0;
    }

    if (CsrH4Inst.txBufFullTid != 0)
    {
        CsrSchedTimerCancel(CsrH4Inst.txBufFullTid, 0, NULL);
        CsrH4Inst.txBufFullTid = 0;
    }
}

#ifdef __cplusplus
}
#endif
