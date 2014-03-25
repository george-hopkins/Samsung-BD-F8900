/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_BNEP_MODULE

#include "csr_bt_cm_bnep.h"
#include "csr_exceptionhandler.h"

/***************************************************************************
  CsrBtCmBnepArrivalHandler:
****************************************************************************/
void CsrBtCmBnepArrivalHandler(cmInstanceData_t *cmData)
{ /* Identify the event type from the BNEP layer and handle it */

    BNEP_UPRIM_T    *bnepPrim;

    bnepPrim = (BNEP_UPRIM_T *)cmData->recvMsgP;

    switch(bnepPrim->type)
    {
        case BNEP_EXTENDED_DATA_IND:
        {
            CsrBtCmBnepExtendedDataIndHandler(cmData);
            break;
        }

        case BNEP_CONNECT_CFM:
        {
            CsrBtCmBnepConnectCfmHandler(cmData);
            break;
        }

        case BNEP_CONNECT_IND:
        {
            CsrBtCmBnepConnectIndHandler(cmData);
            break;
        }

        case BNEP_DISCONNECT_IND:
        {
            CsrBtCmBnepDisconnectIndHandler(cmData);
            break;
        }

        default :
        {
            CsrBtCmStateEventException(CSR_BT_BNEP_PRIM, bnepPrim->type, cmData->globalState);
            break;
        }
    }
}
#endif

