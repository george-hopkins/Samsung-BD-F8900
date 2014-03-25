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

#include "platform/csr_mmu.h"
#include "platform/csr_s3c6410.h"
#include "platform/csr_interrupt.h"


/******************************/
/********* Parameters *********/
/******************************/

#define SDIO_DEVICE_COUNT       3           /* Number of SDIO devices */
#define SDIO_CMD5_RETRIES       65536       /* Maximum times to retry voltage negotiation */
#define SDIO_HOST_OCR           0x300000    /* Support 3.2V-3.4V */


/**************************************/
/********* Host Configuration *********/
/**************************************/

/* Reset the Host Hardware */
#define SDIO_RESET(ch)      {rSWRST(ch) = 0x7; \
                             while (rSWRST(ch) & 0x7) {}}

#define SDIO_RESET_DATA(ch) {rSWRST(ch) = 1 << 2; \
                             while (rSWRST(ch) & (1 << 2)) {}}

/* Configure SDCLK Frequency (CLKCON) */
#define SDIO_CLOCK_DISABLE(ch)          {rCLKCON(ch) = 0;}
#define SDIO_CLOCK_ENABLE(ch, divider)  {SDIO_CLOCK_DISABLE(ch); \
                                         rCLKCON(ch) = ((divider) << 8) | 1; \
                                         while (!BIT_CHECK(rCLKCON(ch), 1)) {} \
                                         BIT_SET(rCLKCON(ch), 2);}

/* Host Control Register (HOSTCTL) */
#define SDIO_1BIT_MODE_SET(ch)  BIT_CLEAR(rHOSTCTL(ch), 1)
#define SDIO_4BIT_MODE_SET(ch)  BIT_SET(rHOSTCTL(ch), 1)


/*************************************/
/********* Interrupt Control *********/
/*************************************/

/* Normal Interrupt Status Register */
#define SDIO_NINT_ERROR                             (1 << 15)   /* Cannot be cleared here */
#define SDIO_NINT_CARD_INTERRUPT                    (1 << 8)
#define SDIO_NINT_TRANSFER_COMPLETE                 (1 << 1)
#define SDIO_NINT_CMD_COMPLETE                      (1 << 0)
#define SDIO_NINT_STATUS_READ(ch)                   (rNORINTSTS(ch))

#define SDIO_NINT_CLEAR(ch, interrupt)              {rNORINTSTS(ch) = interrupt;}
#define SDIO_NINT_WAIT(ch, interrupt)               {while (rNORINTSTS(ch) & (interrupt)) {}}
#define SDIO_NINT_WAIT_READ(ch, interrupt, status)  {do {status = rNORINTSTS(ch);} while (status & (interrupt));}
#define SDIO_NINT_CLEAR_WAIT(ch, interrupt)         {SDIO_NINT_CLEAR(ch, interrupt); \
                                                     SDIO_NINT_WAIT(ch, interrupt);}

#define SDIO_NINT_ENABLE(ch, interrupt)             {rNORINTSTSEN(ch) |= (interrupt);}
#define SDIO_NINT_DISABLE(ch, interrupt)            {rNORINTSTSEN(ch) &= ~(interrupt);}
#define SDIO_NINT_UNMASK(ch, interrupt)             {rNORINTSIGEN(ch) |= (interrupt);}
#define SDIO_NINT_UNMASK_ATOMIC(ch, interrupt)      INT_ATOMIC_EXECUTE(SDIO_NINT_UNMASK(ch, interrupt))
#define SDIO_NINT_MASK(ch, interrupt)               {rNORINTSIGEN(ch) &= ~(interrupt);}
#define SDIO_NINT_MASK_ATOMIC(ch, interrupt)        INT_ATOMIC_EXECUTE(SDIO_NINT_MASK(ch, interrupt))

#define SDIO_NINT_MASK_GET(ch)                      (rNORINTSIGEN(ch))

/* Error Interrupt Status Register */
#define SDIO_EINT_ADMA_ERROR                        (1 << 9)
#define SDIO_EINT_DATA_ENDBIT                       (1 << 6)
#define SDIO_EINT_DATA_CRC                          (1 << 5)
#define SDIO_EINT_DATA_TIMEOUT                      (1 << 4)
#define SDIO_EINT_CMD_INDEX                         (1 << 3)
#define SDIO_EINT_CMD_ENDBIT                        (1 << 2)
#define SDIO_EINT_CMD_CRC                           (1 << 1)
#define SDIO_EINT_CMD_TIMEOUT                       (1 << 0)
#define SDIO_EINT_STATUS_READ(ch)                   (rERRINTSTS(ch))
#define SDIO_EINT_CLEAR(ch, interrupt)              {rERRINTSTS(ch) = interrupt;}
#define SDIO_EINT_WAIT(ch, interrupt)               {while (rERRINTSTS(ch) & (interrupt)) {}}
#define SDIO_EINT_WAIT_READ(ch, interrupt, status)  {do {status = rERRINTSTS(ch);} while (status & (interrupt));}
#define SDIO_EINT_CLEAR_WAIT(ch, interrupt)         {SDIO_EINT_CLEAR(ch, interrupt); \
                                                     SDIO_EINT_WAIT(ch, interrupt);}

