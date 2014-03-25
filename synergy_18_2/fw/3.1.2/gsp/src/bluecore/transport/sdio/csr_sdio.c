/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_macro.h"
#include "csr_framework_ext.h"
#include "csr_log_text_2.h"

#include "csr_sdio.h"
#include "csr_sdio_common.h"

#ifdef CSR_SDIO_USE_SDIO
#include "csr_sdio_master.h"
#include "csr_sdio_sdio.h"
#endif
#ifdef CSR_SDIO_USE_CSPI
#include "csr_spi_master.h"
#include "csr_sdio_cspi.h"
#endif

#if !defined(CSR_SDIO_USE_SDIO) && !defined(CSR_SDIO_USE_CSPI)
#error Must define at least one of CSR_SDIO_USE_SDIO and CSR_SDIO_USE_CSPI
#endif


/******************************/
/********* Parameters *********/
/******************************/

#define ENABLE_DISABLE_RETRIES 65536


/**********************************/
/********* Implementation *********/
/**********************************/

#define FORCE_FUNCTION_0_ADDRESS (1U << 31)

/* Shared Mutex */
static CsrMutexHandle sharedMutex;
static CsrUint8 registerCount = 0;

/* Function Driver and Attached Functions Counters */
static CsrUint8 functionDriversCount = 0;
static CsrUint8 attachedFunctionsCount = 0;

/* Card/Device Information Structure - maintained by lower layer */
#ifdef CSR_SDIO_USE_SDIO
static CsrUint8 sdioMasterDevicesCount;
static CsrSdioMasterDevice *sdioMasterDevices;
#endif
#ifdef CSR_SDIO_USE_CSPI
static CsrUint8 spiMasterDevicesCount;
static CsrSpiMasterDevice *spiMasterDevices;
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DEFINE(CsrSdioLto);

#ifdef CSR_LOG_ENABLE
static const CsrCharString *subOrigins[] =
{
#ifdef CSR_SDIO_USE_SDIO
    "SDIO",
#endif
#ifdef CSR_SDIO_USE_CSPI
    "CSPI",
#endif
};
#endif

#ifdef CSR_SDIO_USE_SDIO
static CsrResult sdioTranslateResult(CsrResult result)
{
    switch (result)
    {
        case CSR_RESULT_SUCCESS:
            return CSR_RESULT_SUCCESS;
        case CSR_SDIO_MASTER_RESULT_TIMEOUT:
            return CSR_SDIO_RESULT_TIMEOUT;
        case CSR_SDIO_MASTER_RESULT_CRC:
            return CSR_SDIO_RESULT_CRC_ERROR;
        case CSR_SDIO_MASTER_RESULT_NOT_RESET:
            return CSR_SDIO_RESULT_NOT_RESET;
        case CSR_RESULT_FAILURE:
        default:
            return CSR_RESULT_FAILURE;
    }
}

#endif

#ifdef CSR_SDIO_USE_CSPI
static CsrResult spiTranslateResult(CsrResult result)
{
    switch (result)
    {
        case CSR_RESULT_SUCCESS:
            return CSR_RESULT_SUCCESS;
        case CSR_SPI_MASTER_RESULT_NOT_RESET:
            return CSR_SDIO_RESULT_NOT_RESET;
        case CSR_RESULT_FAILURE:
        default:
            return CSR_RESULT_FAILURE;
    }
}

#endif

#ifdef CSR_SDIO_USE_SDIO
static void sdioCallbackInhibitEnter(CsrSdioDevice *sdioDevice)
{
    CsrUint8 i;
    for (i = 0; i < sdioDevice->bus->devicesCount; ++i)
    {
        CsrSdioMasterCallbackInhibitEnter(sdioDevice->bus->devices[i]->masterDevice);
    }
}

static void sdioCallbackInhibitLeave(CsrSdioDevice *sdioDevice)
{
    CsrUint8 i;
    for (i = 0; i < sdioDevice->bus->devicesCount; ++i)
    {
        CsrSdioMasterCallbackInhibitLeave(sdioDevice->bus->devices[i]->masterDevice);
    }
}

#endif

#ifdef CSR_SDIO_USE_CSPI
static void spiCallbackInhibitEnter(CsrSdioDevice *sdioDevice)
{
    CsrUint8 i;
    for (i = 0; i < sdioDevice->bus->devicesCount; ++i)
    {
        CsrSpiMasterCallbackInhibitEnter(sdioDevice->bus->devices[i]->masterDevice);
    }
}

static void spiCallbackInhibitLeave(CsrSdioDevice *sdioDevice)
{
    CsrUint8 i;
    for (i = 0; i < sdioDevice->bus->devicesCount; ++i)
    {
        CsrSpiMasterCallbackInhibitLeave(sdioDevice->bus->devices[i]->masterDevice);
    }
}

#endif

static void callbackInhibitEnter(CsrSdioDevice *sdioDevice)
{
    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            sdioCallbackInhibitEnter(sdioDevice);
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            spiCallbackInhibitEnter(sdioDevice);
            break;
#endif
    }
}

static void callbackInhibitLeave(CsrSdioDevice *sdioDevice)
{
    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            sdioCallbackInhibitLeave(sdioDevice);
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            spiCallbackInhibitLeave(sdioDevice);
            break;
#endif
    }
}

/* Forward declaration for internal unprotected functions */
static void read8Async(CsrSdioFunction *function, CsrUint32 address, CsrUint8 *data, CsrSdioAsyncCallback callback);
static void write8Async(CsrSdioFunction *function, CsrUint32 address, CsrUint8 data, CsrSdioAsyncCallback callback);
static void readAsync(CsrSdioFunction *function, CsrUint32 address, void *data, CsrUint32 length, CsrSdioAsyncCallback callback);
static void writeAsync(CsrSdioFunction *function, CsrUint32 address, const void *data, CsrUint32 length, CsrSdioAsyncCallback callback);

static void queueElementKick(CsrSdioFunctionPrivate *functionPrivate)
{
    CsrSdioQueueElement *queueElement = &functionPrivate->queueElement;

    switch (queueElement->operationType)
    {
        case CSR_SDIO_OPERATION_TYPE_READ8:
            read8Async(&functionPrivate->pub, queueElement->address, queueElement->dataPtr, queueElement->callback);
            break;
        case CSR_SDIO_OPERATION_TYPE_WRITE8:
            write8Async(&functionPrivate->pub, queueElement->address, queueElement->data8, queueElement->callback);
            break;
        case CSR_SDIO_OPERATION_TYPE_READ:
            readAsync(&functionPrivate->pub, queueElement->address, queueElement->dataPtr, queueElement->length, queueElement->callback);
            break;
        case CSR_SDIO_OPERATION_TYPE_WRITE:
            writeAsync(&functionPrivate->pub, queueElement->address, queueElement->dataPtr, queueElement->length, queueElement->callback);
            break;
        default:
            break;
    }
    queueElement->operationType = CSR_SDIO_OPERATION_TYPE_NONE;
}

static void queueElementSave(CsrSdioFunctionPrivate *functionPrivate,
    CsrUint8 type,
    CsrUint32 address,
    CsrUint8 data8,
    CsrUint32 length,
    CsrUint8 *dataPtr,
    CsrSdioAsyncCallback callback)
{
    functionPrivate->queueElement.operationType = type;
    functionPrivate->queueElement.address = address;
    functionPrivate->queueElement.data8 = data8;
    functionPrivate->queueElement.length = length;
    functionPrivate->queueElement.dataPtr = dataPtr;
    functionPrivate->queueElement.callback = callback;
}

static void waitQueuePush(CsrSdioFunctionPrivate *functionPrivate)
{
    CsrSdioDevice *sdioDevice = (&functionPrivate->pub)->device;
    CsrSdioBus *bus = sdioDevice->bus;

    functionPrivate->prev = NULL;
    if (bus->waitQueueIn == NULL)
    {
        functionPrivate->next = NULL;
        bus->waitQueueIn = bus->waitQueueOut = functionPrivate;
    }
    else
    {
        functionPrivate->next = bus->waitQueueIn;
        bus->waitQueueIn->prev = functionPrivate;
        bus->waitQueueIn = functionPrivate;
    }
}

static CsrResult waitQueueBlockedPeek(CsrSdioBus *bus)
{
    return (bus->waitQueueOut != NULL) && bus->waitQueueOut->blocked;
}

static CsrSdioFunctionPrivate *waitQueuePop(CsrSdioBus *bus)
{
    CsrSdioFunctionPrivate *functionPrivate = NULL;
    if (bus->waitQueueOut != NULL)
    {
        functionPrivate = bus->waitQueueOut;
        bus->waitQueueOut = functionPrivate->prev;
        if (bus->waitQueueOut != NULL)
        {
            bus->waitQueueOut->next = NULL;
        }
        else
        {
            bus->waitQueueIn = NULL;
        }
    }
    return functionPrivate;
}

