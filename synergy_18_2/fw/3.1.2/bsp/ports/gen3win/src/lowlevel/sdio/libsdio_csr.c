/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include "csr_types.h"
#include "csr_framework_ext.h"
#include "csr_formatted_io.h"
#include "csr_util.h"
#include "csr_log_text.h"

#include "list.h"

#include "libsdio.h"
#include "sdio.h"

#include "csr_sdio.h"

#define IO_EN_TIMEOUT_MS 5000

#define SDIO_MAX_CARDS 10

struct queue_elem
{
    CsrSdioAsyncCallback callback;
    struct glue_func    *func;
};

struct glue_driver
{
    struct os_list_node    node;
    CsrSdioFunctionDriver *functionDriver;
};

#define to_glue_driver(n) (struct glue_driver *) (               \
        (CsrUint8 *) (n) - offsetof(struct glue_driver, node))

struct glue_func
{
    CsrSdioFunction     function; /* must be first */
    struct os_list_node node;
    CsrSdioCard        *card;
    sdio_uif_t          sdev;
    struct glue_driver *glue_driver;
    CsrEventHandle      acknowledge_event;
    CsrResult           acknowledge_result;
};

#define to_glue_func(n) (struct glue_func *) ((CsrUint8 *) (n) - offsetof(struct glue_func, node))

struct CsrSdioCardImpl
{
    struct os_list_node node;
    struct glue_func   *func[SDIO_MAX_FUNCTIONS];
    CsrMutexHandle      mutex;
    CsrUint8            pendingIntAcks;
    CsrUint8            intrEnabled;
    sdio_uif_t          sdev;
};

#define to_glue_card(n) (CsrSdioCard *) ((CsrUint8 *) (n) - offsetof(CsrSdioCard, node))

#ifdef CSR_LOG_ENABLE
#define CSR_SDIO_LTO ((0x0100 << 16) | CSR_SCHED_QID_INVALID)
#define CSR_SDIO_LTSO_SDIO 1

static const CsrLogSubOrigin subOrigins[] =
{
    {CSR_SDIO_LTSO_SDIO, "SDIO"},
};
#endif

static CsrUint32 registerCount;
static CsrMutexHandle glue_mutex;
static struct os_list glue_drivers;
static struct os_list glue_funcs;
static struct os_list glue_cards;

static CsrSdioCard *card_add(const char *device);
static void card_del(CsrSdioCard *card);
static void module_init(void);
static void module_cleanup(void);
static void cleanup_cards(void);
static void discover_cards(void);

static CsrResult libsdio_status_to_csrresult(int status)
{
    switch (status)
    {
        case SDIO_SUCCESS:
            return CSR_RESULT_SUCCESS;
        case SDIO_EINVAL:
            return CSR_SDIO_RESULT_INVALID_VALUE;
        case SDIO_ENODEV:
            return CSR_SDIO_RESULT_NO_DEVICE;
        case SDIO_EIO:
            return CSR_SDIO_RESULT_CRC_ERROR;
        case SDIO_ETIMEDOUT:
            return CSR_SDIO_RESULT_TIMEOUT;
        default:
            return CSR_RESULT_FAILURE;
    }
}

static struct queue_elem *create_queue_elem(struct glue_func *func, CsrSdioAsyncCallback callback)
{
    struct queue_elem *elem = CsrMemAlloc(sizeof(*elem));
    elem->func = func;
    elem->callback = callback;

    return elem;
}

/*
 * Does an SDIO function driver work with this SDIO function?
 */
CsrBool CsrSdioFunctionDriverMatch(CsrSdioFunctionDriver *functionDriver, CsrSdioFunction *function)
{
    int i;
    CsrSdioFunctionId *a, *b;

    b = &function->sdioId;

    for (i = 0; i < functionDriver->idsCount; i++)
    {
        a = &functionDriver->ids[i];

        if (((a->manfId == CSR_SDIO_ANY_MANF_ID) || (a->manfId == b->manfId)) &&
            ((a->cardId == CSR_SDIO_ANY_CARD_ID) || (a->cardId == b->cardId)) &&
            ((a->sdioFunction == CSR_SDIO_ANY_SDIO_FUNCTION) || (a->sdioFunction == b->sdioFunction)) &&
            ((a->sdioInterface == CSR_SDIO_ANY_SDIO_INTERFACE) || (a->sdioInterface == b->sdioInterface)))
        {
            return TRUE;
        }
    }
    return FALSE;
}

