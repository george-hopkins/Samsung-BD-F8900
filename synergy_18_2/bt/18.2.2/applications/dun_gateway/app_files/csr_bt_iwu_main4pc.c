/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #1 $
****************************************************************************/

#include "csr_synergy.h"


#include "csr_sched.h"
#include "csr_panic.h"

#include "csr_bt_iwu_handler.h"
#include "csr_bt_iwu_prim.h"
#include "csr_bt_at_lib.h"

#include "csr_bt_iwu.h"

#ifndef __arm
#include <stdio.h>
#endif

/******************************************************************************
CsrBtIwuInit
******************************************************************************/
void CsrBtIwuInit(void **gash)
{
    iwuInstanceData_t  *iwuData;

    *gash                           = (void *) CsrPmemAlloc(sizeof(iwuInstanceData_t));
    iwuData                         = (iwuInstanceData_t *) *gash;
    iwuData->state                  = modemNotReady;
    iwuData->iwuMsgP                = NULL;
    iwuData->bufCount               = 0;
    iwuData->timerFlag              = FALSE;
    iwuData->waterMarkFlag          = FALSE;
    iwuData->iwuDataReqQueue        = NULL;
    iwuData->iwuCfmFlag             = TRUE;
    iwuData->bitSetting             = CSR_BT_MODEM_DCD_MASK | CSR_BT_MODEM_CTS_MASK;
    iwuData->btConnId               = 0x00ff;
    iwuData->serverChannel          = 0xff;
    iwuData->queueCounter           = 0;
    iwuData->count                  = 0;
    iwuData->countN                 = 0;
    iwuData->startUpTimerActive     = FALSE;
    setUpSerialPort(iwuData);
}


/******************************************************************************
CsrBtIwuDeinit
******************************************************************************/
void CsrBtIwuDeinit(void **gash)
{
    iwuInstanceData_t  *iwuData;
    CsrUint16          eventClass=0;
    void *              msg=NULL;
    CsrBool    lastMsg;

    /* continue to poll any message of the input queue */
    lastMsg = FALSE;
    iwuData = (iwuInstanceData_t *) (*gash);

    while (!lastMsg)
    {
        if (!CsrMessageQueuePop( &(iwuData->iwuDataReqQueue), &eventClass, &msg))
        {
            lastMsg = !CsrSchedMessageGet(&eventClass, &msg);
        }

        if ( !lastMsg )
        {
            switch(eventClass)
            {
                case CSR_BT_IWU_PRIM:
                    {
                        CsrPrim         *primPtr;
                        primPtr = (CsrPrim *)msg;
                        switch(*primPtr)
                        {
                            /* data req is the only signal with data
                             * included */
                            case IWU_MODEM_DATA_REQ:
                                {
                                    IWU_MODEM_DATA_REQ_T *prim;

                                    prim = (IWU_MODEM_DATA_REQ_T *) msg;

                                    CsrPmemFree(prim->modemPayload);
                                    break;
                                }

                            default:
                                {/*    unexpected CSR_BT_IWU_PRIM type received */
                                    printf("####### Unknown CSR_BT_IWU_PRIM type in CsrBtIwuDeinit %x\n", *primPtr);
                                    break;
                                }
                        }
                        break;
                    }

                case CSR_BT_AT_PRIM:
                    {
                        CsrPrim         *primPtr;
                        primPtr = (CsrPrim *)msg;
                        switch(*primPtr)
                        {
                            case CSR_BT_AT_DG_DATA_IND :
                                {
                                    CsrBtAtDgDataInd *prim;

                                    prim = (CsrBtAtDgDataInd *) msg;
                                    CsrPmemFree(prim->payload);
                                    break;
                                }
                            case CSR_BT_AT_DG_ATC_D_IND:
                                {
                                    CsrBtAtDgAtcDInd *prim;

                                    prim = (CsrBtAtDgAtcDInd *) msg;
                                    CsrPmemFree(prim->payload);
                                    break;
                                }

                            default:
                                {/*    unexpected CSR_BT_AT_PRIM type received */
                                    printf("####### Unknown CSR_BT_AT_PRIM type in CsrBtIwuDeinit %x\n", *primPtr);
                                    break;
                                }
                        }
                        break;
                    }
            }
            CsrPmemFree(msg);
        }
    }
    if (iwuData->timerFlag)
    {
        CsrSchedTimerCancel(iwuData->timerId, NULL, NULL);
    }
    CsrPmemFree(iwuData);
    CsrIwuDisconnect();
}