#define SDIO_EINT_ENABLE(ch, interrupt)             {rERRINTSTSEN(ch) |= interrupt;}
#define SDIO_EINT_DISABLE(ch, interrupt)            {rERRINTSTSEN(ch) &= ~(interrupt);}
#define SDIO_EINT_UNMASK(ch, interrupt)             {rERRINTSIGEN(ch) |= interrupt;}
#define SDIO_EINT_MASK(ch, interrupt)               {rERRINTSIGEN(ch) &= ~(interrupt);}

#define SDIO_EINT_MASK_GET(ch)                      (rERRINTSIGEN(ch))


/************************************/
/********* Command Transfer *********/
/************************************/

/* Present State Register */
#define SDIO_STATUS_CMD_INHIBIT_DAT         (1 << 1)
#define SDIO_STATUS_CMD_INHIBIT_CMD         (1 << 0)
#define SDIO_STATUS_GET(ch)                 (rPRNSTS(ch))

/* Options for Command Register (CMDREG) */
#define SDIO_CMDREG_WITHDATA                (1 << 5)
#define SDIO_CMDREG_INDEX_CHECK             (1 << 4)
#define SDIO_CMDREG_CRC_CHECK               (1 << 3)
#define SDIO_CMDREG_RESPONSE                (1 << 1)

/* Command with no response */
#define SDIO_CMD(ch, cmd, arg)              {rARGUMENT(ch) = (arg); \
                                             rCMDREG(ch) = ((cmd) << 8) | SDIO_CMDREG_CRC_CHECK | SDIO_CMDREG_INDEX_CHECK;}
/* Command with response */
#define SDIO_CMD_RESP(ch, cmd, arg)         {rARGUMENT(ch) = (arg); \
                                             rCMDREG(ch) = ((cmd) << 8) | SDIO_CMDREG_CRC_CHECK | SDIO_CMDREG_INDEX_CHECK | SDIO_CMDREG_RESPONSE;}

/* Wait for response */
#define SDIO_CMD_WAIT(ch)                   {while (!(SDIO_NINT_STATUS_READ(ch) & SDIO_NINT_CMD_COMPLETE)) {}}

/* Retrieve response */
#define SDIO_CMD_RESPONSE_GET(ch)           (rRSPREG0(ch))

/* CMD0: GO_IDLE_STATE -> No Response */
#define SDIO_CMD0(ch)                       {SDIO_CMD(ch, 0, 0); \
                                             SDIO_CMD_WAIT(ch);}

/* CMD3: SEND_RELATIVE_ADDR -> R6:
    [31:16] - New Published RCA
    [15] - COM_CRC_ERROR
    [14] - ILLEGAL_COMMAND
    [13] - ERROR */
#define SDIO_CMD3(ch)                       {SDIO_CMD_RESP(ch, 3, 0); \
                                             SDIO_CMD_WAIT(ch);}

/* Decoding of response for CMD3 */
#define SDIO_CMD3_RESPONSE_COM_CRC_ERROR    (1 << 15)
#define SDIO_CMD3_RESPONSE_ILLEGAL_COMMAND  (1 << 14)
#define SDIO_CMD3_RESPONSE_ERROR            (1 << 13)
#define SDIO_CMD3_RESPONSE_ALL_ERRORS       (SDIO_CMD3_RESPONSE_COM_CRC_ERROR | \
                                             SDIO_CMD3_RESPONSE_ILLEGAL_COMMAND | \
                                             SDIO_CMD3_RESPONSE_ERROR)

/* CMD5: IO_SEND_OP_COND -> R4:
    [23:0] - Supported OCR */
#define SDIO_CMD5(ch, ocr)                  {SDIO_CMD_RESP(ch, 5, ocr); \
                                             SDIO_CMD_WAIT(ch);}

/* CMD7: SELECT/DESELECT_CARD -> R1b:
    [31:0] - Card Status:
    [31] - OUT_OF_RANGE
    [23] - COM_CRC_ERROR
    [22] - ILLEGAL_COMMAND
    [19] - ERROR
    [12:9] - IO_CURRENT_STATE */
#define SDIO_CMD7(ch, rca)                  {SDIO_CMD_RESP(ch, 7, (rca) << 16); \
                                             SDIO_CMD_WAIT(ch);}

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
#define SDIO_CMD52(ch, rw, function, address, data)         SDIO_CMD_RESP(ch, 52, (((CsrUint32) (rw)) << 31) | ((function) << 28) | ((address) << 9) | (data))
#define SDIO_CMD52_READ(ch, function, address)              SDIO_CMD52(ch, 0, function, address, 0)
#define SDIO_CMD52_WRITE(ch, function, address, data)       SDIO_CMD52(ch, 1, function, address, data)
#define SDIO_CMD52_RESET_IO(ch)                             SDIO_CMD52_WRITE(ch, 0, 0x06, 1 << 3)

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

/* SDMA System Address */
#define SDIO_SDMA_SYSTEM_ADDRESS_SET(ch, address)   {rSDMASYSAD(ch) = (CsrUint32) address;}
#define SDIO_SDMA_SYSTEM_ADDRESS_RESET(ch)          {rSDMASYSAD(ch) = rSDMASYSAD(ch);}

/* Data Timeout */
#define SDIO_DATA_TIMEOUT_DEFAULT                           0
#define SDIO_DATA_TIMEOUT_SET(ch, timeout)                  REG_SET(rTIMEOUTCON(ch), 22, 0, timeout)

