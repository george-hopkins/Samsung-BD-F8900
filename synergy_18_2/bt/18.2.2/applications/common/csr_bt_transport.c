/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"
#include "csr_serial_init.h"
#include "csr_bt_app.h"

void CsrBtAppTransportSetDefault(CsrUint16 transportType)
{
    if (CsrStrLen(casira_ComPort) == 0)
    {
        switch(transportType)
        {
            case TRANSPORT_TYPE_H5_THREE_WIRE: /* fall through */
            case TRANSPORT_TYPE_H4_UART:       /* fall through */
            case TRANSPORT_TYPE_H4_DS:         /* fall through */
            case TRANSPORT_TYPE_BCSP:
                {
                    CsrStrCpy(casira_ComPort, DEFAULT_COMPORT);

                    if (CsrStrLen(baudRate) == 0)
                    {
                        CsrStrCpy(baudRate, DEFAULT_BAUDRATE);
                    }
                    break;
                }
#ifndef __arm
            case TRANSPORT_TYPE_USB:
                {
                    CsrStrCpy(casira_ComPort, DEFAULT_USB_DEVICE);
                    break;
                }
#endif
        }
    }
}

void CsrBtAppTransportSetTask(CsrUint16 transportType)
{
    switch(transportType)
    {
        case TRANSPORT_TYPE_BCSP:
            {
                CSR_TM_BLUECORE_TRANSPORT_INIT = CSR_TM_BLUECORE_BCSP_INIT;
                break;
            }
#ifndef __arm
        case TRANSPORT_TYPE_USB:
            {
                CSR_TM_BLUECORE_TRANSPORT_INIT = CSR_TM_BLUECORE_USB_INIT;
                break;
            }
#endif

        case TRANSPORT_TYPE_H4_DS:
            {
                CSR_TM_BLUECORE_TRANSPORT_INIT = CSR_TM_BLUECORE_H4DS_INIT;
                break;
            }
#if 0 /* Not supported yet */
        case TRANSPORT_TYPE_TYPE_A:
            {
                CSR_TM_BLUECORE_TRANSPORT_INIT = CSR_TM_BLUECORE_TYPE_A_INIT;
                break;
            }
#endif
        default:
            printf("*** Unhandled transport type: %d\n", transportType);
            exit(1);
            break;
    }
}

void *CsrBtAppTransportInit(void)
{
    void *uartHdl = NULL;

    switch(transportType)
    {
        case TRANSPORT_TYPE_H5_THREE_WIRE: /* fall through */
        case TRANSPORT_TYPE_H4_UART:    /* fall through */
        case TRANSPORT_TYPE_H4_DS:      /* fall through */
            {
                printf("H4/H4DS/H5: Using COM port: %s and baud rate: %s\n",
                       casira_ComPort,baudRate);

                uartHdl = CsrUartDrvInitialise(casira_ComPort,
                                               CsrStrToInt(baudRate),
                                               romBaud,
                                               CSR_UART_DRV_DATA_BITS_8,
                                               CSR_UART_DRV_PARITY_NONE,
                                               CSR_UART_DRV_STOP_BITS_1,
                                               TRUE, NULL);
                if (uartHdl == NULL)
                {
                    printf("Unable to initialise UART driver\n");
                    exit(1);
                }
                else
                {
                    CsrTmBlueCoreRegisterUartHandleH4ds(uartHdl);
                }
            }
            break;

        case TRANSPORT_TYPE_BCSP:
            {
                CsrUint8 token = 0xC0;

                printf("BCSP: Using COM port: %s and baud rate: %s\n",
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
                    printf("Unable to initialise UART driver\n");
                    exit(1);
                }
                else
                {
                    CsrTmBlueCoreRegisterUartHandleBcsp(uartHdl);
                }
                break;
            }
#ifdef CSR_IBFP_H4I_SUPPORT
            case TRANSPORT_TYPE_H4_I:
            {
                printf("H4I: Using COM port: %s and baud rate: %s\n",
                       casira_ComPort,baudRate);

                uartHdl = CsrUartDrvInitialise(casira_ComPort,
                                               CsrStrToInt(baudRate),
                                               romBaud,
                                               CSR_UART_DRV_DATA_BITS_8,
                                               CSR_UART_DRV_PARITY_EVEN,
                                               CSR_UART_DRV_STOP_BITS_1,
                                               FALSE, NULL);


                CsrTmBlueCoreRegisterUartHandleH4i(&uartHdl);
                
                break;
            }
#endif
        
#ifndef __arm
        case TRANSPORT_TYPE_USB:
            {
                printf("USB: Using USB port: %s\n", casira_ComPort);
                CsrUsbDrvConfigure(casira_ComPort);
                break;
            }
#endif

        default:
            printf(" *** Unhandled transport type ***\n");
            exit(1);
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

