/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_result.h"
#include "csr_util.h"
#include "csr_pmem.h"
#include "csr_log_text_2.h"

#ifdef CSR_SDIO_USE_CSPI

#include "csr_spi_master.h"
#include "csr_sdio_cspi.h"
#include "csr_sdio_common.h"


/******************************/
/********* Parameters *********/
/******************************/

#define SSPI_PRECLOCK       10          /* Number of clock cycles to apply before CMD0/CMD5 (unit of 8 clocks) */
#define SSPI_CMD0_RETRIES   256         /* Maximum number of times (~ 100ms) to retry device reset */
#define SSPI_CMD5_RETRIES   256         /* Maximum number of times (~ 100ms) to retry voltage negotiation */
#define SSPI_HOST_OCR       0x300000    /* Support 3.2V-3.4V */

/* SDIO-SPI bus timings in units of 8 clocks */
#define SSPI_CMD_LEN                             6 /* Length of command token */
#define SSPI_CRC_LEN                             2 /* Length of CRC */
#define SSPI_N_RC_MIN                            1 /* Minimum cycles to keep the clock active after response */
#define SSPI_N_CR_MAX                            8 /* Maximum time between command and response */

#define CSPI_PADDING_REGISTER    2   /* Number of padding bytes for byte oriented transfers */
#define CSPI_PADDING_BURST       2   /* Number of padding bytes for burst transfers */
#define CSPI_PADDING             ((CSPI_PADDING_BURST << 4) | (CSPI_PADDING_REGISTER))

#define CSPI_SPI_INITIAL_FREQUENCY  400000


/*********************************/
/********* Chip Specific *********/
/*********************************/

/* Common */
#define CSPI_F0_IO_ENABLE               0x02                /* Address of IO_ENABLE register in function 0 */
#define CSPI_F0_INT_ENABLE              0x04                /* Address of INT_ENABLE register in function 0 */
#define CSPI_F0_BUS_INTERFACE_CONTROL   0x07                /* Address of BUS_INTERFACE_CONTROL register in function 0 */
#define CSPI_F0_CSPI_PADDING            0xF9                /* Address of CSPI_PADDING register in function 0 */
#define CSPI_F0_CSPI_STATUS             0xF8                /* Address of CSPI_STATUS register in function 0 */
#define CSPI_F0_CSPI_MODE               0xF7                /* Address of CSPI_MODE register in function 0 */

/* Default */
#define CSPI_SDIO_MODE_DEF              (0xF935 << 1)       /* Address of SDIO_MODE register in Generic Function */
#define CSPI_SDIO_MODE_CSPI_EN_DEF      (1 << 6)            /* CSPI Enable Bit in the SDIO_MODE register */

/* BlueCore 6 */
#define CSPI_DEVICE_ID_BC6              0x0004              /* Device ID */
#define CSPI_SDIO_MODE_BC6              (0xFC26 << 1)       /* Address of SDIO_MODE register in Generic Function */
#define CSPI_SDIO_MODE_CSPI_EN_BC6      (1 << 6)            /* CSPI Enable Bit in the SDIO_MODE register */


/********************************/
/********* Context Data *********/
/********************************/

#define STATE_IDLE          0
#define STATE_READ8_CMD     1
#define STATE_READ8_DAT     2
#define STATE_WRITE8        3
#define STATE_READ16_CMD    4
#define STATE_READ16_DAT    5
#define STATE_WRITE16       6
#define STATE_READ_CMD      7
#define STATE_READ_DAT      8
#define STATE_WRITE_CMD     9
#define STATE_WRITE_DAT     10
#define STATE_FINAL         11

typedef struct
{
    /* Transfer State */
    CsrUint8 state;

    /* Operation State data */
    CsrUint8  byteBuffer[CSRMAX(CSRMAX(1 + CSPI_PADDING_BURST, 4 + 1 + CSPI_PADDING_REGISTER), 6)];
    CsrUint32 length;
    void     *data;

    CsrSpiMasterCallback eventOperationCallback;
} CsrSdioCspiContext;


/**********************************/
/********* Implementation *********/
/**********************************/