static CsrResult wait_for_acknowledge(struct glue_func *glue_func)
{
    CsrUint32 eventBits;

    CsrEventWait(&glue_func->acknowledge_event, CSR_EVENT_WAIT_INFINITE, &eventBits);
    return glue_func->acknowledge_result;
}

static CsrResult try_insert(struct glue_driver *glue_driver, struct glue_func *glue_func)
{
    CsrResult result = CSR_SDIO_RESULT_NO_DEVICE;

    if (glue_func->glue_driver != NULL)
    {
        /* Already used */
        return CSR_RESULT_SUCCESS;
    }

    if (CsrSdioFunctionDriverMatch(glue_driver->functionDriver, &glue_func->function))
    {
        glue_func->glue_driver = glue_driver;
        CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u insert", glue_func->function.sdioId.sdioFunction));
        glue_driver->functionDriver->inserted(&glue_func->function);
        result = wait_for_acknowledge(glue_func);
        if (result)
        {
            glue_func->glue_driver = NULL;
        }
    }
    return result;
}

static void try_remove(struct glue_driver *glue_driver, struct glue_func *glue_func)
{
    if (glue_driver && (glue_func->glue_driver == glue_driver))
    {
        CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u remove", glue_func->function.sdioId.sdioFunction));
        glue_driver->functionDriver->removed(&glue_func->function);
        wait_for_acknowledge(glue_func);
        glue_func->glue_driver = NULL;
    }
}

static void add_func(struct glue_func *glue_func)
{
    struct os_list_node *node;
    CsrResult result;

    CsrMutexLock(&glue_mutex);

    os_list_add_tail(&glue_funcs, &glue_func->node);

    for (node = os_list_head(&glue_drivers); node != os_list_end(&glue_drivers);
         node = node->next)
    {
        struct glue_driver *glue_driver = to_glue_driver(node);

        result = try_insert(glue_driver, glue_func);
        if (result == CSR_RESULT_SUCCESS)
        {
            break;
        }
    }

    CsrMutexUnlock(&glue_mutex);
}

static DWORD WINAPI add_driver_thread(LPVOID arg)
{
    struct glue_driver *glue_driver = arg;

    struct os_list_node *node;

    discover_cards();

    CsrMutexLock(&glue_mutex);

    for (node = os_list_head(&glue_funcs); node != os_list_end(&glue_funcs); node = node->next)
    {
        struct glue_func *glue_func = to_glue_func(node);
        try_insert(glue_driver, glue_func);
    }

    CsrMutexUnlock(&glue_mutex);

    cleanup_cards();

    return 0;
}

static CsrResult add_driver(struct glue_driver *glue_driver)
{
    HANDLE async_thread;

    CsrMutexLock(&glue_mutex);
    os_list_add_tail(&glue_drivers, &glue_driver->node);
    CsrMutexUnlock(&glue_mutex);

    async_thread = CreateThread(NULL, 0, add_driver_thread, glue_driver, 0, NULL);

    return async_thread == INVALID_HANDLE_VALUE ? CSR_RESULT_FAILURE : CSR_RESULT_SUCCESS;
}

static DWORD WINAPI del_driver_thread(LPVOID arg)
{
    struct glue_driver *glue_driver = arg;
    struct os_list_node *node;

    CsrMutexLock(&glue_mutex);
    for (node = os_list_head(&glue_funcs); node != os_list_end(&glue_funcs); node = node->next)
    {
        struct glue_func *glue_func = to_glue_func(node);
        try_remove(glue_driver, glue_func);
    }
    CsrMutexUnlock(&glue_mutex);

    cleanup_cards();
    module_cleanup();

    CsrMemFree(glue_driver);

    return 0;
}

