/*****************************************************************************

            (c) Cambridge Silicon Radio Limited 2010
            All rights reserved and confidential information of CSR

            Refer to LICENSE.txt included with this source for details
            on the license terms.

*****************************************************************************/

#ifdef CSR_UART_DEBUG
#define DPRINTF(x) printf x
#else
#define DPRINTF(x)
#endif

#include <stdarg.h>
#define _BSD_SOURCE /* for CRTSCTS */
#include <termios.h>

#include <poll.h>
#include <sys/ioctl.h>

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#include "csr_types.h"
#include "csr_sched.h"
#include "csr_macro.h"
#include "csr_serial_com.h"
#include "csr_transport.h"
#include "csr_util.h"

#include "platform/csr_serial_init.h"

#define QUEUE_SIZE             (9000)

static CsrUint8 UART0_RX_QUEUE[QUEUE_SIZE];
static CsrUint32 UART0_RX_in_PTR;
static CsrUint32 UART0_RX_out_PTR;
static CsrUint32 UART0_RX_numbers;

static pthread_mutex_t receiveMutex;
static pthread_t worker_thread;

/* Designates the reader and writer fds in a pipe(2) array. */
#define PIPE_READ   0
#define PIPE_WRITE  1

/* This maps the file descriptors into the pollfd struture. */
#define FD_UART     0
#define FD_CONTROL  1

static struct pollfd pfd[2];
static int control;

static CsrBool instInUse;
static CsrCharString instDevice[128];
static CsrUint32 curBaud;
static CsrUint32 instBaud;
static CsrUint32 instResetBaud;
static CsrUartDrvDataBits instDataBits;
static CsrUartDrvParity instParity;
static CsrUartDrvStopBits instStopBits;
static CsrBool instFlowControl;
static CsrUint8 instToken;
static CsrBool instUseToken;

static CsrSchedBgint rxBgint = CSR_SCHED_BGINT_INVALID;
static CsrUartDrvDataRx rxData = NULL;

static speed_t baudrate_map(CsrUint32 b)
{
    speed_t retval;

	if(b > 900000 && b < 1000000)	
		b = 921600;

	if(b> 500000 && b < 600000)
		b =  576000;

    switch (b)
    {
        case 110:
            retval = B110;
            break;

        case 300:
            retval = B300;
            break;

        case 1200:
            retval = B1200;
            break;

        case 2400:
            retval = B2400;
            break;

        case 4800:
            retval = B4800;
            break;

        case 9600:
            retval = B9600;
            break;

        case 19200:
            retval = B19200;
            break;

        case 38400:
            retval = B38400;
            break;

        case 57600:
            retval = B57600;
            break;

        case 115200:
            retval = B115200;
            break;

#ifdef B230400
        case 230400:
            retval = B230400;
            break;
#endif

#ifdef B460800
        case 460800:
            retval = B460800;
            break;
#endif

#ifdef B500000
        case 500000:
            retval = B500000;
            break;
#endif

#ifdef B576000
        case 576000:
            retval = B576000;
            break;
#endif

#ifdef B921600
        case 921600:
            retval = B921600;
            break;
#endif

#ifdef B1000000
        case 1000000:
            retval = B1000000;
            break;
#endif

#ifdef B1152000
        case 1152000:
            retval = B1152000;
            break;
#endif

#ifdef B1500000
        case 1500000:
            retval = B1500000;
            break;
#endif

#ifdef B2000000
        case 2000000:
            retval = B2000000;
            break;
#endif

#ifdef B2500000
        case 2500000:
            retval = B2500000;
            break;
#endif

#ifdef B3000000
        case 3000000:
            retval = B3000000;
            break;
#endif

#ifdef B3500000
        case 3500000:
            retval = B3500000;
            break;
#endif

#ifdef B4000000
        case 4000000:
            retval = B4000000;
            break;
#endif

        default:
            retval = 0;
            break;
    }

    return retval;
}

/* THREAD FUNCTIONS */