static CsrBool eventOperationPartial(CsrSpiMasterDevice *spiMasterDevice)
{
    CsrSdioDevice *sdioDevice = spiMasterDevice->driverData;
    CsrSdioCspiContext *cspiContext = sdioDevice->context;

    switch (cspiContext->state)
    {
        case STATE_READ8_CMD:
        {
            cspiContext->state = STATE_READ8_DAT;
            CsrSpiMasterReadAsync(spiMasterDevice, cspiContext->byteBuffer, 3 + CSPI_PADDING_REGISTER, eventOperationPartial);
            break;
        }
        case STATE_READ8_DAT:
        {
            CsrUint8 *data = cspiContext->data;
            *data = cspiContext->byteBuffer[1 + CSPI_PADDING_REGISTER];
            CsrSpiMasterChipSelectNegate(spiMasterDevice);
            cspiContext->state = STATE_IDLE;
            return cspiContext->eventOperationCallback(spiMasterDevice);
        }
        case STATE_WRITE8:
        {
            CsrSpiMasterChipSelectNegate(spiMasterDevice);
            cspiContext->state = STATE_IDLE;
            return cspiContext->eventOperationCallback(spiMasterDevice);
        }
        case STATE_READ16_CMD:
        {
            cspiContext->state = STATE_READ16_DAT;
            CsrSpiMasterReadAsync(spiMasterDevice, cspiContext->byteBuffer, 3 + CSPI_PADDING_REGISTER, eventOperationPartial);
            break;
        }
        case STATE_READ16_DAT:
        {
            CsrUint16 *data = cspiContext->data;
            *data = (cspiContext->byteBuffer[1 + CSPI_PADDING_REGISTER] << 8) | cspiContext->byteBuffer[2 + CSPI_PADDING_REGISTER];
            CsrSpiMasterChipSelectNegate(spiMasterDevice);
            cspiContext->state = STATE_IDLE;
            return cspiContext->eventOperationCallback(spiMasterDevice);
        }
        case STATE_WRITE16:
        {
            CsrSpiMasterChipSelectNegate(spiMasterDevice);
            cspiContext->state = STATE_IDLE;
            return cspiContext->eventOperationCallback(spiMasterDevice);
        }
        case STATE_READ_CMD:
        {
            cspiContext->state = STATE_READ_DAT;
            CsrSpiMasterReadAsync(spiMasterDevice, (CsrUint8 *) cspiContext->data, cspiContext->length, eventOperationPartial);
            break;
        }
        case STATE_READ_DAT:
        {
            CsrSpiMasterChipSelectNegate(spiMasterDevice);
            cspiContext->state = STATE_IDLE;
            return cspiContext->eventOperationCallback(spiMasterDevice);
        }
        case STATE_WRITE_CMD:
        {
            cspiContext->state = STATE_WRITE_DAT;
            CsrSpiMasterWriteAsync(spiMasterDevice, cspiContext->data, cspiContext->length, eventOperationPartial);
            break;
        }
        case STATE_WRITE_DAT:
        {
            CsrSpiMasterChipSelectNegate(spiMasterDevice);
            cspiContext->state = STATE_IDLE;
            return cspiContext->eventOperationCallback(spiMasterDevice);
        }
        default:
        {
            break;
        }
    }

    return FALSE;
}

static CsrResult sdioSpiCmd(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 command, CsrUint32 argument, CsrUint32 *response)
{
    CsrUint8 byteBuffer[CSRMAX(SSPI_CMD_LEN, SSPI_PRECLOCK)];
    CsrUint32 i;

    if (command != 52)
    {
        /* Clock some cycles to allow the chip to become ready */
        CsrSpiMasterRead(spiMasterDevice, byteBuffer, SSPI_PRECLOCK);
    }

    byteBuffer[0] = (CsrUint8) (0x40 | command); /* START bit | Direction bit | Command index */
    byteBuffer[1] = (CsrUint8) ((argument >> 24) & 0xFF);
    byteBuffer[2] = (CsrUint8) ((argument >> 16) & 0xFF);
    byteBuffer[3] = (CsrUint8) ((argument >> 8) & 0xFF);
    byteBuffer[4] = (CsrUint8) ((argument >> 0) & 0xFF);
    byteBuffer[5] = (CsrUint8) (0x94 | 0x01); /* Precalculated CRC for CMD0 (CRC is ignored for other commands) | END bit */

    CsrSpiMasterChipSelectAssert(spiMasterDevice);

    /* Transmit Command */
    CsrSpiMasterWrite(spiMasterDevice, byteBuffer, SSPI_CMD_LEN);

    /* Wait for Response */
    CsrSpiMasterRead(spiMasterDevice, byteBuffer, 1);
    for (i = 0; i < SSPI_N_CR_MAX; i++)
    {
        CsrSpiMasterRead(spiMasterDevice, byteBuffer, 1);
        if (*byteBuffer != 0xFF)
        {
            /* Response found */
            if (command == 5)
            {
                /* Read in last 4 byte of R4 response */
                CsrSpiMasterRead(spiMasterDevice, byteBuffer, 4);
                if (response != NULL)
                {
                    *response = (byteBuffer[0] << 24) | (byteBuffer[1] << 16) | (byteBuffer[2] << 8) | (byteBuffer[3] << 0);
                }
            }
            else if (command == 52)
            {
                /* Read in data byte */
                CsrSpiMasterRead(spiMasterDevice, byteBuffer, 1);
                if (response != NULL)
                {
                    *response = (CsrUint32) * byteBuffer;
                }
            }
            CsrSpiMasterChipSelectNegate(spiMasterDevice);
            CsrSpiMasterRead(spiMasterDevice, byteBuffer, SSPI_N_RC_MIN);
            return CSR_RESULT_SUCCESS;
        }
    }

    CsrSpiMasterChipSelectNegate(spiMasterDevice);

    return CSR_RESULT_FAILURE;
}

