/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "nucleus.h"

#include "csr_types.h"
#include "csr_result.h"
#include "csr_spi_master.h"
#include "csr_util.h"
#include "csr_sched.h"
#include "csr_log_text_2.h"

#include "platform/csr_dma.h"
#include "platform/csr_gpio.h"
#include "platform/csr_i2c.h"
#include "platform/csr_interrupt.h"


/******************************/
/********* Parameters *********/
/******************************/

#define DMA_CHANNEL_RX 0
#define DMA_CHANNEL_TX 1
#define DEVICE_COUNT 2
#define INITIAL_FREQUENCY  400000
#define MAX_FREQUENCY 25000000
#define POLL_TRANSFER_LIMIT 16


/**************************************/
/***** SPI Configuration Register *****/
/**************************************/

#define SPIM_CH_CFG_HIGH_SPEED_ENABLE       (1 << 6)    /* Enable High Speed mode */
#define SPIM_CH_CFG_HIGH_SPEED_DISABLE      (0 << 6)    /* Disable High Speed mode */
#define SPIM_CH_CFG_SW_RST_SET              (1 << 5)    /* Set Software Reset*/
#define SPIM_CH_CFG_SW_RST_CLEAR            (0 << 5)    /* Clear Software Reset */
#define SPIM_CH_CFG_SLAVE                   (1 << 4)    /* Slave mode on SPI Channel */
#define SPIM_CH_CFG_MASTER                  (0 << 4)    /* Master mode on SPI Channel */
#define SPIM_CH_CFG_CLOCK_ACTIVE_LOW        (1 << 3)    /* Active low clock  */
#define SPIM_CH_CFG_CLOCK_ACTIVE_HIGH       (0 << 3)    /* Active high clock */
#define SPIM_CH_CFG_FORMAT_B                (1 << 2)    /* Format B */
#define SPIM_CH_CFG_FORMAT_A                (0 << 2)    /* Format A */
#define SPIM_CH_CFG_RX_ON                   (1 << 1)    /* SPI RX Channel On */
#define SPIM_CH_CFG_RX_OFF                  (0 << 1)    /* SPI RX Channel Off */
#define SPIM_CH_CFG_TX_ON                   (1 << 0)    /* SPI TX Channel On */
#define SPIM_CH_CFG_TX_OFF                  (0 << 0)    /* SPI TX Channel Off */

#define SPIM_CH_CFG_DEFAULT                 (SPIM_CH_CFG_HIGH_SPEED_DISABLE |   \
                                             SPIM_CH_CFG_SW_RST_CLEAR |         \
                                             SPIM_CH_CFG_MASTER |               \
                                             SPIM_CH_CFG_CLOCK_ACTIVE_LOW |     \
                                             SPIM_CH_CFG_FORMAT_B |             \
                                             SPIM_CH_CFG_RX_OFF |               \
                                             SPIM_CH_CFG_TX_OFF)

#define SPIM_CH_CFG_SET(tx, rx)             {rCHCFG(0) = (SPIM_CH_CFG_DEFAULT | \
                                                          (tx) | \
                                                          (rx));}

#define SPIM_CH_CFG_RESET_SET()            {rCHCFG(0) |= SPIM_CH_CFG_SW_RST_SET;}

#define SPIM_CH_CFG_RESET_CLEAR()          {rCHCFG(0) &= ~SPIM_CH_CFG_SW_RST_SET;}


/**************************************/
/**** Clock Configuration Register ****/
/**************************************/

#define SPIM_CLK_CFG_SPICLKSEL_PCLK             (0x0 << 9)    /* PCLK */
#define SPIM_CLK_CFG_SPICLKSEL_USBCLK           (0x1 << 9)    /* USBCLK */
#define SPIM_CLK_CFG_SPICLKSEL_EPLLCLK          (0x2 << 9)    /* EPLL clock */
#define SPIM_CLK_CFG_SPICLKSEL_RESERVED         (0x3 << 9)    /* RESERVED */
#define SPIM_CLK_CFG_ENCLK_ENABLE               (1 << 8)      /* Enable Clock */
#define SPIM_CLK_CFG_ENCLK_DISABLE              (0 << 8)      /* Disable Clock */

#define SPIM_CLK_CFG_DEFAULT                    (SPIM_CLK_CFG_SPICLKSEL_EPLLCLK)