/* Block size and block count registers (BLKSIZE, BLKCNT) */
#define SDIO_BLOCK_SIZE_SET(ch, blockSize)                  {rBLKSIZE(ch) = (blockSize);}
#define SDIO_BLOCK_COUNT_SET(ch, count)                     {rBLKCNT(ch) = (count);}
#define SDIO_BLOCK_SIZE_AND_COUNT_SET(ch, blockSize, count) {rBLKSIZEBLKCNT(ch) = ((count) << 16) | (blockSize);}

/* Options for Transfer Mode Register (TRNMOD) */
#define SDIO_TRNMOD_MULTIBLOCK      (1 << 5)
#define SDIO_TRNMOD_READ            (1 << 4)
#define SDIO_TRNMOD_WRITE           (0 << 4)
#define SDIO_TRNMOD_BLOCKCOUNT      (1 << 1)
#define SDIO_TRNMOD_DMA             (1 << 0)

/* Command with data and response */
#define SDIO_CMD_DATA_RESP(ch, cmd, arg, trnmod)    {rARGUMENT(ch) = (arg); \
                                                     rTRNMODCMDREG(ch) = ((((cmd) << 8) | SDIO_CMDREG_WITHDATA | SDIO_CMDREG_CRC_CHECK | SDIO_CMDREG_INDEX_CHECK | SDIO_CMDREG_RESPONSE) << 16) | (trnmod);}

/* CMD53: IO_RW_EXTENDED -> R5:
    [15] - COM_CRC_ERROR
    [14] - ILLEGAL_COMMAND
    [13:12] - IO_CURRENT_STATE
    [11] - ERROR
    [9] - FUNCTION_NUMBER (invalid)
    [8] - OUT_OF_RANGE */
#define SDIO_CMD53(ch, rw, function, block, address, count)         SDIO_CMD_DATA_RESP(ch, 53, (((CsrUint32) (rw)) << 31) | ((function) << 28) | ((block) << 27) | ((address) << 9) | (count & 0x1FF), SDIO_TRNMOD_DMA | SDIO_TRNMOD_BLOCKCOUNT | ((rw) ? SDIO_TRNMOD_WRITE : SDIO_TRNMOD_READ) | SDIO_TRNMOD_MULTIBLOCK)
#define SDIO_CMD53_BYTEMODE_READ(ch, function, address, length)     SDIO_CMD53(ch, 0, function, 0, address, length)
#define SDIO_CMD53_BLOCKMODE_READ(ch, function, address, blocks)    SDIO_CMD53(ch, 0, function, 1, address, blocks)
#define SDIO_CMD53_BYTEMODE_WRITE(ch, function, address, length)    SDIO_CMD53(ch, 1, function, 0, address, length)
#define SDIO_CMD53_BLOCKMODE_WRITE(ch, function, address, blocks)   SDIO_CMD53(ch, 1, function, 1, address, blocks)

#define SDIO_CMD53_RESPONSE_COM_CRC_ERROR       SDIOM_RESULT_RESPONSE_COM_CRC_ERROR
#define SDIO_CMD53_RESPONSE_ILLEGAL_COMMAND     SDIOM_RESULT_RESPONSE_ILLEGAL_COMMAND
#define SDIO_CMD53_RESPONSE_ERROR               SDIOM_RESULT_RESPONSE_ERROR
#define SDIO_CMD53_RESPONSE_FUNCTION_NUMBER     SDIOM_RESULT_RESPONSE_FUNCTION_NUMBER
#define SDIO_CMD53_RESPONSE_OUT_OF_RANGE        SDIOM_RESULT_RESPONSE_OUT_OF_RANGE
#define SDIO_CMD53_RESPONSE_ALL_ERRORS          (SDIO_CMD53_RESPONSE_COM_CRC_ERROR | \
                                                 SDIO_CMD53_RESPONSE_ILLEGAL_COMMAND | \
                                                 SDIO_CMD53_RESPONSE_ERROR | \
                                                 SDIO_CMD53_RESPONSE_FUNCTION_NUMBER | \
                                                 SDIO_CMD53_RESPONSE_OUT_OF_RANGE)


/*******************************/
/********* Static Data *********/
/*******************************/

static CsrBool initialised = FALSE;

static const CsrBool enable4bitMode = TRUE;

#define SDIO_DIRECTION_READ     0
#define SDIO_DIRECTION_WRITE    1

#define OPERATION_HISR_STACK_SIZE 1024
#define INTERRUPT_HISR_STACK_SIZE 1024

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
    CsrUint8                 direction;
    CsrResult                result;
} CsrSdioMasterDeviceContext;

static CsrSdioMasterDevice sdioDevices[SDIO_DEVICE_COUNT];
static CsrSdioMasterDeviceContext sdioDeviceContexts[SDIO_DEVICE_COUNT];

/* Blocking mechanism */
static NU_EVENT_GROUP eventGroup;


/**********************************/
/********* Implementation *********/
/**********************************/

static void executeOperationDsrCallback(CsrUint8 busIndex)
{
    if (sdioDeviceContexts[busIndex].operationCallback == NULL)
    {
        NU_Set_Events(&eventGroup, 1 << busIndex, NU_OR);
    }
    else
    {
        CsrSdioMasterDsrCallback dsrCallback = sdioDeviceContexts[busIndex].operationDsrCallback;
        if (dsrCallback != NULL)
        {
            dsrCallback(&sdioDevices[busIndex]);
        }
    }
}