static void assocPush(CsrSdioFunctionPrivate *functionPrivate, CsrSdioAsyncDsrCallback callback, CsrResult result)
{
    CsrSdioFunction *function = &functionPrivate->pub;
    CsrSdioDevice *sdioDevice = function->device;
    sdioDevice->bus->assocFunction[sdioDevice->bus->assocIndex] = functionPrivate;
    sdioDevice->bus->assocCallback[sdioDevice->bus->assocIndex] = callback;
    sdioDevice->bus->assocResult[sdioDevice->bus->assocIndex] = result;
    sdioDevice->bus->assocIndex = (sdioDevice->bus->assocIndex + 1) % ASSOC_DATA_QUEUE_LENGTH;
}

static void assocPop(CsrSdioBus *bus, CsrSdioFunctionPrivate **functionPrivate, CsrSdioAsyncDsrCallback *callback, CsrResult *result)
{
    *functionPrivate = bus->assocFunction[bus->assocOutdex];
    *callback = bus->assocCallback[bus->assocOutdex];
    *result = bus->assocResult[bus->assocOutdex];
    bus->assocOutdex = (bus->assocOutdex + 1) % ASSOC_DATA_QUEUE_LENGTH;
}

static void interruptDsrCallback(CsrSdioDevice *sdioDevice)
{
    CsrUint8 functionNumber;

    for (functionNumber = 1; functionNumber <= sdioDevice->sdioFunctionsCount; ++functionNumber)
    {
        CsrSdioFunctionPrivate *functionPrivate = &sdioDevice->sdioFunctions[functionNumber - 1];

        if (functionPrivate->interruptDsrCallback != NULL)
        {
            functionPrivate->interruptDsrCallback(&functionPrivate->pub);
            functionPrivate->interruptDsrCallback = NULL;
        }
    }
}

#ifdef CSR_SDIO_USE_SDIO
static void sdioInterruptDsrCallback(CsrSdioMasterDevice *sdioMasterDevice)
{
    interruptDsrCallback(sdioMasterDevice->driverData);
}

#endif

#ifdef CSR_SDIO_USE_CSPI
static void spiInterruptDsrCallback(CsrSpiMasterDevice *spiMasterDevice)
{
    interruptDsrCallback(spiMasterDevice->driverData);
}

#endif

static CsrBool interruptCallback(CsrSdioDevice *sdioDevice)
{
    CsrBool triggerInterruptDsrCallback = FALSE;

    if (sdioDevice->localInterruptEnable != 0)
    {
        CsrUint8 functionNumber;

        for (functionNumber = 1; functionNumber <= sdioDevice->sdioFunctionsCount; ++functionNumber)
        {
            CsrSdioFunctionPrivate *functionPrivate = &sdioDevice->sdioFunctions[functionNumber - 1];

            if ((sdioDevice->localInterruptEnable & (1U << functionNumber)) != 0)
            {
                sdioDevice->localInterruptMask |= (1U << functionNumber);
                functionPrivate->callbackContext = TRUE;
                functionPrivate->interruptDsrCallback = functionPrivate->functionDriver->intr(&functionPrivate->pub);
                functionPrivate->callbackContext = FALSE;
                if (functionPrivate->interruptDsrCallback != NULL)
                {
                    triggerInterruptDsrCallback = TRUE;
                }
            }
        }
    }
    else
    {
        sdioDevice->localInterruptMask |= 1;
    }

    return triggerInterruptDsrCallback;
}

#ifdef CSR_SDIO_USE_SDIO
static CsrBool sdioInterruptCallback(CsrSdioMasterDevice *sdioMasterDevice, CsrResult result)
{
    return interruptCallback(sdioMasterDevice->driverData);
}

#endif

#ifdef CSR_SDIO_USE_CSPI
static CsrBool spiInterruptCallback(CsrSpiMasterDevice *spiMasterDevice)
{
    return interruptCallback(spiMasterDevice->driverData);
}

#endif

static void operationDsrCallback(CsrSdioDevice *sdioDevice)
{
    CsrSdioFunctionPrivate *functionPrivate;
    CsrSdioAsyncDsrCallback callback;
    CsrResult result;

    /* Check if any blocked operations need to be kicked off */
    if (waitQueueBlockedPeek(sdioDevice->bus) && !sdioDevice->bus->claimed)
    {
        CsrSdioFunctionPrivate *functionPrivateQueued = waitQueuePop(sdioDevice->bus);
        sdioDevice->bus->claimed = TRUE;
        functionPrivateQueued->blocked = FALSE;
        CsrEventSet(&functionPrivateQueued->blockEvent, 1);
    }

    /* Conditionally callback */
    assocPop(sdioDevice->bus, &functionPrivate, &callback, &result);
    if (callback != NULL)
    {
        callback(&functionPrivate->pub, result);
    }
}

#ifdef CSR_SDIO_USE_SDIO
static void sdioOperationDsrCallback(CsrSdioMasterDevice *sdioMasterDevice)
{
    sdioCallbackInhibitEnter(sdioMasterDevice->driverData);
    operationDsrCallback(sdioMasterDevice->driverData);
    sdioCallbackInhibitLeave(sdioMasterDevice->driverData);
}

#endif

#ifdef CSR_SDIO_USE_CSPI
static void spiOperationDsrCallback(CsrSpiMasterDevice *spiMasterDevice)
{
    spiCallbackInhibitEnter(spiMasterDevice->driverData);
    operationDsrCallback(spiMasterDevice->driverData);
    spiCallbackInhibitLeave(spiMasterDevice->driverData);
}

#endif

static CsrBool operationCallback(CsrSdioDevice *sdioDevice, CsrResult result)
{
    CsrSdioFunctionPrivate *functionPrivate = sdioDevice->operationHandle;
    CsrSdioAsyncDsrCallback callback;
    CsrBool kickBlocked = FALSE;
    CsrBool savedCallbackContext = functionPrivate->callbackContext;

    sdioDevice->bus->claimed = FALSE;

    functionPrivate->callbackContext = TRUE;
    callback = functionPrivate->operationCallback(&functionPrivate->pub, result);
    functionPrivate->callbackContext = savedCallbackContext;

    if (!sdioDevice->bus->claimed)
    {
        if (waitQueueBlockedPeek(sdioDevice->bus))
        {
            kickBlocked = TRUE;
        }
        else
        {
            CsrSdioFunctionPrivate *functionPrivateQueued = waitQueuePop(sdioDevice->bus);
            if (functionPrivateQueued != NULL)
            {
                sdioDevice->bus->claimed = TRUE;
                queueElementKick(functionPrivateQueued);
            }
        }
    }

    if ((callback != NULL) || kickBlocked)
    {
        assocPush(functionPrivate, callback, result);
        return TRUE;
    }

    return FALSE;
}

#ifdef CSR_SDIO_USE_SDIO
static CsrBool sdioOperationCallback(CsrSdioMasterDevice *sdioMasterDevice, CsrResult result)
{
    CsrBool scheduleDsr;
    sdioCallbackInhibitEnter(sdioMasterDevice->driverData);
    scheduleDsr = operationCallback(sdioMasterDevice->driverData, sdioTranslateResult(result));
    sdioCallbackInhibitLeave(sdioMasterDevice->driverData);
    return scheduleDsr;
}

#endif

#ifdef CSR_SDIO_USE_CSPI
static CsrBool spiOperationCallback(CsrSpiMasterDevice *spiMasterDevice)
{
    CsrBool scheduleDsr;
    spiCallbackInhibitEnter(spiMasterDevice->driverData);
    scheduleDsr = operationCallback(spiMasterDevice->driverData, CSR_RESULT_SUCCESS);
    spiCallbackInhibitLeave(spiMasterDevice->driverData);
    return scheduleDsr;
}

#endif

/* Remove the CsrSdioBus from a CsrSdioDevice, freeing the CsrSdioBus if possible */
static void removeBusFromDevice(CsrSdioDevice *sdioDevice)
{
    CsrUint8 devicesCount = 0;
    CsrUint8 i;

    /* Search through all the devices on the bus */
    for (i = 0; i < sdioDevice->bus->devicesCount; ++i)
    {
        if (sdioDevice->bus->devices[i] == sdioDevice)
        {
            sdioDevice->bus->devices[i] = NULL;
        }
        else if (sdioDevice->bus->devices[i] != NULL)
        {
            devicesCount += 1;
        }
    }

    if (devicesCount == 0)
    {
        /* Free the bus when it is empty */
        CsrPmemFree(sdioDevice->bus->devices);
        CsrPmemFree(sdioDevice->bus);
    }
}

