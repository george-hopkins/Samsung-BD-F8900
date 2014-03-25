/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "nucleus.h"

#include "csr_types.h"
#include "platform/csr_uart.h"
#include "platform/csr_interrupt.h"

/* Trigger level for hardware transmit FIFO interrupt - performance parameter.
   Must be any of 0, 16, 32 or 48.
   A high trigger level means smaller more frequent data transfer bursts
   from software to hardware buffer, which is less efficient.
   A low trigger level means larger less frequent data transfer bursts
   from software to hardware buffer, which is more efficient.
   If the trigger level is too low, and the baud rate is too high, the
   hardware shifter will consume the data in the hardware buffer before
   more data can be transfered to the hardware buffer. This results in
   gaps in the serial data stream. In this case, setting the trigger
   level higher reduces the gaps (or eliminates them), at the cost of
   more processing power. Consequently the optimal parameter value is a
   system and application dependent parameter. Recommended general purpose
   default value is 16. */
#define UART_TX_TRIGGER_LEVEL 16

/* Configure baud rate (UBRDIV) - use UART_BAUD_XXX in csr_uart.h */
#define UART_BAUD_SET(ch, value)    {rUBRDIV(ch) = value;}

/* Configure UART (ULCON) - use UART_CONFIG_XXX in csr_uart.h */
#define UART_CON_SET(ch, value)     {rULCON(ch) = value;}

/* UART Control (UCON) */
#define UART_CTRL_CLKSOURCE_PCLK        (0 << 10)
#define UART_CTRL_CLKSOURCE_UARTCLK     (1 << 10)
#define UART_CTRL_TXINT_TRIGGER_PULSE   (0 << 9)
#define UART_CTRL_TXINT_TRIGGER_LEVEL   (1 << 9)
#define UART_CTRL_RXINT_TRIGGER_PULSE   (0 << 8)
#define UART_CTRL_RXINT_TRIGGER_LEVEL   (1 << 8)
#define UART_CTRL_RXINT_TIMEOUT         (1 << 7)
#define UART_CTRL_RXINT_OVERRUN         (1 << 6)
#define UART_CTRL_LOOPBACK              (1 << 5)
#define UART_CTRL_TXMODE_DISABLE        (0 << 2)
#define UART_CTRL_TXMODE_INT            (1 << 2)
#define UART_CTRL_TXMODE_DMA_0_2        (2 << 2)    /* Only for UART0 */
#define UART_CTRL_TXMODE_DMA_1_3        (3 << 2)    /* Only for UART1 */
#define UART_CTRL_RXMODE_DISABLE        (0 << 0)
#define UART_CTRL_RXMODE_INT            (1 << 0)
#define UART_CTRL_RXMODE_DMA_0_2        (2 << 0)    /* Only for UART0 */
#define UART_CTRL_RXMODE_DMA_1_3        (3 << 0)    /* Only for UART1 */
#define UART_CTRL_DEFAULT               (UART_CTRL_RXINT_TIMEOUT | UART_CTRL_TXMODE_INT | UART_CTRL_RXMODE_INT)
#define UART_CTRL_SET(ch, value)        {rUCON(ch) = value;}

/* Send break signal (UCON) */
#define UART_CTRL_BREAK_SEND(ch)        BIT_SET(rUCON(ch), 4)

/* FIFO configuration (UFCON) */
#define UART_FCON_TX_TRIGGER_LVL_0      (0 << 6)
#define UART_FCON_TX_TRIGGER_LVL_16     (1 << 6)
#define UART_FCON_TX_TRIGGER_LVL_32     (2 << 6)
#define UART_FCON_TX_TRIGGER_LVL_48     (3 << 6)
#define UART_FCON_TX_TRIGGER_LVL(_lvl)  (((_lvl) >> 4) << 6)
#define UART_FCON_RX_TRIGGER_LVL_1      (0 << 4)
#define UART_FCON_RX_TRIGGER_LVL_8      (1 << 4)
#define UART_FCON_RX_TRIGGER_LVL_16     (2 << 4)
#define UART_FCON_RX_TRIGGER_LVL_32     (3 << 4)
#define UART_FCON_FIFO_ENABLE           (1 << 0)
#define UART_FCON_DEFAULT               (UART_FCON_FIFO_ENABLE | UART_FCON_TX_TRIGGER_LVL(UART_TX_TRIGGER_LEVEL) | UART_FCON_RX_TRIGGER_LVL_32)
#define UART_FCON_SET(ch, value)        {rUFCON(ch) = value;}

