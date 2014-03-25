/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "nucleus.h"

#include "csr_types.h"
#include "csr_macro.h"
#include "csr_util.h"
#include "platform/csr_uart.h"
#include "platform/csr_interrupt.h"
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
#include "csr_ips.h"
#endif

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
#define UART_CTRL_CLKSOURCE_EXT_UCLK0   (1 << 10)
#define UART_CTRL_CLKSOURCE_EXT_UCLK1   (3 << 10)
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
#define UART_CTRL_SET(ch, value)        {rUCON(ch) = UART_CTRL_CLKSOURCE_EXT_UCLK1 | UART_CTRL_TXINT_TRIGGER_LEVEL | UART_CTRL_RXINT_TRIGGER_LEVEL | (value);}

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
#define UART_RX_FIFO_COUNT_GET(ch)      REG_CHECK(rUFSTAT(ch), 6, 0)
#define UART_TX_FIFO_COUNT_GET(ch)      REG_CHECK(rUFSTAT(ch), 14, 8)

/* Manual flow control */
#define UART_CTS_GET(ch)                BIT_CHECK(rUMSTAT(ch), 0)
#define UART_RTS_SET(ch)                BIT_SET(rUMCON(ch), 0)

/* Data transmission/reception */
#define UART_TX(ch, value)              {rUTXH(ch) = value;}
#define UART_RX(ch)                     (rURXH(ch))

/* Interrupt Handling */
#define INT_UART(ch)                            (INT_UART0 + ch)
#define UART_INT_MODEM                          3
#define UART_INT_TXD                            2
#define UART_INT_ERROR                          1
#define UART_INT_RXD                            0
#define UART_INT_CLEAR(ch, interrupt)           {rUINTP(ch) = (1 << (interrupt));}
#define UART_INT_CLEAR_ALL(ch)                  {rUINTP(ch) = 0xF;}
#define UART_INT_PENDING(ch, interrupt)         BIT_CHECK(rUINTP(ch), interrupt)
#define UART_INT_MASK(ch, interrupt)            BIT_SET(rUINTM(ch), interrupt)
#define UART_INT_MASK_ATOMIC(ch, interrupt)     INT_ATOMIC_EXECUTE(UART_INT_MASK(ch, interrupt))
#define UART_INT_UNMASK(ch, interrupt)          BIT_CLEAR(rUINTM(ch), interrupt)
#define UART_INT_UNMASK_ATOMIC(ch, interrupt)   INT_ATOMIC_EXECUTE(UART_INT_UNMASK(ch, interrupt))
#define UART_INT_MASK_ALL(ch)                   {rUINTM(ch) = 0xF;}

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

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
static CsrUint32 *uartMeasurements[UART_CHANNEL_COUNT];
#endif

static void eventUART(INT channel)
{
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrUint32 *previousMeasurements;
    previousMeasurements = CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, uartMeasurements[channel - INT_UART0]);