/* Defines for various SDIO-SPI CMD52 commands */
#define SDIO_SPI_CMD52(spiMasterDevice, rw, function, raw, address, data, resp) sdioSpiCmd((spiMasterDevice), 52, (((CsrUint32) (rw & 0x1) << 31) | ((function) & 0x7) << 28) | (((raw) & 0x1) << 27) | (((address) & 0x1FFFF) << 9) | ((data) & 0xFF), (resp))
#define SDIO_SPI_CMD52_READ(spiMasterDevice, function, address, resp)           SDIO_SPI_CMD52((spiMasterDevice), 0, (function), 0, (address), 0, (resp))
#define SDIO_SPI_CMD52_WRITE(spiMasterDevice, function, address, data)          SDIO_SPI_CMD52((spiMasterDevice), 1, (function), 0, (address), (data), NULL)

/* Read byte in SDIO-SPI mode */
static CsrResult sdioSpiByteRead(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint8 *data)
{
    CsrResult result;
    CsrUint32 response;

    result = SDIO_SPI_CMD52_READ(spiMasterDevice, function, address, &response);
    *data = (CsrUint8) (response & 0xFF);

    return result;
}

/* Write byte in SDIO-SPI mode */
static CsrResult sdioSpiByteWrite(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint8 data)
{
    return SDIO_SPI_CMD52_WRITE(spiMasterDevice, function, address, data);
}

void CsrSdioCspiRead8Async(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint8 *data,
    CsrSpiMasterCallback operationCallback)
{
    CsrSdioDevice *sdioDevice = spiMasterDevice->driverData;
    CsrSdioCspiContext *cspiContext = sdioDevice->context;

    cspiContext->byteBuffer[0] = (CsrUint8) (0x10 | (function & 0x7)); /* Command: STOP:nREG/BURST:WRITE:READ:0:F2:F1:F0 */
    cspiContext->byteBuffer[1] = (CsrUint8) ((address >> 16) & 0xFF);
    cspiContext->byteBuffer[2] = (CsrUint8) ((address >> 8) & 0xFF);
    cspiContext->byteBuffer[3] = (CsrUint8) ((address >> 0) & 0xFF);

    CsrSpiMasterChipSelectAssert(spiMasterDevice);

    if (operationCallback != NULL)
    {
        cspiContext->state = STATE_READ8_CMD;
        cspiContext->eventOperationCallback = operationCallback;
        cspiContext->data = (void *) data;
        CsrSpiMasterWriteAsync(spiMasterDevice, cspiContext->byteBuffer, 4, eventOperationPartial);
    }
    else
    {
        CsrSpiMasterWrite(spiMasterDevice, cspiContext->byteBuffer, 4);
        CsrSpiMasterRead(spiMasterDevice, cspiContext->byteBuffer, 3 + CSPI_PADDING_REGISTER);
        CsrSpiMasterChipSelectNegate(spiMasterDevice);

        *data = cspiContext->byteBuffer[1 + CSPI_PADDING_REGISTER];
    }
}

void CsrSdioCspiRead8(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint8 *data)
{
    CsrSdioCspiRead8Async(spiMasterDevice, function, address, data, NULL);
}

static CsrResult cspiByteRead(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint8 *data)
{
    CsrSdioDevice *sdioDevice = spiMasterDevice->driverData;
    CsrSdioCspiContext *cspiContext = sdioDevice->context;

    CsrSdioCspiRead8Async(spiMasterDevice, function, address, data, NULL);

    /* Check error byte */
    if (cspiContext->byteBuffer[CSPI_PADDING_REGISTER] != 0)
    {
        return CSR_RESULT_FAILURE;
    }

    return CSR_RESULT_SUCCESS;
}

void CsrSdioCspiWrite8Async(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint8 data,
    CsrSpiMasterCallback operationCallback)
{
    CsrSdioDevice *sdioDevice = spiMasterDevice->driverData;
    CsrSdioCspiContext *cspiContext = sdioDevice->context;

    cspiContext->byteBuffer[0] = (CsrUint8) (0x20 | (function & 0x7)); /* Command: STOP:nREG/BURST:WRITE:READ:0:F2:F1:F0 */
    cspiContext->byteBuffer[1] = (CsrUint8) ((address >> 16) & 0xFF);
    cspiContext->byteBuffer[2] = (CsrUint8) ((address >> 8) & 0xFF);
    cspiContext->byteBuffer[3] = (CsrUint8) ((address >> 0) & 0xFF);
    cspiContext->byteBuffer[4] = data;
    cspiContext->byteBuffer[5] = 0xFF;

    CsrSpiMasterChipSelectAssert(spiMasterDevice);

    if (operationCallback != NULL)
    {
        cspiContext->state = STATE_WRITE8;
        cspiContext->eventOperationCallback = operationCallback;
        CsrSpiMasterWriteAsync(spiMasterDevice, cspiContext->byteBuffer, 6, eventOperationPartial);
    }
    else
    {
        CsrSpiMasterWrite(spiMasterDevice, cspiContext->byteBuffer, 6);
        CsrSpiMasterChipSelectNegate(spiMasterDevice);
    }
}