/* FIFO reset (UFCON) */
#define UART_FIFO_RESET(ch)             REG_SET(rUFCON(ch), 2, 1, 3)
#define UART_FIFO_RESET_WAIT(ch)        {while (REG_CHECK(rUFCON(ch), 2, 1)) {}}

/* Auto Flow Control disable/enable */
#define UART_FLOWCTRL_ENABLE(ch)        {rUMCON(ch) = 1 << 4;}
#define UART_FLOWCTRL_DISABLE(ch)       {rUMCON(ch) = 1;}

/* Transmit/Receive status (in Non-FIFO mode) */
#define UART_TX_EMPTY(ch)               BIT_CHECK(rUTRSTAT(ch), 1)
#define UART_RX_FULL(ch)                BIT_CHECK(rUTRSTAT(ch), 0)

/* Receive overrun detection (auto-cleared on read) */
#define UART_OVERRUN_DETECTED(ch)       (rUERSTAT(ch) & 1)

/* Transmit/Receive status (in FIFO mode) */
#define UART_RX_FIFO_FULL(ch)           BIT_CHECK(rUFSTAT(ch), 6)
#define UART_TX_FIFO_FULL(ch)           BIT_CHECK(rUFSTAT(ch), 14)
#define UART_RX_FIFO_COUNT_GET(ch)      (rUFSTAT(ch) & 0x7F)
#define UART_TX_FIFO_COUNT_GET(ch)      ((rUFSTAT(ch) >> 8) & 0x7F)

/* Manual flow control */
#define UART_CTS_GET(ch)                BIT_CHECK(rUMSTAT(ch), 0)
#define UART_RTS_SET(ch)                BIT_SET(rUMCON(ch), 0)

/* Data transmission/reception */
#define UART_TX(ch, value)              {rUTXH(ch) = value;}
#define UART_RX(ch)                     (rURXH(ch))

#define INT_SUB_TXD(ch)                 (((ch) ? INT_SUB_TXD1 : INT_SUB_TXD0))
#define INT_SUB_RXD(ch)                 (((ch) ? INT_SUB_RXD1 : INT_SUB_RXD0))
#define INT_UART(ch)                    (((ch) ? INT_UART1 : INT_UART0))

/* Size of hardware FIFO */
#define UART_HW_FIFO_SIZE               64

/* Transmit Software Buffer */
static CsrUint32 txBufferSize[UART_CHANNEL_COUNT];
static CsrUint8 *txBuffer[UART_CHANNEL_COUNT];
static CsrUint32 txBufferCount[UART_CHANNEL_COUNT];
static CsrUint32 txIn[UART_CHANNEL_COUNT];
static CsrUint32 txOut[UART_CHANNEL_COUNT];

/* Receive Software Buffer */
static CsrUint32 rxBufferSize[UART_CHANNEL_COUNT];
static CsrUint8 *rxBuffer[UART_CHANNEL_COUNT];
static CsrUint32 rxBufferCount[UART_CHANNEL_COUNT];
static CsrUint32 rxIn[UART_CHANNEL_COUNT];
static CsrUint32 rxOut[UART_CHANNEL_COUNT];

/* Token Detector */
static CsrUint16 pToken[UART_CHANNEL_COUNT];
static UARTNotifyFunction pTokenDetect[UART_CHANNEL_COUNT];

/* Data Detector */
static UARTNotifyFunction pDataDetect[UART_CHANNEL_COUNT];
static CsrUint8 pDataDetectInhibit[UART_CHANNEL_COUNT];

