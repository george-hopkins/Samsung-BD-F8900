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

#ifdef CSR_SDIO_USE_SDIO

#include "csr_sdio_master.h"
#include "csr_sdio_sdio.h"
#include "csr_sdio_common.h"


/********************************/
/********* Context Data *********/
/********************************/

typedef struct
{
    /* Operation State data */
    CsrUint8  function;
    CsrUint32 address;
    CsrUint8 *data;
    CsrUint32 length;

    CsrSdioMasterCallback operationCallback;
} CsrSdioSdioContext;


/**********************************/
/********* Implementation *********/
/**********************************/

/* Calculate next fragment, returns TRUE if unit is in blocks */
static CsrBool calculateNextFragment(CsrSdioMasterDevice *sdioMasterDevice,
    CsrUint32 *units, CsrUint32 length, CsrUint8 function)
{
    CsrSdioDevice *sdioDevice = sdioMasterDevice->driverData;

    *units = 0;

    if ((sdioMasterDevice->blockSize[function] > 0) &&
        !((length <= sdioDevice->maxBlockSize[function]) &&
          (length <= CSR_SDIO_MASTER_BYTE_MODE_TRANSFER_LIMIT)))
    {
        *units = length / sdioMasterDevice->blockSize[function];
        if (*units > CSR_SDIO_MASTER_BLOCK_MODE_TRANSFER_LIMIT)
        {
            *units = CSR_SDIO_MASTER_BLOCK_MODE_TRANSFER_LIMIT;
        }
    }

    if (*units == 0)
    {
        *units = length;
        if (*units > CSR_SDIO_MASTER_BYTE_MODE_TRANSFER_LIMIT)
        {
            *units = CSR_SDIO_MASTER_BYTE_MODE_TRANSFER_LIMIT;
        }
        if (*units > sdioDevice->maxBlockSize[function])
        {
            *units = sdioDevice->maxBlockSize[function];
        }
        return FALSE;
    }
    return TRUE;
}

CsrResult CsrSdioSdioRead16(CsrSdioMasterDevice *sdioMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint16 *data)
{
    CsrUint8 byteLower, byteUpper;
    CsrResult result;

    result = CsrSdioMasterRead8(sdioMasterDevice, function, address, &byteLower);
    if (result == CSR_RESULT_SUCCESS)
    {
        result = CsrSdioMasterRead8(sdioMasterDevice, function, address + 1, &byteUpper);
        *data = (((CsrUint16) byteUpper) << 8) | byteLower;
    }

    return result;
}

CsrResult CsrSdioSdioWrite16(CsrSdioMasterDevice *sdioMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint16 data)
{
    CsrResult result;

    result = CsrSdioMasterWrite8(sdioMasterDevice, function, address + 1, (CsrUint8) ((data >> 8) & 0xFF));
    if (result == CSR_RESULT_SUCCESS)
    {
        result = CsrSdioMasterWrite8(sdioMasterDevice, function, address, (CsrUint8) (data & 0xFF));
    }

    return result;
}

CsrResult CsrSdioSdioRead(CsrSdioMasterDevice *sdioMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrUint32 length)
{
    CsrResult result;

    do
    {
        CsrUint32 units = 0;

        if (calculateNextFragment(sdioMasterDevice, &units, length, function))
        {
            CsrUint32 bytesToTransfer = (units * sdioMasterDevice->blockSize[function]);

            if (length == bytesToTransfer)
            {
                return CsrSdioMasterBlockModeRead(sdioMasterDevice, function, address, data, (CsrUint16) units);
            }
            else
            {
                result = CsrSdioMasterBlockModeRead(sdioMasterDevice, function, address, data, (CsrUint16) units);
                data += bytesToTransfer;
                length -= bytesToTransfer;
            }
        }
        else
        {
            CsrUint32 bytesToTransfer = units;

            if (length == bytesToTransfer)
            {
                return CsrSdioMasterByteModeRead(sdioMasterDevice, function, address, data, (CsrUint16) bytesToTransfer);
            }
            else
            {
                result = CsrSdioMasterByteModeRead(sdioMasterDevice, function, address, data, (CsrUint16) bytesToTransfer);
                data += bytesToTransfer;
                length -= bytesToTransfer;
            }
        }
    } while (result == CSR_RESULT_SUCCESS);

    return result;
}

