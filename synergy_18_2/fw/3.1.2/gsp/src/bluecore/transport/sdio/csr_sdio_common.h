#ifndef CSR_SDIO_COMMON_H__
#define CSR_SDIO_COMMON_H__
/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_sdio.h"
#include "csr_framework_ext.h"
#include "csr_log_text_2.h"

#ifdef CSR_SDIO_USE_SDIO
#include "csr_sdio_master.h"
#endif
#ifdef CSR_SDIO_USE_CSPI
#include "csr_spi_master.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Log Text Handle */
CSR_LOG_TEXT_HANDLE_DECLARE(CsrSdioLto);

/* Sub Origins */
#define CSR_SDIO_LTSO_SDIO 1
#define CSR_SDIO_LTSO_CSPI 2

typedef CsrUint8 CsrSdioOperationType;
#define CSR_SDIO_OPERATION_TYPE_NONE    ((CsrSdioOperationType) 0)
#define CSR_SDIO_OPERATION_TYPE_READ8   ((CsrSdioOperationType) 1)
#define CSR_SDIO_OPERATION_TYPE_WRITE8  ((CsrSdioOperationType) 2)
#define CSR_SDIO_OPERATION_TYPE_READ    ((CsrSdioOperationType) 3)
#define CSR_SDIO_OPERATION_TYPE_WRITE   ((CsrSdioOperationType) 4)

typedef struct
{
    CsrSdioOperationType operationType;
    CsrUint32            address;
    CsrUint8             data8;
    CsrUint32            length;
    CsrUint8            *dataPtr;
    CsrSdioAsyncCallback callback;
} CsrSdioQueueElement;

typedef CsrUint8 CsrSdioFunctionState;
#define CSR_SDIO_FUNCTION_STATE_DETACHED   ((CsrSdioFunctionState) 0)
#define CSR_SDIO_FUNCTION_STATE_ATTACHING  ((CsrSdioFunctionState) 1)
#define CSR_SDIO_FUNCTION_STATE_DETACHING  ((CsrSdioFunctionState) 2)
#define CSR_SDIO_FUNCTION_STATE_ATTACHED   ((CsrSdioFunctionState) 3)

/* Structure representing a Device Function */
typedef struct CsrSdioFunctionPrivate
{
    CsrSdioFunctionState   state;
    CsrSdioFunctionDriver *functionDriver;
    CsrSdioFunction        pub;

    CsrSdioOperationType        operationType;
    CsrSdioAsyncCallback        operationCallback;
    CsrSdioInterruptDsrCallback interruptDsrCallback;

    /* Queue (for nonblocking operations) */
    CsrSdioQueueElement queueElement;

    /* Callback context indicator */
    CsrBool callbackContext;

    /* Event (for blocking operations) */
    CsrBool        blocked;
    CsrEventHandle blockEvent;

    /* Wait queue chaining */
    struct CsrSdioFunctionPrivate *next;
    struct CsrSdioFunctionPrivate *prev;

    CsrUint32 maxFrequency;
} CsrSdioFunctionPrivate;

typedef CsrUint8 CsrSdioDeviceType;
#define CSR_SDIO_DEVICE_TYPE_SDIO ((CsrSdioDeviceType) 0)
#define CSR_SDIO_DEVICE_TYPE_CSPI ((CsrSdioDeviceType) 1)

typedef struct
{
    /* Device type */
    CsrSdioDeviceType type;

    /* Pointer to master device */
    void *masterDevice;

    /* Protocol specific context */
    void *context;

    /* Local handle to currently active function */
    CsrSdioFunctionPrivate *operationHandle;

    /* Interrupt enable/disable for each function (bitmask) */
    CsrUint8 localInterruptEnable;

    /* Current interrupt mask (automatically set when an interrupt occurs) */
    CsrUint8 localInterruptMask;

    /* Capability information retrieved from device */
    CsrUint16 maxBlockSize[8];
    CsrUint32 maxFrequency;

    /* Array of CsrSdioFunctionPrivate */
    CsrUint8                sdioFunctionsCount; /* Count not including Function 0 */
    CsrSdioFunctionPrivate *sdioFunctions;      /* Function 1 is at index 0 */

    /* The bus this device is attached to */
    struct CsrSdioBus *bus;
} CsrSdioDevice;

#define ASSOC_DATA_QUEUE_LENGTH 8

typedef struct CsrSdioBus
{
    /* Whether the bus is claimed for some operation */
    CsrBool claimed;

    /* Count of devices attached to this bus */
    CsrUint8        devicesCount;
    CsrSdioDevice **devices;

    /* Wait queue (for both blocked and queued operations) */
    CsrSdioFunctionPrivate *waitQueueIn;
    CsrSdioFunctionPrivate *waitQueueOut;

    /* Associated data queue */
    CsrUint8                assocIndex;
    CsrUint8                assocOutdex;
    CsrSdioFunctionPrivate *assocFunction[ASSOC_DATA_QUEUE_LENGTH];
    CsrSdioAsyncDsrCallback assocCallback[ASSOC_DATA_QUEUE_LENGTH];
    CsrResult               assocResult[ASSOC_DATA_QUEUE_LENGTH];
} CsrSdioBus;

#ifdef __cplusplus
}
#endif

#endif