static void executeOperationDsrCallback0(void)
{
    executeOperationDsrCallback(0);
}

static void executeOperationDsrCallback1(void)
{
    executeOperationDsrCallback(1);
}

static void executeOperationDsrCallback2(void)
{
    executeOperationDsrCallback(2);
}

static void executeInterruptDsrCallback0(void)
{
    CsrSdioMasterDsrCallback dsrCallback = sdioDeviceContexts[0].interruptDsrCallback;
    if (dsrCallback != NULL)
    {
        dsrCallback(&sdioDevices[0]);
    }
}

static void executeInterruptDsrCallback1(void)
{
    CsrSdioMasterDsrCallback dsrCallback = sdioDeviceContexts[1].interruptDsrCallback;
    if (dsrCallback != NULL)
    {
        dsrCallback(&sdioDevices[1]);
    }
}

static void executeInterruptDsrCallback2(void)
{
    CsrSdioMasterDsrCallback dsrCallback = sdioDeviceContexts[2].interruptDsrCallback;
    if (dsrCallback != NULL)
    {
        dsrCallback(&sdioDevices[2]);
    }
}

static CsrResult cmdErrorStatusCheckAndClear(CsrSdioMasterDevice *device)
{
    CsrUint16 status = SDIO_EINT_STATUS_READ(device->busIndex);
    SDIO_EINT_CLEAR_WAIT(device->busIndex, SDIO_EINT_CMD_INDEX | SDIO_EINT_CMD_ENDBIT | SDIO_EINT_CMD_CRC | SDIO_EINT_CMD_TIMEOUT);
    if (status & (SDIO_EINT_CMD_ENDBIT | SDIO_EINT_CMD_INDEX))
    {
        return CSR_RESULT_FAILURE;
    }
    else if (status & SDIO_EINT_CMD_CRC)
    {
        return CSR_SDIO_MASTER_RESULT_CRC;
    }
    else if (status & SDIO_EINT_CMD_TIMEOUT)
    {
        return CSR_SDIO_MASTER_RESULT_TIMEOUT;
    }
    return CSR_RESULT_SUCCESS;
}

static CsrResult dataErrorStatusCheckAndClear(CsrSdioMasterDevice *device)
{
    CsrUint16 status = SDIO_EINT_STATUS_READ(device->busIndex);
    SDIO_EINT_CLEAR_WAIT(device->busIndex, SDIO_EINT_DATA_ENDBIT | SDIO_EINT_DATA_CRC | SDIO_EINT_DATA_TIMEOUT);
    if (status & SDIO_EINT_DATA_ENDBIT)
    {
        return CSR_RESULT_FAILURE;
    }
    else if (status & SDIO_EINT_DATA_CRC)
    {
        return CSR_SDIO_MASTER_RESULT_CRC;
    }
    else if (status & SDIO_EINT_DATA_TIMEOUT)
    {
        return CSR_SDIO_MASTER_RESULT_TIMEOUT;
    }
    return CSR_RESULT_SUCCESS;
}

/* Interrupt Handler for INT_SDI (host hardware interrupts) */
static void eventInterrupt(CsrSdioMasterDevice *device)
{
    CsrUint16 status;
    CsrSdioMasterDeviceContext *deviceContext = &sdioDeviceContexts[device->busIndex];

    /* Read status and clear bits that are masked (except SDIO_NINT_ERROR) */
    status = SDIO_NINT_STATUS_READ(device->busIndex) & (SDIO_NINT_ERROR | SDIO_NINT_MASK_GET(device->busIndex));

    if (status & SDIO_NINT_CMD_COMPLETE)
    {
        CsrResult result = CSR_RESULT_SUCCESS;
        SDIO_NINT_CLEAR_WAIT(device->busIndex, SDIO_NINT_CMD_COMPLETE);

        if (status & SDIO_NINT_ERROR)
        {
            result = cmdErrorStatusCheckAndClear(device);
        }
        if (deviceContext->direction == SDIO_DIRECTION_READ)
        {
            CsrUint8 *data = (CsrUint8 *) deviceContext->data;
            *data = (CsrUint8) (SDIO_CMD_RESPONSE_GET(device->busIndex) & 0xFF);
        }
        if ((deviceContext->operationCallback != NULL) && deviceContext->operationCallback(device, result))
        {
            NU_Activate_HISR(&deviceContext->operationHisr);
        }
    }
    else if (status & SDIO_NINT_TRANSFER_COMPLETE)
    {
        SDIO_NINT_CLEAR_WAIT(device->busIndex, SDIO_NINT_CMD_COMPLETE | SDIO_NINT_TRANSFER_COMPLETE);
        deviceContext->result = CSR_RESULT_SUCCESS;

        if (status & SDIO_NINT_ERROR)
        {
            deviceContext->result = cmdErrorStatusCheckAndClear(device);
            if (deviceContext->result == CSR_RESULT_SUCCESS)
            {
                deviceContext->result = dataErrorStatusCheckAndClear(device);
                if (deviceContext->result == CSR_SDIO_MASTER_RESULT_TIMEOUT)
                {
                    deviceContext->result = CSR_RESULT_SUCCESS;
                }
            }
            if (deviceContext->result != CSR_RESULT_SUCCESS)
            {
                SDIO_RESET_DATA(device->busIndex);
            }
        }
        if (deviceContext->direction == SDIO_DIRECTION_READ)
        {
            MMUInvalidateDCacheRegion((CsrUint32) deviceContext->data, deviceContext->dataLength);
        }
        if (((deviceContext->operationCallback != NULL) && deviceContext->operationCallback(device, deviceContext->result)) || (deviceContext->operationCallback == NULL))
        {
            NU_Activate_HISR(&deviceContext->operationHisr);
        }
    }
    else if ((status & SDIO_NINT_ERROR) && (SDIO_EINT_STATUS_READ(device->busIndex) & SDIO_EINT_DATA_TIMEOUT))
    {
        deviceContext->result = cmdErrorStatusCheckAndClear(device);
        if (deviceContext->result == CSR_RESULT_SUCCESS)
        {
            deviceContext->result = dataErrorStatusCheckAndClear(device);
        }
        SDIO_RESET_DATA(device->busIndex);
        if (((deviceContext->operationCallback != NULL) && deviceContext->operationCallback(device, deviceContext->result)) || (deviceContext->operationCallback == NULL))
        {
            NU_Activate_HISR(&deviceContext->operationHisr);
        }
    }

    if (status & SDIO_NINT_CARD_INTERRUPT)
    {
        SDIO_NINT_MASK(device->busIndex, SDIO_NINT_CARD_INTERRUPT);

        if ((deviceContext->interruptCallback != NULL) && deviceContext->interruptCallback(device, CSR_RESULT_SUCCESS))
        {
            NU_Activate_HISR(&deviceContext->interruptHisr);
        }
    }

    INT_CLEAR();
}

