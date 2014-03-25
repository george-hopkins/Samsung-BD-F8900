/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_time.h"
#include "csr_util.h"
#include "csr_log_text_2.h"
#include "csr_sdio.h"

/* CSR UI Interface */
#include "csr_ui_task.h"
#include "csr_ui_prim.h"
#include "csr_ui_lib.h"

/* CSR APP Interface */
#include "csr_app_task.h"
#include "csr_app_prim.h"
#include "csr_app_lib.h"

/* SDIO Probe Headers */
#include "csr_sdio_probe_task.h"
#include "csr_sdio_probe_handler.h"
#include "csr_sdio_probe_sef.h"
#include "csr_sdio_probe_internal_lib.h"
#include "csr_sdio_probe_test.h"

static void timeoutCallback(CsrUint16 mi, void *mv);
static CsrSdioAsyncDsrCallback operationCallback(CsrSdioFunction *function, CsrResult result);

static CsrResult functionEnable(CsrSdioFunction *function)
{
    CsrResult result;
    CsrUint8 data;

    result = CsrSdioFunctionEnable(function);
    if (result != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] CsrSdioFunctionEnable failed (%u)", (CsrUint32) (CsrUintptr) function->device, result));
        return result;
    }

    result = CsrSdioF0Read8(function, 0x02, &data);
    if (result != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] Unable to read IOE (%u)", (CsrUint32) (CsrUintptr) function->device, result));
        return result;
    }
    if (~data & (1 << function->sdioId.sdioFunction))
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] IOE bit not properly set (%02X)", (CsrUint32) (CsrUintptr) function->device, data));
        return CSR_RESULT_FAILURE;
    }

    result = CsrSdioF0Read8(function, 0x03, &data);
    if (result != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] Unable to read IOR (%u)", (CsrUint32) (CsrUintptr) function->device, result));
        return result;
    }
    if (~data & (1 << function->sdioId.sdioFunction))
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] IOR bit not properly set (%02X)", (CsrUint32) (CsrUintptr) function->device, data));
        return CSR_RESULT_FAILURE;
    }

    CSR_LOG_TEXT_INFO((CsrSdioProbeLto, 0, "[%08X] Function Enabled", (CsrUint32) (CsrUintptr) function->device));
    return result;
}

static CsrResult functionDisable(CsrSdioFunction *function)
{
    CsrResult result;
    CsrUint8 data;

    result = CsrSdioFunctionDisable(function);
    if (result != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] CsrSdioFunctionDisable failed (%u)", (CsrUint32) (CsrUintptr) function->device, result));
        return result;
    }

    result = CsrSdioF0Read8(function, 0x02, &data);
    if (result != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] Unable to read IOE (%u)", (CsrUint32) (CsrUintptr) function->device, result));
        return result;
    }
    if (data & (1 << function->sdioId.sdioFunction))
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] IOE bit not properly cleared (%02X)", (CsrUint32) (CsrUintptr) function->device, data));
        return CSR_RESULT_FAILURE;
    }

    result = CsrSdioF0Read8(function, 0x03, &data);
    if (result != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] Unable to read IOR (%u)", (CsrUint32) (CsrUintptr) function->device, result));
        return result;
    }
    if (data & (1 << function->sdioId.sdioFunction))
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] IOR bit not properly cleared (%02X)", (CsrUint32) (CsrUintptr) function->device, data));
        return CSR_RESULT_FAILURE;
    }

    CSR_LOG_TEXT_INFO((CsrSdioProbeLto, 0, "[%08X] Function Disabled", (CsrUint32) (CsrUintptr) function->device));
    return result;
}

static CsrResult blockSizeSet(CsrSdioFunction *function)
{
    CsrResult result;

    result = CsrSdioBlockSizeSet(function, 2048);
    if (result != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] CsrSdioBlockSizeSet failed (%u)", (CsrUint32) (CsrUintptr) function->device, result));
    }
    else
    {
        CSR_LOG_TEXT_CONDITIONAL_INFO(function->blockSize != 0, (CsrSdioProbeLto, 0, "[%08X] Block size set to %u", (CsrUint32) (CsrUintptr) function->device, function->blockSize));
    }
    return result;
}