#endif

    channel -= INT_UART0;

    if (UART_INT_PENDING(channel, UART_INT_TXD))
    {
        /* Check if data is available for transmission */
        if (txBufferCount[channel] > 0)
        {
            CsrUint32 amount, contiguous, i;

            amount = CSRMIN(UART_HW_FIFO_SIZE - UART_TX_FIFO_COUNT_GET(channel), txBufferCount[channel]);
            contiguous = txIn[channel] <= txOut[channel] ?
                         txBufferCount[channel] - txIn[channel] : txBufferCount[channel];

            if (amount >= contiguous)
            {
                CsrUint8 *buffer = &txBuffer[channel][txOut[channel]];
                for (i = 0; i < contiguous; ++i)
                {
                    UART_TX(channel, buffer[i]);
                }
                txOut[channel] += contiguous;
                if (txOut[channel] == txBufferSize[channel])
                {
                    txOut[channel] = 0;
                }
                txBufferCount[channel] -= contiguous;
                amount -= contiguous;
            }
            if (amount > 0)
            {
                CsrUint8 *buffer = &txBuffer[channel][txOut[channel]];
                for (i = 0; i < amount; ++i)
                {
                    UART_TX(channel, buffer[i]);
                }
                txOut[channel] += amount;
                txBufferCount[channel] -= amount;
            }
        }

        /* Mask if we're done */
        if (txBufferCount[channel] == 0)
        {
            UART_INT_MASK(channel, UART_INT_TXD);
        }

        /* Acknowledge the TX interrupt */
        UART_INT_CLEAR(channel, UART_INT_TXD);
    }

    if (UART_INT_PENDING(channel, UART_INT_RXD))
    {
        CsrUint32 tokensDetected = 0;
        CsrUint32 space, amount, i;

        space = rxBufferSize[channel] - rxBufferCount[channel];
        amount = CSRMIN(space, UART_RX_FIFO_COUNT_GET(channel));

        if (amount > 0)
        {
            CsrUint32 contiguous = rxOut[channel] <= rxIn[channel] ?
                                   rxBufferSize[channel] - rxIn[channel] : space;

            if (amount >= contiguous)
            {
                CsrUint8 *buffer = &rxBuffer[channel][rxIn[channel]];
                if (pTokenDetect[channel] != NULL)
                {
                    for (i = 0; i < contiguous; ++i)
                    {
                        buffer[i] = UART_RX(channel);
                        if (buffer[i] == pToken[channel])
                        {
                            tokensDetected += 1;
                        }
                    }
                }
                else
                {
                    for (i = 0; i < contiguous; ++i)
                    {
                        buffer[i] = UART_RX(channel);
                    }
                }
                rxIn[channel] += contiguous;
                if (rxIn[channel] == rxBufferSize[channel])
                {
                    rxIn[channel] = 0;
                }
                rxBufferCount[channel] += contiguous;
                amount -= contiguous;
            }
            if (amount > 0)
            {
                CsrUint8 *buffer = &rxBuffer[channel][rxIn[channel]];
                if (pTokenDetect[channel] != NULL)
                {
                    for (i = 0; i < amount; ++i)
                    {
                        buffer[i] = UART_RX(channel);
                        if (buffer[i] == pToken[channel])
                        {
                            tokensDetected += 1;
                        }
                    }
                }
                else
                {
                    for (i = 0; i < amount; ++i)
                    {
                        buffer[i] = UART_RX(channel);
                    }
                }
                rxIn[channel] += amount;
                rxBufferCount[channel] += amount;
            }
        }

        if (rxBufferCount[channel] == rxBufferSize[channel])
        {
            /* Mask if no space for more in software buffer */
            UART_INT_MASK(channel, UART_INT_RXD);
        }

        /* Acknowledge the RX interrupt */
        UART_INT_CLEAR(channel, UART_INT_RXD);

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
    INT_CLEAR();
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, previousMeasurements);
#endif
}

void UARTInitialise(
    CsrUint8 channel,
    CsrUint16 baudRate,
    CsrUint8 configuration,
    CsrUint8 hardwareFlowControl,
    CsrUint32 receiveBufferSize,
    CsrUint8 *receiveBuffer,
    CsrUint32 transmitBufferSize,
    CsrUint8 *transmitBuffer)
{
    VOID (*previous)(INT);

    /* Deinit */
    UARTDeinitialise(channel);

    /* Register interrupt service routine */
    switch (channel)
    {
        case 0:
            NU_Register_LISR(INT_UART(channel), eventUART,
            &previous);
            break;
        case 1:
            NU_Register_LISR(INT_UART(channel), eventUART,
            &previous);
            break;
        case 2:
            NU_Register_LISR(INT_UART(channel), eventUART,
            &previous);
            break;
        default:
            break;
    }

    /* Reset buffer */
    txBuffer[channel] = transmitBuffer;
    rxBuffer[channel] = receiveBuffer;
    txBufferSize[channel] = transmitBufferSize;
    rxBufferSize[channel] = receiveBufferSize;
    txBufferCount[channel] = rxBufferCount[channel] = 0;
    txIn[channel] = txOut[channel] = rxIn[channel] = rxOut[channel] = 0;

    /* Reset token detector */
    pTokenDetect[channel] = NULL;

    /* Reset data detector */
    pDataDetect[channel] = NULL;

    /* Configure */
    UART_BAUD_SET(channel, baudRate);
    UART_CON_SET(channel, configuration);
    UART_CTRL_SET(channel, UART_CTRL_DEFAULT);
    UART_FCON_SET(channel, UART_FCON_DEFAULT);
    if (channel != 2) /* UART2 does not have hardware controlled RTS/CTS lines */
    {
        if (hardwareFlowControl)
        {
            UART_FLOWCTRL_ENABLE(channel);
        }
        else
        {
            UART_FLOWCTRL_DISABLE(channel);
        }
    }

    /* Enable Interrupts */
    UART_INT_MASK_ALL(channel);
    UART_INT_UNMASK(channel, UART_INT_RXD);
    INT_UNMASK_ATOMIC(INT_UART(channel));
}

