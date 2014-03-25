#ifndef SERIAL_IWU_H__
#define SERIAL_IWU_H__

#include "csr_synergy.h"
/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#define STRICT
#include <stdio.h>
#include <stdlib.h>
#include "csr_types.h"

/* Linux */
#ifdef __linux__
#include <termios.h>
#define SETXOFF           1
#define SETXON            2
#define SETRTS            3
#define CLRRTS            4
#define SETDTR            5
#define CLRDTR            6
#define SETBREAK          8
#define CLRBREAK          9

#define MS_CTS_ON         1
#define MS_DSR_ON         2
#define MS_RING_ON        4
#define MS_RLSD_ON        8

/* Windows */
#elif defined(_WIN32)
#include <process.h>
#include <windows.h>

#endif

#ifdef __cplusplus
extern "C" {
#endif

#define TXUARTBUFFERSIZE    (4096)
#define RXUARTBUFFERSIZE    (4096)

typedef enum
{
    spNONE,
    spODD,
    spEVEN
} serial_parity_t;

CsrBool CsrIwuModemStatGet(CsrUint32 *lpModemStat);
CsrBool CsrIwuModemStatSet(CsrUint32 modemStat);
void   CsrIwuSerialConfig(void);
void   CsrIwuSerialDestroy(void);
void   CsrIwuDisconnect(void);
int    CsrIwuConnect(char *port_arg, int rate_arg, serial_parity_t parity_arg);
void   CsrIwuArraySend(char *buffer, int len);
int    CsrIwuArrayGet(char *buffer, int len);
int    CsrIwuRxQLenGet(void);
int    CsrIwuTxQLenGet(void);

/*  wrapper for ARM uart interface */
void Uart1Drv_Tx(char *dataPtr, CsrUint16 valueLen, CsrUint16 *numSend);

#ifdef __cplusplus
}
#endif

#endif
