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
#include "platform/csr_mmu.h"


/******************************/
/********* Parameters *********/
/******************************/

#define DMA_CHANNEL 1
#define DEVICE_COUNT 2
#define INITIAL_FREQUENCY  400000
#define MAX_FREQUENCY 25000000
#define POLL_TRANSFER_LIMIT 16


/**************************************/
/********* Host Configuration *********/
/**************************************/

/* Options for SPI Control Register (SPCON0) */
#define SPIM_CON_MODE_INTERRUPT         (1 << 5)    /* Enable Interrupt Mode */
#define SPIM_CON_MODE_DMA               (2 << 5)    /* Enable DMA Mode */
#define SPIM_CON_CLOCK                  (1 << 4)    /* Enable SPI CLK output */
#define SPIM_CON_MASTER                 (1 << 3)    /* Configure host as master */
#define SPIM_CON_CLOCK_ACTIVE_HIGH      (0 << 2)    /* Active high clock */
#define SPIM_CON_CLOCK_ACTIVE_LOW       (1 << 2)    /* Active low clock  */
#define SPIM_CON_FORMAT_A               (0 << 1)    /* Format A */
#define SPIM_CON_FORMAT_B               (1 << 1)    /* Format B */
#define SPIM_CON_AUTO_GARBAGE           (1 << 0)    /* TX Auto garbage */

/* Configuration of SPI Control Register (SPCON0) */
#define SPIM_CON_DEFAULT                (SPIM_CON_CLOCK | SPIM_CON_MASTER | SPIM_CON_CLOCK_ACTIVE_LOW | SPIM_CON_FORMAT_B)
#define SPIM_CON_DISABLE                (0)
#define SPIM_CON_SET(options)           {rSPCON0 = (options);}

/* Options for SPI Pin Control Register (SPPIN0) */
#define SPIM_CON_PIN_ENMUL              (1 << 2)    /* Enable multi master error detect (not applicable in single-master system) */
#define SPIM_CON_PIN_KEEP               (1 << 0)    /* Drive the previous level after 1byte transmit finish */

/* Configuration of SPI Pin Control Register (SPPIN0) */
#define SPIM_CON_PIN_DEFAULT            (0)
#define SPIM_CON_PIN_CLEAR()            {rSPPIN0 = 0;}
#define SPIM_CON_PIN_SET(options)       {rSPPIN0 = (options);}

/* Configure SPI CLK Frequency (SPPRE0) */
#define SPIM_PRESCALER_SET(value)       {rSPPRE0 = value;}
#define SPIM_PRESCALER_GET()            (rSPPRE0)


/**************************/
/********* Status *********/
/**************************/

#define SPIM_STATUS_DCOL                (1 << 2)        /* Indicates read/write while RX/TX is in progress */
#define SPIM_STATUS_MULF                (1 << 1)        /* Indicates multi-master error (not applicable in single-master system) */
#define SPIM_STATUS_REDY                (1 << 0)        /* Indicates SPTDAT or SPRDAT is ready for TX or RX */
#define SPIM_STATUS_GET()               (rSPSTA0)       /* Get the Status Register */

/* Note: DCOL and MULF is cleared by reading status. REDY is cleared by writing to SPTDAT */

/* Poll the REDY bit of the status register until set */
#define SPIM_READY_WAIT()               {while (!(SPIM_STATUS_GET() & SPIM_STATUS_REDY)) {}}


/*********************************/
/********* Data Transfer *********/
/*********************************/

#define SPIM_TRANSMIT(value)            {rSPTDAT0 = value;}
#define SPIM_RECEIVE()                  (rSPRDAT0)


/*******************************/
/********* Static Data *********/
/*******************************/

#define SPIM_STATE_STOPPED              0
#define SPIM_STATE_IDLE                 1
#define SPIM_STATE_BURST_INPUT          2
#define SPIM_STATE_BURST_OUTPUT         3

static CsrSpiMasterDevice spimDevices[DEVICE_COUNT];
static CsrSpiMasterDevice *operationDevice;