/* Assign a CsrSdioBus to a CsrSdioDevice, creating the CsrSdioBus if necessary */
static void assignBusToDevice(CsrSdioDevice *sdioDevice)
{
    CsrUint8 devicesCount = 0;
    CsrUint8 i;

    sdioDevice->bus = NULL;

    /* Look for existing CsrSdioBus while counting number of devices on same bus */
    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
        {
            CsrSdioMasterDevice *sdioMasterDevice = sdioDevice->masterDevice;
            for (i = 0; i < sdioMasterDevicesCount; ++i)
            {
                CsrSdioMasterDevice *sdioMasterDeviceI = &sdioMasterDevices[i];
                if (sdioMasterDevice->busIndex == sdioMasterDeviceI->busIndex)
                {
                    CsrSdioDevice *sdioDeviceI = sdioMasterDeviceI->driverData;
                    if ((sdioDeviceI != NULL) && (sdioDeviceI->bus != NULL))
                    {
                        /* Found existing bus */
                        sdioDevice->bus = sdioDeviceI->bus;
                        break;
                    }
                    else
                    {
                        devicesCount += 1;
                    }
                }
            }
            break;
        }
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
        {
            CsrSpiMasterDevice *spiMasterDevice = sdioDevice->masterDevice;
            for (i = 0; i < spiMasterDevicesCount; ++i)
            {
                CsrSpiMasterDevice *spiMasterDeviceI = &spiMasterDevices[i];
                if (spiMasterDevice->busIndex == spiMasterDeviceI->busIndex)
                {
                    CsrSdioDevice *sdioDeviceI = spiMasterDeviceI->driverData;
                    if ((sdioDeviceI != NULL) && (sdioDeviceI->bus != NULL))
                    {
                        /* Found existing bus */
                        sdioDevice->bus = sdioDeviceI->bus;
                        break;
                    }
                    else
                    {
                        devicesCount += 1;
                    }
                }
            }
            break;
        }
#endif
    }

    /* Allocate new bus if none was found */
    if (sdioDevice->bus == NULL)
    {
        sdioDevice->bus = CsrPmemAlloc(sizeof(CsrSdioBus));
        sdioDevice->bus->claimed = FALSE;
        sdioDevice->bus->devices = CsrPmemZalloc(sizeof(CsrSdioDevice *) * devicesCount);
        sdioDevice->bus->devices[0] = sdioDevice;
        sdioDevice->bus->devicesCount = devicesCount;
        sdioDevice->bus->waitQueueIn = NULL;
        sdioDevice->bus->waitQueueOut = NULL;
        sdioDevice->bus->assocIndex = 0;
        sdioDevice->bus->assocOutdex = 0;
    }
    else /* Insert into free slot in existing devices chain of the found bus */
    {
        for (i = 0; i < sdioDevice->bus->devicesCount; ++i)
        {
            if (sdioDevice->bus->devices[i] == NULL)
            {
                sdioDevice->bus->devices[i] = sdioDevice;
            }
        }
    }
}

/* Initialise the contents of the CsrSdioDevice */
static void functionsInitialise(CsrSdioDevice *sdioDevice,
    CsrUint16 vendorId, CsrUint16 deviceId, CsrUint8 standardInterface[8], CsrBool dmaCapableMemRequired)
{
    CsrUint8 functionNumber;

    /* Allocate array of CsrSdioFunctionPrivate (which contain CsrSdioFunction structures) */
    sdioDevice->sdioFunctions = CsrPmemAlloc(sizeof(CsrSdioFunctionPrivate) * sdioDevice->sdioFunctionsCount);

    /* Initialise all members of the array */
    for (functionNumber = 1; functionNumber <= sdioDevice->sdioFunctionsCount; ++functionNumber)
    {
        CsrResult result;
        CsrSdioFunctionPrivate *functionPrivate = &sdioDevice->sdioFunctions[functionNumber - 1];
        CsrSdioFunction *function = &functionPrivate->pub;

        functionPrivate->state = CSR_SDIO_FUNCTION_STATE_DETACHED;
        functionPrivate->functionDriver = NULL;
        function->sdioId.manfId = vendorId;
        function->sdioId.cardId = deviceId;
        function->sdioId.sdioFunction = functionNumber;
        function->sdioId.sdioInterface = standardInterface[functionNumber];
        function->blockSize = 0;
        function->features = CSR_SDIO_FEATURE_BYTE_MODE;
        if (dmaCapableMemRequired)
        {
            function->features |= CSR_SDIO_FEATURE_DMA_CAPABLE_MEM_REQUIRED;
        }
        function->device = sdioDevice;
        function->priv = functionPrivate;
        function->driverData = NULL;
        functionPrivate->operationType = CSR_SDIO_OPERATION_TYPE_NONE;
        functionPrivate->operationCallback = NULL;
        functionPrivate->interruptDsrCallback = NULL;
        functionPrivate->queueElement.operationType = CSR_SDIO_OPERATION_TYPE_NONE;
        functionPrivate->callbackContext = FALSE;
        functionPrivate->blocked = FALSE;
        result = CsrEventCreate(&functionPrivate->blockEvent);
        CSR_LOG_TEXT_CONDITIONAL_CRITICAL(result != CSR_RESULT_SUCCESS, (CsrSdioLto, 0, "CsrSdioFunctionPrivate block event creation failed"));
        CSR_UNUSED(result);
        functionPrivate->next = NULL;
        functionPrivate->prev = NULL;
        functionPrivate->maxFrequency = 0;
    }
}

static void functionsDeinitialise(CsrSdioDevice *sdioDevice)
{
    CsrUint8 functionNumber;

    for (functionNumber = 1; functionNumber <= sdioDevice->sdioFunctionsCount; ++functionNumber)
    {
        CsrSdioFunctionPrivate *functionPrivate = &sdioDevice->sdioFunctions[functionNumber - 1];
        CsrEventDestroy(&functionPrivate->blockEvent);
    }
    CsrPmemFree(sdioDevice->sdioFunctions);
}

/* Initialise all master devices and create corresponding CsrSdioDevice */
static void devicesInitialise(void)
{
    CsrUint8 i;

#ifdef CSR_SDIO_USE_SDIO
    /* Initialise SDIO Master */
    CsrSdioMasterInitialise(&sdioMasterDevicesCount, &sdioMasterDevices);
    CSR_LOG_TEXT_INFO((CsrSdioLto, CSR_SDIO_LTSO_SDIO, "Initialising (%u devices)", sdioMasterDevicesCount));
    for (i = 0; i < sdioMasterDevicesCount; ++i)
    {
        CsrSdioMasterDevice *sdioMasterDevice = &sdioMasterDevices[i];
        sdioMasterDevice->driverData = NULL;
    }
    for (i = 0; i < sdioMasterDevicesCount; ++i)
    {
        CsrSdioMasterDevice *sdioMasterDevice = &sdioMasterDevices[i];
        CsrSdioDevice *sdioDevice;
        CsrResult result;
        CsrUint16 vendorId = 0, deviceId = 0;
        CsrUint8 standardInterface[8];

        sdioDevice = CsrPmemAlloc(sizeof(CsrSdioDevice));
        sdioDevice->type = CSR_SDIO_DEVICE_TYPE_SDIO;
        sdioDevice->masterDevice = sdioMasterDevice;
        sdioDevice->context = NULL;
        sdioDevice->operationHandle = NULL;
        sdioDevice->localInterruptEnable = 0;
        sdioDevice->localInterruptMask = 0;

        sdioMasterDevice->driverData = sdioDevice;

        /* Initialise device */
        result = CsrSdioSdioDeviceInitialise(sdioDevice, &vendorId, &deviceId, standardInterface);
        if (result != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_WARNING((CsrSdioLto, CSR_SDIO_LTSO_SDIO, "[%u,%u] Device initialisation failed", sdioMasterDevice->busIndex, sdioMasterDevice->deviceIndex));
            sdioMasterDevice->driverData = NULL;
            CsrPmemFree(sdioDevice->context);
            CsrPmemFree(sdioDevice);
            continue;
        }

        /* Register callbacks */
        CsrSdioMasterCallbackRegister(sdioMasterDevice,
            sdioInterruptCallback,
            sdioInterruptDsrCallback,
            sdioOperationDsrCallback);

        assignBusToDevice(sdioDevice);
        functionsInitialise(sdioDevice, vendorId, deviceId, standardInterface,
            (CsrBool) ((sdioMasterDevice->features & CSR_SDIO_MASTER_FEATURE_DMA_CAPABLE_MEM_REQUIRED) ? TRUE : FALSE));
    }
#endif
#ifdef CSR_SDIO_USE_CSPI
    /* Initialise SPI Master */
    CsrSpiMasterInitialise(&spiMasterDevicesCount, &spiMasterDevices);
    CSR_LOG_TEXT_INFO((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "Initialising (%u devices)", spiMasterDevicesCount));
    for (i = 0; i < spiMasterDevicesCount; ++i)
    {
        CsrSpiMasterDevice *spiMasterDevice = &spiMasterDevices[i];
        spiMasterDevice->driverData = NULL;
    }
    for (i = 0; i < spiMasterDevicesCount; ++i)
    {
        CsrSpiMasterDevice *spiMasterDevice = &spiMasterDevices[i];
        CsrSdioDevice *sdioDevice;
        CsrResult result;
        CsrUint16 vendorId = 0, deviceId = 0;
        CsrUint8 standardInterface[8];

        sdioDevice = CsrPmemAlloc(sizeof(CsrSdioDevice));
        sdioDevice->type = CSR_SDIO_DEVICE_TYPE_CSPI;
        sdioDevice->masterDevice = spiMasterDevice;
        sdioDevice->context = NULL;
        sdioDevice->operationHandle = NULL;
        sdioDevice->localInterruptEnable = 0;
        sdioDevice->localInterruptMask = 0;

        spiMasterDevice->driverData = sdioDevice;

        /* Initialise device */
        result = CsrSdioCspiDeviceInitialise(sdioDevice, &vendorId, &deviceId, standardInterface);
        if (result != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_WARNING((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "[%u,%u] Device initialisation failed", spiMasterDevice->busIndex, spiMasterDevice->deviceIndex));
            spiMasterDevice->driverData = NULL;
            CsrPmemFree(sdioDevice->context);
            CsrPmemFree(sdioDevice);
            continue;
        }

        /* Register callbacks */
        CsrSpiMasterCallbackRegister(spiMasterDevice,
            spiInterruptCallback,
            spiInterruptDsrCallback,
            spiOperationDsrCallback);

        assignBusToDevice(sdioDevice);
        functionsInitialise(sdioDevice, vendorId, deviceId, standardInterface,
            (CsrBool) ((spiMasterDevice->features & CSR_SPI_MASTER_FEATURE_DMA_CAPABLE_MEM_REQUIRED) ? TRUE : FALSE));
    }
#endif
}