#define SPIM_CLK_CFG_SET(clk, prescaler)    {rCLKCFG(0) = (SPIM_CLK_CFG_DEFAULT | \
                                                           ((CsrUint32) (prescaler)) | \
                                                           (clk));}

#define SPIM_CLK_ENABLE()                   {rCLKCFG(0) |= SPIM_CLK_CFG_ENCLK_ENABLE;}
#define SPIM_CLK_DISABLE()                  {rCLKCFG(0) &= ~SPIM_CLK_CFG_ENCLK_ENABLE;}


/**************************************/
/***** FIFO Configuration Register ****/
/**************************************/

#define SPIM_MODE_CFG_CH_WIDTH_BYTE             (0x0 << 29) /* Byte */
#define SPIM_MODE_CFG_CH_WIDTH_HALFWORD         (0x1 << 29) /* Half word */
#define SPIM_MODE_CFG_CH_WIDTH_WORD             (0x2 << 29) /* Word */
#define SPIM_MODE_CFG_CH_WIDTH_RESERVED         (0x3 << 29) /* RESERVED */
#define SPIM_MODE_CFG_TRAILING_CNT              (0 << 19)   /* Trailing Count */
#define SPIM_MODE_CFG_BUS_WIDTH_BYTE            (0x0 << 17) /* Byte */
#define SPIM_MODE_CFG_BUS_WIDTH_HALFWORD        (0x1 << 17) /* Half word */
#define SPIM_MODE_CFG_BUS_WIDTH_WORD            (0x2 << 17) /* Word */
#define SPIM_MODE_CFG_BUS_WIDTH_RESERVED        (0x3 << 17) /* RESERVED */
#define SPIM_MODE_CFG_RX_READY_LVL              (0 << 11)   /* RX Ready Level */
#define SPIM_MODE_CFG_TX_READY_LVL              (0 << 5)    /* TX Ready Level */
#define SPIM_MODE_CFG_RX_DMA_SW_ENABLE          (1 << 2)    /* Enable RX DMA */
#define SPIM_MODE_CFG_RX_DMA_SW_DISABLE         (0 << 2)    /* Disable RX DMA */
#define SPIM_MODE_CFG_TX_DMA_SW_ENABLE          (1 << 1)    /* Enable TX DMA */
#define SPIM_MODE_CFG_TX_DMA_SW_DISABLE         (0 << 1)    /* Disable TX DMA */
#define SPIM_MODE_CFG_DMA_TYPE_BURST            (1 << 0)    /* Single DMA transfer */
#define SPIM_MODE_CFG_DMA_TYPE_SINGLE           (0 << 0)    /* Burst DMA transfer */

#define SPIM_MODE_CFG_DEFAULT                   (SPIM_MODE_CFG_CH_WIDTH_BYTE | \
                                                 SPIM_MODE_CFG_TRAILING_CNT | \
                                                 SPIM_MODE_CFG_RX_READY_LVL | \
                                                 SPIM_MODE_CFG_TX_READY_LVL | \
                                                 SPIM_MODE_CFG_DMA_TYPE_SINGLE)

#define SPIM_MODE_CFG_SET(buswidth, rx, tx)     {rMODECFG(0) = (SPIM_MODE_CFG_DEFAULT | \
                                                                (buswidth) | \
                                                                (rx) | \
                                                                (tx));}


/**************************************/
/******* Slave Selection Register *****/
/**************************************/

#define SPIM_CS_REG_NCS_TIME_COUNT              (0 << 4)  /* nCS Time Count */
#define SPIM_CS_REG_SS_AUTO                     (1 << 1)  /* Auto chip select */
#define SPIM_CS_REG_SS_MANUAL                   (0 << 1)  /* Manual chip select */
#define SPIM_CS_REG_NSSOUT_INACTIVE             (1 << 0)  /* Assert chip select */
#define SPIM_CS_REG_NSSOUT_ACTIVE               (0 << 0)  /* Negate chip select */

#define SPIM_CS_REG_SET(ssmode)                 {rCSREG(0) = (SPIM_CS_REG_NCS_TIME_COUNT | \
                                                              (ssmode) | \
                                                              SPIM_CS_REG_NSSOUT_INACTIVE);}

#define SPIM_CS_REG_NSSOUT_ASSERT()             {rCSREG(0) = (SPIM_CS_REG_NCS_TIME_COUNT | \
                                                              SPIM_CS_REG_SS_MANUAL | \
                                                              SPIM_CS_REG_NSSOUT_ACTIVE);}