/******************************************************************************
CsrBtIwuHandler
******************************************************************************/
void CsrBtIwuHandler(void **gash)
{
    iwuInstanceData_t  *iwuData;
    CsrUint16            eventClass=0;
    void *        msg=NULL;
    static CsrUint16    lastEventClass     = 0;
    static CsrUint16    lastPrimType      = 0;

    iwuData = (iwuInstanceData_t *) (*gash);

    CsrSchedMessageGet(&eventClass , &msg);
    iwuData->iwuMsgP = msg;

    lastEventClass     = eventClass;
    lastPrimType      =  *((CsrUint16*) msg) ;

    switch(eventClass)
    {
        case CSR_BT_IWU_PRIM:
            {
                CsrPrim         *primPtr;
                primPtr = (CsrPrim *)msg;
                switch(*primPtr)
                {
                    case IWU_MODEM_DATA_REQ:
                        {
                            iwuModemDataReq(iwuData);
                            break;
                        }
#ifndef __arm
                    case IWU_ACTIVATE_REQ:
                        {
                            printf("Scanning active - waiting for client to connect ....\n");
                            CsrBtAtDgActivateReqSend(CSR_BT_IWU_IFACEQUEUE, INFINIT_TIME);
                            break;
                        }
                    case IWU_DEACTIVATE_REQ:
                        {
                            deactivateReq(iwuData);
                            break;
                        }
                    case IWU_RELEASE_REQ:
                        {
                            releaseReq(iwuData);
                            break;
                        }
#endif
                    case IWU_MODEM_RESET_REQ:
                        {
                            iwuSendModemResetReq(iwuData);
                            break;
                        }
                    case IWU_MODEM_STATUS_REQ:
                        {
                            iwuModemStatusReqHandler(iwuData);
                            break;
                        }
                    default:
                        {/*    unexpected CSR_BT_IWU_PRIM type received */
                            printf("####### Unknown CSR_BT_IWU_PRIM type in iwu_handler %x\n", *primPtr);
                            break;
                        }
                }
                break;
            }

        case CSR_BT_AT_PRIM:
            {
                CsrPrim         *primPtr;
                primPtr = (CsrPrim *)msg;

                switch(iwuData->state)
                {
                    case modemReady :
                        {
                            switch(*primPtr)
                            {
                                case CSR_BT_AT_DG_DATA_IND :
                                    {
                                        iwuAtDgDataIndHandler(iwuData);
                                        break;
                                    }
                                case CSR_BT_AT_DG_ATC_AND_C_IND :
                                    {
                                        CsrBtAtDgAtcResultCodeResSend(0);
                                        break;
                                    }
                                case CSR_BT_AT_DG_ATC_AND_D_IND :
                                    {
                                        CsrBtAtDgAtcResultCodeResSend(0);
                                        break;
                                    }
                                case CSR_BT_AT_DG_ATC_AND_F_IND :
                                    {
                                        iwuAtDgAtcAndFIndHandler(iwuData);
                                        break;
                                    }
                                case CSR_BT_AT_DG_ATC_A_IND :
                                    {
                                        iwuSendCommandAtA(iwuData);
                                        break;
                                    }
                                case CSR_BT_AT_DG_ATC_D_IND :
                                    {
                                        iwuSendCommandAtD(iwuData);
                                        break;
                                    }
                                case CSR_BT_AT_DG_ATC_H_IND :
                                    {
                                        CsrSchedTimerSet(100000, iwuSendModemResetReqIn, 0, iwuData);
                                        CsrSchedTimerSet(100000, iwuSendModemResetReqIn, 0, iwuData);
                                        CsrSchedTimerSet(100000, iwuSendModemResetReqIn, 0, iwuData);
                                        CsrSchedTimerSet(500000, iwuSendCommandAthIn, 0, iwuData);
                                        CsrBtAtDgAtcResultCodeResSend(0);
                                        break;
                                    }
                                case CSR_BT_AT_DG_ATC_O_IND :
                                    {
                                        iwuAtDgAtcOIndHandler();
                                        break;
                                    }
                                case CSR_BT_AT_DG_ATC_X_IND :
                                    {
                                        iwuAtDgAtcXIndHandler(iwuData);
                                        break;
                                    }
                                case CSR_BT_AT_DG_ATC_Z_IND :
                                    {
                                        iwuAtDgAtcZIndHandler(iwuData);
                                        break;
                                    }
                                case CSR_BT_AT_DG_ATC_ABORT_IND :
                                    {
                                        iwuSendAbortReq(iwuData);
                                        break;
                                    }
                                case CSR_BT_AT_DG_CONTROL_IND:
                                    {
                                        iwuAtDgControlIndHandler(iwuData);
                                        break;
                                    }
                                case CSR_BT_AT_DG_DISCONNECT_IND :
                                    {
                                        iwuAtDgDisconnectIndHandler(iwuData);
                                        break;
                                    }
                                case CSR_BT_AT_DG_CONNECT_IND :
                                    {
                                        iwuAtDgConnectIndHandler(iwuData);
                                        break;
                                    }
                                case CSR_BT_AT_DG_PORTNEG_IND :
                                    {
                                        iwuAtDgPortnegIndHandler(iwuData);
                                        break;
                                    }
                                case CSR_BT_AT_DG_DATA_CFM :
                                    {
                                        iwuAtDgDataCfmHandler(iwuData);
                                        break;
                                    }
                                case CSR_BT_AT_DG_DEACTIVATE_CFM :
                                    {
                                        iwuAtDgDeactivateCfmHandler(iwuData);
                                        break;
                                    }
                                case CSR_BT_AT_DG_DATA_PATH_STATUS_IND:
                                    {
                                        iwuAtDgDataPathStatusIndHandler(iwuData);
                                        break;
                                    }
                                default:
                                    {/*    unexpected CSR_BT_IWU_PRIM type received */
                                        printf("####### Unknown CSR_BT_AT_PRIM type in iwu_handler %x\n", *primPtr);
                                        break;
                                    }
                            }
                            break;
                        }
                    default :
                        {
                            printf("####### CSR_BT_AT_PRIM type: %x,  is received in iwu_handler in a state where modem is not ready\n", *primPtr);
                            break;
                        }
                }
                break;
            }
        default:
            {/*    unexpected eventClass received */
                printf("####### unexpected eventClass received in iwu_handler %x\n",eventClass);
                break;
            }
    } /* end switch eventclass */
    CsrPmemFree(iwuData->iwuMsgP);
}

