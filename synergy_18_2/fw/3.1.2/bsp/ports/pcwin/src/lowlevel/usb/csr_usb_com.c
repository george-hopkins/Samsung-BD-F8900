/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/
#include <windows.h>
#include <winioctl.h>
#include <process.h>
#include <stdlib.h>
#include <stdio.h>

#include "csr_types.h"
#include "csr_pmem.h"
#include "csr_sched.h"
#include "csr_usb_com.h"
#include "csr_transport.h"
#include "csr_tm_bluecore_usb.h"

#include "csr_aclbuf.h"
#include "csr_aclbuf_lower.h"

#define CSRBC_IOCTL_INDEX  0x0000


#define IOCTL_CSRBC_SEND_HCI_COMMAND            CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX, \
    METHOD_BUFFERED, \
    FILE_ANY_ACCESS)

/* for compatibility reasons with older software */
#define IOCTL_CSRBC_SEND_HCI_COMMAND_LEGACY CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX, \
    METHOD_OUT_DIRECT, \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_GET_HCI_EVENT               CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX + 1, \
    METHOD_BUFFERED, \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_GET_VERSION             CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX + 2, \
    METHOD_BUFFERED, \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_GET_DRIVER_NAME         CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX + 3, \
    METHOD_BUFFERED, \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_GET_CONFIG_DESCRIPTOR     CTL_CODE(FILE_DEVICE_UNKNOWN,  \
    CSRBC_IOCTL_INDEX + 4, \
    METHOD_BUFFERED,  \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_GET_DEVICE_DESCRIPTOR     CTL_CODE(FILE_DEVICE_UNKNOWN,  \
    CSRBC_IOCTL_INDEX + 5, \
    METHOD_BUFFERED,  \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_RESET_DEVICE                CTL_CODE(FILE_DEVICE_UNKNOWN,  \
    CSRBC_IOCTL_INDEX + 6, \
    METHOD_BUFFERED,  \
    FILE_ANY_ACCESS)

/* for compatibility reasons with older software */
#define IOCTL_CSRBC_SEND_HCI_DATA_LEGACY        CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX + 10, \
    METHOD_OUT_DIRECT, \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_SEND_HCI_DATA               CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX + 10, \
    METHOD_BUFFERED, \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_GET_HCI_DATA                CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX + 11, \
    METHOD_BUFFERED, \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_BLOCK_HCI_EVENT         CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX + 12, \
    METHOD_BUFFERED, \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_BLOCK_HCI_DATA          CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX + 13, \
    METHOD_BUFFERED, \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_SEND_CONTROL_TRANSFER       CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX + 14, \
    METHOD_BUFFERED, \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_START_SCO_DATA          CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX + 16, \
    METHOD_BUFFERED, \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_SEND_SCO_DATA               CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX + 17, \
    METHOD_BUFFERED, \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_RECV_SCO_DATA               CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX + 18, \
    METHOD_BUFFERED, \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_STOP_SCO_DATA               CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX + 19, \
    METHOD_BUFFERED, \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_SET_EVENT_LENGTH            CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX + 20, \
    METHOD_BUFFERED, \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_HCI_HID_SWITCH_COMMAND  CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX + 21, \
    METHOD_BUFFERED, \
    FILE_ANY_ACCESS)

#define IOCTL_CSRBC_RECV_SCO_DATA_MD                CTL_CODE(FILE_DEVICE_UNKNOWN, \
    CSRBC_IOCTL_INDEX + 22, \
    METHOD_OUT_DIRECT, \
    FILE_ANY_ACCESS)

#ifdef USB_DEBUG_ENABLE
CsrUint16 dbg_i;
#define DBG_PRINT(t)       printf("%s in file: %s, line %i\n", t, __FILE__, __LINE__);
#define DBG_PRINT_INT(t, i) printf("%s %i in file: %s, line %i\n", t, i, __FILE__, __LINE__);
#define DBG_DUMP(t, b, l)    printf("\n%s", t); \
    for (dbg_i = 0; dbg_i < l; dbg_i++) {printf("%2X ", (CsrUint8) b[dbg_i]);}
#else
#define DBG_PRINT(t)
#define DBG_PRINT_INT(t, i)
#define DBG_DUMP(t, b, l)
#endif /*DEBUG_ENABLE*/
#define CSR_MAX_SCO_CONNECTIONS 0
/* buffer types */
typedef struct
{
    CsrUint8 *buf;
    CsrUint32 buflen;
} hciTxBuf_t;

typedef struct
{
    CsrUint8  type;
    CsrUint8 *buf;
    CsrUint32 buflen;
} hciRxBuf_t;

#define NUM_ACL_BUFFERS    12
#define NUM_HCI_BUFFERS    12
#define NUM_SCO_BUFFERS    12
#define NUM_RX_BUFFERS     16

/* rx buffers */
static CsrUint8 data_buffer[65536 + 64];   /* ACL data size + bulk data payload size */
static CsrUint8 event_buffer[256 + 16];    /* HCI event size + interrupt data payload size */
#if CSR_MAX_SCO_CONNECTIONS > 0
static CsrUint8 sco_buffer[65536];       /* SCO data size */
#endif

/* internal events used to signal the thread */
static HANDLE send_data;
#if CSR_MAX_SCO_CONNECTIONS > 0
static HANDLE send_sco;
#endif
static HANDLE send_cmd;
static HANDLE shutdown_event;
static HANDLE thread_terminated;
static HANDLE wait_for_data;
static HANDLE wait_for_event;
#if CSR_MAX_SCO_CONNECTIONS > 0
static HANDLE wait_for_sco;
static HANDLE wait_for_start_sco;
static HANDLE wait_for_stop_sco;
#define MAX_USB_TX_EVENTS 7
#else
#define MAX_USB_TX_EVENTS 5
#endif
static HANDLE txEvents[MAX_USB_TX_EVENTS];

#define USB_DRIVER_NAME         "\\\\.\\CSR"
#define USB_DEFAULT_DRIVER      "\\\\.\\CSR0"
#define USB_INIT_MAX_RETRIES    10

/* the USB device handle */
static HANDLE devHandle;
static char devName[32] = "";

/* background interrupt handle */
static CsrSchedBgint usb_bgint_rx = CSR_SCHED_BGINT_INVALID;

/* ensure mutual exclusion for write and read */
static CRITICAL_SECTION txAclMutex;
static CRITICAL_SECTION txHciMutex;
#if CSR_MAX_SCO_CONNECTIONS > 0
static CRITICAL_SECTION txScoMutex;
#endif
static CRITICAL_SECTION rxMutex;
static CRITICAL_SECTION closeMutex;

/* the threads for rx and tx */
static HANDLE rxAclThreadHandle;
static HANDLE rxHciThreadHandle;
#if CSR_MAX_SCO_CONNECTIONS > 0
static HANDLE rxScoThreadHandle;
#endif
static HANDLE txThreadHandle;
static CsrUint8 thread_count;

/* buffers for exchange of data between scheduler and rx/tx threads */
static hciTxBuf_t aclTxBufArray[NUM_ACL_BUFFERS];
static CsrUint8 aclTxBufIndexCurrent;
static CsrUint8 aclTxBufIndexFree;

static hciTxBuf_t hciTxBufArray[NUM_HCI_BUFFERS];
static CsrUint8 hciTxBufIndexCurrent;
static CsrUint8 hciTxBufIndexFree;