CsrResult CsrSdioSdioWrite(CsrSdioMasterDevice *sdioMasterDevice,
    CsrUint8 function, CsrUint32 address, const CsrUint8 *data, CsrUint32 length)
{
    CsrResult result;

    do
    {
        CsrUint32 units = 0;

        if (calculateNextFragment(sdioMasterDevice, &units, length, function))
        {
            CsrUint32 bytesToTransfer = (units * sdioMasterDevice->blockSize[function]);

            if (length == bytesToTransfer)
            {
                return CsrSdioMasterBlockModeWrite(sdioMasterDevice, function, address, data, (CsrUint16) units);
            }
            else
            {
                result = CsrSdioMasterBlockModeWrite(sdioMasterDevice, function, address, data, (CsrUint16) units);
                data += bytesToTransfer;
                length -= bytesToTransfer;
            }
        }
        else
        {
            CsrUint32 bytesToTransfer = units;

            if (length == bytesToTransfer)
            {
                return CsrSdioMasterByteModeWrite(sdioMasterDevice, function, address, data, (CsrUint16) bytesToTransfer);
            }
            else
            {
                result = CsrSdioMasterByteModeWrite(sdioMasterDevice, function, address, data, (CsrUint16) bytesToTransfer);
                data += bytesToTransfer;
                length -= bytesToTransfer;
            }
        }
    } while (result == CSR_RESULT_SUCCESS);

    return result;
}

static CsrBool operationReadPartialCallback(CsrSdioMasterDevice *sdioMasterDevice, CsrResult result)
{
    CsrSdioDevice *sdioDevice = sdioMasterDevice->driverData;
    CsrSdioSdioContext *sdioContext = sdioDevice->context;

    if (result == CSR_RESULT_SUCCESS)
    {
        CsrUint32 units = 0;

        if (calculateNextFragment(sdioMasterDevice, &units, sdioContext->length, sdioContext->function))
        {
            CsrUint32 bytesToTransfer = (units * sdioMasterDevice->blockSize[sdioContext->function]);

            if (sdioContext->length == bytesToTransfer)
            {
                CsrSdioMasterBlockModeReadAsync(sdioMasterDevice, sdioContext->function, sdioContext->address, sdioContext->data, (CsrUint16) units, sdioContext->operationCallback);
            }
            else
            {
                CsrUint8 *data;

                data = sdioContext->data;
                sdioContext->data = data + bytesToTransfer;
                sdioContext->length -= bytesToTransfer;

                CsrSdioMasterBlockModeReadAsync(sdioMasterDevice, sdioContext->function, sdioContext->address, data, (CsrUint16) units, operationReadPartialCallback);
            }
        }
        else
        {
            CsrUint32 bytesToTransfer = units;

            if (sdioContext->length == bytesToTransfer)
            {
                CsrSdioMasterByteModeReadAsync(sdioMasterDevice, sdioContext->function, sdioContext->address, sdioContext->data, (CsrUint16) bytesToTransfer, sdioContext->operationCallback);
            }
            else
            {
                CsrUint8 *data;

                data = sdioContext->data;
                sdioContext->data = data + bytesToTransfer;
                sdioContext->length -= bytesToTransfer;

                CsrSdioMasterByteModeReadAsync(sdioMasterDevice, sdioContext->function, sdioContext->address, data, (CsrUint16) bytesToTransfer, operationReadPartialCallback);
            }
        }
    }
    else
    {
        return sdioContext->operationCallback(sdioMasterDevice, result);
    }

    return FALSE;
}

static CsrBool operationWritePartialCallback(CsrSdioMasterDevice *sdioMasterDevice, CsrResult result)
{
    CsrSdioDevice *sdioDevice = sdioMasterDevice->driverData;
    CsrSdioSdioContext *sdioContext = sdioDevice->context;

    if (result == CSR_RESULT_SUCCESS)
    {
        CsrUint32 units = 0;

        if (calculateNextFragment(sdioMasterDevice, &units, sdioContext->length, sdioContext->function))
        {
            CsrUint32 bytesToTransfer = (units * sdioMasterDevice->blockSize[sdioContext->function]);

            if (sdioContext->length == bytesToTransfer)
            {
                CsrSdioMasterBlockModeWriteAsync(sdioMasterDevice, sdioContext->function, sdioContext->address, sdioContext->data, (CsrUint16) units, sdioContext->operationCallback);
            }
            else
            {
                CsrUint8 *data;

                data = sdioContext->data;
                sdioContext->data = data + bytesToTransfer;
                sdioContext->length -= bytesToTransfer;

                CsrSdioMasterBlockModeWriteAsync(sdioMasterDevice, sdioContext->function, sdioContext->address, data, (CsrUint16) units, operationWritePartialCallback);
            }
        }
        else
        {
            CsrUint32 bytesToTransfer = units;

            if (sdioContext->length == bytesToTransfer)
            {
                CsrSdioMasterByteModeWriteAsync(sdioMasterDevice, sdioContext->function, sdioContext->address, sdioContext->data, (CsrUint16) bytesToTransfer, sdioContext->operationCallback);
            }
            else
            {
                CsrUint8 *data;

                data = sdioContext->data;
                sdioContext->data = data + bytesToTransfer;
                sdioContext->length -= bytesToTransfer;

                CsrSdioMasterByteModeWriteAsync(sdioMasterDevice, sdioContext->function, sdioContext->address, data, (CsrUint16) bytesToTransfer, operationWritePartialCallback);
            }
        }
    }
    else
    {
        return sdioContext->operationCallback(sdioMasterDevice, result);
    }

    return FALSE;
}

