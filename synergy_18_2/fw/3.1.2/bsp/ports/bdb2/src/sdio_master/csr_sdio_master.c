/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "nucleus.h"

#include "csr_types.h"
#include "csr_result.h"
#include "csr_util.h"
#include "csr_sdio_master.h"

#include "platform/csr_s3c24a0.h"
#include "platform/csr_dma.h"
#include "platform/csr_interrupt.h"
#include "platform/csr_mmu.h"
#include "platform/csr_gpio.h"


/******************************/
/********* Parameters *********/
/******************************/

#define SDIO_DEVICE_COUNT   1           /* Number of SDIO devices */
#define SDIO_CMD5_RETRIES   65536       /* Maximum times to retry voltage negotiation */
#define SDIO_CMD_POLLS_MAX  65536       /* Maximum number of times to poll for command completion/timeout */
#define SDIO_HOST_OCR       0x300000    /* Support 3.2V-3.4V */
#define SDIO_DMA_CHANNEL    2           /* DMA channel to use */


/**************************************/
/********* Host Configuration *********/
/**************************************/

/* Reset the Host Hardware */
#define SDIO_RESET()                BIT_SET(rSDICON, 8)

/* Configure SDCLK Frequency (SDIPRE) */
#define SDIO_PRESCALER_SET(_value)  {rSDIPRE = _value;}

/* Options for SDIO Control Register (SDICON) */
#define SDIO_CON_CLOCK_SD           (0 << 5)    /* SD clock type */
#define SDIO_CON_CLOCK_MMC          (1 << 5)    /* MMC clock type */
#define SDIO_CON_LITTLEENDIAN       (0 << 4)    /* Little Endian byte order at FIFO access */
#define SDIO_CON_BIGENDIAN          (1 << 4)    /* Big Endian byte order at FIFO access*/
#define SDIO_CON_INT                (1 << 3)    /* Receive Card Interrupts from SDIO card */
#define SDIO_CON_READWAIT           (1 << 2)    /* Enable Read Wait Request Signal to card */
#define SDIO_CON_CLOCK              (1 << 0)    /* Enable SDCLK output */

/* Configuration of SDIO Control Register (SDICON) */
#define SDIO_CON_DEFAULT            (SDIO_CON_CLOCK_SD | SDIO_CON_LITTLEENDIAN | SDIO_CON_INT | SDIO_CON_CLOCK)
#define SDIO_CON_CLEAR()            {rSDICON = 0;}
#define SDIO_CON_SET(_options)      {rSDICON = (_options);}


/*************************************/
/********* Interrupt Control *********/
/*************************************/

#define SDIO_INT_CMDTOUT                    (1 << 15)   /* Command Timeout */
#define SDIO_INT_RSPEND                     (1 << 14)   /* Command Response */
#define SDIO_INT_INTDET                     (1 << 12)   /* SDIO Interrupt */
#define SDIO_INT_DATTOUT                    (1 << 8)    /* Data Timeout */
#define SDIO_INT_DATFIN                     (1 << 7)    /* Data Complete */

#define SDIO_INT_MASK(interrupt)            {rSDIIMSK &= ~(interrupt);}
#define SDIO_INT_MASK_ATOMIC(interrupt)     INT_ATOMIC_EXECUTE(SDIO_INT_MASK(interrupt))
#define SDIO_INT_UNMASK(interrupt)          {rSDIIMSK |= interrupt;}
#define SDIO_INT_UNMASK_ATOMIC(interrupt)   INT_ATOMIC_EXECUTE(SDIO_INT_UNMASK(interrupt))
#define SDIO_INT_ENABLED(interrupt)         (rSDIIMSK & (interrupt))

#define SDIO_INT_MASK_UNMASK(mask, unmask)          {rSDIIMSK = (rSDIIMSK & ~(mask)) | (unmask);}
#define SDIO_INT_MASK_UNMASK_ATOMIC(mask, unmask)   INT_ATOMIC_EXECUTE(SDIO_INT_MASK_UNMASK(mask, unmask))


/************************************/
/********* Command Transfer *********/
/************************************/

/* Decoding of Command Status Register (SDICSTA) */
#define SDIO_CMD_STATUS_INPROGRESS      (1 << 8)    /* Command transfer in progress */
#define SDIO_CMD_STATUS_SENT            (1 << 11)   /* Command has been sent */
#define SDIO_CMD_STATUS_RESPONSE        (1 << 9)    /* Response has been received */
#define SDIO_CMD_STATUS_TIMEOUT         (1 << 10)   /* Response was not received in 64 clocks */
#define SDIO_CMD_STATUS_CRC             (1 << 12)   /* CRC error in response */
#define SDIO_CMD_STATUS_GET()           (rSDICSTA)  /* Get the Command Status Register */
#define SDIO_CMD_STATUS_CLEAR(value)    {rSDICSTA = value;}
#define SDIO_CMD_STATUS_ALL             (SDIO_CMD_STATUS_SENT | \
                                         SDIO_CMD_STATUS_RESPONSE | \
                                         SDIO_CMD_STATUS_TIMEOUT | \
                                         SDIO_CMD_STATUS_CRC)

/* Options for SDIO Command Control Register (SDICCON) */
#define SDIO_CMD_CON_ABORT              (1 << 12)   /* Command is for abort (CMD52 write to CCCR I/O Abort Register) */
#define SDIO_CMD_CON_WITHDATA           (1 << 11)   /* Command is with data */
#define SDIO_CMD_CON_LONGRESPONSE       (1 << 10)   /* Receive 136bit response (not used in SDIO) */
#define SDIO_CMD_CON_WAITRESPONSE       (1 << 9)    /* Wait for a response */
#define SDIO_CMD_CON_START              (1 << 8)    /* Execute the command */

/* Command with no response */
#define SDIO_CMD(_cmd, _arg)            {rSDICARG = (_arg); \
                                         rSDICCON = SDIO_CMD_CON_START | (0x40 | (_cmd));}
/* Command with response */
#define SDIO_CMD_RESP(_cmd, _arg)       {rSDICARG = (_arg); \
                                         rSDICCON = SDIO_CMD_CON_WAITRESPONSE | SDIO_CMD_CON_START | (0x40 | (_cmd));}
/* Command with data and response */
#define SDIO_CMD_DATA_RESP(_cmd, _arg)  {rSDICARG = (_arg); \
                                         rSDICCON = SDIO_CMD_CON_WITHDATA | SDIO_CMD_CON_WAITRESPONSE | SDIO_CMD_CON_START | (0x40 | (_cmd));}

/* Wait for Command Sent (Block) */
#define SDIO_CMD_SENT_WAIT()            {CsrUint32 c = 0; \
                                         while (!(SDIO_CMD_STATUS_GET() & SDIO_CMD_STATUS_SENT) && c++ < SDIO_CMD_POLLS_MAX) {}}