static CsrResult busFrequencySet(CsrSdioFunction *function)
{
    CsrResult result;

    result = CsrSdioMaxBusClockFrequencySet(function, 50000000);
    if (result != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] CsrSdioMaxBusClockFrequencySet failed (%u)", (CsrUint32) (CsrUintptr) function->device, result));
    }

    CSR_LOG_TEXT_INFO((CsrSdioProbeLto, 0, "[%08X] Bus clock frequency set", (CsrUint32) (CsrUintptr) function->device));
    return result;
}

static CsrResult interruptEnable(CsrSdioFunction *function)
{
    CsrResult result;
    CsrUint8 data;

    result = CsrSdioInterruptEnable(function);
    if (result != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] CsrSdioInterruptEnable failed (%u)", (CsrUint32) (CsrUintptr) function->device, result));
    }

    result = CsrSdioF0Read8(function, 0x04, &data);
    if (result != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] Unable to read IEN (%u)", (CsrUint32) (CsrUintptr) function->device, result));
        return result;
    }
    if ((data & ((1 << function->sdioId.sdioFunction) | 1)) != ((1 << function->sdioId.sdioFunction) | 1))
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] IEN bit not properly set (%02X)", (CsrUint32) (CsrUintptr) function->device, data));
        return CSR_RESULT_FAILURE;
    }

    CSR_LOG_TEXT_INFO((CsrSdioProbeLto, 0, "[%08X] Interrupt Enabled", (CsrUint32) (CsrUintptr) function->device));
    return result;
}

static CsrResult interruptDisable(CsrSdioFunction *function)
{
    CsrResult result;
    CsrUint8 data;

    result = CsrSdioInterruptDisable(function);
    if (result != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] CsrSdioInterruptDisable failed (%u)", (CsrUint32) (CsrUintptr) function->device, result));
    }

    result = CsrSdioF0Read8(function, 0x04, &data);
    if (result != CSR_RESULT_SUCCESS)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] Unable to read IEN (%u)", (CsrUint32) (CsrUintptr) function->device, result));
        return result;
    }
    if (data != 0)
    {
        CSR_LOG_TEXT_ERROR((CsrSdioProbeLto, 0, "[%08X] IEN bit not properly cleared (%02X)", (CsrUint32) (CsrUintptr) function->device, data));
        return CSR_RESULT_FAILURE;
    }

    CSR_LOG_TEXT_INFO((CsrSdioProbeLto, 0, "[%08X] Interrupt Disabled", (CsrUint32) (CsrUintptr) function->device));
    return result;
}

#define REGISTER_HOST_INT 0x1F25E

static CsrResult triggerInterrupt(CsrSdioFunction *function, CsrSdioProbeTestData *testData)
{
    switch (testData->chipType)
    {
        case CSR_SDIO_PROBE_CHIP_TYPE_WIFI:
            return CsrSdioWrite8(function, REGISTER_HOST_INT, 1);
        case CSR_SDIO_PROBE_CHIP_TYPE_BC6:
        case CSR_SDIO_PROBE_CHIP_TYPE_BC7:
            return CsrSdioF0Write8(function, 0xF0, (CsrUint8) ((function->sdioId.sdioFunction << 4) | 3));
        default:
            break;
    }
    return CSR_RESULT_SUCCESS;
}

static CsrResult clearInterrupt(CsrSdioFunction *function, CsrSdioProbeTestData *testData)
{
    switch (testData->chipType)
    {
        case CSR_SDIO_PROBE_CHIP_TYPE_WIFI:
            return CsrSdioWrite8(function, REGISTER_HOST_INT, 0);
        case CSR_SDIO_PROBE_CHIP_TYPE_BC6:
        case CSR_SDIO_PROBE_CHIP_TYPE_BC7:
            return CsrSdioF0Write8(function, 0xF0, (CsrUint8) (function->sdioId.sdioFunction << 4));
        default:
            break;
    }
    return CSR_RESULT_SUCCESS;
}

static void terminateTest(CsrSdioProbeInstanceData *instanceData, CsrSdioProbeTestData *testData)
{
    CsrSchedTimerCancel(testData->timeout, NULL, NULL);
    CsrSchedBgintUnreg(testData->bgint);
    CsrPmemFree(testData->data);
    testData->testType = CSR_SDIO_PROBE_TEST_TYPE_NONE;
    instanceData->testsRunning -= 1;
    CsrSdioProbeTestMenuUpdate(instanceData, testData->key);
}