void CsrSdioCspiWrite8(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint8 data)
{
    CsrSdioCspiWrite8Async(spiMasterDevice, function, address, data, NULL);
}

void CsrSdioCspiRead16Async(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint16 *data,
    CsrSpiMasterCallback operationCallback)
{
    CsrSdioDevice *sdioDevice = spiMasterDevice->driverData;
    CsrSdioCspiContext *cspiContext = sdioDevice->context;

    cspiContext->byteBuffer[0] = (CsrUint8) (0x10 | (function & 0x7)); /* Command: STOP:nREG/BURST:WRITE:READ:0:F2:F1:F0 */
    cspiContext->byteBuffer[1] = (CsrUint8) ((address >> 16) & 0xFF);
    cspiContext->byteBuffer[2] = (CsrUint8) ((address >> 8) & 0xFF);
    cspiContext->byteBuffer[3] = (CsrUint8) ((address >> 0) & 0xFF);

    CsrSpiMasterChipSelectAssert(spiMasterDevice);

    if (operationCallback != NULL)
    {
        cspiContext->state = STATE_READ16_CMD;
        cspiContext->eventOperationCallback = operationCallback;
        cspiContext->data = (void *) data;
        CsrSpiMasterWriteAsync(spiMasterDevice, cspiContext->byteBuffer, 4, eventOperationPartial);
    }
    else
    {
        CsrSpiMasterWrite(spiMasterDevice, cspiContext->byteBuffer, 4);
        CsrSpiMasterRead(spiMasterDevice, cspiContext->byteBuffer, 3 + CSPI_PADDING_REGISTER);
        CsrSpiMasterChipSelectNegate(spiMasterDevice);

        *data = (cspiContext->byteBuffer[1 + CSPI_PADDING_REGISTER] << 8) |
                cspiContext->byteBuffer[2 + CSPI_PADDING_REGISTER];
    }
}

void CsrSdioCspiRead16(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint16 *data)
{
    CsrSdioCspiRead16Async(spiMasterDevice, function, address, data, NULL);
}

void CsrSdioCspiWrite16Async(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint16 data,
    CsrSpiMasterCallback operationCallback)
{
    CsrSdioDevice *sdioDevice = spiMasterDevice->driverData;
    CsrSdioCspiContext *cspiContext = sdioDevice->context;

    cspiContext->byteBuffer[0] = (CsrUint8) (0x20 | (function & 0x7)); /* Command: STOP:nREG/BURST:WRITE:READ:0:F2:F1:F0 */
    cspiContext->byteBuffer[1] = (CsrUint8) ((address >> 16) & 0xFF);
    cspiContext->byteBuffer[2] = (CsrUint8) ((address >> 8) & 0xFF);
    cspiContext->byteBuffer[3] = (CsrUint8) ((address >> 0) & 0xFF);
    cspiContext->byteBuffer[4] = (CsrUint8) ((data >> 8) & 0xFF);
    cspiContext->byteBuffer[5] = (CsrUint8) (data & 0xFF);

    CsrSpiMasterChipSelectAssert(spiMasterDevice);

    if (operationCallback != NULL)
    {
        cspiContext->state = STATE_WRITE16;
        cspiContext->eventOperationCallback = operationCallback;
        CsrSpiMasterWriteAsync(spiMasterDevice, cspiContext->byteBuffer, 6, eventOperationPartial);
    }
    else
    {
        CsrSpiMasterWrite(spiMasterDevice, cspiContext->byteBuffer, 6);
        CsrSpiMasterChipSelectNegate(spiMasterDevice);
    }
}

void CsrSdioCspiWrite16(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint16 data)
{
    CsrSdioCspiWrite16Async(spiMasterDevice, function, address, data, NULL);
}