void UARTDeinitialise(CsrUint8 channel)
{
    /* Disable/reset everything and mask and clear interrupts */
    INT_MASK_ATOMIC(INT_UART(channel));
    UART_INT_MASK_ALL(channel);
    UART_CTRL_SET(channel, UART_CTRL_TXMODE_DISABLE | UART_CTRL_RXMODE_DISABLE);
    UART_FIFO_RESET(channel);
    UART_FIFO_RESET_WAIT(channel);
    UART_INT_CLEAR_ALL(channel);
}

void UARTTokenDetectorRegister(CsrUint8 channel, CsrUint16 token, UARTNotifyFunction detect)
{
    UARTTokenDetectorUnregister(channel);

    if (token == UART_TOKEN_EOL)
    {
        token = '\n';
    }

    pToken[channel] = token & 0xFF;
    pTokenDetect[channel] = detect;
}

void UARTTokenDetectorUnregister(CsrUint8 channel)
{
    pTokenDetect[channel] = NULL;
}

void UARTDataDetectorRegister(CsrUint8 channel, UARTNotifyFunction detect)
{
    UARTDataDetectorUnregister(channel);
    pDataDetect[channel] = detect;
    pDataDetectInhibit[channel] = FALSE;
}

void UARTDataDetectorUnregister(CsrUint8 channel)
{
    pDataDetect[channel] = NULL;
}

CsrUint32 UARTReceiveDataCountGet(CsrUint8 channel)
{
    return rxBufferCount[channel];
}

CsrUint32 UARTTransmitDataCountGet(CsrUint8 channel)
{
    return txBufferCount[channel];
}

CsrUint32 UARTReceive(CsrUint8 channel, CsrUint32 length, CsrUint8 *data)
{
    CsrUint32 retval;
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrUint32 *previousMeasurements;
    previousMeasurements = CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, uartMeasurements[channel]);
#endif

    UART_INT_MASK_ATOMIC(channel, UART_INT_RXD);
    UART_INT_CLEAR(channel, UART_INT_RXD);

    if (data != NULL)
    {
        CsrUint32 amount = CSRMIN(length, rxBufferCount[channel]);

        retval = amount;

        if (amount > 0)
        {
            CsrUint32 contiguous = rxIn[channel] <= rxOut[channel] ?
                                   rxBufferCount[channel] - rxIn[channel] : rxBufferCount[channel];

            if (amount >= contiguous)
            {
                CsrMemCpy(data, &rxBuffer[channel][rxOut[channel]], contiguous);
                rxOut[channel] += contiguous;
                if (rxOut[channel] == rxBufferSize[channel])
                {
                    rxOut[channel] = 0;
                }
                rxBufferCount[channel] -= contiguous;
                amount -= contiguous;
                data += contiguous;
            }
            if (amount > 0)
            {
                CsrMemCpy(data, &rxBuffer[channel][rxOut[channel]], amount);
                rxOut[channel] += amount;
                rxBufferCount[channel] -= amount;
            }
        }
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
            rxOut[channel] = rxIn[channel] = 0;
        }
    }

    /* Allow notification when more data is received */
    pDataDetectInhibit[channel] = FALSE;

    if (rxBufferCount[channel] < rxBufferSize[channel])
    {
        /* Unmask if free space in software buffer */
        UART_INT_UNMASK_ATOMIC(channel, UART_INT_RXD);
    }

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, previousMeasurements);
#endif
    return retval;
}