static void reportTime(CsrSdioProbeTestData *testData)
{
    CsrSdioProbeInstanceData *instanceData = testData->instanceData;
    CsrSdioFunction *function = instanceData->functions[testData->key];

    CsrUint32 byteOperationAverage = testData->byteOperationsTime / testData->byteOperations;
    CsrUint32 dataOperationAverage = testData->dataOperationsTime / testData->dataOperations;

    CsrSdioProbeEventNotification(instanceData, TRUE, TRUE,
        "[%08X] Statistics:\n%u byte operations (%uus/op), %u data operations of %u byte (%uus/op).",
        (CsrUint32) (CsrUintptr) function->device,
        testData->byteOperations, byteOperationAverage,
        testData->dataOperations, testData->dataLength, dataOperationAverage);
}

static void reportInt(CsrSdioProbeTestData *testData)
{
    CsrSdioProbeInstanceData *instanceData = testData->instanceData;
    CsrSdioFunction *function = instanceData->functions[testData->key];

    CsrUint32 latency = testData->interruptsTime / testData->interrupts;

    CsrSdioProbeEventNotification(instanceData, TRUE, TRUE,
        "[%08X] Statistics:\n%u interrupts received with %uus average interrupt latency.",
        (CsrUint32) (CsrUintptr) function->device,
        testData->interrupts, latency);
}

static void addTimeByte(CsrSdioProbeTestData *testData)
{
    CsrTime now = CsrTimeGet(NULL);
    if (now > testData->startTime)
    {
        testData->byteOperationsTime += now - testData->startTime;
        testData->byteOperations += 1;
    }
}

static void addTimeData(CsrSdioProbeTestData *testData)
{
    CsrTime now = CsrTimeGet(NULL);
    if (now > testData->startTime)
    {
        testData->dataOperationsTime += now - testData->startTime;
        testData->dataOperations += 1;
    }
}

static void addTimeInt(CsrSdioProbeTestData *testData)
{
    CsrTime now = CsrTimeGet(NULL);
    if (now > testData->startTime)
    {
        testData->interruptsTime += now - testData->startTime;
        testData->interrupts += 1;
    }
}

static void bgintCallbackAsync(void *context)
{
    CsrSdioProbeTestData *testData = context;
    CsrSdioProbeInstanceData *instanceData = testData->instanceData;
    CsrSdioFunction *function = instanceData->functions[testData->key];

    if ((testData->testState == 6) && !testData->spurious && !testData->stop)
    {
        /* Reset timeout timer and run again */
        CsrSchedTimerCancel(testData->timeout, NULL, NULL);
        testData->timeout = CsrSchedTimerSet(1 * CSR_SCHED_SECOND, timeoutCallback, 0, testData);
        testData->testState = 2;
        testData->startTime = CsrTimeGet(NULL);
        CsrSdioF0Write8Async(function, testData->scratchRegister, 0xAA, operationCallback);
        return;
    }

    terminateTest(instanceData, testData);

    switch (testData->testState)
    {
        case 0:
            CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Failed to clear wakeup interrupt.", (CsrUint32) (CsrUintptr) function->device);
            break;
        case 1:
            CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Spurious interrupt.", (CsrUint32) (CsrUintptr) function->device);
            break;
        case 2:
            CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Write8 operation failed (%u).", (CsrUint32) (CsrUintptr) function->device, testData->result);
            break;
        case 3:
            CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Read8 operation failed (%u).", (CsrUint32) (CsrUintptr) function->device, testData->result);
            break;
        case 4:
            CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Write operation failed (%u).", (CsrUint32) (CsrUintptr) function->device, testData->result);
            break;
        case 5:
            CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Read operation failed (%u).", (CsrUint32) (CsrUintptr) function->device, testData->result);
            break;
        case 6:
            if (testData->spurious)
            {
                CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Spurious callbacks detected, stopping.", (CsrUint32) (CsrUintptr) function->device);
            }
            else if (testData->stop)
            {
                CsrResult result;
                result = interruptDisable(function);
                if (result == CSR_RESULT_SUCCESS)
                {
                    result = functionDisable(function);
                    if (result == CSR_RESULT_SUCCESS)
                    {
                        reportTime(testData);
                    }
                    else
                    {
                        CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Failed at function disable.", (CsrUint32) (CsrUintptr) function->device);
                    }
                }
                else
                {
                    CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Failed interrupt disable.", (CsrUint32) (CsrUintptr) function->device);
                }
            }
        default:
            break;
    }
}