/* Deinitialise all master devices freeing the corresponding CsrSdioDevice and any contents */
static void devicesDeinitialise(void)
{
    CsrUint8 i;

    /* Deinitialise all devices */
#ifdef CSR_SDIO_USE_SDIO
    CSR_LOG_TEXT_INFO((CsrSdioLto, CSR_SDIO_LTSO_SDIO, "Deinitialising (%u devices)", sdioMasterDevicesCount));
    for (i = 0; i < sdioMasterDevicesCount; ++i)
    {
        CsrSdioMasterDevice *sdioMasterDevice = &sdioMasterDevices[i];
        CsrSdioDevice *sdioDevice = sdioMasterDevice->driverData;

        sdioMasterDevice->driverData = NULL;
        if (sdioDevice != NULL)
        {
            functionsDeinitialise(sdioDevice);
            removeBusFromDevice(sdioDevice);
            CsrPmemFree(sdioDevice->context);
            CsrPmemFree(sdioDevice);
        }
    }
    CsrSdioMasterDeinitialise();
#endif
#ifdef CSR_SDIO_USE_CSPI
    CSR_LOG_TEXT_INFO((CsrSdioLto, CSR_SDIO_LTSO_CSPI, "Deinitialising (%u devices)", spiMasterDevicesCount));
    for (i = 0; i < spiMasterDevicesCount; ++i)
    {
        CsrSpiMasterDevice *spiMasterDevice = &spiMasterDevices[i];
        CsrSdioDevice *sdioDevice = spiMasterDevice->driverData;

        spiMasterDevice->driverData = NULL;
        if (sdioDevice != NULL)
        {
            functionsDeinitialise(sdioDevice);
            removeBusFromDevice(sdioDevice);
            CsrPmemFree(sdioDevice->context);
            CsrPmemFree(sdioDevice);
        }
    }
    CsrSpiMasterDeinitialise();
#endif
}

/* Returns TRUE if the given function descriptor is supported by the function drivers function descriptors */
static CsrBool isMatch(CsrSdioFunctionDriver *functionDriver, CsrSdioFunctionId *function)
{
    CsrUint8 i;

    for (i = 0; i < functionDriver->idsCount; ++i)
    {
        CsrSdioFunctionId *functionDriverId = &functionDriver->ids[i];

        if (((functionDriverId->manfId == function->manfId) || (functionDriverId->manfId == CSR_SDIO_ANY_MANF_ID)) &&
            ((functionDriverId->cardId == function->cardId) || (functionDriverId->cardId == CSR_SDIO_ANY_CARD_ID)) &&
            ((functionDriverId->sdioFunction == function->sdioFunction) || (functionDriverId->sdioFunction == CSR_SDIO_ANY_SDIO_FUNCTION)) &&
            ((functionDriverId->sdioInterface == function->sdioInterface) || (functionDriverId->sdioInterface == CSR_SDIO_ANY_SDIO_INTERFACE)))
        {
            return TRUE;
        }
    }
    return FALSE;
}

/* Claim the bus or block until it is available */
static void busSyncClaim(CsrSdioFunction *function)
{
    CsrSdioFunctionPrivate *functionPrivate = function->priv;
    CsrSdioDevice *sdioDevice = function->device;
    CsrBool block = FALSE;
    CsrUint32 eventBits = 0;

    /* Protect against other threads and the callbacks */
    CsrMutexLock(&sharedMutex);
    callbackInhibitEnter(sdioDevice);

    if (sdioDevice->bus->claimed)
    {
        functionPrivate->blocked = TRUE;
        waitQueuePush(functionPrivate);
        block = TRUE;
    }
    else
    {
        sdioDevice->bus->claimed = TRUE;
    }

    /* Lift protection against other threads and the callbacks */
    callbackInhibitLeave(sdioDevice);
    CsrMutexUnlock(&sharedMutex);

    if (block)
    {
        CsrEventWait(&functionPrivate->blockEvent, CSR_EVENT_WAIT_INFINITE, &eventBits);
    }

    CSR_LOG_TEXT_ASSERT(CsrSdioLto, 0, sdioDevice->bus->claimed);
}

static void busSyncRelease(CsrSdioFunction *function)
{
    CsrSdioDevice *sdioDevice = function->device;
    CsrSdioFunctionPrivate *functionPrivateQueued;

    /* Protect against other threads and the callbacks */
    CsrMutexLock(&sharedMutex);
    callbackInhibitEnter(sdioDevice);

    /* Check if there are anyone blocked or if an element is on a queue */
    functionPrivateQueued = waitQueuePop(sdioDevice->bus);
    if (functionPrivateQueued == NULL)
    {
        /* Release the bus */
        sdioDevice->bus->claimed = FALSE;
    }
    else
    {
        if (functionPrivateQueued->blocked)
        {
            functionPrivateQueued->blocked = FALSE;
            CsrEventSet(&functionPrivateQueued->blockEvent, 1);
        }
        else
        {
            /* Kick the queued element */
            queueElementKick(functionPrivateQueued);
        }
    }

    /* Lift protection against other threads and the callbacks */
    callbackInhibitLeave(sdioDevice);
    CsrMutexUnlock(&sharedMutex);
}

static void interruptAcknowledge(CsrSdioFunction *function, CsrUint8 functionNumber)
{
    CsrSdioFunctionPrivate *functionPrivate = function->priv;
    CsrSdioDevice *sdioDevice = function->device;
    CsrBool acknowledge = FALSE;

    /* Protect against other threads and the callbacks (from this device only) */
    if (!functionPrivate->callbackContext)
    {
        CsrMutexLock(&sharedMutex);
    }
    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            CsrSdioMasterCallbackInhibitEnter(sdioDevice->masterDevice);
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            CsrSpiMasterCallbackInhibitEnter(sdioDevice->masterDevice);
            break;
#endif
    }

    if (sdioDevice->localInterruptMask != 0)
    {
        sdioDevice->localInterruptMask &= ~(1U << functionNumber);
        if (sdioDevice->localInterruptMask == 0)
        {
            acknowledge = TRUE;
        }
    }

    /* Lift protection against other threads and the callbacks (from this device only) */
    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            CsrSdioMasterCallbackInhibitLeave(sdioDevice->masterDevice);
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            CsrSpiMasterCallbackInhibitLeave(sdioDevice->masterDevice);
            break;
#endif
    }
    if (!functionPrivate->callbackContext)
    {
        CsrMutexUnlock(&sharedMutex);
    }

    if (acknowledge)
    {
        switch (sdioDevice->type)
        {
            default:
                /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
            case CSR_SDIO_DEVICE_TYPE_SDIO:
                CsrSdioMasterInterruptAcknowledge(sdioDevice->masterDevice);
                break;
#endif
#ifdef CSR_SDIO_USE_CSPI
            case CSR_SDIO_DEVICE_TYPE_CSPI:
                CsrSpiMasterInterruptAcknowledge(sdioDevice->masterDevice);
                break;
#endif
        }
    }
}