void CsrSdioCspiReadAsync(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrUint32 length,
    CsrSpiMasterCallback operationCallback)
{
    CsrSdioDevice *sdioDevice = spiMasterDevice->driverData;
    CsrSdioCspiContext *cspiContext = sdioDevice->context;

    cspiContext->byteBuffer[0] = (CsrUint8) (0x50 | (function & 0x7));
    cspiContext->byteBuffer[1] = (CsrUint8) ((address >> 16) & 0xFF);
    cspiContext->byteBuffer[2] = (CsrUint8) ((address >> 8) & 0xFF);
    cspiContext->byteBuffer[3] = (CsrUint8) ((address >> 0) & 0xFF);
    CsrMemSet(&cspiContext->byteBuffer[4], 0xFF, 1 + CSPI_PADDING_BURST);

    CsrSpiMasterChipSelectAssert(spiMasterDevice);

    if (operationCallback != NULL)
    {
        cspiContext->state = STATE_READ_CMD;
        cspiContext->eventOperationCallback = operationCallback;
        cspiContext->length = length;
        cspiContext->data = (void *) data;
        CsrSpiMasterWriteAsync(spiMasterDevice,
            cspiContext->byteBuffer,
            4 + 1 + CSPI_PADDING_BURST,
            eventOperationPartial);
    }
    else
    {
        CsrSpiMasterWrite(spiMasterDevice, cspiContext->byteBuffer, 4 + 1 + CSPI_PADDING_BURST);
        CsrSpiMasterRead(spiMasterDevice, data, length);
        CsrSpiMasterChipSelectNegate(spiMasterDevice);
    }
}

void CsrSdioCspiRead(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrUint32 length)
{
    CsrSdioCspiReadAsync(spiMasterDevice, function, address, data, length, NULL);
}

void CsrSdioCspiWriteAsync(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 function, CsrUint32 address, const CsrUint8 *data, CsrUint32 length,
    CsrSpiMasterCallback operationCallback)
{
    CsrSdioDevice *sdioDevice = spiMasterDevice->driverData;
    CsrSdioCspiContext *cspiContext = sdioDevice->context;

    cspiContext->byteBuffer[0] = (CsrUint8) (0x60 | (function & 0x7));
    cspiContext->byteBuffer[1] = (CsrUint8) ((address >> 16) & 0xFF);
    cspiContext->byteBuffer[2] = (CsrUint8) ((address >> 8) & 0xFF);
    cspiContext->byteBuffer[3] = (CsrUint8) ((address >> 0) & 0xFF);

    CsrSpiMasterChipSelectAssert(spiMasterDevice);

    if (operationCallback != NULL)
    {
        cspiContext->state = STATE_WRITE_CMD;
        cspiContext->eventOperationCallback = operationCallback;
        cspiContext->length = length;
        cspiContext->data = (void *) data;
        CsrSpiMasterWriteAsync(spiMasterDevice, cspiContext->byteBuffer, 4, eventOperationPartial);
    }
    else
    {
        CsrSpiMasterWrite(spiMasterDevice, cspiContext->byteBuffer, 4);
        CsrSpiMasterWrite(spiMasterDevice, data, length);
        CsrSpiMasterChipSelectNegate(spiMasterDevice);
    }
}

void CsrSdioCspiWrite(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 function, CsrUint32 address, const CsrUint8 *data, CsrUint32 length)
{
    CsrSdioCspiWriteAsync(spiMasterDevice, function, address, data, length, NULL);
}

#define CISTPL_NULL          0x00        /* End of Chain */
#define CISTPL_END           0xFF        /* End of Chain */

#define CISTPL_MANFID        0x20        /* Manufacturer Identification Tuple */
#define TPLMID_MANF          0x00        /* Manufacturer Code */
#define TPLMID_CARD          0x02        /* Manufacturer Information (Part number and/or revision) */

#define CISTPL_FUNCE         0x22        /* Function Extension Tuple */
#define TPLFE_FN0_BLK_SIZE   0x01        /* Maximum Block Size for Function 0 */
#define TPLFE_MAX_TRAN_SPEED 0x03        /* Maximum Transfer Rate */
#define TPLFE_MAX_BLK_SIZE   0x0C        /* Maximum Block Size for Function 1-7 */