#define SPIM_CS_REG_NSSOUT_NEGATE()             {rCSREG(0) = (SPIM_CS_REG_NCS_TIME_COUNT | \
                                                              SPIM_CS_REG_SS_MANUAL | \
                                                              SPIM_CS_REG_NSSOUT_INACTIVE);}


/**************************************/
/******* SPI Status Register **********/
/**************************************/

#define SPIM_SPI_STATUS_TX_DONE_WAIT()          {while (!BIT_CHECK(rSPISTATUS(0), 21)) {}}
#define SPIM_SPI_STATUS_RX_FIFO_WAIT()          {while (!REG_CHECK(rSPISTATUS(0), 19, 13)) {}}
#define SPIM_SPI_STATUS_RX_FIFO_EMPTY_WAIT()    {while (REG_CHECK(rSPISTATUS(0), 19, 13)) {}}
#define SPIM_SPI_STATUS_RX_FIFO_LEVEL()         REG_CHECK(rSPISTATUS(0), 19, 13)

#define SPIM_SPI_STATUS_TX_FIFO_EMPTY_WAIT()    {while (REG_CHECK(rSPISTATUS(0), 12, 6)) {}}
#define SPIM_SPI_STATUS_TX_FIFO_LEVEL()         REG_CHECK(rSPISTATUS(0), 12, 6)

#define SPIM_SPI_STATUS_OVERRUN_WAIT()          {while (REG_CHECK(rSPISTATUS(0), 5, 2)) {}}


/*********************************/
/********* Data Transfer *********/
/*********************************/

#define SPIM_TRANSMIT(value)                    {rSPITXDATA8(0) = (value);}
#define SPIM_RECEIVE()                          (rSPIRXDATA8(0))


/*******************************/
/********* Static Data *********/
/*******************************/

static CsrUint8 txGarbageBuffer[16 * 1024];

#define SPIM_STATE_STOPPED              0
#define SPIM_STATE_IDLE                 1
#define SPIM_STATE_BURST_OUTPUT         2
#define SPIM_STATE_BURST_INPUT_1        3
#define SPIM_STATE_BURST_INPUT_2        4

static CsrSpiMasterDevice spimDevices[DEVICE_COUNT];
static CsrSpiMasterDevice *operationDevice;

static CsrUint32 state = SPIM_STATE_STOPPED;

static CsrSpiMasterCallback eventOperationCallback = NULL;
static CsrSpiMasterDsrCallback eventOperationDsrCallback[DEVICE_COUNT] = {NULL, NULL};
static CsrSpiMasterCallback eventInterruptCallback[DEVICE_COUNT] = {NULL, NULL};
static CsrSpiMasterDsrCallback eventInterruptDsrCallback[DEVICE_COUNT] = {NULL, NULL};

/* High Level Callback HISR's */
#define OPERATION_HISR_STACK_SIZE 1024
#define INTERRUPT_HISR_STACK_SIZE 1024
static NU_HISR operationHisr[DEVICE_COUNT];
static NU_HISR interruptHisr[DEVICE_COUNT];
static CsrUint8 operationHisrStack[DEVICE_COUNT][OPERATION_HISR_STACK_SIZE];
static CsrUint8 interruptHisrStack[DEVICE_COUNT][INTERRUPT_HISR_STACK_SIZE];

/* Blocking mechanism */
static NU_EVENT_GROUP eventGroup;

/* DMA */
static DMAHandle dmaHandleRx;
static DMAHandle dmaHandleTx;

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrSpiMasterLto);


/**********************************/
/********* Implementation *********/
/**********************************/

static void executeOperationDsrCallback0(void)
{
    if (eventOperationCallback == NULL)
    {
        NU_Set_Events(&eventGroup, 1, NU_OR);
    }
    else if (eventOperationDsrCallback[0] != NULL)
    {
        eventOperationDsrCallback[0](&spimDevices[0]);
    }
}

static void executeOperationDsrCallback1(void)
{
    if (eventOperationCallback == NULL)
    {
        NU_Set_Events(&eventGroup, 1, NU_OR);
    }
    else if (eventOperationDsrCallback[1] != NULL)
    {
        eventOperationDsrCallback[1](&spimDevices[1]);
    }
}