#if CSR_MAX_SCO_CONNECTIONS > 0
static hciTxBuf_t scoTxBufArray[NUM_SCO_BUFFERS];
static CsrUint8 scoTxBufIndexCurrent;
static CsrUint8 scoTxBufIndexFree;
#endif

static HANDLE rxWaitBuffer;
static hciRxBuf_t rxBufArray[NUM_RX_BUFFERS];
static CsrUint8 rxBufIndexCurrent;
static CsrUint8 rxBufIndexFree;

static CsrUint8 data_paused = 0;
static CsrUint8 event_paused = 0;
static CsrBool usb_hci_running;

#if CSR_MAX_SCO_CONNECTIONS > 0
static CsrUint8 sco_packet_size;
#endif

/*----------------------------------------------------------------------------*
 * NAME
 *    get_hci_data (internal fct.)
 *
 * DESCRIPTION
 *    Performs the actual read of HCI events from the USB driver.
 *
 *
 * RETURNS
 *    The number of bytes read.
 *
 *******************************************************************************/
CsrUint32 get_hci_event(void *buffer, CsrUint32 length)
{
    CsrUint32 status;

    DWORD read;

    static OVERLAPPED overlapped = {0, 0, 0, 0, 0};

    HANDLE handles[2] = {wait_for_event, shutdown_event};
    DWORD err;

    if (event_paused > 0)
    {
        event_paused = 1;

        overlapped.hEvent = wait_for_event;

        status = DeviceIoControl(devHandle,
            IOCTL_CSRBC_BLOCK_HCI_EVENT,
            0,
            0,
            0,
            0,
            &read,
            &overlapped);

        if ((!status) && ((err = GetLastError()) == ERROR_IO_PENDING))
        {
            if (WaitForMultipleObjects(2, handles, FALSE, (DWORD) -1) == WAIT_OBJECT_0 + 1)
            {
                return 0;
            }
            read = 0;
        }
        else if (!status)
        {
            DBG_PRINT("get_hci_block_event fail");
            CsrUsbDrvStop();
            return 0;
        }
    }

    overlapped.hEvent = wait_for_event;

    status = DeviceIoControl(devHandle,
        IOCTL_CSRBC_GET_HCI_EVENT,
        0,
        0,
        buffer,
        length,
        &read,
        &overlapped);

    if ((!status) && ((err = GetLastError()) == ERROR_IO_PENDING))
    {
        if (WaitForMultipleObjects(2, handles, FALSE, (DWORD) -1) == WAIT_OBJECT_0 + 1)
        {
            return 0;
        }

        status = GetOverlappedResult(devHandle, &overlapped, &read, FALSE);
    }
    else if (!status)
    {
        DBG_PRINT_INT("get_hci_event fail", err);
        CsrUsbDrvStop();
        return 0;
    }

    if (status == 0)
    {
        read = 0;
    }

    if (read == 0)
    {
        event_paused++;
    }
    else
    {
        event_paused = 0;
    }

    return (CsrUint32) read;
}

/*----------------------------------------------------------------------------*
 * NAME
 *     rxHciThread (internal fct.)
 *
 * DESCRIPTION
 *
 *   Thread that reads HCI events from the USB driver. The events are entered into
 *    a buffer protected by mutex. The scheduler is triggered by a background
 *    interrupt to take over the data buffered.
 *
 * RETURNS
 *    -
 *
 *******************************************************************************/
void rxHciThread(void)
{
    CsrUint32 offset, length, size;
    HANDLE bufferEvents[2] = {shutdown_event, rxWaitBuffer};

    /* ''offset'' is the number of bytes in the buffer (always) */
    /* ''length'' is the length of the next event */
    /* ''size''   is the length of the last read from the driver*/

    thread_count++;
    size = 0;

    while (usb_hci_running)
    {
        offset = size;

        /* If we only have a small amount of data in the buffer, then
           we need to read in another chunk before we know the length */
        while ((offset < 2) && usb_hci_running)
        {
            size = get_hci_event(event_buffer + offset, 16);
            offset += size;
        }

        /* We now know the length of the current packet */
        length = (CsrUint8) event_buffer[1] + 2;

        /* Read in the entire packet (plus perhaps the start of the next one) */
        while ((offset < length) && usb_hci_running)
        {
            size = get_hci_event(event_buffer + offset, 16);
            offset += size;
        }

        if (usb_hci_running)
        {
            CsrUint8 *hciPacket;
            CsrBool found;

            /* Send the packet up */
            hciPacket = CsrMemDup(event_buffer, length);
            /*DBG_DUMP("Rx HCI:", event_buffer, length);*/

            found = FALSE;

            EnterCriticalSection(&rxMutex);

            do
            {
                if (rxBufArray[rxBufIndexFree].buflen != 0)
                {
                    DWORD event;

                    ResetEvent(rxWaitBuffer);

                    LeaveCriticalSection(&rxMutex);

                    event = WaitForMultipleObjects(2, bufferEvents,
                        FALSE, INFINITE);

                    switch (event)
                    {
                        case WAIT_OBJECT_0 + 0:
                        {
                            /*
                             * Driver shut down requested.  Free
                             * packet and go on -- thread terminates
                             * because of usb_hci_running.
                             */
                            CsrPmemFree(hciPacket);
                            break;
                        }

                        case WAIT_OBJECT_0 + 1:
                        {
                            EnterCriticalSection(&rxMutex);

                            if (rxBufArray[rxBufIndexFree].buflen == 0)
                            {
                                found = TRUE;
                            }
                            break;
                        }
                    }
                }
                else
                {
                    found = TRUE;
                }
            } while ((found == FALSE) && usb_hci_running);

            rxBufArray[rxBufIndexFree].buf = hciPacket;
            rxBufArray[rxBufIndexFree].type = TRANSPORT_CHANNEL_HCI;
            rxBufArray[rxBufIndexFree].buflen = length;
            rxBufIndexFree = (rxBufIndexFree + 1) % NUM_RX_BUFFERS;
            LeaveCriticalSection(&rxMutex);

            CsrSchedBgintSet(usb_bgint_rx);
        }

        /* If there is more data in the buffer that we have not read,
           then copy it down, update the remaining size, and loop */
        if (offset > length)
        {
            memmove(event_buffer, event_buffer + length, offset - length);
        }
        size = offset - length;
    }

    SetEvent(thread_terminated);
    ExitThread(0);
}

/*----------------------------------------------------------------------------*
 * NAME
 *    get_acl_data (internal fct.)
 *
 * DESCRIPTION
 *    Performs the actual read of ACL data from the USB driver.
 *
 *
 * RETURNS
 *    The number of bytes read.
 *
 *******************************************************************************/