/* Wait for Command Response or Timeout */
#define SDIO_CMD_WAIT()                 {CsrUint32 c = 0; \
                                         while ((!(SDIO_CMD_STATUS_GET() & SDIO_CMD_STATUS_RESPONSE) && !(SDIO_CMD_STATUS_GET() & SDIO_CMD_STATUS_TIMEOUT)) && c++ < SDIO_CMD_POLLS_MAX) {}}

/* Retrieve response */
#define SDIO_CMD_RESPONSE_GET()         (rSDIRSP0)

/* CMD0: GO_IDLE_STATE -> No Response */
#define SDIO_CMD0()                     {SDIO_CMD(0, 0); \
                                         SDIO_CMD_SENT_WAIT();}

/* CMD3: SEND_RELATIVE_ADDR -> R6:
    [31:16] - New Published RCA
    [15] - COM_CRC_ERROR
    [14] - ILLEGAL_COMMAND
    [13] - ERROR */
#define SDIO_CMD3()                 {SDIO_CMD_RESP(3, 0); \
                                     SDIO_CMD_WAIT();}

/* Decoding of response for CMD3 */
#define SDIO_CMD3_RESPONSE_COM_CRC_ERROR        (1 << 15)
#define SDIO_CMD3_RESPONSE_ILLEGAL_COMMAND      (1 << 14)
#define SDIO_CMD3_RESPONSE_ERROR                (1 << 13)
#define SDIO_CMD3_RESPONSE_ALL_ERRORS           (SDIO_CMD3_RESPONSE_COM_CRC_ERROR | \
                                                 SDIO_CMD3_RESPONSE_ILLEGAL_COMMAND | \
                                                 SDIO_CMD3_RESPONSE_ERROR)

/* CMD5: IO_SEND_OP_COND -> R4:
    [23:0] - Supported OCR */
#define SDIO_CMD5(_ocr)             {SDIO_CMD_RESP(5, (_ocr) & 0xFFFFFF); \
                                     SDIO_CMD_WAIT();}

/* CMD7: SELECT/DESELECT_CARD -> R1b:
    [31:0] - Card Status:
    [31] - OUT_OF_RANGE
    [23] - COM_CRC_ERROR
    [22] - ILLEGAL_COMMAND
    [19] - ERROR
    [12:9] - IO_CURRENT_STATE */
#define SDIO_CMD7(_rca)             {SDIO_CMD_RESP(7, (((_rca) & 0xFFFF) << 16)); \
                                     SDIO_CMD_WAIT();}

/* Decoding of response for CMD7 */
#define SDIO_CMD7_RESPONSE_OUT_OF_RANGE         ((CsrUint32) 1 << 31)
#define SDIO_CMD7_RESPONSE_COM_CRC_ERROR        (1 << 23)
#define SDIO_CMD7_RESPONSE_ILLEGAL_COMMAND      (1 << 22)
#define SDIO_CMD7_RESPONSE_ERROR                (1 << 19)
#define SDIO_CMD7_RESPONSE_ALL_ERRORS           (SDIO_CMD7_RESPONSE_OUT_OF_RANGE | \
                                                 SDIO_CMD7_RESPONSE_COM_CRC_ERROR | \
                                                 SDIO_CMD7_RESPONSE_ILLEGAL_COMMAND | \
                                                 SDIO_CMD7_RESPONSE_ERROR)

/* CMD52: IO_RW_DIRECT -> R5:
    [15] - COM_CRC_ERROR
    [14] - ILLEGAL_COMMAND
    [13:12] - IO_CURRENT_STATE
    [11] - ERROR
    [9] - FUNCTION_NUMBER (invalid)
    [8] - OUT_OF_RANGE
    [7:0] - Data */
#define SDIO_CMD52(_rw, _function, _raw, _address, _data)   SDIO_CMD_RESP(52, (((CsrUint32) (_rw & 0x1) << 31) | ((_function) & 0x7) << 28) | (((_raw) & 0x1) << 27) | (((_address) & 0x1FFFF) << 9) | ((_data) & 0xFF))
#define SDIO_CMD52_READ(_function, _address)                SDIO_CMD52(0, (_function), 0, (_address), 0)
#define SDIO_CMD52_WRITE(_function, _address, _data)        SDIO_CMD52(1, (_function), 0, (_address), (_data))
#define SDIO_CMD52_WRITE_READ(_function, _address, _data)   SDIO_CMD52(1, (_function), 1, (_address), (_data))
#define SDIO_CMD52_RESET_IO()                               SDIO_CMD52_WRITE(0, 0x06, 1 << 3)

/* Decoding of response for CMD52 */
#define SDIO_CMD52_RESPONSE_COM_CRC_ERROR       SDIOM_RESULT_RESPONSE_COM_CRC_ERROR
#define SDIO_CMD52_RESPONSE_ILLEGAL_COMMAND     SDIOM_RESULT_RESPONSE_ILLEGAL_COMMAND
#define SDIO_CMD52_RESPONSE_ERROR               SDIOM_RESULT_RESPONSE_ERROR
#define SDIO_CMD52_RESPONSE_FUNCTION_NUMBER     SDIOM_RESULT_RESPONSE_FUNCTION_NUMBER
#define SDIO_CMD52_RESPONSE_OUT_OF_RANGE        SDIOM_RESULT_RESPONSE_OUT_OF_RANGE
#define SDIO_CMD52_RESPONSE_ALL_ERRORS          (SDIO_CMD52_RESPONSE_COM_CRC_ERROR | \
                                                 SDIO_CMD52_RESPONSE_ILLEGAL_COMMAND | \
                                                 SDIO_CMD52_RESPONSE_ERROR | \
                                                 SDIO_CMD52_RESPONSE_FUNCTION_NUMBER | \
                                                 SDIO_CMD52_RESPONSE_OUT_OF_RANGE)


/*********************************/
/********* Data Transfer *********/
/*********************************/

/* Data/Busy Timer Register (SDIDTIMER) */
#define SDIO_DATA_TIMEOUT_DEFAULT                           0x10000
#define SDIO_DATA_TIMEOUT_SET(timeout)                      REG_SET(rSDIDTIMER, 22, 0, (timeout))

/* Block Size Register (SDIBSIZE) */
#define SDIO_DATA_BLOCKSIZE_SET(blocksize)                  REG_SET(rSDIBSIZE, 11, 0, (blocksize))