static void bgintCallbackSync(void *context)
{
    CsrSdioProbeTestData *testData = context;
    CsrSdioProbeInstanceData *instanceData = testData->instanceData;
    CsrSdioFunction *function = instanceData->functions[testData->key];
    CsrResult result;

    switch (testData->testState)
    {
        case 0:
            result = CsrSdioF0Write8(function, testData->wakeupInterruptClearAddress, 1);
            if (result == CSR_RESULT_SUCCESS)
            {
                testData->testState = 1;
                CsrSdioInterruptAcknowledge(function);
                return;
            }
            else
            {
                terminateTest(instanceData, testData);
                CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Failed to clear wakeup interrupt.", (CsrUint32) (CsrUintptr) function->device);
            }
            break;
        case 1:
            terminateTest(instanceData, testData);
            CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Spurious interrupt.", (CsrUint32) (CsrUintptr) function->device);
            break;
        case 2:
            testData->startTime = CsrTimeGet(NULL);
            result = CsrSdioF0Read8(function, testData->scratchRegister, &testData->data8);
            if ((result == CSR_RESULT_SUCCESS) && (testData->data8 == 0xAA))
            {
                addTimeByte(testData);
                testData->testState = 3;
                CsrSchedBgintSet(testData->bgint);
            }
            else
            {
                terminateTest(instanceData, testData);
                CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Read8 operation failed (%u).", (CsrUint32) (CsrUintptr) function->device, result);
            }
            break;
        case 3:
            CsrMemSet(testData->data, 0xAA, testData->dataLength);
            testData->startTime = CsrTimeGet(NULL);
            result = CsrSdioWrite(function, 1, testData->data, testData->dataLength);
            if (result == CSR_RESULT_SUCCESS)
            {
                addTimeData(testData);
                testData->testState = 4;
                CsrSchedBgintSet(testData->bgint);
            }
            else
            {
                terminateTest(instanceData, testData);
                CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Write operation failed (%u).", (CsrUint32) (CsrUintptr) function->device, result);
            }
            break;
        case 4:
            CsrMemSet(testData->data, 0, testData->dataLength);
            testData->startTime = CsrTimeGet(NULL);
            result = CsrSdioRead(function, 1, testData->data, testData->dataLength);
            if (result == CSR_RESULT_SUCCESS)
            {
                CsrUint8 *data = testData->data;
                CsrUint32 i;
                addTimeData(testData);
                if (testData->dataVerify)
                {
                    for (i = 0; i < testData->dataLength; ++i)
                    {
                        if (data[i] != 0xAA)
                        {
                            result = CSR_RESULT_FAILURE;
                            break;
                        }
                    }
                }
            }
            if (result == CSR_RESULT_SUCCESS)
            {
                testData->testState = 5;
                CsrSchedBgintSet(testData->bgint);
            }
            else
            {
                terminateTest(instanceData, testData);
                CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Read operation failed (%u).", (CsrUint32) (CsrUintptr) function->device, result);
            }
            break;
        case 5:
            if (testData->spurious)
            {
                terminateTest(instanceData, testData);
                CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Spurious callbacks detected, stopping.", (CsrUint32) (CsrUintptr) function->device);
            }
            else if (testData->stop)
            {
                terminateTest(instanceData, testData);

                result = interruptDisable(function);
                if (result == CSR_RESULT_SUCCESS)
                {
                    result = functionDisable(function);
                    if (result == CSR_RESULT_SUCCESS)
                    {
                        reportTime(testData);
                    }
                    else
                    {
                        CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Failed at function disable.", (CsrUint32) (CsrUintptr) function->device);
                    }
                }
                else
                {
                    CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Failed interrupt disable.", (CsrUint32) (CsrUintptr) function->device);
                }
            }
            else
            {
                /* Reset timeout timer and run again */
                CsrSchedTimerCancel(testData->timeout, NULL, NULL);
                testData->timeout = CsrSchedTimerSet(1 * CSR_SCHED_SECOND, timeoutCallback, 0, testData);
                testData->startTime = CsrTimeGet(NULL);
                result = CsrSdioF0Write8(function, testData->scratchRegister, 0xAA);
                if (result == CSR_RESULT_SUCCESS)
                {
                    addTimeByte(testData);
                    testData->testState = 2;
                    CsrSchedBgintSet(testData->bgint);
                }
                else
                {
                    terminateTest(instanceData, testData);
                    CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Write8 operation failed (%u).", (CsrUint32) (CsrUintptr) function->device, result);
                }
            }
        default:
            break;
    }
}