CsrUint32 get_acl_data(void *buffer, CsrUint32 length)
{
    CsrUint32 status;
    DWORD read;

    static OVERLAPPED overlapped = {0, 0, 0, 0, 0};
    HANDLE handles[2] = {wait_for_data, shutdown_event};
    DWORD err;

    if (data_paused > 0)
    {
        data_paused = 1;

        overlapped.hEvent = wait_for_data;

        status = DeviceIoControl(devHandle,
            IOCTL_CSRBC_BLOCK_HCI_DATA,
            0,
            0,
            0,
            0,
            &read,
            &overlapped);

        if ((!status) && ((err = GetLastError()) == ERROR_IO_PENDING))
        {
            if (WaitForMultipleObjects(2, handles, FALSE, (DWORD) -1) == WAIT_OBJECT_0 + 1)
            {
                return 0;
            }
        }
        else if (!status)
        {
            DBG_PRINT("get_acl_data fail");
            CsrUsbDrvStop();
            return 0;
        }
    }

    overlapped.hEvent = wait_for_data;

    status = ReadFile(devHandle, buffer, length, &read, &overlapped);

    if ((!status) && ((err = GetLastError()) == ERROR_IO_PENDING))
    {
        if (WaitForMultipleObjects(2, handles, FALSE, (DWORD) -1) == WAIT_OBJECT_0 + 1)
        {
            return 0;
        }

        status = GetOverlappedResult(devHandle, &overlapped, &read, FALSE);
    }
    else if (!status)
    {
        DBG_PRINT("get_acl_data fail");
        CsrUsbDrvStop();
        return 0;
    }

    if (status == 0)
    {
        read = 0;
    }

    if (read == 0)
    {
        data_paused++;
    }
    else
    {
        data_paused = 0;
    }

    return (CsrUint32) read;
}

/*----------------------------------------------------------------------------*
 * NAME
 *     rxAclThread (internal fct.)
 *
 * DESCRIPTION
 *
 *   Thread that reads ACL data from the USB driver. The data is entered into
 *    a buffer protected by mutex. The scheduler is triggered by a background
 *    interrupt to take over the data buffered..
 *
 * RETURNS
 *    -
 *
 *******************************************************************************/
/* Define USB_PEEK_BYTES unless defined. */
#ifndef USB_PEEK_BYTES
#ifdef CSR_ACLBUF_USB_ENABLE
#define USB_PEEK_BYTES 16
#else
#define USB_PEEK_BYTES 0
#endif
#endif

#define ACL_HEADER_LENGTH 4

void rxAclThread(void)
{
    CsrUint32 offset, lengthInclHdr, size;
    CsrUint16 length;
    CsrUint16 aclHdlFlags;
    HANDLE bufferEvents[2] = {shutdown_event, rxWaitBuffer};

    /* ''offset'' is the number of bytes in the buffer (always) */
    /* ''lengthInclHdr'' is the lengthInclHdr of the next event */
    /* ''size''   is the lengthInclHdr of the last read from the driver*/

    thread_count++;
    size = 0;

    while (usb_hci_running)
    {
#ifdef CSR_ACLBUF_USB_ENABLE
        CsrUint8 rv;
        void *ctx;
        CsrUint8 *data;
#endif
        CsrUint32 peekOffsetEnd;

        offset = size;

        /* Read ACL header */
        while ((offset < ACL_HEADER_LENGTH) && usb_hci_running)
        {
            size = get_acl_data(data_buffer + offset, 64);
            offset += size;
        }

        /* Get ACL header */
        aclHdlFlags = data_buffer[0] + (((CsrUint16) data_buffer[1]) << 8);
        /* Get ACL length of the next block */
        length = data_buffer[2] + (((CsrUint16) data_buffer[3]) << 8);

        /* Be safe in case 'length' is smaller than USB_PEEK_BYTES */
        peekOffsetEnd = ACL_HEADER_LENGTH + CSRMIN(USB_PEEK_BYTES, length);
        /* Peek ahead */
        while ((offset < peekOffsetEnd) && usb_hci_running)
        {
            size = get_acl_data(data_buffer + offset, peekOffsetEnd - offset);
            offset += size;
        }

#ifdef CSR_ACLBUF_USB_ENABLE
        rv = CsrAclBufAlloc(aclHdlFlags, length,
            data_buffer + ACL_HEADER_LENGTH,
            USB_PEEK_BYTES,
            &ctx,
            &data);

        if (rv == CSR_ACLBUF_SUCCESS)
        {
            CsrAclBufCb aclbufcb;
            CsrUint32 payloadOffsetEnd;

            /* Move peeked data into newly allocated buffer */
            memmove(data, data_buffer + ACL_HEADER_LENGTH, offset - ACL_HEADER_LENGTH);
            offset -= ACL_HEADER_LENGTH;
            payloadOffsetEnd = length;

            /* Read in rest of the ACL packet */
            while ((offset < payloadOffsetEnd) && usb_hci_running)
            {
                size = get_acl_data(data + offset, 64);
                offset += size;
            }

            aclbufcb = CsrAclBufDeliver(aclHdlFlags, ctx, length);
            if (aclbufcb != NULL)
            {
                aclbufcb();
            }

            /* If there is more data in the buffer that we have not read,
               then copy it down and loop */
            if (offset > payloadOffsetEnd)
            {
                memmove(data_buffer, data + payloadOffsetEnd, offset - payloadOffsetEnd);
            }

            size = offset - payloadOffsetEnd;
        }
        else if (rv == CSR_ACLBUF_NONE)
#endif
        {
            lengthInclHdr = length + 4;
            /* Now we read in the body of the data, we keep going until
               we have read at least ''lengthInclHdr'' bytes. */
            while ((offset < lengthInclHdr) && usb_hci_running)
            {
                size = get_acl_data(data_buffer + offset, 64);
                offset += size;
            }

            /* send the data up the stack */
            if (usb_hci_running)
            {
                CsrUint8 *aclPacket;
                CsrBool found;

                /* Send the packet up */
                aclPacket = CsrMemDup(data_buffer, lengthInclHdr);
                /*DBG_DUMP("Rx ACL:", aclPacket, lengthInclHdr);*/

                found = FALSE;

                EnterCriticalSection(&rxMutex);

                do
                {
                    if (rxBufArray[rxBufIndexFree].buflen != 0)
                    {
                        DWORD event;

                        ResetEvent(rxWaitBuffer);

                        LeaveCriticalSection(&rxMutex);

                        event = WaitForMultipleObjects(2, bufferEvents,
                            FALSE, INFINITE);

                        switch (event)
                        {
                            case WAIT_OBJECT_0 + 0:
                            {
                                /*
                                 * Driver shut down requested.  Free
                                 * packet and go on -- thread terminates
                                 * because of usb_hci_running.
                                 */
                                CsrPmemFree(aclPacket);
                                break;
                            }

                            case WAIT_OBJECT_0 + 1:
                            {
                                EnterCriticalSection(&rxMutex);

                                if (rxBufArray[rxBufIndexFree].buflen == 0)
                                {
                                    found = TRUE;
                                }
                                break;
                            }
                        }
                    }
                    else
                    {
                        found = TRUE;
                    }
                } while ((found == FALSE) && usb_hci_running);

                rxBufArray[rxBufIndexFree].buf = aclPacket;
                rxBufArray[rxBufIndexFree].type = TRANSPORT_CHANNEL_ACL;
                rxBufArray[rxBufIndexFree].buflen = lengthInclHdr;
                rxBufIndexFree = (rxBufIndexFree + 1) % NUM_RX_BUFFERS;
                LeaveCriticalSection(&rxMutex);

                CsrSchedBgintSet(usb_bgint_rx);

                /* If there is more data in the buffer that we have not read,
                   then copy it down and loop */
                if (offset > lengthInclHdr)
                {
                    memmove(data_buffer, data_buffer + lengthInclHdr, offset - lengthInclHdr);
                }

                /* Set the size of what is left in the buffer */
                size = offset - lengthInclHdr;
            }
        }
#ifdef CSR_ACLBUF_USB_ENABLE
        else
        {
            DBG_PRINT("Could not allocate ACL buffer");
            CsrUsbDrvStop();
        }
#endif
    }

    SetEvent(thread_terminated);
    ExitThread(0);
}