static void eventDMA(DMAHandle *handle, CsrResult result)
{
    CSR_UNUSED(handle);

    switch (state)
    {
        case SPIM_STATE_BURST_INPUT_1:
        {
            state = SPIM_STATE_BURST_INPUT_2;
            break;
        }
        case SPIM_STATE_BURST_INPUT_2:
        {
            /* Turn off TX/RX channel */
            SPIM_CS_REG_NSSOUT_NEGATE();
            SPIM_CH_CFG_SET(SPIM_CH_CFG_TX_OFF, SPIM_CH_CFG_RX_OFF);

            state = SPIM_STATE_IDLE;

            if (((eventOperationCallback != NULL) && eventOperationCallback(operationDevice)) || (eventOperationCallback == NULL))
            {
                NU_Activate_HISR(&operationHisr[operationDevice->deviceIndex]);
            }
            break;
        }
        case SPIM_STATE_BURST_OUTPUT:
        {
            /* Wait for last byte to be fully clocked */
            SPIM_SPI_STATUS_TX_FIFO_EMPTY_WAIT();
            SPIM_SPI_STATUS_TX_DONE_WAIT();
            /* Turn off TX/RX channel */
            SPIM_CS_REG_NSSOUT_NEGATE();
            SPIM_CH_CFG_SET(SPIM_CH_CFG_TX_OFF, SPIM_CH_CFG_RX_OFF);

            state = SPIM_STATE_IDLE;

            if (((eventOperationCallback != NULL) && eventOperationCallback(operationDevice)) || (eventOperationCallback == NULL))
            {
                NU_Activate_HISR(&operationHisr[operationDevice->deviceIndex]);
            }
            break;
        }
        default:
            state = SPIM_STATE_IDLE;
            break;
    }
}

void CsrSpiMasterInitialise(CsrUint8 *devicesCount, CsrSpiMasterDevice **devices)
{
    if (state == SPIM_STATE_STOPPED)
    {
        CsrResult result;
        CsrUint8 deviceIndex;
        CsrUint32 frequency = INITIAL_FREQUENCY;
        CsrUint8 byte;

        CSR_LOG_TEXT_REGISTER(&CsrSpiMasterLto, "SPI_MASTER", 0, NULL);

        *devicesCount = DEVICE_COUNT;
        *devices = spimDevices;

        CsrMemSet(txGarbageBuffer, 0xFF, CSR_ARRAY_SIZE(txGarbageBuffer));

        /* Route CSPI to Radio Module */
        byte = 0x04;
        I2CWrite(0, 0x25, 1, &byte);

        /* SPI_INT(0): ARM(GPIO0) => FPGA(APPS_UART_RTS) */
        byte = 16; /* CSPI_INT0 */
        I2CWrite(0, 0x26, 1, &byte);
        /* SPI_CSB(0): ARM(GPIO19) => FPGA(APPS_UART_CTS) */
        byte = 17; /* CSPI_CSB0 */
        I2CWrite(0, 0x28, 1, &byte);

        /* SPI_INT(1): ARM(GPIO1) => FPGA(PIO15) */
        byte = 15; /* CSPI_INT1 */
        I2CWrite(0, 0x27, 1, &byte);
        /* SPI_CSB(1): ARM(GPIO20) => FPGA(PIO14) */
        byte = 14; /* CSPI_CSB1 */
        I2CWrite(0, 0x29, 1, &byte);

        I2CRead(0, 0x19, 1, &byte);
        byte |= 1 << 6; /* PIO14_RW */
        I2CWrite(0, 0x19, 1, &byte);

        SPIM_CH_CFG_SET(SPIM_CH_CFG_TX_OFF, SPIM_CH_CFG_RX_OFF);
        SPIM_CS_REG_SET(SPIM_CS_REG_SS_MANUAL);

        for (deviceIndex = 0; deviceIndex < DEVICE_COUNT; deviceIndex++)
        {
            spimDevices[deviceIndex].deviceIndex = deviceIndex;
            spimDevices[deviceIndex].busIndex = 0;
            spimDevices[deviceIndex].features = 0;
            spimDevices[deviceIndex].driverData = NULL;
            spimDevices[deviceIndex].priv = NULL;

            /* Configure the chip select pin */
            GPIO_M_FUNCTION_SET(3 + 10 * deviceIndex, GPIO_FUNCTION_OUTPUT);
            GPIO_PULL_SET(GPIO_PORT_M, 3 + 10 * deviceIndex, GPIO_PULL_OFF);

            CsrSpiMasterChipSelectNegate(&spimDevices[deviceIndex]);
            CsrSpiMasterBusClockFrequencySet(&spimDevices[deviceIndex], &frequency);
        }

        /* Reset state */
        state = SPIM_STATE_IDLE;

        /* Create HISR */
        NU_Create_HISR(&operationHisr[0], "SPIM0", &executeOperationDsrCallback0, 1, operationHisrStack[0], OPERATION_HISR_STACK_SIZE);
        NU_Create_HISR(&operationHisr[1], "SPIM1", &executeOperationDsrCallback1, 1, operationHisrStack[1], OPERATION_HISR_STACK_SIZE);

        /* Create blocking mechanism */
        NU_Create_Event_Group(&eventGroup, "SPIM");

        /* Register DMA handler function */
        dmaHandleTx.source = DMA_SOURCE_SPI0_TX;
        dmaHandleTx.channel = DMA_CHANNEL_TX;
        dmaHandleTx.sourceAddress = (void *) &rSPITXDATA(0);
        dmaHandleTx.sourceAddressIncrement = FALSE;
        result = DMAHandleRegister(&dmaHandleTx);
        CSR_LOG_TEXT_CONDITIONAL_CRITICAL(result != CSR_RESULT_SUCCESS, (CsrSpiMasterLto, 0, "Could not register DMA TX channel"));
        CSR_UNUSED(result);

        dmaHandleRx.source = DMA_SOURCE_SPI0_RX;
        dmaHandleRx.channel = DMA_CHANNEL_RX;
        dmaHandleRx.sourceAddress = (void *) &rSPIRXDATA(0);
        dmaHandleRx.sourceAddressIncrement = FALSE;
        result = DMAHandleRegister(&dmaHandleRx);
        CSR_LOG_TEXT_CONDITIONAL_CRITICAL(result != CSR_RESULT_SUCCESS, (CsrSpiMasterLto, 0, "Could not register DMA RX channel"));
        CSR_UNUSED(result);
    }
}