/* Read any tuple from any function CIS (function 1-7) or the commmon CIS (function 0) */
static CsrResult tupleRead(CsrSpiMasterDevice *spiMasterDevice,
    CsrUint8 function, CsrUint8 tupleCode, CsrUint8 offset, CsrUint8 length, CsrUint8 *value,
    CsrResult (*byteRead)(CsrSpiMasterDevice *, CsrUint8, CsrUint32, CsrUint8 *))
{
    CsrUint8 tpl, lnk;
    CsrUint32 functionBaseAddress, cisPtr;
    CsrUint8 data[3];

    functionBaseAddress = (function & 0x7) * 0x100;

    if ((byteRead(spiMasterDevice, 0, 0x09 + functionBaseAddress, &data[0]) != CSR_RESULT_SUCCESS) ||
        (byteRead(spiMasterDevice, 0, 0x0A + functionBaseAddress, &data[1]) != CSR_RESULT_SUCCESS) ||
        (byteRead(spiMasterDevice, 0, 0x0B + functionBaseAddress, &data[2]) != CSR_RESULT_SUCCESS))

    {
        /* Read CIS Pointer Failed */
        return CSR_RESULT_FAILURE;
    }
    cisPtr = (data[2] << 16) | (data[1] << 8) | data[0];

    while (1)
    {
        /* Read Tuple Code */
        if (byteRead(spiMasterDevice, 0, cisPtr++, &tpl) != CSR_RESULT_SUCCESS)
        {
            return CSR_RESULT_FAILURE;
        }

        /* Read Link */
        if (byteRead(spiMasterDevice, 0, cisPtr++, &lnk) != CSR_RESULT_SUCCESS)
        {
            return CSR_RESULT_FAILURE;
        }

        /* Check End of Chain Marker */
        if ((tpl == CISTPL_END) || (tpl == CISTPL_NULL))
        {
            return CSR_RESULT_FAILURE;
        }

        /* Check Tuple Code Match */
        if (tpl == tupleCode)
        {
            /* Read in data */
            CsrInt32 i;
            for (i = offset; (i < lnk) && (i < (offset + length)); i++)
            {
                if (byteRead(spiMasterDevice, 0, cisPtr + i, value++) != CSR_RESULT_SUCCESS)
                {
                    return CSR_RESULT_FAILURE;
                }
            }

            if ((i - offset) < length)
            {
                return CSR_RESULT_FAILURE;
            }
            return CSR_RESULT_SUCCESS;
        }
        if (lnk == 255)
        {
            /* Special End of Chain Condition */
            return CSR_RESULT_FAILURE;
        }
        cisPtr += lnk;
    }
}

static CsrUint32 convertFrequency(CsrUint8 maxTranSpeed)
{
    const CsrUint32 units[] =
    {
        (100 * 1000) / 10,        /* 100 kbit/s */
        (1 * 1000 * 1000) / 10,   /* 1 Mbit/s */
        (10 * 1000 * 1000) / 10,  /* 10 Mbit/s */
        (100 * 1000 * 1000) / 10, /* 100 Mbit/s */
    };

    const CsrUint8 timeValues[] =
    {
        0,  /* Reserved */
        10,
        12,
        13,
        15,
        20,
        25,
        30,
        35,
        40,
        45,
        50,
        55,
        60,
        70,
        80
    };

    return (CsrUint32) (units[maxTranSpeed & 0x7] * timeValues[(maxTranSpeed & 0x78) >> 3]);
}