#if CSR_MAX_SCO_CONNECTIONS > 0
/*----------------------------------------------------------------------------*
 * NAME
 *    get_sco_data (internal fct.)
 *
 * DESCRIPTION
 *    Performs the actual read of SCO data from the USB driver.
 *
 *
 * RETURNS
 *    The number of bytes read.
 *
 *******************************************************************************/
static CsrUint32 get_sco_data(void *buffer, CsrUint32 length)
{
    CsrUint32 status;
    CsrUint32 read = 0;
    static OVERLAPPED overlapped = {0, 0, 0, 0, 0};
    HANDLE handles[2] = {wait_for_sco, shutdown_event};

    overlapped.hEvent = wait_for_sco;

    status = DeviceIoControl(devHandle,
        IOCTL_CSRBC_RECV_SCO_DATA,
        0,
        0,
        buffer,
        length,
        &read,
        &overlapped);

    /* its possible the request has not completed yet so check if it is in progress */
    if ((!status) && (GetLastError() == ERROR_IO_PENDING))
    {
        /* yes, it's in progress */
        if (WaitForMultipleObjects(2, handles, FALSE, INFINITE) == WAIT_OBJECT_0)
        {
            /* wait_for_sco event */
            status = GetOverlappedResult(devHandle, &overlapped, &read, TRUE);
        }
        else
        {
            /* we are probably shutting down */
            read = 0;
        }
    }
    else if (!status)
    {
        DBG_PRINT("Receive SCO failed");
        CsrUsbDrvStop();
    }

    return read;
}

/*----------------------------------------------------------------------------*
 * NAME
 *     rxScoThread (internal fct.)
 *
 * DESCRIPTION
 *
 *   Thread that reads SCO data from the USB driver. The data is entered into
 *    a buffer protected by mutex. The scheduler is triggered by a background
 *    interrupt to take over the data buffered..
 *
 * RETURNS
 *    -
 *
 *******************************************************************************/
static void rxScoThread(void)
{
    CsrUint32 offset, length, index;
    CsrBool alldone;
    CsrUint32 size = 0;

    /* ''offset'' is the number of bytes in the buffer (always) */
    /* ''length'' is the length of the next event */
    /* ''size''   is the length of the last read from the driver*/

    thread_count++;

    while (usb_hci_running)
    {
        Sleep(1);

        /* do nothing unless a calling app is interested in the data and an alternate interface
           has been selected */
        if (sco_packet_size == 0)
        {
            Sleep(0);
            continue;
        }

        /* start position for writing data to the buffer */
        offset = size;

        /* keep requesting data until we have at least got a sco header */
        while ((offset < 3) && usb_hci_running)
        {
            size = get_sco_data(sco_buffer + offset, 65536 - offset);
            offset += size;
            if (size == 0)
            {
                Sleep(0);
                continue;
            }
        }

        /* We now know the length of the current packet and it's header */
        length = ((CsrUint8) * (sco_buffer + 2)) + 3;

        /* keep requesting data until we have got at least one complete sco primitive */
        while ((offset < length) && usb_hci_running)
        {
            size = get_sco_data(sco_buffer + offset, 65536 - offset);
            offset += size;
        }

        /* process the primitive plus any other primitives we may have been given */
        if (usb_hci_running)
        {
            /* go through the data we have been given looking for complete packets */
            for (index = 0, alldone = FALSE; (index < offset) && (alldone == FALSE); )
            {
                /* send the complete packet up the stack */
                CsrUint8 *scoPacket = (CsrUint8 *) CsrPmemAlloc(length);
                memcpy(scoPacket, sco_buffer + index, length);
                /*DBG_DUMP("Rx SCO:", scoPacket, length);*/

                EnterCriticalSection(&rxMutex);
                if (rxBufArray[rxBufIndexFree].buflen == 0)
                {
                    rxBufArray[rxBufIndexFree].buf = scoPacket;
                    rxBufArray[rxBufIndexFree].type = TRANSPORT_CHANNEL_SCO;
                    rxBufArray[rxBufIndexFree].buflen = (CsrUint32) length;
                    rxBufIndexFree = (rxBufIndexFree + 1) % NUM_RX_BUFFERS;
                    LeaveCriticalSection(&rxMutex);

                    CsrSchedBgintSet(usb_bgint_rx);
                }
                else
                {
                    LeaveCriticalSection(&rxMutex);
                    DBG_PRINT("rxScoThread(): Rx buffer overrun");
                    CsrPmemFree(scoPacket);
                    Sleep(0);
                    continue;
                }

                /* index to the start of the next protential packet */
                index += length;

                /* are there enough bytes in the buffer to contain the next sco header? */
                if (((index + 3) < offset) &&
                    /* and are there enough bytes to contain the next sco payload? */
                    ((index + (*(sco_buffer + index + 2)) + 3) <= offset))
                {
                    /* yes, so get the length of the payload and header */
                    length = (*(sco_buffer + index + 2)) + 3;
                }
                else
                {
                    /* no more packets so exit out of loop */
                    alldone = TRUE;
                }
            }

            /* if there is a paritally complete packet remaining, copy it to the buffer start */
            if (index < offset)
            {
                memmove(sco_buffer, sco_buffer + index, offset - index);
            }

            /* update the size of valid data in the buffer */
            size = offset - index;
        }
    }

    SetEvent(thread_terminated);
    ExitThread(0);
}

/*----------------------------------------------------------------------------*
 * NAME
 *     start_sco (internal fct.)
 *
 * DESCRIPTION
 *
 *   Starts up SCO data, creates SCO rx thread
 *
 * RETURNS
 *    -
 *
 *******************************************************************************/
static void start_sco(CsrUint8 alternate_setting)
{
    static OVERLAPPED overlapped = {0, 0, 0, 0, 0};
    CsrUint32 written;
    DWORD thread_id;
    CsrUint8 buf[2];

    /* only start monitoring the sco pipe if sco is enabled */
    if (rxScoThreadHandle == INVALID_HANDLE_VALUE)
    {
        /* set the alternate SCO bandwidth */
        buf[0] = alternate_setting;

        Sleep(500);

        overlapped.hEvent = wait_for_start_sco;

        if (!DeviceIoControl(devHandle, IOCTL_CSRBC_START_SCO_DATA, buf, 1, 0, 0, &written, &overlapped))
        {
            DBG_PRINT("Failed to issue start SCO");

            if (GetLastError() == ERROR_IO_PENDING)
            {
                GetOverlappedResult(devHandle, &overlapped, &written, TRUE);
            }
        }

        rxScoThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) rxScoThread, NULL, 0, &thread_id);
    }
}

/*----------------------------------------------------------------------------*
 * NAME
 *     stop_sco (internal fct.)
 *
 * DESCRIPTION
 *
 *   Stops SCO data
 *
 * RETURNS
 *    -
 *
 *******************************************************************************/
