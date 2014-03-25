/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_IWU_MODULE

#define STRICT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "csr_bt_iwu.h"
#include "csr_types.h"
#include "csr_util.h"

static serial_parity_t    parityMode;
static char             port[10];                      /* port name "com1",... */
static int              rate;                          /* baudrate */
static HANDLE           *serial_handle;                /* ... */
static CsrUint32            txQMaxSize;


CsrBool CsrIwuModemStatGet(CsrUint32 *modemStat)
{
    return GetCommModemStatus(serial_handle, modemStat);
}

/*********************************************************************
CLRDTR Clears the DTR (data-terminal-ready) signal.
CLRRTS Clears the RTS (request-to-send) signal.
SETDTR Sends the DTR (data-terminal-ready) signal.
SETRTS Sends the RTS (request-to-send) signal.
SETXOFF Causes transmission to act as if an XOFF character has been received.
SETXON Causes transmission to act as if an XON character has been received.
SETBREAK Suspends character transmission and places the transmission line in a break state until
    the ClearCommBreak function is called (or EscapeCommFunction is called with the CLRBREAK
    extended function code). The SETBREAK extended function code is identical to the
    SetCommBreak function. Note that this extended function does not flush data that has not
    been transmitted.
CLRBREAK Restores character transmission and places the transmission line in a nonbreak state.
    The CLRBREAK extended function code is identical to the ClearCommBreak function.
**********************************************************************/
CsrBool CsrIwuModemStatSet(CsrUint32 modemStat)
{
    return EscapeCommFunction(serial_handle, modemStat);
}

/* -------------------------------------------------------------------- */
/* -------------------------    Tserial   ----------------------------- */
/* -------------------------------------------------------------------- */
void CsrIwuSerialConfig()
{
    parityMode       = spNONE;
    port[0]          = 0;
    rate             = 0;
    serial_handle    = INVALID_HANDLE_VALUE;
    txQMaxSize = 0;
}