static CsrResult del_driver(struct glue_driver *glue_driver)
{
    HANDLE async_thread;

    CsrMutexLock(&glue_mutex);
    os_list_del(&glue_driver->node);
    CsrMutexUnlock(&glue_mutex);

    async_thread = CreateThread(NULL, 0, del_driver_thread, glue_driver, 0, NULL);

    return async_thread == INVALID_HANDLE_VALUE ? CSR_RESULT_FAILURE : CSR_RESULT_SUCCESS;
}

static void discover_cards(void)
{
    CsrUint8 deviceIndex;
    CsrCharString devName[] = "sdio_uifXX";

    for (deviceIndex = 0; deviceIndex < SDIO_MAX_CARDS; ++deviceIndex)
    {
        CsrSdioCard *card;

        CsrSnprintf(devName, sizeof(devName), "sdio_uif%u", deviceIndex);

        CsrMutexLock(&glue_mutex);

        if ((card = card_add(devName)) != NULL)
        {
            os_list_add_tail(&glue_cards, &card->node);
        }

        CsrMutexUnlock(&glue_mutex);
    }
}

static void cleanup_cards(void)
{
    struct os_list_node *node;
    struct os_list_node *next;

    CsrMutexLock(&glue_mutex);

    for (node = os_list_head(&glue_cards), next = node->next;
         node != os_list_end(&glue_cards);
         node = next, next = node->next)
    {
        CsrSdioCard *card = to_glue_card(node);
        int f;

        for (f = 1; f <= sdio_num_functions(card->sdev); f++)
        {
            struct glue_func *glue_func = card->func[f];

            if (glue_func->glue_driver)
            {
                /* One function driver is still active so keep card alive */
                card = NULL;
                break;
            }
        }

        card_del(card);
    }

    CsrMutexUnlock(&glue_mutex);
}

static void LIBSDIOAPI io_callback(sdio_uif_t uif, void *arg, int status)
{
    struct queue_elem *elem = arg;
    struct glue_func *glue_func = elem->func;
    CsrSdioAsyncCallback callback = elem->callback;
    CsrSdioFunction *function = &glue_func->function;
    CsrResult result;
    CsrSdioAsyncDsrCallback dsrCallback;

    result = libsdio_status_to_csrresult(status);
    if ((dsrCallback = callback(function, result)) != NULL)
    {
        dsrCallback(function, result);
    }

    CsrMemFree(elem);
}

CsrResult CsrSdioFunctionDriverRegister(CsrSdioFunctionDriver *functionDriver)
{
    struct glue_driver *glue_driver;

    glue_driver = CsrMemAlloc(sizeof(*glue_driver));
    if (glue_driver == NULL)
    {
        return CSR_RESULT_FAILURE;
    }
    functionDriver->priv = glue_driver;

    glue_driver->functionDriver = functionDriver;

    module_init();

    CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Registering Function Driver %08X", (CsrUint32) (CsrUintptr) functionDriver));

    add_driver(glue_driver);

    return CSR_RESULT_SUCCESS;
}

void CsrSdioFunctionDriverUnregister(CsrSdioFunctionDriver *functionDriver)
{
    struct glue_driver *glue_driver = functionDriver->priv;

    CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Unregistering Function Driver %08X", (CsrUint32) (CsrUintptr) functionDriver));

    del_driver(glue_driver);
}

void CsrSdioInsertedAcknowledge(CsrSdioFunction *function, CsrResult result)
{
    struct glue_func *glue_func = function->priv;

    glue_func->acknowledge_result = result;
    CsrEventSet(&glue_func->acknowledge_event, 1);

    CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u insert acknowledged", function->sdioId.sdioFunction));
}

void CsrSdioRemovedAcknowledge(CsrSdioFunction *function)
{
    struct glue_func *glue_func = function->priv;

    CsrEventSet(&glue_func->acknowledge_event, 1);

    CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u removed acknowledged", function->sdioId.sdioFunction));
}