static VOID eventInterrupt0(INT interrupt)
{
    eventInterrupt(&sdioDevices[0]);
}

static VOID eventInterrupt1(INT interrupt)
{
    eventInterrupt(&sdioDevices[1]);
}

static VOID eventInterrupt2(INT interrupt)
{
    eventInterrupt(&sdioDevices[2]);
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
        NU_Register_LISR(INT_HSMMC0, eventInterrupt0, &previous);
        INT_UNMASK_ATOMIC(INT_HSMMC0);
        NU_Register_LISR(INT_HSMMC1, eventInterrupt1, &previous);
        INT_UNMASK_ATOMIC(INT_HSMMC1);
        NU_Register_LISR(INT_HSMMC2, eventInterrupt2, &previous);
        INT_UNMASK_ATOMIC(INT_HSMMC2);

        /* Create Operation HISR's */
        NU_Create_HISR(&sdioDeviceContexts[0].operationHisr, "SDIOO0", &executeOperationDsrCallback0, 1, sdioDeviceContexts[0].operationHisrStack, OPERATION_HISR_STACK_SIZE);
        NU_Create_HISR(&sdioDeviceContexts[1].operationHisr, "SDIOO1", &executeOperationDsrCallback1, 1, sdioDeviceContexts[1].operationHisrStack, OPERATION_HISR_STACK_SIZE);
        NU_Create_HISR(&sdioDeviceContexts[2].operationHisr, "SDIOO2", &executeOperationDsrCallback2, 1, sdioDeviceContexts[2].operationHisrStack, OPERATION_HISR_STACK_SIZE);

        /* Create Interrupt HISR's */
        NU_Create_HISR(&sdioDeviceContexts[0].interruptHisr, "SDIOI0", &executeInterruptDsrCallback0, 1, sdioDeviceContexts[0].interruptHisrStack, INTERRUPT_HISR_STACK_SIZE);
        NU_Create_HISR(&sdioDeviceContexts[1].interruptHisr, "SDIOI1", &executeInterruptDsrCallback1, 1, sdioDeviceContexts[0].interruptHisrStack, INTERRUPT_HISR_STACK_SIZE);
        NU_Create_HISR(&sdioDeviceContexts[2].interruptHisr, "SDIOI2", &executeInterruptDsrCallback2, 1, sdioDeviceContexts[0].interruptHisrStack, INTERRUPT_HISR_STACK_SIZE);

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
        INT_MASK_ATOMIC(INT_HSMMC0);
        NU_Register_LISR(INT_HSMMC0, NULL, &previous);
        INT_MASK_ATOMIC(INT_HSMMC1);
        NU_Register_LISR(INT_HSMMC1, NULL, &previous);
        INT_MASK_ATOMIC(INT_HSMMC2);
        NU_Register_LISR(INT_HSMMC2, NULL, &previous);

        /* Reset host hardware */
        SDIO_RESET(0);
        SDIO_RESET(1);
        SDIO_RESET(2);

        /* Destroy Operation HISR's */
        NU_Delete_HISR(&sdioDeviceContexts[0].operationHisr);
        NU_Delete_HISR(&sdioDeviceContexts[1].operationHisr);
        NU_Delete_HISR(&sdioDeviceContexts[2].operationHisr);

        /* Destroy Interrupt HISR's */
        NU_Delete_HISR(&sdioDeviceContexts[0].interruptHisr);
        NU_Delete_HISR(&sdioDeviceContexts[1].interruptHisr);
        NU_Delete_HISR(&sdioDeviceContexts[2].interruptHisr);

        initialised = FALSE;
    }
}

void CsrSdioMasterCallbackRegister(CsrSdioMasterDevice *device, CsrSdioMasterCallback interruptCallback, CsrSdioMasterDsrCallback interruptDsrCallback, CsrSdioMasterDsrCallback operationDsrCallback)
{
    sdioDeviceContexts[device->busIndex].interruptCallback = interruptCallback;
    sdioDeviceContexts[device->busIndex].interruptDsrCallback = interruptDsrCallback;
    sdioDeviceContexts[device->busIndex].operationDsrCallback = operationDsrCallback;
}