/* Decoding of Data Status Register (SDIDSTA) */
#define SDIO_DATA_STATUS_INTERRUPT                          (1 << 9)
#define SDIO_DATA_STATUS_CRC_TRANSMIT                       (1 << 7)
#define SDIO_DATA_STATUS_CRC_RECEIVE                        (1 << 6)
#define SDIO_DATA_STATUS_TIMEOUT                            (1 << 5)
#define SDIO_DATA_STATUS_COMPLETE                           (1 << 4)
#define SDIO_DATA_STATUS_GET()                              (rSDIDSTA)
#define SDIO_DATA_STATUS_CLEAR(value)                       {rSDIDSTA = (value);}
#define SDIO_DATA_STATUS_ALL                                (SDIO_DATA_STATUS_CRC_TRANSMIT | \
                                                             SDIO_DATA_STATUS_CRC_RECEIVE | \
                                                             SDIO_DATA_STATUS_TIMEOUT | \
                                                             SDIO_DATA_STATUS_COMPLETE)

/* Options for Data Control Register (SDIDCON) */
#define SDIO_DATA_CON_BYTE                                  (0 << 22)                                       /* Byte FIFO access size */
#define SDIO_DATA_CON_HALFWORD                              (1 << 22)                                       /* Half-word FIFO access size */
#define SDIO_DATA_CON_WORD                                  (2 << 22)                                       /* Word FIFO access size */
#define SDIO_DATA_CON_ACCESS_SIZE(_size)                    (_size << 22)                                   /* Specified FIFO access size, valid values: 0=BYTE, 1=HALFWORD, 2=WORD */
#define SDIO_DATA_CON_MULTICYCLEINT                         (1 << 21)                                       /* SDIO Interrupt period > 2 cycles */
#define SDIO_DATA_CON_AUTOSTART_TRANSMIT                    ((1 << 20) | (1 << 14))                         /* Start after Command */
#define SDIO_DATA_CON_AUTOSTART_RECEIVE                     ((1 << 19) | (1 << 14))                         /* Start after Command */
#define SDIO_DATA_CON_AUTOSTART_BUSYCHECK                   ((1 << 18) | (1 << 14))                         /* Start after Command */
#define SDIO_DATA_CON_BLOCK                                 (1 << 17)                                       /* Specify block mode */
#define SDIO_DATA_CON_WIDE                                  (1 << 16)                                       /* Enable 4bit mode */
#define SDIO_DATA_CON_DMA                                   (1 << 15)                                       /* Enable DMA mode */
#define SDIO_DATA_CON_BURST                                 (1 << 24)                                       /* Enable DMA Burst4 Mode */

/* Operations for Data Control Register (SDIDCON) */
#define SDIO_DATA_CON_OP_RECEIVE                            (2 << 12)
#define SDIO_DATA_CON_OP_TRANSMIT                           (3 << 12)

#define SDIO_DATA_CON_SET(options, op, blockcount)          {rSDIDCON = ((options) | (op) | (blockcount));}
#define SDIO_DATA_CON_CLEAR()                               {rSDIDCON = 0;}


/* CMD53: IO_RW_EXTENDED -> R5:
    [15] - COM_CRC_ERROR
    [14] - ILLEGAL_COMMAND
    [13:12] - IO_CURRENT_STATE
    [11] - ERROR
    [9] - FUNCTION_NUMBER (invalid)
    [8] - OUT_OF_RANGE */
#define SDIO_CMD53(rw, function, block, address, count)         SDIO_CMD_DATA_RESP(53, (((CsrUint32) (rw & 0x1) << 31) | ((function) & 0x7) << 28) | (((block) & 0x1) << 27) | (((address) & 0x1FFFF) << 9) | ((count) & 0x1FF))
#define SDIO_CMD53_BYTEMODE_READ(function, address, count)      SDIO_CMD53(0, function, 0, address, length)
#define SDIO_CMD53_BLOCKMODE_READ(function, address, count)     SDIO_CMD53(0, function, 1, address, blocks)
#define SDIO_CMD53_BYTEMODE_WRITE(function, address, count)     SDIO_CMD53(1, function, 0, address, length)
#define SDIO_CMD53_BLOCKMODE_WRITE(function, address, count)    SDIO_CMD53(1, function, 1, address, blocks)


/*******************************************/
/********* FIFO Control and Status *********/
/*******************************************/

/* Reset FIFO */
#define SDIO_FIFO_RESET()   {rSDIFSTA = (1 << 16);}

/* Decoding of FIFO Status Register (SDIFSTA) */
#define SDIO_FIFO_STATUS_RX_LASTDATA    (1 << 9)
#define SDIO_FIFO_STATUS_CLEAR(value)   {rSDIFSTA = value;}


/*******************************/
/********* Static Data *********/
/*******************************/

CsrBool initialised = FALSE;

static const CsrBool enable4bitMode = TRUE;

#define OPERATION_HISR_STACK_SIZE    1024
#define INTERRUPT_HISR_STACK_SIZE    1024

typedef struct
{
    CsrSdioMasterCallback    operationCallback;
    CsrSdioMasterDsrCallback operationDsrCallback;
    CsrSdioMasterCallback    interruptCallback;
    CsrSdioMasterDsrCallback interruptDsrCallback;
    NU_HISR                  operationHisr;
    NU_HISR                  interruptHisr;
    CsrUint8                 operationHisrStack[OPERATION_HISR_STACK_SIZE];
    CsrUint8                 interruptHisrStack[INTERRUPT_HISR_STACK_SIZE];
    void                    *data;
    CsrUint32                dataLength;
    CsrResult                result;
    CsrBool                  wide;
} CsrSdioMasterDeviceContext;

static CsrSdioMasterDevice sdioDevices[SDIO_DEVICE_COUNT];
static CsrSdioMasterDeviceContext sdioDeviceContexts[SDIO_DEVICE_COUNT];

/* Blocking mechanism */
NU_EVENT_GROUP eventGroup;

/* DMA/FIFO Access Size Conversion Table */
static CsrUint8 lengthToAccessSize[4] = {2, 0, 1, 0}; /* WORD, BYTE, HALFWORD, BYTE */


/**********************************/
/********* Implementation *********/
/**********************************/

static void executeOperationDsrCallback(void)
{
    if (sdioDeviceContexts[0].operationCallback == NULL)
    {
        NU_Set_Events(&eventGroup, 1, NU_OR);
    }
    else
    {
        CsrSdioMasterDsrCallback dsrCallback = sdioDeviceContexts[0].operationDsrCallback;
        if (dsrCallback != NULL)
        {
            dsrCallback(&sdioDevices[0]);
        }
    }
}

static void executeOperationCallback(CsrResult result)
{
    if ((sdioDeviceContexts[0].operationCallback != NULL) && sdioDeviceContexts[0].operationCallback(&sdioDevices[0], result) || (sdioDeviceContexts[0].operationCallback == NULL))
    {
        NU_Activate_HISR(&sdioDeviceContexts[0].operationHisr);
    }
}