CsrResult CsrSdioFunctionDriverRegister(CsrSdioFunctionDriver *functionDriver)
{
    CsrUint8 i, functionNumber;

    CSR_LOG_TEXT_REGISTER(&CsrSdioLto, "SDIO", CSR_ARRAY_SIZE(subOrigins), subOrigins);

    /* Create the shared mutex */
    CsrGlobalMutexLock();
    if (registerCount == 0)
    {
        CsrResult result;

        result = CsrMutexCreate(&sharedMutex);
        if (result == CSR_RESULT_SUCCESS)
        {
            registerCount = 1;
        }
        else
        {
            CsrGlobalMutexUnlock();
            CSR_LOG_TEXT_ERROR((CsrSdioLto, 0, "Unable to create shared mutex"));
            return CSR_RESULT_FAILURE;
        }
    }
    else
    {
        registerCount += 1;
    }
    CsrGlobalMutexUnlock();

    CsrMutexLock(&sharedMutex);

    CSR_LOG_TEXT_INFO((CsrSdioLto, 0, "Registering Function Driver %08X", (CsrUint32) (CsrUintptr) functionDriver));

    /* Check if device initialisation should be performed */
    if ((functionDriversCount == 0) && (attachedFunctionsCount == 0))
    {
        devicesInitialise();
    }
    functionDriversCount += 1;

    CsrMutexUnlock(&sharedMutex);

#ifdef CSR_SDIO_USE_SDIO
    for (i = 0; i < sdioMasterDevicesCount; ++i)
    {
        CsrSdioMasterDevice *sdioMasterDevice = &sdioMasterDevices[i];
        CsrSdioDevice *sdioDevice = sdioMasterDevice->driverData;

        if (sdioDevice != NULL)
        {
            for (functionNumber = 1; functionNumber <= sdioDevice->sdioFunctionsCount; ++functionNumber)
            {
                CsrSdioFunctionPrivate *functionPrivate = &sdioDevice->sdioFunctions[functionNumber - 1];

                CsrMutexLock(&sharedMutex);
                if ((functionPrivate->state == CSR_SDIO_FUNCTION_STATE_DETACHED) &&
                    isMatch(functionDriver, &functionPrivate->pub.sdioId))
                {
                    functionPrivate->state = CSR_SDIO_FUNCTION_STATE_ATTACHING;
                    functionPrivate->functionDriver = functionDriver;
                    attachedFunctionsCount += 1;
                    CSR_LOG_TEXT_INFO((CsrSdioLto, CSR_SDIO_LTSO_SDIO, "Attaching function %08X to Function Driver %08X", (CsrUint32) (CsrUintptr) (&functionPrivate->pub), (CsrUint32) (CsrUintptr) functionDriver));
                    CsrMutexUnlock(&sharedMutex);
                    functionPrivate->functionDriver->inserted(&functionPrivate->pub);
                }
                else
                {
                    CsrMutexUnlock(&sharedMutex);
                }
            }
        }
    }
#endif
#ifdef CSR_SDIO_USE_CSPI
    for (i = 0; i < spiMasterDevicesCount; ++i)
    {
        CsrSpiMasterDevice *spiMasterDevice = &spiMasterDevices[i];
        CsrSdioDevice *sdioDevice = spiMasterDevice->driverData;

        if (sdioDevice != NULL)
        {
            for (functionNumber = 1; functionNumber <= sdioDevice->sdioFunctionsCount; ++functionNumber)
            {
                CsrSdioFunctionPrivate *functionPrivate = &sdioDevice->sdioFunctions[functionNumber - 1];

                CsrMutexLock(&sharedMutex);
                if ((functionPrivate->state == CSR_SDIO_FUNCTION_STATE_DETACHED) &&
                    isMatch(functionDriver, &functionPrivate->pub.sdioId))
                {
                    functionPrivate->state = CSR_SDIO_FUNCTION_STATE_ATTACHING;
                    functionPrivate->functionDriver = functionDriver;
                    attachedFunctionsCount += 1;
                    CSR_LOG_TEXT_INFO((CsrSdioLto, CSR_SDIO_LTSO_SDIO, "Attaching function %08X to Function Driver %08X", (CsrUint32) (CsrUintptr) (&functionPrivate->pub), (CsrUint32) (CsrUintptr) functionDriver));
                    CsrMutexUnlock(&sharedMutex);
                    functionPrivate->functionDriver->inserted(&functionPrivate->pub);
                }
                else
                {
                    CsrMutexUnlock(&sharedMutex);
                }
            }
        }
    }
#endif

    return CSR_RESULT_SUCCESS;
}

void CsrSdioFunctionDriverUnregister(CsrSdioFunctionDriver *functionDriver)
{
    CsrUint8 i, functionNumber;

    /* Call removed callback on all function drivers */
#ifdef CSR_SDIO_USE_SDIO
    for (i = 0; i < sdioMasterDevicesCount; ++i)
    {
        CsrSdioMasterDevice *sdioMasterDevice = &sdioMasterDevices[i];
        CsrSdioDevice *sdioDevice = sdioMasterDevice->driverData;

        if (sdioDevice != NULL)
        {
            for (functionNumber = 1; functionNumber <= sdioDevice->sdioFunctionsCount; ++functionNumber)
            {
                CsrSdioFunctionPrivate *functionPrivate = &sdioDevice->sdioFunctions[functionNumber - 1];

                CsrMutexLock(&sharedMutex);
                if (((functionPrivate->state == CSR_SDIO_FUNCTION_STATE_ATTACHING) ||
                     (functionPrivate->state == CSR_SDIO_FUNCTION_STATE_ATTACHED)) &&
                    (functionPrivate->functionDriver == functionDriver))
                {
                    functionPrivate->state = CSR_SDIO_FUNCTION_STATE_DETACHING;
                    CsrMutexUnlock(&sharedMutex);
                    functionDriver->removed(&functionPrivate->pub);
                }
                else
                {
                    CsrMutexUnlock(&sharedMutex);
                }
            }
        }
    }
#endif
#ifdef CSR_SDIO_USE_CSPI
    for (i = 0; i < spiMasterDevicesCount; ++i)
    {
        CsrSpiMasterDevice *spiMasterDevice = &spiMasterDevices[i];
        CsrSdioDevice *sdioDevice = spiMasterDevice->driverData;

        if (sdioDevice != NULL)
        {
            for (functionNumber = 1; functionNumber <= sdioDevice->sdioFunctionsCount; ++functionNumber)
            {
                CsrSdioFunctionPrivate *functionPrivate = &sdioDevice->sdioFunctions[functionNumber - 1];

                CsrMutexLock(&sharedMutex);
                if (((functionPrivate->state == CSR_SDIO_FUNCTION_STATE_ATTACHING) ||
                     (functionPrivate->state == CSR_SDIO_FUNCTION_STATE_ATTACHED)) &&
                    (functionPrivate->functionDriver == functionDriver))
                {
                    functionPrivate->state = CSR_SDIO_FUNCTION_STATE_DETACHING;
                    CsrMutexUnlock(&sharedMutex);
                    functionDriver->removed(&functionPrivate->pub);
                }
                else
                {
                    CsrMutexUnlock(&sharedMutex);
                }
            }
        }
    }
#endif

    CsrMutexLock(&sharedMutex);

    CSR_LOG_TEXT_INFO((CsrSdioLto, 0, "Unregistering Function Driver %08X", (CsrUint32) (CsrUintptr) functionDriver));

    /* Check if device deinitialisation should be performed */
    functionDriversCount -= 1;
    if ((functionDriversCount == 0) && (attachedFunctionsCount == 0))
    {
        devicesDeinitialise();
    }

    /* Check if shared mutex should be destroyed, else unlock it */
    CsrGlobalMutexLock();
    registerCount -= 1;
    if ((registerCount == 0) && (functionDriversCount == 0) && (attachedFunctionsCount == 0))
    {
        CsrMutexDestroy(&sharedMutex);
        CsrGlobalMutexUnlock();
    }
    else
    {
        CsrGlobalMutexUnlock();
        CsrMutexUnlock(&sharedMutex);
    }
}

CsrResult CsrSdioFunctionEnable(CsrSdioFunction *function)
{
    CsrSdioDevice *sdioDevice = function->device;
    CsrUint32 i;
    CsrUint8 data;
    CsrResult result = CSR_SDIO_RESULT_TIMEOUT;

    busSyncClaim(function);

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            CsrSdioMasterRead8(sdioDevice->masterDevice, 0, 0x02, &data);
            data |= 1U << function->sdioId.sdioFunction;
            CsrSdioMasterWrite8(sdioDevice->masterDevice, 0, 0x02, data);
            for (i = 0; i < ENABLE_DISABLE_RETRIES; ++i)
            {
                CsrSdioMasterRead8(sdioDevice->masterDevice, 0, 0x03, &data);
                if (data & (1U << function->sdioId.sdioFunction))
                {
                    result = CSR_RESULT_SUCCESS;
                    break;
                }
            }
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            CsrSdioCspiRead8(sdioDevice->masterDevice, 0, 0x02, &data);
            data |= 1U << function->sdioId.sdioFunction;
            CsrSdioCspiWrite8(sdioDevice->masterDevice, 0, 0x02, data);
            for (i = 0; i < ENABLE_DISABLE_RETRIES; ++i)
            {
                CsrSdioCspiRead8(sdioDevice->masterDevice, 0, 0x03, &data);
                if (data & (1U << function->sdioId.sdioFunction))
                {
                    result = CSR_RESULT_SUCCESS;
                    break;
                }
            }
            break;
#endif
    }

    busSyncRelease(function);

    return result;
}