static void *workerThread(void *threadParameter)
{
    int running = 1;

    DPRINTF(("THREAD START\n"));
    while (running)
    {
        int ready;

        DPRINTF(("POLL: 0x%x 0x%x\n", pfd[FD_UART].events,
                 pfd[FD_CONTROL].events));
        /*
         * We wait for events on the file descriptors indefinitely
         * using the following scheme:
         *
         *  FD_UART:    POLLIN if buffer is not full, and POLLOUT if we
         *              have pending data to transmit.
         *  FD_CONTROL: Always, and only, POLLIN to receive rescheduling
         *              and termination requests.
         */
        ready = poll(pfd, 2, -1);

        DPRINTF(("ready %d 0x%x 0x%x\n", ready,
                 pfd[FD_UART].revents, pfd[FD_CONTROL].revents));
        if (ready > 0)
        {
            if (pfd[FD_UART].revents & (POLLERR | POLLHUP | POLLNVAL))
            {
                DPRINTF(("ERROR FD_UART FAILED\n"));
                running = 0;
            }

            if (pfd[FD_CONTROL].revents & (POLLERR | POLLHUP | POLLNVAL))
            {
                DPRINTF(("ERROR FD_CONTROL FAILED\n"));
                running = 0;
            }

            if (pfd[FD_CONTROL].revents & POLLIN)
            {
                char stop;

                DPRINTF(("FD_CONTROL\n"));

                /*
                 * If stop == 0 this is just a rescheduling signal
                 * but otherwise we treat it as a termination request.
                 */
                if ((read(pfd[FD_CONTROL].fd, &stop, 1) != 1) ||
                    (stop != 0))
                {
                    DPRINTF(("STOP RECEIVED\n"));

                    running = 0;
                }
                else
                {
                    DPRINTF(("RESCHEDULING\n"));
                }
            }

            if (pfd[FD_UART].revents & POLLIN)
            {
                ssize_t bytesRead;
                int iores;
                int iocount, bufferSpace;
                CsrBool tokenDetected = FALSE;

                pthread_mutex_lock(&receiveMutex);
                bufferSpace = (int) UART0_RX_numbers;
                pthread_mutex_unlock(&receiveMutex);

                bufferSpace = QUEUE_SIZE - bufferSpace;

                /* Find out how much data there is */
                iocount = 0;

                iores = ioctl(pfd[FD_UART].fd, FIONREAD, &iocount);
                if (iores)
                {
                    iocount = 0;
                }

                iocount = CSRMIN(iocount, (int) (QUEUE_SIZE - UART0_RX_in_PTR));
                iocount = CSRMIN(iocount, bufferSpace);

                DPRINTF(("READ: %u\n", iocount));
                /* Read in and wrap around the list */
                bytesRead = read(pfd[FD_UART].fd, &UART0_RX_QUEUE[UART0_RX_in_PTR], iocount);
                if (bytesRead > 0)
                {
                    DPRINTF(("BYTES READ: %u\n", bytesRead));
                    if (instUseToken)
                    {
                        int i;

                        for (i = 0; i < bytesRead; i++)
                        {
                            if (UART0_RX_QUEUE[UART0_RX_in_PTR + i] == instToken)
                            {
                                tokenDetected = TRUE;
                                break;
                            }
                        }
                    }

                    UART0_RX_in_PTR += bytesRead;

                    if (UART0_RX_in_PTR >= QUEUE_SIZE)
                    {
                        UART0_RX_in_PTR = 0;
                    }

                    if (!instUseToken)
                    {
                        DPRINTF(("BGINT\n"));
                        CsrSchedBgintSet(rxBgint);
                    }
                    else if (instUseToken && tokenDetected)
                    {
                        DPRINTF(("TOKEN\n"));
                        CsrSchedBgintSet(rxBgint);
                    }
                }
                else
                {
                    bytesRead = 0;
                }

                /*
                 * Indicate that there is data in the buffer,
                 * and stop subscribing to UART read events if
                 * the buffer is full.
                 */
                pthread_mutex_lock(&receiveMutex);
                UART0_RX_numbers += bytesRead;
                if (UART0_RX_numbers >= QUEUE_SIZE)
                {
                    pfd[FD_UART].events &= ~POLLIN;
                }
                pthread_mutex_unlock(&receiveMutex);
            }
        }
    }

    DPRINTF(("THREAD STOP\n"));
    return NULL;
}

static int init(void *handle, CsrUint32 baud)
{
    int fd;
    struct termios options;

    fd = open(instDevice, O_RDWR | O_NOCTTY);

    if (fd == -1)
    {
        char txt[200];

        printf(txt, "open_port: Unable to open serial port - %s", instDevice);
        perror(txt);

        return -1;
    }

    fcntl(fd, F_SETFL, 0);

    tcgetattr(fd, &options);

    cfsetispeed(&options, baudrate_map(baud));
    cfsetospeed(&options, baudrate_map(baud));

    options.c_cflag |= (CLOCAL | CREAD);

    if (instParity == CSR_UART_DRV_PARITY_EVEN)
    {
        options.c_cflag |= PARENB;
        options.c_cflag &= ~PARODD;
    }
    else /* CSR_UART_DRV_PARITY_NONE */
    {
        options.c_cflag &= ~PARENB;
    }

    if (instStopBits == CSR_UART_DRV_STOP_BITS_2)
    {
        options.c_cflag |= CSTOPB;
    }
    else /* CSR_UART_DRV_STOP_BITS_1 */
    {
        options.c_cflag &= ~CSTOPB;
    }

    if (instDataBits == CSR_UART_DRV_DATA_BITS_8)
    {
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;
    }
    else /* CSR_UART_DRV_DATA_BITS_7 */
    {
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS7;
    }

    if (instFlowControl)
    {
        options.c_cflag |= CRTSCTS;
    }
    else
    {
        options.c_cflag &= ~CRTSCTS;
    }

    options.c_lflag &= ~(ICANON | IEXTEN | ISIG | ECHO);
    options.c_oflag &= ~OPOST;
    options.c_iflag &= ~(ICRNL | INPCK | ISTRIP | IXON | BRKINT);

    options.c_cc[VMIN] = 1;
    options.c_cc[VTIME] = 0;

    tcsetattr(fd, TCSANOW, &options);

    return fd;
}