CsrResult CsrSdioCspiDeviceInitialise(CsrSdioDevice *sdioDevice, CsrUint16 *vendorId, CsrUint16 *deviceId, CsrUint8 standardInterface[8])
{
    CsrUint32 i;
    CsrUint32 ocr, response;
    CsrUint8 data;
    CsrUint8 word[2];
    CsrUint32 frequency = CSPI_SPI_INITIAL_FREQUENCY;
    CsrSpiMasterDevice *spiMasterDevice = sdioDevice->masterDevice;
    CsrSdioCspiContext *cspiContext = sdioDevice->context;

    if (cspiContext == NULL)
    {
        cspiContext = sdioDevice->context = CsrPmemAlloc(sizeof(CsrSdioCspiContext));
        cspiContext->state = STATE_IDLE;
    }

    /* Set initial frequency */
    CsrSpiMasterBusClockFrequencySet(spiMasterDevice, &frequency);

    /* Preclock */
    CsrSpiMasterChipSelectAssert(spiMasterDevice);
    CsrSpiMasterRead(spiMasterDevice, cspiContext->byteBuffer, 6);
    CsrSpiMasterChipSelectNegate(spiMasterDevice);
    CsrSpiMasterRead(spiMasterDevice, cspiContext->byteBuffer, 6);

    /* Enable CSPI by doing a CSPI write to the CSPI_PADDING (0xF9) register and check
       if CSPI is enabled by doing a CSPI byte read to the CSPI_F0_CSPI_STATUS register */
    CsrSdioCspiWrite8(spiMasterDevice, 0, CSPI_F0_CSPI_PADDING, CSPI_PADDING);
    if ((cspiByteRead(spiMasterDevice, 0, CSPI_F0_CSPI_STATUS, &data) != CSR_RESULT_SUCCESS) || (data != 0))
    {
        /* Assume that CSPI is not enabled */
        CSR_LOG_TEXT_INFO((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Device is not in CSPI mode, performing mode switch", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));

        /* Repeatedly do a CMD0 (limited by SSPI_CMD0_RETRIES) until a reply is detected */
        for (i = 0; i < SSPI_CMD0_RETRIES; i++)
        {
            if (sdioSpiCmd(spiMasterDevice, 0, 0, NULL) == CSR_RESULT_SUCCESS)
            {
                break;
            }
        }
        if (i == SSPI_CMD0_RETRIES)
        {
            /* No response from card received */
            CSR_LOG_TEXT_WARNING((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] No response to CMD0 after %u retries", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex, SSPI_CMD0_RETRIES));
            return CSR_RESULT_FAILURE;
        }

        /* Inquire about supported voltages */
        if (sdioSpiCmd(spiMasterDevice, 5, 0, &response) != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] No response to first CMD5", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
            return CSR_RESULT_FAILURE;
        }
        else
        {
            ocr = response & 0xFFFFFF; /* Supported Voltages */
            sdioDevice->sdioFunctionsCount = (CsrUint8) ((response >> 28) & 0x7); /* Number of device functions */
            if (sdioDevice->sdioFunctionsCount == 0)
            {
                CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Unexpected response to first CMD5 (functions is 0): %08lX", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex, response));
                return CSR_RESULT_FAILURE;
            }
        }

        /* Check if host supports the voltages accepted by the device */
        if (!(ocr & SSPI_HOST_OCR))
        {
            CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Unexpected response to first CMD5 (OCR outside supported range): %08lX", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex, response));
            return CSR_RESULT_FAILURE;
        }

        /* Specify operational voltage, and poll until ready (limited by SSPI_CMD5_RETRIES) */
        for (i = 0; i < SSPI_CMD5_RETRIES; i++)
        {
            if (sdioSpiCmd(spiMasterDevice, 5, SSPI_HOST_OCR, &response) == CSR_RESULT_SUCCESS)
            {
                if ((response >> 31) & 0x1)
                {
                    break;
                }
            }
            else
            {
                CSR_LOG_TEXT_WARNING((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] No response to CMD5", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
            }
        }
        if (i == SSPI_CMD5_RETRIES)
        {
            /* Voltage Negotiation Failed */
            CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Timeout while doing CMD5 voltage negotiation", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
            return CSR_RESULT_FAILURE;
        }

        /* Read vendorId from the CIS using SDIO-SPI commands. */
        if (tupleRead(spiMasterDevice, 0, CISTPL_MANFID, TPLMID_MANF, 2, word, sdioSpiByteRead) != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Failed to read Vendor ID from CIS", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
            return CSR_RESULT_FAILURE;
        }
        *vendorId = (word[1] << 8) | word[0];

        /* Read deviceId from the CIS using SDIO-SPI commands. */
        if (tupleRead(spiMasterDevice, 0, CISTPL_MANFID, TPLMID_CARD, 2, word, sdioSpiByteRead) != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Failed to read Device ID from CIS", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
            return CSR_RESULT_FAILURE;
        }
        *deviceId = (word[1] << 8) | word[0];

        /* Perform CSPI Mode Switch using SDIO-SPI commands */
        if (*vendorId == 0x032A)
        {
            CSR_LOG_TEXT_INFO((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Performing CSPI mode switch on CSR device with ID 0x%04hX", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex, *deviceId));

            /* Enable Generic Function */
            if (sdioSpiByteWrite(spiMasterDevice, 0, CSPI_F0_IO_ENABLE, (CsrUint8) (1 << sdioDevice->sdioFunctionsCount)) != CSR_RESULT_SUCCESS)
            {
                CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Failed to enable generic function", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
                return CSR_RESULT_FAILURE;
            }

            if (*deviceId == CSPI_DEVICE_ID_BC6)
            {
                /* Do a SDIO-SPI write of CSPI_SDIO_MODE_CSPI_EN_BC6 to CSPI_SDIO_MODE_BC6 to put the chip into CSPI mode. */
                if (sdioSpiByteWrite(spiMasterDevice, sdioDevice->sdioFunctionsCount, CSPI_SDIO_MODE_BC6, CSPI_SDIO_MODE_CSPI_EN_BC6) != CSR_RESULT_SUCCESS)
                {
                    CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Failed to write SDIO_MODE register, CSPI mode switch failed", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
                    return CSR_RESULT_FAILURE;
                }
            }
            else
            {
                /* Do a SDIO-SPI write of CSPI_SDIO_MODE_CSPI_EN_DEF to CSPI_SDIO_MODE_DEF to put the chip into CSPI mode. */
                if (sdioSpiByteWrite(spiMasterDevice, sdioDevice->sdioFunctionsCount, CSPI_SDIO_MODE_DEF, CSPI_SDIO_MODE_CSPI_EN_DEF) != CSR_RESULT_SUCCESS)
                {
                    CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Failed to write SDIO_MODE register, CSPI mode switch failed", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
                    return CSR_RESULT_FAILURE;
                }
            }
        }
        else
        {
            /* Unknown vendor */
            CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Unknown Vendor ID: 0x%04hX, should be 0x032A (CSR)", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
            return CSR_RESULT_FAILURE;
        }

        /* CSPI write to CSPI_PADDING register */
        CsrSdioCspiWrite8(spiMasterDevice, 0, CSPI_F0_CSPI_PADDING, CSPI_PADDING);
    }
    else
    {
        /* Assume that device is already in CSPI mode */
        CSR_LOG_TEXT_INFO((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Device is already in CSPI mode, continuing initialisation", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));

        /* Do a CSPI write 0xFE to the INT_ENABLE (0x04) register in function 0. */
        CsrSdioCspiWrite8(spiMasterDevice, 0, CSPI_F0_INT_ENABLE, 0xFE);

        /* Do a CSPI read from the INT_ENABLE (0x04) register in function 0. */
        if (cspiByteRead(spiMasterDevice, 0, CSPI_F0_INT_ENABLE, &data) != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Failed to read INT_ENABLE register", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
            return CSR_RESULT_FAILURE;
        }

        /* Count the number of bits set, to determine the number of functions. */
        sdioDevice->sdioFunctionsCount = 0;
        for (i = 0; i < 8; ++i)
        {
            if (data & 1)
            {
                sdioDevice->sdioFunctionsCount += 1;
            }
            data >>= 1;
        }
        if (sdioDevice->sdioFunctionsCount == 0)
        {
            CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] No functions on device", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
            return CSR_RESULT_FAILURE;
        }

        /* Read vendorId from the CIS using CSPI commands. */
        if (tupleRead(spiMasterDevice, 0, CISTPL_MANFID, TPLMID_MANF, 2, word, cspiByteRead) != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Failed to read Vendor ID from CIS", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
            return CSR_RESULT_FAILURE;
        }
        *vendorId = (word[1] << 8) | word[0];

        /* Read deviceId from the CIS using CSPI commands. */
        if (tupleRead(spiMasterDevice, 0, CISTPL_MANFID, TPLMID_CARD, 2, word, cspiByteRead) != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Failed to read Device ID from CIS", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
            return CSR_RESULT_FAILURE;
        }
        *deviceId = (word[1] << 8) | word[0];
    }

    /* Set interrupt to continuous */
    if (cspiByteRead(spiMasterDevice, 0, CSPI_F0_BUS_INTERFACE_CONTROL, &data) != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Failed to read BUS_INTERFACE_CONTROL register", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
        return CSR_RESULT_FAILURE;
    }
    if (!(data & 0x40))
    {
        CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] No continuous SPI interrupt support", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
        return CSR_RESULT_FAILURE;
    }
    CsrSdioCspiWrite8(spiMasterDevice, 0, CSPI_F0_BUS_INTERFACE_CONTROL, (CsrUint8) (data | 0x20));

    /* DriveMisoOnRisingClock */
    if (cspiByteRead(spiMasterDevice, 0, CSPI_F0_CSPI_MODE, &data) != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Failed to read CSPI_MODE register", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
        return CSR_RESULT_FAILURE;
    }
    CsrSdioCspiWrite8(spiMasterDevice, 0, CSPI_F0_CSPI_MODE, (CsrUint8) (data | 1));

    /* Determine maximum clock frequency supported by the device */
    if (tupleRead(spiMasterDevice, 0, CISTPL_FUNCE, TPLFE_MAX_TRAN_SPEED, 1, &data, cspiByteRead) != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Failed to read maximum transfer speed from CIS", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
        return CSR_RESULT_FAILURE;
    }
    sdioDevice->maxFrequency = convertFrequency(data);

    /* Switch to operational frequency */
    frequency = sdioDevice->maxFrequency;
    CsrSpiMasterBusClockFrequencySet(spiMasterDevice, &frequency);

    /* Read SDIO Standard Interface Code for each function and initialise max block size */
    standardInterface[0] = 0;
    sdioDevice->maxBlockSize[0] = 0;
    for (i = 1; i <= sdioDevice->sdioFunctionsCount; i++)
    {
        CsrResult result;

        sdioDevice->maxBlockSize[i] = 0;

        result = cspiByteRead(spiMasterDevice, 0, i * 0x100, &standardInterface[i]);

        /* Check for extended SDIO Standard Interface Code */
        if ((result == CSR_RESULT_SUCCESS) && ((standardInterface[i] & 0xF) == 0xF))
        {
            result = cspiByteRead(spiMasterDevice, 0, i * 0x100 + 1, &standardInterface[i]);
        }

        if (result != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Failed to read standard interface code (function %u)", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex, i));
            return CSR_RESULT_FAILURE;
        }
    }

    /* Disable all interrupts */
    CsrSdioCspiWrite8(spiMasterDevice, 0, CSPI_F0_INT_ENABLE, 0x00);

    /* Disable all functions */
    CsrSdioCspiWrite8(spiMasterDevice, 0, CSPI_F0_IO_ENABLE, 0x00);

    CSR_LOG_TEXT_INFO((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Device successfully initialised", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
    return CSR_RESULT_SUCCESS;
}

#endif /* CSR_SDIO_USE_CSPI */