static void stop_sco(void)
{
    static OVERLAPPED overlapped = {0, 0, 0, 0, 0};
    CsrUint8 buf[2];
    CsrUint32 written;

    /* only stop SCO if started */
    if (rxScoThreadHandle != INVALID_HANDLE_VALUE)
    {
        overlapped.hEvent = wait_for_stop_sco;
        buf[0] = 1;

        if (!DeviceIoControl(devHandle, IOCTL_CSRBC_STOP_SCO_DATA, buf, 1, 0, 0, &written, &overlapped))
        {
            DBG_PRINT("Failed to issue stop SCO");

            if (GetLastError() == ERROR_IO_PENDING)
            {
                GetOverlappedResult(devHandle, &overlapped, &written, TRUE);
            }
        }
    }
}

#endif

/*----------------------------------------------------------------------------*
 * NAME
 *     txThread (internal fct.)
 *
 * DESCRIPTION
 *
 *   Thread that passes HCI cmds, ACL and SCO data to the USB driver. The data
 *    is taken from buffers protected by mutex's for each data type. The function
 *    gets its triggers by events sent from CsrUsbDrvTx().
 *
 * RETURNS
 *    -
 *
 *******************************************************************************/
void txThread(void)
{
    OVERLAPPED txAclDone = {0};
    OVERLAPPED txHciDone = {0};
    OVERLAPPED txScoDone = {0};
    DWORD written;
    DWORD event;
    CsrBool txAclBusy, txHciBusy, txScoBusy;
    CsrUint32 dataWaiting;

    txAclDone.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    txHciDone.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    txScoDone.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    if ((txAclDone.hEvent == NULL) || (txHciDone.hEvent == NULL) || (txScoDone.hEvent == NULL))
    {
        DBG_PRINT("Create event failed in tx thread");
        exit(0);
    }

    txEvents[0] = txAclDone.hEvent;
    txEvents[1] = send_data;
    txEvents[2] = txHciDone.hEvent;
    txEvents[3] = send_cmd;
#if CSR_MAX_SCO_CONNECTIONS > 0
    txEvents[4] = txScoDone.hEvent;
    txEvents[5] = send_sco;
    txEvents[6] = shutdown_event;
#else
    txEvents[4] = shutdown_event;
#endif

    txAclBusy = FALSE;
    txHciBusy = FALSE;
    txScoBusy = FALSE;

    thread_count++;

    while (TRUE)
    {
        dataWaiting = 0;

        event = WaitForMultipleObjects(MAX_USB_TX_EVENTS, txEvents, FALSE, INFINITE);
        switch (event)
        {
            case WAIT_OBJECT_0: /* txAclDoneEvent */
            {
                BOOL success;

                if (txAclBusy)
                {
                    txAclBusy = FALSE;
                    success = GetOverlappedResult(devHandle, &txAclDone, &written, FALSE);
                    if (success)
                    {
                        EnterCriticalSection(&txAclMutex);

                        if (written != aclTxBufArray[aclTxBufIndexCurrent].buflen)
                        {
                            DBG_PRINT("Sending of ACL data incomplete");
                        }

                        CsrPmemFree(aclTxBufArray[aclTxBufIndexCurrent].buf);
                        aclTxBufArray[aclTxBufIndexCurrent].buf = NULL;
                        aclTxBufArray[aclTxBufIndexCurrent].buflen = 0;
                        aclTxBufIndexCurrent = (aclTxBufIndexCurrent + 1) % NUM_ACL_BUFFERS;

                        dataWaiting = aclTxBufArray[aclTxBufIndexCurrent].buflen;
                        LeaveCriticalSection(&txAclMutex);
                    }
                    else
                    {
                        DBG_PRINT("Serious error sending ACL data");
                    }

                    if (dataWaiting != 0)
                    {
                        /* data is waiting to be sent, lets send an event to ourself */
                        SetEvent(send_data);
                    }
                }
                break;
            }

            case WAIT_OBJECT_0 + 1: /* send_data */
            {
                /* do not try to send while busy */
                if (!txAclBusy)
                {
                    CsrUint32 size;

                    EnterCriticalSection(&txAclMutex);
                    size = aclTxBufArray[aclTxBufIndexCurrent].buflen;
                    LeaveCriticalSection(&txAclMutex);

                    if (size == 0)
                    {
                        /* no data so take another loop */
                        break;
                    }

                    /*DBG_DUMP("Tx ACL:", aclTxBufArray[aclTxBufIndexCurrent].buf, size);*/

                    if (WriteFile(devHandle, aclTxBufArray[aclTxBufIndexCurrent].buf, size, &written, &txAclDone))
                    {
                        if (written != size)
                        {
                            DBG_PRINT("Sending of ACL data incomplete");
                        }

                        EnterCriticalSection(&txAclMutex);

                        CsrPmemFree(aclTxBufArray[aclTxBufIndexCurrent].buf);
                        aclTxBufArray[aclTxBufIndexCurrent].buf = NULL;
                        aclTxBufArray[aclTxBufIndexCurrent].buflen = 0;
                        aclTxBufIndexCurrent = (aclTxBufIndexCurrent + 1) % NUM_ACL_BUFFERS;

                        dataWaiting = aclTxBufArray[aclTxBufIndexCurrent].buflen;
                        LeaveCriticalSection(&txAclMutex);

                        if (dataWaiting != 0)
                        {
                            /* data is waiting to be sent, lets send an event to ourself */
                            SetEvent(send_data);
                        }
                    }
                    else
                    {
                        if (GetLastError() != ERROR_IO_PENDING)
                        {
                            DBG_PRINT("Serious error in write file (ACL data) in tx thread");
                            break;
                        }
                        else
                        {
                            /* write ACL pending */
                            txAclBusy = TRUE;
                        }
                    }
                }
                break;
            }

            case WAIT_OBJECT_0 + 2: /* txHciDoneEvent */
            {
                BOOL success;

                if (txHciBusy)
                {
                    txHciBusy = FALSE;

                    success = GetOverlappedResult(devHandle, &txHciDone, &written, FALSE);
                    if (success)
                    {
                        EnterCriticalSection(&txHciMutex);

                        if (written != hciTxBufArray[hciTxBufIndexCurrent].buflen)
                        {
                            DBG_PRINT("Sending of HCI data incomplete");
                        }

                        CsrPmemFree(hciTxBufArray[hciTxBufIndexCurrent].buf);
                        hciTxBufArray[hciTxBufIndexCurrent].buf = NULL;
                        hciTxBufArray[hciTxBufIndexCurrent].buflen = 0;
                        hciTxBufIndexCurrent = (hciTxBufIndexCurrent + 1) % NUM_HCI_BUFFERS;

                        dataWaiting = hciTxBufArray[hciTxBufIndexCurrent].buflen;
                        LeaveCriticalSection(&txHciMutex);
                    }
                    else
                    {
                        DBG_PRINT("Serious error sending HCI data");
                    }

                    if (dataWaiting != 0)
                    {
                        /* data is waiting to be sent, lets send an event to ourself */
                        SetEvent(send_cmd);
                    }
                }
                break;
            }

            case WAIT_OBJECT_0 + 3: /* send_cmd */
            {
                /* do not try to send while busy */
                if (!txHciBusy)
                {
                    CsrUint32 size;
                    DWORD err;

                    EnterCriticalSection(&txHciMutex);
                    size = hciTxBufArray[hciTxBufIndexCurrent].buflen;
                    LeaveCriticalSection(&txHciMutex);

                    if (size == 0)
                    {
                        /* no data so take another loop */
                        break;
                    }
                    /*DBG_DUMP("Tx HCI:", hciTxBufArray[hciTxBufIndexCurrent].buf, size);*/
                    if (DeviceIoControl(devHandle,
                            IOCTL_CSRBC_SEND_HCI_COMMAND,
                            hciTxBufArray[hciTxBufIndexCurrent].buf,
                            size,
                            0,
                            0,
                            &written,
                            &txHciDone))
                    {
                        EnterCriticalSection(&txHciMutex);
                        CsrPmemFree(hciTxBufArray[hciTxBufIndexCurrent].buf);
                        hciTxBufArray[hciTxBufIndexCurrent].buf = NULL;
                        hciTxBufArray[hciTxBufIndexCurrent].buflen = 0;
                        hciTxBufIndexCurrent = (hciTxBufIndexCurrent + 1) % NUM_HCI_BUFFERS;

                        dataWaiting = hciTxBufArray[hciTxBufIndexCurrent].buflen;

                        LeaveCriticalSection(&txHciMutex);

                        if (dataWaiting != 0)
                        {
                            /* data is waiting to be sent, lets send an event to ourself */
                            SetEvent(send_cmd);
                        }
                    }
                    else
                    {
                        err = GetLastError();
                        if (err != ERROR_IO_PENDING)
                        {
                            DBG_PRINT_INT("Serious error in DeviceIoControl (HCI cmd) in tx thread", err);
                            break;
                        }
                        else
                        {
                            /* write HCI cmd pending */
                            txHciBusy = TRUE;
                        }
                    }
                }
                break;
            }

#if CSR_MAX_SCO_CONNECTIONS > 0
            case WAIT_OBJECT_0 + 4: /* txScoDoneEvent */
            {
                BOOL success;

                if (txScoBusy)
                {
                    txScoBusy = FALSE;
                    dataWaiting = 0;

                    success = GetOverlappedResult(devHandle, &txScoDone, &written, FALSE);
                    if (success)
                    {
                        EnterCriticalSection(&txScoMutex);

                        if (written != scoTxBufArray[scoTxBufIndexCurrent].buflen)
                        {
                            DBG_PRINT("Sending of SCO data incomplete");
                        }


                        CsrPmemFree(scoTxBufArray[scoTxBufIndexCurrent].buf);
                        scoTxBufArray[scoTxBufIndexCurrent].buf = NULL;
                        scoTxBufArray[scoTxBufIndexCurrent].buflen = 0;

                        scoTxBufIndexCurrent = (scoTxBufIndexCurrent + 1) % NUM_SCO_BUFFERS;
                        dataWaiting = scoTxBufArray[scoTxBufIndexCurrent].buflen;
                        LeaveCriticalSection(&txScoMutex);
                    }
                    else
                    {
                        DBG_PRINT("Serious error sending SCO data");
                    }

                    if (dataWaiting != 0)
                    {
                        /* data is waiting to be sent, lets send an event to ourself */
                        SetEvent(send_sco);
                    }
                }
                break;
            }

            case WAIT_OBJECT_0 + 5: /* send_sco */
            {
                /* do not try to send while busy */
                if (!txScoBusy)
                {
                    CsrUint32 size;

                    EnterCriticalSection(&txScoMutex);
                    size = scoTxBufArray[scoTxBufIndexCurrent].buflen;
                    LeaveCriticalSection(&txScoMutex);

                    if (size == 0)
                    {
                        /* no data so take another loop */
                        break;
                    }

                    /*DBG_DUMP("Tx SCO:", scoTxBufArray[scoTxBufIndexCurrent].buf, size);*/
                    if (DeviceIoControl(devHandle,
                            IOCTL_CSRBC_SEND_SCO_DATA,
                            scoTxBufArray[scoTxBufIndexCurrent].buf,
                            size,
                            0,
                            0,
                            &written,
                            &txScoDone))
                    {
                        EnterCriticalSection(&txScoMutex);
                        CsrPmemFree(scoTxBufArray[scoTxBufIndexCurrent].buf);
                        scoTxBufArray[scoTxBufIndexCurrent].buf = NULL;
                        scoTxBufArray[scoTxBufIndexCurrent].buflen = 0;
                        LeaveCriticalSection(&txScoMutex);

                        scoTxBufIndexCurrent = (scoTxBufIndexCurrent + 1) % NUM_SCO_BUFFERS;
                    }
                    else
                    {
                        if (GetLastError() != ERROR_IO_PENDING)
                        {
                            DBG_PRINT("Serious error in DeviceIoControl (SCO data) in tx thread");
                            break;
                        }
                        else
                        {
                            /* write SCO data pending */
                            txScoBusy = TRUE;
                        }
                    }
                }
                break;
            }

            case WAIT_OBJECT_0 + 6: /* shutdown_event */
#else
            case WAIT_OBJECT_0 + 4: /* shutdown_event */
#endif
                {
                    /* terminating thread */
                    CloseHandle(txAclDone.hEvent);
                    CloseHandle(txHciDone.hEvent);
                    CloseHandle(txScoDone.hEvent);
                    SetEvent(thread_terminated);
                    ExitThread(0);
                    break;
                }

            default:
            {
                /* error occurred */
                DBG_PRINT("Unknown event received by USB tx thread");
                break;
            }
        }    /* end switch */
#if CSR_MAX_SCO_CONNECTIONS > 0
        if (event == WAIT_OBJECT_0 + 6) /* shutdown_event */
#else
        if (event == WAIT_OBJECT_0 + 4) /* shutdown_event */
#endif
        {
            break;
        }
    }    /* end while */
}

