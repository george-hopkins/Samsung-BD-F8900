/****************************************************************************

               (c) Cambridge Silicon Radio Limited 2009

               All rights reserved and confidential information of CSR

REVISION:      $Revision: #2 $
****************************************************************************/

#include "csr_synergy.h"

#ifndef EXCLUDE_CSR_BT_RFC_MODULE

#include "csr_bt_cm_main.h"
#include "csr_bt_cm_l2cap.h"
#include "csr_bt_cm_rfc.h"
#include "csr_bt_cm_dm.h"
#include "csr_bt_cm_bccmd.h"

/***************************************************************************
  CsrBtCmRfcArrivalHandler:
****************************************************************************/
void CsrBtCmRfcArrivalHandler(cmInstanceData_t *cmData)
{
    RFCOMM_UPRIM_T  *rfcPrim;
    rfcPrim = (RFCOMM_UPRIM_T *)cmData->recvMsgP;

    switch(rfcPrim->type)
    {
        case RFC_DATAREAD_IND :
        {
            CsrBtCmRfcDataIndHandler(cmData);
            break;
        }
        case RFC_DATAWRITE_CFM :
        {
            CsrBtCmRfcDataWriteCfmHandler(cmData);
            break;
        }
        case RFC_PORTNEG_IND:
        {
            CsrBtCmRfcPortNegIndHandler(cmData);
            break;
        }
        case RFC_PORTNEG_CFM:
        {
            CsrBtCmRfcPortNegCfmHandler(cmData);
            break;
        }
        case RFC_MODEM_STATUS_CFM:
        {
            CsrBtCmRfcModemStatusCfmHandler(cmData);
            break;
        }
        case RFC_MODEM_STATUS_IND :
        {
            CsrBtCmRfcModemStatusIndHandler(cmData);
            break;
        }
        case RFC_DISCONNECT_CFM:
        {
            CsrBtCmRfcReleaseIndHandler(cmData);
            break;
        }
        case RFC_DISCONNECT_IND :
        {
            CsrBtCmRfcReleaseIndHandler(cmData);
            break;
        }
        case RFC_SERVER_CONNECT_IND:
        {
            CsrBtCmRfcServerConnectIndHandler(cmData);
            break;
        }
        case RFC_SERVER_CONNECT_CFM:
        {
            CsrBtCmRfcServerConnectCfmHandler(cmData);
            break;
        }
        case RFC_CLIENT_CONNECT_CFM:
        {
            CsrBtCmRfcClientConnectCfmHandler(cmData);
            break;
        }
        case RFC_UNREGISTER_CFM:
        {
            break;
        }
        case RFC_REGISTER_CFM :
        {
            CsrBtCmRfcRegisterCfmHandler(cmData);
            break;
        }
        case RFC_INIT_CFM:
        {
            CsrBccmdReadPsValueReqSend(CSR_BT_CM_IFACEQUEUE,
                                       0x0000, /* seqno */
                                       PSKEY_ONCHIP_HCI_CLIENT,
                                       CSR_BCCMD_STORES_DEFAULT,
                                       sizeof(CsrUint16));
            break;
        }
        case RFC_LINESTATUS_IND:
        {
            break;
        }
        case RFC_LINESTATUS_CFM:
        {
            break;
        }
        case RFC_TEST_CFM:
        {
            RFC_TEST_CFM_T *ptr = (RFC_TEST_CFM_T *) cmData->recvMsgP;
            
            CsrMblkDestroy(ptr->test_data);
            ptr->test_data = NULL;
            break;
        }
        case RFC_ERROR_IND:
        {
            break;
        }
        case RFC_NSC_IND:
        {
            break;
        }

#ifdef CSR_AMP_ENABLE
        case RFC_L2CA_MOVE_CHANNEL_CFM:
        {
            CsrBtCmRfcAmpMoveChannelCfmHandler(cmData);
            break;
        }
        case RFC_L2CA_MOVE_CHANNEL_CMP_IND:
        {
            CsrBtCmRfcAmpMoveChannelCmpIndHandler(cmData);
            break;
        }
        case RFC_L2CA_MOVE_CHANNEL_IND:
        {
            CsrBtCmRfcAmpMoveChannelIndHandler(cmData);
            break;
        }
        case RFC_L2CA_AMP_LINK_LOSS_IND:
        {
            CsrBtCmRfcAmpLinkLossIndHandler(cmData);
            break;
        }
#endif

        default:
        {
            CsrBtCmStateEventException(RFCOMM_PRIM, rfcPrim->type, cmData->globalState);
            break;
        }
    }
}
#endif

