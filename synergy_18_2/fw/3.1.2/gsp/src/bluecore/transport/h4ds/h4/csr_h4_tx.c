/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include "csr_panic.h"
#include "csr_queue_lib.h"
#include "csr_h4ds_config.h"
#include "csr_h4ds_private.h"
#include "csr_h4.h"

#define SIZE_ONE_BYTE           (1)
#define LAST_FRAG_FLAG_TRUE     (1)
#define LAST_FRAG_FLAG_FALSE    (0)

static void h4_buf_full_end_wait(CsrUint16 tmp, void *tmp2)
{
    CsrH4Inst.txBufFullTid = 0;
    H4_REQ_PUMPTXMSGS(CsrH4Inst);
}

static void h4_buf_full_start_wait(void)
{
    if (CsrH4DSCanTransmit())
    {
        /* Tx buffer is full. Give it a little piece and quite to send something */
        if (CsrH4Inst.txBufFullTid != 0)
        {
            CsrSchedTimerCancel(CsrH4Inst.txBufFullTid, 0, NULL);
        }
        CsrH4Inst.txBufFullTid = CsrSchedTimerSet((CsrTime) CsrH4Inst.txBufFullWaitTime, h4_buf_full_end_wait, 0, (void *) (NULL));
    }
    else
    {
        /* TX buffer not full. Push on! */
        H4_REQ_PUMPTXMSGS(CsrH4Inst);
    }
}

/***********************************************************************
    Set up a message to be send to the UART driver and request a
    background interrupt
***********************************************************************/
CsrUint32 CsrH4MsgSend(H4InstanceData *h4inst, void *msg, CsrUint32 chan)
{
    /* Parameter sanity checks. */
    if ((chan < TRANSPORT_CHANNEL_HCI) || (chan > TRANSPORT_CHANNEL_SCO) || (msg == NULL))
    {
        H4_EVENT(H4_EVT_DUFF_TX_PARAMS);
        return 0;
    }

    if ((chan == TRANSPORT_CHANNEL_HCI) || (chan == TRANSPORT_CHANNEL_ACL))
    {
        QMSG_STORE(&h4inst->sendQ, msg);
    }
    else if (chan == TRANSPORT_CHANNEL_SCO)
    {
        /* SCO message */
        if (h4inst->scoData[h4inst->scoIndex] != NULL)
        {
            CsrPmemFree(h4inst->scoData[h4inst->scoIndex]);
        }
        h4inst->scoData[h4inst->scoIndex] = msg;
    }

    /* Tell external code that it needs to call CsrH4TxMsgsPump(). */
    H4_REQ_PUMPTXMSGS(h4inst);

    /* Report message accepted. */
    return 1;
}