static void eventUART(CsrInt32 channel)
{
    CsrUint32 pending;

    pending = INT_SUB_PENDING_GET();

    if (BIT_CHECK(pending, INT_SUB_TXD(channel)))
    {
        /* Check if data is available for transmission */
        if (txBufferCount[channel] > 0)
        {
            /* Transfer data from software buffer to hardware buffer */
            CsrUint32 hwFifoSpaceAvailable, hwFifoDataCount;

            /* Acknowledge Interrupt */
            INT_SUB_CLEAR(INT_SUB_TXD(channel));

            hwFifoSpaceAvailable = UART_HW_FIFO_SIZE - UART_TX_FIFO_COUNT_GET(channel);

            /* Write the first part directly to the hardware FIFO */
            while (TRUE)
            {
                while ((hwFifoSpaceAvailable > 0) && (txBufferCount[channel] > 0))
                {
                    UART_TX(channel, txBuffer[channel][txOut[channel]]);
                    txOut[channel] = (txOut[channel] + 1) % txBufferSize[channel];
                    txBufferCount[channel]--;
                    hwFifoSpaceAvailable--;
                }

                /* Check transmit buffer level */
                hwFifoDataCount = UART_TX_FIFO_COUNT_GET(channel);

                /* Break out if we're done or if mark has been exceeded */
                if ((txBufferCount[channel] == 0) || (hwFifoDataCount > UART_TX_TRIGGER_LEVEL))
                {
                    break;
                }

                /* More data to deliver, and mark has not been exceeded, so allow more data to go into the hardware FIFO directly */
                hwFifoSpaceAvailable += UART_HW_FIFO_SIZE - hwFifoDataCount;
            }
        }
        else
        {
            /* No more data in buffer - mask and acknowledge interrupt */
            INT_SUB_MASK(INT_SUB_TXD(channel));
            INT_SUB_CLEAR(INT_SUB_TXD(channel));
        }
    }

    if (BIT_CHECK(pending, INT_SUB_RXD(channel)))
    {
        CsrUint32 tokensDetected;
        CsrUint32 hwFifoDataCount;

        hwFifoDataCount = UART_RX_FIFO_COUNT_GET(channel);
        tokensDetected = 0;

        if (pTokenDetect[channel] != NULL)
        {
            while ((hwFifoDataCount > 0) && (rxBufferCount[channel] < rxBufferSize[channel]))
            {
                CsrUint8 byte;

                /* Get byte from HW FIFO */
                byte = UART_RX(channel);
                rxBuffer[channel][rxIn[channel]] = byte;

                /* Token detector */
                if (byte == pToken[channel])
                {
                    tokensDetected++;
                }

                rxIn[channel] = (rxIn[channel] + 1) % rxBufferSize[channel];
                hwFifoDataCount--;
                rxBufferCount[channel]++;
            }
        }
        else
        {
            while ((hwFifoDataCount > 0) && (rxBufferCount[channel] < rxBufferSize[channel]))
            {
                rxBuffer[channel][rxIn[channel]] = UART_RX(channel);
                rxIn[channel] = (rxIn[channel] + 1) % rxBufferSize[channel];
                hwFifoDataCount--;
                rxBufferCount[channel]++;
            }
        }

        if (rxBufferCount[channel] == rxBufferSize[channel])
        {
            /* Software buffer is full - mask interrupt */
            INT_SUB_MASK(INT_SUB_RXD(channel));
        }
        else
        {
            /* No more data in buffer */
            INT_SUB_CLEAR(INT_SUB_RXD(channel));
        }

        /* Token detect notification */
        while (tokensDetected--)
        {
            pTokenDetect[channel]();
        }

        /* Data detect notification */
        if (pDataDetect[channel] && !pDataDetectInhibit[channel])
        {
            pDataDetectInhibit[channel] = TRUE;
            pDataDetect[channel]();
        }
    }
}

static void eventUART0(INT unused)
{
    eventUART(0);
    INT_CLEAR(INT_UART(0));
}

static void eventUART1(INT unused)
{
    eventUART(1);
    INT_CLEAR(INT_UART(1));
}

void UARTInitialise(CsrUint8 channel, CsrUint16 baudRate, CsrUint8 configuration, CsrUint8 hardwareFlowControl, CsrUint32 receiveBufferSize, CsrUint8 *receiveBuffer, CsrUint32 transmitBufferSize, CsrUint8 *transmitBuffer)
{
    VOID (*previous)(INT);

    /* Deinit */
    UARTDeinitialise(channel);

    /* Restrict to channel 0 and 1 */
    channel = channel & 1;

    /* Register interrupt service routine */
    NU_Register_LISR(INT_UART(channel), (channel) ? eventUART1 : eventUART0,
        &previous);

    /* Reset buffer */
    txBuffer[channel] = transmitBuffer;
    rxBuffer[channel] = receiveBuffer;
    txBufferSize[channel] = transmitBufferSize;
    rxBufferSize[channel] = receiveBufferSize;
    txBufferCount[channel] = rxBufferCount[channel] = txIn[channel] = txOut[channel] = rxIn[channel] = rxOut[channel] = 0;

    /* Reset token detector */
    pTokenDetect[channel] = NULL;

    /* Reset data detector */
    pDataDetect[channel] = NULL;

    /* Configure */
    UART_BAUD_SET(channel, baudRate);
    UART_CON_SET(channel, configuration);
    UART_CTRL_SET(channel, UART_CTRL_DEFAULT);
    UART_FCON_SET(channel, UART_FCON_DEFAULT);
    if (hardwareFlowControl)
    {
        UART_FLOWCTRL_ENABLE(channel);
    }
    else
    {
        UART_FLOWCTRL_DISABLE(channel);
    }

    /* Enable Interrupts */
    INT_UNMASK_ATOMIC(INT_UART(channel));
    INT_SUB_UNMASK_ATOMIC(INT_SUB_RXD(channel));
}