static CsrInt32 getLineLength(CsrUint8 channel)
{
    CsrInt32 i;
    CsrUint32 outPtr;
    CsrUint32 count;
    CsrUint8 byte, lastByte;

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
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrUint32 *previousMeasurements;
    previousMeasurements = CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, uartMeasurements[channel]);
#endif

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

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, previousMeasurements);
#endif
    return lineLength;
}

CsrUint32 UARTReceiveDataPointerGet(CsrUint8 channel, CsrUint8 **data)
{
    CsrUint32 retval;

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
    CsrUint32 c = 0;
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrUint32 *previousMeasurements;
    previousMeasurements = CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, uartMeasurements[channel]);
#endif
    UART_INT_MASK_ATOMIC(channel, UART_INT_TXD);
    UART_INT_CLEAR(channel, UART_INT_TXD);

    if (txBufferCount[channel] == 0)
    {
        /* Write the first part directly to the hardware FIFO */
        CsrUint32 amount = CSRMIN(length, UART_HW_FIFO_SIZE - UART_TX_FIFO_COUNT_GET(channel));
        for (c = 0; c < amount; ++c)
        {
            UART_TX(channel, data[c]);
        }

        /* Return if done (TX interrupt is masked) */
        if (c == length)
        {
#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
            CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, previousMeasurements);
#endif
            return length;
        }

        /* Enqueue the remainder in the (empty) software buffer */
        txBufferCount[channel] = CSRMIN(txBufferSize[channel], length - c);
        txOut[channel] = 0;
        txIn[channel] = ((txBufferCount[channel] == txBufferSize[channel]) ? 0 : txBufferCount[channel]);
        CsrMemCpy(txBuffer[channel], data + c, txBufferCount[channel]);
        c += txBufferCount[channel];
    }
    else
    {
        /* Software buffer already contains data - enqueue the data in the software buffer */
        CsrUint32 space = txBufferSize[channel] - txBufferCount[channel];
        CsrUint32 amount = CSRMIN(space, length);

        if (amount > 0)
        {
            CsrUint32 contiguous = txOut[channel] < txIn[channel] ? txBufferSize[channel] - txIn[channel] : space;

            c = amount;

            if (amount >= contiguous)
            {
                CsrMemCpy(&txBuffer[channel][txIn[channel]], data, contiguous);
                txIn[channel] += contiguous;
                if (txIn[channel] == txBufferSize[channel])
                {
                    txIn[channel] = 0;
                }
                txBufferCount[channel] += contiguous;
                amount -= contiguous;
                data += contiguous;
            }
            if (amount > 0)
            {
                CsrMemCpy(&txBuffer[channel][txIn[channel]], data, amount);
                txIn[channel] += amount;
                txBufferCount[channel] += amount;
            }
        }
    }

    /* Make sure that TXD interrupt is enabled (software buffer contains data) */
    UART_INT_UNMASK_ATOMIC(channel, UART_INT_TXD);

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
    CsrIpsInsert(CSR_IPS_CONTEXT_CURRENT, previousMeasurements);
#endif
    return c;
}

#ifdef CSR_INSTRUMENTED_PROFILING_SERVICE
CsrUint32 *UARTIpsInsert(CsrUint8 channel, CsrUint32 *measurements)
{
    CsrUint32 *ret;

    INT_ATOMIC_EXECUTE
    (
        ret = uartMeasurements[channel];
        uartMeasurements[channel] = measurements;
    );

    return ret;
}
#endif
