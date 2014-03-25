/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <string.h>
#include <stdio.h>

#include "nucleus.h"
#include "csr_types.h"
#include "csr_sched.h"
#include "csr_panic.h"
#include "csr_serial_com.h"
#include "csr_transport.h"
#include "csr_util.h"

#include "platform/csr_uart.h"
#include "platform/csr_i2c.h"
#include "platform/csr_serial_init.h"

#define BAUD_TO_DIVIDER(baud) ((6250000 + (baud / 2)) / baud - 1)
#define DIVIDER_TO_BAUD(divider) ((6250000 + (divider + 1) / 2) / (divider + 1))
#define BAUD_ROUND(baud) DIVIDER_TO_BAUD(BAUD_TO_DIVIDER(baud))

/* UART software buffer sizes */
#define UART_RX_BUFFER_SIZE (20 * 1024)
static CsrUint8 rxBuffer[UART_CHANNEL_COUNT][UART_RX_BUFFER_SIZE];
#define UART_TX_BUFFER_SIZE (20 * 1024)
static CsrUint8 txBuffer[UART_CHANNEL_COUNT][UART_TX_BUFFER_SIZE];

static CsrBool instInUse[UART_CHANNEL_COUNT] = {FALSE, FALSE};
static CsrUint8 instIdx[UART_CHANNEL_COUNT];
static CsrUint8 instChannel[UART_CHANNEL_COUNT];
static CsrUint32 curBaud[UART_CHANNEL_COUNT];
static CsrUint32 instBaud[UART_CHANNEL_COUNT];
static CsrUint32 instResetBaud[UART_CHANNEL_COUNT];
static CsrUartDrvDataBits instDataBits[UART_CHANNEL_COUNT];
static CsrUartDrvParity instParity[UART_CHANNEL_COUNT];
static CsrUartDrvStopBits instStopBits[UART_CHANNEL_COUNT];
static CsrBool instFlowControl[UART_CHANNEL_COUNT];
static CsrUint8 instToken[UART_CHANNEL_COUNT];
static CsrBool instUseToken[UART_CHANNEL_COUNT];

/* Channel for BlueCore communication */
static CsrSchedBgint rxBgint[UART_CHANNEL_COUNT] = {CSR_SCHED_BGINT_INVALID, CSR_SCHED_BGINT_INVALID};
static CsrUartDrvDataRx rxData[UART_CHANNEL_COUNT] = {NULL, NULL};

typedef void (*csrUartBgIntWrapper)(void);

#define BG_INT_HISR_STACK_SIZE 4096

static NU_HISR bgIntHisr[UART_CHANNEL_COUNT];
static CsrUint8 bgIntHisrStack[UART_CHANNEL_COUNT][BG_INT_HISR_STACK_SIZE];
static CsrBool bgIntHisrCreated[UART_CHANNEL_COUNT] = {FALSE, FALSE};

static void bgIntHisrActivate1(void)
{
    NU_Activate_HISR(&bgIntHisr[0]);
}

static void bgIntHisrActivate2(void)
{
    NU_Activate_HISR(&bgIntHisr[1]);
}

csrUartBgIntWrapper bgIntHisrActivate[UART_CHANNEL_COUNT] = {bgIntHisrActivate1, bgIntHisrActivate2};

static void csrUartBgIntWrapper1(void)
{
    CsrSchedBgintSet(rxBgint[0]);
}

static void csrUartBgIntWrapper2(void)
{
    CsrSchedBgintSet(rxBgint[1]);
}

csrUartBgIntWrapper bgIntWrapper[UART_CHANNEL_COUNT] = {csrUartBgIntWrapper1, csrUartBgIntWrapper2};

