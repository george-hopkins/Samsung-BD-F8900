/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <initguid.h>
#include "libsdio_internal.h"

#include <stdlib.h>
#include <setupapi.h>

static DWORD WINAPI int_thread_func(LPVOID arg);
static int error_to_status(ULONG error);
static BOOL find_iface(const char **device, const char *iface);

sdio_uif_t LIBSDIOAPI sdio_open(const char *dev_filename,
    sdio_int_handler_t int_handler, void *arg)
{
    struct sdio_uif *uif;
    struct sdioemb_uif_input input;
    struct sdioemb_uif_output output;
    CsrUint8 i;
    BOOL found;
    char *device;
    int ret;

    /* Strip '/dev/' from start of the device name. */
    if (strncmp(dev_filename, "/dev/", 5) == 0)
    {
        dev_filename += 5;
    }

    uif = malloc(sizeof(*uif));
    if (!uif)
    {
        goto err;
    }
    memset(uif, 0, sizeof(*uif));

    uif->handle = NULL;
    uif->int_handler = int_handler;
    uif->arg = arg;

    uif->int_thread_stop_event = CreateEvent(NULL, TRUE, FALSE, NULL);
    uif->int_unmasked_event = CreateEvent(NULL, TRUE, TRUE, NULL);

    found = find_iface(&device, dev_filename);
    if (!found)
    {
        goto err;
    }
    uif->handle = CreateFile(device, GENERIC_READ | GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    if (uif->handle == INVALID_HANDLE_VALUE)
    {
        goto err;
    }

    ret = uif_ioctl(uif, SDIOEMB_UIF_IOC_GET_CARD_INFO, &input, &output);
    if (ret < 0)
    {
        goto err;
    }
    uif->card_info = output.card_info;

    for (i = 0; i <= uif->card_info.num_functions; i++)
    {
        input.get_func_info.function = i;
        ret = uif_ioctl(uif, SDIOEMB_UIF_IOC_GET_FUNC_INFO, &input, &output);
        if (ret < 0)
        {
            goto err;
        }
        uif->func_info[i] = output.func_info;
    }

    ret = async_thread_start(uif);
    if (ret < 0)
    {
        goto err;
    }

    if (uif->int_handler)
    {
        uif->int_thread = CreateThread(NULL, 0, int_thread_func, uif, 0, NULL);
        if (uif->int_thread == INVALID_HANDLE_VALUE)
        {
            goto err;
        }
    }

    return uif;

err:
    sdio_close(uif);
    return NULL;
}

void LIBSDIOAPI sdio_close(sdio_uif_t uif)
{
    if (uif == NULL)
    {
        return;
    }
    if (uif->int_thread && uif->int_handler)
    {
        SetEvent(uif->int_thread_stop_event);
        WaitForSingleObject(uif->int_thread, INFINITE);
    }
    if (uif->async_thread)
    {
        async_thread_stop(uif);
    }
    if (uif->handle)
    {
        CloseHandle(uif->handle);
    }
    if (uif->int_unmasked_event)
    {
        CloseHandle(uif->int_unmasked_event);
    }
    if (uif->int_thread_stop_event)
    {
        CloseHandle(uif->int_thread_stop_event);
    }
    free(uif);
}

int LIBSDIOAPI sdio_num_functions(sdio_uif_t uif)
{
    return uif->card_info.num_functions;
}

int LIBSDIOAPI sdio_set_bus_width(sdio_uif_t uif, int bus_width)
{
    struct sdioemb_uif_input input;
    struct sdioemb_uif_output output;

    input.set_bus_width.bus_width = (CsrUint8) bus_width;

    if (uif_ioctl(uif, SDIOEMB_UIF_IOC_SET_BUS_WIDTH, &input, &output) < 0)
    {
        return -1;
    }
    return 0;
}

void LIBSDIOAPI sdio_set_max_bus_freq(sdio_uif_t uif, int max_freq)
{
    struct sdioemb_uif_input input;
    struct sdioemb_uif_output output;

    input.set_bus_freq.bus_freq = max_freq;

    uif_ioctl(uif, SDIOEMB_UIF_IOC_SET_BUS_FREQ, &input, &output);
}

CsrUint16 LIBSDIOAPI sdio_manf_id(sdio_uif_t uif)
{
    return uif->func_info[0].manf_id;
}

CsrUint16 LIBSDIOAPI sdio_card_id(sdio_uif_t uif)
{
    return uif->func_info[0].card_id;
}

CsrUint8 LIBSDIOAPI sdio_std_if(sdio_uif_t uif, int func)
{
    return uif->func_info[func].std_if;
}

int LIBSDIOAPI sdio_max_block_size(sdio_uif_t uif, int func)
{
    return uif->func_info[func].max_block_size;
}

int LIBSDIOAPI sdio_block_size(sdio_uif_t uif, int func)
{
    struct sdioemb_uif_input input;
    struct sdioemb_uif_output output;

    input.get_func_info.function = (CsrUint8) func;

    if (uif_ioctl(uif, SDIOEMB_UIF_IOC_GET_FUNC_INFO, &input, &output) < 0)
    {
        return -1;
    }
    return output.func_info.block_size;
}

int LIBSDIOAPI sdio_set_block_size(sdio_uif_t uif, int func, int blksz)
{
    struct sdioemb_uif_input input;
    struct sdioemb_uif_output output;

    input.set_block_size.function = (CsrUint8) func;
    input.set_block_size.block_size = (CsrUint16) blksz;

    return uif_ioctl(uif, SDIOEMB_UIF_IOC_SET_BLOCK_SIZE, &input, &output);
}

int LIBSDIOAPI sdio_read8(sdio_uif_t uif, int func, CsrUint32 addr, CsrUint8 *data)
{
    struct sdioemb_uif_input input;
    struct sdioemb_uif_output output;

    input.cmd.type = SDIOEMB_UIF_CMD52_READ;
    input.cmd.function = func;
    input.cmd.address = addr;
    input.cmd.data = data;
    input.cmd.len = 1;

    return uif_ioctl(uif, SDIOEMB_UIF_IOC_CMD, &input, &output);
}

int LIBSDIOAPI sdio_write8(sdio_uif_t uif, int func, CsrUint32 addr, CsrUint8 data)
{
    struct sdioemb_uif_input input;
    struct sdioemb_uif_output output;

    input.cmd.type = SDIOEMB_UIF_CMD52_WRITE;
    input.cmd.function = func;
    input.cmd.address = addr;
    input.cmd.data = &data;
    input.cmd.len = 1;

    return uif_ioctl(uif, SDIOEMB_UIF_IOC_CMD, &input, &output);
}

int LIBSDIOAPI sdio_read(sdio_uif_t uif, int func, CsrUint32 addr, CsrUint8 *data,
    size_t len, int block_size)
{
    struct sdioemb_uif_input input;
    struct sdioemb_uif_output output;

    input.cmd.type = SDIOEMB_UIF_CMD53_READ;
    input.cmd.function = func;
    input.cmd.address = addr;
    input.cmd.data = data;
    input.cmd.len = len;
    input.cmd.block_size = (CsrUint16) block_size;

    return uif_ioctl(uif, SDIOEMB_UIF_IOC_CMD, &input, &output);
}

int LIBSDIOAPI sdio_write(sdio_uif_t uif, int func, CsrUint32 addr, const CsrUint8 *data,
    size_t len, int block_size)
{
    struct sdioemb_uif_input input;
    struct sdioemb_uif_output output;

    input.cmd.type = SDIOEMB_UIF_CMD53_WRITE;
    input.cmd.function = func;
    input.cmd.address = addr;
    input.cmd.data = (CsrUint8 *) data;      /* const cast is safe */
    input.cmd.len = len;
    input.cmd.block_size = (CsrUint16) block_size;

    return uif_ioctl(uif, SDIOEMB_UIF_IOC_CMD, &input, &output);
}

int LIBSDIOAPI sdio_hard_reset(sdio_uif_t uif)
{
    struct sdioemb_uif_input input;
    struct sdioemb_uif_output output;

    return uif_ioctl(uif, SDIOEMB_UIF_IOC_HARDRESET, &input, &output);
}

int uif_ioctl(sdio_uif_t uif, DWORD ioctl, struct sdioemb_uif_input *input,
    struct sdioemb_uif_output *output)
{
    OVERLAPPED ol = {0, };
    BOOL success;
    DWORD len;
    DWORD error;

    ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    success = DeviceIoControl(uif->handle, ioctl, input, sizeof(*input), output, sizeof(*output),
        NULL, &ol);
    if (!success)
    {
        error = GetLastError();
        if (error != ERROR_IO_PENDING)
        {
            goto out;
        }
    }
    WaitForSingleObject(&ol.hEvent, INFINITE);
    success = GetOverlappedResult(uif->handle, &ol, &len, TRUE);
    if (!success)
    {
        error = GetLastError();
        goto out;
    }
    error = 0;
out:
    CloseHandle(ol.hEvent);
    return error_to_status(error);
}

static DWORD WINAPI int_thread_func(LPVOID arg)
{
    sdio_uif_t uif = arg;
    OVERLAPPED ol = {0, };
    HANDLE events[2];

    ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    events[0] = uif->int_thread_stop_event;
    events[1] = ol.hEvent;

    for ( ; ; )
    {
        struct sdioemb_uif_input input;
        struct sdioemb_uif_output output;
        DWORD len;
        BOOL success;
        DWORD result;

        success = DeviceIoControl(uif->handle, SDIOEMB_UIF_IOC_WAITFORINT, &input, sizeof(input),
            &output, sizeof(output), NULL, &ol);
        if (!success && (GetLastError() != ERROR_IO_PENDING))
        {
            break;
        }
        result = WaitForMultipleObjects(2, events, FALSE, INFINITE);
        if (result == WAIT_OBJECT_0)
        {
            CancelIo(uif->handle);
        }
        success = GetOverlappedResult(uif->handle, &ol, &len, TRUE);
        if (!success || (result == WAIT_OBJECT_0))
        {
            break;
        }
        uif->int_handler(uif, uif->arg);
        WaitForSingleObject(uif->int_unmasked_event, INFINITE);

        ResetEvent(ol.hEvent);
    }

    CloseHandle(ol.hEvent);

    return 0;
}

void LIBSDIOAPI sdio_interrupt_mask(sdio_uif_t uif)
{
    ResetEvent(uif->int_unmasked_event);
}

void LIBSDIOAPI sdio_interrupt_unmask(sdio_uif_t uif)
{
    SetEvent(uif->int_unmasked_event);
}

static int error_to_status(ULONG error)
{
    /* From http://support.microsoft.com/kb/113996 */
    switch (error)
    {
        case NO_ERROR:
            return SDIO_SUCCESS;
        case ERROR_BUSY:
            return SDIO_EAGAIN;
        case ERROR_INVALID_PARAMETER:
            return SDIO_EINVAL;
        case ERROR_IO_DEVICE:
            return SDIO_EIO;
        case ERROR_FILE_NOT_FOUND:
            return SDIO_ENODEV;
        case ERROR_NO_SYSTEM_RESOURCES:
            return SDIO_ENOMEM;
        case ERROR_NOT_SUPPORTED:
            return SDIO_ENOTSUPP;
        case ERROR_NOT_FOUND:
            return SDIO_ENXIO;
        case ERROR_SEM_TIMEOUT:
            return SDIO_ETIMEDOUT;
        default:
            return SDIO_EIO;
    }
}

static BOOL find_iface(const char **device, const char *iface)
{
    HDEVINFO hardwareDeviceInfo;
    SP_DEVICE_INTERFACE_DATA deviceInterfaceData;
    PSP_DEVICE_INTERFACE_DETAIL_DATA deviceInterfaceDetailData = NULL;
    ULONG predictedLength = 0;
    ULONG requiredLength = 0, bytes = 0;
    int i;
    BOOL found = FALSE;

    /*
       Open a handle to the device interface information set of all
       present UCAPS logger class interfaces.
    */

    hardwareDeviceInfo = SetupDiGetClassDevs(
        (LPGUID) &GUID_DEVINTERFACE_SDIOEMB_UIF,
        NULL,                /* Define no enumerator (global) */
        NULL,                /* Define no */
        (DIGCF_PRESENT |                /* Only Devices present */
         DIGCF_DEVICEINTERFACE));               /* Function class devices. */
    if (hardwareDeviceInfo == INVALID_HANDLE_VALUE)
    {
        return FALSE;
    }

    deviceInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    /* Enumerate devices. */
    for (i = 0; ; i++)
    {
        if (SetupDiEnumDeviceInterfaces(hardwareDeviceInfo,
                0,                          /* No care about specific PDOs */
                (LPGUID) &GUID_DEVINTERFACE_SDIOEMB_UIF,
                i,
                &deviceInterfaceData))
        {
            if (deviceInterfaceDetailData)
            {
                free(deviceInterfaceDetailData);
                deviceInterfaceDetailData = NULL;
            }

            /*
               Allocate a function class device data structure to
               receive the information about this particular device.
            */

            /*
               First find out required length of the buffer
            */

            if (!SetupDiGetDeviceInterfaceDetail(
                    hardwareDeviceInfo,
                    &deviceInterfaceData,
                    NULL, /* probing so no output buffer yet */

                    0, /* probing so output buffer length of zero */
                    &requiredLength,
                    NULL))  /* not interested in the specific dev-node */
            {
                if (ERROR_INSUFFICIENT_BUFFER != GetLastError())
                {
                    SetupDiDestroyDeviceInfoList(hardwareDeviceInfo);
                    return FALSE;
                }
            }

            predictedLength = requiredLength;

            deviceInterfaceDetailData = malloc(predictedLength);

            if (deviceInterfaceDetailData)
            {
                deviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
            }
            else
            {
                break;
            }

            if (!SetupDiGetDeviceInterfaceDetail(
                    hardwareDeviceInfo,
                    &deviceInterfaceData,
                    deviceInterfaceDetailData,
                    predictedLength,
                    &requiredLength,
                    NULL))
            {
                break;
            }

            if (strstr(deviceInterfaceDetailData->DevicePath, iface) != NULL)
            {
                found = TRUE;
                *device = strdup(deviceInterfaceDetailData->DevicePath);
                break;
            }
        }
        else if (ERROR_NO_MORE_ITEMS == GetLastError())
        {
            break;
        }
    }

    SetupDiDestroyDeviceInfoList(hardwareDeviceInfo);
    free(deviceInterfaceDetailData);
    return found;
}