void CsrSdioSdioReadAsync(CsrSdioMasterDevice *sdioMasterDevice,
    CsrUint8 function, CsrUint32 address, CsrUint8 *data, CsrUint32 length,
    CsrSdioMasterCallback operationCallback)
{
    CsrSdioDevice *sdioDevice = sdioMasterDevice->driverData;
    CsrSdioSdioContext *sdioContext = sdioDevice->context;

    sdioContext->function = function;
    sdioContext->address = address;
    sdioContext->data = data;
    sdioContext->length = length;
    sdioContext->operationCallback = operationCallback;

    operationReadPartialCallback(sdioMasterDevice, CSR_RESULT_SUCCESS);
}

void CsrSdioSdioWriteAsync(CsrSdioMasterDevice *sdioMasterDevice,
    CsrUint8 function, CsrUint32 address, const CsrUint8 *data, CsrUint32 length,
    CsrSdioMasterCallback operationCallback)
{
    CsrSdioDevice *sdioDevice = sdioMasterDevice->driverData;
    CsrSdioSdioContext *sdioContext = sdioDevice->context;

    sdioContext->function = function;
    sdioContext->address = address;
    sdioContext->data = (CsrUint8 *) data;
    sdioContext->length = length;
    sdioContext->operationCallback = operationCallback;

    operationWritePartialCallback(sdioMasterDevice, CSR_RESULT_SUCCESS);
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
static CsrResult tupleRead(CsrSdioMasterDevice *sdioMasterDevice,
    CsrUint8 function, CsrUint8 tupleCode, CsrUint8 offset, CsrUint8 length, CsrUint8 *value)
{
    CsrUint8 tpl, lnk;
    CsrUint32 functionBaseAddress, cisPtr;
    CsrUint8 data[3];

    functionBaseAddress = (function & 0x7) * 0x100;

    if ((CsrSdioMasterRead8(sdioMasterDevice, 0, 0x09 + functionBaseAddress, &data[0]) != CSR_RESULT_SUCCESS) ||
        (CsrSdioMasterRead8(sdioMasterDevice, 0, 0x0A + functionBaseAddress, &data[1]) != CSR_RESULT_SUCCESS) ||
        (CsrSdioMasterRead8(sdioMasterDevice, 0, 0x0B + functionBaseAddress, &data[2]) != CSR_RESULT_SUCCESS))
    {
        /* Read CIS Pointer Failed */
        return CSR_RESULT_FAILURE;
    }
    cisPtr = (data[2] << 16) | (data[1] << 8) | data[0];

    while (TRUE)
    {
        /* Read Tuple Code */
        if (CsrSdioMasterRead8(sdioMasterDevice, 0, cisPtr++, &tpl))
        {
            return CSR_RESULT_FAILURE;
        }

        /* Read Link */
        if (CsrSdioMasterRead8(sdioMasterDevice, 0, cisPtr++, &lnk))
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
            CsrUint16 i;
            for (i = offset; (i < lnk) && (i < (offset + length)); i++)
            {
                if (CsrSdioMasterRead8(sdioMasterDevice, 0, cisPtr + i, value++))
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

static CsrResult maxBlockSizeInitialise(CsrSdioMasterDevice *sdioMasterDevice, CsrUint16 maxBlockSize[8])
{
    CsrUint8 word[2];
    CsrUint8 function = 0;

    /* Function 0 */
    if (tupleRead(sdioMasterDevice, function, CISTPL_FUNCE, TPLFE_FN0_BLK_SIZE, 2, word) != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_SDIO, "[%u,%u] Failed to read maximum block size from CIS (function %u)", sdioMasterDevice->busIndex, sdioMasterDevice->deviceIndex, function));
        return CSR_RESULT_FAILURE;
    }
    maxBlockSize[0] = (word[1] << 8) | word[0];

    /* Function 1 to 7 */
    for (function = 1; function <= sdioMasterDevice->functions; function++)
    {
        if (tupleRead(sdioMasterDevice, function, CISTPL_FUNCE, TPLFE_MAX_BLK_SIZE, 2, word) != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_SDIO, "[%u,%u] Failed to read maximum block size from CIS (function %u)", sdioMasterDevice->busIndex, sdioMasterDevice->deviceIndex, function));
            return CSR_RESULT_FAILURE;
        }
        maxBlockSize[function] = (word[1] << 8) | word[0];
    }

    return CSR_RESULT_SUCCESS;
}