CsrResult CsrSdioRead8(CsrSdioFunction *function, CsrUint32 address, CsrUint8 *data)
{
    struct glue_func *glue_func = function->priv;
    int status;

    status = sdio_read8(glue_func->sdev, function->sdioId.sdioFunction, address, data);
    CSR_LOG_TEXT_DEBUG((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u read8 address 0x%X data: 0x%X", function->sdioId.sdioFunction, address, *data));
    return libsdio_status_to_csrresult(status);
}

void CsrSdioRead8Async(CsrSdioFunction *function, CsrUint32 address, CsrUint8 *data, CsrSdioAsyncCallback callback)
{
    struct glue_func *glue_func = function->priv;

    sdio_read8_async(glue_func->sdev, function->sdioId.sdioFunction, address, data,
        io_callback, create_queue_elem(glue_func, callback));
}

CsrResult CsrSdioF0Read8(CsrSdioFunction *function, CsrUint32 address, CsrUint8 *data)
{
    struct glue_func *glue_func = function->priv;
    int status;

    status = sdio_read8(glue_func->sdev, 0, address, data);
    CSR_LOG_TEXT_DEBUG((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u read8 address 0x%X data: 0x%X", 0, address, *data));
    return libsdio_status_to_csrresult(status);
}

void CsrSdioF0Read8Async(CsrSdioFunction *function, CsrUint32 address, CsrUint8 *data, CsrSdioAsyncCallback callback)
{
    struct glue_func *glue_func = function->priv;

    sdio_read8_async(glue_func->sdev, 0, address, data,
        io_callback, create_queue_elem(glue_func, callback));
}

CsrResult CsrSdioWrite8(CsrSdioFunction *function, CsrUint32 address, CsrUint8 data)
{
    struct glue_func *glue_func = function->priv;
    int status;

    status = sdio_write8(glue_func->sdev, function->sdioId.sdioFunction, address, data);
    CSR_LOG_TEXT_DEBUG((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u write8 address 0x%X data: 0x%X", function->sdioId.sdioFunction, address, data));
    return libsdio_status_to_csrresult(status);
}

void CsrSdioWrite8Async(CsrSdioFunction *function, CsrUint32 address, CsrUint8 data, CsrSdioAsyncCallback callback)
{
    struct glue_func *glue_func = function->priv;

    sdio_write8_async(glue_func->sdev, function->sdioId.sdioFunction, address, data,
        io_callback, create_queue_elem(glue_func, callback));
}

CsrResult CsrSdioF0Write8(CsrSdioFunction *function, CsrUint32 address, CsrUint8 data)
{
    struct glue_func *glue_func = (struct glue_func *) function->priv;
    int status;

    status = sdio_write8(glue_func->sdev, 0, address, data);
    CSR_LOG_TEXT_DEBUG((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u write8 address 0x%X data: 0x%X", 0, address, data));
    return libsdio_status_to_csrresult(status);
}

void CsrSdioF0Write8Async(CsrSdioFunction *function, CsrUint32 address, CsrUint8 data, CsrSdioAsyncCallback callback)
{
    struct glue_func *glue_func = function->priv;

    sdio_write8_async(glue_func->sdev, 0, address, data,
        io_callback, create_queue_elem(glue_func, callback));
}

CsrResult CsrSdioRead16(CsrSdioFunction *function, CsrUint32 address, CsrUint16 *data)
{
    CsrResult csrResult;
    CsrUint8 b0, b1;

    csrResult = CsrSdioRead8(function, address, &b0);
    if (csrResult != CSR_RESULT_SUCCESS)
    {
        return csrResult;
    }

    csrResult = CsrSdioRead8(function, address + 1, &b1);
    if (csrResult != CSR_RESULT_SUCCESS)
    {
        return csrResult;
    }

    *data = ((CsrUint16) b1 << 8) | b0;

    return CSR_RESULT_SUCCESS;
}

void CsrSdioRead16Async(CsrSdioFunction *function, CsrUint32 address, CsrUint16 *data, CsrSdioAsyncCallback callback)
{
    /* Not implemented */
}

CsrResult CsrSdioWrite16(CsrSdioFunction *function, CsrUint32 address, CsrUint16 data)
{
    CsrResult csrResult;
    CsrUint8 b0, b1;

    b1 = (CsrUint8) ((data >> 8) & 0xFF);
    csrResult = CsrSdioWrite8(function, address + 1, b1);
    if (csrResult != CSR_RESULT_SUCCESS)
    {
        return csrResult;
    }

    b0 = (CsrUint8) (data & 0xFF);
    csrResult = CsrSdioWrite8(function, address, b0);
    if (csrResult != CSR_RESULT_SUCCESS)
    {
        return csrResult;
    }
    return CSR_RESULT_SUCCESS;
}

void CsrSdioWrite16Async(CsrSdioFunction *function, CsrUint32 address, CsrUint16 data, CsrSdioAsyncCallback callback)
{
    /* Not implemented */
}

CsrResult CsrSdioRead(CsrSdioFunction *function, CsrUint32 address, void *data, CsrUint32 length)
{
    CsrInt32 rc;
    struct glue_func *glue_func = (struct glue_func *) function->priv;

    rc = sdio_read(glue_func->sdev, function->sdioId.sdioFunction, address, data, length,
        function->blockSize);

    return libsdio_status_to_csrresult(rc);
}

void CsrSdioReadAsync(CsrSdioFunction *function, CsrUint32 address, void *data, CsrUint32 length, CsrSdioAsyncCallback callback)
{
    struct glue_func *glue_func = function->priv;

    sdio_read_async(glue_func->sdev, function->sdioId.sdioFunction, address, data, length, function->blockSize,
        io_callback, create_queue_elem(glue_func, callback));
}

CsrResult CsrSdioWrite(CsrSdioFunction *function, CsrUint32 address,
    const void *data, CsrUint32 length)
{
    CsrInt32 rc;
    struct glue_func *glue_func = (struct glue_func *) function->priv;

    rc = sdio_write(glue_func->sdev, function->sdioId.sdioFunction, address, data, length,
        function->blockSize);

    return libsdio_status_to_csrresult(rc);
}

void CsrSdioWriteAsync(CsrSdioFunction *function, CsrUint32 address, const void *data, CsrUint32 length, CsrSdioAsyncCallback callback)
{
    struct glue_func *glue_func = function->priv;

    sdio_write_async(glue_func->sdev, function->sdioId.sdioFunction, address, data, length, function->blockSize,
        io_callback, create_queue_elem(glue_func, callback));
}

static CsrResult wait_for_io_ready(CsrSdioFunction *function, CsrUint8 val)
{
    CsrUint8 bit = 1 << function->sdioId.sdioFunction;
    CsrUint16 timeout = IO_EN_TIMEOUT_MS;
    CsrUint8 io_ready;
    CsrResult result;

    while (timeout > 0)
    {
        result = CsrSdioF0Read8(function, SDIO_CCCR_IO_READY, &io_ready);
        if (result != CSR_RESULT_SUCCESS)
        {
            return result;
        }
        if ((io_ready & bit) == val)
        {
            return CSR_RESULT_SUCCESS;
        }

        CsrThreadSleep(10);
        timeout -= 10;
    }
    return CSR_RESULT_FAILURE;
}

CsrResult CsrSdioFunctionEnable(CsrSdioFunction *function)
{
    struct glue_func *glue_func = (struct glue_func *) function->priv;
    CsrUint8 bit = 1 << function->sdioId.sdioFunction;
    CsrUint8 io_en;
    CsrResult result;

    CsrMutexLock(&glue_func->card->mutex);

    result = CsrSdioF0Read8(function, SDIO_CCCR_IO_EN, &io_en);
    if (result != CSR_RESULT_SUCCESS)
    {
        goto out;
    }
    io_en |= bit;
    result = CsrSdioF0Write8(function, SDIO_CCCR_IO_EN, io_en);
    if (result != CSR_RESULT_SUCCESS)
    {
        goto out;
    }
    result = wait_for_io_ready(function, bit);
out:
    CsrMutexUnlock(&glue_func->card->mutex);

    CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u enable (0x%X)", function->sdioId.sdioFunction, result));
    return result;
}

CsrResult CsrSdioFunctionDisable(CsrSdioFunction *function)
{
    struct glue_func *glue_func = (struct glue_func *) function->priv;
    CsrUint8 bit = 1 << function->sdioId.sdioFunction;
    CsrUint8 io_en;
    CsrResult result;

    CsrMutexLock(&glue_func->card->mutex);

    result = CsrSdioF0Read8(function, SDIO_CCCR_IO_EN, &io_en);
    if (result != CSR_RESULT_SUCCESS)
    {
        goto out;
    }
    io_en &= ~bit;
    result = CsrSdioF0Write8(function, SDIO_CCCR_IO_EN, io_en);
    if (result != CSR_RESULT_SUCCESS)
    {
        goto out;
    }
    result = wait_for_io_ready(function, 0);
out:
    CsrMutexUnlock(&glue_func->card->mutex);

    CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u disable (0x%X)", function->sdioId.sdioFunction, result));
    return result;
}

static void try_int_handler_invoke(struct glue_func *glue_func)
{
    int bit = glue_func->function.sdioId.sdioFunction;

    CsrMutexLock(&glue_func->card->mutex);

    if (glue_func->card->pendingIntAcks & bit)
    {
        CsrSdioInterruptDsrCallback dsr;
        CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u delayed intr callback", glue_func->function.sdioId.sdioFunction));
        dsr = glue_func->glue_driver->functionDriver->intr(&glue_func->function);
        if (dsr)
        {
            CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u delayed intr dsr callback", glue_func->function.sdioId.sdioFunction));
            dsr(&glue_func->function);
        }
    }

    CsrMutexUnlock(&glue_func->card->mutex);
}

static CsrResult wait_for_int_en(CsrSdioFunction *function, CsrUint8 val)
{
    CsrUint8 bit = 1 << function->sdioId.sdioFunction;
    CsrUint16 timeout = IO_EN_TIMEOUT_MS;
    CsrUint8 int_en;
    CsrResult result;

    while (timeout > 0)
    {
        result = CsrSdioF0Read8(function, SDIO_CCCR_INT_EN, &int_en);
        if (result != CSR_RESULT_SUCCESS)
        {
            return result;
        }
        if ((int_en & bit) == val)
        {
            return CSR_RESULT_SUCCESS;
        }

        CsrThreadSleep(10);
        timeout -= 10;
    }
    return CSR_RESULT_FAILURE;
}

CsrResult CsrSdioInterruptEnable(CsrSdioFunction *function)
{
    struct glue_func *glue_func = (struct glue_func *) function->priv;
    CsrUint32 bit = 1 << function->sdioId.sdioFunction;
    CsrUint8 int_en;
    CsrResult result;

    CsrMutexLock(&glue_func->card->mutex);

    result = CsrSdioF0Read8(function, SDIO_CCCR_INT_EN, &int_en);
    if (result == CSR_RESULT_SUCCESS)
    {
        int_en |= bit;
        result = CsrSdioF0Write8(function, SDIO_CCCR_INT_EN, int_en | SDIO_CCCR_INT_EN_MIE);

        if (result == CSR_RESULT_SUCCESS)
        {
            if ((result = wait_for_int_en(function, bit)) == CSR_RESULT_SUCCESS)
            {
                glue_func->card->intrEnabled |= bit;
                try_int_handler_invoke(glue_func);
            }
        }
    }

    CsrMutexUnlock(&glue_func->card->mutex);

    CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u intr enable (result 0x%X, enabled: 0x%X, pending int: 0x%X)", function->sdioId.sdioFunction, result, glue_func->card->intrEnabled, glue_func->card->pendingIntAcks));
    return result;
}

CsrResult CsrSdioInterruptDisable(CsrSdioFunction *function)
{
    struct glue_func *glue_func = (struct glue_func *) function->priv;
    CsrUint32 bit = 1 << function->sdioId.sdioFunction;
    CsrUint8 int_en;
    CsrResult result;

    CsrMutexLock(&glue_func->card->mutex);

    result = CsrSdioF0Read8(function, SDIO_CCCR_INT_EN, &int_en);
    if (result == CSR_RESULT_SUCCESS)
    {
        int_en &= ~(bit | SDIO_CCCR_INT_EN_MIE);
        result = CsrSdioF0Write8(function, SDIO_CCCR_INT_EN, int_en != 0 ? int_en | SDIO_CCCR_INT_EN_MIE : 0);

        if (result == CSR_RESULT_SUCCESS)
        {
            if ((result = wait_for_int_en(function, 0)))
            {
                glue_func->card->intrEnabled &= ~bit;
            }
        }
    }

    CsrMutexUnlock(&glue_func->card->mutex);

    CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u intr disable (result 0x%X, enabled: 0x%X)", function->sdioId.sdioFunction, result, glue_func->card->intrEnabled));
    return result;
}

void CsrSdioInterruptAcknowledge(CsrSdioFunction *function)
{
    struct glue_func *glue_func = (struct glue_func *) function->priv;

    CsrMutexLock(&glue_func->card->mutex);

    glue_func->card->pendingIntAcks &= ~(1 << function->sdioId.sdioFunction);

    CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u intr ack (pending int 0x%X)", function->sdioId.sdioFunction, glue_func->card->pendingIntAcks));

    if (glue_func->card->pendingIntAcks == 0)
    {
        CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "SDIO card %08X intr unmasked", (CsrUint32) (CsrUintptr) glue_func->card));
        sdio_interrupt_unmask(glue_func->sdev);
    }

    CsrMutexUnlock(&glue_func->card->mutex);
}

void CsrSdioSuspendAcknowledge(CsrSdioFunction *function, CsrResult result)
{
    /* No-op as suspend/resume is not supported. */
}

void CsrSdioResumeAcknowledge(CsrSdioFunction *function, CsrResult result)
{
    /* No-op as suspend/resume is not supported. */
}

CsrResult CsrSdioMaxBusClockFrequencySet(CsrSdioFunction *function, CsrUint32 maxFrequency)
{
    struct glue_func *glue_func = (struct glue_func *) function->priv;

    sdio_set_max_bus_freq(glue_func->sdev, (int) maxFrequency);

    CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u frequency %u", function->sdioId.sdioFunction, maxFrequency));

    return CSR_RESULT_SUCCESS;
}

CsrResult CsrSdioPowerOn(CsrSdioFunction *function)
{
    return CSR_SDIO_RESULT_NOT_RESET;
}

void CsrSdioPowerOff(CsrSdioFunction *function)
{
}

void CsrSdioFunctionActive(CsrSdioFunction *function)
{
}

void CsrSdioFunctionIdle(CsrSdioFunction *function)
{
}

CsrResult CsrSdioHardReset(CsrSdioFunction *function)
{
    struct glue_func *glue_func = function->priv;
    int status;

    status = sdio_hard_reset(glue_func->sdev);
    return libsdio_status_to_csrresult(status);
}

CsrResult CsrSdioBlockSizeSet(CsrSdioFunction *function, CsrUint16 blockSize)
{
    struct glue_func *glue_func = function->priv;
    int status;

    blockSize = CSRMIN(blockSize, sdio_max_block_size(glue_func->sdev, (int) function->sdioId.sdioFunction));

    status = sdio_set_block_size(glue_func->sdev, (int) function->sdioId.sdioFunction, blockSize);
    if (status == SDIO_SUCCESS)
    {
        function->blockSize = sdio_block_size(glue_func->sdev, (int) function->sdioId.sdioFunction);
        CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u blocksize %u", function->sdioId.sdioFunction, function->blockSize));
    }

    return libsdio_status_to_csrresult(status);
}

static void LIBSDIOAPI glue_int_handler(sdio_uif_t sdev, void *arg)
{
    CsrSdioCard *card = arg;
    CsrSdioInterruptDsrCallback dsr;
    CsrUint8 int_pending;
    int f;
    int status;

    status = sdio_read8(sdev, 0, SDIO_CCCR_INT_PENDING, &int_pending);
    if (status != SDIO_SUCCESS)
    {
        return;
    }

    CsrMutexLock(&card->mutex);

    for (f = 1; f < SDIO_MAX_FUNCTIONS; f++)
    {
        struct glue_func *glue_func = card->func[f];
        int bit = 1 << f;

        if (int_pending & bit && card->func[f])
        {
            card->pendingIntAcks |= bit;

            if (card->intrEnabled & bit)
            {
                CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u intr callback", f));
                dsr = glue_func->glue_driver->functionDriver->intr(&glue_func->function);
                if (dsr)
                {
                    CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u intr dsr callback", f));
                    dsr(&glue_func->function);
                }
            }
            else
            {
                CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "Function %u intr pending", f));
            }
        }
    }

    if (card->pendingIntAcks)
    {
        CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "SDIO card %08X intr masked (pending intr: 0x%0X)", (CsrUint32) (CsrUintptr) card, card->pendingIntAcks));
        sdio_interrupt_mask(sdev);
    }

    CsrMutexUnlock(&card->mutex);
}

static CsrSdioCard *card_add(const char *device)
{
    CsrSdioCard *card;
    struct glue_func *glue_func;
    int f = 0;

    card = CsrMemAlloc(sizeof(*card));
    if (card == NULL)
    {
        return NULL;
    }

    CsrMemSet(card, 0, sizeof(*card));

    if (CsrMutexCreate(&card->mutex) != CSR_RESULT_SUCCESS)
    {
        CsrMemFree(card);
        return NULL;
    }

    CsrMutexLock(&card->mutex);

    card->sdev = sdio_open(device, glue_int_handler, card);
    if (!card->sdev)
    {
        goto err;
    }

    CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "SDIO card opened %s %08X", device, (CsrUint32) (CsrUintptr) card));

    for (f = 1; f <= sdio_num_functions(card->sdev); f++)
    {
        glue_func = card->func[f] = CsrMemAlloc(sizeof(*glue_func));
        if (glue_func == NULL)
        {
            goto err;
        }

        glue_func->function.sdioId.manfId = sdio_manf_id(card->sdev);
        glue_func->function.sdioId.cardId = sdio_card_id(card->sdev);
        glue_func->function.sdioId.sdioFunction = f;
        glue_func->function.sdioId.sdioInterface = sdio_std_if(card->sdev, f);
        glue_func->function.blockSize = sdio_block_size(card->sdev, f);
        glue_func->function.features = CSR_SDIO_FEATURE_BYTE_MODE;
        glue_func->function.device = card;
        glue_func->function.priv = glue_func;
        glue_func->card = card;
        glue_func->sdev = card->sdev;
        glue_func->glue_driver = NULL;

        if (CsrEventCreate(&glue_func->acknowledge_event) != CSR_RESULT_SUCCESS)
        {
            CsrMemFree(glue_func);
            goto err;
        }

        add_func(glue_func);
    }

    CsrMutexUnlock(&card->mutex);
    return card;

err:
    if (card)
    {
        for ( ; f > 0; f--)
        {
            CsrMemFree(card->func[f]);
        }
        if (card->sdev)
        {
            sdio_close(card->sdev);
        }
        CsrMutexUnlock(&card->mutex);
        CsrMutexDestroy(&card->mutex);
        CsrMemFree(card);
    }
    return NULL;
}

