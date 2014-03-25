/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_IWU_MODULE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#ifndef __USE_BSD
#define __USE_BSD
#endif

#include <termios.h>

#include "csr_bt_iwu.h"
#include "csr_types.h"
#include "csr_util.h"

#define TIOCMGET    0x5415
#define TIOCMSET    0x5418

#define TIOCM_LE    0x001
#define TIOCM_DTR   0x002
#define TIOCM_RTS   0x004
#define TIOCM_ST    0x008
#define TIOCM_SR    0x010
#define TIOCM_CTS   0x020
#define TIOCM_DSR   0x100
#define TIOCM_OUT1  0x2000
#define TIOCM_OUT2  0x4000
#define TIOCM_LOOP  0x8000

#ifndef TIOCM_CD
#define TIOCM_CD    0x040
#endif

#ifndef TIOCM_RI
#define TIOCM_RI    0x080
#endif

#define MAX_PATH_LEN 255
static char             port[MAX_PATH_LEN];
static int              fd;

/* Get modem flags */
CsrBool CsrIwuModemStatGet(CsrUint32 *modemStat)
{
    CsrUint32 tio;

    /* Get current settings */
    tio = 0;
    *modemStat = 0;
    if(ioctl(fd, TIOCMGET, &tio))
    {
        return FALSE;
    }

    /* Clear to send */
    if((tio & TIOCM_CTS) == TIOCM_CTS)
    {
        *modemStat |= MS_CTS_ON;
    }
    /* Data set ready */
    if((tio & TIOCM_DSR) == TIOCM_DSR)
    {
        *modemStat |= MS_DSR_ON;
    }
    /* Ring indicator */
    if((tio & TIOCM_RI) == TIOCM_RI)
    {
        *modemStat |= MS_RING_ON;
    }
    /* Carrier detect */
    if((tio & TIOCM_CD) == TIOCM_CD)
    {
        *modemStat |= MS_RLSD_ON;
    }

    return TRUE;
}

/* Set a single special modem flag */
CsrBool CsrIwuModemStatSet(CsrUint32 modemStat)
{
    CsrUint32 tionew;
    CsrUint32 tioold;
    int ret;
    tionew = 0;
    tioold = 0;

    /* Get current settings */
    ret = ioctl(fd, TIOCMGET, &tioold);
    tionew = tioold;

    if(!ret)
    {
        switch(modemStat)
        {
            case CLRDTR:
                tionew &= ~TIOCM_DTR;
                break;

            case CLRRTS:
                tionew &= ~TIOCM_RTS;
                break;

            case SETDTR:
                tionew |= TIOCM_DTR;
                break;

            case SETRTS:
                tionew |= TIOCM_DTR;
                break;

            case SETXOFF:
                printf("xoff not supported\n");
                break;

            case SETXON:
                printf("xon not supported\n");
                break;

            case SETBREAK:
                printf("break not supported\n");
                break;

            case CLRBREAK:
                printf("break not supported\n");
                break;
        }

        /* Set new ioctl */
        if(tionew != tioold)
        {
            ret = ioctl(fd, TIOCMSET, &tionew);
        }
    }

    return (ret == 0);
}

/* Reset global variables */
void CsrIwuSerialConfig()
{
    port[0] = 0;
    fd      = -1;
}

/* Close file */
void CsrIwuSerialDestroy()
{
    if(fd != -1)
    {
        close(fd);
    }
    fd = -1;
}

/* Close file */
void CsrIwuDisconnect(void)
{
    if(fd != -1)
    {
        close(fd);
    }
    fd = -1;
}

/* Open file and set preferences */
int CsrIwuConnect(char *port_arg, int rate_arg, serial_parity_t parity_arg)
{
    struct termios tc;
    int error;

    error = 0;
    CsrIwuSerialConfig();
    if(fd != -1)
    {
        close(fd);
    }
    fd = -1;

    if(port_arg==0)
    {
        error = 1;
    }

    /* Prepare port */
    if(!error)
    {
        strncpy(port, port_arg, MAX_PATH_LEN);
        CsrMemSet(&tc, 0, sizeof(tc));

        fd = open(port, O_RDWR | O_NOCTTY);
        if(fd == -1)
        {
            error = 2;
        }
    }

    /* Get current settings */
    if(tcgetattr(fd, &tc))
    {
        error = 3;
    }

    if(!error)
    {
        /* Set most generic stuff
         * data bits: 8
         * stop bits: 1
         * no parity
         * no xon/xoff
         * no echo
         * no line end convertion
         */
        cfmakeraw(&tc);

        /* Baud rate */
        cfsetospeed(&tc, rate_arg);

        /* Parity */
        switch(parity_arg)
        {
            case spNONE:
                tc.c_cflag &= ~PARENB;
                break;

            case spEVEN:
                tc.c_cflag |= PARENB;
                break;

            case spODD:
                tc.c_cflag |= PARENB | PARODD;
                break;
        }

        /* Enable hardware flow control */
        tc.c_cc[VSTART] = 0x13;
        tc.c_cc[VSTOP]  = 0x19;
    }

    if(!error)
    {
        /* Store settings */
        if(tcsetattr(fd, TCSANOW, &tc))
        {
            error = 4;
        }
    }

    if(error != 0)
    {
        if(fd < 0)
        {
            close(fd);
        }
        fd = -1;
    }

    return error;
}

/* Write data to file descriptor, blocking */
void CsrIwuArraySend(char *buffer, int len)
{
    if(fd != -1)
    {
        ssize_t res;

        res = write(fd, buffer, len);
        if(res == -1)
        {
            printf("error sending data\n");
        }
        else if(res != len)
        {
            printf("not all data was sent\n");
        }
    }
}

/* Read data from file descriptor, blocking */
int CsrIwuArrayGet(char *buffer, int len)
{
    ssize_t res;
    res = 0;

    if(fd != -1)
    {
        res = read(fd, buffer, len);

        if(res == -1)
        {
            printf("error reading data\n");
            res = 0;
        }
        else if(res != len)
        {
            printf("not all data was read");
        }
    }

    return res;
}

/* Number of bytes waiting to be read */
int CsrIwuRxQLenGet(void)
{
    int res;
    int count;

    count = 0;
    res = ioctl(fd, FIONREAD, &count);
    if(res)
    {
        count = 0;
    }

    return count;
}

/* Number of bytes waiting to be sent (always zero in blocking mode!) */
int CsrIwuTxQLenGet(void)
{
    return 0;
}

/* wrapper for ARM uart interface */
void Uart1Drv_Tx(char *dataPtr, CsrUint16 valueLen, CsrUint16 *numSend)
{
    CsrIwuArraySend(dataPtr, valueLen);
}
#endif