static void bgintCallbackInt(void *context)
{
    CsrSdioProbeTestData *testData = context;
    CsrSdioProbeInstanceData *instanceData = testData->instanceData;
    CsrSdioFunction *function = instanceData->functions[testData->key];
    CsrResult result;

    switch (testData->testState)
    {
        case 0:
            result = CsrSdioF0Write8(function, testData->wakeupInterruptClearAddress, 1);
            if (result == CSR_RESULT_SUCCESS)
            {
                testData->testState = 1;
                CsrSdioInterruptAcknowledge(function);
                return;
            }
            else
            {
                terminateTest(instanceData, testData);
                CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Failed to clear wakeup interrupt.", (CsrUint32) (CsrUintptr) function->device);
            }
            break;
        case 1:
            terminateTest(instanceData, testData);
            CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Spurious interrupt.", (CsrUint32) (CsrUintptr) function->device);
            break;
        case 2:
            break;
        case 3:
            /* Reset timer to short duration for detecting spurious interrupts for a short time */
            clearInterrupt(function, testData);
            CsrSchedTimerCancel(testData->timeout, NULL, NULL);
            testData->timeout = CsrSchedTimerSet(10 * CSR_SCHED_MILLISECOND, timeoutCallback, 0, testData);
            testData->testState = 4;
            CsrSdioInterruptAcknowledge(function);
            break;
        case 4:
            /* Spurious interrupt */
            terminateTest(instanceData, testData);
            CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Spurious interrupt.", (CsrUint32) (CsrUintptr) function->device);
            break;
        default:
            break;
    }
}

static void operationDsrCallback(CsrSdioFunction *function, CsrResult result)
{
    CsrSdioProbeTestData *testData = function->driverData;
    CsrSchedBgintSet(testData->bgint);
}

static CsrSdioAsyncDsrCallback operationCallback(CsrSdioFunction *function, CsrResult result)
{
    CsrSdioProbeTestData *testData = function->driverData;

    testData->result = result;

    switch (testData->testState)
    {
        case 0:
            /* Cleared wakeup interrupt - acknowledge */
            testData->testState = 1;
            if (result == CSR_RESULT_SUCCESS)
            {
                CsrSdioInterruptAcknowledge(function);
            }
            else
            {
                return &operationDsrCallback;
            }
            break;
        case 1:
            /* Spurious operation complete callback */
            return &operationDsrCallback;
        case 2:
            if (result == CSR_RESULT_SUCCESS)
            {
                addTimeByte(testData);
                testData->testState = 3;
                testData->startTime = CsrTimeGet(NULL);
                CsrSdioF0Read8Async(function, testData->scratchRegister, &testData->data8, operationCallback);
            }
            else
            {
                return &operationDsrCallback;
            }
            break;
        case 3:
            if ((result == CSR_RESULT_SUCCESS) && (testData->data8 == 0xAA))
            {
                addTimeByte(testData);
                testData->testState = 4;
                CsrMemSet(testData->data, 0xAA, testData->dataLength);
                testData->startTime = CsrTimeGet(NULL);
                CsrSdioWriteAsync(function, 1, testData->data, testData->dataLength, operationCallback);
            }
            else
            {
                return &operationDsrCallback;
            }
            break;
        case 4:
            if (result == CSR_RESULT_SUCCESS)
            {
                addTimeData(testData);
                testData->testState = 5;
                CsrMemSet(testData->data, 0, testData->dataLength);
                testData->startTime = CsrTimeGet(NULL);
                CsrSdioReadAsync(function, 1, testData->data, testData->dataLength, operationCallback);
            }
            else
            {
                return &operationDsrCallback;
            }
            break;
        case 5:
            if (result == CSR_RESULT_SUCCESS)
            {
                CsrUint8 *data = testData->data;
                CsrUint32 i;
                addTimeData(testData);
                if (testData->dataVerify)
                {
                    for (i = 0; i < testData->dataLength; ++i)
                    {
                        if (data[i] != 0xAA)
                        {
                            testData->result = CSR_RESULT_FAILURE;
                            return &operationDsrCallback;
                        }
                    }
                }
                testData->testState = 6;
            }
            return &operationDsrCallback;
        default:
            /* Spurious operation complete */
            testData->spurious = TRUE;
            return &operationDsrCallback;
    }

    return NULL;
}