static CsrResult frequencyInitialise(CsrSdioMasterDevice *sdioMasterDevice, CsrUint32 *maxFrequency)
{
    CsrUint8 data;

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

    /* Retrieve the maximum transfer rate supported by the device */
    if (tupleRead(sdioMasterDevice, 0, CISTPL_FUNCE, TPLFE_MAX_TRAN_SPEED, 1, &data) != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_SDIO, "[%u,%u] Failed to read maximum transfer speed from CIS", sdioMasterDevice->busIndex, sdioMasterDevice->deviceIndex));
        return CSR_RESULT_FAILURE;
    }

    *maxFrequency = units[data & 0x7] * timeValues[(data & 0x78) >> 3];

    return CSR_RESULT_SUCCESS;
}

static CsrResult deviceInfoInitialise(CsrSdioMasterDevice *sdioMasterDevice, CsrUint16 *vendorId, CsrUint16 *deviceId, CsrUint8 standardInterface[8])
{
    CsrUint8 word[2];
    CsrInt32 i;

    /* Retrieve Vendor ID */
    if (tupleRead(sdioMasterDevice, 0, CISTPL_MANFID, TPLMID_MANF, 2, word) != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_SDIO, "[%u,%u] Failed to read Vendor ID from CIS", sdioMasterDevice->busIndex, sdioMasterDevice->deviceIndex));
        return CSR_RESULT_FAILURE;
    }
    *vendorId = (word[1] << 8) | word[0];

    /* Retrieve Device ID */
    if (tupleRead(sdioMasterDevice, 0, CISTPL_MANFID, TPLMID_CARD, 2, word) != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_SDIO, "[%u,%u] Failed to read Device ID from CIS", sdioMasterDevice->busIndex, sdioMasterDevice->deviceIndex));
        return CSR_RESULT_FAILURE;
    }
    *deviceId = (word[1] << 8) | word[0];

    /* Read SDIO Standard Interface Code for each function */
    standardInterface[0] = 0;
    for (i = 1; i <= sdioMasterDevice->functions; i++)
    {
        CsrResult result;

        result = CsrSdioMasterRead8(sdioMasterDevice, 0, i * 0x100, &standardInterface[i]);

        /* Check for extended SDIO Standard Interface Code */
        if ((result == CSR_RESULT_SUCCESS) && ((standardInterface[i] & 0xF) == 0xF))
        {
            result = CsrSdioMasterRead8(sdioMasterDevice, 0, i * 0x100 + 1, &standardInterface[i]);
        }

        if (result != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_ERROR((CsrSdioLto, CSR_SDIO_LTSO_SDIO, "[%u,%u] Failed to read standard interface code (function %u)", sdioMasterDevice->busIndex, sdioMasterDevice->deviceIndex, i));
            return CSR_RESULT_FAILURE;
        }
    }

    return CSR_RESULT_SUCCESS;
}

CsrResult CsrSdioSdioDeviceInitialise(CsrSdioDevice *sdioDevice, CsrUint16 *vendorId, CsrUint16 *deviceId, CsrUint8 standardInterface[8])
{
    CsrSdioMasterDevice *sdioMasterDevice = sdioDevice->masterDevice;
    CsrSdioSdioContext *sdioContext = sdioDevice->context;
    CsrResult result;

    if (sdioContext == NULL)
    {
        sdioContext = sdioDevice->context = CsrPmemAlloc(sizeof(CsrSdioSdioContext));
    }

    /* Initialise device */
    result = CsrSdioMasterDeviceInitialise(sdioMasterDevice);
    if (result == CSR_RESULT_SUCCESS)
    {
        sdioDevice->sdioFunctionsCount = sdioMasterDevice->functions;
        result = deviceInfoInitialise(sdioMasterDevice, vendorId, deviceId, standardInterface);
        if (result == CSR_RESULT_SUCCESS)
        {
            result = maxBlockSizeInitialise(sdioMasterDevice, sdioDevice->maxBlockSize);
            if (result == CSR_RESULT_SUCCESS)
            {
                result = frequencyInitialise(sdioMasterDevice, &sdioDevice->maxFrequency);
            }
        }
    }

    CSR_LOG_TEXT_CONDITIONAL_INFO(result == CSR_RESULT_SUCCESS, (CsrSdioLto, CSR_SDIO_LTSO_SDIO, "[%u,%u] Device successfully initialised", sdioMasterDevice->busIndex, sdioMasterDevice->deviceIndex));

    return result;
}

#endif /* CSR_SDIO_USE_SDIO */