/*----------------------------------------------------------------------------*
 * NAME
 *     CsrUsbDrvTx
 *
 * DESCRIPTION
 *
 *   Called to send data (HCI, ACL or SCO) to the BlueCore via USB
 *
 * RETURNS
 *    Return TRUE if data can be buffered for transmission, otherwise FALSE
 *
 *******************************************************************************/
CsrBool CsrUsbDrvTx(CsrUint8 type, CsrUint8 *data, CsrUint32 size)
{
    if (size == 0)
    {
        DBG_PRINT("Tx packet size = 0");
        return FALSE;
    }

    if (type == TRANSPORT_CHANNEL_ACL)
    {
        EnterCriticalSection(&txAclMutex);

        if (aclTxBufArray[aclTxBufIndexFree].buflen == 0)
        {
            aclTxBufArray[aclTxBufIndexFree].buflen = size;
            aclTxBufArray[aclTxBufIndexFree].buf = data;
            LeaveCriticalSection(&txAclMutex);

            aclTxBufIndexFree = (aclTxBufIndexFree + 1) % NUM_ACL_BUFFERS;

            /* signal tx thread that ACL data has arrived */
            SetEvent(send_data);
        }
        else
        {
            LeaveCriticalSection(&txAclMutex);
            DBG_PRINT("ACL buffer is full, discarding packet!");
            CsrPmemFree(data);
            return FALSE;
        }
    }
    else if (type == TRANSPORT_CHANNEL_HCI)
    {
        EnterCriticalSection(&txHciMutex);

        if (hciTxBufArray[hciTxBufIndexFree].buflen == 0)
        {
            hciTxBufArray[hciTxBufIndexFree].buflen = size;
            hciTxBufArray[hciTxBufIndexFree].buf = data;
            LeaveCriticalSection(&txHciMutex);

            hciTxBufIndexFree = (hciTxBufIndexFree + 1) % NUM_HCI_BUFFERS;

            /* signal tx thread that HCI cmd has arrived */
            SetEvent(send_cmd);
        }
        else
        {
            LeaveCriticalSection(&txHciMutex);
            DBG_PRINT("HCI buffer is full, discarding packet!");
            CsrPmemFree(data);
            return FALSE;
        }
    }
    else /* must be TRANSPORT_CHANNEL_SCO */
    {
#if CSR_MAX_SCO_CONNECTIONS > 0
        EnterCriticalSection(&txScoMutex);

        if (scoTxBufArray[scoTxBufIndexFree].buflen == 0)
        {
            scoTxBufArray[scoTxBufIndexFree].buflen = size;
            scoTxBufArray[scoTxBufIndexFree].buf = data;
            LeaveCriticalSection(&txScoMutex);

            scoTxBufIndexFree = (scoTxBufIndexFree + 1) % NUM_SCO_BUFFERS;

            /* signal tx thread that SCO data has arrived */
            SetEvent(send_sco);
        }
        else
        {
            LeaveCriticalSection(&txScoMutex);
            DBG_PRINT("SCO buffer is full, discarding packet!");
            CsrPmemFree(data);
            return FALSE;
        }
#endif
    }

    return TRUE;
}