CsrBool CsrUartDrvStart(void *handle, CsrUint8 reset)
{
    /* Switch to reset baudrate when we're done with bootstrap */
    CsrUint8 idx = *((CsrUint8 *) handle);
    CsrUint32 baud = (reset == 0) ? instBaud[idx] : instResetBaud[idx];

    curBaud[idx] = baud;

    if (!bgIntHisrCreated[idx])
    {
        NU_Create_HISR(&bgIntHisr[idx], "BG1_HISR", bgIntWrapper[idx], 1, bgIntHisrStack[idx], BG_INT_HISR_STACK_SIZE);
        bgIntHisrCreated[idx] = TRUE;
    }

    UARTInitialise(instChannel[idx],
        BAUD_TO_DIVIDER(baud),
        instDataBits[idx] | instParity[idx] | instStopBits[idx],
        instFlowControl[idx],
        UART_RX_BUFFER_SIZE,
        rxBuffer[idx],
        UART_TX_BUFFER_SIZE,
        txBuffer[idx]);

    if (instUseToken[idx])
    {
        UARTTokenDetectorRegister(instChannel[idx], instToken[idx], bgIntHisrActivate[idx]);
    }
    else
    {
        UARTDataDetectorRegister(instChannel[idx], bgIntHisrActivate[idx]);
    }

    return TRUE;
}

CsrBool CsrUartDrvStop(void *handle)
{
    CsrUint8 idx = *((CsrUint8 *) handle);

    curBaud[idx] = 0;

    UARTDeinitialise(instChannel[idx]);

    if (bgIntHisrCreated[idx])
    {
        NU_Delete_HISR(&bgIntHisr[idx]);
        bgIntHisrCreated[idx] = FALSE;
    }

    return TRUE;
}

CsrBool CsrUartDrvTx(void *handle, const CsrUint8 *data, CsrUint32 dataLength, CsrUint32 *numSent)
{
    CsrBool retVal = FALSE;
    CsrUint8 idx = *((CsrUint8 *) handle);

    *numSent = UARTTransmit(instChannel[idx], dataLength, data);
    if (*numSent == dataLength)
    {
        retVal = TRUE;
    }
    return retVal;
}

CsrUint32 CsrUartDrvGetTxSpace(void *handle)
{
    CsrUint8 idx = *((CsrUint8 *) handle);
    return UART_TX_BUFFER_SIZE - UARTTransmitDataCountGet(instChannel[idx]);
}

CsrUint32 CsrUartDrvGetBaudrate(void *handle)
{
    CsrUint8 idx = *((CsrUint8 *) handle);
    return curBaud[idx];
}

CsrUint32 CsrUartDrvGetRxAvailable(void *handle)
{
    CsrUint8 idx = *((CsrUint8 *) handle);
    return UARTReceiveDataCountGet(instChannel[idx]);
}

void CsrUartDrvRx(void *handle)
{
    CsrUint8 *bufPtr;
    CsrUint32 bytesConsumed = 0;
    CsrUint32 bytesToRead;
    CsrUint32 bytesInBuf;
    CsrUint8 idx = *((CsrUint8 *) handle);

    /* Note: order is important - get this value first */
    bytesInBuf = UARTReceiveDataCountGet(instChannel[idx]);
    bytesToRead = UARTReceiveDataPointerGet(instChannel[idx], &bufPtr);

    /* truncate */
    if (bytesToRead)
    {
        bytesConsumed = rxData[idx](bufPtr, bytesToRead);
        if (bytesConsumed > 0)
        {
            UARTReceive(instChannel[idx], bytesConsumed, NULL);
        }
    }

    if (bytesInBuf > bytesConsumed)
    {
        CsrSchedBgintSet(rxBgint[idx]);
    }
}

CsrUint32 CsrUartDrvLowLevelTransportTxBufLevel(void *handle)
{
    CsrUint8 idx = *((CsrUint8 *) handle);
    return UARTTransmitDataCountGet(instChannel[idx]);
}

CsrUint32 CsrUartDrvLowLevelTransportRx(void *handle, CsrUint32 dataLength, CsrUint8 *data)
{
    CsrUint8 *bufPtr;
    CsrUint32 bytesToRead;
    CsrUint32 bytesInBuf;
    CsrUint8 idx = *((CsrUint8 *) handle);

    /* Note: order is important - get this value first */
    bytesInBuf = UARTReceiveDataCountGet(instChannel[idx]);
    bytesToRead = UARTReceiveDataPointerGet(instChannel[idx], &bufPtr);

    /* truncate */
    if (bytesToRead > dataLength)
    {
        bytesToRead = dataLength;
    }
    if (bytesToRead)
    {
        memcpy(data, bufPtr, bytesToRead);
        UARTReceive(instChannel[idx], bytesToRead, NULL);
    }

    /* If buffer is not empty initialize a new bgint */
    if (bytesInBuf > bytesToRead)
    {
        CsrSchedBgintSet(rxBgint[idx]);
    }
    return bytesToRead;
}

