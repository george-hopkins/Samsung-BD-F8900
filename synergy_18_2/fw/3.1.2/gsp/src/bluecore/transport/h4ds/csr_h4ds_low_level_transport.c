/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_sched.h"
#include "csr_h4ds_config.h"
#include "csr_h4ds.h"
#include "csr_h4.h"
#include "csr_h4ds_inst.h"
#include "csr_h4ds_time.h"

#ifdef __cplusplus
extern "C" {
#endif

extern H4DSInstanceData CsrH4DSInst;

/* Forward declaration */
void CsrH4DSAlertArmTxBufferEmpty(void);

static void alert_tx_buffer_local(CsrUint16 tmp, void *tmp2)
{
    /* Reset timer id to avoid timers being cancelled later */
    CsrH4DSInst.tx_tid = 0;
    CsrH4DSAlertArmTxBufferEmpty();
}

void CsrH4DSAlertArmRxUartActivity(void)
{
    CsrH4DSInst.rxAlert = TRUE;
}

void CsrH4DSAlertCancelRxUartActivity(void)
{
    CsrH4DSInst.rxAlert = FALSE;
}

void CsrH4DSAlertArmTxBufferEmpty(void)
{
    if (CsrUartDrvLowLevelTransportTxBufLevel(CsrH4DSInst.uartHandle) == 0)
    {
        CsrH4DSAlertTxBufferEmpty();
    }
    else
    {
        CsrH4DSInst.tx_tid = CsrSchedTimerSet((CsrTime) TTX_WATCH_TIME, alert_tx_buffer_local, 0, (void *) (NULL));
    }
}

void CsrH4DSAlertCancelTxBufferEmpty(void)
{
    if (CsrH4DSInst.tx_tid != 0)
    {
        CsrSchedTimerCancel(CsrH4DSInst.tx_tid, 0, NULL);
        CsrH4DSInst.tx_tid = 0;
    }
}

CsrUint32 CsrH4DSUartBuffersEmpty(void)
{
    if ((CsrUartDrvLowLevelTransportTxBufLevel(CsrH4DSInst.uartHandle) == 0) && (CsrUartDrvGetRxAvailable(CsrH4DSInst.uartHandle) == 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static CsrBool h4dsLlHandlePayloadAllocation(void)
{
    CsrBool res = TRUE;
    CsrBool dsHeader = FALSE;

    switch (CsrH4DSInst.headerBuf[0])
    {
        case HCI_HDR_CMD:
        {
            CsrH4DSInst.packetType = HCI_HDR_CMD;
            if (CsrH4DSInst.bytesRead == HCI_COMMAND_HDR_SIZE + H4_PACKET_HEADER_SIZE)
            {
                CsrH4DSInst.payloadLength = CsrH4DSInst.headerBuf[3];
                CsrH4DSInst.totalHciMsgSize = HCI_COMMAND_HDR_SIZE + CsrH4DSInst.payloadLength;
                CsrH4DSInst.payload = CsrPmemAlloc(CsrH4DSInst.totalHciMsgSize);
                CsrMemCpy(CsrH4DSInst.payload, &(CsrH4DSInst.headerBuf[1]), HCI_COMMAND_HDR_SIZE);
                CsrH4DSInst.writePtr = CsrH4DSInst.payload + HCI_COMMAND_HDR_SIZE;
                CsrH4DSInst.bytesRead = 0;
                CsrH4DSInst.reassemblyState = H4DS_LL_REASSEMBLY_READ_BODY_STATE;
            }
            else
            {
                CsrH4DSInst.RemainingBytes = (HCI_COMMAND_HDR_SIZE + H4_PACKET_HEADER_SIZE) - CsrH4DSInst.bytesRead;
            }
            break;
        }
        case HCI_HDR_ACL:
        {
            CsrH4DSInst.packetType = HCI_HDR_ACL;
            if (CsrH4DSInst.bytesRead == HCI_ACL_HDR_SIZE + H4_PACKET_HEADER_SIZE)
            {
                CsrH4DSInst.payloadLength = CsrH4DSInst.headerBuf[4] << 8;                      /* Length field of ACL packts */
                CsrH4DSInst.payloadLength = CsrH4DSInst.payloadLength + CsrH4DSInst.headerBuf[3];  /* are 2 bytes long */
                CsrH4DSInst.totalHciMsgSize = HCI_ACL_HDR_SIZE + CsrH4DSInst.payloadLength;
                CsrH4DSInst.payload = CsrPmemAlloc(CsrH4DSInst.totalHciMsgSize);
                CsrMemCpy(CsrH4DSInst.payload, &(CsrH4DSInst.headerBuf[1]), HCI_ACL_HDR_SIZE);
                CsrH4DSInst.writePtr = CsrH4DSInst.payload + HCI_ACL_HDR_SIZE;
                CsrH4DSInst.bytesRead = 0;
                CsrH4DSInst.reassemblyState = H4DS_LL_REASSEMBLY_READ_BODY_STATE;
            }
            else
            {
                CsrH4DSInst.RemainingBytes = (HCI_ACL_HDR_SIZE + H4_PACKET_HEADER_SIZE) - CsrH4DSInst.bytesRead;
            }
            break;
        }
        case HCI_HDR_SCO:
        {
            CsrH4DSInst.packetType = HCI_HDR_SCO;
            if (CsrH4DSInst.bytesRead == HCI_SCO_HDR_SIZE + H4_PACKET_HEADER_SIZE)
            {
                CsrH4DSInst.payloadLength = CsrH4DSInst.headerBuf[3];
                CsrH4DSInst.totalHciMsgSize = HCI_SCO_HDR_SIZE + CsrH4DSInst.payloadLength;
                CsrH4DSInst.payload = CsrPmemAlloc(CsrH4DSInst.totalHciMsgSize);
                CsrMemCpy(CsrH4DSInst.payload, &(CsrH4DSInst.headerBuf[1]), HCI_SCO_HDR_SIZE);
                CsrH4DSInst.writePtr = CsrH4DSInst.payload + HCI_SCO_HDR_SIZE;
                CsrH4DSInst.bytesRead = 0;
                CsrH4DSInst.reassemblyState = H4DS_LL_REASSEMBLY_READ_BODY_STATE;
            }
            else
            {
                CsrH4DSInst.RemainingBytes = (HCI_SCO_HDR_SIZE + H4_PACKET_HEADER_SIZE) - CsrH4DSInst.bytesRead;
            }
            break;
        }
        case HCI_HDR_EVT:
        {
            CsrH4DSInst.packetType = HCI_HDR_EVT;
            if (CsrH4DSInst.bytesRead == HCI_EVENT_HDR_SIZE + H4_PACKET_HEADER_SIZE)
            {
                CsrH4DSInst.payloadLength = CsrH4DSInst.headerBuf[2];
                CsrH4DSInst.totalHciMsgSize = HCI_EVENT_HDR_SIZE + CsrH4DSInst.payloadLength;
                CsrH4DSInst.payload = CsrPmemAlloc(CsrH4DSInst.totalHciMsgSize);
                CsrMemCpy(CsrH4DSInst.payload, &(CsrH4DSInst.headerBuf[1]), HCI_EVENT_HDR_SIZE);
                CsrH4DSInst.writePtr = CsrH4DSInst.payload + HCI_EVENT_HDR_SIZE;
                CsrH4DSInst.bytesRead = 0;
                CsrH4DSInst.reassemblyState = H4DS_LL_REASSEMBLY_READ_BODY_STATE;
            }
            else
            {
                CsrH4DSInst.RemainingBytes = (HCI_EVENT_HDR_SIZE + H4_PACKET_HEADER_SIZE) - CsrH4DSInst.bytesRead;
            }
            break;
        }
        case H4DS_HDR:
        {
            CsrH4DSInst.packetType = H4DS_HDR;
            dsHeader = TRUE;
            break;
        }
        default:
        {
            res = FALSE;
        }
    }
    if ((res != FALSE) && (dsHeader == TRUE))
    {
        CsrH4DSInst.writePtr = &(CsrH4DSInst.headerBuf[CsrH4DSInst.bytesRead]);
        CsrH4DSInst.reassemblyState = H4DS_LL_REASSEMBLY_READ_DS_HDR_STATE;
    }

    /* Kick start the reading of the payload */
    CsrSchedBgintSet(CsrH4DSInst.bgint_reassemble);

    return res;
}

void CsrH4DSLowLevelPacketReassembly(void *unused)
{
    CsrUint16 bytesFromUart = 0;

    if (CsrH4DSInst.rxAlert)
    {
        CsrH4DSAlertRxUartActivity();
        CsrH4DSAlertCancelRxUartActivity();
    }

    switch (CsrH4DSInst.reassemblyState)
    {
        case H4DS_LL_REASSEMBLY_READ_HEADER_STATE:
        {
            bytesFromUart = (CsrUint16) CsrUartDrvLowLevelTransportRx(CsrH4DSInst.uartHandle, CsrH4DSInst.RemainingBytes, CsrH4DSInst.writePtr);
            CsrH4DSInst.bytesRead = (CsrUint16) (CsrH4DSInst.bytesRead + bytesFromUart);
            CsrH4DSInst.writePtr += bytesFromUart;

            if ((bytesFromUart != 0) || (CsrH4DSInst.RemainingBytes == 0))
            {
                if (h4dsLlHandlePayloadAllocation() == TRUE)
                {
                }
                else
                {
                    CsrH4DSInst.reassemblyState = H4DS_LL_REASSEMBLY_SCAN_DS_STATE;
                    CsrH4DSInst.bytesRead = 0;
                    CsrH4DSInst.writePtr = &(CsrH4DSInst.headerBuf[0]);
                }
            }
            else
            {
                /* Do nothing - wait for more data/next BgInt*/
            }
            break;
        }
        case H4DS_LL_REASSEMBLY_READ_DS_HDR_STATE:
        {
            bytesFromUart = (CsrUint16) CsrUartDrvLowLevelTransportRx(CsrH4DSInst.uartHandle, (CsrUint16) (H4DS_MAX_PACKET_SIZE - CsrH4DSInst.bytesRead), CsrH4DSInst.writePtr);
            CsrH4DSInst.bytesRead = (CsrUint16) (CsrH4DSInst.bytesRead + bytesFromUart);
            CsrH4DSInst.writePtr += bytesFromUart;

            if ((bytesFromUart != 0) || (H4DS_MAX_PACKET_SIZE - CsrH4DSInst.bytesRead == 0))
            {
                if (CsrH4DSInst.bytesRead < H4DS_MAX_PACKET_SIZE)
                {
                    /* Do nothing - wait for more data/next BgInt*/
                }
                else
                {
                    H4DS_RX_PUTMESSAGE(CsrH4DSInst.headerBuf, CsrH4DSInst.packetType, CsrH4DSInst.bytesRead);

                    CsrH4DSInst.reassemblyState = H4DS_LL_REASSEMBLY_READ_HEADER_STATE;
                    CsrH4DSInst.writePtr = &(CsrH4DSInst.headerBuf[0]);
                    CsrH4DSInst.bytesRead = 0;
                    CsrH4DSInst.payloadLength = 0;
                    CsrH4DSInst.RemainingBytes = 1;
                }
            }
            else
            {
                /* Do nothing - wait for more data/next BgInt*/
            }
            break;
        }
        case H4DS_LL_REASSEMBLY_READ_BODY_STATE:
        {
            bytesFromUart = (CsrUint16) CsrUartDrvLowLevelTransportRx(CsrH4DSInst.uartHandle, (CsrUint16) (CsrH4DSInst.payloadLength - CsrH4DSInst.bytesRead), CsrH4DSInst.writePtr);
            CsrH4DSInst.bytesRead = (CsrUint16) (CsrH4DSInst.bytesRead + bytesFromUart);
            CsrH4DSInst.writePtr += bytesFromUart;

            if ((bytesFromUart != 0) || (CsrH4DSInst.RemainingBytes == 0))
            {
                if (CsrH4DSInst.bytesRead < CsrH4DSInst.payloadLength)
                {
                    /* Do nothing - wait for more data/next BgInt*/
                }
                else
                {
                    H4DS_RX_PUTMESSAGE(CsrH4DSInst.payload, CsrH4DSInst.packetType, CsrH4DSInst.totalHciMsgSize);

                    CsrH4DSInst.payload = NULL;
                    CsrH4DSInst.reassemblyState = H4DS_LL_REASSEMBLY_READ_HEADER_STATE;
                    CsrH4DSInst.writePtr = &(CsrH4DSInst.headerBuf[0]);
                    CsrH4DSInst.bytesRead = 0;
                    CsrH4DSInst.payloadLength = 0;
                    CsrH4DSInst.RemainingBytes = 1;
                    CsrH4DSInst.totalHciMsgSize = 0;

                    /* Kick start the reading of the next header */
                    CsrSchedBgintSet(CsrH4DSInst.bgint_reassemble);
                }
            }
            else
            {
                /* Do nothing - wait for more data/next BgInt*/
            }
            break;
        }
        case H4DS_LL_REASSEMBLY_SCAN_DS_STATE:
        {
            bytesFromUart = (CsrUint16) CsrUartDrvLowLevelTransportRx(CsrH4DSInst.uartHandle, 1, CsrH4DSInst.writePtr);
            CsrH4DSInst.bytesRead = (CsrUint16) (CsrH4DSInst.bytesRead + bytesFromUart);
            CsrH4DSInst.writePtr += bytesFromUart;

            if (bytesFromUart != 0)
            {
                if ((CsrH4DSInst.headerBuf[0]) == H4DS_HDR)
                {
                    CsrH4DSInst.reassemblyState = H4DS_LL_REASSEMBLY_READ_DS_HDR_STATE;
                }
                else
                {
                    CsrH4DSInst.reassemblyState = H4DS_LL_REASSEMBLY_SCAN_DS_STATE;
                    CsrH4DSInst.bytesRead = 0;
                    CsrH4DSInst.writePtr = &(CsrH4DSInst.headerBuf[0]);
                }
                CsrSchedBgintSet(CsrH4DSInst.bgint_reassemble);
            }
            else
            {
                /* Do nothing - wait for more data/next BgInt*/
            }
            break;
        }
        default:
        {
            /* State machine in unknown state - This should never happen */
            /* If this actually happens a _very serious_ error occurred! */
            CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_UNEXPECTED_VALUE,
                "h4ds state error");
        }
    }
}

#ifdef __cplusplus
}
#endif