void CsrSpiMasterDeinitialise(void)
{
    if (state != SPIM_STATE_STOPPED)
    {
        CsrUint8 deviceIndex;

        /* Stop listening to DMA and stop it */
        DMAHandleUnregister(&dmaHandleRx);
        DMAHandleUnregister(&dmaHandleTx);

        for (deviceIndex = 0; deviceIndex < DEVICE_COUNT; deviceIndex++)
        {
            CsrSpiMasterCallbackRegister(&spimDevices[deviceIndex], NULL, NULL, NULL);
            CsrSpiMasterChipSelectNegate(&spimDevices[deviceIndex]);
        }

        /* Turn off TX/RX channel */
        SPIM_CH_CFG_SET(SPIM_CH_CFG_TX_OFF, SPIM_CH_CFG_RX_OFF);

        /* Disable the clock */
        SPIM_CLK_DISABLE();

        NU_Delete_HISR(&operationHisr[0]);
        NU_Delete_HISR(&operationHisr[1]);

        state = SPIM_STATE_STOPPED;
    }
}

static void read(CsrSpiMasterDevice *device, CsrUint8 *data, CsrUint32 length)
{
    CSR_LOG_TEXT_ASSERT(CsrSpiMasterLto, 0, length <= CSR_ARRAY_SIZE(txGarbageBuffer));

    /* Set state */
    state = SPIM_STATE_BURST_INPUT_1;
    operationDevice = device;

    /* Configure transfer options (DMA) */
    SPIM_MODE_CFG_SET(SPIM_MODE_CFG_BUS_WIDTH_BYTE, SPIM_MODE_CFG_RX_DMA_SW_ENABLE, SPIM_MODE_CFG_TX_DMA_SW_ENABLE);

    /* Start transfer */
    SPIM_CH_CFG_SET(SPIM_CH_CFG_TX_ON, SPIM_CH_CFG_RX_ON);
    SPIM_CS_REG_NSSOUT_ASSERT();

    /* Setup and start DMA */
    DMARead(&dmaHandleRx, eventDMA, DMA_WIDTH_BYTE, length, data);
    DMAWrite(&dmaHandleTx, eventDMA, DMA_WIDTH_BYTE, length, txGarbageBuffer);
}