static void executeInterruptDsrCallback(void)
{
    CsrSdioMasterDsrCallback dsrCallback = sdioDeviceContexts[0].interruptDsrCallback;
    if (dsrCallback != NULL)
    {
        dsrCallback(&sdioDevices[0]);
    }
}

static VOID eventInterrupt(INT interrupt)
{
    CsrUint32 cStatus, dStatus;
    CsrSdioMasterDevice *device = &sdioDevices[0];
    CsrSdioMasterDeviceContext *deviceContext = &sdioDeviceContexts[0];

    cStatus = SDIO_CMD_STATUS_GET();
    if ((cStatus & SDIO_CMD_STATUS_RESPONSE) && SDIO_INT_ENABLED(SDIO_INT_RSPEND))
    {
        SDIO_CMD_STATUS_CLEAR(SDIO_CMD_STATUS_ALL);

        deviceContext->result = CSR_RESULT_SUCCESS;
        if (cStatus & SDIO_CMD_STATUS_CRC)
        {
            deviceContext->result = CSR_SDIO_MASTER_RESULT_CRC;
        }
        if (deviceContext->data != NULL)
        {
            CsrUint8 *data = (CsrUint8 *) deviceContext->data;
            *data = (CsrUint8) (SDIO_CMD_RESPONSE_GET() & 0xFF);
        }
        executeOperationCallback(deviceContext->result);
    }
    else if ((cStatus & SDIO_CMD_STATUS_TIMEOUT) && SDIO_INT_ENABLED(SDIO_INT_CMDTOUT))
    {
        SDIO_DATA_CON_CLEAR();
        SDIO_CMD_STATUS_CLEAR(SDIO_CMD_STATUS_ALL);
        SDIO_DATA_STATUS_CLEAR(SDIO_DATA_STATUS_ALL);
        DMA_STOP(SDIO_DMA_CHANNEL);
        deviceContext->result = CSR_SDIO_MASTER_RESULT_TIMEOUT;
        executeOperationCallback(deviceContext->result);
    }

    dStatus = SDIO_DATA_STATUS_GET();
    if (dStatus & (SDIO_DATA_STATUS_COMPLETE | SDIO_DATA_STATUS_TIMEOUT | SDIO_DATA_STATUS_INTERRUPT))
    {
        if ((dStatus & SDIO_DATA_STATUS_COMPLETE) && SDIO_INT_ENABLED(SDIO_INT_DATFIN))
        {
            SDIO_DATA_CON_CLEAR();
            SDIO_CMD_STATUS_CLEAR(SDIO_CMD_STATUS_ALL);
            SDIO_DATA_STATUS_CLEAR(SDIO_DATA_STATUS_ALL);
            DMA_STOP(SDIO_DMA_CHANNEL);
            DMA_PENDING_CLEAR(SDIO_DMA_CHANNEL);

            deviceContext->result = CSR_RESULT_SUCCESS;
            if ((cStatus & SDIO_CMD_STATUS_CRC) || (dStatus & SDIO_DATA_STATUS_CRC_TRANSMIT))
            {
                deviceContext->result = CSR_SDIO_MASTER_RESULT_CRC;
            }
            executeOperationCallback(deviceContext->result);
        }

        if ((dStatus & SDIO_DATA_STATUS_TIMEOUT) && SDIO_INT_ENABLED(SDIO_INT_DATTOUT))
        {
            SDIO_DATA_CON_CLEAR();
            SDIO_CMD_STATUS_CLEAR(SDIO_CMD_STATUS_ALL);
            SDIO_DATA_STATUS_CLEAR(SDIO_DATA_STATUS_ALL);
            DMA_STOP(SDIO_DMA_CHANNEL);
            deviceContext->result = CSR_SDIO_MASTER_RESULT_TIMEOUT;
            executeOperationCallback(deviceContext->result);
        }

        if ((dStatus & SDIO_DATA_STATUS_INTERRUPT) && (SDIO_INT_ENABLED(SDIO_INT_INTDET)))
        {
            SDIO_INT_MASK(SDIO_INT_INTDET);
            if ((sdioDeviceContexts[0].interruptCallback != NULL) && sdioDeviceContexts[0].interruptCallback(device, CSR_RESULT_SUCCESS))
            {
                NU_Activate_HISR(&sdioDeviceContexts[0].interruptHisr);
            }
        }
    }

    INT_CLEAR(INT_SDI);
}

static void eventDMA(void)
{
    CsrUint32 cStatus, dStatus;
    CsrSdioMasterDeviceContext *deviceContext = &sdioDeviceContexts[0];

    cStatus = SDIO_CMD_STATUS_GET();
    dStatus = SDIO_DATA_STATUS_GET();

    SDIO_DATA_CON_CLEAR();
    SDIO_CMD_STATUS_CLEAR(SDIO_CMD_STATUS_ALL);
    SDIO_DATA_STATUS_CLEAR(SDIO_DATA_STATUS_ALL);
    SDIO_FIFO_STATUS_CLEAR(SDIO_FIFO_STATUS_RX_LASTDATA);
    DMA_STOP(SDIO_DMA_CHANNEL);

    deviceContext->result = CSR_RESULT_SUCCESS;
    if ((cStatus & SDIO_CMD_STATUS_CRC) || (dStatus & SDIO_DATA_STATUS_CRC_RECEIVE))
    {
        deviceContext->result = CSR_SDIO_MASTER_RESULT_CRC;
    }

    MMUInvalidateDCacheRegion((CsrUint32) deviceContext->data, deviceContext->dataLength);

    executeOperationCallback(deviceContext->result);
}

void CsrSdioMasterInitialise(CsrUint8 *devicesCount, CsrSdioMasterDevice **devices)
{
    if (!initialised)
    {
        CsrUint8 busIndex;
        VOID (*previous)(INT);

        *devicesCount = SDIO_DEVICE_COUNT;
        *devices = sdioDevices;

        CsrMemSet(sdioDevices, 0, sizeof(sdioDevices));
        for (busIndex = 0; busIndex < SDIO_DEVICE_COUNT; busIndex++)
        {
            sdioDevices[busIndex].busIndex = busIndex;
            sdioDevices[busIndex].features = 0;
        }

        /* Register interrupt handlers */
        NU_Register_LISR(INT_SDI, eventInterrupt, &previous);
        INT_UNMASK_ATOMIC(INT_SDI);
        DMARegisterHandler(SDIO_DMA_CHANNEL, eventDMA);

        /* Create Operation HISR */
        NU_Create_HISR(&sdioDeviceContexts[0].operationHisr, "SDIOO0", &executeOperationDsrCallback, 1, sdioDeviceContexts[0].operationHisrStack, OPERATION_HISR_STACK_SIZE);

        /* Create Interrupt HISR */
        NU_Create_HISR(&sdioDeviceContexts[0].interruptHisr, "SDIOI0", &executeInterruptDsrCallback, 1, sdioDeviceContexts[0].interruptHisrStack, INTERRUPT_HISR_STACK_SIZE);

        /* Create blocking mechanism */
        NU_Create_Event_Group(&eventGroup, "SDIO");

        initialised = TRUE;
    }
}