CsrResult CsrSdioFunctionDisable(CsrSdioFunction *function)
{
    CsrSdioDevice *sdioDevice = function->device;
    CsrUint32 i;
    CsrUint8 data;
    CsrResult result = CSR_SDIO_RESULT_TIMEOUT;

    busSyncClaim(function);

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            CsrSdioMasterRead8(sdioDevice->masterDevice, 0, 0x02, &data);
            data &= ~(1U << function->sdioId.sdioFunction);
            CsrSdioMasterWrite8(sdioDevice->masterDevice, 0, 0x02, data);
            for (i = 0; i < ENABLE_DISABLE_RETRIES; ++i)
            {
                CsrSdioMasterRead8(sdioDevice->masterDevice, 0, 0x03, &data);
                if (!(data & (1U << function->sdioId.sdioFunction)))
                {
                    result = CSR_RESULT_SUCCESS;
                    break;
                }
            }
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            CsrSdioCspiRead8(sdioDevice->masterDevice, 0, 0x02, &data);
            data &= ~(1U << function->sdioId.sdioFunction);
            CsrSdioCspiWrite8(sdioDevice->masterDevice, 0, 0x02, data);
            for (i = 0; i < ENABLE_DISABLE_RETRIES; ++i)
            {
                CsrSdioCspiRead8(sdioDevice->masterDevice, 0, 0x03, &data);
                if (!(data & (1U << function->sdioId.sdioFunction)))
                {
                    result = CSR_RESULT_SUCCESS;
                    break;
                }
            }
            break;
#endif
    }

    busSyncRelease(function);

    return result;
}

CsrResult CsrSdioInterruptEnable(CsrSdioFunction *function)
{
    CsrSdioDevice *sdioDevice = function->device;
    CsrResult result = CSR_RESULT_SUCCESS;

    busSyncClaim(function);

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            sdioDevice->localInterruptEnable |= 1U << function->sdioId.sdioFunction;
            result = CsrSdioMasterWrite8(sdioDevice->masterDevice,
            0, 0x04, (CsrUint8) (sdioDevice->localInterruptEnable | 1));
            if (result != CSR_RESULT_SUCCESS)
            {
                result = sdioTranslateResult(result);
                sdioDevice->localInterruptEnable &= ~(1U << function->sdioId.sdioFunction);
            }
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            sdioDevice->localInterruptEnable |= 1U << function->sdioId.sdioFunction;
            CsrSdioCspiWrite8(sdioDevice->masterDevice, 0, 0x04, (CsrUint8) (sdioDevice->localInterruptEnable | 1));
            break;
#endif
    }

    busSyncRelease(function);

    /* Acknowledge interrupt if one was received while all interrupts were disabled */
    interruptAcknowledge(function, 0);

    return result;
}

CsrResult CsrSdioInterruptDisable(CsrSdioFunction *function)
{
    CsrSdioDevice *sdioDevice = function->device;
    CsrResult result = CSR_RESULT_SUCCESS;
    CsrUint8 localInterruptEnable;

    busSyncClaim(function);

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            localInterruptEnable = sdioDevice->localInterruptEnable & ~(1U << function->sdioId.sdioFunction);
            result = CsrSdioMasterWrite8(sdioDevice->masterDevice,
            0, 0x04, (CsrUint8) ((localInterruptEnable != 0) ? (localInterruptEnable | 1) : 0));
            if (result == CSR_RESULT_SUCCESS)
            {
                sdioDevice->localInterruptEnable = localInterruptEnable;
            }
            else
            {
                result = sdioTranslateResult(result);
            }
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            localInterruptEnable = sdioDevice->localInterruptEnable & ~(1U << function->sdioId.sdioFunction);
            CsrSdioCspiWrite8(sdioDevice->masterDevice,
            0, 0x04, (CsrUint8) ((localInterruptEnable != 0) ? (localInterruptEnable | 1) : 0));
            sdioDevice->localInterruptEnable = localInterruptEnable;
            break;
#endif
    }

    busSyncRelease(function);

    return result;
}

void CsrSdioInterruptAcknowledge(CsrSdioFunction *function)
{
    interruptAcknowledge(function, function->sdioId.sdioFunction);
}

void CsrSdioInsertedAcknowledge(CsrSdioFunction *function, CsrResult result)
{
    CsrSdioFunctionPrivate *functionPrivate = function->priv;

    CsrMutexLock(&sharedMutex);

    if ((functionPrivate->state == CSR_SDIO_FUNCTION_STATE_ATTACHING) &&
        (functionPrivate->functionDriver != NULL))
    {
        if (result != CSR_RESULT_SUCCESS)
        {
            CSR_LOG_TEXT_INFO((CsrSdioLto, 0, "Detaching function %08X from Function Driver %08X (Rejected)", (CsrUint32) (CsrUintptr) (&functionPrivate->pub), (CsrUint32) (CsrUintptr) functionPrivate->functionDriver));

            functionPrivate->state = CSR_SDIO_FUNCTION_STATE_DETACHED;
            functionPrivate->functionDriver = NULL;
            attachedFunctionsCount -= 1;

            /* Check if device deinitialisation should be performed */
            if ((functionDriversCount == 0) && (attachedFunctionsCount == 0))
            {
                devicesDeinitialise();
            }

            /* Check if shared mutex should be destroyed */
            CsrGlobalMutexLock();
            if ((registerCount == 0) && (functionDriversCount == 0) && (attachedFunctionsCount == 0))
            {
                CsrMutexDestroy(&sharedMutex);
                CsrGlobalMutexUnlock();
                return;
            }
            else
            {
                CsrGlobalMutexUnlock();
            }
        }
        else
        {
            functionPrivate->state = CSR_SDIO_FUNCTION_STATE_ATTACHED;
        }
    }

    CsrMutexUnlock(&sharedMutex);
}

void CsrSdioRemovedAcknowledge(CsrSdioFunction *function)
{
    CsrSdioFunctionPrivate *functionPrivate = function->priv;

    CsrMutexLock(&sharedMutex);

    if ((functionPrivate->state == CSR_SDIO_FUNCTION_STATE_DETACHING) &&
        (functionPrivate->functionDriver != NULL))
    {
        CSR_LOG_TEXT_INFO((CsrSdioLto, 0, "Detaching function %08X from Function Driver %08X (Removed)", (CsrUint32) (CsrUintptr) (&functionPrivate->pub), (CsrUint32) (CsrUintptr) functionPrivate->functionDriver));

        functionPrivate->state = CSR_SDIO_FUNCTION_STATE_DETACHED;
        functionPrivate->functionDriver = NULL;
        attachedFunctionsCount -= 1;

        /* Check if device deinitialisation should be performed */
        if ((functionDriversCount == 0) && (attachedFunctionsCount == 0))
        {
            devicesDeinitialise();
        }

        /* Check if shared mutex should be destroyed */
        CsrGlobalMutexLock();
        if ((registerCount == 0) && (functionDriversCount == 0) && (attachedFunctionsCount == 0))
        {
            CsrMutexDestroy(&sharedMutex);
            CsrGlobalMutexUnlock();
            return;
        }
        else
        {
            CsrGlobalMutexUnlock();
        }
    }

    CsrMutexUnlock(&sharedMutex);
}

void CsrSdioSuspendAcknowledge(CsrSdioFunction *function, CsrResult result)
{
    /* This function is never called because this implementation never calls
       the suspend callback. */
}

void CsrSdioResumeAcknowledge(CsrSdioFunction *function, CsrResult result)
{
    /* This function is never called because this implementation never calls
       the resume callback. */
}

CsrResult CsrSdioBlockSizeSet(CsrSdioFunction *function, CsrUint16 blockSize)
{
    CsrSdioDevice *sdioDevice = function->device;
    CsrResult result = CSR_RESULT_SUCCESS;

    busSyncClaim(function);

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
        {
            CsrSdioMasterDevice *sdioMasterDevice = sdioDevice->masterDevice;
            blockSize = CSRMIN(blockSize, sdioDevice->maxBlockSize[function->sdioId.sdioFunction]);
            result = sdioTranslateResult(CsrSdioMasterBlockSizeSet(sdioMasterDevice,
                    function->sdioId.sdioFunction, blockSize));
            function->blockSize = sdioMasterDevice->blockSize[function->sdioId.sdioFunction];
            break;
        }
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            break;
#endif
    }

    busSyncRelease(function);

    return result;
}