CsrResult CsrSdioMasterDeviceInitialise(CsrSdioMasterDevice *device)
{
    CsrUint8 data;
    CsrInt32 i;
    CsrUint16 status;
    CsrUint32 response, ocr, rca;

    CsrMemSet(device->blockSize, 0, sizeof(device->blockSize));

    /* Reset the corresponding host controller */
    SDIO_RESET(device->busIndex);

    /* Set the base clock source to MPLL (100MHz) */
    REG_SET(rCONTROL2(device->busIndex), 5, 4, 2);

    /* Set clock frequency to 390625Hz */
    SDIO_CLOCK_ENABLE(device->busIndex, 0x80);

    /* Enable status bits for CMD52 */
    SDIO_NINT_ENABLE(device->busIndex, SDIO_NINT_CMD_COMPLETE);
    SDIO_EINT_ENABLE(device->busIndex, SDIO_EINT_CMD_TIMEOUT);

    /* Reset SDIO part of the device if previously initialised - might not produce reply, so do not check */
    SDIO_CMD52_RESET_IO(device->busIndex);
    SDIO_CMD_WAIT(device->busIndex);
    SDIO_NINT_CLEAR_WAIT(device->busIndex, SDIO_NINT_CMD_COMPLETE);
    SDIO_EINT_CLEAR_WAIT(device->busIndex, SDIO_EINT_CMD_TIMEOUT);

    /* Reset combo card - does not produce a reply */
    SDIO_CMD0(device->busIndex);
    SDIO_NINT_CLEAR_WAIT(device->busIndex, SDIO_NINT_CMD_COMPLETE);
    SDIO_EINT_CLEAR_WAIT(device->busIndex, SDIO_EINT_CMD_TIMEOUT);

    /* Inquire about supported voltages */
    SDIO_CMD5(device->busIndex, 0);
    status = SDIO_EINT_STATUS_READ(device->busIndex);
    SDIO_NINT_CLEAR_WAIT(device->busIndex, SDIO_NINT_CMD_COMPLETE);
    if (status & SDIO_EINT_CMD_TIMEOUT)
    {
        SDIO_EINT_CLEAR_WAIT(device->busIndex, SDIO_EINT_CMD_TIMEOUT);
        return CSR_RESULT_FAILURE;
    }
    response = SDIO_CMD_RESPONSE_GET(device->busIndex);
    ocr = response & 0xFFFFFF;                   /* Supported Voltages */
    device->functions = (response >> 28) & 0x7;  /* Number of Card Functions */

    /* Check if host supports the voltages accepted by the card */
    if (!(ocr & SDIO_HOST_OCR))
    {
        return CSR_RESULT_FAILURE;
    }

    /* Specify operational voltage, and poll until ready (limited by SDIO_CMD5_RETRIES) */
    for (i = 0; i < SDIO_CMD5_RETRIES; i++)
    {
        SDIO_CMD5(device->busIndex, SDIO_HOST_OCR);
        status = SDIO_EINT_STATUS_READ(device->busIndex);
        SDIO_NINT_CLEAR_WAIT(device->busIndex, SDIO_NINT_CMD_COMPLETE);
        if (status & SDIO_EINT_CMD_TIMEOUT)
        {
            SDIO_EINT_CLEAR_WAIT(device->busIndex, SDIO_EINT_CMD_TIMEOUT);
            return CSR_RESULT_FAILURE;
        }
        if ((SDIO_CMD_RESPONSE_GET(device->busIndex) >> 31) & 0x1)
        {
            break;
        }
    }
    if (i == SDIO_CMD5_RETRIES)
    {
        /* Voltage Negotiation Failed */
        return CSR_RESULT_FAILURE;
    }

    /* Enable all relevant error status bits */
    SDIO_EINT_ENABLE(device->busIndex, SDIO_EINT_DATA_ENDBIT | SDIO_EINT_DATA_CRC | SDIO_EINT_DATA_TIMEOUT | SDIO_EINT_CMD_INDEX | SDIO_EINT_CMD_ENDBIT | SDIO_EINT_CMD_CRC | SDIO_EINT_CMD_TIMEOUT);

    /* Assign Address */
    SDIO_CMD3(device->busIndex);
    status = SDIO_EINT_STATUS_READ(device->busIndex);
    SDIO_NINT_CLEAR_WAIT(device->busIndex, SDIO_NINT_CMD_COMPLETE);
    if (status & (SDIO_EINT_CMD_TIMEOUT | SDIO_EINT_CMD_CRC | SDIO_EINT_CMD_INDEX))
    {
        SDIO_EINT_CLEAR_WAIT(device->busIndex, SDIO_EINT_CMD_TIMEOUT | SDIO_EINT_CMD_CRC | SDIO_EINT_CMD_INDEX);
        return CSR_RESULT_FAILURE;
    }
    else
    {
        response = SDIO_CMD_RESPONSE_GET(device->busIndex);
        if (response & SDIO_CMD3_RESPONSE_ALL_ERRORS)
        {
            /* Card Error (Possible CRC error in command) */
            return CSR_RESULT_FAILURE;
        }
        rca = (response >> 16) & 0xFFFF;
    }

    /* Set clock frequency to 25MHz */
    SDIO_CLOCK_ENABLE(device->busIndex, 0x02);

    /* Select card */
    SDIO_CMD7(device->busIndex, rca);
    status = SDIO_EINT_STATUS_READ(device->busIndex);
    SDIO_NINT_CLEAR_WAIT(device->busIndex, SDIO_NINT_CMD_COMPLETE);
    if (status & (SDIO_EINT_CMD_TIMEOUT | SDIO_EINT_CMD_CRC | SDIO_EINT_CMD_INDEX))
    {
        SDIO_EINT_CLEAR_WAIT(device->busIndex, SDIO_EINT_CMD_TIMEOUT | SDIO_EINT_CMD_CRC | SDIO_EINT_CMD_INDEX);
        return CSR_RESULT_FAILURE;
    }
    else
    {
        response = SDIO_CMD_RESPONSE_GET(device->busIndex);
        if (response & SDIO_CMD7_RESPONSE_ALL_ERRORS)
        {
            /* Card Error (Possible CRC error in command) */
            return CSR_RESULT_FAILURE;
        }
    }

    /* Enable interrupt generation */
    SDIO_NINT_ENABLE(device->busIndex, SDIO_NINT_TRANSFER_COMPLETE | SDIO_NINT_CARD_INTERRUPT);
    SDIO_NINT_UNMASK(device->busIndex, SDIO_NINT_TRANSFER_COMPLETE | SDIO_NINT_CARD_INTERRUPT);
    SDIO_EINT_UNMASK(device->busIndex, SDIO_EINT_DATA_TIMEOUT);

    /* Set bus width */
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
                SDIO_4BIT_MODE_SET(device->busIndex);
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
    CsrUint16 status;

    /* Issue Command */
    SDIO_NINT_MASK_ATOMIC(device->busIndex, SDIO_NINT_CMD_COMPLETE);
    SDIO_CMD52_READ(device->busIndex, function, address);

    /* Wait for response */
    SDIO_CMD_WAIT(device->busIndex);

    /* Acknowledge and read status */
    SDIO_NINT_CLEAR(device->busIndex, SDIO_NINT_CMD_COMPLETE);
    SDIO_NINT_WAIT_READ(device->busIndex, SDIO_NINT_CMD_COMPLETE, status);

    /* Check for errors in status */
    if (status & SDIO_NINT_ERROR)
    {
        return cmdErrorStatusCheckAndClear(device);
    }

    /* Read out the data */
    *data = (CsrUint8) (SDIO_CMD_RESPONSE_GET(device->busIndex) & 0xFF);

    return CSR_RESULT_SUCCESS;
}