/*----------------------------------------------------------------------------*
 * NAME
 *     CsrUsbDrvRx
 *
 * DESCRIPTION
 *
 *   This function is called by a background interrupt each time a packet
 *    (ACL, HCI or SCO) is received from the USB device.
 *
 * RETURNS
 *    -
 *
 *******************************************************************************/
void CsrUsbDrvRx(void *unused)
{
    MessageStructure hciMsg;
    CsrUint32 dataWaiting;

    EnterCriticalSection(&rxMutex);

    if (rxBufArray[rxBufIndexCurrent].buflen != 0)
    {
        hciMsg.buf = rxBufArray[rxBufIndexCurrent].buf;
        hciMsg.buflen = rxBufArray[rxBufIndexCurrent].buflen;
        hciMsg.chan = rxBufArray[rxBufIndexCurrent].type;

        rxBufArray[rxBufIndexCurrent].buflen = 0;
        rxBufArray[rxBufIndexCurrent].buf = NULL;

        rxBufIndexCurrent = (rxBufIndexCurrent + 1) % NUM_RX_BUFFERS;
        dataWaiting = rxBufArray[rxBufIndexCurrent].buflen;
        SetEvent(rxWaitBuffer);
        LeaveCriticalSection(&rxMutex);

        /* pass it to higher layers */
        CsrTmBlueCoreUsbDeliverMsg(&hciMsg, 0);

        if (dataWaiting != 0)
        {
            /* data waiting, have the scheduler assign one more run to us */
            CsrSchedBgintSet(usb_bgint_rx);
        }
    }
    else
    {
        LeaveCriticalSection(&rxMutex);
    }
}

/*----------------------------------------------------------------------------*
 * NAME
 *     CsrUsbDrvStart
 *
 * DESCRIPTION
 *
 *   This function is called to open the USB device specified in
 *   CsrUsbDrvConfigure. If none is specified the default \\.\CSR0
 *   device is opened.
 *
 * PARAMETERS
 *   reset: Number of previous resets
 *
 * RETURNS
 *    TRUE: the device was opened successfully, otherwise FALSE
 *
 *******************************************************************************/
CsrBool CsrUsbDrvStart(CsrUint8 reset)
{
    DWORD threadId, err;
    CsrUint8 i;
#if CSR_MAX_SCO_CONNECTIONS > 0
    CsrUint8 alternate_setting;
#endif
    CsrUint8 BcCmdReset[22] =
    {
        0x00, 0xFC, 0x13, 0xC2,
        0x02, 0x00, 0x09, 0x00, 0x00, 0x00, 0x02, 0x40, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    CSR_UNUSED(reset);

    if (usb_hci_running == TRUE)
    {
        /* already started!!! */
        return TRUE;
    }

    /* Try opening the device */
    if (strlen(devName) > 0)
    {
        CsrUint8 j;

        for (j = 0;
             j < USB_INIT_MAX_RETRIES;
             j++)
        {
            /* Try opening the device */
            devHandle = CreateFile(devName,
                GENERIC_READ | GENERIC_WRITE,
                0,
                0,
                OPEN_EXISTING,
                FILE_FLAG_OVERLAPPED,
                0);

            if (devHandle != INVALID_HANDLE_VALUE)
            {
                break;
            }
            else
            {
                Sleep(j * 128);
            }
        }
    }
    else
    {
        CsrUint8 j;

        for (j = 0;
             j < USB_INIT_MAX_RETRIES;
             j++)
        {
            sprintf(devName, "%s%u", USB_DRIVER_NAME, j);

            devHandle = CreateFile(devName,
                GENERIC_READ | GENERIC_WRITE,
                0,
                0,
                OPEN_EXISTING,
                FILE_FLAG_OVERLAPPED,
                0);

            if (devHandle != INVALID_HANDLE_VALUE)
            {
                break;
            }
        }
    }

    if (devHandle == INVALID_HANDLE_VALUE)
    {
        DBG_PRINT_INT("Open USB device failure, error code:", GetLastError());

        return FALSE;
    }

    InitializeCriticalSection(&txHciMutex);
    InitializeCriticalSection(&txAclMutex);
#if CSR_MAX_SCO_CONNECTIONS > 0
    InitializeCriticalSection(&txScoMutex);
#endif
    InitializeCriticalSection(&rxMutex);
    InitializeCriticalSection(&closeMutex);

    /* create internal events for thread communication */
    send_data = CreateEvent(NULL, FALSE, FALSE, NULL);
    send_cmd = CreateEvent(NULL, FALSE, FALSE, NULL);
    shutdown_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    thread_terminated = CreateEvent(NULL, FALSE, FALSE, NULL);
    wait_for_event = CreateEvent(NULL, FALSE, FALSE, NULL);
    wait_for_data = CreateEvent(NULL, FALSE, FALSE, NULL);
    rxWaitBuffer = CreateEvent(NULL, FALSE, FALSE, NULL);

#if CSR_MAX_SCO_CONNECTIONS > 0
    send_sco = CreateEvent(NULL, FALSE, FALSE, NULL);
    wait_for_sco = CreateEvent(NULL, FALSE, FALSE, NULL);
    wait_for_start_sco = CreateEvent(NULL, FALSE, FALSE, NULL);
    wait_for_stop_sco = CreateEvent(NULL, FALSE, FALSE, NULL);
#endif
#if CSR_MAX_SCO_CONNECTIONS > 0
    if ((send_data == NULL) || (send_cmd == NULL)
        || (shutdown_event == NULL) || (wait_for_event == NULL)
        || (send_sco == NULL) || (wait_for_sco == NULL)
        || (wait_for_start_sco == NULL)
        || (wait_for_data == NULL)
        || (thread_terminated == NULL)
        || (rxWaitBuffer == NULL))
#else
    if ((send_data == NULL) || (send_cmd == NULL)
        || (shutdown_event == NULL) || (wait_for_event == NULL)
        || (wait_for_data == NULL)
        || (thread_terminated == NULL))
#endif
    {
        DBG_PRINT("Create events failure in CsrUsbDrvStart");
        return FALSE;
    }

    /* initialize buffers and administration */
    rxBufIndexCurrent = 0;
    rxBufIndexFree = 0;
    aclTxBufIndexCurrent = 0;
    aclTxBufIndexFree = 0;
    hciTxBufIndexCurrent = 0;
    hciTxBufIndexFree = 0;
#if CSR_MAX_SCO_CONNECTIONS > 0
    scoTxBufIndexCurrent = 0;
    scoTxBufIndexFree = 0;
#endif
    for (i = 0; i < NUM_ACL_BUFFERS; i++)
    {
        aclTxBufArray[i].buflen = 0;
    }
    for (i = 0; i < NUM_HCI_BUFFERS; i++)
    {
        hciTxBufArray[i].buflen = 0;
    }
#if CSR_MAX_SCO_CONNECTIONS > 0
    for (i = 0; i < NUM_SCO_BUFFERS; i++)
    {
        scoTxBufArray[i].buflen = 0;
    }
#endif
    for (i = 0; i < NUM_RX_BUFFERS; i++)
    {
        rxBufArray[i].buflen = 0;
    }

    usb_bgint_rx = CsrSchedBgintReg(CsrUsbDrvRx, NULL, "CsrUsbDrvRx"); /* the handler for rx data */

#ifdef CSR_ENABLE_WARM_RESET // Sean added this definition for non-warm-reset version 
    /* initiate a warm reset of the BlueCore chip */
    DeviceIoControl(devHandle, IOCTL_CSRBC_SEND_HCI_COMMAND, BcCmdReset, sizeof(BcCmdReset), 0, 0, &err, NULL);
#else
    err=0;
#endif
    /* close the device handle, driver gets unloaded when chip resets */
    CloseHandle(devHandle);

    /* allow the chip to boot and the USB driver to be re-loaded, then re-connect... */
    Sleep(500);
    while (INVALID_HANDLE_VALUE == (devHandle = CreateFile(devName,
                                        GENERIC_READ | GENERIC_WRITE,
                                        0,
                                        0,
                                        OPEN_EXISTING,
                                        FILE_FLAG_OVERLAPPED,
                                        0
                                        )))
    {
        Sleep(100);
    }

    /* set 'running' flag and fire up the rx/tx threads */
    usb_hci_running = TRUE;

    thread_count = 0;

    txThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) txThread, NULL, 0, &threadId);
    rxAclThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) rxAclThread, NULL, 0, &threadId);
    rxHciThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) rxHciThread, NULL, 0, &threadId);