void CsrSdioMasterDeinitialise(void)
{
    if (initialised)
    {
        VOID (*previous)(INT);

        /* Unregister interrupt handlers */
        INT_MASK_ATOMIC(INT_SDI);
        INT_CLEAR(INT_SDI);
        NU_Register_LISR(INT_SDI, NULL, &previous);
        DMA_STOP(SDIO_DMA_CHANNEL);
        DMARegisterHandler(SDIO_DMA_CHANNEL, NULL);

        /* Reset host hardware */
        SDIO_RESET();
        SDIO_FIFO_RESET();
        SDIO_CON_SET(SDIO_CON_DEFAULT);

        /* Destroy Operation HISR */
        NU_Delete_HISR(&sdioDeviceContexts[0].operationHisr);

        /* Destroy Interrupt HISR */
        NU_Delete_HISR(&sdioDeviceContexts[0].interruptHisr);

        initialised = FALSE;
    }
}

void CsrSdioMasterCallbackRegister(CsrSdioMasterDevice *device, CsrSdioMasterCallback interruptCallback, CsrSdioMasterDsrCallback interruptDsrCallback, CsrSdioMasterDsrCallback operationDsrCallback)
{
    sdioDeviceContexts[0].interruptCallback = interruptCallback;
    sdioDeviceContexts[0].interruptDsrCallback = interruptDsrCallback;
    sdioDeviceContexts[0].operationDsrCallback = operationDsrCallback;
}

CsrResult CsrSdioMasterDeviceInitialise(CsrSdioMasterDevice *device)
{
    CsrUint8 data;
    CsrInt32 i;
    CsrUint32 status;
    CsrUint32 response, ocr, rca;

    CsrMemSet(device->blockSize, 0, sizeof(device->blockSize));

    /* Reset the corresponding host controller */
    SDIO_RESET();
    SDIO_FIFO_RESET();
    SDIO_CON_SET(SDIO_CON_DEFAULT);

    /* Set clock frequency to 400kHz */
    SDIO_PRESCALER_SET(249);

    /* Reset SDIO part of the device if previously initialised - might not produce reply, so do not check */
    SDIO_CMD52_RESET_IO();
    SDIO_CMD_WAIT();
    SDIO_CMD_STATUS_CLEAR(SDIO_CMD_STATUS_ALL);

    /* Reset combo card - does not produce a reply */
    SDIO_CMD0();
    SDIO_CMD_STATUS_CLEAR(SDIO_CMD_STATUS_ALL);

    /* Inquire about supported voltages */
    SDIO_CMD5(0);
    status = SDIO_CMD_STATUS_GET();
    SDIO_CMD_STATUS_CLEAR(SDIO_CMD_STATUS_ALL);
    if (status & SDIO_CMD_STATUS_TIMEOUT)
    {
        return CSR_RESULT_FAILURE;
    }
    response = SDIO_CMD_RESPONSE_GET();
    ocr = response & 0xFFFFFF;                   /* Supported Voltages */
    device->functions = (response >> 28) & 0x7;   /* Number of Card Functions */

    /* Check if host supports the voltages accepted by the card */
    if (!(ocr & SDIO_HOST_OCR))
    {
        return CSR_RESULT_FAILURE;
    }

    /* Specify operational voltage, and poll until ready (limited by SDIO_CMD5_RETRIES) */
    for (i = 0; i < SDIO_CMD5_RETRIES; i++)
    {
        SDIO_CMD5(SDIO_HOST_OCR);
        status = SDIO_CMD_STATUS_GET();
        SDIO_CMD_STATUS_CLEAR(SDIO_CMD_STATUS_ALL);
        if (status & SDIO_CMD_STATUS_TIMEOUT)
        {
            return CSR_RESULT_FAILURE;
        }
        if ((SDIO_CMD_RESPONSE_GET() >> 31) & 0x1)
        {
            break;
        }
    }
    if (i == SDIO_CMD5_RETRIES)
    {
        /* Voltage Negotiation Failed */
        return CSR_RESULT_FAILURE;
    }

    /* Assign Address */
    SDIO_CMD3();
    status = SDIO_CMD_STATUS_GET();
    SDIO_CMD_STATUS_CLEAR(SDIO_CMD_STATUS_ALL);
    if (status & (SDIO_CMD_STATUS_TIMEOUT | SDIO_CMD_STATUS_CRC))
    {
        return CSR_RESULT_FAILURE;
    }
    else
    {
        response = SDIO_CMD_RESPONSE_GET();
        if (response & SDIO_CMD3_RESPONSE_ALL_ERRORS)
        {
            /* Card Error (Possible CRC error in command) */
            return CSR_RESULT_FAILURE;
        }
        rca = (response >> 16) & 0xFFFF;
    }

    /* Set clock frequency to 25MHz */
    SDIO_PRESCALER_SET(3);

    /* Select card */
    SDIO_CMD7(rca);
    status = SDIO_CMD_STATUS_GET();
    SDIO_CMD_STATUS_CLEAR(SDIO_CMD_STATUS_ALL);
    if (status & (SDIO_CMD_STATUS_TIMEOUT | SDIO_CMD_STATUS_CRC))
    {
        return CSR_RESULT_FAILURE;
    }
    else
    {
        if (SDIO_CMD_RESPONSE_GET() & SDIO_CMD7_RESPONSE_ALL_ERRORS)
        {
            /* Card Error (Possible CRC error in command) */
            return CSR_RESULT_FAILURE;
        }
    }

    /* Enable interrupt generation */
    SDIO_INT_UNMASK(SDIO_INT_INTDET);

    /* Set bus width */
    sdioDeviceContexts[0].wide = FALSE;
    if (enable4bitMode)
    {
        if (CsrSdioMasterRead8(device, 0, 0x08, &data) == CSR_RESULT_SUCCESS)
        {
            CsrBool lsc, bls;
            lsc = BIT_CHECK(data, 6) ? TRUE : FALSE;
            bls = BIT_CHECK(data, 7) ? TRUE : FALSE;

            if ((!lsc || (lsc && bls)) &&
                (CsrSdioMasterRead8(device, 0, 0x07, &data) == CSR_RESULT_SUCCESS) &&
                (CsrSdioMasterWrite8(device, 0, 0x07, (data & 0xFC) | (1 << 1)) == CSR_RESULT_SUCCESS))
            {
                sdioDeviceContexts[0].wide = TRUE;
            }
            else
            {
                return CSR_RESULT_FAILURE;
            }
        }
        else
        {
            return CSR_RESULT_FAILURE;
        }
    }

    return CSR_RESULT_SUCCESS;
}