#if POLL_TRANSFER_LIMIT > 64
#error POLL_TRANSFER_LIMIT cannot exceed hardware FIFO size (64byte)
#endif

static void readPolled(CsrSpiMasterDevice *device, CsrUint8 *data, CsrUint32 length)
{
    CsrUint32 txLength = length;

    /* Configure transfer options (DMA) */
    SPIM_MODE_CFG_SET(SPIM_MODE_CFG_BUS_WIDTH_BYTE, SPIM_MODE_CFG_RX_DMA_SW_DISABLE, SPIM_MODE_CFG_TX_DMA_SW_DISABLE);

    while (txLength-- > 0)
    {
        /* The FIFO is able to contain 64 bytes so simply fill it as fast as we can */
        SPIM_TRANSMIT(0xFF);
    }

    /* Start transfer */
    SPIM_CH_CFG_SET(SPIM_CH_CFG_TX_ON, SPIM_CH_CFG_RX_ON);
    SPIM_CS_REG_NSSOUT_ASSERT();

    /* Do polled RX */
    while (length > 0)
    {
        CsrUint32 rxFifoLevel;

        SPIM_SPI_STATUS_RX_FIFO_WAIT();
        rxFifoLevel = SPIM_SPI_STATUS_RX_FIFO_LEVEL();
        length -= rxFifoLevel;
        while (rxFifoLevel-- > 0)
        {
            *data = SPIM_RECEIVE();
            ++data;
        }
    }

    /* Turn off TX/RX channel */
    SPIM_CS_REG_NSSOUT_NEGATE();
    SPIM_CH_CFG_SET(SPIM_CH_CFG_TX_OFF, SPIM_CH_CFG_RX_OFF);
}

void CsrSpiMasterRead(CsrSpiMasterDevice *device, CsrUint8 *data, CsrUint32 length)
{
    if (length > POLL_TRANSFER_LIMIT)
    {
        UNSIGNED eventBits;
        eventOperationCallback = NULL;
        read(device, data, length);
        NU_Retrieve_Events(&eventGroup, 1, NU_OR_CONSUME, &eventBits, NU_SUSPEND);
    }
    else
    {
        readPolled(device, data, length);
    }
}

void CsrSpiMasterReadAsync(CsrSpiMasterDevice *device, CsrUint8 *data, CsrUint32 length, CsrSpiMasterCallback operationCallback)
{
    eventOperationCallback = operationCallback;
    read(device, data, length);
}

static void write(CsrSpiMasterDevice *device, const CsrUint8 *data, CsrUint32 length)
{
    /* Set state */
    state = SPIM_STATE_BURST_OUTPUT;
    operationDevice = device;

    /* Configure transfer options (DMA) */
    SPIM_MODE_CFG_SET(SPIM_MODE_CFG_BUS_WIDTH_BYTE, SPIM_MODE_CFG_RX_DMA_SW_DISABLE, SPIM_MODE_CFG_TX_DMA_SW_ENABLE);

    /* Start transfer */
    SPIM_CH_CFG_SET(SPIM_CH_CFG_TX_ON, SPIM_CH_CFG_RX_OFF);
    SPIM_CS_REG_NSSOUT_ASSERT();

    /* Setup and start DMA */
    DMAWrite(&dmaHandleTx, eventDMA, DMA_WIDTH_BYTE, length, (void *) data);
}

static void writePolled(CsrSpiMasterDevice *device, const CsrUint8 *data, CsrUint32 length)
{
    /* Configure transfer options (DMA) */
    SPIM_MODE_CFG_SET(SPIM_MODE_CFG_BUS_WIDTH_BYTE, SPIM_MODE_CFG_RX_DMA_SW_DISABLE, SPIM_MODE_CFG_TX_DMA_SW_DISABLE);

    /* Start transfer */
    SPIM_CH_CFG_SET(SPIM_CH_CFG_TX_ON, SPIM_CH_CFG_RX_OFF);
    SPIM_CS_REG_NSSOUT_ASSERT();

    /* Do polled TX */
    while (length-- > 0)
    {
        /* The FIFO is able to contain 64 bytes so simply fill it as fast as we can */
        SPIM_TRANSMIT(*data);
        ++data;
    }

    /* Wait for last byte to be fully clocked */
    SPIM_SPI_STATUS_TX_FIFO_EMPTY_WAIT();
    SPIM_SPI_STATUS_TX_DONE_WAIT();

    /* Turn off TX/RX channel */
    SPIM_CS_REG_NSSOUT_NEGATE();
    SPIM_CH_CFG_SET(SPIM_CH_CFG_TX_OFF, SPIM_CH_CFG_RX_OFF);
}