static void card_del(CsrSdioCard *card)
{
    int f;

    if (card == NULL)
    {
        return;
    }

    for (f = 1; f < SDIO_MAX_FUNCTIONS; f++)
    {
        if (card->func[f])
        {
            os_list_del(&card->func[f]->node);
            CsrEventDestroy(&card->func[f]->acknowledge_event);
            CsrMemFree(card->func[f]);
        }
    }
    os_list_del(&card->node);
    sdio_close(card->sdev);
    CsrMutexDestroy(&card->mutex);
    CsrMemFree(card);

    CSR_LOG_TEXT_INFO((CSR_SDIO_LTO, CSR_SDIO_LTSO_SDIO, "SDIO card closed %08X", (CsrUint32) (CsrUintptr) card));
}

static void module_init(void)
{
    CsrGlobalMutexLock();

    if (registerCount == 0)
    {
        CsrLogTextRegister(CSR_SDIO_LTO, "SDIO", CSR_ARRAY_SIZE(subOrigins), subOrigins);

        CsrMutexCreate(&glue_mutex);
        os_list_init(&glue_drivers);
        os_list_init(&glue_funcs);
        os_list_init(&glue_cards);
    }

    registerCount++;

    CsrGlobalMutexUnlock();
}

static void module_cleanup(void)
{
    CsrMutexLock(&glue_mutex);
    CsrGlobalMutexLock();

    if (--registerCount == 0)
    {
        CsrMutexUnlock(&glue_mutex);
        CsrMutexDestroy(&glue_mutex);
    }
    else
    {
        CsrMutexUnlock(&glue_mutex);
    }

    CsrGlobalMutexUnlock();
}