CsrBool CsrUartDrvStart(void *handle, CsrUint8 reset)
{
    int ret, fd;
    static int controlPipe[2];
    CsrUint32 baud;

    DPRINTF(("UARTDRIVER START\n"));

    if (reset == 0)
    {
        baud = instBaud;
    }
    else
    {
        baud = instResetBaud;
    }

    curBaud = baud;

    fd = init(handle, baud);
    if (fd == -1)
    {
        return FALSE;
    }

    if (pipe(controlPipe) != 0)
    {
        close(fd);

        return FALSE;
    }

    /*
     * Set up poll structure to provide read events
     * from the UART fd and reader pipe.
     */
    pfd[FD_UART].fd = fd;
    pfd[FD_UART].events = POLLIN;
    pfd[FD_CONTROL].fd = controlPipe[PIPE_READ];
    pfd[FD_CONTROL].events = POLLIN;

    control = controlPipe[PIPE_WRITE];

    UART0_RX_in_PTR = UART0_RX_out_PTR = UART0_RX_numbers = 0x00;

    if (pthread_mutex_init(&receiveMutex, NULL) != 0)
    {
        close(controlPipe[0]);
        close(controlPipe[1]);
        close(fd);

        return FALSE;
    }

    ret = pthread_create(&worker_thread, NULL, workerThread, NULL);
    if (ret != 0)
    {
        perror("CsrUartDrvStart: pthread_create, failed to create worker thread\n");
        close(controlPipe[0]);
        close(controlPipe[1]);
        close(fd);

        pthread_mutex_destroy(&receiveMutex);

        return FALSE;
    }

    return TRUE;
}

CsrBool CsrUartDrvStop(void *handle)
{
    void *thread_res;
    int ret;
    char stop = 1;

    DPRINTF(("UARTDRIVER STOP\n"));
    /* Stop worker thread */
    if (write(control, &stop, 1) != 1)
    {
        perror("CsrUartDrvStop: could not signal stop\n");
        return FALSE;
    }
    else
    {
        DPRINTF(("STOPPING THREAD\n"));
        ret = pthread_join(worker_thread, &thread_res);
        if (ret != 0)
        {
            perror("CsrUartDrvStop: worker thread join error");
        }
        else
        {
            DPRINTF(("THREAD STOPPED\n"));
        }
    }

    close(pfd[FD_UART].fd);
    close(pfd[FD_CONTROL].fd);
    close(control);

    pthread_mutex_destroy(&receiveMutex);

    return TRUE;
}

CsrUint32 CsrUartDrvGetTxSpace(void *handle)
{
    return QUEUE_SIZE;
}

CsrBool CsrUartDrvTx(void *handle, const CsrUint8 *data, CsrUint32 dataLength, CsrUint32 *numSent)
{
    ssize_t res;
    CsrBool rv;

    DPRINTF(("TX %u\n", dataLength));

    res = write(pfd[FD_UART].fd, data, dataLength);

    if ((res >= 0) &&
        ((CsrUint32) res == dataLength))
    {
        DPRINTF(("TX COMPLETE\n"));
        *numSent = dataLength;
        rv = TRUE;
    }
    else
    {
        DPRINTF(("TX FAILURE\n"));
        rv = FALSE;
    }

    return rv;
}

CsrUint32 CsrUartDrvGetRxAvailable(void *handle)
{
    CsrUint32 rxBytes;

    pthread_mutex_lock(&receiveMutex);
    rxBytes = UART0_RX_numbers;
    pthread_mutex_unlock(&receiveMutex);

    return QUEUE_SIZE - rxBytes;
}