static void interruptDsrCallback(CsrSdioFunction *function)
{
    CsrSdioProbeTestData *testData = function->driverData;
    CsrSchedBgintSet(testData->bgint);
}

CsrSdioInterruptDsrCallback CsrSdioProbeInterruptCallback(CsrSdioFunction *function)
{
    CsrSdioProbeTestData *testData = function->driverData;

    if (testData->testType == CSR_SDIO_PROBE_TEST_TYPE_ASYNC)
    {
        if (testData->testState == 0)
        {
            CsrSdioF0Write8Async(function, testData->wakeupInterruptClearAddress, 1, operationCallback);
        }
        else
        {
            testData->spurious = TRUE;
            return &interruptDsrCallback;
        }
    }
    else if (testData->testType == CSR_SDIO_PROBE_TEST_TYPE_SYNC)
    {
        if (testData->testState != 0)
        {
            testData->spurious = TRUE;
        }
        return &interruptDsrCallback;
    }
    else if (testData->testType == CSR_SDIO_PROBE_TEST_TYPE_INT)
    {
        switch (testData->testState)
        {
            case 0:
                return &interruptDsrCallback;
            case 2:
                testData->testState = 3;
                testData->startTime = CsrTimeGet(NULL);
                CsrSdioInterruptAcknowledge(function);
                break;
            case 3:
                addTimeInt(testData);
                return &interruptDsrCallback;
            case 1:
            case 4:
            default:
                testData->spurious = TRUE;
                return &interruptDsrCallback;
        }
    }

    return NULL;
}

static void timeoutCallback(CsrUint16 mi, void *mv)
{
    CsrSdioProbeTestData *testData = mv;
    CsrSdioProbeInstanceData *instanceData = testData->instanceData;
    CsrSdioFunction *function = instanceData->functions[testData->key];

    switch (testData->testState)
    {
        case 0:
        case 1:
            /* Timeout waiting for wakeup interrupt or successfully cleared */
            testData->timeout = CsrSchedTimerSet(1 * CSR_SCHED_SECOND, timeoutCallback, 0, testData);
            if (testData->testType == CSR_SDIO_PROBE_TEST_TYPE_ASYNC)
            {
                testData->testState = 2;
                testData->startTime = CsrTimeGet(NULL);
                CsrSdioF0Write8Async(function, testData->scratchRegister, 0xAA, operationCallback);
            }
            else if (testData->testType == CSR_SDIO_PROBE_TEST_TYPE_SYNC)
            {
                CsrResult result;

                testData->startTime = CsrTimeGet(NULL);
                result = CsrSdioF0Write8(function, testData->scratchRegister, 0xAA);
                if (result == CSR_RESULT_SUCCESS)
                {
                    addTimeByte(testData);
                    testData->testState = 2;
                    CsrSchedBgintSet(testData->bgint);
                }
                else
                {
                    terminateTest(instanceData, testData);
                    CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Write8 operation failed (%u).", (CsrUint32) (CsrUintptr) function->device, result);
                }
            }
            else if (testData->testType == CSR_SDIO_PROBE_TEST_TYPE_INT)
            {
                CsrResult result;
                testData->testState = 2;
                result = triggerInterrupt(function, testData);
                if (result != CSR_RESULT_SUCCESS)
                {
                    terminateTest(instanceData, testData);
                    CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Trigger interrupt failed (%u).", (CsrUint32) (CsrUintptr) function->device, result);
                }
            }
            break;
        case 4:
            testData->timeout = CSR_SCHED_TID_INVALID;
            if (testData->testType == CSR_SDIO_PROBE_TEST_TYPE_INT)
            {
                if (testData->spurious)
                {
                    CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Spurious callbacks detected, stopping.", (CsrUint32) (CsrUintptr) function->device);
                    terminateTest(instanceData, testData);
                }
                else if (testData->stop)
                {
                    CsrResult result;

                    terminateTest(instanceData, testData);

                    result = interruptDisable(function);
                    if (result == CSR_RESULT_SUCCESS)
                    {
                        result = functionDisable(function);
                        if (result == CSR_RESULT_SUCCESS)
                        {
                            reportInt(testData);
                        }
                        else
                        {
                            CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Failed at function disable.", (CsrUint32) (CsrUintptr) function->device);
                        }
                    }
                    else
                    {
                        CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Failed interrupt disable.", (CsrUint32) (CsrUintptr) function->device);
                    }
                }
                else
                {
                    CsrResult result;
                    testData->timeout = CsrSchedTimerSet(1 * CSR_SCHED_SECOND, timeoutCallback, 0, testData);
                    testData->testState = 2;
                    result = triggerInterrupt(function, testData);
                    if (result != CSR_RESULT_SUCCESS)
                    {
                        terminateTest(instanceData, testData);
                        CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Trigger interrupt failed (%u).", (CsrUint32) (CsrUintptr) function->device, result);
                    }
                }
                break;
            }
        /* Fallthrough */
        default:
            /* Timeout */
            CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Timeout.", (CsrUint32) (CsrUintptr) function->device);
            testData->timeout = CSR_SCHED_TID_INVALID;
            terminateTest(instanceData, testData);
            break;
    }
}