#if CSR_MAX_SCO_CONNECTIONS > 0
    rxScoThreadHandle = INVALID_HANDLE_VALUE;
#endif
    if ((txThreadHandle == INVALID_HANDLE_VALUE)
        || (rxAclThreadHandle == INVALID_HANDLE_VALUE)
        || (rxHciThreadHandle == INVALID_HANDLE_VALUE))
    {
        DBG_PRINT("USB threads create failure");
        return FALSE;
    }

#if CSR_MAX_SCO_CONNECTIONS > 0
    if ((VOICE_SETTING & INPUTSAMPLESIZE_16BIT) == INPUTSAMPLESIZE_16BIT)
    {
        sco_packet_size = (CSR_MAX_SCO_CONNECTIONS << 4) + 1;
        alternate_setting = CSR_MAX_SCO_CONNECTIONS << 1;
    }
    else
    {
        sco_packet_size = (CSR_MAX_SCO_CONNECTIONS << 3) + 1;
        alternate_setting = CSR_MAX_SCO_CONNECTIONS;
    }

    if (alternate_setting > 5)
    {
        alternate_setting = 5;
    }

    /* alternate SCO setting
       1: One 8-bit voice channel
       2: Two 8-bit voice channels or one 16-bit channel
       3: Three 8-bit voice channels
       4: Two 16-bit voice channels
       5: Three 16-bit voice channels
 */
    /*start_sco(alternate_setting); */
#endif

    return TRUE;
}

/*----------------------------------------------------------------------------*
 * NAME
 *     CsrUsbDrvStop
 *
 * DESCRIPTION
 *
 *   This function is called to free module resources and close the USB device.
 *
 * RETURNS
 *    -
 *
 *******************************************************************************/
CsrBool CsrUsbDrvStop(void)
{
    CsrUint8 i;
    DWORD written;

    if (usb_hci_running)
    {
#if CSR_MAX_SCO_CONNECTIONS > 0
        stop_sco();
#endif

        usb_hci_running = FALSE;

        /* stop all threads one by one */
        SetEvent(shutdown_event);
        while (WaitForSingleObject(thread_terminated, INFINITE) == WAIT_OBJECT_0)
        {
            thread_count--;

            if (thread_count > 0)
            {
                SetEvent(shutdown_event);
            }
            else
            {
                break;
            }
        }

        EnterCriticalSection(&closeMutex);
        CloseHandle(send_data);
        CloseHandle(send_cmd);
        CloseHandle(shutdown_event);
        CloseHandle(thread_terminated);
        CloseHandle(wait_for_event);
        CloseHandle(wait_for_data);
#if CSR_MAX_SCO_CONNECTIONS > 0
        CloseHandle(send_sco);
        CloseHandle(wait_for_sco);
        CloseHandle(wait_for_start_sco);
#endif
        CloseHandle(rxWaitBuffer);
        CloseHandle(rxAclThreadHandle);
        CloseHandle(rxHciThreadHandle);
        CloseHandle(txThreadHandle);

#if CSR_MAX_SCO_CONNECTIONS > 0
        CloseHandle(rxScoThreadHandle);
        rxScoThreadHandle = INVALID_HANDLE_VALUE;
#endif

        if (!DeviceIoControl(devHandle, IOCTL_CSRBC_RESET_DEVICE, 0, 0, 0, 0, &written, 0))
        {
            DBG_PRINT("USB device reset failure");
        }

        CloseHandle(devHandle);
        devHandle = INVALID_HANDLE_VALUE;

        CsrSchedBgintUnreg(usb_bgint_rx);
        usb_bgint_rx = CSR_SCHED_BGINT_INVALID;

        LeaveCriticalSection(&closeMutex);

        DeleteCriticalSection(&txAclMutex);
        DeleteCriticalSection(&txHciMutex);
#if CSR_MAX_SCO_CONNECTIONS > 0
        DeleteCriticalSection(&txScoMutex);
#endif
        DeleteCriticalSection(&rxMutex);
        DeleteCriticalSection(&closeMutex);

        /* make sure that buffers are empty */
        for (i = 0; i < NUM_ACL_BUFFERS; i++)
        {
            if (aclTxBufArray[i].buflen != 0)
            {
                CsrPmemFree(aclTxBufArray[i].buf);
            }
        }
        for (i = 0; i < NUM_HCI_BUFFERS; i++)
        {
            if (hciTxBufArray[i].buflen != 0)
            {
                CsrPmemFree(hciTxBufArray[i].buf);
            }
        }
#if CSR_MAX_SCO_CONNECTIONS > 0
        for (i = 0; i < NUM_SCO_BUFFERS; i++)
        {
            if (scoTxBufArray[i].buflen != 0)
            {
                CsrPmemFree(scoTxBufArray[i].buf);
            }
        }
#endif
        for (i = 0; i < NUM_RX_BUFFERS; i++)
        {
            if (rxBufArray[i].buflen != 0)
            {
                CsrPmemFree(rxBufArray[i].buf);
            }
        }
    }

    return TRUE;
}

void CsrUsbDrvConfigure(const char *device)
{
    if (device != NULL)
    {
        strcpy(devName, device);
    }
}