static CsrUint32 h4_execute_txpump(H4InstanceData *h4inst)
{
    CsrUint16 num_send = 0;
    CsrUint8 hciChannel;

    /* If the buffer is full and a timer is running wait for this timer to fire
       before sending anything */
    if (!h4inst->txBufFullTid)
    {
        /* Check to see if there is an ongoing transmission */
        if (h4inst->currMsg == NULL)
        {
            /* Check to see if SCO data is available. This must be send first */
            if (h4inst->scoData[h4inst->scoIndex] != NULL)
            {
                CsrUint8 oldPtr = h4inst->scoIndex;

                hciChannel = HCI_HDR_SCO;
                /* Send a HCI SCO header followed by the SCO data itself */
                if (H4_UART_SENDBYTES(&hciChannel, SIZE_ONE_BYTE, LAST_FRAG_FLAG_FALSE))
                {
                    /*if (H4_UART_SENDBYTES(&hciChannel, SIZE_ONE_BYTE, LAST_FRAG_FLAG_TRUE))*/
                    if (h4inst->scoIndex == 0)
                    {
                        h4inst->scoIndex = 1;
                    }
                    else
                    {
                        h4inst->scoIndex = 0;
                    }
                    if (H4_UART_SENDBYTES(h4inst->scoData[oldPtr], (((CsrUint8 *) h4inst->scoData[oldPtr])[2] + 3),
                            LAST_FRAG_FLAG_TRUE))
                    {
                        CsrPmemFree(h4inst->scoData[oldPtr]);
                        h4inst->scoData[oldPtr] = NULL;
                    }
                }

                /* End function successfully */
                return 1;
            }
            else
            {
                /* If no message is being send and no SCO data is waiting get and send
                    data from the queue */
                h4inst->currMsg = CsrQueueMsgTake(&h4inst->sendQ);

                /* If no data is retrieved nothing need to be sent */
                if (h4inst->currMsg == NULL)
                {
                    return 0;
                }
            }
        }

        /* Send a header byte prior to any data */
        if (h4inst->currMsg->chan == TRANSPORT_CHANNEL_ACL)
        {
            hciChannel = HCI_HDR_ACL;
        }
        else
        {
            hciChannel = HCI_HDR_CMD;
        }

        if (h4inst->txState == H4_TX_HDR_STATE)
        {
            if (H4_UART_SENDBYTES(&hciChannel, SIZE_ONE_BYTE, LAST_FRAG_FLAG_FALSE))
            {
                /*if (H4_UART_SENDBYTES(&hciChannel, SIZE_ONE_BYTE, LAST_FRAG_FLAG_TRUE))*/
                h4inst->txState = H4_TX_BODY_STATE;
            }
            else
            {
                h4_buf_full_start_wait();
                return 1;
            }
        }


        /* Send the HCI package itself */
        if ((h4inst->txState == H4_TX_BODY_STATE) || ((h4inst->txState == H4_TX_HDR_STATE) && (h4inst->currMsg->m.dex > 0)))
        {
            /* The typecast is necessary because the deep sleep lib returns an CsrUint32. */
            /* however the CSRMAX packet size is 4k so it fits comfortably in a CsrUint16 */
            num_send = (CsrUint16) H4_UART_SENDBYTES((CsrUint8 *) (h4inst->currMsg->m.buf + h4inst->currMsg->m.dex),
                (h4inst->currMsg->m.buflen - h4inst->currMsg->m.dex), LAST_FRAG_FLAG_TRUE);

            if (num_send)
            {
                /* If everything is sent free the data */
                if ((h4inst->currMsg->m.dex + num_send) == h4inst->currMsg->m.buflen)
                {
                    h4inst->txState = H4_TX_HDR_STATE;

                    CsrPmemFree(h4inst->currMsg->m.buf);
                    CsrPmemFree(h4inst->currMsg);
                    h4inst->currMsg = NULL;

                    /* If the queue is not yet empty request another package be sent */
                    if (CsrQueueLength(h4inst->sendQ))
                    {
                        H4_REQ_PUMPTXMSGS(h4inst);
                    }
                }
                /* If not all data is sent update index and request another pump */
                else if ((h4inst->currMsg->m.dex + num_send) < h4inst->currMsg->m.buflen)
                {
                    h4inst->currMsg->m.dex += num_send;
                    h4inst->txState = H4_TX_BODY_STATE;
                    h4_buf_full_start_wait();
                }
                else
                {
                    /* Should NEVER happen! */
                    H4_EVENT(H4_EVT_TOO_MUCH_TX);
                    CsrPanic(CSR_TECH_FW, CSR_PANIC_FW_H4_TX_OVERRUN,
                        "h4 tx overrun");
                    return 0;
                }
            }
            else
            {
                h4_buf_full_start_wait();
            }
        }
    }

    return 1;
}

/***********************************************************************
    On arrival of a background interrupt send data to the UART
    driver. First finish send already started messages, then send
    SCO data if any and finally send everything else.
***********************************************************************/
void CsrH4TxMsgsPump(void)
{
    /* The return value of this is needed in H4DS */
    h4_execute_txpump(&CsrH4Inst);
}