CsrResult CsrSdioMasterRead8(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, CsrUint8 *data)
{
    CsrUint32 status;

    /* Issue Command */
    SDIO_INT_MASK_ATOMIC(SDIO_INT_RSPEND | SDIO_INT_CMDTOUT);
    SDIO_CMD52_READ(function, address);

    /* Wait for response */
    SDIO_CMD_WAIT();

    /* Acknowledge and read status */
    status = SDIO_CMD_STATUS_GET();
    SDIO_CMD_STATUS_CLEAR(SDIO_CMD_STATUS_ALL);

    if (status & (SDIO_CMD_STATUS_TIMEOUT | SDIO_CMD_STATUS_CRC))
    {
        if (status & SDIO_CMD_STATUS_TIMEOUT)
        {
            return CSR_SDIO_MASTER_RESULT_TIMEOUT;
        }
        else if (status & SDIO_CMD_STATUS_CRC)
        {
            return CSR_SDIO_MASTER_RESULT_CRC;
        }
    }

    *data = (CsrUint8) (SDIO_CMD_RESPONSE_GET() & 0xFF);

    return CSR_RESULT_SUCCESS;
}

void CsrSdioMasterRead8Async(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrSdioMasterCallback operationCallback)
{
    CsrSdioMasterDeviceContext *deviceContext = &sdioDeviceContexts[0];

    /* Setup state */
    deviceContext->operationCallback = operationCallback;
    deviceContext->data = data;

    /* Issue Command */
    SDIO_INT_UNMASK_ATOMIC(SDIO_INT_RSPEND | SDIO_INT_CMDTOUT);
    SDIO_CMD52_READ(function, address);
}

CsrResult CsrSdioMasterWrite8(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, CsrUint8 data)
{
    CsrUint32 status;

    /* Issue Command */
    SDIO_INT_MASK_ATOMIC(SDIO_INT_RSPEND | SDIO_INT_CMDTOUT);
    SDIO_CMD52_WRITE(function, address, data);

    /* Wait for response */
    SDIO_CMD_WAIT();

    /* Acknowledge and read status */
    status = SDIO_CMD_STATUS_GET();
    SDIO_CMD_STATUS_CLEAR(SDIO_CMD_STATUS_ALL);

    if (status & (SDIO_CMD_STATUS_TIMEOUT | SDIO_CMD_STATUS_CRC))
    {
        if (status & SDIO_CMD_STATUS_TIMEOUT)
        {
            return CSR_SDIO_MASTER_RESULT_TIMEOUT;
        }
        else if (status & SDIO_CMD_STATUS_CRC)
        {
            return CSR_SDIO_MASTER_RESULT_CRC;
        }
    }

    return CSR_RESULT_SUCCESS;
}

void CsrSdioMasterWrite8Async(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, CsrUint8 data, CsrSdioMasterCallback operationCallback)
{
    CsrSdioMasterDeviceContext *deviceContext = &sdioDeviceContexts[0];

    /* Setup state */
    deviceContext->operationCallback = operationCallback;
    deviceContext->data = NULL;

    /* Issue Command */
    SDIO_INT_UNMASK_ATOMIC(SDIO_INT_RSPEND | SDIO_INT_CMDTOUT);
    SDIO_CMD52_WRITE(function, address, data);
}

static CsrResult blockModeRead(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrUint16 blocks, CsrSdioMasterCallback operationCallback)
{
    CsrUint8 accessSize;
    CsrSdioMasterDeviceContext *deviceContext = &sdioDeviceContexts[device->busIndex];

    /* Setup state */
    deviceContext->operationCallback = operationCallback;
    deviceContext->data = data;
    deviceContext->dataLength = blocks * device->blockSize[function];

    /* Lookup Access Size for DMA and FIFO */
    accessSize = lengthToAccessSize[(deviceContext->dataLength | (CsrUint32) data) & 0x3]; /* Conversion: 0->2 (WORD). 1->0 (BYTE). 2->1 (HALFWORD). 3->0 (BYTE)*/

    /* Writeback from cache to main memory before starting the DMA */
    MMUCleanDCacheRegion((CsrUint32) data, deviceContext->dataLength);
    MMUDrainWriteBuffer();

    /* Setup for DMA */
    DMA_SOURCE_ADDRESS_SET(SDIO_DMA_CHANNEL, &rSDIDAT);
    DMA_SOURCE_CONTROL_SET(SDIO_DMA_CHANNEL, 1, 1);
    DMA_DESTINATION_ADDRESS_SET(SDIO_DMA_CHANNEL, data);
    DMA_DESTINATION_CONTROL_SET(SDIO_DMA_CHANNEL, 0, 0, 0);
    DMA_CON_SET(SDIO_DMA_CHANNEL, DMA_CON_HANDSHAKE | DMA_CON_SYNC_APB | DMA_CON_INT | DMA_CON_SINGLESERVICE | DMA_CON_HARDTRIGGER | DMA_CON_NOAUTORELOAD | DMA_CON_ACCESS_SIZE(accessSize), 2, deviceContext->dataLength >> accessSize);
    DMA_ON(SDIO_DMA_CHANNEL);
    SDIO_DATA_BLOCKSIZE_SET(device->blockSize[function]);

    /* Issue Command */
    INT_ATOMIC_EXECUTE
    (
        SDIO_INT_MASK_UNMASK(SDIO_INT_RSPEND | SDIO_INT_DATFIN, SDIO_INT_CMDTOUT | SDIO_INT_DATTOUT);
        DMA_UNMASK_CHANNEL(SDIO_DMA_CHANNEL);
    );
    SDIO_DATA_CON_SET((deviceContext->wide ? SDIO_DATA_CON_WIDE : 0) | SDIO_DATA_CON_ACCESS_SIZE(accessSize) | SDIO_DATA_CON_AUTOSTART_RECEIVE | SDIO_DATA_CON_BLOCK | SDIO_DATA_CON_DMA, SDIO_DATA_CON_OP_RECEIVE, blocks);
    SDIO_CMD53_BLOCKMODE_READ(function, address, blocks);

    if (operationCallback == NULL)
    {
        /* Wait until command is complete */
        UNSIGNED eventBits;
        NU_Retrieve_Events(&eventGroup, 1, NU_OR_CONSUME, &eventBits, NU_SUSPEND);
    }

    return deviceContext->result;
}

CsrResult CsrSdioMasterBlockModeRead(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrUint16 blocks)
{
    return blockModeRead(device, function, address, data, blocks, NULL);
}

void CsrSdioMasterBlockModeReadAsync(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrUint16 blocks, CsrSdioMasterCallback operationCallback)
{
    blockModeRead(device, function, address, data, blocks, operationCallback);
}