void CsrSpiMasterWrite(CsrSpiMasterDevice *device, const CsrUint8 *data, CsrUint32 length)
{
    if (length > POLL_TRANSFER_LIMIT)
    {
        UNSIGNED eventBits;
        eventOperationCallback = NULL;
        write(device, data, length);
        NU_Retrieve_Events(&eventGroup, 1, NU_OR_CONSUME, &eventBits, NU_SUSPEND);
    }
    else
    {
        writePolled(device, data, length);
    }
}

void CsrSpiMasterWriteAsync(CsrSpiMasterDevice *device, const CsrUint8 *data, CsrUint32 length, CsrSpiMasterCallback operationCallback)
{
    eventOperationCallback = operationCallback;
    write(device, data, length);
}

void CsrSpiMasterChipSelectAssert(CsrSpiMasterDevice *device)
{
    GPIO_CLEAR_ATOMIC(GPIO_PORT_N, 3 + 10 * device->deviceIndex);
}

void CsrSpiMasterChipSelectNegate(CsrSpiMasterDevice *device)
{
    GPIO_SET_ATOMIC(GPIO_PORT_N, 3 + 10 * device->deviceIndex);
}

void CsrSpiMasterBusClockFrequencySet(CsrSpiMasterDevice *device, CsrUint32 *frequency)
{
    if (frequency != NULL)
    {
        CsrUint32 prescalerValue;

        /* Cap at MAX_FREQUENCY */
        *frequency = *frequency > MAX_FREQUENCY ? MAX_FREQUENCY : *frequency;

        /* Calulate divider */
        prescalerValue = ((50000000 - 1 + *frequency) / *frequency) - 1;
        prescalerValue = prescalerValue > 255 ? 255 : prescalerValue;

        /* Calculate actual configured frequency */
        *frequency = 50000000 / (prescalerValue + 1);

        /* Set frequency */
        SPIM_CLK_DISABLE();
        SPIM_CLK_CFG_SET(SPIM_CLK_CFG_ENCLK_ENABLE, prescalerValue);

        /* Reset controller */
        SPIM_CH_CFG_RESET_SET();
        SPIM_SPI_STATUS_RX_FIFO_EMPTY_WAIT();
        SPIM_SPI_STATUS_TX_FIFO_EMPTY_WAIT();
        SPIM_SPI_STATUS_OVERRUN_WAIT();
        SPIM_CH_CFG_RESET_CLEAR();

        SPIM_CLK_ENABLE();
    }
}

static void executeInterruptDsrCallback0(void)
{
    if (eventInterruptDsrCallback[0] != NULL)
    {
        eventInterruptDsrCallback[0](&spimDevices[0]);
    }
}

static void executeInterruptDsrCallback1(void)
{
    if (eventInterruptDsrCallback[1] != NULL)
    {
        eventInterruptDsrCallback[1](&spimDevices[1]);
    }
}

static void executeInterruptCallback(CsrUint8 deviceIndex)
{
    if (eventInterruptCallback[deviceIndex] != NULL)
    {
        if (eventInterruptCallback[deviceIndex](&spimDevices[deviceIndex]))
        {
            NU_Activate_HISR(&interruptHisr[deviceIndex]);
        }
    }
}

static VOID eventInterrupt(INT interrupt)
{
    CsrUint32 pending;

    /* Read pending and clear bits that are masked */
    pending = INT_EXT_PENDING_GET() & ~INT_EXT_MASK_GET();

    if (BIT_CHECK(pending, INT_EXT_EINT24))
    {
        INT_EXT_MASK(INT_EXT_EINT24);
        executeInterruptCallback(0);
    }

    if (BIT_CHECK(pending, INT_EXT_EINT25))
    {
        INT_EXT_MASK(INT_EXT_EINT25);
        executeInterruptCallback(1);
    }

    INT_CLEAR();
}

static void setupServiceRoutine(void)
{
    VOID (*previous)(INT);
    if (eventInterruptCallback[0] || eventInterruptCallback[1])
    {
        NU_Register_LISR(INT_EINT3, &eventInterrupt, &previous);
        INT_UNMASK(INT_EINT3);
    }
    else
    {
        INT_MASK(INT_EINT3);
        NU_Register_LISR(INT_EINT3, NULL, &previous);
    }
}