void UARTDeinitialise(CsrUint8 channel)
{
    /* Restrict to channel 0 and 1 */
    channel = channel & 1;

    /* Disable/reset everything and mask and clear interrupts */
    INT_MASK_ATOMIC(INT_UART(channel));
    UART_CTRL_SET(channel, UART_CTRL_TXMODE_DISABLE | UART_CTRL_RXMODE_DISABLE);
    UART_FIFO_RESET(channel);
    UART_FIFO_RESET_WAIT(channel);
    INT_SUB_MASK_ATOMIC(INT_SUB_TXD(channel));
    INT_SUB_MASK_ATOMIC(INT_SUB_RXD(channel));
    INT_SUB_CLEAR(INT_SUB_TXD(channel));
    INT_SUB_CLEAR(INT_SUB_RXD(channel));
    INT_CLEAR(INT_UART(channel));
}

void UARTTokenDetectorRegister(CsrUint8 channel, CsrUint16 token, UARTNotifyFunction detect)
{
    UARTTokenDetectorUnregister(channel);

    if (token == UART_TOKEN_EOL)
    {
        token = '\n';
    }

    pToken[channel & 1] = token & 0xFF;
    pTokenDetect[channel & 1] = detect;
}

void UARTTokenDetectorUnregister(CsrUint8 channel)
{
    pTokenDetect[channel & 1] = NULL;
}

void UARTDataDetectorRegister(CsrUint8 channel, UARTNotifyFunction detect)
{
    UARTDataDetectorUnregister(channel);
    pDataDetect[channel & 1] = detect;
    pDataDetectInhibit[channel & 1] = FALSE;
}

void UARTDataDetectorUnregister(CsrUint8 channel)
{
    pDataDetect[channel & 1] = NULL;
}

CsrUint32 UARTReceiveDataCountGet(CsrUint8 channel)
{
    return rxBufferCount[channel & 1];
}

CsrUint32 UARTTransmitDataCountGet(CsrUint8 channel)
{
    return txBufferCount[channel & 1];
}

CsrUint32 UARTReceive(CsrUint8 channel, CsrUint32 length, CsrUint8 *data)
{
    CsrInt32 retval;

    /* Restrict to channel 0 and 1 */
    channel = channel & 1;

    retval = length;

    INT_SUB_MASK_ATOMIC(INT_SUB_RXD(channel));

    if (data != NULL)
    {
        /* Copy data to supplied buffer and update buffer */
        while ((length > 0) && (rxBufferCount[channel] > 0))
        {
            *(data++) = rxBuffer[channel][rxOut[channel]];
            rxOut[channel] = (rxOut[channel] + 1) % rxBufferSize[channel];
            rxBufferCount[channel]--;
            length--;
        }

        retval -= length;
    }
    else
    {
        /* Update buffer to consume specified amount of data */
        if (length < rxBufferCount[channel])
        {
            /* Consume specified amount */
            retval = length;
            rxBufferCount[channel] -= length;
            rxOut[channel] = (rxOut[channel] + length) % rxBufferSize[channel];
        }
        else
        {
            /* Consume everything */
            retval = rxBufferCount[channel];
            rxBufferCount[channel] = 0;
            rxOut[channel] = rxIn[channel];
        }
    }

    /* Allow notification when more data is received */
    pDataDetectInhibit[channel] = FALSE;

    INT_SUB_UNMASK_ATOMIC(INT_SUB_RXD(channel));

    return retval;
}