void CsrUartDrvRx(void *handle)
{
    CsrUint32 bytesConsumed;
    CsrUint32 noOfBytes;
    CsrBool reschedule = FALSE;

    DPRINTF(("RX\n"));
    pthread_mutex_lock(&receiveMutex);
    noOfBytes = UART0_RX_numbers;
    pthread_mutex_unlock(&receiveMutex);

    if (noOfBytes > (QUEUE_SIZE - UART0_RX_out_PTR))
    {
        noOfBytes = (QUEUE_SIZE - UART0_RX_out_PTR);
    }

    bytesConsumed = rxData(&UART0_RX_QUEUE[UART0_RX_out_PTR],
        noOfBytes);

    DPRINTF(("CONSUMED %u\n", bytesConsumed));
    UART0_RX_out_PTR = UART0_RX_out_PTR + bytesConsumed;

    if (UART0_RX_out_PTR >= QUEUE_SIZE)
    {
        UART0_RX_out_PTR = 0x00; /* Buffer start from beginning*/
    }

    /*
     * Adjust buffer data counter and, if disabled, enable UART
     * read events if disabled, because there is free space now.
     */
    pthread_mutex_lock(&receiveMutex);
    UART0_RX_numbers = UART0_RX_numbers - bytesConsumed;
    noOfBytes = UART0_RX_numbers;
    if ((pfd[FD_UART].events & POLLIN) == 0)
    {
        DPRINTF(("RESCHEDULE\n"));
        pfd[FD_UART].events |= POLLIN;
        reschedule = TRUE;
    }
    pthread_mutex_unlock(&receiveMutex);

    if (reschedule)
    {
        char stop = 0;
        ssize_t res = write(control, &stop, 1);
        CSR_UNUSED(res);
    }

    /* If buffer is not empty, reschedule bgint. */
    if (noOfBytes > 0)
    {
        DPRINTF(("BGINT\n"));
        CsrSchedBgintSet(rxBgint);
    }
}

CsrUint32 CsrUartDrvLowLevelTransportTxBufLevel(void *handle)
{
    return 0;
}

CsrUint32 CsrUartDrvLowLevelTransportRx(void *handle, CsrUint32 dataLength, CsrUint8 *data)
{
    CsrUint32 rxCount, rxLeft;

    pthread_mutex_lock(&receiveMutex);
    rxCount = UART0_RX_numbers;
    pthread_mutex_unlock(&receiveMutex);

    if (rxCount > dataLength)
    {
        rxCount = dataLength;
    }

    if (rxCount > (QUEUE_SIZE - UART0_RX_out_PTR))
    {
        rxCount = (QUEUE_SIZE - UART0_RX_out_PTR);
    }
    memcpy(data, &UART0_RX_QUEUE[UART0_RX_out_PTR], rxCount);

    UART0_RX_out_PTR = UART0_RX_out_PTR + rxCount;

    if (UART0_RX_out_PTR >= QUEUE_SIZE)
    {
        UART0_RX_out_PTR = 0x00; /* Buffer start from beginning*/
    }

    pthread_mutex_lock(&receiveMutex);
    UART0_RX_numbers -= rxCount;
    rxLeft = UART0_RX_numbers;
    pthread_mutex_unlock(&receiveMutex);

    /* If buffer is not empty initialize a new bg_int */
    if (rxLeft > 0)
    {
        CsrSchedBgintSet(rxBgint);
    }
    return rxCount;
}

CsrUint32 CsrUartDrvGetBaudrate(void *handle)
{
    return curBaud;
}

void CsrUartDrvRegister(void *handle, CsrUartDrvDataRx rxDataFn, CsrSchedBgint rxBgintHdl)
{
    rxData = rxDataFn;
    rxBgint = rxBgintHdl;
}

void *CsrUartDrvInitialise(const CsrCharString *device,
    CsrUint32 baud, CsrUint32 resetBaud,
    CsrUartDrvDataBits dataBits,
    CsrUartDrvParity parity,
    CsrUartDrvStopBits stopBits,
    CsrBool flowControl,
    const CsrUint8 *token)
{
    if (!instInUse)
    {
        instInUse = TRUE;

        curBaud = 0;
        instBaud = baud;
        instResetBaud = resetBaud;
        instDataBits = dataBits;
        instParity = parity;
        instStopBits = stopBits;
        instFlowControl = flowControl;
        if (token != NULL)
        {
            instUseToken = TRUE;
            instToken = *token;
        }
        CsrStrNCpyZero(instDevice, device, sizeof(instDevice));

        return &instInUse;
    }

    return NULL;
}

void CsrUartDrvDeinitialise(void *handle)
{
    instInUse = FALSE;
}

CsrUint32 CsrUartDrvBaudrateActualGet(CsrUint32 wantedBaudrate)
{
    return wantedBaudrate;
}