void CsrSdioMasterRead8Async(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrSdioMasterCallback operationCallback)
{
    CsrSdioMasterDeviceContext *deviceContext = &sdioDeviceContexts[device->busIndex];

    /* Setup state */
    deviceContext->direction = SDIO_DIRECTION_READ;
    deviceContext->data = data;
    deviceContext->operationCallback = operationCallback;

    /* Issue Command */
    SDIO_NINT_UNMASK_ATOMIC(device->busIndex, SDIO_NINT_CMD_COMPLETE);
    SDIO_CMD52_READ(device->busIndex, function, address);
}

CsrResult CsrSdioMasterWrite8(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, CsrUint8 data)
{
    CsrUint16 status;

    /* Issue Command */
    SDIO_NINT_MASK_ATOMIC(device->busIndex, SDIO_NINT_CMD_COMPLETE);
    SDIO_CMD52_WRITE(device->busIndex, function, address, data);

    /* Wait for response */
    SDIO_CMD_WAIT(device->busIndex);

    /* Acknowledge and read status */
    SDIO_NINT_CLEAR(device->busIndex, SDIO_NINT_CMD_COMPLETE);
    SDIO_NINT_WAIT_READ(device->busIndex, SDIO_NINT_CMD_COMPLETE, status);

    /* Check for errors */
    if (status & SDIO_NINT_ERROR)
    {
        return cmdErrorStatusCheckAndClear(device);
    }

    return CSR_RESULT_SUCCESS;
}

void CsrSdioMasterWrite8Async(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, CsrUint8 data, CsrSdioMasterCallback operationCallback)
{
    CsrSdioMasterDeviceContext *deviceContext = &sdioDeviceContexts[device->busIndex];

    /* Setup state */
    deviceContext->direction = SDIO_DIRECTION_WRITE;
    deviceContext->operationCallback = operationCallback;

    /* Issue Command */
    SDIO_NINT_UNMASK_ATOMIC(device->busIndex, SDIO_NINT_CMD_COMPLETE);
    SDIO_CMD52_WRITE(device->busIndex, function, address, data);
}

