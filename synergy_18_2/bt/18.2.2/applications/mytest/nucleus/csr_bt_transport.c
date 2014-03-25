/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_serial_init.h"
#include "csr_bt_app.h"

static CsrUint8 CsrBtAppUartHandle = 1;


void CsrBtAppTransportSetDefault(CsrUint16 transportType)
{
    if (CsrStrLen(casira_ComPort) == 0)
    {
        switch(transportType)
        {
            case TRANSPORT_TYPE_BCSP:
            {
             	//CL_API_LOGPrintf("\r\n BCSP: CsrBtAppTransportSetDefault%s",casira_ComPort);
                CsrStrCpy(casira_ComPort, DEFAULT_COMPORT);

                if (CsrStrLen(baudRate) == 0)
                {
                    CsrStrCpy(baudRate, DEFAULT_BAUDRATE);
                }
                break;
            }
			default:
            //CL_API_LOGPrintf("*** Unhandled transport type: %d\n", transportType);
            break;
        }
    }
}

void CsrBtAppTransportSetTask(CsrUint16 transportType)
{
    switch(transportType)
    {
        case TRANSPORT_TYPE_BCSP:
            {
             //CL_API_LOGPrintf("\r\n BCSP: CsrBtAppTransportSetTask");
                CSR_TM_BLUECORE_TRANSPORT_INIT = CSR_TM_BLUECORE_BCSP_INIT;
                break;
            }

        default:
            //CL_API_LOGPrintf("*** Unhandled transport type: %d\n", transportType);
           // exit(1);
            break;
    }
}

void *CsrBtAppTransportInit(void)
{
    void *uartHdl = NULL;

    switch(transportType)
    {
        case TRANSPORT_TYPE_BCSP:
            {
                CsrUint8 token = 0xC0;

                CL_API_LOGPrintf("BCSP: Using COM port: %s and baud rate: %s\n",
                       casira_ComPort,baudRate);

                uartHdl = CsrUartDrvInitialise(casira_ComPort,
                                               CsrStrToInt(baudRate),
                                               romBaud,
                                               CSR_UART_DRV_DATA_BITS_8,
                                               CSR_UART_DRV_PARITY_EVEN,
                                               CSR_UART_DRV_STOP_BITS_1,
                                               FALSE, &token);
                if (uartHdl == NULL)
                {
                    //CL_API_LOGPrintf("Unable to initialise UART driver\n");
                 //   exit(1);
                }
                else
                {
                    CsrTmBlueCoreRegisterUartHandleBcsp(uartHdl);
                }
                break;
            }

        default:
            //CL_API_LOGPrintf(" *** Unhandled transport type ***\n");
            //exit(1);
            break;
    }

    return(uartHdl);
}

void CsrBtAppTransportDeinit(void *uartHdl)
{
    if (uartHdl != NULL)
    {
        CsrUartDrvDeinitialise(uartHdl);
    }
    else
    {
        /* Do nothing */
    }
}