static CsrResult blockModeWrite(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, const CsrUint8 *data, CsrUint16 blocks, CsrSdioMasterCallback operationCallback)
{
    CsrUint32 length;
    CsrUint8 accessSize;
    CsrSdioMasterDeviceContext *deviceContext = &sdioDeviceContexts[device->busIndex];

    /* Setup state */
    deviceContext->operationCallback = operationCallback;
    deviceContext->data = NULL;

    /* Lookup Access Size for DMA and FIFO */
    length = blocks * device->blockSize[function];
    accessSize = lengthToAccessSize[(length | (CsrUint32) data) & 0x3]; /* Conversion: 0->2 (WORD). 1->0 (BYTE). 2->1 (HALFWORD). 3->0 (BYTE)*/

    /* Writeback from cache to main memory before starting the DMA */
    MMUCleanDCacheRegion((CsrUint32) data, length);
    MMUDrainWriteBuffer();

    /* Setup for DMA */
    DMA_SOURCE_ADDRESS_SET(SDIO_DMA_CHANNEL, data);
    DMA_SOURCE_CONTROL_SET(SDIO_DMA_CHANNEL, 0, 0);
    DMA_DESTINATION_ADDRESS_SET(SDIO_DMA_CHANNEL, &rSDIDAT);
    DMA_DESTINATION_CONTROL_SET(SDIO_DMA_CHANNEL, 0, 1, 1);
    DMA_CON_SET(SDIO_DMA_CHANNEL, DMA_CON_HANDSHAKE | DMA_CON_SYNC_APB | DMA_CON_INT | DMA_CON_SINGLESERVICE | DMA_CON_HARDTRIGGER | DMA_CON_NOAUTORELOAD | DMA_CON_ACCESS_SIZE(accessSize), 2, length >> accessSize);
    DMA_ON(SDIO_DMA_CHANNEL);
    SDIO_DATA_BLOCKSIZE_SET(device->blockSize[function]);

    /* Issue Command */
    INT_ATOMIC_EXECUTE
    (
        SDIO_INT_MASK_UNMASK(SDIO_INT_RSPEND, SDIO_INT_CMDTOUT | SDIO_INT_DATFIN | SDIO_INT_DATTOUT);
        DMA_MASK_CHANNEL(SDIO_DMA_CHANNEL);
    );
    SDIO_DATA_CON_SET((deviceContext->wide ? SDIO_DATA_CON_WIDE : 0) | SDIO_DATA_CON_ACCESS_SIZE(accessSize) | SDIO_DATA_CON_AUTOSTART_TRANSMIT | SDIO_DATA_CON_BLOCK | SDIO_DATA_CON_DMA, SDIO_DATA_CON_OP_TRANSMIT, blocks);
    SDIO_CMD53_BLOCKMODE_WRITE(function, address, blocks);

    if (operationCallback == NULL)
    {
        /* Wait until command is complete */
        UNSIGNED eventBits;
        NU_Retrieve_Events(&eventGroup, 1, NU_OR_CONSUME, &eventBits, NU_SUSPEND);
    }

    return deviceContext->result;
}

CsrResult CsrSdioMasterBlockModeWrite(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, const CsrUint8 *data, CsrUint16 blocks)
{
    return blockModeWrite(device, function, address, data, blocks, NULL);
}

void CsrSdioMasterBlockModeWriteAsync(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, const CsrUint8 *data, CsrUint16 blocks, CsrSdioMasterCallback operationCallback)
{
    blockModeWrite(device, function, address, data, blocks, operationCallback);
}

static CsrResult byteModeRead(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrUint16 length, CsrSdioMasterCallback operationCallback)
{
    CsrUint8 accessSize;
    CsrSdioMasterDeviceContext *deviceContext = &sdioDeviceContexts[device->busIndex];

    /* Setup state */
    deviceContext->operationCallback = operationCallback;
    deviceContext->data = data;
    deviceContext->dataLength = length;

    /* Lookup Access Size for DMA and FIFO */
    accessSize = lengthToAccessSize[(length | (CsrUint32) data) & 0x3]; /* Conversion: 0->2 (WORD). 1->0 (BYTE). 2->1 (HALFWORD). 3->0 (BYTE)*/

    /* Writeback from cache to main memory before starting the DMA */
    MMUCleanDCacheRegion((CsrUint32) data, length);
    MMUDrainWriteBuffer();

    /* Setup for DMA */
    DMA_SOURCE_ADDRESS_SET(SDIO_DMA_CHANNEL, &rSDIDAT);
    DMA_SOURCE_CONTROL_SET(SDIO_DMA_CHANNEL, 1, 1);
    DMA_DESTINATION_ADDRESS_SET(SDIO_DMA_CHANNEL, data);
    DMA_DESTINATION_CONTROL_SET(SDIO_DMA_CHANNEL, 0, 0, 0);
    DMA_CON_SET(SDIO_DMA_CHANNEL, DMA_CON_HANDSHAKE | DMA_CON_SYNC_APB | DMA_CON_INT | DMA_CON_SINGLESERVICE | DMA_CON_HARDTRIGGER | DMA_CON_NOAUTORELOAD | DMA_CON_ACCESS_SIZE(accessSize), 2, length >> accessSize);
    DMA_ON(SDIO_DMA_CHANNEL);
    SDIO_DATA_BLOCKSIZE_SET(length);

    /* Issue Command */
    INT_ATOMIC_EXECUTE
    (
        SDIO_INT_MASK_UNMASK(SDIO_INT_RSPEND | SDIO_INT_DATFIN, SDIO_INT_CMDTOUT | SDIO_INT_DATTOUT);
        DMA_UNMASK_CHANNEL(SDIO_DMA_CHANNEL);
    );
    SDIO_DATA_CON_SET(SDIO_DATA_CON_ACCESS_SIZE(accessSize) | (deviceContext->wide ? SDIO_DATA_CON_WIDE : 0) | SDIO_DATA_CON_AUTOSTART_RECEIVE | SDIO_DATA_CON_BLOCK | SDIO_DATA_CON_DMA, SDIO_DATA_CON_OP_RECEIVE, 1);
    SDIO_CMD53_BYTEMODE_READ(function, address, length);

    if (operationCallback == NULL)
    {
        /* Wait until command is complete */
        UNSIGNED eventBits;
        NU_Retrieve_Events(&eventGroup, 1, NU_OR_CONSUME, &eventBits, NU_SUSPEND);
    }

    return deviceContext->result;
}

CsrResult CsrSdioMasterByteModeRead(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrUint16 length)
{
    return byteModeRead(device, function, address, data, length, NULL);
}

void CsrSdioMasterByteModeReadAsync(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrUint16 length, CsrSdioMasterCallback operationCallback)
{
    byteModeRead(device, function, address, data, length, operationCallback);
}