static CsrResult blockModeRead(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrUint16 blocks, CsrSdioMasterCallback operationCallback)
{
    CsrSdioMasterDeviceContext *deviceContext = &sdioDeviceContexts[device->busIndex];

    /* Setup state */
    deviceContext->direction = SDIO_DIRECTION_READ;
    deviceContext->operationCallback = operationCallback;
    deviceContext->data = data;
    deviceContext->dataLength = blocks * device->blockSize[function];

    /* Writeback from cache to main memory before starting the DMA */
    MMUCleanDCacheRegion((CsrUint32) data, deviceContext->dataLength);
    MMUDrainWriteBuffer();

    /* Setup for DMA */
    SDIO_SDMA_SYSTEM_ADDRESS_SET(device->busIndex, data);
    SDIO_BLOCK_SIZE_AND_COUNT_SET(device->busIndex, device->blockSize[function], blocks);

    /* Issue Command */
    SDIO_NINT_MASK_ATOMIC(device->busIndex, SDIO_NINT_CMD_COMPLETE);
    SDIO_CMD53_BLOCKMODE_READ(device->busIndex, function, address, blocks);

    if (operationCallback == NULL)
    {
        /* Wait until command is complete */
        UNSIGNED eventBits;
        NU_Retrieve_Events(&eventGroup, 1 << device->busIndex, NU_OR_CONSUME, &eventBits, NU_SUSPEND);
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
    CsrSdioMasterDeviceContext *deviceContext = &sdioDeviceContexts[device->busIndex];

    /* Setup state */
    deviceContext->direction = SDIO_DIRECTION_WRITE;
    deviceContext->operationCallback = operationCallback;

    /* Writeback from cache to main memory before starting the DMA */
    MMUCleanDCacheRegion((CsrUint32) data, blocks * device->blockSize[function]);
    MMUDrainWriteBuffer();

    /* Setup for DMA */
    SDIO_SDMA_SYSTEM_ADDRESS_SET(device->busIndex, data);
    SDIO_BLOCK_SIZE_AND_COUNT_SET(device->busIndex, device->blockSize[function], blocks);

    /* Issue Command */
    SDIO_NINT_MASK_ATOMIC(device->busIndex, SDIO_NINT_CMD_COMPLETE);
    SDIO_CMD53_BLOCKMODE_WRITE(device->busIndex, function, address, blocks);

    if (operationCallback == NULL)
    {
        /* Wait until command is complete */
        UNSIGNED eventBits;
        NU_Retrieve_Events(&eventGroup, 1 << device->busIndex, NU_OR_CONSUME, &eventBits, NU_SUSPEND);
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
    CsrSdioMasterDeviceContext *deviceContext = &sdioDeviceContexts[device->busIndex];

    /* Setup state */
    deviceContext->direction = SDIO_DIRECTION_READ;
    deviceContext->operationCallback = operationCallback;
    deviceContext->data = data;
    deviceContext->dataLength = length;

    /* Writeback from cache to main memory before starting the DMA */
    MMUCleanDCacheRegion((CsrUint32) data, length);
    MMUDrainWriteBuffer();

    /* Setup for DMA */
    SDIO_SDMA_SYSTEM_ADDRESS_SET(device->busIndex, data);
    SDIO_BLOCK_SIZE_AND_COUNT_SET(device->busIndex, length, 1);

    /* Issue Command */
    SDIO_NINT_MASK_ATOMIC(device->busIndex, SDIO_NINT_CMD_COMPLETE);
    SDIO_CMD53_BYTEMODE_READ(device->busIndex, function, address, length);

    if (operationCallback == NULL)
    {
        /* Wait until command is complete */
        UNSIGNED eventBits;
        NU_Retrieve_Events(&eventGroup, 1 << device->busIndex, NU_OR_CONSUME, &eventBits, NU_SUSPEND);
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
    CsrSdioMasterDeviceContext *deviceContext = &sdioDeviceContexts[device->busIndex];

    /* Setup state */
    deviceContext->direction = SDIO_DIRECTION_WRITE;
    deviceContext->operationCallback = operationCallback;

    /* Writeback from cache to main memory before starting the DMA */
    MMUCleanDCacheRegion((CsrUint32) data, length);
    MMUDrainWriteBuffer();

    /* Setup for DMA */
    SDIO_SDMA_SYSTEM_ADDRESS_SET(device->busIndex, data);
    SDIO_BLOCK_SIZE_AND_COUNT_SET(device->busIndex, length, 1);

    /* Issue Command */
    SDIO_NINT_MASK_ATOMIC(device->busIndex, SDIO_NINT_CMD_COMPLETE);
    SDIO_CMD53_BYTEMODE_WRITE(device->busIndex, function, address, length);

    if (operationCallback == NULL)
    {
        /* Wait until command is complete */
        UNSIGNED eventBits;
        NU_Retrieve_Events(&eventGroup, 1 << device->busIndex, NU_OR_CONSUME, &eventBits, NU_SUSPEND);
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
    /* Clear */
    SDIO_NINT_CLEAR(device->busIndex, SDIO_NINT_CARD_INTERRUPT);

    /* Unmask */
    SDIO_NINT_UNMASK_ATOMIC(device->busIndex, SDIO_NINT_CARD_INTERRUPT);
}

CsrResult CsrSdioMasterBlockSizeSet(CsrSdioMasterDevice *device, CsrUint8 function, CsrUint16 blockSize)
{
    CsrResult result;
    CsrUint32 functionBaseAddress;

    functionBaseAddress = (function & 0x7) * 0x100;

    /* Disqualify invalid values */
    if (blockSize > 512)
    {
        blockSize = 512;
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
        CsrUint32 divider;

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
        divider = (data & 2) ? 1 : 2;
        while (((100000000 / (divider * 2)) > *frequency) && (divider < 0x80))
        {
            divider *= 2;
        }
        *frequency = 100000000 / (divider * 2);

        SDIO_CLOCK_ENABLE(device->busIndex, divider);
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