static CsrInt32 getLineLength(CsrUint8 channel)
{
    CsrInt32 i;
    CsrUint32 outPtr;
    CsrUint32 count;
    CsrUint8 byte, lastByte;

    /* Restrict to channel 0 and 1 */
    channel = channel & 1;

    outPtr = rxOut[channel];
    count = 0;
    lastByte = 0;

    for (i = 0; i < rxBufferCount[channel]; i++)
    {
        byte = rxBuffer[channel][outPtr];

        if (byte == '\n')
        {
            if (lastByte == '\r')
            {
                count--;
            }

            return count;
        }

        count++;
        lastByte = byte;
        outPtr = (outPtr + 1) % rxBufferSize[channel];
    }

    return UART_NO_FULL_LINE;
}

CsrInt32 UARTReceiveLine(CsrUint8 channel, CsrUint32 length, CsrUint8 *data)
{
    CsrUint8 byte;
    CsrInt32 lineLength;

    /* Determine line length */
    lineLength = getLineLength(channel);

    if (length && (lineLength >= 0) && (length > lineLength))
    {
        if (lineLength > 0)
        {
            UARTReceive(channel, lineLength, data);
        }
        if (data != NULL)
        {
            data[lineLength] = 0;
        }

        /* Consume the end-of-line token */
        UARTReceive(channel, 1, &byte);
        if (byte != '\n')
        {
            UARTReceive(channel, 1, NULL);
        }
    }

    return lineLength;
}

CsrUint32 UARTReceiveDataPointerGet(CsrUint8 channel, CsrUint8 **data)
{
    CsrUint32 retval;

    /* Restrict to channel 0 and 1 */
    channel = channel & 1;

    *data = &rxBuffer[channel][rxOut[channel]];
    INT_ATOMIC_EXECUTE
    (
        retval = ((rxIn[channel] <= rxOut[channel]) && (rxBufferCount[channel] > 0)) ?
                 rxBufferCount[channel] - rxIn[channel] : rxBufferCount[channel];
    );

    return retval;
}

CsrUint32 UARTTransmit(CsrUint8 channel, CsrUint32 length, const CsrUint8 *data)
{
    CsrInt32 i, c;
    CsrUint32 fifoSpaceAvailable;

    /* Restrict to channel 0 and 1 */
    channel = channel & 1;

    INT_SUB_MASK_ATOMIC(INT_SUB_TXD(channel));

    if (txBufferCount[channel] == 0)
    {
        /* Software buffer is empty */
        CsrUint32 hwFifoDataCount;

        fifoSpaceAvailable = UART_HW_FIFO_SIZE - UART_TX_FIFO_COUNT_GET(channel);

        /* Write the first part directly to the hardware FIFO */
        while (TRUE)
        {
            for (c = 0; (c < fifoSpaceAvailable) && (c < length); c++)
            {
                UART_TX(channel, data[c]);
            }

            /* Return if done */
            if (c == length)
            {
                return length;
            }

            /* Check transmit buffer level */
            hwFifoDataCount = UART_TX_FIFO_COUNT_GET(channel);

            /* Break out if mark has been exceeded (which means the rest can be safely queued in the software buffer) */
            if (hwFifoDataCount > UART_TX_TRIGGER_LEVEL)
            {
                break;
            }

            /* More data to deliver, and mark has not been exceeded, so allow more data to go into the hardware FIFO directly */
            fifoSpaceAvailable += UART_HW_FIFO_SIZE - hwFifoDataCount;
        }

        /* Enqueue the remainder in the (empty) software buffer */
        txBufferCount[channel] = txBufferSize[channel] >= (length - c) ? (length - c) : txBufferSize[channel];
        txOut[channel] = 0;
        txIn[channel] = txBufferCount[channel] % txBufferSize[channel];
        for (i = 0; i < txBufferCount[channel]; i++)
        {
            txBuffer[channel][i] = data[c++];
        }
    }
    else
    {
        /* Software buffer already contains data */

        /* Enqueue the data in the software buffer */
        fifoSpaceAvailable = txBufferSize[channel] - txBufferCount[channel];
        txBufferCount[channel] += fifoSpaceAvailable >= length ? length : fifoSpaceAvailable;
        for (c = 0; (c < fifoSpaceAvailable) && (c < length); c++)
        {
            txBuffer[channel][txIn[channel]++] = data[c];
            if (txIn[channel] == txBufferSize[channel])
            {
                txIn[channel] = 0;
            }
        }
    }

    /* Make sure that TXD interrupt is enabled (software buffer contains data) */
    INT_SUB_UNMASK_ATOMIC(INT_SUB_TXD(channel));

    return c;
}