static CsrResult byteModeWrite(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, const CsrUint8 *data, CsrUint16 length, CsrSdioMasterCallback operationCallback)
{
    CsrUint8 accessSize;
    CsrSdioMasterDeviceContext *deviceContext = &sdioDeviceContexts[device->busIndex];

    /* Setup state */
    deviceContext->data = NULL;
    deviceContext->operationCallback = operationCallback;

    /* Lookup Access Size for DMA and FIFO */
    accessSize = lengthToAccessSize[(length | (CsrUint32) data) & 0x3]; /* Conversion: 0->2 (WORD). 1->0 (BYTE). 2->1 (HALFWORD). 3->0 (BYTE)*/

    /* Writeback from cache to main memory before starting the DMA */
    MMUCleanDCacheRegion((CsrUint32) data, length);
    MMUDrainWriteBuffer();

    /* Setup for DMA */
    DMA_SOURCE_ADDRESS_SET(SDIO_DMA_CHANNEL, data);
    DMA_SOURCE_CONTROL_SET(SDIO_DMA_CHANNEL, 0, 0);
    DMA_DESTINATION_ADDRESS_SET(SDIO_DMA_CHANNEL, &rSDIDAT);
    DMA_DESTINATION_CONTROL_SET(SDIO_DMA_CHANNEL, 0, 1, 1);
    DMA_CON_SET(SDIO_DMA_CHANNEL, DMA_CON_HANDSHAKE | DMA_CON_SYNC_APB | DMA_CON_INT | DMA_CON_SINGLESERVICE | DMA_CON_HARDTRIGGER | DMA_CON_NOAUTORELOAD | DMA_CON_ACCESS_SIZE(accessSize), 2, length >> accessSize);
    DMA_ON(SDIO_DMA_CHANNEL);
    SDIO_DATA_BLOCKSIZE_SET(length);

    /* Issue Command */
    INT_ATOMIC_EXECUTE
    (
        SDIO_INT_MASK_UNMASK(SDIO_INT_RSPEND, SDIO_INT_CMDTOUT | SDIO_INT_DATFIN | SDIO_INT_DATTOUT);
        DMA_MASK_CHANNEL(SDIO_DMA_CHANNEL);
    );
    SDIO_DATA_CON_SET(SDIO_DATA_CON_ACCESS_SIZE(accessSize) | (deviceContext->wide ? SDIO_DATA_CON_WIDE : 0) | SDIO_DATA_CON_AUTOSTART_TRANSMIT | SDIO_DATA_CON_BLOCK | SDIO_DATA_CON_DMA, SDIO_DATA_CON_OP_TRANSMIT, 1);
    SDIO_CMD53_BYTEMODE_WRITE(function, address, length);

    if (operationCallback == NULL)
    {
        /* Wait until command is complete */
        UNSIGNED eventBits;
        NU_Retrieve_Events(&eventGroup, 1, NU_OR_CONSUME, &eventBits, NU_SUSPEND);
    }

    return deviceContext->result;
}

CsrResult CsrSdioMasterByteModeWrite(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, const CsrUint8 *data, CsrUint16 length)
{
    return byteModeWrite(device, function, address, data, length, NULL);
}

void CsrSdioMasterByteModeWriteAsync(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, const CsrUint8 *data, CsrUint16 length, CsrSdioMasterCallback operationCallback)
{
    byteModeWrite(device, function, address, data, length, operationCallback);
}

void CsrSdioMasterInterruptAcknowledge(CsrSdioMasterDevice *device)
{
    /* Clear Pending */
    SDIO_DATA_STATUS_CLEAR(SDIO_DATA_STATUS_INTERRUPT);

    /* Unmask */
    SDIO_INT_UNMASK_ATOMIC(SDIO_INT_INTDET);
}

CsrResult CsrSdioMasterBlockSizeSet(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint16 blockSize)
{
    CsrResult result;
    CsrUint32 functionBaseAddress;

    functionBaseAddress = (function & 0x7) * 0x100;

    /* Disqualify invalid values */
    if (blockSize > 2048)
    {
        blockSize = 2048;
    }

    result = CsrSdioMasterWrite8(device, 0, 0x10 + functionBaseAddress, blockSize & 0x00FC);
    if (result == CSR_RESULT_SUCCESS)
    {
        result = CsrSdioMasterWrite8(device, 0, 0x11 + functionBaseAddress, blockSize >> 8);
    }

    if (result == CSR_RESULT_SUCCESS)
    {
        device->blockSize[function & 0x7] = blockSize & 0xFFFC;
    }
    else
    {
        device->blockSize[function & 0x7] = 0;
    }

    return result;
}

CsrResult CsrSdioMasterBusClockFrequencySet(CsrSdioMasterDevice *device, CsrUint32 *frequency)
{
    if (frequency)
    {
        CsrUint8 data;
        CsrResult result;
        CsrUint32 prescaler;

        /* Manage the EHS bit */
        result = CsrSdioMasterRead8(device, 0, 0x13, &data);
        if (result != CSR_RESULT_SUCCESS)
        {
            return result;
        }
        if ((*frequency > 25000000) && (data & 1) && !(data & 2))
        {
            BIT_SET(data, 1);
            result = CsrSdioMasterWrite8(device, 0, 0x13, data);
            if (result != CSR_RESULT_SUCCESS)
            {
                return result;
            }
        }
        else if ((*frequency <= 25000000) && (data & 2))
        {
            BIT_CLEAR(data, 1);
            result = CsrSdioMasterWrite8(device, 0, 0x13, data);
            if (result != CSR_RESULT_SUCCESS)
            {
                return result;
            }
        }

        /* Cap the frequency if EHS bit is not set */
        if (!(data & 2) && (*frequency > 250000000))
        {
            *frequency = 25000000;
        }

        prescaler = ((100000000 - 1 + *frequency) / *frequency) - 1;
        prescaler = prescaler > 255 ? 255 : prescaler;
        *frequency = 100000000 / (prescaler + 1);

        SDIO_PRESCALER_SET(prescaler);
    }

    return CSR_RESULT_SUCCESS;
}

CsrResult CsrSdioMasterPowerOn(CsrSdioMasterDevice *device)
{
    /* Not implemented */
    return CSR_SDIO_MASTER_RESULT_NOT_RESET;
}

void CsrSdioMasterPowerOff(CsrSdioMasterDevice *device)
{
    /* Not implemented */
}

static int topState;
static CsrUint32 nestCount = 0;

void CsrSdioMasterCallbackInhibitEnter(CsrSdioMasterDevice *device)
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

void CsrSdioMasterCallbackInhibitLeave(CsrSdioMasterDevice *device)
{
    if (--nestCount == 0)
    {
        __asm {
            msr CPSR_cxsf, topState
        };
    }
}