CsrResult CsrSdioMaxBusClockFrequencySet(CsrSdioFunction *function, CsrUint32 maxFrequency)
{
    CsrSdioFunctionPrivate *functionPrivate = function->priv;
    CsrSdioDevice *sdioDevice = function->device;
    CsrResult result = CSR_RESULT_SUCCESS;
    CsrUint8 functionNumber;
    CsrUint8 i;

    busSyncClaim(function);

    /* Save the specified max frequency for the function */
    functionPrivate->maxFrequency = maxFrequency;

    /* Determine the frequency to configure */
    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
        {
            CsrSdioMasterDevice *sdioMasterDevice = sdioDevice->masterDevice;
            for (i = 0; i < sdioMasterDevicesCount; ++i)
            {
                CsrSdioMasterDevice *sdioMasterDeviceI = &sdioMasterDevices[i];
                CsrSdioDevice *sdioDeviceI = sdioMasterDeviceI->driverData;
                if ((sdioDeviceI != NULL) && (sdioMasterDevice->busIndex == sdioMasterDeviceI->busIndex))
                {
                    /* Maximum frequency supported by the device (from the CIS) */
                    maxFrequency = CSRMIN(sdioDeviceI->maxFrequency, maxFrequency);

                    /* Maximum frequency specified for each of the functions (not including function 0) */
                    for (functionNumber = 1; functionNumber <= sdioDeviceI->sdioFunctionsCount; ++functionNumber)
                    {
                        if (sdioDeviceI->sdioFunctions[functionNumber - 1].maxFrequency != 0)
                        {
                            maxFrequency = CSRMIN(sdioDeviceI->sdioFunctions[functionNumber - 1].maxFrequency, maxFrequency);
                        }
                    }
                }
            }
            result = sdioTranslateResult(CsrSdioMasterBusClockFrequencySet(sdioMasterDevice, &maxFrequency));
            break;
        }
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
        {
            CsrSpiMasterDevice *spiMasterDevice = sdioDevice->masterDevice;
            for (i = 0; i < spiMasterDevicesCount; ++i)
            {
                CsrSpiMasterDevice *spiMasterDeviceI = &spiMasterDevices[i];
                CsrSdioDevice *sdioDeviceI = spiMasterDeviceI->driverData;
                if ((sdioDeviceI != NULL) && (spiMasterDevice->busIndex == spiMasterDeviceI->busIndex))
                {
                    /* Maximum frequency supported by the device (from the CIS) */
                    maxFrequency = CSRMIN(sdioDeviceI->maxFrequency, maxFrequency);

                    /* Maximum frequency specified for each of the functions (not including function 0) */
                    for (functionNumber = 1; functionNumber <= sdioDeviceI->sdioFunctionsCount; ++functionNumber)
                    {
                        if (sdioDeviceI->sdioFunctions[functionNumber - 1].maxFrequency != 0)
                        {
                            maxFrequency = CSRMIN(sdioDeviceI->sdioFunctions[functionNumber - 1].maxFrequency, maxFrequency);
                        }
                    }
                }
            }
            CsrSpiMasterBusClockFrequencySet(spiMasterDevice, &maxFrequency);
            break;
        }
#endif
    }

    busSyncRelease(function);

    return result;
}

CsrResult CsrSdioRead8(CsrSdioFunction *function, CsrUint32 address, CsrUint8 *data)
{
    CsrSdioDevice *sdioDevice = function->device;
    CsrResult result = CSR_RESULT_SUCCESS;
    CsrUint8 functionNumber;

    functionNumber = (CsrUint8) (address & FORCE_FUNCTION_0_ADDRESS ? 0 : function->sdioId.sdioFunction);
    address = address & ~FORCE_FUNCTION_0_ADDRESS;

    busSyncClaim(function);

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            result = sdioTranslateResult(CsrSdioMasterRead8(sdioDevice->masterDevice,
                functionNumber, address, data));
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            CsrSdioCspiRead8(sdioDevice->masterDevice,
            functionNumber, address, data);
            break;
#endif
    }

    busSyncRelease(function);

    return result;
}

CsrResult CsrSdioWrite8(CsrSdioFunction *function, CsrUint32 address, CsrUint8 data)
{
    CsrSdioDevice *sdioDevice = function->device;
    CsrResult result = CSR_RESULT_SUCCESS;
    CsrUint8 functionNumber;

    functionNumber = (CsrUint8) (address & FORCE_FUNCTION_0_ADDRESS ? 0 : function->sdioId.sdioFunction);
    address = address & ~FORCE_FUNCTION_0_ADDRESS;

    busSyncClaim(function);

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            result = sdioTranslateResult(CsrSdioMasterWrite8(sdioDevice->masterDevice,
                functionNumber, address, data));
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            CsrSdioCspiWrite8(sdioDevice->masterDevice,
            functionNumber, address, data);
            break;
#endif
    }

    busSyncRelease(function);

    return result;
}

static void read8Async(CsrSdioFunction *function, CsrUint32 address, CsrUint8 *data, CsrSdioAsyncCallback callback)
{
    CsrSdioFunctionPrivate *functionPrivate = function->priv;
    CsrSdioDevice *sdioDevice = function->device;
    CsrUint8 functionNumber;

    functionPrivate->operationType = CSR_SDIO_OPERATION_TYPE_READ8;
    functionPrivate->operationCallback = callback;
    sdioDevice->operationHandle = functionPrivate;

    functionNumber = (CsrUint8) (address & FORCE_FUNCTION_0_ADDRESS ? 0 : function->sdioId.sdioFunction);
    address = address & ~FORCE_FUNCTION_0_ADDRESS;

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            CsrSdioMasterRead8Async(sdioDevice->masterDevice, functionNumber, address, data, sdioOperationCallback);
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            CsrSdioCspiRead8Async(sdioDevice->masterDevice, functionNumber, address, data, spiOperationCallback);
            break;
#endif
    }
}

void CsrSdioRead8Async(CsrSdioFunction *function, CsrUint32 address, CsrUint8 *data, CsrSdioAsyncCallback callback)
{
    CsrSdioFunctionPrivate *functionPrivate = function->priv;
    CsrSdioDevice *sdioDevice = function->device;

    /* Protect against other threads and the callbacks */
    if (!functionPrivate->callbackContext)
    {
        CsrMutexLock(&sharedMutex);
    }
    callbackInhibitEnter(sdioDevice);

    if (!sdioDevice->bus->claimed)
    {
        sdioDevice->bus->claimed = TRUE;
        read8Async(function, address, data, callback);
    }
    else
    {
        queueElementSave(functionPrivate, CSR_SDIO_OPERATION_TYPE_READ8, address, 0, 0, data, callback);
        waitQueuePush(functionPrivate);
    }

    /* Lift protection against other threads and the callbacks */
    callbackInhibitLeave(sdioDevice);
    if (!functionPrivate->callbackContext)
    {
        CsrMutexUnlock(&sharedMutex);
    }
}

static void write8Async(CsrSdioFunction *function, CsrUint32 address, CsrUint8 data, CsrSdioAsyncCallback callback)
{
    CsrSdioFunctionPrivate *functionPrivate = function->priv;
    CsrSdioDevice *sdioDevice = function->device;
    CsrUint8 functionNumber;

    functionPrivate->operationType = CSR_SDIO_OPERATION_TYPE_WRITE8;
    functionPrivate->operationCallback = callback;
    sdioDevice->operationHandle = functionPrivate;

    functionNumber = (CsrUint8) (address & FORCE_FUNCTION_0_ADDRESS ? 0 : function->sdioId.sdioFunction);
    address = address & ~FORCE_FUNCTION_0_ADDRESS;

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            CsrSdioMasterWrite8Async(sdioDevice->masterDevice, functionNumber, address, data, sdioOperationCallback);
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            CsrSdioCspiWrite8Async(sdioDevice->masterDevice, functionNumber, address, data, spiOperationCallback);
            break;
#endif
    }
}

void CsrSdioWrite8Async(CsrSdioFunction *function, CsrUint32 address, CsrUint8 data, CsrSdioAsyncCallback callback)
{
    CsrSdioFunctionPrivate *functionPrivate = function->priv;
    CsrSdioDevice *sdioDevice = function->device;

    /* Protect against other threads and the callbacks */
    if (!functionPrivate->callbackContext)
    {
        CsrMutexLock(&sharedMutex);
    }
    callbackInhibitEnter(sdioDevice);

    if (!sdioDevice->bus->claimed)
    {
        sdioDevice->bus->claimed = TRUE;
        write8Async(function, address, data, callback);
    }
    else
    {
        queueElementSave(functionPrivate, CSR_SDIO_OPERATION_TYPE_WRITE8, address, data, 0, NULL, callback);
        waitQueuePush(functionPrivate);
    }

    /* Lift protection against other threads and the callbacks */
    callbackInhibitLeave(sdioDevice);
    if (!functionPrivate->callbackContext)
    {
        CsrMutexUnlock(&sharedMutex);
    }
}

CsrResult CsrSdioRead16(CsrSdioFunction *function, CsrUint32 address, CsrUint16 *data)
{
    CsrSdioDevice *sdioDevice = function->device;
    CsrResult result = CSR_RESULT_SUCCESS;

    busSyncClaim(function);

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            result = sdioTranslateResult(CsrSdioSdioRead16(sdioDevice->masterDevice,
                function->sdioId.sdioFunction, address, data));
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            CsrSdioCspiRead16(sdioDevice->masterDevice,
            function->sdioId.sdioFunction, address, data);
            break;
#endif
    }

    busSyncRelease(function);

    return result;
}