static CsrUint32 state = SPIM_STATE_STOPPED;

static CsrSpiMasterCallback eventOperationCallback = NULL;
static CsrSpiMasterDsrCallback eventOperationDsrCallback[DEVICE_COUNT] = {NULL, NULL};
static CsrSpiMasterCallback eventInterruptCallback[DEVICE_COUNT] = {NULL, NULL};
static CsrSpiMasterDsrCallback eventInterruptDsrCallback[DEVICE_COUNT] = {NULL, NULL};

static CsrUint8 *dataPtr;
static CsrUint32 dataLength;

/* High Level Callback HISR's */
#define OPERATION_HISR_STACK_SIZE 1024
#define INTERRUPT_HISR_STACK_SIZE 1024
static NU_HISR operationHisr[DEVICE_COUNT];
static NU_HISR interruptHisr[DEVICE_COUNT];
static CsrUint8 operationHisrStack[DEVICE_COUNT][OPERATION_HISR_STACK_SIZE];
static CsrUint8 interruptHisrStack[DEVICE_COUNT][INTERRUPT_HISR_STACK_SIZE];

/* Blocking mechanism */
static NU_EVENT_GROUP eventGroup;

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

static void eventDMA(void)
{
    DMA_STOP(DMA_CHANNEL);

    SPIM_CON_SET(SPIM_CON_DEFAULT);

    switch (state)
    {
        case SPIM_STATE_BURST_INPUT:
        {
            /* Invalidate Data Cache */
            MMUInvalidateDCacheRegion((CsrUint32) dataPtr, dataLength == 1 ? 1 : dataLength - 1);

            /* Read in the last byte when it arrives */
            SPIM_READY_WAIT();
            *(dataPtr + dataLength - 1) = SPIM_RECEIVE();

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
            SPIM_READY_WAIT();

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
        CsrUint8 deviceIndex;
        CsrUint32 frequency = INITIAL_FREQUENCY;
        CsrUint8 byte;

        CSR_LOG_TEXT_REGISTER(&CsrSpiMasterLto, "SPI_MASTER", 0, NULL);

        *devicesCount = DEVICE_COUNT;
        *devices = spimDevices;

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

        for (deviceIndex = 0; deviceIndex < DEVICE_COUNT; deviceIndex++)
        {
            spimDevices[deviceIndex].deviceIndex = deviceIndex;
            spimDevices[deviceIndex].busIndex = 0;
            spimDevices[deviceIndex].features = 0;
            spimDevices[deviceIndex].driverData = NULL;
            spimDevices[deviceIndex].priv = NULL;

            /* Configure the chip select pin */
            GPIOPinConfigure(19 + deviceIndex, GPIO_FUNCTION_OUTPUT, GPIO_PULLUP_OFF, GPIO_TRIGGER_LOWLEVEL);
            CsrSpiMasterChipSelectNegate(&spimDevices[deviceIndex]);
            CsrSpiMasterBusClockFrequencySet(&spimDevices[deviceIndex], &frequency);
        }

        /* Stop listening to DMA and stop it */
        DMARegisterHandler(DMA_CHANNEL, NULL);
        DMA_STOP(DMA_CHANNEL);

        /* Configure host controller */
        SPIM_CON_SET(SPIM_CON_DEFAULT);
        SPIM_CON_PIN_SET(SPIM_CON_PIN_DEFAULT);

        /* Reset state */
        state = SPIM_STATE_IDLE;

        /* Create HISR */
        NU_Create_HISR(&operationHisr[0], "SPIM0", &executeOperationDsrCallback0, 1, operationHisrStack[0], OPERATION_HISR_STACK_SIZE);
        NU_Create_HISR(&operationHisr[1], "SPIM1", &executeOperationDsrCallback1, 1, operationHisrStack[1], OPERATION_HISR_STACK_SIZE);

        /* Create blocking mechanism */
        NU_Create_Event_Group(&eventGroup, "SPIM");

        /* Register DMA handler function */
        DMARegisterHandler(DMA_CHANNEL, eventDMA);
    }
}

void CsrSpiMasterDeinitialise(void)
{
    if (state != SPIM_STATE_STOPPED)
    {
        CsrUint8 deviceIndex;

        /* Stop listening to DMA and stop it */
        DMARegisterHandler(DMA_CHANNEL, NULL);
        DMA_STOP(DMA_CHANNEL);

        for (deviceIndex = 0; deviceIndex < DEVICE_COUNT; deviceIndex++)
        {
            CsrSpiMasterCallbackRegister(&spimDevices[deviceIndex], NULL, NULL, NULL);
            CsrSpiMasterChipSelectNegate(&spimDevices[deviceIndex]);
        }

        /* Disable hardware and clock */
        SPIM_CON_SET(SPIM_CON_DISABLE);
        SPIM_CON_PIN_CLEAR();

        NU_Delete_HISR(&operationHisr[0]);
        NU_Delete_HISR(&operationHisr[1]);

        state = SPIM_STATE_STOPPED;
    }
}

static void read(CsrSpiMasterDevice *device, CsrUint8 *data, CsrUint32 length)
{
    /* Manually clock first byte (required to get the first valid byte into the receive register before starting the DMA) */
    SPIM_TRANSMIT(0xFF);
    SPIM_READY_WAIT();

    /* Setup active buffer (last byte must be read out manually after DMA completes) */
    dataPtr = data;
    dataLength = length;

    /* Setup Host Hardware */
    if (length == 1)
    {
        SPIM_CON_SET(SPIM_CON_DEFAULT | SPIM_CON_MODE_DMA);
    }
    else
    {
        SPIM_CON_SET(SPIM_CON_DEFAULT | SPIM_CON_AUTO_GARBAGE | SPIM_CON_MODE_DMA);
    }

    /* Set state */
    state = SPIM_STATE_BURST_INPUT;
    operationDevice = device;

    /* Writeback from cache to main memory before starting the DMA */
    MMUCleanDCacheRegion((CsrUint32) data, length == 1 ? 1 : length - 1);
    MMUDrainWriteBuffer();

    /* Setup DMA Channel */
    DMA_SOURCE_ADDRESS_SET(DMA_CHANNEL, &rSPRDAT0);
    DMA_SOURCE_CONTROL_SET(DMA_CHANNEL, 1, 1);
    DMA_DESTINATION_ADDRESS_SET(DMA_CHANNEL, data);
    DMA_DESTINATION_CONTROL_SET(DMA_CHANNEL, 0, 0, 0);
    DMA_CON_SET(DMA_CHANNEL, DMA_CON_HANDSHAKE | DMA_CON_SYNC_APB | DMA_CON_INT | DMA_CON_SINGLESERVICE | DMA_CON_HARDTRIGGER | DMA_CON_NOAUTORELOAD | DMA_CON_BYTE, 3, length == 1 ? 1 : length - 1);
    DMA_ON(DMA_CHANNEL);
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
        while (length > 0)
        {
            SPIM_TRANSMIT(0xFF);
            length--;
            data++;
            SPIM_READY_WAIT();
            *(data - 1) = SPIM_RECEIVE();
        }
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

    /* Setup Host Hardware */
    SPIM_CON_SET(SPIM_CON_DEFAULT | SPIM_CON_MODE_DMA);

    /* Writeback from cache to main memory before starting the DMA */
    MMUCleanDCacheRegion((CsrUint32) data, length);
    MMUDrainWriteBuffer();

    /* Setup DMA Channel */
    DMA_SOURCE_ADDRESS_SET(DMA_CHANNEL, data);
    DMA_SOURCE_CONTROL_SET(DMA_CHANNEL, 0, 0);
    DMA_DESTINATION_ADDRESS_SET(DMA_CHANNEL, &rSPTDAT0);
    DMA_DESTINATION_CONTROL_SET(DMA_CHANNEL, 0, 1, 1);
    DMA_CON_SET(DMA_CHANNEL, DMA_CON_HANDSHAKE | DMA_CON_SYNC_APB | DMA_CON_INT | DMA_CON_SINGLESERVICE | DMA_CON_HARDTRIGGER | DMA_CON_NOAUTORELOAD | DMA_CON_BYTE, 3, length);
    DMA_ON(DMA_CHANNEL);
}

void CsrSpiMasterWrite(CsrSpiMasterDevice *device, const CsrUint8 *data, CsrUint32 length)
{
    if (length >= 16)
    {
        UNSIGNED eventBits;
        eventOperationCallback = NULL;
        write(device, data, length);
        NU_Retrieve_Events(&eventGroup, 1, NU_OR_CONSUME, &eventBits, NU_SUSPEND);
    }
    else
    {
        while (length > 0)
        {
            SPIM_TRANSMIT(*data);
            data++;
            length--;
            SPIM_READY_WAIT();
        }
    }
}

void CsrSpiMasterWriteAsync(CsrSpiMasterDevice *device, const CsrUint8 *data, CsrUint32 length, CsrSpiMasterCallback operationCallback)
{
    eventOperationCallback = operationCallback;
    write(device, data, length);
}

void CsrSpiMasterChipSelectAssert(CsrSpiMasterDevice *device)
{
    GPIO_CLEAR_ATOMIC(19 + device->deviceIndex);
}

void CsrSpiMasterChipSelectNegate(CsrSpiMasterDevice *device)
{
    GPIO_SET_ATOMIC(19 + device->deviceIndex);
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
        SPIM_PRESCALER_SET(prescalerValue)
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

    if (BIT_CHECK(pending, INT_EXT_EINT0))
    {
        INT_EXT_MASK(INT_EXT_EINT0);
        executeInterruptCallback(0);
    }

    if (BIT_CHECK(pending, INT_EXT_EINT1))
    {
        INT_EXT_MASK(INT_EXT_EINT1);
        executeInterruptCallback(1);
    }
    INT_CLEAR(INT_EINT0_2);
}

static void setupServiceRoutine(void)
{
    VOID (*previous)(INT);
    if (eventInterruptCallback[0] || eventInterruptCallback[1])
    {
        NU_Register_LISR(INT_EINT0_2, &eventInterrupt, &previous);
        INT_CLEAR(INT_EINT0_2);
        INT_UNMASK(INT_EINT0_2);
    }
    else
    {
        INT_MASK(INT_EINT0_2);
        INT_CLEAR(INT_EINT0_2);
        NU_Register_LISR(INT_EINT0_2, NULL, &previous);
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
            GPIOPinConfigure((device->deviceIndex & 0x1) ? GPIO_PIN_1 : GPIO_PIN_0, GPIO_FUNCTION_SPECIAL_1, GPIO_PULLUP_OFF, GPIO_TRIGGER_LOWLEVEL);

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
            INT_EXT_CLEAR(device->deviceIndex ? INT_EXT_EINT1 : INT_EXT_EINT0);
            INT_EXT_UNMASK(device->deviceIndex ? INT_EXT_EINT1 : INT_EXT_EINT0);

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
            INT_EXT_MASK(device->deviceIndex ? INT_EXT_EINT1 : INT_EXT_EINT0);
            INT_EXT_CLEAR(device->deviceIndex ? INT_EXT_EINT1 : INT_EXT_EINT0);

            /* Setup appropriate service routine */
            setupServiceRoutine();
        }
    }

    INT_ATOMIC_EXECUTE_TAIL();
}

void CsrSpiMasterInterruptAcknowledge(CsrSpiMasterDevice *device)
{
    /* Clear and unmask */
    INT_EXT_CLEAR(device->deviceIndex ? INT_EXT_EINT1 : INT_EXT_EINT0);
    INT_EXT_UNMASK_ATOMIC(device->deviceIndex ? INT_EXT_EINT1 : INT_EXT_EINT0);
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
