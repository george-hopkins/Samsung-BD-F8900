/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdlib.h>
#include <limits.h>

#include "csr_types.h"

#include "libsdio_internal.h"

static DWORD WINAPI async_thread_func(LPVOID arg);

int async_thread_start(sdio_uif_t uif)
{
    BOOL success;

    success = InitializeCriticalSectionAndSpinCount(&uif->cmd_queue_mutex, 0x80000400);
    if (!success)
    {
        return -1;
    }

    uif->cmd_queue.head = NULL;
    uif->cmd_queue.tail = NULL;

    uif->cmd_queue_sem = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
    uif->cmd_queue_stop_event = CreateEvent(NULL, TRUE, FALSE, NULL);

    uif->async_thread = CreateThread(NULL, 0, async_thread_func, uif, 0, NULL);
    if (uif->async_thread == INVALID_HANDLE_VALUE)
    {
        goto err;
    }
    return 0;

err:
    async_thread_stop(uif);
    return -1;
}

void async_thread_stop(sdio_uif_t uif)
{
    struct cmd_queue_elem *elem;

    if (uif->async_thread)
    {
        SetEvent(uif->cmd_queue_stop_event);
        WaitForSingleObject(uif->async_thread, INFINITE);
    }

    elem = uif->cmd_queue.head;
    while (elem)
    {
        struct cmd_queue_elem *next = elem->next;
        free(elem);
        elem = next;
    }

    DeleteCriticalSection(&uif->cmd_queue_mutex);
    CloseHandle(uif->cmd_queue_stop_event);
    CloseHandle(uif->cmd_queue_sem);
}

static int cmd_queue_push(sdio_uif_t uif, enum sdioemb_uif_cmd_type type, int func, CsrUint32 addr,
    const CsrUint8 *data, size_t len, int block_size,
    sdio_io_callback_t callback, void *arg)
{
    struct cmd_queue_elem *elem;

    elem = malloc(sizeof(struct cmd_queue_elem));
    if (elem == NULL)
    {
        return -1;
    }

    elem->callback = callback;
    elem->arg = arg;

    if (type == SDIOEMB_UIF_CMD52_WRITE)
    {
        elem->write8_data = *data;
        data = &elem->write8_data;
    }

    elem->cmd.type = type;
    elem->cmd.function = func;
    elem->cmd.address = addr;
    elem->cmd.data = (CsrUint8 *) data;      /* const cast is safe */
    elem->cmd.len = len;
    elem->cmd.block_size = (UINT16) block_size;

    EnterCriticalSection(&uif->cmd_queue_mutex);
    elem->next = NULL;
    if (uif->cmd_queue.tail != NULL)
    {
        uif->cmd_queue.tail->next = elem;
    }
    if (uif->cmd_queue.head == NULL)
    {
        uif->cmd_queue.head = elem;
    }
    uif->cmd_queue.tail = elem;
    LeaveCriticalSection(&uif->cmd_queue_mutex);
    ReleaseSemaphore(uif->cmd_queue_sem, 1, NULL);

    return 0;
}

static struct cmd_queue_elem *cmd_queue_pop(sdio_uif_t uif)
{
    struct cmd_queue_elem *elem;
    HANDLE events[2];
    DWORD result;

    events[0] = uif->cmd_queue_stop_event;
    events[1] = uif->cmd_queue_sem;

    result = WaitForMultipleObjects(2, events, FALSE, INFINITE);
    if (result == WAIT_OBJECT_0)
    {
        return NULL;
    }

    EnterCriticalSection(&uif->cmd_queue_mutex);
    elem = uif->cmd_queue.head;
    uif->cmd_queue.head = uif->cmd_queue.head->next;
    if (uif->cmd_queue.head == NULL)
    {
        uif->cmd_queue.tail = NULL;
    }
    LeaveCriticalSection(&uif->cmd_queue_mutex);

    return elem;
}

static DWORD WINAPI async_thread_func(LPVOID arg)
{
    struct sdio_uif *uif = arg;

    for ( ; ; )
    {
        struct cmd_queue_elem *elem;
        struct sdioemb_uif_input input;
        struct sdioemb_uif_output output;
        int ret;

        elem = cmd_queue_pop(uif);
        if (elem == NULL)
        {
            break;
        }
        input.cmd = elem->cmd;
        ret = uif_ioctl(uif, SDIOEMB_UIF_IOC_CMD, &input, &output);
        elem->callback(uif, elem->arg, ret);
        free(elem);
    }

    return 0;
}

int LIBSDIOAPI sdio_read8_async(sdio_uif_t uif, int func, CsrUint32 addr, CsrUint8 *data,
    sdio_io_callback_t callback, void *arg)
{
    return cmd_queue_push(uif, SDIOEMB_UIF_CMD52_READ, func, addr, data, 1, 0, callback, arg);
}

int LIBSDIOAPI sdio_write8_async(sdio_uif_t uif, int func, CsrUint32 addr, CsrUint8 data,
    sdio_io_callback_t callback, void *arg)
{
    return cmd_queue_push(uif, SDIOEMB_UIF_CMD52_WRITE, func, addr, &data, 1, 0, callback, arg);
}

int LIBSDIOAPI sdio_read_async(sdio_uif_t uif, int func, CsrUint32 addr, CsrUint8 *data,
    size_t len, int block_size,
    sdio_io_callback_t callback, void *arg)
{
    return cmd_queue_push(uif, SDIOEMB_UIF_CMD53_READ, func, addr, data, len, block_size,
        callback, arg);
}

int LIBSDIOAPI sdio_write_async(sdio_uif_t uif, int func, CsrUint32 addr, const CsrUint8 *data,
    size_t len, int block_size,
    sdio_io_callback_t callback, void *arg)
{
    return cmd_queue_push(uif, SDIOEMB_UIF_CMD53_WRITE, func, addr, data, len, block_size,
        callback, arg);
}