void CsrSdioProbeStartStopTest(CsrSdioProbeInstanceData *instanceData, CsrUint16 key, CsrSdioProbeTestType testType)
{
    CsrSdioFunction *function = instanceData->functions[key];
    CsrSdioProbeTestData *testData = &instanceData->testData[key];
    CsrResult result;

    if (testData->testType == CSR_SDIO_PROBE_TEST_TYPE_NONE)
    {
        function->driverData = testData;
        testData->key = key;
        testData->instanceData = instanceData;
        testData->testType = testType;
        testData->testState = 0;
        testData->timeout = CsrSchedTimerSet(1 * CSR_SCHED_SECOND, timeoutCallback, 0, testData);
        if (testType == CSR_SDIO_PROBE_TEST_TYPE_ASYNC)
        {
            testData->bgint = CsrSchedBgintReg(bgintCallbackAsync, testData, NULL);
        }
        else if (testType == CSR_SDIO_PROBE_TEST_TYPE_SYNC)
        {
            testData->bgint = CsrSchedBgintReg(bgintCallbackSync, testData, NULL);
        }
        else if (testType == CSR_SDIO_PROBE_TEST_TYPE_INT)
        {
            testData->bgint = CsrSchedBgintReg(bgintCallbackInt, testData, NULL);
        }
        testData->spurious = FALSE;
        testData->stop = FALSE;
        testData->data = CsrPmemAlloc(testData->dataLength);
        testData->byteOperations = 0;
        testData->byteOperationsTime = 0;
        testData->dataOperations = 0;
        testData->dataOperationsTime = 0;
        instanceData->testsRunning += 1;

        result = functionEnable(function);
        if (result == CSR_RESULT_SUCCESS)
        {
            result = blockSizeSet(function);
            if (result == CSR_RESULT_SUCCESS)
            {
                result = interruptEnable(function);
                if (result == CSR_RESULT_SUCCESS)
                {
                    result = busFrequencySet(function);
                    if (result == CSR_RESULT_SUCCESS)
                    {
                        CsrSdioProbeTestMenuUpdate(instanceData, key);
                    }
                    else
                    {
                        CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Failed bus clock frequency set.", (CsrUint32) (CsrUintptr) function->device);
                    }
                }
                else
                {
                    CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Failed interrupt enable.", (CsrUint32) (CsrUintptr) function->device);
                }
            }
            else
            {
                CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Failed at block size set.", (CsrUint32) (CsrUintptr) function->device);
            }
        }
        else
        {
            CsrSdioProbeEventNotification(instanceData, TRUE, TRUE, "[%08X] Failed at function enable.", (CsrUint32) (CsrUintptr) function->device);
        }
        if (result != CSR_RESULT_SUCCESS)
        {
            terminateTest(instanceData, testData);
        }
    }
    else if (testData->testType == testType)
    {
        testData->stop = TRUE;
    }
    else
    {
        CsrSdioProbeEventNotification(instanceData, FALSE, TRUE, "Unable to start test while one is already running on the same device.");
    }
}