void CsrSpiMasterCallbackRegister(CsrSpiMasterDevice *device, CsrSpiMasterCallback interruptCallback, CsrSpiMasterDsrCallback interruptDsrCallback, CsrSpiMasterDsrCallback operationDsrCallback)
{
    INT_ATOMIC_EXECUTE_HEAD();

    eventOperationDsrCallback[device->deviceIndex] = operationDsrCallback;
    eventInterruptDsrCallback[device->deviceIndex] = interruptDsrCallback;

    if (interruptCallback != NULL)
    {
        if (eventInterruptCallback[device->deviceIndex] != NULL)
        {
            /* Already registered - just replace the handler */
            eventInterruptCallback[device->deviceIndex] = interruptCallback;
        }
        else
        {
            /* New registration */
            eventInterruptCallback[device->deviceIndex] = interruptCallback;

            /* Configure the interrupt pin */
            GPIO_M_FUNCTION_SET(1 + device->deviceIndex, 3);
            GPIO_EINT_24_25_TRIGGER_SET(GPIO_TRIGGER_LOWLEVEL);
            GPIO_PULL_SET(GPIO_PORT_M, 1 + device->deviceIndex, GPIO_PULL_UP);

            /* Create HISR */
            if (device->deviceIndex == 0)
            {
                NU_Create_HISR(&interruptHisr[0], "SPIMI0", &executeInterruptDsrCallback0, 1, interruptHisrStack[0], INTERRUPT_HISR_STACK_SIZE);
            }
            else
            {
                NU_Create_HISR(&interruptHisr[1], "SPIMI1", &executeInterruptDsrCallback1, 1, interruptHisrStack[1], INTERRUPT_HISR_STACK_SIZE);
            }

            /* Clear and unmask */
            INT_EXT_CLEAR(device->deviceIndex ? INT_EXT_EINT25 : INT_EXT_EINT24);
            INT_EXT_UNMASK(device->deviceIndex ? INT_EXT_EINT25 : INT_EXT_EINT24);

            /* Setup appropriate service routine */
            setupServiceRoutine();
        }
    }
    else
    {
        if (eventInterruptCallback[device->deviceIndex] != NULL)
        {
            eventInterruptCallback[device->deviceIndex] = NULL;

            /* Destroy HISR */
            NU_Delete_HISR(&interruptHisr[device->deviceIndex]);

            /* Mask and clear */
            INT_EXT_MASK(device->deviceIndex ? INT_EXT_EINT25 : INT_EXT_EINT24);
            INT_EXT_CLEAR(device->deviceIndex ? INT_EXT_EINT25 : INT_EXT_EINT24);

            /* Setup appropriate service routine */
            setupServiceRoutine();
        }
    }

    INT_ATOMIC_EXECUTE_TAIL();
}

void CsrSpiMasterInterruptAcknowledge(CsrSpiMasterDevice *device)
{
    /* Clear and unmask */
    INT_EXT_CLEAR(device->deviceIndex ? INT_EXT_EINT25 : INT_EXT_EINT24);
    INT_EXT_UNMASK_ATOMIC(device->deviceIndex ? INT_EXT_EINT25 : INT_EXT_EINT24);
}

CsrResult CsrSpiMasterPowerOn(CsrSpiMasterDevice *device)
{
    /* Not implemented */
    return CSR_SPI_MASTER_RESULT_NOT_RESET;
}

void CsrSpiMasterPowerOff(CsrSpiMasterDevice *device)
{
    /* Not implemented */
}

static int topState;
static CsrUint32 nestCount = 0;

void CsrSpiMasterCallbackInhibitEnter(CsrSpiMasterDevice *device)
{
    int previousState, lockedState;
    __asm {
        mrs previousState, CPSR
    };
    __asm {
        orr lockedState, previousState, 0xC0
    };
    __asm {
        msr CPSR_cxsf, lockedState
    };
    if (nestCount++ == 0)
    {
        topState = previousState;
    }
}

void CsrSpiMasterCallbackInhibitLeave(CsrSpiMasterDevice *device)
{
    if (--nestCount == 0)
    {
        __asm {
            msr CPSR_cxsf, topState
        };
    }
}
