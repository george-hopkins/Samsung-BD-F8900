/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_bt_iwu_handler.h"
#include "csr_bt_iwu_prim.h"

#ifdef __arm
#include "armLowLev.h"
#include "csr_bt_Uart1.h"
#else
#include "csr_sched.h"
#include "csr_bt_iwu.h"

#define RX_READ_DELAY                (100000)
#define UART_BAUD_RATE               (115200)
#define UART_MAX_CHARS_2_READ        (50)
#define IDLE_COUNTER_LIMIT           (10)

extern char extra_ComPort[128];
static int idleCounter;
#endif


/*************************************************************************************
  UartDrvTx : Wrapper for ARM or PC uart interface
************************************************************************************/
void UartDrvTx(char *dataPtr, CsrUint16 valueLen, CsrUint16 *numSend)
{
#ifdef __arm
    Uart1Drv_Tx((CsrUint8*)dataPtr, valueLen, numSend);
#else
    CsrIwuArraySend(dataPtr, valueLen);
#endif
}


/*************************************************************************************
  setUpSerialPort :
************************************************************************************/
void setUpSerialPort(iwuInstanceData_t *iwuData)
{
#ifdef __arm
    modemDefaultSetupReq(iwuData);
#else

    if ( CsrIwuConnect(extra_ComPort, UART_BAUD_RATE, spNONE) != 0 )
    {
        printf("Com port could not be properly initialised\n\n");
        printf("     Hit <Esc> to exit\n\n");
    }
    else
    {
        idleCounter = 0;
        CsrSchedTimerSet(RX_READ_DELAY, rxTimerFkt, 0, iwuData);

        iwuData->bgint_rx = CsrSchedBgintReg(rxBgIntFkt,
                                             iwuData,
                                             "dg_rx");
        modemDefaultSetupReq(iwuData);
        printf("Com port opened...\n");
    }
#endif
}


#ifndef __arm

/*************************************************************************************
  rxTimerFkt :
************************************************************************************/
void rxTimerFkt(CsrUint16 unused1, void *arg)
{
    iwuInstanceData_t *iwuData;
    int    rxQLen;

    iwuData = (iwuInstanceData_t *)arg;

    rxQLen = CsrIwuRxQLenGet();
    if ( (rxQLen > 1) || (idleCounter > IDLE_COUNTER_LIMIT) )
    {
        /*    data in the buffer so set the bg int flag to activate the read function */
        CsrSchedBgintSet(iwuData->bgint_rx);
        idleCounter = 0;
    }
    else
    {
        /*    no data so activate the timer function again */
        CsrSchedTimerSet(RX_READ_DELAY, rxTimerFkt, 0, arg);
        if (rxQLen > 0)
        {
            idleCounter++;
        }
    }
    checkModemStat();
}


/*************************************************************************************
  rxBgIntFkt :
************************************************************************************/
void rxBgIntFkt(void *arg)
{
    iwuInstanceData_t *iwuData;
    int chars2Read;
    char *readBuffer;

    iwuData = (iwuInstanceData_t *)arg;

    chars2Read = CsrIwuRxQLenGet();
    if (chars2Read != 0)
    {
        /*    find CSRMAX number of chars to read */
        if (chars2Read > UART_MAX_CHARS_2_READ)
        {
            chars2Read = UART_MAX_CHARS_2_READ;
            /*    there is more data in the buffer so set the bg int flag to call this function again */
            CsrSchedBgintSet(iwuData->bgint_rx);
        }
        else
        {
            /*    start the timer function again as we will read all the data */
            CsrSchedTimerSet(RX_READ_DELAY, rxTimerFkt, 0, arg);
        }

        readBuffer = (char *) CsrPmemAlloc(chars2Read + 12);
        CsrIwuArrayGet(readBuffer, chars2Read);
        iwuSendModemDataReq(chars2Read, readBuffer, CSR_BT_IWU_DG_UART1_SUCCESS);
    }
    else
    {
        /*    no data, should function should not have been called */
    }
    checkModemStat();
}

#endif