void CsrUartDrvRegister(void *handle, CsrUartDrvDataRx rxDataFn, CsrSchedBgint rxBgintHdl)
{
    CsrUint8 idx = *((CsrUint8 *) handle);
    rxData[idx] = rxDataFn;
    rxBgint[idx] = rxBgintHdl;
}

#define UART_DEVICE_NAME_LENGTH 5

void *CsrUartDrvInitialise(const CsrCharString *device,
    CsrUint32 baud, CsrUint32 resetBaud,
    CsrUartDrvDataBits dataBits,
    CsrUartDrvParity parity,
    CsrUartDrvStopBits stopBits,
    CsrBool flowControl,
    const CsrUint8 *token)
{
    CsrUint8 idx;
    for (idx = 0; idx < UART_CHANNEL_COUNT; idx++)
    {
        if (!instInUse[idx])
        {
            instInUse[idx] = TRUE;
            instIdx[idx] = idx;

            curBaud[idx] = 0;
            instBaud[idx] = baud;
            instResetBaud[idx] = resetBaud;
            instDataBits[idx] = dataBits;
            instParity[idx] = parity;
            instStopBits[idx] = stopBits;
            instFlowControl[idx] = flowControl;
            if (token != NULL)
            {
                instUseToken[idx] = TRUE;
                instToken[idx] = *token;
            }

            if (CsrStrNCmp(device, "uart0", UART_DEVICE_NAME_LENGTH) == 0)
            {
                instChannel[idx] = 0;
            }
            else if (CsrStrNCmp(device, "uart1", UART_DEVICE_NAME_LENGTH) == 0)
            {
                instChannel[idx] = 1;
            }
            else
            {
                instInUse[idx] = FALSE;
                return NULL;
            }

            if (instChannel[idx] < 2)
            {
                if (CsrStrCmp(device + UART_DEVICE_NAME_LENGTH, ":csr1") == 0)
                {
                    /* Route UART to CSR1 on Radio Module */
                    CsrUint8 byte = 0x02;
                    I2CWrite(0, 0x20 + instChannel[idx], 1, &byte);
                }
                else if (CsrStrCmp(device + UART_DEVICE_NAME_LENGTH, ":csr2") == 0)
                {
                    /* Route UART to CSR2 on Radio Module */
                    CsrUint8 byte = 0x04;
                    I2CWrite(0, 0x20 + instChannel[idx], 1, &byte);
                }
                else if (CsrStrCmp(device + UART_DEVICE_NAME_LENGTH, ":ext1") == 0)
                {
                    /* Route UART to EXT1 on Breakout Board */
                    CsrUint8 byte = 0x08;
                    I2CWrite(0, 0x20 + instChannel[idx], 1, &byte);
                }
                else if (CsrStrCmp(device + UART_DEVICE_NAME_LENGTH, ":ext2") == 0)
                {
                    /* Route UART to EXT2 on Breakout Board */
                    CsrUint8 byte = 0x10;
                    I2CWrite(0, 0x20 + instChannel[idx], 1, &byte);
                }
                else if (CsrStrCmp(device + UART_DEVICE_NAME_LENGTH, ":debug") == 0)
                {
                    /* Route UART to DEBUG on Breakout Board */
                    CsrUint8 byte = 0x80 | (1 << (5 + instChannel[idx]));
                    I2CWrite(0, 0x22, 1, &byte);
                }
            }

            return &instIdx[idx];
        }
    }
    return NULL;
}

void CsrUartDrvDeinitialise(void *handle)
{
    CsrUint8 idx = *((CsrUint8 *) handle);
    instInUse[idx] = FALSE;
}

CsrUint32 CsrUartDrvBaudrateActualGet(CsrUint32 wantedBaudrate)
{
    if (wantedBaudrate < 1200)
    {
        wantedBaudrate = 1200;
    }
    else if (wantedBaudrate > 3072000)
    {
        wantedBaudrate = 3072000;
    }
    return DIVIDER_TO_BAUD(BAUD_TO_DIVIDER(wantedBaudrate));
}
