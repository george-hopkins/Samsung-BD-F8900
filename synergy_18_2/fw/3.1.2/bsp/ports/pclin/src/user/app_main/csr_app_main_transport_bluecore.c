/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#include <stdio.h>

#include "csr_types.h"
#include "csr_util.h"
#include "csr_tm_bluecore_task.h"
#include "csr_transport.h"
#include "csr_app_main.h"
#include "csr_arg_search.h"
#include "platform/csr_serial_init.h"
#ifdef CSR_HCI_SOCKET_TRANSPORT
#include "platform/csr_hci_socket_init.h"
#else
#include "platform/csr_usb_init.h"
#endif

#ifdef CSR_HCI_SOCKET_TRANSPORT
#define BLUECORE_USB_DEVICE "hci0"
#else
#define BLUECORE_USB_DEVICE "/dev/bt_usb"
#endif

#define BLUECORE_UART_DEVICE "/dev/ttyS0"
#define BLUECORE_UART_DEFAULT_BAUD_RATE 115200

static CsrUint16 bluecoreTransport = TRANSPORT_TYPE_BCSP;
CsrUint16 CsrAppMainBluecoreTransportGet(void)
{
    return bluecoreTransport;
}

static CsrUint32 bluecorePreBootstrapBaudrate = 0;
CsrUint32 CsrAppMainBluecorePreBootstrapBaudrateGet(void)
{
    return bluecorePreBootstrapBaudrate;
}

static CsrUint32 bluecorePostBootstrapBaudrate = 0;
CsrUint32 CsrAppMainBluecorePostBootstrapBaudrateGet(void)
{
    return bluecorePostBootstrapBaudrate;
}

static CsrUint32 bluecoreXtalFrequency = 26000;
CsrUint32 CsrAppMainBluecoreXtalFrequencyGet(void)
{
    return bluecoreXtalFrequency;
}

static CsrUint8 bluecoreLocalDeviceBluetoothAddress[6] = {0x00, 0x02, 0x5B, 0x01, 0x02, 0x03};
void CsrAppMainBluecoreLocalDeviceBluetoothAddressGet(CsrUint8 address[6])
{
    if (address != NULL)
    {
        CsrMemCpy(address, bluecoreLocalDeviceBluetoothAddress, sizeof(bluecoreLocalDeviceBluetoothAddress));
    }
}

static CsrCharString *bluecoreLocalDeviceName = NULL;
const CsrCharString *CsrAppMainBluecoreLocalDeviceNameGet(void)
{
    return bluecoreLocalDeviceName;
}

static CsrCharString *bluecorePsrFile = NULL;
const CsrCharString *CsrAppMainBluecorePsrFileGet(void)
{
    return bluecorePsrFile;
}

static CsrCharString *bluecorePsrString = NULL;
const CsrCharString *CsrAppMainBluecorePsrStringGet(void)
{
    return bluecorePsrString;
}

static void *uartHandle = NULL;