CsrResult CsrSdioWrite16(CsrSdioFunction *function, CsrUint32 address, CsrUint16 data)
{
    CsrSdioDevice *sdioDevice = function->device;
    CsrResult result = CSR_RESULT_SUCCESS;

    busSyncClaim(function);

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            result = sdioTranslateResult(CsrSdioSdioWrite16(sdioDevice->masterDevice,
                function->sdioId.sdioFunction, address, data));
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            CsrSdioCspiWrite16(sdioDevice->masterDevice,
            function->sdioId.sdioFunction, address, data);
            break;
#endif
    }

    busSyncRelease(function);

    return result;
}

void CsrSdioRead16Async(CsrSdioFunction *function, CsrUint32 address, CsrUint16 *data, CsrSdioAsyncCallback callback)
{
    /* Not implemented */
}

void CsrSdioWrite16Async(CsrSdioFunction *function, CsrUint32 address, CsrUint16 data, CsrSdioAsyncCallback callback)
{
    /* Not implemented */
}

CsrResult CsrSdioF0Read8(CsrSdioFunction *function, CsrUint32 address, CsrUint8 *data)
{
    return CsrSdioRead8(function, (CsrUint32) (address | FORCE_FUNCTION_0_ADDRESS), data);
}

CsrResult CsrSdioF0Write8(CsrSdioFunction *function, CsrUint32 address, CsrUint8 data)
{
    return CsrSdioWrite8(function, (CsrUint32) (address | FORCE_FUNCTION_0_ADDRESS), data);
}

void CsrSdioF0Read8Async(CsrSdioFunction *function, CsrUint32 address, CsrUint8 *data, CsrSdioAsyncCallback callback)
{
    CsrSdioRead8Async(function, (CsrUint32) (address | FORCE_FUNCTION_0_ADDRESS), data, callback);
}

void CsrSdioF0Write8Async(CsrSdioFunction *function, CsrUint32 address, CsrUint8 data, CsrSdioAsyncCallback callback)
{
    CsrSdioWrite8Async(function, (CsrUint32) (address | FORCE_FUNCTION_0_ADDRESS), data, callback);
}

CsrResult CsrSdioRead(CsrSdioFunction *function, CsrUint32 address, void *data, CsrUint32 length)
{
    CsrSdioDevice *sdioDevice = function->device;
    CsrResult result = CSR_RESULT_SUCCESS;

    busSyncClaim(function);

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            result = sdioTranslateResult(CsrSdioSdioRead(sdioDevice->masterDevice,
                function->sdioId.sdioFunction, address, data, length));
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            CsrSdioCspiRead(sdioDevice->masterDevice,
            function->sdioId.sdioFunction, address, data, length);
            break;
#endif
    }

    busSyncRelease(function);

    return result;
}

CsrResult CsrSdioWrite(CsrSdioFunction *function, CsrUint32 address, const void *data, CsrUint32 length)
{
    CsrSdioDevice *sdioDevice = function->device;
    CsrResult result = CSR_RESULT_SUCCESS;

    busSyncClaim(function);

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            result = sdioTranslateResult(CsrSdioSdioWrite(sdioDevice->masterDevice,
                function->sdioId.sdioFunction, address, data, length));
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            CsrSdioCspiWrite(sdioDevice->masterDevice,
            function->sdioId.sdioFunction, address, data, length);
            break;
#endif
    }

    busSyncRelease(function);

    return result;
}

static void readAsync(CsrSdioFunction *function, CsrUint32 address, void *data, CsrUint32 length, CsrSdioAsyncCallback callback)
{
    CsrSdioFunctionPrivate *functionPrivate = function->priv;
    CsrSdioDevice *sdioDevice = function->device;

    functionPrivate->operationType = CSR_SDIO_OPERATION_TYPE_READ;
    functionPrivate->operationCallback = callback;
    sdioDevice->operationHandle = functionPrivate;

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            CsrSdioSdioReadAsync(sdioDevice->masterDevice, function->sdioId.sdioFunction, address, data, length, sdioOperationCallback);
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            CsrSdioCspiReadAsync(sdioDevice->masterDevice, function->sdioId.sdioFunction, address, data, length, spiOperationCallback);
            break;
#endif
    }
}

void CsrSdioReadAsync(CsrSdioFunction *function, CsrUint32 address, void *data, CsrUint32 length, CsrSdioAsyncCallback callback)
{
    CsrSdioFunctionPrivate *functionPrivate = function->priv;
    CsrSdioDevice *sdioDevice = function->device;

    /* Protect against other threads and the callbacks */
    if (!functionPrivate->callbackContext)
    {
        CsrMutexLock(&sharedMutex);
    }
    callbackInhibitEnter(sdioDevice);

    if (!sdioDevice->bus->claimed)
    {
        sdioDevice->bus->claimed = TRUE;
        readAsync(function, address, data, length, callback);
    }
    else
    {
        queueElementSave(functionPrivate, CSR_SDIO_OPERATION_TYPE_READ, address, 0, length, data, callback);
        waitQueuePush(functionPrivate);
    }

    /* Lift protection against other threads and the callbacks */
    callbackInhibitLeave(sdioDevice);
    if (!functionPrivate->callbackContext)
    {
        CsrMutexUnlock(&sharedMutex);
    }
}

static void writeAsync(CsrSdioFunction *function, CsrUint32 address, const void *data, CsrUint32 length, CsrSdioAsyncCallback callback)
{
    CsrSdioFunctionPrivate *functionPrivate = function->priv;
    CsrSdioDevice *sdioDevice = function->device;

    functionPrivate->operationType = CSR_SDIO_OPERATION_TYPE_WRITE;
    functionPrivate->operationCallback = callback;
    sdioDevice->operationHandle = functionPrivate;

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            CsrSdioSdioWriteAsync(sdioDevice->masterDevice, function->sdioId.sdioFunction, address, data, length, sdioOperationCallback);
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            CsrSdioCspiWriteAsync(sdioDevice->masterDevice, function->sdioId.sdioFunction, address, data, length, spiOperationCallback);
            break;
#endif
    }
}

void CsrSdioWriteAsync(CsrSdioFunction *function, CsrUint32 address, const void *data, CsrUint32 length, CsrSdioAsyncCallback callback)
{
    CsrSdioFunctionPrivate *functionPrivate = function->priv;
    CsrSdioDevice *sdioDevice = function->device;

    /* Protect against other threads and the callbacks */
    if (!functionPrivate->callbackContext)
    {
        CsrMutexLock(&sharedMutex);
    }
    callbackInhibitEnter(sdioDevice);

    if (!sdioDevice->bus->claimed)
    {
        sdioDevice->bus->claimed = TRUE;
        writeAsync(function, address, data, length, callback);
    }
    else
    {
        queueElementSave(functionPrivate, CSR_SDIO_OPERATION_TYPE_WRITE, address, 0, length, (CsrUint8 *) data, callback);
        waitQueuePush(functionPrivate);
    }

    /* Lift protection against other threads and the callbacks */
    callbackInhibitLeave(sdioDevice);
    if (!functionPrivate->callbackContext)
    {
        CsrMutexUnlock(&sharedMutex);
    }
}

CsrResult CsrSdioPowerOn(CsrSdioFunction *function)
{
    CsrSdioDevice *sdioDevice = function->device;
    CsrResult result = CSR_RESULT_FAILURE;

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            result = CsrSdioMasterPowerOn(sdioDevice->masterDevice);
            if (result == CSR_RESULT_SUCCESS)
            {
                CsrUint16 vendorId, deviceId;
                CsrUint8 standardInterface[8];
                result = CsrSdioSdioDeviceInitialise(sdioDevice, &vendorId, &deviceId, standardInterface);
            }
            result = sdioTranslateResult(result);
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            result = CsrSpiMasterPowerOn(sdioDevice->masterDevice);
            if (result == CSR_RESULT_SUCCESS)
            {
                CsrUint16 vendorId, deviceId;
                CsrUint8 standardInterface[8];
                result = CsrSdioCspiDeviceInitialise(sdioDevice, &vendorId, &deviceId, standardInterface);
            }
            result = spiTranslateResult(result);
            break;
#endif
    }
    return result;
}

void CsrSdioPowerOff(CsrSdioFunction *function)
{
    CsrSdioDevice *sdioDevice = function->device;

    switch (sdioDevice->type)
    {
        default:
            /* Fallthrough */
#ifdef CSR_SDIO_USE_SDIO
        case CSR_SDIO_DEVICE_TYPE_SDIO:
            CsrSdioMasterPowerOff(function->device);
            break;
#endif
#ifdef CSR_SDIO_USE_CSPI
        case CSR_SDIO_DEVICE_TYPE_CSPI:
            CsrSpiMasterPowerOff(function->device);
            break;
#endif
    }
}

CsrResult CsrSdioHardReset(CsrSdioFunction *function)
{
    return CSR_SDIO_RESULT_NOT_RESET;
}

void CsrSdioFunctionActive(CsrSdioFunction *function)
{
}

void CsrSdioFunctionIdle(CsrSdioFunction *function)
{
}