/* -------------------------------------------------------------------- */
/* --------------------------    ~Tserial     ------------------------- */
/* -------------------------------------------------------------------- */
void CsrIwuSerialDestroy()
{
    if (serial_handle!=INVALID_HANDLE_VALUE)
        CloseHandle(serial_handle);
    serial_handle = INVALID_HANDLE_VALUE;
}
/* -------------------------------------------------------------------- */
/* --------------------------    disconnect   ------------------------- */
/* -------------------------------------------------------------------- */
void CsrIwuDisconnect(void)
{
    if (serial_handle!=INVALID_HANDLE_VALUE)
        CloseHandle(serial_handle);
    serial_handle = INVALID_HANDLE_VALUE;
}
/* -------------------------------------------------------------------- */
/* --------------------------    connect      ------------------------- */
/* -------------------------------------------------------------------- */
int CsrIwuConnect(char *port_arg, int rate_arg, serial_parity_t parity_arg)
{
    int error;
    DCB  dcb;
    COMMTIMEOUTS cto = { 0, 0, 0, 10000, 10000 };

    CsrIwuSerialConfig();
    /* --------------------------------------------- */
    if (serial_handle!=INVALID_HANDLE_VALUE)
        CloseHandle(serial_handle);
    serial_handle = INVALID_HANDLE_VALUE;

    error = 0;

    if (port_arg!=0)
    {
        strncpy(port, port_arg, 10);
        rate      = rate_arg;
        parityMode= parity_arg;
        CsrMemSet(&dcb,0,sizeof(dcb));

        /* -------------------------------------------------------------------- */
        /* set DCB to configure the serial port*/
        dcb.DCBlength       = sizeof(dcb);

        /* ---------- Serial Port Config ------- */
        dcb.BaudRate        = rate;

        switch(parityMode)
        {
            case spNONE:
                dcb.Parity      = NOPARITY;
                dcb.fParity     = 0;
                break;
            case spEVEN:
                dcb.Parity      = EVENPARITY;
                dcb.fParity     = 1;
                break;
            case spODD:
                dcb.Parity      = ODDPARITY;
                dcb.fParity     = 1;
                break;
        }


        dcb.StopBits        = ONESTOPBIT;
        dcb.ByteSize        = 8;

        dcb.fOutxCtsFlow    = TRUE;
        dcb.fOutxDsrFlow    = TRUE;
        dcb.fDtrControl     = DTR_CONTROL_ENABLE;
        dcb.fDsrSensitivity = 0;
        dcb.fRtsControl     = RTS_CONTROL_ENABLE;
        dcb.fOutX           = 0;
        dcb.fInX            = 0;

        /* ----------------- misc parameters ----- */
        dcb.fErrorChar      = 0;
        dcb.fBinary         = 1;
        dcb.fNull           = 0;
        dcb.fAbortOnError   = 0;
        dcb.wReserved       = 0;
        dcb.XonLim          = 2;
        dcb.XoffLim         = 4;
        dcb.XonChar         = 0x13;
        dcb.XoffChar        = 0x19;
        dcb.EvtChar         = 0;

        /* -------------------------------------------------------------------- */
        serial_handle    = CreateFile(port, GENERIC_READ | GENERIC_WRITE,
                                      0, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
        /* opening serial port */


        if (serial_handle    != INVALID_HANDLE_VALUE)
        {
            if(!SetCommMask(serial_handle, 0))
                error = 1;

            /* set timeouts*/
            if(!SetCommTimeouts(serial_handle,&cto))
                error = 2;

            /* set DCB*/
            if(!SetCommState(serial_handle,&dcb))
                error = 4;

            /*    set the buffer sizes*/
            if(!SetupComm(serial_handle, RXUARTBUFFERSIZE, TXUARTBUFFERSIZE))
                error = 8;
        }
        else
            error = 16;
    }
    else
        error = 32;


    /* --------------------------------------------- */
    if (error!=0)
    {
        CloseHandle(serial_handle);
        serial_handle = INVALID_HANDLE_VALUE;
    }
    return(error);
}

/* -------------------------------------------------------------------- */
/* --------------------------    sendArray    ------------------------- */
/* -------------------------------------------------------------------- */
void CsrIwuArraySend(char *buffer, int len)
{
    if (serial_handle!=INVALID_HANDLE_VALUE)
    {
        int            result;
        int            charsWritten;

        result = WriteFile(serial_handle, buffer, len, &charsWritten, NULL);
        if (charsWritten != len)
        {
            printf("Not all chars are written\n");
        }
        if (result == 0)
        {
            printf("data not sent \n");
        }
        if (((CsrUint32) CsrIwuTxQLenGet()) > txQMaxSize )
        {
            txQMaxSize = CsrIwuTxQLenGet();
        }
    }
}


/* -------------------------------------------------------------------- */
/* --------------------------    getArray     ------------------------- */
/* -------------------------------------------------------------------- */
int CsrIwuArrayGet(char *buffer, int len)
{
    CsrUint32 read_nbr;

    read_nbr = 0;
    if (serial_handle!=INVALID_HANDLE_VALUE)
    {
        ReadFile(serial_handle, buffer, len, &read_nbr, NULL);
    }
    if ( (int) read_nbr != len)
    {
        printf("Not all chars are read\n");
    }

    return((int) read_nbr);
}


/*---------------------------------------------------------------------

-----------------------------------------------------------------------*/
int CsrIwuRxQLenGet(void)
{
    struct _COMSTAT status;
    int             n;
    CsrUint32   etat;

    n = 0;

    if (serial_handle!=INVALID_HANDLE_VALUE)
    {
        ClearCommError(serial_handle, &etat, &status);
        n = status.cbInQue;
    }

    return(n);
}


/*---------------------------------------------------------------------

-----------------------------------------------------------------------*/
int CsrIwuTxQLenGet(void)
{
    struct _COMSTAT status;
    int             n;
    CsrUint32   etat;

    n = 0;

    if (serial_handle!=INVALID_HANDLE_VALUE)
    {
        ClearCommError(serial_handle, &etat, &status);
        n = status.cbOutQue;
    }

    return(n);
}


/*    wrapper for ARM uart interface */
void Uart1Drv_Tx(char *dataPtr, CsrUint16 valueLen, CsrUint16 *numSend)
{
    CsrIwuArraySend(dataPtr, valueLen);
}
#endif