CsrResult CsrAppMainTransportInitialise(void)
{
    CsrCharString *parameter;
    CsrCharString *value;
    CsrCharString *port = BLUECORE_UART_DEVICE;

    if (CsrArgSearch(NULL, "--bc-transport", &parameter, &value))
    {
        if (value == NULL)
        {
            printf("No value specified for parameter --bc-transport\n");
            return CSR_RESULT_FAILURE;
        }

        if (CsrStrCmp(value, "bcsp") == 0)
        {
            bluecoreTransport = TRANSPORT_TYPE_BCSP;
            port = BLUECORE_UART_DEVICE;
        }
        else if (CsrStrCmp(value, "usb") == 0)
        {
            bluecoreTransport = TRANSPORT_TYPE_USB;
            port = BLUECORE_USB_DEVICE;
        }
        else if (CsrStrCmp(value, "h4i") == 0)
        {
            bluecoreTransport = TRANSPORT_TYPE_H4_I;
            port = BLUECORE_UART_DEVICE;
        }
        else if (CsrStrCmp(value, "h4ds") == 0)
        {
            bluecoreTransport = TRANSPORT_TYPE_H4_DS;
            port = BLUECORE_UART_DEVICE;
        }
        else if (CsrStrCmp(value, "typea") == 0)
        {
            bluecoreTransport = TRANSPORT_TYPE_TYPE_A;
        }
        else
        {
            printf("Unknown transport '%s' for parameter %s\n", value, parameter);
            return CSR_RESULT_FAILURE;
        }
    }

    if (CsrArgSearch(NULL, "--bc-port", &parameter, &value))
    {
        if (value == NULL)
        {
            printf("No value specified for parameter %s\n", parameter);
            return CSR_RESULT_FAILURE;
        }
        port = value;
    }

    if (CsrArgSearch(NULL, "--bc-baud", &parameter, &value))
    {
        if (value == NULL)
        {
            printf("No value specified for parameter %s\n", parameter);
            return CSR_RESULT_FAILURE;
        }
        bluecorePreBootstrapBaudrate = CsrUartDrvBaudrateActualGet(CsrStrToInt(value));
    }
    else
    {
        bluecorePreBootstrapBaudrate = CsrUartDrvBaudrateActualGet(BLUECORE_UART_DEFAULT_BAUD_RATE);
    }

    if (CsrArgSearch(NULL, "--bc-bs-baud", &parameter, &value))
    {
        if (value == NULL)
        {
            printf("No value specified for parameter %s\n", parameter);
            return CSR_RESULT_FAILURE;
        }
        bluecorePostBootstrapBaudrate = CsrUartDrvBaudrateActualGet(CsrStrToInt(value));
    }
    else
    {
        bluecorePostBootstrapBaudrate = CsrUartDrvBaudrateActualGet(BLUECORE_UART_DEFAULT_BAUD_RATE);
    }

    if (CsrArgSearch(NULL, "--bc-bs-xtal", &parameter, &value))
    {
        if (value == NULL)
        {
            printf("No value specified for parameter %s\n", parameter);
            return CSR_RESULT_FAILURE;
        }
        bluecoreXtalFrequency = CsrStrToInt(value);
    }

    if (CsrArgSearch(NULL, "--bc-bs-address", &parameter, &value))
    {
        CsrUint8 *addr = bluecoreLocalDeviceBluetoothAddress;
        if (value == NULL)
        {
            printf("No value specified for parameter %s\n", parameter);
            return CSR_RESULT_FAILURE;
        }
        if (sscanf(value, "%2hhx%2hhx:%2hhx:%2hhx%2hhx%2hhx", &addr[0], &addr[1], &addr[2], &addr[3], &addr[4], &addr[5]) != 6)
        {
            printf("Invalid value '%s' for parameter %s\n", value, parameter);
            return CSR_RESULT_FAILURE;
        }
    }
    if (CsrArgSearch(NULL, "--bc-bs-device-name", &parameter, &value))
    {
        if (value == NULL)
        {
            printf("No value specified for parameter %s\n", parameter);
            return CSR_RESULT_FAILURE;
        }
        bluecoreLocalDeviceName = value;
    }

    if (CsrArgSearch(NULL, "--bc-bs-psr-file", &parameter, &value))
    {
        if (value == NULL)
        {
            printf("No value specified for parameter %s\n", parameter);
            return CSR_RESULT_FAILURE;
        }
        bluecorePsrFile = value;
    }

    if (CsrArgSearch(NULL, "--bc-bs-psr-string", &parameter, &value))
    {
        if (value == NULL)
        {
            printf("No value specified for parameter %s\n", parameter);
            return CSR_RESULT_FAILURE;
        }
        bluecorePsrString = value;
    }

    switch (bluecoreTransport)
    {
        case TRANSPORT_TYPE_BCSP:
        {
            CsrUint8 token = 0xC0;
            uartHandle = CsrUartDrvInitialise(port,
                bluecorePreBootstrapBaudrate,
                bluecorePostBootstrapBaudrate,
                CSR_UART_DRV_DATA_BITS_8,
                CSR_UART_DRV_PARITY_EVEN,
                CSR_UART_DRV_STOP_BITS_1,
                FALSE, &token);
            if (uartHandle == NULL)
            {
                printf("Unable to initialise UART driver\n");
                return CSR_RESULT_FAILURE;
            }
            CsrTmBlueCoreRegisterUartHandleBcsp(uartHandle);
            break;
        }
        case TRANSPORT_TYPE_USB:
        {
#ifdef CSR_HCI_SOCKET_TRANSPORT
            CsrTransportHciSocketConfigure(port);
#else
            CsrUsbDrvConfigure(port);
#endif
            break;
        }
        case TRANSPORT_TYPE_H4_DS:
        {
            uartHandle = CsrUartDrvInitialise(port,
                bluecorePreBootstrapBaudrate,
                bluecorePostBootstrapBaudrate,
                CSR_UART_DRV_DATA_BITS_8,
                CSR_UART_DRV_PARITY_NONE,
                CSR_UART_DRV_STOP_BITS_1,
                TRUE, NULL);
            if (uartHandle == NULL)
            {
                printf("Unable to initialise UART driver\n");
                return CSR_RESULT_FAILURE;
            }
            CsrTmBlueCoreRegisterUartHandleH4ds(uartHandle);
            break;
        }
        case TRANSPORT_TYPE_TYPE_A:
            break;
        default:
            break;
    }

    return CSR_RESULT_SUCCESS;
}

void CsrAppMainTransportDeinitialise(void)
{
    if (uartHandle != NULL)
    {
        CsrUartDrvDeinitialise(uartHandle);
        uartHandle = NULL;
    }
}

void CsrAppMainTransportUsage(void)
{
    printf("--bc-transport <bcsp|usb|h4i|h4ds|typea> - Bluecore transport type (bcsp)\n");
    printf("--bc-port <port> - Bluecore device port for bcsp/h4ds/usb transports (" BLUECORE_UART_DEVICE ", " BLUECORE_USB_DEVICE ")\n");
    printf("--bc-baud <rate> - Initial baud rate for bcsp/h4ds transports (%d)\n", BLUECORE_UART_DEFAULT_BAUD_RATE);
    printf("--bc-bs-baud <rate> - Bootstrap baud rate for bcsp/h4ds transports (%d)\n", BLUECORE_UART_DEFAULT_BAUD_RATE);
    printf("--bc-bs-xtal <frequency> - Bootstrap crystal frequency in kHz (%u)\n", bluecoreXtalFrequency);
    printf("--bc-bs-address <XXXX:XX:XXXXXX> - Bootstrap local device address (%02X%02X:%02X:%02X%02X%02X)\n", bluecoreLocalDeviceBluetoothAddress[0], bluecoreLocalDeviceBluetoothAddress[1], bluecoreLocalDeviceBluetoothAddress[2], bluecoreLocalDeviceBluetoothAddress[3], bluecoreLocalDeviceBluetoothAddress[4], bluecoreLocalDeviceBluetoothAddress[5]);
    printf("--bc-bs-device-name <name> - Bootstrap local device name\n");
    printf("--bc-bs-psr-file <file> - PSR file to pass to the bootstrap\n");
    printf("--bc-bs-psr-string <string> - PSR string to pass to the bootstrap\n");
}
